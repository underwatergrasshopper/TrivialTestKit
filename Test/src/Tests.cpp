#include "Tests.h"

#include "TrivialTestKit.h"
#include "LineDependents.h"

#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>

#define _USE_MATH_DEFINES 
#include <math.h>

//==============================================================================

void SystemCommand(const std::wstring& command) {
#ifdef TTK_WIDE_ORIENTED
    _wsystem(command.c_str());
#else
    const char* locale = setlocale(LC_ALL, nullptr);
    std::string backup = locale ? locale : "";

    setlocale(LC_ALL, ".utf8");

    system(TTK_UTF16_ToUTF8(command).c_str());

    setlocale(LC_ALL, backup.c_str());
#endif
}

bool SystemMakeFile(const std::wstring& file_name) {
    HANDLE file_handle = CreateFileW(
        file_name.c_str(), 
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    const bool is_ok = file_handle != INVALID_HANDLE_VALUE;
    if (is_ok) CloseHandle(file_handle);
    return is_ok;
}

bool SystemMakeFile(const std::string& file_name) {
    return SystemMakeFile(TTK_UTF8_ToUTF16(file_name));
}

bool SystemMakeFolder(const std::wstring& folder_name) {
    return CreateDirectoryW(folder_name.c_str(), 0);
}

bool SystemMakeFolder(const std::string& folder_name) {
    return SystemMakeFolder(TTK_UTF8_ToUTF16(folder_name));
}

//------------------------------------------------------------------------------

class Output {
public:
    Output() : m_out(nullptr) {}

    explicit Output(const std::wstring& file_name) : m_out(nullptr) {
#ifdef TTK_WIDE_ORIENTED
        if (_wfopen_s(&m_out, file_name.c_str(), L"w") != 0) m_out = nullptr;
#else
        if (fopen_s(&m_out, TTK_UTF16_ToUTF8(file_name).c_str(), "w") != 0) m_out = nullptr;
#endif
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

inline std::wstring GetDefSolutionBuildDirCutOff() {
#ifdef _DEBUG

#ifdef TTK_WIDE_ORIENTED

#ifdef _WIN64 
    return L"\\x64\\DebugWide";
#else
    return L"\\DebugWide";
#endif

#else

#ifdef _WIN64  
    return L"\\x64\\Debug";
#else
    return L"\\Debug";
#endif

#endif // TTK_WIDE_ORIENTED

#else

#ifdef _WIN64  
    return L"\\x64\\Release";
#else
    return L"\\Release";
#endif

#endif
}

inline std::wstring GetDefSolutionDir() {
    std::wstring cwd = TTK_GetCWD_UTF16();
    auto pos = cwd.find(GetDefSolutionBuildDirCutOff());
    return cwd.substr(0, pos);
}

inline std::wstring GetSourceFileName() {
#ifdef _MSC_VER
    return GetDefSolutionDir() + L"\\Test\\src\\LineDependents.h";
#else
    return L"./src/LineDependents.h";
#endif
}

#ifdef TTK_WIDE_ORIENTED
#define Notice() wprintf(L"%ls\n", TTK_UTF8_ToUTF16(__func__).c_str()); fflush(stdout)
// name     A c-string literal.
#define NoticeWithName(name) wprintf(L"%ls\n", L##name); fflush(stdout)
#else
#define Notice() puts(__func__); fflush(stdout)
// name     A c-string literal.
#define NoticeWithName(name) puts(name); fflush(stdout)
#endif

//==============================================================================

void TestTTK_UTF8_ToUTF16() {
    NoticeWithName("TestTTK_UTF8_ToUTF16");

    assert(TTK_UTF8_ToUTF16(u8"") == L"");
    assert(TTK_UTF8_ToUTF16(u8"Some text\u0444.") == L"Some text\u0444.");

    {
        std::string     long_text_utf8  = u8"Some text\u0444.";
        std::wstring    long_text_utf16 = L"Some text\u0444.";

        enum { DOUBLE_TIMES = 10 };

        for (unsigned ix = 0; ix < DOUBLE_TIMES; ++ix) {
            long_text_utf8  += long_text_utf8;
            long_text_utf16 += long_text_utf16;
        }

        assert(TTK_UTF8_ToUTF16(long_text_utf8) == long_text_utf16);
    }
}

void TestTTK_UTF16_ToUTF8() {
    NoticeWithName("TestTTK_UTF8_ToUTF16");

    assert(TTK_UTF16_ToUTF8(L"") == u8"");
    assert(TTK_UTF16_ToUTF8(L"Some text\u0444.") == u8"Some text\u0444.");

    {
        std::wstring    long_text_utf16 = L"Some text\u0444.";
        std::string     long_text_utf8  = u8"Some text\u0444.";

        enum { DOUBLE_TIMES = 10 };

        for (unsigned ix = 0; ix < DOUBLE_TIMES; ++ix) {
            long_text_utf16 += long_text_utf16;
            long_text_utf8  += long_text_utf8;
        }

        assert(TTK_UTF16_ToUTF8(long_text_utf16) == long_text_utf8);
    }
}


void TestTTK_IsFileExist() {
    Notice();

    // existing
    {
        const std::string file_url = "log/ExistingFile.txt";
        SystemMakeFile(file_url);
        assert(TTK_IsFileExist(file_url));
    }

    // not existing
    {
        assert(TTK_IsFileExist("log/NotExistingFile.txt") == false);
    }

    // wrong type
    {
        const std::string folder_url = "log/NotFile";
        SystemMakeFolder(folder_url);
        assert(TTK_IsFileExist(folder_url) == false);
    }

    // utf8 existing
    {
        const std::string file_url = u8"log/ExistingFile\u0444.txt";
        SystemMakeFile(TTK_UTF8_ToUTF16(file_url));
        assert(TTK_IsFileExist(file_url));
    }
    
    // utf8 not existing
    {
        assert(TTK_IsFileExist(u8"log/NotExistingFile\u0444.txt") == false);
    }
    
    // utf8 wrong type
    {
        const std::string folder_url = u8"log/NotFile\u0444";
        SystemMakeFolder(TTK_UTF8_ToUTF16(folder_url));
        assert(TTK_IsFileExist(folder_url) == false);
    }

    // utf16 existing
    {
        const std::wstring file_url = L"log/ExistingFile_utf16\u0444.txt";
        SystemMakeFile(file_url);
        assert(TTK_IsFileExist(file_url));
    }

    // utf16 not existing
    {
        assert(TTK_IsFileExist(L"log/NotExistingFile_utf16\u0444.txt") == false);
    }

    // utf16 wrong type
    {
        const std::wstring folder_url = L"log/NotFile_utf16\u0444";
        SystemMakeFolder(folder_url);
        assert(TTK_IsFileExist(folder_url) == false);
    }
}

void TestTTK_IsFolderExist() {
    Notice();

    // existing
    {
        const std::string folder_url = "log/ExistingFolder";
        SystemMakeFolder(folder_url);
        assert(TTK_IsFolderExist(folder_url));
    }

    // not existing
    {
        assert(TTK_IsFolderExist("log/NotExistingFolder") == false);
    }

    // wrong type
    {
        const std::string file_url = "log/NotFolder.txt";
        SystemMakeFile(file_url);
        assert(TTK_IsFolderExist(file_url) == false);
    }

    // utf8 existing
    {
        const std::string folder_url = u8"log/ExistingFolder\u0444";
        SystemMakeFolder(TTK_UTF8_ToUTF16(folder_url));
        assert(TTK_IsFolderExist(folder_url));
    }

    // utf8 not existing
    {
        assert(TTK_IsFolderExist(u8"log/NotExistingFolder\u0444") == false);
    }

    // utf8 wrong type
    {
        const std::string file_url = u8"log/NotFolder\u0444.txt";
        SystemMakeFile(TTK_UTF8_ToUTF16(file_url));
        assert(TTK_IsFolderExist(file_url) == false);
    }

    // utf16 existing
    {
        const std::wstring folder_url = L"log/ExistingFolder_utf16\u0444";
        SystemMakeFolder(folder_url);
        assert(TTK_IsFolderExist(folder_url));
    }

    // utf16 not existing
    {
        assert(TTK_IsFolderExist(L"log/NotExistingFolder_utf16\u0444") == false);
    }

    // utf16 wrong type
    {
        const std::wstring file_url = L"log/NotFolder_utf16\u0444.txt";
        SystemMakeFile(file_url);
        assert(TTK_IsFolderExist(file_url) == false);
    }
}

void TestTTK_DeleteFile() {
    Notice();
#if 1
    // existing
    {
        const std::string file_url = "log/FileToDelete.txt";
        SystemMakeFile(file_url);
        assert(TTK_IsFileExist(file_url));
        assert(TTK_DeleteFile(file_url));
        assert(TTK_IsFileExist(file_url) == false);
    }

    // not existing
    {
        const std::string file_url = "log/NotExistingFileToDelete.txt";
        assert(TTK_DeleteFile(file_url) == false);
    }

    // utf8 existing
    {
        const std::string file_url = u8"log/FileToDelete\u0444.txt";
        SystemMakeFile(TTK_UTF8_ToUTF16(file_url));
        assert(TTK_IsFileExist(file_url));
        assert(TTK_DeleteFile(file_url));
        assert(TTK_IsFileExist(file_url) == false);
    }

    // utf8 not existing
    {
        const std::string file_url = u8"log/NotExistingFileToDelete\u0444.txt";
        assert(TTK_DeleteFile(file_url) == false);
    }

    // utf16 existing
    {
        const std::wstring file_url = L"log/FileToDelete_utf16\u0444.txt";
        SystemMakeFile(file_url);
        assert(TTK_IsFileExist(file_url));
        assert(TTK_DeleteFile(file_url));
        assert(TTK_IsFileExist(file_url) == false);
    }

    // utf16 not existing
    {
        const std::wstring file_url = L"log/NotExistingFileToDelete_utf16\u0444.txt";
        assert(TTK_DeleteFile(file_url) == false);
    }
#endif
}

void TestTTK_LoadSaveFileContent() {
    Notice();

    // existing file
    {
        const std::string file_name          = "log/SaveLoadTest_ExistingFile.txt";
        const std::string expected_content  = "Some text to save.\nAnother line.\n";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, expected_content));

        bool is_success = false;
        const std::string content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }

    // existing file no success info
    {
        const std::string file_name          = "log/SaveLoadTest_ExistingFile_NoSuccessInfoe.txt";
        const std::string expected_content  = "Some text to save.\nAnother line.\n";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, expected_content));

        const std::string content = TTK_LoadFromFile(file_name);
        assert(content == expected_content);
    }

    // overriding file
    {
        const std::string file_name          = "log/SaveLoadTest_OverridingFile.txt";
        const std::string initial_content   = "Initial text to save.";
        const std::string expected_content  = "Some text to save.\nAnother line.\n";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, initial_content));
        assert(TTK_SaveToFile(file_name, expected_content));

        bool is_success = false;
        const std::string content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }

    // load not existing file
    {
        const std::string file_name          = "log/SaveLoadTest_NotExistingFile.txt";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        bool is_success = false;
        const std::string content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success == false);
        assert(content == "");
    }

    // load not existing file no success info
    {
        const std::string file_name          = "log/SaveLoadTest_NotExistingFile_NoSuccessInfo.txt";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        const std::string content = TTK_LoadFromFile(file_name);
        assert(content == "");
    }

    // === utf8 === //
