#ifndef _CHECKSMODESTE_
#define _CHECKSMODESTE_

#include <stdio.h>
#include <thread>

#define PRINT_FAIL_MESSAGE_EXPECT_TRUE(expr, status)   \
printf("Fail at line: %d\n", __LINE__);                \
printf("Value of %s\n", #expr);                        \
printf("Expected: %s\n", status ? "false" : "true");   \
printf("Actual: %s\n\n", status ? "true" : "false")

#define PRINT_FAIL_MESSAGE_ASSERT_TRUE(expr, status)   \
printf("Fail at line: %d\n", __LINE__);                \
printf("Value of %s\n", #expr);                        \
printf("Asserted: %s\n", status ? "false" : "true");   \
printf("Actual: %s\n\n", status ? "true" : "false")

#define PRINT_FAIL_MESSAGE_EXPECT_EQ(lexpr, rexpr)     \
printf("Fail at line: %d\n", __LINE__);                \
printf("Expected equal: %s and %s\n", #lexpr, #rexpr); \
printf("Actual: not equal\n\n")

#define PRINT_FAIL_MESSAGE_ASSERT_EQ(lexpr, rexpr)     \
printf("Fail at line: %d\n", __LINE__);                \
printf("Asserted equal: %s and %s\n", #lexpr, #rexpr); \
printf("Actual: not equal\n\n")

#define PRINT_FAIL_MESSAGE_EXPECT_NEQ(lexpr, rexpr)        \
printf("Fail at line: %d\n", __LINE__);                    \
printf("Expected not equal: %s and %s\n", #lexpr, #rexpr); \
printf("Actual: equal\n\n")

#define PRINT_FAIL_MESSAGE_ASSERT_NEQ(lexpr, rexpr)        \
printf("Fail at line: %d\n", __LINE__);                    \
printf("Asserted not equal: %s and %s\n", #lexpr, #rexpr); \
printf("Actual: equal\n\n")

#define PRINT_FAIL_MESSAGE_ASSERT_EX(expr)        \
printf("Fail at line: %d\n", __LINE__);           \
printf("Asserted: %s throws exception\n", #expr); \
printf("Actual: it doesn't\n\n")

#define PRINT_FAIL_MESSAGE_EXPECT_EX(expr)        \
printf("Fail at line: %d\n", __LINE__);           \
printf("Expected: %s throws exception\n", #expr); \
printf("Actual: it doesn't\n\n")

#define PRINT_FAIL_MESSAGE_ASSERT_THIS_EX(expr, exc, ex)       \
printf("Fail at line: %d\n", __LINE__);                        \
printf("Asserted: %s throws exception - %s\n", #expr, exc.c_str());   \
printf("Actual: it throws exception - %s\n\n", ex.c_str())

#define PRINT_FAIL_MESSAGE_EXPECT_THIS_EX(expr, exc, ex)       \
printf("Fail at line: %d\n", __LINE__);                        \
printf("Expected: %s throws exception - %s\n", #expr, exc.c_str());   \
printf("Actual: it throws exception - %s\n\n", ex.c_str())

#define FAIL()       \
*TEST_STAT = false;  \
return               

#define SUCCESS()    \
*TEST_STAT = true;   \
return

#define ASSERT_TRUE(expr)                        \
if (!(expr)) {                                   \
    PRINT_FAIL_MESSAGE_ASSERT_TRUE(expr, false); \
    FAIL();                                      \
}

#define ASSERT_FALSE(expr)                       \
if (expr) {                                      \
    PRINT_FAIL_MESSAGE_ASSERT_TRUE(expr, true);  \
    FAIL();                                      \
}

#define EXPECT_TRUE(expr)                        \
if (!(expr)) {                                   \
    *TEST_STAT = false;                          \
    PRINT_FAIL_MESSAGE_EXPECT_TRUE(expr, false); \
}

#define EXPECT_FALSE(expr)                       \
if (expr) {                                      \
    *TEST_STAT = false;                          \
    PRINT_FAIL_MESSAGE_EXPECT_TRUE(expr, true);  \
}

#define ASSERT_EQ(lexpr, rexpr)                  \
if ((lexpr) != (rexpr)) {                        \
    PRINT_FAIL_MESSAGE_ASSERT_EQ(lexpr, rexpr);  \
    FAIL();                                      \
}

#define EXPECT_EQ(lexpr, rexpr)                  \
if ((lexpr) != (rexpr)) {                        \
    *TEST_STAT = false;                          \
    PRINT_FAIL_MESSAGE_EXPECT_EQ(lexpr, rexpr);  \
}

#define ASSERT_NEQ(lexpr, rexpr)                  \
if ((lexpr) == (rexpr)) {                         \
    PRINT_FAIL_MESSAGE_ASSERT_NEQ(lexpr, rexpr);  \
    FAIL();                                       \
}

#define EXPECT_NEQ(lexpr, rexpr)                  \
if ((lexpr) == (rexpr)) {                         \
    *TEST_STAT = false;                           \
    PRINT_FAIL_MESSAGE_EXPECT_NEQ(lexpr, rexpr);  \
}

#define ASSERT_EX(expr)                           \
try {                                             \
    (expr);                                         \
    PRINT_FAIL_MESSAGE_ASSERT_EX(expr);           \
    FAIL();                                       \
} catch (...){}

#define EXPECT_EX(expr)                           \
try {                                             \
    (expr);                                       \
    PRINT_FAIL_MESSAGE_EXPECT_EX(expr);           \
    *TEST_STAT = false;                           \
} catch (...){}

#define ASSERT_THIS_EX(expr, exc)                         \
try {                                                     \
    (expr);                                                 \
    PRINT_FAIL_MESSAGE_ASSERT_EX(expr);                   \
    FAIL();                                               \
} catch (const std::string& ex){                          \
    if (ex != exc) {                                      \
        PRINT_FAIL_MESSAGE_ASSERT_THIS_EX(expr, exc, ex); \
        FAIL();                                           \
    }                                                     \
}

#define EXPECT_THIS_EX(expr, exc)                         \
try {                                                     \
    (expr);                                               \
    PRINT_FAIL_MESSAGE_EXPECT_EX(expr);                   \
    *TEST_STAT = false;                                   \
} catch (const std::string& ex){                          \
    if (ex != exc) {                                      \
        PRINT_FAIL_MESSAGE_EXPECT_THIS_EX(expr, exc, ex); \
        *TEST_STAT = false;                               \
    }                                                     \
}

#define WAIT_MS(time)                                        \
std::this_thread::sleep_for(std::chrono::milliseconds(time)) 

#define WAIT_SEC(time)                                   \
std::this_thread::sleep_for(std::chrono::seconds(time)) 

#define WAIT_MIN(time)                                   \
std::this_thread::sleep_for(std::chrono::minutes(time)) 

#define THROW_EX()                        \
throw std::string("Modestest exception")

#endif