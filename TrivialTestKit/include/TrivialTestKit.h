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

// Important !!!
// TTK_WIDE_ORIENTED need to be defined if any function from wprintf family is used (wprintf, fwopen, getwc, ...). 
// #define TTK_WIDE_ORIENTED

// Test function pointer type.
using TTK_TestFnP_T = void (*)();

// Note: The 'if' statement inside TTK_AssertM macro must be in macro. Message can be fetched only after condition fail, because don't exist before fail.

// Checks condition. If condition is false then communicate an assertion fail message (by default to stdout) and exits from calling function.
// condition            Any expression which is castable to bool.
// message              An additional message which will be communicated when condition is false. If it's a statement then will be invoked after the condition fail.
//                      Either c-string (for utf-8), std::string (for utf-8) or std::wstring (for utf-16).
// return_statement     This statement will be returned by calling function when condition fails.
#define TTK_Assert(condition)                               if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__));             return; } (void)0
#define TTK_AssertM(condition, message)                     if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), message);    return; } (void)0
#define TTK_AssertR(condition, return_statement)            if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__));             return (return_statement); } (void)0
#define TTK_AssertMR(condition, message, return_statement)  if (!(condition)) { TKK_CommunicateAssertFail(__LINE__, #condition, TTK_L(__FILE__), message);    return (return_statement); } (void)0

// Place as first line in each test function.
#define TTK_NotifyTest()                                    TTK_InnerNotifyTest(__func__)

// Communicates location in the code and message.
// message          Either c-string (for utf-8), std::string (for utf-8) or std::wstring (for utf-16).
#define TTK_Trace(message)                                  TTK_InnerTrace(__func__, message)
#define TTK_FullTrace(message)                              TTK_InnerFullTrace(__LINE__, __func__, TTK_L(__FILE__), message)

// Runs provided tests and displays result.
// return   true - if all tests finished without failing any assertion; false - otherwise.
template <uint64_t NUMBER> 
bool TTK_RunTests(const TTK_TestFnP_T (&tests)[NUMBER]);

#ifdef TTK_SHORT_NAMES
#define Assert      TTK_Assert
#define AssertM     TTK_AssertM
#define AssertR     TTK_AssertR
#define AssertMR    TTK_AssertMR

#define NotifyTest  TTK_NotifyTest
#define Trace       TTK_Trace
#define FullTrace   TTK_FullTrace
#define RunTests    TTK_RunTests
#endif // TTK_SHORT_NAMES

// file_name        Name of the file from which content will be loaded. Either in utf-8 or utf-16.
// is_success       (optional) If is not nullptr then stores at pointer location: true - when content has been loaded from file, false - otherwise.
// return           Content of the file.
std::string TTK_LoadFromFile(const std::string& file_name, bool* is_success = nullptr);
std::wstring TTK_LoadFromFile(const std::wstring& file_name, bool* is_success = nullptr);

// file_name        Name of the file to which content will be saved. Either in utf-8 or utf-16.
//                  If file not exist, then will be created. 
//                  If file exists then its content will be overridden.
// content          New content of the file.
// return           true    - when content has been saved to file; 
//                  false   - otherwise.
bool TTK_SaveToFile(const std::string& file_name, const std::string& content);
bool TTK_SaveToFile(const std::wstring& file_name, const std::wstring& content);

// file_name        Name of the File with absolute or relative path to it. Either in utf-8 or utf-16.
// return           true    - if file exists; 
//                  false   - if file doesn't exist or it's not a file.
bool TTK_IsFileExist(const std::string& file_name);
bool TTK_IsFileExist(const std::wstring& file_name);

// folder_name      Name of the folder with absolute or relative path to it. Either in utf-8 or utf-16.
// return           true    - if folder exists; 
//                  false   - if folder doesn't exist or it's not a folder.
bool TTK_IsFolderExist(const std::string& folder_name);
bool TTK_IsFolderExist(const std::wstring& folder_name);

// file_name        Name of the File with absolute or relative path to it. Either in utf-8 or utf-16.
// return           true    - if file exists; 
//                  false   - if file doesn't exist or it's not a file.
bool TTK_DeleteFile(const std::string file_name);
bool TTK_DeleteFile(const std::wstring file_name);

