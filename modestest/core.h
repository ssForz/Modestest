#ifndef _MODESTEST_CORE_
#define _MODESTEST_CORE_

#include <iostream>
#include <exception>
#include <chrono>
#include <vector>
#include <map>
#include <functional>

namespace NModestest
{
    class Ttesting
    {

    private:
        void StartTestSuite(const std::string& suiteName) {
            CountTests(suiteName);

            if (Status.Total == 0) {
                return;
            }

            std::string num = Status.Total == 1 ? "test" : "tests";

            std::cout << "====[RUNNING TEST SUITE]====\n";
            std::cout << "TEST SUITE: " << suiteName <<"\n";
            std::cout << "RUNNING " << Status.Total << " " << num << "\n";

            Status.startSuite = std::chrono::steady_clock::now();

        };

        void StartTest(const std::string& suiteName, const std::string& testName) {
            std::cout << "[START    ] " << suiteName << "." << testName << "\n\n";
            Status.startTest = std::chrono::steady_clock::now();
        };

        void FinishTestSuite(const std::string& suiteName, const bool status) {
            Status.finishSuite = std::chrono::steady_clock::now();
            std::string st = status ? "[ PASSED SUITE ]" : "[ FAILED SUITE ]";
            
            std::chrono::duration<double> elapsed = Status.finishSuite - Status.startSuite;

            std::cout << st << " " << suiteName << " (" 
            << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() <<" ms)\n";

            if (!status) {
                std::cout << ">>>> Passed tests: " << Status.Passed << "\n";
                std::cout << "<<<< Failed tests: " << Status.Total - Status.Passed << "\n";
            }

            if (Status.Disabled != 0) {
                std::cout << "++++ Disabled tests: " << Status.Disabled << "\n";
            }

            std::cout << "\n";
        };

        void FinishTest(const std::string& suiteName, const std::string& testName, const bool status) {
            std::string st = status ? "[      OK ]" : "[     FAIL]";
            if (status) {
                
            }
            Status.finishTest = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = Status.finishTest - Status.startTest;

            std::cout << st << " " << suiteName << "." << testName 
            << " (" << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() <<" ms)\n\n";
        };

        void GlobalTestingStart() {
            std::cout << "GLOBAL NOTE: Running with filter: \"" << NameFilter << "\"\n\n";
            std::cout << "====STARTING GLOBAL TESTING====\n\n";

            GlobalInfo = TGlobalInfo();
            GlobalInfo.start = std::chrono::steady_clock::now();

        }

        void GlobalTestingFinish() {
            GlobalInfo.finish = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = GlobalInfo.finish - GlobalInfo.start;
            std::cout << "====FINISH GLOBAL TESTING (total time " << 
            std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << " ms)====\n";

            std::cout << "TOTAL Passed: " << GlobalInfo.Passed << "\n";
            std::cout << "TOTAL Failed: " << GlobalInfo.Total - GlobalInfo.Passed << "\n";
            std::cout << "TOTAL Disabled: " << GlobalInfo.Disabled << "\n";

        }

        void AddInfoToGlobal() {
            GlobalInfo.Total += Status.Total;
            GlobalInfo.Passed += Status.Passed;
            GlobalInfo.Disabled += Status.Disabled;
        }
        
        static Ttesting* testingInstance;

        Ttesting() {};

        void SetArgs(int argc, char *argv[]) {
            for (int i = 0; i < argc; ++i) {
                auto& arg = std::string(argv[i]);
                
                if (arg.find("-filter") != std::string::npos) {

                    if (arg[7] != '=' || arg[8] != '*' || arg[arg.size() - 1] != '*') {
                        PrintArgErrorMessage("filter");
                        continue;
                    }

                    NameFilter = arg.substr(9, arg.size() - 10);
                }
            }
        };

