# TrivialTestKit
A simple C++ library for testing code. 
Allows to create unit tests and execute them with check if they compleated with or without fail.

## HOWTO: Run tests manualy
To run tests. Select in ***TrivialTestKIt/Test*** project a one of following ***Solution Configuration***:
- ***Release***, for test without definied `WIDE_ORIENTATION` \*;
- ***ReleaseWide***, for test with definied `WIDE_ORIENTATION`;
- ***Debug***, for test without definied `WIDE_ORIENTATION`;
- ***DebugWide***, for test with definied `WIDE_ORIENTATION`.

And then ***Start Without Debugging***.

## Builds and tests results

MSVC (automated):

| Platform | Compiler | Architecture | Wide Oriented Stream \* | Build and Test |
|-|-|-|-|-|
| Windows | Visual Studio 2022, Visual Studio  2019 | x86, x64 | With and Without | [![Build and Test](https://github.com/underwatergrasshopper/TrivialTestKit/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/underwatergrasshopper/TrivialTestKit/actions/workflows/build_and_test.yml) |

 MinGW (by hand on local machine):

| Platform | Compiler | Architecture | Wide Oriented Stream \* | Build | Passing Tests |
|-|-|-|-|-|-|
| Windows 10 | LLVM GCC 13.0.0 | x86 | No | <span style="color:grey">Not builded</span> | <span style="color:grey">Not Tested</span> |
| Windows 10 | LLVM GCC 13.0.0 | x86 | Yes | <span style="color:grey">Not builded</span> | <span style="color:grey">Not Tested</span> |
| Windows 10 | LLVM GCC 13.0.0 | x64 | No | <span style="color:green">Success</span> | <span style="color:green">Yes</span> |
| Windows 10 | LLVM GCC 13.0.0 | x64 | Yes | <span style="color:green">Success</span> | <span style="color:red">No\*\*</span> |

<sup>\* When using functions from wprintf family only (those for wide characters, which manipulate any stream).</sup>

<sup>\*\* Testing ends with stack owerflow error code when `fwprintf` function is used. No output is generated. Same happens when a separate project is made with only `fwprintf` function called. Looks like bug in llvm-gcc.</sup>

## HOWTO: Use library and make unit tests
### Example: Until First Assertion Fail
Runs tests until first assertion fails.

Code:
```C++
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
    TTK_AssertM(2 + 2 == 5, "Surprising result!");
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

int main() {
    return TTK_RunTests({
        TestA,
        TestB,
        TestC,
        TestD,
    });
}
```
Printed result:
```
--- TEST ---
[test] TestA
[test] TestB
    [fail] [line:23] [file:C:\Path\To\Test\main.cpp] [condition:2 + 2 == 5]
--- TEST FAIL ---
number of runned notified tests : 2
number of failed tests          : 1
```
### Example: All
Runs all tests, regardless occurred assertions fails, by calling `TTK_SetIsAbortAtFail(false)`.

Code:
```C++
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
    TTK_AssertM(2 + 2 == 5, "Surprising result!");
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

int main() {
    TTK_SetIsAbortAtFail(false);

    return TTK_RunTests({
        TestA,
        TestB,
        TestC,
        TestD,
    });
}
```
Printed result:
```
--- TEST ---
[test] TestA
[test] TestB
    [fail] [line:23] [file:C:\Path\To\Test\main.cpp] [condition:2 + 2 == 5] [message:Surprising result!]
[test] TestC
    [fail] [line:31] [file:C:\Path\To\Test\main.cpp] [condition:false]
[test] TestD
--- TEST FAIL ---
number of runned notified tests : 4
number of failed tests          : 2
```

### Example: Mixed
Runs test `TestA`, `TestB` and doesn't abort test execution at assertion fail, by using `TTK_DisableAbortAtFail`.

Runs test `TestC`, `TestD` and aborts test execution at first assertion fail, by using `TTK_EnableAbortAtFail`.

Code:
```C++
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
    TTK_AssertM(2 + 2 == 5, "Surprising result!");
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
```
Printed result:
```
--- TEST ---
[test] TestA
[test] TestB
    [fail] [line:23] [file:C:\work\code\c_cpp\TrivialTestKit\Example\src\main.cpp] [condition:2 + 2 == 5] [message:Surprising result!]
[test] TestC
    [fail] [line:31] [file:C:\work\code\c_cpp\TrivialTestKit\Example\src\main.cpp] [condition:false]
--- TEST FAIL ---
number of runned notified tests : 3
number of failed tests          : 2
```