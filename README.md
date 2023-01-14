# TrivialTestKit
A simple C++ library for testing code. 
Allows to create unit tests and execute them with check if they compleated with or without fail.

## HOWTO: Run tests manualy
To run all tests for Visual Studio, call `Test.bat`.             
To run all tests for MinGW)\*, call `MinGW_Test.bat`.     
To run selected tests for MinGW\*, call `MinGW_MakeTest.bat run <build_target> <architecture> (<flag> )*`, where
```
<build_target>
    Release
    Debug
<architecture>
    64
    32
<flag>
    WIDE                        - Tests will use function from wprintf family only.
    IN_PLACE                    - Runs test with adding test function to running 
                                  at place where are declared them.
    NO_TEST_IN_UNICODE_FOLDER   - Skip tests where tested code is placed in folder 
                                  which name contains unicode characters.
```
.
            
<sup>\*) To be able compile with 64 bit and 32 bit gcc, add paths to mingw bin folder in `TrivialTestKit/TrivialTestKit_Test/MinGW_MakeCache.bat`:
```
set MINGW32_BIN_PATH=<path>
set MINGW64_BIN_PATH=<path>
```
.
</sup>

## Builds and tests results

Compiler: **MSVC** (automated)

| Test Platform | Target Platform | Environment | Architecture | Stream Orientation \* | Build and Test |
|-|-|-|-|-|-|
| Windows Server 2022, Windows Server 2019 | Windows 10 |  Visual Studio 2022, Visual Studio  2019 | x86, x86_64 | Narrow and Wide | [![Build and Test](https://github.com/underwatergrasshopper/TrivialTestKit/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/underwatergrasshopper/TrivialTestKit/actions/workflows/build_and_test.yml) |

 Compiler: **MinGW** (automated)

| Test Platform | Target Platform | Environment | Architecture | Stream Orientation \* | Build and Test |
|-|-|-|-|-|-|
| Windows Server 2022 | Windows 10 | MinGW-W64 (LLVM Clang 15.0.0) | x86, x86_64 | Narrow and Wide |  [![Build and Test (MinGW)](https://github.com/underwatergrasshopper/TrivialTestKit/actions/workflows/build_and_test_mingw.yml/badge.svg)](https://github.com/underwatergrasshopper/TrivialTestKit/actions/workflows/build_and_test_mingw.yml) |

<sup>\* Narrow for a stream when first function which operate on the stream is from `printf` family.
Wide for a stream when first function which operate on the stream is from `wprintf` family. See also documentation of `fwide`.</sup>

## HOWTO: Use library and make unit tests (examples)
Following example adds `TestA` and `TestB` to run at the place of definition. Test functions are run in the order they were added. 
Second argument in `TTK_TEST` is execution mode of test function. Value `0` means - test function will be executed. 

```c++
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
```
```
--- TEST ---
[test] TestA
[test] TestB
--- TEST SUCCESS ---
number of executed asserts      : 4
number of failed asserts        : 0
number of executed tests        : 2
number of failed tests          : 0
```

If condition in assertion fails (is false) then current test function and all remaining test functions are aborted. Information about fail is displayed.

```c++
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
```
```
--- TEST ---
[test] TestA
    [fail] [file:main.cpp] [line:50] [condition:2 + 2 == 9000]
--- TEST FAIL ---
number of executed asserts      : 2
number of failed asserts        : 1
number of executed tests        : 1
number of failed tests          : 1
```

To assertion can be added message which will be displayed when condition fails. Message can be c-string literal, c-string variable, std::string variable, with coding in either ASCII or UTF8.

```c++
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
```
```
--- TEST ---
[test] TestA
    [fail] [file:main.cpp] [line:70] [condition:2 + 2 == 9000] [message:How this can be!]
--- TEST FAIL ---
number of executed asserts      : 2
number of failed asserts        : 1
number of executed tests        : 1
number of failed tests          : 1
```

If `TTK_EXPECT` is used instead `TTK_ASSERT`, then execution of current test function and remaining test functions is not aborted when condition fails. 
Information about fail is still displayed.

```c++
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
```
```
--- TEST ---
[test] TestA
    [fail] [file:main.cpp] [line:90] [condition:2 + 2 == 9000] [message:How this can be!]
[test] TestB
--- TEST FAIL ---
number of executed asserts      : 4
number of failed asserts        : 1
number of executed tests        : 2
number of failed tests          : 1

```

Test function will be skipped from execution if `TTK_DISABLE` is used as second argument in `TTK_TEST`.

```c++
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
```
```
--- TEST ---
[test] TestB
--- TEST SUCCESS ---
number of executed asserts      : 2
number of failed asserts        : 0
number of executed tests        : 1
number of failed tests          : 0
```

If `TTK_NO_ABORT` is used as second argument in `TTK_TEST`, then execution of remaining test functions will continue if assertion fail. 
Rest of current test function will be skipped from execution, in that case.

```c++
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
```
```
--- TEST ---
[test] TestA
    [fail] [file:main.cpp] [line:129] [condition:1 + 1 == 9000]
[test] TestB
--- TEST FAIL ---
number of executed asserts      : 3
number of failed asserts        : 1
number of executed tests        : 2
number of failed tests          : 1
```

Both `TTK_NO_ABORT` and `TTK_DISABLE` can be combined. 

```c++
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
```
```
--- TEST ---
[test] TestB
--- TEST SUCCESS ---
number of executed asserts      : 2
number of failed asserts        : 0
number of executed tests        : 1
number of failed tests          : 0
```

Test functions can be added after their definition.

```c++
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
```
```
--- TEST ---
[test] TestA
[test] TestB
--- TEST SUCCESS ---
number of executed asserts      : 4
number of failed asserts        : 0
number of executed tests        : 2
number of failed tests          : 0
```