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

// Targeted minimal language version: C++11.

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

// Checks the condition. If the condition failed (is false), then information about fail is displayed.
// Further execution of current test function is aborted. Further execution of remaining test functions is aborted.
// condition            An expression which will be checked if it's equal to 'true'. 
//                      Must resolve to bool type value.
// message              (Optional) An addition message, to be displayed when condition fails. 
//                      Type can by either an c-string or std::string. Encoding can be either ASCII or UTF8.
#define TTK_ASSERT(condition)               { TTK_ToSuite().Count(); if (!(condition)) { TTK_ToSuite().CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), nullptr); TTK_ToSuite().RequestAbort(); return; } } (void)0
#define TTK_ASSERT_M(condition, message)    { TTK_ToSuite().Count(); if (!(condition)) { TTK_ToSuite().CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), message); TTK_ToSuite().RequestAbort(); return; } } (void)0

// Checks the condition. If the condition failed (is false), then information about fail is displayed. 
// Further execution of current test and remaining tests is continued (no test abort).
// condition            An expression which will be checked if it's equal to 'true'. 
//                      Must resolve to bool type value.
// message              (Optional) An addition message, to be displayed when condition fails. 
//                      Type can by either an c-string or std::string. Encoding can be either ASCII or UTF8.
#define TTK_EXPECT(condition)               { TTK_ToSuite().Count(); if (!(condition)) { TTK_ToSuite().CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), nullptr); } } (void)0
#define TTK_EXPECT_M(condition, message)    { TTK_ToSuite().Count(); if (!(condition)) { TTK_ToSuite().CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), TTK_U8(__FILE__), message); } } (void)0

enum : uint64_t {
    TTK_DEFAULT     =   0x0000,
    TTK_DISABLE     =   0x0001,     // disable test
    TTK_NO_ABORT    =   0x0002,     // no abort or remaining test functions at assertion fail, still aborts current test function
};

// Adds test functions to be executed.
// TestFunction         Existing test function of type: void (*)().
// mode                 Bitfield made from any combination of flags: 
//                          0, TTK_DEFAULT      - no changes to default behavior, 
//                          TTK_DISABLE         - this test function will be skipped, 
//                          TTK_NO_ABORT        - execution of remaining tests will be continued even if this test function fails.
#define TTK_ADD_TEST(TestFunction, mode) TTK_ToSuite().AddTest({TestFunction, #TestFunction, mode})

// Declares test function and adds it to be executed.
// TestFunction         Not-existing test function.
// mode                 Bitfield made from any combination of flags: 
//                          0, TTK_DEFAULT      - no changes to default behavior, 
//                          TTK_DISABLE         - this test function will be skipped, 
//                          TTK_NO_ABORT        - execution of remaining tests will be continued even if this test function fails.
#define TTK_TEST(TestFunction, mode) \
    void TestFunction(); \
    static bool s_is_force_called_before_main_##TestFunction = (TTK_ADD_TEST(TestFunction, mode), true); \
    void TestFunction()

// Executes all test functions.
// return   true    - if all tests finished without failing any assertion; 
//          false   - otherwise.
bool TTK_Run();

// Sets output where all generated communicates (including fail messages) by this library will be sent. Can be stdout, stderr or opened file.
void TTK_SetOutput(FILE* output);

// Sets output stream orientation for character type. Same parameter rules as in fwide function.
// orientation      > 0     - wide      (wchar_t)
//                    0     - current 
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

// Test function pointer type.
using TTK_TestFnP_T = void (*)();

struct TTK_TestData{
    TTK_TestFnP_T   function;
    const char*     name;
    uint64_t        mode;      // bitfield
};

//------------------------------------------------------------------------------

class TTK_Register {
public:
    TTK_Register() {
        m_tests             = nullptr;
        m_num_of_tests      = 0;

        m_max_num_of_tests  =  DEF_MAX_NUM_OF_TESTS;
    }