// return           Current working directory.
std::wstring TTK_GetCWD_UTF16();
std::string TTK_GetCWD_UTF8();
std::string TTK_GetCWD(); // As ASCII

// output           Sets output where all generated communicates by this library will be sent. Can be stdout, stderr or opened file.
//                  An communicate is: 
//                      any assert fail from TTK_Assert, TTK_AssertM functions;
//                      any track message from TTK_Trace, TTK_TraceFunction functions;
//                      any statistic info from TTK_PrintResults, TTK_RunTests functions.
void TTK_SetOutput(FILE* output);

// Converts text from utf-8 encoding format to utf-16 encoding format.
std::wstring TTK_UTF8_ToUTF16(const std::string& text);

// Converts text from utf-16 encoding format to utf-8 encoding format.
std::string TTK_UTF16_ToUTF8(const std::wstring& text);

//==============================================================================

// Makes regular character array literal a wide character array literal.
#define TTK_INNER_L(text) L##text
#define TTK_L(text) TTK_INNER_L(text)

struct TTK_Data {
    FILE*       output;
    uint64_t    number_of_executed_tests;
    uint64_t    number_of_failed_tests;
};

inline TTK_Data TTK_MakeDefaultData() {
    TTK_Data data = {};

    data.output = stdout;

    return data;
}

inline TTK_Data& TTK_ToData() {
    static TTK_Data s_data = TTK_MakeDefaultData();
    return s_data;
}

class TTK_LocaleUTF8_Guardian {
public:
    TTK_LocaleUTF8_Guardian() {
#ifdef TTK_WIDE_ORIENTED
        SetLocaleWide();
#else
        SetLocale();
#endif
    }

    virtual ~TTK_LocaleUTF8_Guardian() {
#ifdef TTK_WIDE_ORIENTED
        RestoreLocaleWide();
#else
        RestoreLocale();
#endif
    }

private:
    void SetLocaleWide() {
        const char* locale = setlocale(LC_ALL, nullptr);
        if (locale) m_prev_locale_backup = locale;

        setlocale(LC_ALL, ".utf8");
    }

    void SetLocale() {
        const char* locale = setlocale(LC_ALL, nullptr);
        if (locale) m_prev_locale_backup = locale;

        setlocale(LC_ALL, ".utf8");
    }

    void RestoreLocaleWide() {
        _wsetlocale(LC_ALL, m_prev_locale_backup_wide.c_str());
    }

    void RestoreLocale() {
        setlocale(LC_ALL, m_prev_locale_backup.c_str());
    }

    std::string     m_prev_locale_backup;
    std::wstring    m_prev_locale_backup_wide;
};

// Enables unicode codepage for functions from wprintf family in a scope where it's placed. 
// Restores previous codepage when exits the scope.
#define TTK_GuardLocaleUTF8() TTK_LocaleUTF8_Guardian locale_utf8_guardian

//------------------------------------------------------------------------------

inline std::wstring TTK_UTF8_ToUTF16(const std::string& text) {
    std::wstring text_utf16;

    enum { COUNT = 512 };
    static wchar_t s_buffer[COUNT] = {};

    wchar_t* buffer = nullptr;

    if (text.length()) {
        const int count = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);

        buffer = (count > COUNT) ? (new wchar_t[count]) : s_buffer;

        if (MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, buffer, count)) {
            text_utf16 = std::wstring(buffer);
        }

        if (buffer != s_buffer) delete[] buffer;
    }
    return text_utf16;
}

inline std::string TTK_UTF16_ToUTF8(const std::wstring& text) {
    std::string text_utf8;

    enum { COUNT = 512 };
    static char s_buffer[COUNT] = {};

    char* buffer = nullptr;

    if (text.length()) {
        const int count = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, NULL, 0, NULL, NULL);

        buffer = (count > COUNT) ? (new char[count]) : s_buffer;

        if (WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, buffer, count, NULL, NULL)) {
            text_utf8 = std::string(buffer);
        }

        if (buffer != s_buffer) delete[] buffer;
    }
    return text_utf8;
}

//------------------------------------------------------------------------------

// Note: _NoWide uses functions from printf function family. _Wide uses functions from wprintf function family.

