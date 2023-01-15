#include "TrivialTestKit.h"

#define _USE_MATH_DEFINES 
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template <typename T>
inline bool IsReal(T&&) {
    return true;
}

void TestA() {
    TTK_ASSERT(IsReal(M_PI));
    TTK_ASSERT(IsReal(0));
    TTK_ASSERT(IsReal("This text."));
}

void TestB() {
    TTK_ASSERT(1 + 1 == 2);
    TTK_ASSERT_M(2 + 2 == 5, "Surprising result!");
    TTK_ASSERT(3 + 3 == 6);
}

void TestC() {
    TTK_ASSERT(false);
}

void TestD() {
    TTK_ASSERT(true);
}

void TestE() {
    TTK_EXPECT(1 + 1 == 2);
    TTK_EXPECT_M(2 + 2 == 5, "Surprising result!");
    TTK_EXPECT(3 + 3 == 6);
    TTK_EXPECT_M(2 + 123 == 5, "Another surprising result!");
}

// Change 0 to 1 if you wanna run chosen example. Only one example can be running.

int main() {
#if 1
    TTK_ADD_TEST(TestA, 0);
    TTK_ADD_TEST(TestB, 0);
    TTK_Run();
    TTK_Clear();
#endif 

#if 0
    // Stops execution of tests at first assertion fail.
    TTK_ADD_TEST(TestA, 0);
    TTK_ADD_TEST(TestB, 0);
    TTK_ADD_TEST(TestC, 0);
    TTK_ADD_TEST(TestD, 0);
    TTK_ADD_TEST(TestE, 0);
    TTK_Run();
    TTK_Clear();
#endif 

#if 0
    // Runs all tests regardless assertions fails.
    TTK_ADD_TEST(TestA, TTK_NO_ABORT);
    TTK_ADD_TEST(TestB, TTK_NO_ABORT);
    TTK_ADD_TEST(TestC, TTK_NO_ABORT);
    TTK_ADD_TEST(TestD, TTK_NO_ABORT);
    TTK_ADD_TEST(TestE, TTK_NO_ABORT);
    TTK_Run();
    TTK_Clear();
#endif 

#if 0
    // Runs tests regardless assertions fails.
    TTK_ADD_TEST(TestA, 0);
    TTK_ADD_TEST(TestB, TTK_NO_ABORT);
    TTK_ADD_TEST(TestC, TTK_NO_ABORT);
    TTK_ADD_TEST(TestD, 0);
    TTK_ADD_TEST(TestE, TTK_NO_ABORT);
    TTK_Run();
    TTK_Clear();
#endif 

#if 0
    // Runs tests regardless assertions fails.
    TTK_ADD_TEST(TestE, 0);
    TTK_ADD_TEST(TestD, 0);

    TTK_Run();
    TTK_Clear();
#endif 

    return 0;
}