    void AddTest(const TTK_TestData& test_data) {
        if (!m_tests) {
            m_tests = new TTK_TestData[m_max_num_of_tests];
        }

        if (m_num_of_tests >= m_max_num_of_tests) {
            TTK_TestData*   old_tests               = m_tests;
            uint64_t        old_max_num_of_tests    = m_max_num_of_tests;

            m_max_num_of_tests *= 2;
            m_tests = new TTK_TestData[m_max_num_of_tests];

            if (m_num_of_tests > 0) {
                for (uint64_t index = 0; index < m_num_of_tests; ++index) {
                    m_tests[index] = old_tests[index];
                }
            }
            delete[] old_tests;
        }

        m_tests[m_num_of_tests] = test_data;
        m_num_of_tests += 1;
    }

    TTK_TestData& ToTest(uint64_t index) {
        return m_tests[index];
    }

    const TTK_TestData& ToTest(uint64_t index) const {
        return m_tests[index];
    }

    uint64_t GetNumberOfTests() const {
        return m_num_of_tests;
    }

    void Clear() {
        delete[] m_tests;

        m_tests             = nullptr;
        m_num_of_tests      = 0;

        m_max_num_of_tests  =  DEF_MAX_NUM_OF_TESTS;
    }

    virtual ~TTK_Register() {
        delete[] m_tests;
    }
private:
    enum { DEF_MAX_NUM_OF_TESTS = 4096 };

    TTK_TestData*   m_tests;
    uint64_t        m_num_of_tests;

    uint64_t        m_max_num_of_tests;

    void DisplayDBG(FILE* out = stdout) const {
        for (uint64_t index = 0; index < m_num_of_tests; ++index) {
            fprintf(out, "%s\n", m_tests[index].name);
        }
    }
};

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

// Enables unicode codepage for functions which belongs to printf family in a code scope, where it's placed. 
// Restores previous codepage when exits the code scope.
#define TTK_GuardLocaleUTF8() TTK_LocaleGuardianUTF8 locale_guardian_utf8

//------------------------------------------------------------------------------

#if defined(_MSC_VER)      
#define TTK_TRY_FORCE_NON_INLINE __declspec(noinline)
#elif defined(__GNUC__) 
#define TTK_TRY_FORCE_NON_INLINE __attribute__((noinline))
#else
#define TTK_TRY_FORCE_NON_INLINE
#endif

//------------------------------------------------------------------------------

class TTK_Suite {
public:
    TTK_Suite() {
        m_output                        = stdout;

        m_number_of_executed_asserts    = 0;
        m_number_of_failed_asserts      = 0;

        m_number_of_executed_tests      = 0;
        m_number_of_failed_tests        = 0;

        m_forced_orientation            = false;

        m_is_request_abort              = false;
    }

    virtual ~TTK_Suite() {
        if (m_output) fclose(m_output);
    }

    void AddTest(const TTK_TestData& test_data) {
        m_tests.AddTest(test_data);
    }

    void Clear() {
        m_tests.Clear();
    }

    TTK_TRY_FORCE_NON_INLINE
    void CommunicateAssertFail(unsigned line, const char* condition, const wchar_t* file_name_utf16, const char* file_name_utf8, const char* message) {
        TTK_GuardLocaleUTF8();

        m_number_of_failed_asserts += 1;

        if (m_output) {
            if (SolveOutputOrientation() > 0) {
                fwprintf(m_output, L"    [fail] [file:%s] [line:%d] [condition:%hs]", file_name_utf16, line, condition);
                if (message) {
                    fwprintf(m_output, L" [message:%hs]\n", message);
                } else {
                    fwprintf(m_output, L"\n");
                }
            } else {
                fprintf(m_output, "    [fail] [file:%s] [line:%d] [condition:%s]", file_name_utf8, line, condition);
                if (message) {
                    fprintf(m_output, " [message:%s]\n", message);
                } else {
                    fprintf(m_output, "\n");
                }
            }
            fflush(m_output);
        }
    }

    void CommunicateAssertFail(unsigned line, const char* condition, const wchar_t* file_name_utf16, const char* file_name_utf8, const std::string& message) {
        CommunicateAssertFail(line, condition, file_name_utf16, file_name_utf8, message.c_str());
    }