inline void TKK_CommunicateAssertFail_NoWide(unsigned line, const char* condition, const std::wstring& file_name, const std::string& message = "") {
    TTK_GuardLocaleUTF8();

    TTK_Data& data = TTK_ToData();

    data.number_of_failed_tests += 1;

    if (data.output) {
        fprintf(data.output, "    [fail] [line:%d] [file:%s] [condition:%s]", line,  TTK_UTF16_ToUTF8(file_name).c_str(), condition);

        if (message.length() > 0) {
            fprintf(data.output, " [message:%s]\n", message.c_str());
        } else {
            fprintf(data.output, "\n");
        }
        fflush(data.output);
    }
}

inline void TKK_CommunicateAssertFail_Wide(unsigned line, const char* condition, const std::wstring& file_name, const std::wstring& message) {
    TTK_GuardLocaleUTF8();

    TTK_Data& data = TTK_ToData();

    data.number_of_failed_tests += 1;

    if (data.output) {

        fwprintf(data.output, L"    [fail] [line:%d] [file:%ls] [condition:%ls]", line,  file_name.c_str(), TTK_UTF8_ToUTF16(condition).c_str());

        if (message.length() > 0) {
            fwprintf(data.output, L" [message:%ls]\n", message.c_str());
        } else {
            fwprintf(data.output, L"\n");
        }
        fflush(data.output);
    }
}

inline void TKK_CommunicateAssertFail(unsigned line, const char* condition, const std::wstring& file_name, const std::string& message = "") {
#ifdef TTK_WIDE_ORIENTED
    TKK_CommunicateAssertFail_Wide(line, condition, file_name, TTK_UTF8_ToUTF16(message));
#else
    TKK_CommunicateAssertFail_NoWide(line, condition, file_name, message);
#endif
}
inline void TKK_CommunicateAssertFail(unsigned line, const char* condition, const std::wstring& file_name, const std::wstring& message) {
#ifdef TTK_WIDE_ORIENTED
    TKK_CommunicateAssertFail_Wide(line, condition, file_name, message);
#else
    TKK_CommunicateAssertFail_NoWide(line, condition, file_name, TTK_UTF16_ToUTF8(message));
#endif
}

//------------------------------------------------------------------------------

inline void TTK_InnerNotifyTest(const std::string& caller_name) {
    TTK_Data& data = TTK_ToData();
#ifdef TTK_WIDE_ORIENTED
    fwprintf(data.output, L"[test] %ls\n", TTK_UTF8_ToUTF16(caller_name).c_str());
#else
    fprintf(data.output, "[test] %s\n", caller_name.c_str());
#endif
    fflush(data.output);
}

//------------------------------------------------------------------------------

inline void TTK_InnerTrace_NoWide(const std::string& caller_name, const std::string& message) {
    TTK_GuardLocaleUTF8();

    TTK_Data& data = TTK_ToData();

    fprintf(data.output, "[trace] [function:%s] %s\n", caller_name.c_str(), message.c_str());
    fflush(data.output);
}

inline void TTK_InnerTrace_Wide(const std::string& caller_name,  const std::wstring& message) {
    TTK_GuardLocaleUTF8();

    TTK_Data& data = TTK_ToData();

    fwprintf(data.output, L"[trace] [function:%ls] %ls\n", TTK_UTF8_ToUTF16(caller_name).c_str(), message.c_str());
    fflush(data.output);
}

inline void TTK_InnerTrace(const std::string& caller_name, const std::string& message) {
#ifdef TTK_WIDE_ORIENTED
    TTK_InnerTrace_Wide(caller_name, TTK_UTF8_ToUTF16(message));
#else
    TTK_InnerTrace_NoWide(caller_name, message);
#endif
}

inline void TTK_InnerTrace(const std::string& caller_name,  const std::wstring& message) {
#ifdef TTK_WIDE_ORIENTED
    TTK_InnerTrace_Wide(caller_name, message);
#else
    TTK_InnerTrace_NoWide(caller_name, TTK_UTF16_ToUTF8(message));
#endif
}

//------------------------------------------------------------------------------

