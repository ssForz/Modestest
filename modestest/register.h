#ifndef _MODESTEST_REGISTER_
#define _MODESTEST_REGISTER_

#define INIT_TEST_LIST(TestSuite)                  \
namespace NSuite_##TestSuite                       \
{                                                  \
void RegisterTests()                               \
{                                                  \
TestSuite::GetInstance()->AddSuiteName(#TestSuite) 


#define TEST_START(TestSuite, name)      \
{                                        \
auto saveSuiteName = #TestSuite;         \
auto saveName = name;                    \
auto inst = TestSuite::GetInstance();    \
auto test = [](bool *TEST_STAT)

#define TEST_END()                              \
;                                               \
inst->AddTest(saveSuiteName, saveName, test);   \
}

#define END_TEST_LIST() \
}                       \
}

#define RUN_TESTS(TestSuite)        \
TestSuite::GetInstance()->RunTests(#TestSuite);

#define RUN_ALL_TESTS()             \
NModestest::Ttesting::GetInstance()->RunAllTests()

#endif