#if 1
    // existing file
    {
        const std::string file_name            = u8"log/SaveLoadTest_ExistingFile\u0444.txt";
        const std::string expected_content     = u8"Some text to save.\nAnother line.\u0444\n";
    
        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);
    
        assert(TTK_SaveToFile(file_name, expected_content));
        assert(TTK_IsFileExist(file_name));
    
        bool is_success = false;
        const std::string content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }
    
    // existing file no success info
    {
        const std::string file_name            = u8"log/SaveLoadTest_ExistingFile_NoSuccessInfoe\u0444.txt";
        const std::string expected_content     = u8"Some text to save.\nAnother line.\u0444\n";
    
        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);
    
        assert(TTK_SaveToFile(file_name, expected_content));
        assert(TTK_IsFileExist(file_name));
    
        const std::string content = TTK_LoadFromFile(file_name);
        assert(content == expected_content);
    }
    
    // overriding file
    {
        const std::string file_name            = u8"log/SaveLoadTest_OverridingFile\u0444.txt";
        const std::string initial_content      = u8"Initial text to save.";
        const std::string expected_content     = u8"Some text to save.\nAnother line.\u0444\n";
    
        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);
    
        assert(TTK_SaveToFile(file_name, initial_content));
        assert(TTK_IsFileExist(file_name));
        assert(TTK_SaveToFile(file_name, expected_content));
        assert(TTK_IsFileExist(file_name));
    
        bool is_success = false;
        const std::string content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }
    
    // load not existing file
    {
        const std::string file_name          = u8"log/SaveLoadTest_NotExistingFile\u0444.txt";
    
        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);
    
        bool is_success = false;
        const std::string content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success == false);
        assert(content == u8"");
    }
    
    // load not existing file no success info
    {
        const std::string file_name          = u8"log/SaveLoadTest_NotExistingFile_NoSuccessInfo\u0444.txt";
    
        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);
    
        const std::string content = TTK_LoadFromFile(file_name);
        assert(content == u8"");
    }
