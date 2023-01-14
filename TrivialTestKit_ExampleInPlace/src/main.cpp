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

TTK_TEST(TestSuccess, 0) {
    TTK_ASSERT(IsReal(M_PI));
    TTK_ASSERT(IsReal(0));
    TTK_ASSERT(IsReal("This text."));
}

TTK_TEST(TestFailButContinue, 0) {
    TTK_EXPECT(1 + 1 == 2);
    TTK_EXPECT_M(2 + 2 == 5, "Surprising result!");
    TTK_EXPECT(3 + 3 == 6);
    TTK_EXPECT_M(2 + 123 == 5, "Another surprising result!");
}

TTK_TEST(TestFail, 0) {
    TTK_ASSERT(1 + 1 == 2);
    TTK_ASSERT_M(2 + 2 == 5, "Surprising result!");
    TTK_ASSERT(3 + 3 == 6);
}

int main() {
    TTK_Run();
    TTK_Clear();

    return 0;
}