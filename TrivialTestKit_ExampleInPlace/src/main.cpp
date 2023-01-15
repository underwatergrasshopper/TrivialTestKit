#if 1
#include <TrivialTestKit.h>

TTK_TEST(TestA, 0) {
    TTK_ASSERT(1 + 1 == 2);
    TTK_ASSERT(2 + 2 == 4);
}

TTK_TEST(TestB, 0) {
    TTK_ASSERT(3 + 3 == 6);
    TTK_ASSERT(4 + 4 == 8);
}

int main() {
    TTK_Run();
    TTK_Clear();
    return 0;
}
#endif

#if 0
#include <TrivialTestKit.h>

TTK_TEST(TestA, 0) {
    TTK_ASSERT(1 + 1 == 2);
    TTK_ASSERT(2 + 2 == 9000); // will fail
}

TTK_TEST(TestB, 0) {
    TTK_ASSERT(3 + 3 == 6);
    TTK_ASSERT(4 + 4 == 8);
}

int main() {
    TTK_Run();
    TTK_Clear();
    return 0;
}
#endif

#if 0
#include <TrivialTestKit.h>

TTK_TEST(TestA, 0) {
    TTK_ASSERT(1 + 1 == 2);
    TTK_ASSERT_M(2 + 2 == 9000, "How this can be!"); // will fail
}

TTK_TEST(TestB, 0) {
    TTK_ASSERT(3 + 3 == 6);
    TTK_ASSERT(4 + 4 == 8);
}

int main() {
    TTK_Run();
    TTK_Clear();
    return 0;
}
#endif

#if 0
#include <TrivialTestKit.h>

TTK_TEST(TestA, 0) {
    TTK_EXPECT(1 + 1 == 2);
    TTK_EXPECT_M(2 + 2 == 9000, "How this can be!"); // will fail, but still continue
}

TTK_TEST(TestB, 0) {
    TTK_ASSERT(3 + 3 == 6);
    TTK_ASSERT(4 + 4 == 8);
}

int main() {
    TTK_Run();
    TTK_Clear();
    return 0;
}
#endif

#if 0
#include <TrivialTestKit.h>

TTK_TEST(TestA, TTK_DISABLE) {
    TTK_ASSERT(1 + 1 == 2);
    TTK_ASSERT(2 + 2 == 4);
}

TTK_TEST(TestB, 0) {
    TTK_ASSERT(3 + 3 == 6);
    TTK_ASSERT(4 + 4 == 8);
}

int main() {
    TTK_Run();
    TTK_Clear();
    return 0;
}
#endif

#if 0
#include <TrivialTestKit.h>

TTK_TEST(TestA, TTK_NO_ABORT) {
    TTK_ASSERT(1 + 1 == 9000); // fails
    TTK_ASSERT(2 + 2 == 4);
}

TTK_TEST(TestB, 0) {
    TTK_ASSERT(3 + 3 == 6);
    TTK_ASSERT(4 + 4 == 8);
}

int main() {
    TTK_Run();
    TTK_Clear();
    return 0;
}
#endif

#if 0
#include <TrivialTestKit.h>

TTK_TEST(TestA, TTK_NO_ABORT | TTK_DISABLE) {
    TTK_ASSERT(1 + 1 == 9000); // fails
    TTK_ASSERT(2 + 2 == 4);
}

TTK_TEST(TestB, 0) {
    TTK_ASSERT(3 + 3 == 6);
    TTK_ASSERT(4 + 4 == 8);
}

int main() {
    TTK_Run();
    TTK_Clear();
    return 0;
}
#endif

#if 0
#include <TrivialTestKit.h>

void TestA() {
    TTK_ASSERT(1 + 1 == 2);
    TTK_ASSERT(2 + 2 == 4);
}

void TestB() {
    TTK_ASSERT(3 + 3 == 6);
    TTK_ASSERT(4 + 4 == 8);
}

int main() {
    TTK_ADD_TEST(TestA, 0);
    TTK_ADD_TEST(TestB, 0);

    TTK_Run();
    TTK_Clear();
    return 0;
}
#endif