#endif
    // === utf16 === //
#if 1
    // existing file
    {
        const std::wstring file_name            = L"log/SaveLoadTest_ExistingFile_utf16\u0444.txt";
        const std::wstring expected_content     = L"Some text to save.\nAnother line.\u0444\n";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);
        
        assert(TTK_SaveToFile(file_name, expected_content));
        assert(TTK_IsFileExist(file_name));

        bool is_success = false;
        const std::wstring content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }

    // existing file no success info
    {
        const std::wstring file_name            = L"log/SaveLoadTest_ExistingFile_NoSuccessInfoe_utf16\u0444.txt";
        const std::wstring expected_content     = L"Some text to save.\nAnother line.\u0444\n";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, expected_content));
        assert(TTK_IsFileExist(file_name));

        const std::wstring content = TTK_LoadFromFile(file_name);
        assert(content == expected_content);
    }

    // overriding file
    {
        const std::wstring file_name          = L"log/SaveLoadTest_OverridingFile_utf16\u0444.txt";
        const std::wstring initial_content   = L"Initial text to save.";
        const std::wstring expected_content  = L"Some text to save.\nAnother line.\u0444\n";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, initial_content));
        assert(TTK_IsFileExist(file_name));
        assert(TTK_SaveToFile(file_name, expected_content));
        assert(TTK_IsFileExist(file_name));

        bool is_success = false;
        const std::wstring content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }

    // load not existing file
    {
        const std::wstring file_name          = L"log/SaveLoadTest_NotExistingFile_utf16\u0444.txt";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        bool is_success = false;
        const std::wstring content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success == false);
        assert(content == L"");
    }

    // load not existing file no success info
    {
        const std::wstring file_name          = L"log/SaveLoadTest_NotExistingFile_NoSuccessInfo_utf16\u0444.txt";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        const std::wstring content = TTK_LoadFromFile(file_name);
        assert(content == L"");
    }
