////////////////////////////////////////////////////////////////////////////////
// MIT License
// 
// Copyright (c) 2022 underwatergrasshopper
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

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

