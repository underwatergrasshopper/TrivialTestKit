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
/**
 * @file TrivialTestKit.h
 * @author underwatergrasshopper
 * @version 2.0.0
 * @details This file is a part of project: TrivialTestKit. Can be distributed separately.
 */

// Targeted language version: C++11.

#ifndef TRIVIALTESTKIT_H_
#define TRIVIALTESTKIT_H_

#include <stdio.h>
#include <stdint.h>
#include <locale.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <string>
#include <vector>
#include <utility>

// Checks a condition. If the condition failed (is false), then information about fail is displayed.
// Further execution of current test function is aborted. Further execution of remaining test functions is aborted.
// condition            An expression which will be checked if it's equal to 'true'. 
//                      Must resolve to bool type value.
// message              (Optional) An addition message which will be displayed when condition fail. 
//                      Type can by either an c-string or std::string. Encoding can be either ASCII or UTF8.
#define TTK_ASSERT(condition)               if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), nullptr);  TTK_ToData().is_request_abort = true; return; } TTK_ToData().number_of_executed_asserts += 1; (void)0
#define TTK_ASSERT_M(condition, message)    if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), message);  TTK_ToData().is_request_abort = true; return; } TTK_ToData().number_of_executed_asserts += 1; (void)0

// Checks the condition. If the condition failed (is false), then information about fail is displayed. Further execution of tests is continued (no test abort).
#define TTK_EXPECT(condition)               if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), nullptr); } TTK_ToData().number_of_executed_asserts += 1; (void)0
#define TTK_EXPECT_M(condition, message)    if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), message); } TTK_ToData().number_of_executed_asserts += 1; (void)0


// Test function pointer type.
using TTK_TestFnP_T = void (*)();

// Runs all tests.
// return   true - if all tests finished without failing any assertion; 
//          false - otherwise.
bool TTK_Run();

// Sets output where all generated communicates by this library will be sent. Can be stdout, stderr or opened file.
void TTK_SetOutput(FILE* output);

// Sets output stream orientation for character type. Same parameter rules as in fwide function.
// orientation      > 0     - wide      (wchar_t)
//                  0       - current 
//                  < 0     - narrow    (char)
void TTK_ForceOutputOrientation(int orientation);

//==============================================================================
// Inner
//==============================================================================

// Makes regular character-array-literal a wide character-array-literal.
#define TTK_INNER_L(text) L##text
#define TTK_L(text) TTK_INNER_L(text)

// Makes regular character-array-literal an utf-8 character-array-literal.
#define TTK_INNER_U8(text) reinterpret_cast<const char*>(u8##text)
#define TTK_U8(text) TTK_INNER_U8(text)

//------------------------------------------------------------------------------

enum : uint64_t {
    TTK_DEFAULT     =   0x0000,
    TTK_DISABLE     =   0x0001,     // disable test
    TTK_NO_ABORT    =   0x0002,     // no abort or remaining test functions at assertion fail, still aborts current test function
};

struct TTK_TestData{
    TTK_TestFnP_T   function;
    const char*     name;
    uint64_t        mode;      // bitfield
};

struct TTK_Data {
    FILE*           output;

    uint64_t        number_of_executed_asserts;
    uint64_t        number_of_failed_asserts;

    uint64_t        number_of_executed_tests;
    uint64_t        number_of_failed_tests;

    int             forced_orientation;

    bool            is_request_abort;

    std::vector<TTK_TestData> tests;
};

inline TTK_Data TTK_MakeDefaultData() {
    TTK_Data data = {};

    data.output                         = stdout;

    data.number_of_executed_asserts     = 0;
    data.number_of_failed_asserts       = 0;

    data.number_of_executed_tests       = 0;
    data.number_of_failed_tests         = 0;

    data.forced_orientation             = false;

    data.is_request_abort               = false;

    return data;
}

inline TTK_Data& TTK_ToData() {
    static TTK_Data s_data = TTK_MakeDefaultData();
    return s_data;
}

//------------------------------------------------------------------------------

inline bool TTK_AddTest(const TTK_TestData& test_data) {
    auto& data = TTK_ToData();
    
    data.tests.push_back(test_data);
    return true;
}

inline void TTK_Clear() {
    auto& data = TTK_ToData();

    data.tests.clear();
}

// mode     Bitfield made from any combination of flags: 0, TTK_DEFAULT, TTK_DISABLE, TTK_NO_ABORT.
#define TTK_ADD_TEST(TestFunction, mode) TTK_AddTest({TestFunction, #TestFunction, mode})

// mode     Bitfield made from any combination of flags: 0, TTK_DEFAULT, TTK_DISABLE, TTK_NO_ABORT.
#define TTK_TEST(TestFunction, mode) \
    void TestFunction(); \
    static bool s_is_added_##TestFunction = TTK_ADD_TEST(TestFunction, mode); \
    void TestFunction()



class TTK_LocaleGuardianUTF8 {
public:
    TTK_LocaleGuardianUTF8() {
        const char* locale = setlocale(LC_ALL, nullptr);
        if (locale) m_prev_locale_backup = locale;

        setlocale(LC_ALL, ".utf8");
    }