inline void TTK_InnerFullTrace_NoWide(unsigned line, const std::string& caller_name, const std::wstring& file_name, const std::string& message) {
    TTK_GuardLocaleUTF8();

    TTK_Data& data = TTK_ToData();

    fprintf(data.output, "[trace] [line:%d] [function:%s] [file:%s] %s\n", line, caller_name.c_str(), TTK_UTF16_ToUTF8(file_name).c_str(), message.c_str());
    fflush(data.output);
}

inline void TTK_InnerFullTrace_Wide(unsigned line, const std::string& caller_name, const std::wstring& file_name, const std::wstring& message) {
    TTK_GuardLocaleUTF8();

    TTK_Data& data = TTK_ToData();

    fwprintf(data.output, L"[trace] [line:%d] [function:%ls] [file:%ls] %ls\n", line, TTK_UTF8_ToUTF16(caller_name).c_str(), file_name.c_str(), message.c_str());
    fflush(data.output);
}

inline void TTK_InnerFullTrace(unsigned line, const std::string& caller_name, const std::wstring& file_name, const std::string& message) {
#ifdef TTK_WIDE_ORIENTED
    TTK_InnerFullTrace_Wide(line, caller_name, file_name, TTK_UTF8_ToUTF16(message));
#else
    TTK_InnerFullTrace_NoWide(line, caller_name, file_name, message);
#endif
}

inline void TTK_InnerFullTrace(unsigned line, const std::string& caller_name, const std::wstring& file_name, const std::wstring& message) {
#ifdef TTK_WIDE_ORIENTED
    TTK_InnerFullTrace_Wide(line, caller_name, file_name, message);
#else
    TTK_InnerFullTrace_NoWide(line, caller_name, file_name, TTK_UTF16_ToUTF8(message));
#endif

}

//------------------------------------------------------------------------------

template <uint64_t NUMBER> 
bool TTK_RunTests(const TTK_TestFnP_T (&tests)[NUMBER]) {
    TTK_Data& data = TTK_ToData();

    data.number_of_executed_tests   = 0;
    data.number_of_failed_tests     = 0;

#ifdef TTK_WIDE_ORIENTED
    fwprintf(data.output, L"%ls", L"--- TEST ---\n");
#else
    fprintf(data.output, "%s", "--- TEST ---\n");
#endif
    for (const auto& test : tests) {
        test();
    }

    data.number_of_executed_tests = NUMBER;

    const bool is_success = data.number_of_failed_tests == 0;

#ifdef TTK_WIDE_ORIENTED
    fwprintf(data.output, L"%ls", (is_success ? L"--- TEST SUCCESS ---\n" : L"--- TEST FAIL ---\n"));
    fwprintf(data.output, L"number of executed tests      : %lld\n", data.number_of_executed_tests);
    fwprintf(data.output, L"number of failed tests        : %lld\n", data.number_of_failed_tests);
#else
    fprintf(data.output, "%s", (is_success ? "--- TEST SUCCESS ---\n" : "--- TEST FAIL ---\n"));
    fprintf(data.output, "number of executed tests      : %lld\n", data.number_of_executed_tests);
    fprintf(data.output, "number of failed tests        : %lld\n", data.number_of_failed_tests);
#endif
    return is_success;
}

//------------------------------------------------------------------------------

inline std::string TTK_LoadFromFile_NoWide(const std::string& file_name, bool* is_success) {
    TTK_GuardLocaleUTF8();

    std::string content;

    FILE* file = nullptr;

    if ((fopen_s(&file, file_name.c_str(), "r") == 0) && file) {
        char c;
        while ((c = fgetc(file)) != EOF) {
            content += c;
        }
        fclose(file);

        if (is_success) *is_success = true;
    } else {
        if (is_success) *is_success = false;
    }

    return content;
}

inline std::wstring TTK_LoadFromFile_Wide(const std::wstring& file_name, bool* is_success) {
    TTK_GuardLocaleUTF8();

    std::wstring content;

    FILE* file = nullptr;

    if ((_wfopen_s(&file, file_name.c_str(), L"r") == 0) && file) {
        wchar_t c;
        while ((c = fgetwc(file)) != WEOF) {
            content += c;
        }
        fclose(file);

        if (is_success) *is_success = true;
    } else {
        if (is_success) *is_success = false;
    }

    return content;
}

