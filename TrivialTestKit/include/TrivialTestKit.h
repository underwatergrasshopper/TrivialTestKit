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
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <string>

// Note: The 'If' statement inside TTK_AssertM macro must be in macro. Message can be fetched only after condition fail, because don't exist before fail.

// Checks condition. If condition is false then communicate (by default to stdout) an assertion fail message and exits from calling function.
// condition            Any expression which is castable to bool.
// message              An additional message which will be communicated when condition is false.
// return_statement     Result of this statement will be returned by calling function when condition fails.
#define TTK_Assert(condition)                               if (!(condition)) { TKK_CommunicateAssertFail(TTK_ToData().communication_output, __LINE__, #condition, __FILE__, nullptr); return; } (void)0
#define TTK_AssertM(condition, message)                     if (!(condition)) { TKK_CommunicateAssertFail(TTK_ToData().communication_output, __LINE__, #condition, __FILE__, message); return; } (void)0
#define TTK_AssertR(condition, return_statement)            if (!(condition)) { TKK_CommunicateAssertFail(TTK_ToData().communication_output, __LINE__, #condition, __FILE__, nullptr); return (return_statement); } (void)0
#define TTK_AssertMR(condition, message, return_statement)  if (!(condition)) { TKK_CommunicateAssertFail(TTK_ToData().communication_output, __LINE__, #condition, __FILE__, message); return (return_statement); } (void)0

#ifdef TTK_SHORT_NAMES
#define Assert      TTK_Assert
#define AssertM     TTK_AssertM
#define AssertR     TTK_AssertR
#define AssertMR    TTK_AssertMR
#endif // TTK_SHORT_NAMES

// file_name        Name of the file from which content will be loaded.
// is_success       (optional) If is not nullptr then stores at pointer location: true - when content has been loaded from file, false - otherwise.
// return           Content of the file.
std::string TTK_LoadFromFile(const std::string& file_name, bool* is_success = nullptr);
std::wstring TTK_LoadFromFile(const std::wstring& file_name, bool* is_success = nullptr);

// file_name        Name of the file to which content will be saved. 
//                  If file not exist, then will be created. 
//                  If file exists then its content will be overridden.
// content          New content of the file.
// return           true    - when content has been saved to file; 
//                  false   - otherwise.
bool TTK_SaveToFile(const std::string& file_name, const std::string& content);
bool TTK_SaveToFile(const std::wstring& file_name, const std::wstring& content);

// file_name    Name of the File with absolute or relative path to it.
// return       true    - if file exists; 
//              false   - if file doesn't exist or it's not a file.
bool TTK_IsFileExist(const std::string& file_name);
bool TTK_IsFileExist(const std::wstring& file_name);

// folder_name  Name of the folder with absolute or relative path to it.
// return       true    - if folder exists; 
//              false   - if folder doesn't exist or it's not a folder.
bool TTK_IsFolderExist(const std::string& folder_name);
bool TTK_IsFolderExist(const std::wstring& folder_name);

// file_name    Name of the File with absolute or relative path to it.
// return       true    - if file exists; 
//              false   - if file doesn't exist or it's not a file.
bool TTK_DeleteFile(const std::string file_name);
bool TTK_DeleteFile(const std::wstring file_name);

// return       Current working directory.
std::wstring TTK_GetCWD_UTF16();
std::string TTK_GetCWD();

// communication_output         Sets output where all communicates generated by this library will be sent. Can be stdout, stderr or opened file.
//                              An communicate is: 
//                                  any assert fail from TTK_Assert, TTK_AssertM functions;
//                                  any track message from TTK_Trace, TTK_TraceFunction functions;
//                                  any statistic info from TTK_PrintResults, TTK_RunTests functions.
void TTK_SetCommunitationOutput(FILE* communication_output);

//==============================================================================

struct TTK_Data {
    FILE* communication_output;
};

inline TTK_Data& TTK_ToData() {
    static TTK_Data s_data = { stdout };
    return s_data;
}

//------------------------------------------------------------------------------

inline void TKK_CommunicateAssertFail(FILE* communication_output, unsigned line, const char* condition, const char* file_name, const char* message) {
    if (communication_output) {
        if (message) {
            fprintf(communication_output, "    [fail] [line:%d] [file:%s] [condition:%s] [message:%s]\n", line, file_name, condition, message);
        } else {
            fprintf(communication_output, "    [fail] [line:%d] [file:%s] [condition:%s]\n", line, file_name, condition);
        }
        fflush(communication_output);
    }
}

inline void TKK_CommunicateAssertFail(FILE* communication_output, unsigned line, const char* condition, const char* file_name, const std::string& message) {
    TKK_CommunicateAssertFail(communication_output, line, condition, file_name, message);
}


inline void TTK_InnerTraceTest(const std::string& caller_name) {
    fprintf(TTK_ToData().communication_output, "[test:%s]\n", caller_name.c_str());
}


//------------------------------------------------------------------------------

inline std::string TTK_LoadFromFile(const std::string& file_name, bool* is_success) {
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

inline std::wstring TTK_LoadFromFile(const std::wstring& file_name, bool* is_success) {
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

inline bool TTK_SaveToFile(const std::string& file_name, const std::string& content) {
    FILE* file = nullptr;
    if (fopen_s(&file, file_name.c_str(), "w") == 0 && file) {
        if (fwrite(content.c_str(), sizeof(char), content.length(), file) == content.length()) {
            fclose(file);
            return true;
        }
    }
    return false;
}

inline bool TTK_SaveToFile(const std::wstring& file_name, const std::wstring& content) {
    FILE* file = nullptr;
    if (_wfopen_s(&file, file_name.c_str(), L"w") == 0 && file) {
        for (const auto& sign : content) {
            if (fputwc(sign, file) == WEOF) {
                fclose(file);
                return false;
            }
        }
        fclose(file);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------

inline bool TTK_IsFileExist(const std::string& file_name) {
    DWORD attributes = GetFileAttributesA(file_name.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

inline bool TTK_IsFileExist(const std::wstring& file_name) {
    DWORD attributes = GetFileAttributesW(file_name.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

inline bool TTK_IsFolderExist(const std::string& folder_name) {
    DWORD attributes = GetFileAttributesA(folder_name.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

inline bool TTK_IsFolderExist(const std::wstring& folder_name) {
    DWORD attributes = GetFileAttributesW(folder_name.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY);
}

inline bool TTK_DeleteFile(const std::string file_name) {
    return DeleteFileA(file_name.c_str());
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

inline std::string TTK_GetCWD() {
    char buffer[MAX_PATH] = {0};
    DWORD count = GetModuleFileNameA(NULL, buffer, MAX_PATH);

    std::string dir = std::string(buffer, count);
    auto pos = dir.find_last_of("\\");
    return dir.substr(0, pos);
}

//------------------------------------------------------------------------------

inline void TTK_SetCommunitationOutput(FILE* communication_output) {
    TTK_ToData().communication_output = communication_output;
}

#endif // TRIVIALTESTKIT_H_