    bool Run() {
        m_number_of_executed_asserts = 0;
        m_number_of_failed_asserts   = 0;

        m_number_of_executed_tests   = 0;
        m_number_of_failed_tests     = 0;

        m_is_request_abort           = false;

        if (SolveOutputOrientation() > 0) {
            fwprintf(m_output, L"%hs", "--- TEST ---\n");
        } else {
            fprintf(m_output, "%s", "--- TEST ---\n");
        }

        uint64_t previous_number_of_failed_asserts = 0;

        for (uint64_t index = 0; index < m_tests.GetNumberOfTests(); ++index) {
            TTK_TestData& test_data = m_tests.ToTest(index);

            if (!(test_data.mode & TTK_DISABLE)) {
                if (SolveOutputOrientation() > 0) {
                    fwprintf(m_output, L"[test] %hs\n", test_data.name);
                } else {
                    fprintf(m_output, "[test] %s\n", test_data.name);
                }
                fflush(m_output);

                test_data.function();

                m_number_of_executed_tests += 1;
                if (m_number_of_failed_asserts != previous_number_of_failed_asserts) { 
                    previous_number_of_failed_asserts = m_number_of_failed_asserts;
                    m_number_of_failed_tests += 1;

                    if (m_is_request_abort) {
                        m_is_request_abort = false;
                        if (!(test_data.mode & TTK_NO_ABORT)) break; // abort all tests
                    }
                }
            }
        }

        const bool is_success = m_number_of_failed_tests == 0;

        if (SolveOutputOrientation() > 0) {
            fwprintf(m_output, L"%hs", (is_success ? "--- TEST SUCCESS ---\n" : "--- TEST FAIL ---\n"));
            fwprintf(m_output, L"number of executed asserts      : %lld\n", m_number_of_executed_asserts);
            fwprintf(m_output, L"number of failed asserts        : %lld\n", m_number_of_failed_asserts);
            fwprintf(m_output, L"number of executed tests        : %lld\n", m_number_of_executed_tests);
            fwprintf(m_output, L"number of failed tests          : %lld\n", m_number_of_failed_tests);
        } else {
            fprintf(m_output, "%s", (is_success ? "--- TEST SUCCESS ---\n" : "--- TEST FAIL ---\n"));
            fprintf(m_output, "number of executed asserts      : %lld\n", m_number_of_executed_asserts);
            fprintf(m_output, "number of failed asserts        : %lld\n", m_number_of_failed_asserts);
            fprintf(m_output, "number of executed tests        : %lld\n", m_number_of_executed_tests);
            fprintf(m_output, "number of failed tests          : %lld\n", m_number_of_failed_tests);
        }
        return is_success;
    }


    void SetOutput(FILE* output) {
        m_output = output;
    }

    void ForceOutputOrientation(int orientation) {
        m_forced_orientation = orientation;
    }

    void RequestAbort() {
        m_is_request_abort = true;
    }

    void Count() {
        m_number_of_executed_asserts += 1;
    }

private:
    int SolveOutputOrientation() {
        if (m_forced_orientation != 0) return m_forced_orientation;
        return fwide(m_output, 0);
    }

    FILE*           m_output;

    uint64_t        m_number_of_executed_asserts;
    uint64_t        m_number_of_failed_asserts;

    uint64_t        m_number_of_executed_tests;
    uint64_t        m_number_of_failed_tests;

    int             m_forced_orientation;

    bool            m_is_request_abort;

    TTK_Register    m_tests;
};


//------------------------------------------------------------------------------

TTK_Suite& TTK_ToSuite() {
    static TTK_Suite s_suite;
    return s_suite;
}

inline bool TTK_Run() {
    return TTK_ToSuite().Run();
}

inline void TTK_SetOutput(FILE* output) {
    TTK_ToSuite().SetOutput(output);
}

inline void TTK_ForceOutputOrientation(int orientation) {
    TTK_ToSuite().ForceOutputOrientation(orientation);
}

inline void TTK_Clear() {
    TTK_ToSuite().Clear();
}

#endif // TRIVIALTESTKIT_H_

