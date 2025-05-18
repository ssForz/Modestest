#include "../modestest/modestest.h"

class ModuleX : public NModestest::Ttesting {};

class ModuleY : public NModestest::Ttesting {};

bool RetTrue() {
    return true;
}

void ThrowExpFunc() {
    throw std::string("too big");
}

INIT_TEST_LIST(ModuleX);

TEST_START(ModuleX, SMOKE(JustTesting)) {
    int a = 1;
    int b = 2;
    ASSERT_TRUE(a != b);
    EXPECT_TRUE(a != b);
    EXPECT_NEQ(a, b);
    ASSERT_NEQ(a, b);
}
TEST_END();


TEST_START(ModuleX, SMOKE(Other)) {
    EXPECT_EQ(1, 3);
    EXPECT_FALSE(RetTrue());
    EXPECT_NEQ(1, 3);

    ASSERT_FALSE(RetTrue());

    ASSERT_TRUE(0);

}
TEST_END();

TEST_START(ModuleX, MEDIUM(Mid)) {
    EXPECT_EX(ThrowExpFunc());
    EXPECT_EX(RetTrue());
}
TEST_END()

TEST_START(ModuleX, CUSTOM(Daily, Wait5sec)) {
    //SUCCESS();

    EXPECT_THIS_EX(ThrowExpFunc(), std::string("too big"));
    
    WAIT_SEC(8);

    EXPECT_EX(RetTrue());

    EXPECT_THIS_EX(ThrowExpFunc(), std::string("too small"));

    EXPECT_THIS_EX(THROW_EX(), std::string("Modestest exception"));
}
TEST_END();

END_TEST_LIST();



INIT_TEST_LIST(ModuleY);

TEST_START(ModuleY, SMOKE(EasyTest)) {
    ASSERT_TRUE(true);
}
TEST_END();

TEST_START(ModuleY, SMOKE(FailTest)) {
    FAIL();
}
TEST_END();

TEST_START(ModuleY, CUSTOM(daily, DIS_NonTest)) {
    FAIL();
}
TEST_END();

END_TEST_LIST();





int main(int argc, char *argv[]) {

    NModestest::Ttesting::InitTesting(argc, argv);

    NSuite_ModuleX::RegisterTests();
    NSuite_ModuleY::RegisterTests();

    RUN_ALL_TESTS();

    // RUN_TESTS(ModuleY);

    // RUN_TESTS(MyTestSuite2);
}
