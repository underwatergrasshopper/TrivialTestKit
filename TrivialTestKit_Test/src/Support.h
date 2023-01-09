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

#ifndef SUPPORT_H_
#define SUPPORT_H_

#include <windows.h>
#include <locale.h>

#include <string>

//------------------------------------------------------------------------------

inline bool IsWideOriented() {
#ifdef WIDE_ORIENTED
    return true;
#else
    return false;
#endif
}

//------------------------------------------------------------------------------

class Output {
public:
    Output() : m_out(nullptr) {}

    explicit Output(const std::string& file_name) : m_out(nullptr) {
        if (fopen_s(&m_out, file_name.c_str(), "w") != 0) m_out = nullptr;
    }

    virtual ~Output() {
        if (m_out) fclose(m_out);
    }

    FILE* Access() {
        return m_out;
    }
private:
    FILE* m_out;
};

//------------------------------------------------------------------------------

inline std::wstring ToUTF16(const std::string& text) {
    std::wstring text_utf16;

    enum { COUNT = 512 };
    static wchar_t s_buffer[COUNT] = {};

    if (!text.empty()) {

        const int count = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);

        if (count == 0) {
            const int orientation = fwide(stdout, 0);
            if (orientation > 0) {
                fwprintf(stderr, L"%ls\n", L"ToUTF16 Error: Can not convert text from utf-8 to utf-16.");
            } else {
                fprintf(stderr, "%s\n", "ToUTF16 Error: Can not convert text from utf-8 to utf-16.");
            }
            exit(EXIT_FAILURE);
        }

        wchar_t* buffer = (count > COUNT) ? (new wchar_t[count]) : s_buffer;

        if (MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, buffer, count)) {
            text_utf16 = std::wstring(buffer);
        }

        if (buffer != s_buffer) delete[] buffer;
    }
    return text_utf16;
}

inline std::string ToUTF8(const std::wstring& text) {
    std::string text_utf8;

    enum { COUNT = 512 };
    static char s_buffer[COUNT] = {};

    if (text.length()) {
        const int count = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, NULL, 0, NULL, NULL);

        if (count == 0) {
            const int orientation = fwide(stdout, 0);
            if (orientation > 0) {
                fwprintf(stderr, L"%ls\n", L"ToUTF8 Error: Can not convert text from utf-16 to utf-8.");
            } else {
                fprintf(stderr, "%s\n", "ToUTF8 Error: Can not convert text from utf-16 to utf-8.");
            }
            exit(EXIT_FAILURE);
        }

        char* buffer = (count > COUNT) ? (new char[count]) : s_buffer;

        if (WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, buffer, count, NULL, NULL)) {
            text_utf8 = std::string(buffer);
        }

        if (buffer != s_buffer) delete[] buffer;
}
    return text_utf8;
}

//------------------------------------------------------------------------------

inline void InnerNotice(const std::string& caller_name, const std::wstring& caller_name_utf16) {
    const int orientation = fwide(stdout, 0);
    if (orientation > 0) {
        wprintf(L"%ls\n", caller_name_utf16.c_str());
    } else {
        printf("%s\n", caller_name.c_str());
    }
    fflush(stdout);
}

#define NoticeWithLiteral(name) InnerNotice(name, L##name)

#define Notice() InnerNotice(__func__, ToUTF16(__func__))

//------------------------------------------------------------------------------

inline bool IsFileExist_ASCII(const std::string& file_name) {
    DWORD attributes = GetFileAttributesA(file_name.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

inline bool IsFileExist_UTF16(const std::wstring& file_name) {
    DWORD attributes = GetFileAttributesW(file_name.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

//------------------------------------------------------------------------------

class LocaleGuradianUTF8 {
public:
    LocaleGuradianUTF8() {
        const char* locale = setlocale(LC_ALL, nullptr);
        if (locale) m_locale_backup = locale;

        setlocale(LC_ALL, ".utf8");
    }

    virtual ~LocaleGuradianUTF8() {
        setlocale(LC_ALL, m_locale_backup.c_str());
    }
private:
    std::string m_locale_backup;
};
#define GuardLocaleUTF8() LocaleGuradianUTF8 locale_gurardian_utf8

inline std::string LoadFromFile_UTF8(const std::string& file_name, bool* is_success = nullptr) {
    GuardLocaleUTF8();

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

inline bool SaveToFile_UTF8(const std::string& file_name, const std::string& content) {
    GuardLocaleUTF8();

    FILE* file = nullptr;

    if (fopen_s(&file, file_name.c_str(), "w") == 0 && file) {
        const int count = fprintf(file, "%s", content.c_str());
        fclose(file);

        return count == content.length();
    }

    return false;
}

//------------------------------------------------------------------------------

inline std::string ReplaceAll(const std::string& text, const std::string& old_part, const std::string& new_part) {
    std::string new_text = text;

    if (!old_part.empty()) {
        for (size_t pos = 0; (pos = new_text.find(old_part, pos)) != std::string::npos; pos += new_part.length()) {
            new_text.replace(pos, old_part.length(), new_part);
        }
    }

    return new_text;
}

inline std::wstring GetCWD_UTF16() {
    WCHAR buffer[MAX_PATH] = {0};
    DWORD count = GetModuleFileNameW(NULL, buffer, MAX_PATH);

    std::wstring dir = std::wstring(buffer, count);
    auto pos = dir.find_last_of(L"\\");
    return dir.substr(0, pos);
}


inline std::wstring GetBuildFolder() { 
#ifdef _MSC_VER 
    return L"\\build\\vs";
#else
    return L"\\build\\mingw_llvm";
#endif
}

inline std::wstring GetVS_PlatformName() {
#ifdef _WIN64 
    return L"x64";
#else
    return L"Win32";
#endif
}



inline std::wstring GetDefSolutionBuildDirCutOff() {
#ifdef _DEBUG

#ifdef WIDE_ORIENTED

#ifdef _WIN64 
    return L"\\x64\\DebugWide";
#else
    return L"\\Win32\\DebugWide";
#endif

#else // not WIDE_ORIENTED

#ifdef _WIN64  
    return L"\\x64\\Debug";
#else
    return L"\\Win32\\Debug";
#endif

#endif // WIDE_ORIENTED

#else // not _DEBUG

#ifdef WIDE_ORIENTED

#ifdef _WIN64 
    return L"\\x64\\ReleaseWide";
#else
    return L"\\Win32\\ReleaseWide";
#endif

#else // not WIDE_ORIENTED

#ifdef _WIN64  
    return L"\\x64\\Release";
#else
    return L"\\Win32\\Release";
#endif

#endif // WIDE_ORIENTED

#endif // _DEBUG
}

inline std::wstring GetDefSolutionDir_UTF16() {
    const std::wstring cwd = GetCWD_UTF16();
    auto pos = cwd.find(GetBuildFolder() + GetDefSolutionBuildDirCutOff());
    return cwd.substr(0, pos);
}

inline std::string GetSourceFileName_UTF8(
    const std::string& file_name        = "\\TrivialTestKit_Test\\src\\LineDependents.h", 
    const std::string& file_name_gcc    = "/src/LineDependents.h") {
#ifdef _MSC_VER
    return ToUTF8(GetDefSolutionDir_UTF16()) + file_name;
#else
    return ReplaceAll(ToUTF8(GetDefSolutionDir_UTF16()), "\\", "/") + file_name_gcc;
#endif
}

#endif // SUPPORT_H_