        void CountTests(const std::string& suiteName) {
            Status = TCurrentSuiteInfo();

            for (const auto& nameTest : TestSuites[suiteName]) {

                if (NameFilter.size() != 0 && nameTest.Name.find(NameFilter) == std::string::npos) {
                    continue;
                }

                if (nameTest.Name.substr(0, 3) == "DIS") {
                    Status.Disabled++;
                    continue;
                }

                Status.Total++;
            }
        }

        void PrintArgErrorMessage(const std::string& problem) {
            std::cout<<"Error in "<< problem <<" argument. Running with default setting...\n\n";
        }
    
    public:
        Ttesting(const Ttesting& obj) = delete;

        static Ttesting* GetInstance() {
            if (testingInstance == nullptr) {
                testingInstance = new Ttesting();
            }
            return testingInstance;
        }

        static Ttesting* InitTesting(int argc, char *argv[]) {
            if (testingInstance == nullptr) {
                testingInstance = new Ttesting();
            }

            testingInstance->SetArgs(argc, argv);
            return testingInstance;
        }

        void AddSuiteName(const std::string& suiteName) {
            TestSuites[suiteName] = {};
        }

        void AddTest(const std::string& suiteName, const std::string& name, std::function<void(bool*)> test) {
            TestSuites[suiteName].emplace_back(TTest{ name, test });
        }

        void RunAllTests() {

            GlobalTestingStart();

            for (auto&& nameTests : TestSuites) {

                StartTestSuite(nameTests.first);
                
                if (Status.Total == 0) {
                    continue;
                }

                bool statSuite = true;

                for (const auto& nameTest : nameTests.second) {

                    if (NameFilter.size() != 0 && nameTest.Name.find(NameFilter) == std::string::npos
                        || (nameTest.Name.substr(0, 3) == "DIS")) {
                        continue;
                    }

                    StartTest(nameTests.first, nameTest.Name);

                    bool stat = true;
                    
                    nameTest.Test(&stat);
    
                    if (stat) {
                        Status.Passed++;
                    } else {
                        statSuite = stat;
                    }

                    FinishTest(nameTests.first, nameTest.Name, stat);

                }

                FinishTestSuite(nameTests.first, statSuite);

                AddInfoToGlobal();
            }

            GlobalTestingFinish();
        }

        void RunTests(const std::string& nameSuite) {

            StartTestSuite(nameSuite);
                
            if (Status.Total == 0) {
                return;
            }

            bool statSuite = true;
            for (const auto& nameTest : TestSuites[nameSuite]) {
                if (NameFilter.size() != 0 && nameTest.Name.find(NameFilter) != std::string::npos
                    || (nameTest.Name.substr(0, 3) == "DIS")) {
                    continue;
                }

                StartTest(nameSuite, nameTest.Name);

                bool stat = true;
                nameTest.Test(&stat);

                if (stat) {
                    Status.Passed++;
                } else {
                    statSuite = stat;
                }

                FinishTest(nameSuite, nameTest.Name, stat);
            }

            FinishTestSuite(nameSuite, statSuite);

        }

    private:

        struct TGlobalInfo {
            int Total = 0;
            int Passed = 0;
            int Disabled = 0;

            std::chrono::steady_clock::time_point start;
            std::chrono::steady_clock::time_point finish;
        };

        struct TCurrentSuiteInfo {
            int Total = 0;
            int Passed = 0;
            int Disabled = 0;

            std::chrono::steady_clock::time_point startSuite;
            std::chrono::steady_clock::time_point finishSuite;

            std::chrono::steady_clock::time_point startTest;
            std::chrono::steady_clock::time_point finishTest;
        };

        TGlobalInfo GlobalInfo;

        std::string NameFilter = "";

        TCurrentSuiteInfo Status;
        
        struct TTest {
            std::string Name;
            std::function<void(bool*)> Test;
        };

        std::map<std::string, std::vector<TTest>> TestSuites;
    };

    Ttesting* Ttesting::testingInstance = nullptr;
}

#endif


