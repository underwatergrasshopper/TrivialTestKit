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
 * @version 1.0.1
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
#include <utility>

// Checks a condition. If the condition fails, then displays information about fail 
// and where in code fail happened, after that returns from the caller function (which is a test function).
// condition            An expression which will be checked if it's equal to 'true'. 
//                      Must resolve to bool type value.
// message              (Optional) An addition message which will be displayed when condition fail. 
//                      Type can by either an c-string or std::string. Encoding can be either ASCII or UTF8.
// return_expression    (Optional) An expression which will be returned 
//                      by caller function (which is test function) when condition fails.
#define TTK_Assert(condition)                               if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), nullptr);  return; } (void)0
#define TTK_AssertM(condition, message)                     if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), message);  return; } (void)0
#define TTK_AssertR(condition, return_expression)           if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), nullptr);  return (return_expression); } (void)0
#define TTK_AssertMR(condition, message, return_expression) if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), message);  return (return_expression); } (void)0

// Notifies when it starts run. Place as first line in test function.
#define TTK_NotifyTest()                                    TTK_InnerNotifyTest(__func__)

// Test function pointer type.
using TTK_TestFnP_T = void (*)();

// is       true    - (default) TTK_RunTests function will abort execution of following tests when assertion fails occur in current test;
//          false   - TTK_RunTests function will continue executing following tests when assertion fail occur in current test.
void TTK_SetIsAbortAtFail(bool is);

// TTK_RunTests function will abort execution of following tests when assertion fails occur in current test.
void TTK_EnableAbortAtFail();

// TTK_RunTests function will continue executing following tests when assertion fail occur in current test.
void TTK_DisableAbortAtFail();

// Runs provided tests and displays result.
// return   true - if all tests finished without failing any assertion; 
//          false - otherwise.
template <uint64_t NUMBER> 
bool TTK_RunTests(const TTK_TestFnP_T (&tests)[NUMBER]);

#ifdef TTK_SHORT_NAMES
#define Assert              TTK_Assert
#define AssertM             TTK_AssertM
#define AssertR             TTK_AssertR
#define AssertMR            TTK_AssertMR

#define SetIsAbortAtFail    TTK_SetIsAbortAtFail
#define EnableAbortAtFail   TTK_EnableAbortAtFail
#define DisableAbortAtFail  TTK_DisableAbortAtFail

#define NotifyTest          TTK_NotifyTest
#define RunTests            TTK_RunTests
#endif // TTK_SHORT_NAMES

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

struct TTK_Data {
    FILE*       output;
    uint64_t    number_of_notified_tests;
    uint64_t    number_of_failed_tests;

    int         forced_orientation;
    bool        is_abort_testing_at_assertion_fail;
};

inline TTK_Data TTK_MakeDefaultData() {
    TTK_Data data = {};

    data.output                             = stdout;
    data.is_abort_testing_at_assertion_fail = true;

    return data;
}

inline TTK_Data& TTK_ToData() {
    static TTK_Data s_data = TTK_MakeDefaultData();
    return s_data;
}

//------------------------------------------------------------------------------

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

    data.number_of_failed_tests += 1;

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

inline void TTK_InnerNotifyTest(const std::string& caller_name) {
    TTK_Data& data = TTK_ToData();

    if (TTK_SolveOutputOrientation() > 0) {
        fwprintf(data.output, L"[test] %hs\n", caller_name.c_str());
    } else {
        fprintf(data.output, "[test] %s\n", caller_name.c_str());
    }
    fflush(data.output);

    ++data.number_of_notified_tests;
}

//------------------------------------------------------------------------------

inline void TTK_SetIsAbortAtFail(bool is) {
    TTK_ToData().is_abort_testing_at_assertion_fail = is;
}

inline void TTK_EnableAbortAtFail() {
    TTK_SetIsAbortAtFail(true);
}

inline void TTK_DisableAbortAtFail() {
    TTK_SetIsAbortAtFail(false);
}

template <uint64_t NUMBER> 
bool TTK_RunTests(const TTK_TestFnP_T (&tests)[NUMBER]) {
    TTK_Data& data = TTK_ToData();

    data.number_of_notified_tests   = 0;
    data.number_of_failed_tests     = 0;

    if (TTK_SolveOutputOrientation() > 0) {
        fwprintf(data.output, L"%hs", "--- TEST ---\n");
    } else {
        fprintf(data.output, "%s", "--- TEST ---\n");
    }

    uint64_t previous_number_of_failed_tests = 0;

    for (const auto& test : tests) {
        test();

        if (data.is_abort_testing_at_assertion_fail && data.number_of_failed_tests != previous_number_of_failed_tests) break;

        previous_number_of_failed_tests = data.number_of_failed_tests;
    }

    const bool is_success = data.number_of_failed_tests == 0;

    if (TTK_SolveOutputOrientation() > 0) {
        fwprintf(data.output, L"%hs", (is_success ? "--- TEST SUCCESS ---\n" : "--- TEST FAIL ---\n"));
        fwprintf(data.output, L"number of runned notified tests : %lld\n", data.number_of_notified_tests);
        fwprintf(data.output, L"number of failed tests          : %lld\n", data.number_of_failed_tests);
    } else {
        fprintf(data.output, "%s", (is_success ? "--- TEST SUCCESS ---\n" : "--- TEST FAIL ---\n"));
        fprintf(data.output, "number of runned notified tests : %lld\n", data.number_of_notified_tests);
        fprintf(data.output, "number of failed tests          : %lld\n", data.number_of_failed_tests);
    }
    return is_success;
}

//------------------------------------------------------------------------------

inline void TTK_SetOutput(FILE* output) {
    TTK_ToData().output = output;
}

#endif // TRIVIALTESTKIT_H_