inline std::string TTK_LoadFromFile(const std::string& file_name, bool* is_success) {
#ifdef TTK_WIDE_ORIENTED
    return TTK_UTF16_ToUTF8(TTK_LoadFromFile_Wide(TTK_UTF8_ToUTF16(file_name), is_success));
#else
    return TTK_LoadFromFile_NoWide(file_name, is_success);
#endif
}

inline std::wstring TTK_LoadFromFile(const std::wstring& file_name, bool* is_success) {
#ifdef TTK_WIDE_ORIENTED
    return TTK_LoadFromFile_Wide(file_name, is_success);
#else
    return TTK_UTF8_ToUTF16(TTK_LoadFromFile_NoWide(TTK_UTF16_ToUTF8(file_name), is_success));
#endif
}

//------------------------------------------------------------------------------

inline bool TTK_SaveToFile_NoWide(const std::string& file_name, const std::string& content) {
    TTK_GuardLocaleUTF8();

    FILE* file = nullptr;
    if (fopen_s(&file, file_name.c_str(), "w") == 0 && file) {
        const int count = fprintf(file, "%s", content.c_str());
        fclose(file);

        return count == content.length();
    }

    return false;
}

inline bool TTK_SaveToFile_Wide(const std::wstring& file_name, const std::wstring& content) {
    TTK_GuardLocaleUTF8();

    FILE* file = nullptr;
    if (_wfopen_s(&file, file_name.c_str(), L"w") == 0 && file) {
        const int count = fwprintf(file, L"%ls", content.c_str());
        fclose(file);

        return count == content.length();
    }

    return false;
}

inline bool TTK_SaveToFile(const std::string& file_name, const std::string& content) {
#ifdef TTK_WIDE_ORIENTED
    return TTK_SaveToFile_Wide(TTK_UTF8_ToUTF16(file_name), TTK_UTF8_ToUTF16(content));
#else
    return TTK_SaveToFile_NoWide(file_name, content);
#endif
}

inline bool TTK_SaveToFile(const std::wstring& file_name, const std::wstring& content) {
#ifdef TTK_WIDE_ORIENTED
    return TTK_SaveToFile_Wide(file_name, content);
#else
    return TTK_SaveToFile_NoWide(TTK_UTF16_ToUTF8(file_name), TTK_UTF16_ToUTF8(content));
#endif
}

//------------------------------------------------------------------------------

inline bool TTK_IsFileExist(const std::string& file_name) {
    return TTK_IsFileExist(TTK_UTF8_ToUTF16(file_name));
}

inline bool TTK_IsFileExist(const std::wstring& file_name) {
    DWORD attributes = GetFileAttributesW(file_name.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

inline bool TTK_IsFolderExist(const std::string& folder_name) {
    return TTK_IsFolderExist(TTK_UTF8_ToUTF16(folder_name));
}

inline bool TTK_IsFolderExist(const std::wstring& folder_name) {
    DWORD attributes = GetFileAttributesW(folder_name.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

inline bool TTK_DeleteFile(const std::string file_name) {
    return TTK_DeleteFile(TTK_UTF8_ToUTF16(file_name));
}

inline bool TTK_DeleteFile(const std::wstring file_name) {
    return DeleteFileW(file_name.c_str());
}

inline std::wstring TTK_GetCWD_UTF16() {
    WCHAR buffer[MAX_PATH] = {0};
    DWORD count = GetModuleFileNameW(NULL, buffer, MAX_PATH);

    std::wstring dir = std::wstring(buffer, count);
    auto pos = dir.find_last_of(L"\\");
    return dir.substr(0, pos);
}

inline std::string TTK_GetCWD_UTF8() {
    return TTK_UTF16_ToUTF8(TTK_GetCWD_UTF16());
}

inline std::string TTK_GetCWD() {
    char buffer[MAX_PATH] = {0};
    DWORD count = GetModuleFileNameA(NULL, buffer, MAX_PATH);

    std::string dir = std::string(buffer, count);
    auto pos = dir.find_last_of("\\");
    return dir.substr(0, pos);
}

//------------------------------------------------------------------------------

inline void TTK_SetOutput(FILE* output) {
    TTK_ToData().output = output;
}

#endif // TRIVIALTESTKIT_H_

