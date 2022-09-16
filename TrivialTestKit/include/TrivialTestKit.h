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
#include <string>


struct TTK_Data {
    FILE* communicate_output;
};

inline TTK_Data& TTK_ToData() {
    static TTK_Data s_data = { stdout };
    return s_data;
}

inline void TTK_SetCommunitateOut(FILE* communicate_output) {
    TTK_ToData().communicate_output = communicate_output;
}

inline void TKK_CommunicateAssertFail(FILE* communicate_output, unsigned line, const char* condition, const char* filename, const char* message) {
    if (communicate_output) {
        if (message) {
            fprintf(communicate_output, "[fail] [line:%d] [file:%s] [condition:%s] [message:%s] \n", line, filename, condition, message);
        } else {
            fprintf(communicate_output, "[fail] [line:%d] [file:%s] [condition:%s]\n", line, filename, condition);
        }
        fflush(communicate_output);
    }
}

inline void TKK_CommunicateAssertFail(FILE* communicate_output, unsigned line, const char* condition, const char* file_name, const std::string& message) {
    TKK_CommunicateAssertFail(communicate_output, line, condition, file_name, message);
}

// filename         Name of the file from which content will be loaded.
// is_success       (optional) If is not nullptr then stores at pointer location: true - when content has been loaded from file, false - otherwise.
// return           Content of the file.
inline std::string TKK_LoadContentFromFile(const std::string& filename, bool* is_success = nullptr) {
    std::string content;

    FILE* file = nullptr;
    if (fopen_s(&file, filename.c_str(), "r") == 0 && file) {
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

inline bool TTK_SaveContentToFile(const std::string& filename, const std::string& content) {
    FILE* file = nullptr;
    if (fopen_s(&file, filename.c_str(), "w") == 0 && file) {
        if (fwrite(content.c_str(), sizeof(char), content.length(), file) == content.length()) {
            fclose(file);
            return true;
        }
    }
    return false;
}

#define TTK_Assert(condition) if (!(condition)) { TKK_CommunicateAssertFail(TTK_ToData().communicate_output, __LINE__, #condition, __FILE__, nullptr); } (void)0
#define TTK_AssertM(condition, message) if (!(condition)) { TKK_CommunicateAssertFail(TTK_ToData().communicate_output, __LINE__, #condition, __FILE__, message); } (void)0

#endif // TRIVIALTESTKIT_H_