#endif
}

void TestTTK_Assert() {
    Notice();
    // assert success
    {
        const std::wstring output_file_name = L"log/Out_AssertSuccess.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            SuccessAssert(is_finished);
        }
        assert(is_finished);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(communitate == L"");
    }

    // assert success with return
    {
        const std::wstring output_file_name = L"log/Out_AssertSuccessWithReturn.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            assert(SuccessAssertReturn(is_finished) == 0);
        }
        assert(is_finished);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(communitate == L"");
    }

    // assert fail
    {
        const std::wstring output_file_name = L"log/Out_AssertFail.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            FailAssert(is_finished);
        }
        assert(is_finished == false);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);
        const std::wstring expected_communitate = L"    [fail] [line:14] [file:" + GetSourceFileName() + L"] [condition:10 == 5]\n";

        assert(communitate == expected_communitate);
    }

    // assert fail with message
    {
        const std::wstring output_file_name = L"log/Out_AssertFailWithMessage.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            FailAssertMessage(is_finished);
        }
        assert(is_finished == false);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        const std::wstring expected_communitate = L"    [fail] [line:23] [file:" + GetSourceFileName() + L"] [condition:10 == 5] [message:Some message.]\n";

        assert(communitate == expected_communitate);
    }

    // assert fail with return
    {
        const std::wstring output_file_name = L"log/Out_AssertFailWithReturn.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            assert(FailAssertReturn(is_finished) == ASSERT_FAIL_RETURN);
        }
        assert(is_finished == false);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        const std::wstring expected_communitate = L"    [fail] [line:32] [file:" + GetSourceFileName() + L"] [condition:10 == 5]\n";

        assert(communitate == expected_communitate);
    }

    // assert fail with message and return
    {
        const std::wstring output_file_name = L"log/Out_AssertFailWithMessageAndReturn.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            assert(FailAssertMessageReturn(is_finished) == ASSERT_FAIL_RETURN);
        }
        assert(is_finished == false);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        const std::wstring expected_communitate = L"    [fail] [line:43] [file:" + GetSourceFileName() + L"] [condition:10 == 5] [message:Some message.]\n";

        assert(communitate == expected_communitate);
    }

    // assert fail with message utf8
    {
        const std::wstring output_file_name = L"log/Out_AssertFailWithMessage_utf8.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            FailAssertMessageUTF8(is_finished);
        }
        assert(is_finished == false);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        const std::wstring expected_communitate = L"    [fail] [line:99] [file:" + GetSourceFileName() + L"] [condition:10 == 5] [message:Some message\u0444.]\n";

        assert(communitate == expected_communitate);
    }

    // assert fail with message utf16
    {
        const std::wstring output_file_name = L"log/Out_AssertFailWithMessage_utf16.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            FailAssertMessageUTF16(is_finished);
        }
        assert(is_finished == false);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        const std::wstring expected_communitate = L"    [fail] [line:74] [file:" + GetSourceFileName() + L"] [condition:10 == 5] [message:Some message\u0444.]\n";

        assert(communitate == expected_communitate);
    }
}

