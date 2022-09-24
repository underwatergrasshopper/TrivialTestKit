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

template <typename T>
inline bool IsReal(T&&) {
    return true;
}

void TestA() {
    TTK_NotifyTest();

    TTK_Assert(IsReal(M_PI));
    TTK_Assert(IsReal(0));
    TTK_Assert(IsReal("This text."));
}

void TestB() {
    TTK_NotifyTest();

    TTK_Assert(1 + 1 == 2);
    TTK_Assert(2 + 2 == 5, "Surprising result!");
    TTK_Assert(3 + 3 == 6);
}

void TestC() {
    TTK_NotifyTest();

    TTK_Assert(false);
}

void TestD() {
    TTK_NotifyTest();

    TTK_Assert(true);
}

// Change 0 to 1 if you wanna run chosen example. Only one example can be running.

#if 0
// Stops execution of tests at first assertion fail.
int main() {
    return TTK_RunTests({
        TestA,
        TestB,
        TestC,
        TestD,
    });
}
#endif 

#if 0
// Runs all tests regardless assertions fails.
int main() {
    TTK_SetIsAbortAtFail(false);

    return TTK_RunTests({
        TestA,
        TestB,
        TestC,
        TestD,
    });
}
#endif 

#if 0
int main() {
    return TTK_RunTests({
        // Runs tests regardless assertions fails.
        TTK_DisableAbortAtFail,
        TestA,
        TestB,

        // Stops execution of tests at first assertion fail.
        TTK_EnableAbortAtFail,
        TestC,
        TestD,
    });
}
#endif 