    virtual ~TTK_LocaleGuardianUTF8() {
        setlocale(LC_ALL, m_prev_locale_backup.c_str());
    }

private:
    std::string m_prev_locale_backup;
};

// Enables unicode codepage for functions from printf family in a code scope, where it's placed. 
// Restores previous codepage when exits the code scope.
#define TTK_GuardLocaleUTF8() TTK_LocaleGuardianUTF8 locale_guardian_utf8

//------------------------------------------------------------------------------

inline void TTK_ForceOutputOrientation(int orientation) {
    TTK_ToData().forced_orientation = orientation;
}

inline int TTK_SolveOutputOrientation() {
    TTK_Data& data = TTK_ToData();
    if (data.forced_orientation != 0) return data.forced_orientation;
    return fwide(data.output, 0);
}

//------------------------------------------------------------------------------
#if defined(_MSC_VER)      
#define TTK_TRY_FORCE_NON_INLINE __declspec(noinline)
#elif defined(__GNUC__) 
#define TTK_TRY_FORCE_NON_INLINE __attribute__((noinline))
#else
#define TTK_TRY_FORCE_NON_INLINE
#endif

TTK_TRY_FORCE_NON_INLINE 
inline void TKK_CommunicateAssertFail(unsigned line, const char* condition, const wchar_t* file_name_utf16, const char* file_name_utf8, const char* message) {
    TTK_GuardLocaleUTF8();

    TTK_Data& data = TTK_ToData();

    data.number_of_failed_asserts += 1;

    if (data.output) {
        if (TTK_SolveOutputOrientation() > 0) {
            fwprintf(data.output, L"    [fail] [file:%s] [line:%d] [condition:%hs]", file_name_utf16, line, condition);
            if (message) {
                fwprintf(data.output, L" [message:%hs]\n", message);
            } else {
                fwprintf(data.output, L"\n");
            }
        } else {
            fprintf(data.output, "    [fail] [file:%s] [line:%d] [condition:%s]", file_name_utf8, line, condition);
            if (message) {
                fprintf(data.output, " [message:%s]\n", message);
            } else {
                fprintf(data.output, "\n");
            }
        }
        fflush(data.output);
    }
}

inline void TKK_CommunicateAssertFail(unsigned line, const char* condition, const wchar_t* file_name_utf16, const char* file_name_utf8, const std::string& message) {
    TKK_CommunicateAssertFail(line, condition, file_name_utf16, file_name_utf8, message.c_str());
}

//------------------------------------------------------------------------------

inline bool TTK_Run() {
    TTK_Data& data = TTK_ToData();

    data.number_of_executed_asserts = 0;
    data.number_of_failed_asserts   = 0;

    data.number_of_executed_tests   = 0;
    data.number_of_failed_tests     = 0;

    data.is_request_abort           = false;

    if (TTK_SolveOutputOrientation() > 0) {
        fwprintf(data.output, L"%hs", "--- TEST ---\n");
    } else {
        fprintf(data.output, "%s", "--- TEST ---\n");
    }

    uint64_t previous_number_of_failed_asserts = 0;

    for (const auto& test_data : data.tests) {
        if (!(test_data.mode & TTK_DISABLE)) {
            if (TTK_SolveOutputOrientation() > 0) {
                fwprintf(data.output, L"[test] %hs\n", test_data.name);
            } else {
                fprintf(data.output, "[test] %s\n", test_data.name);
            }
            fflush(data.output);
   
            test_data.function();
   
            data.number_of_executed_tests += 1;
            if (data.number_of_failed_asserts != previous_number_of_failed_asserts) { 
                previous_number_of_failed_asserts = data.number_of_failed_asserts;
                data.number_of_failed_tests += 1;
                
                if (data.is_request_abort) {
                    data.is_request_abort = false;
                    if (!(test_data.mode & TTK_NO_ABORT)) break; // abort all tests
                }
            }
        }
    }

    const bool is_success = data.number_of_failed_tests == 0;

    if (TTK_SolveOutputOrientation() > 0) {
        fwprintf(data.output, L"%hs", (is_success ? "--- TEST SUCCESS ---\n" : "--- TEST FAIL ---\n"));
        fwprintf(data.output, L"number of executed asserts      : %lld\n", data.number_of_executed_asserts);
        fwprintf(data.output, L"number of failed asserts        : %lld\n", data.number_of_failed_asserts);
        fwprintf(data.output, L"number of executed tests        : %lld\n", data.number_of_executed_tests);
        fwprintf(data.output, L"number of failed tests          : %lld\n", data.number_of_failed_tests);
    } else {
        fprintf(data.output, "%s", (is_success ? "--- TEST SUCCESS ---\n" : "--- TEST FAIL ---\n"));
        fprintf(data.output, "number of executed asserts      : %lld\n", data.number_of_executed_asserts);
        fprintf(data.output, "number of failed asserts        : %lld\n", data.number_of_failed_asserts);
        fprintf(data.output, "number of executed tests        : %lld\n", data.number_of_executed_tests);
        fprintf(data.output, "number of failed tests          : %lld\n", data.number_of_failed_tests);
    }
    return is_success;
}

//------------------------------------------------------------------------------

inline void TTK_SetOutput(FILE* output) {
    TTK_ToData().output = output;
}

#endif // TRIVIALTESTKIT_H_