void TestDummy() {
    TTK_NotifyTest();
}

void TestTTK_NotifyTest() {
    Notice();

    {
        const std::wstring output_file_name = L"log/Out_NotifyTest.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TestDummy();
        }

        const std::wstring expected_communicate = L"[test] TestDummy\n";
        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(expected_communicate == communitate);
    }
}

inline void TestDummyTrace() {
    TTK_Trace("Some message.");
}

inline void TestDummyTraceUTF16() {
    TTK_Trace(L"Some message\u0444.");
}

inline void TestDummyTraceUTF8() {
    TTK_Trace(u8"Some message\u0444.");
}

void TestTTK_Trace() {
    Notice();

    // trace
    {    
        const std::wstring output_file_name = L"log/Out_Trace.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TestDummyTrace();
        }

        const std::wstring expected_communicate = L"[trace] [function:TestDummyTrace] Some message.\n";
        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(expected_communicate == communitate);
    }

    // trace utf8
    {    
        const std::wstring output_file_name = L"log/Out_Trace_utf8.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TestDummyTraceUTF8();
        }

        const std::wstring expected_communicate = L"[trace] [function:TestDummyTraceUTF8] Some message\u0444.\n";
        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(expected_communicate == communitate);
    }

    // trace utf16
    {    
        const std::wstring output_file_name = L"log/Out_Trace_utf16.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TestDummyTraceUTF16();
        }

        const std::wstring expected_communicate = L"[trace] [function:TestDummyTraceUTF16] Some message\u0444.\n";
        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(expected_communicate == communitate);
    }

    // full trace
    {    
        const std::wstring output_file_name = L"log/Out_FullTrace.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TestDummyFullTrace();
        }

        const std::wstring expected_communicate = L"[trace] [line:81] [function:TestDummyFullTrace] [file:" + GetSourceFileName() + L"] Some message.\n";

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(expected_communicate == communitate);
    }

    // full trace utf8
    {    
        const std::wstring output_file_name = L"log/Out_FullTrace_utf8.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TestDummyFullTraceUTF8();
        }

        const std::wstring expected_communicate = L"[trace] [line:106] [function:TestDummyFullTraceUTF8] [file:" + GetSourceFileName() + L"] Some message\u0444.\n";

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(expected_communicate == communitate);
    }

    // full trace utf16
    {    
        const std::wstring output_file_name = L"log/Out_FullTrace_utf16.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TestDummyFullTraceUTF16();
        }

        const std::wstring expected_communicate = L"[trace] [line:85] [function:TestDummyFullTraceUTF16] [file:" + GetSourceFileName() + L"] Some message\u0444.\n";

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(expected_communicate == communitate);
    }
}

template <typename Anything>
inline bool IsReal(Anything&& anything) {
    return true;
}


void TestA() {
    TTK_NotifyTest();

    TTK_Assert(true);
    TTK_Assert(2 == 2);
    TTK_Assert(IsReal(M_PI));
}

void TestB() {
    TTK_NotifyTest();

    TTK_Assert(true);
    TTK_Assert(5 == 5);
    TTK_Assert(IsReal(M_PI));
}

void TestC() {
    TTK_NotifyTest();

    TTK_Assert(true);
    TTK_Assert(8 == 8);
    TTK_Assert(IsReal(M_PI));
}


void TestTTK_RunTests() {
    Notice();

    // success
    {    
        const std::wstring output_file_name = L"log/Out_RunTestsSuccess.txt";

        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            const bool is_success = TTK_RunTests({
                TestA,
                TestB,
                TestC
            });
            assert(is_success);
        }

        const std::wstring expected_communicate = 
            L"--- TEST ---\n"
            L"[test] TestA\n"
            L"[test] TestB\n"
            L"[test] TestC\n"
            L"--- TEST SUCCESS ---\n"
            L"number of executed tests      : 3\n"
            L"number of failed tests        : 0\n";
        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(expected_communicate == communitate);
    }

    // fail
    {   
        const std::wstring output_file_name = L"log/Out_RunTestsFail.txt";

        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            const bool is_success = TTK_RunTests({
                TestA,
                TestB,
                TestD_Fail,
                TestC
                });
            assert(!is_success);
        }

        const std::wstring expected_communicate = 
            L"--- TEST ---\n"
            L"[test] TestA\n"
            L"[test] TestB\n"
            L"[test] TestD_Fail\n"
            L"    [fail] [line:92] [file:" + GetSourceFileName() + L"] [condition:5 == 7]\n"
            L"[test] TestC\n"
            L"--- TEST FAIL ---\n"
            L"number of executed tests      : 4\n"
            L"number of failed tests        : 1\n";

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(expected_communicate == communitate);
    }
}

void RunAllTests() {
#ifdef TTK_WIDE_ORIENTED
    _wsystem(L"if exist log @rd /S /Q log");
    _wsystem(L"mkdir log");

    wprintf(L"%ls\n", L"--- Test Begin ---");
#else
    system("if exist log @rd /S /Q log");
    system("mkdir log");

    puts("--- Test Begin ---");
#endif

    TestTTK_UTF8_ToUTF16();
    TestTTK_UTF16_ToUTF8();
    TestTTK_IsFileExist();
    TestTTK_IsFolderExist();
    TestTTK_DeleteFile();
    TestTTK_LoadSaveFileContent();
    TestTTK_Assert();
    TestTTK_NotifyTest();
    TestTTK_Trace();
    TestTTK_RunTests();

#ifdef TTK_WIDE_ORIENTED
    wprintf(L"%ls\n", L"--- Test End ---");
#else
    puts("--- Test End ---");
#endif
}