#include "Tests.h"

#include "TrivialTestKit.h"
#include "LineDependents.h"
#include "Support.h"

#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>

#define _USE_MATH_DEFINES 
#include <math.h>

//==============================================================================

void Test_ToUTF16() {
    NoticeWithLiteral("Test_ToUTF16");

    assert(ToUTF16(u8"") == L"");
    assert(ToUTF16(u8"Some text\u0444.") == L"Some text\u0444.");

    {
        std::string     long_text_utf8  = u8"Some text\u0444.";
        std::wstring    long_text_utf16 = L"Some text\u0444.";

        enum { DOUBLE_TIMES = 10 };

        for (unsigned ix = 0; ix < DOUBLE_TIMES; ++ix) {
            long_text_utf8  += long_text_utf8;
            long_text_utf16 += long_text_utf16;
        }

        assert(ToUTF16(long_text_utf8) == long_text_utf16);
    }
}

void Test_ToUTF8() {
    NoticeWithLiteral("Test_ToUTF16");

    assert(ToUTF8(L"") == u8"");
    assert(ToUTF8(L"Some text\u0444.") == u8"Some text\u0444.");

    {
        std::wstring    long_text_utf16 = L"Some text\u0444.";
        std::string     long_text_utf8  = u8"Some text\u0444.";

        enum { DOUBLE_TIMES = 10 };

        for (unsigned ix = 0; ix < DOUBLE_TIMES; ++ix) {
            long_text_utf16 += long_text_utf16;
            long_text_utf8  += long_text_utf8;
        }

        assert(ToUTF8(long_text_utf16) == long_text_utf8);
    }
}

void Test_IsFileExist_ASCII() {
    Notice();

    // not exists
    {
        const std::string file_url = "log\\NotExistingFile.txt";
        assert(IsFileExist_ASCII(file_url) == false);
    }

    // existing
    {
        const std::string file_url = "log\\ExistingFile.txt";

        const std::string command = std::string("echo. > ") + file_url;
        system(command.c_str());

        assert(IsFileExist_ASCII(file_url));
    }

    // wrong type
    {
        const std::string folder_url = "log\\NotFile";

        const std::string command = std::string("mkdir ") + folder_url;
        system(command.c_str());

        assert(IsFileExist_ASCII(folder_url) == false);
    }
}

#if 0
void Test_CreateFile_ASCII() {
    Notice();

    // new
    {
        const std::string file_url = "log\\NewFile.txt";
        assert(IsFileExist_ASCII(file_url) == false);

        assert(CreateFile_ASCII(file_url));
        assert(IsFileExist_ASCII(file_url));
    }

    // existing
    {
        const std::string file_url = "log\\NewExitingFile.txt";
        assert(IsFileExist_ASCII(file_url) == false);
        assert(CreateFile_ASCII(file_url));   

        assert(IsFileExist_ASCII(file_url));
        assert(CreateFile_ASCII(file_url) == false);
    }
}

void Test_DeleteFile_ASCII() {
    Notice();

    // existing
    {
        const std::string file_url = "log\\FileToDelete.txt";
        assert(IsFileExist_ASCII(file_url) == false);
        assert(CreateFile_ASCII(file_url));
        assert(IsFileExist_ASCII(file_url));

        assert(DeleteFile_ASCII(file_url));
        assert(IsFileExist_ASCII(file_url) == false);
    }

    // not existing
    {
        const std::string file_url = "log\\NotExistingFileToDelete.txt";
        assert(IsFileExist_ASCII(file_url) == false);

        assert(DeleteFile_ASCII(file_url) == false);
        assert(IsFileExist_ASCII(file_url) == false);
    }

    // wrong type
    {
        const std::string folder_url = "log\\NotExistingFileToDelete";

        const std::string command = std::string("mkdir ") + folder_url;
        system(command.c_str());

        assert(DeleteFile_ASCII(folder_url) == false);
        assert(IsFileExist_ASCII(folder_url) == false);
    }
}
#endif
#if 0
void Test_IsFolderExist_ASCII() {
    Notice();

    // not exists
    {
        const std::string folder_url = "log\\NotExistingFolder";
        assert(IsFolderExist_ASCII(folder_url) == false);
    }

    // existing
    {
        const std::string folder_url = "log\\ExistingFolder";

        const std::string command = std::string("mkdir ") + folder_url;
        system(command.c_str());

        assert(IsFolderExist_ASCII(folder_url));
    }

    // wrong type
    {
        const std::string file_url = "log\\NotFolder.txt";

        const std::string command = std::string("echo. > ") + file_url;
        system(command.c_str());

        assert(IsFolderExist_ASCII(file_url) == false);
    }
}
#endif

#if 0

void TestTTK_LoadSaveFileContent() {
    Notice();

    // existing file
    {
        const std::string file_name          = "log/SaveLoadTest_ExistingFile.txt";
        const std::string expected_content  = "Some text to save.\nAnother line.\n";

        TTK_DeleteFile(file_name);
        assert(IsFileExist(file_name) == false);

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
        assert(IsFileExist(file_name) == false);

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
        assert(IsFileExist(file_name) == false);

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
        assert(IsFileExist(file_name) == false);

        bool is_success = false;
        const std::string content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success == false);
        assert(content == "");
    }

    // load not existing file no success info
    {
        const std::string file_name          = "log/SaveLoadTest_NotExistingFile_NoSuccessInfo.txt";

        TTK_DeleteFile(file_name);
        assert(IsFileExist(file_name) == false);

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
        assert(IsFileExist(file_name) == false);
    
        assert(TTK_SaveToFile(file_name, expected_content));
        assert(IsFileExist(file_name));
    
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
        assert(IsFileExist(file_name) == false);
    
        assert(TTK_SaveToFile(file_name, expected_content));
        assert(IsFileExist(file_name));
    
        const std::string content = TTK_LoadFromFile(file_name);
        assert(content == expected_content);
    }
    
    // overriding file
    {
        const std::string file_name            = u8"log/SaveLoadTest_OverridingFile\u0444.txt";
        const std::string initial_content      = u8"Initial text to save.";
        const std::string expected_content     = u8"Some text to save.\nAnother line.\u0444\n";
    
        TTK_DeleteFile(file_name);
        assert(IsFileExist(file_name) == false);
    
        assert(TTK_SaveToFile(file_name, initial_content));
        assert(IsFileExist(file_name));
        assert(TTK_SaveToFile(file_name, expected_content));
        assert(IsFileExist(file_name));
    
        bool is_success = false;
        const std::string content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }
    
    // load not existing file
    {
        const std::string file_name          = u8"log/SaveLoadTest_NotExistingFile\u0444.txt";
    
        TTK_DeleteFile(file_name);
        assert(IsFileExist(file_name) == false);
    
        bool is_success = false;
        const std::string content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success == false);
        assert(content == u8"");
    }
    
    // load not existing file no success info
    {
        const std::string file_name          = u8"log/SaveLoadTest_NotExistingFile_NoSuccessInfo\u0444.txt";
    
        TTK_DeleteFile(file_name);
        assert(IsFileExist(file_name) == false);
    
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
        assert(IsFileExist(file_name) == false);
        
        assert(TTK_SaveToFile(file_name, expected_content));
        assert(IsFileExist(file_name));

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
        assert(IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, expected_content));
        assert(IsFileExist(file_name));

        const std::wstring content = TTK_LoadFromFile(file_name);
        assert(content == expected_content);
    }

    // overriding file
    {
        const std::wstring file_name          = L"log/SaveLoadTest_OverridingFile_utf16\u0444.txt";
        const std::wstring initial_content   = L"Initial text to save.";
        const std::wstring expected_content  = L"Some text to save.\nAnother line.\u0444\n";

        TTK_DeleteFile(file_name);
        assert(IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, initial_content));
        assert(IsFileExist(file_name));
        assert(TTK_SaveToFile(file_name, expected_content));
        assert(IsFileExist(file_name));

        bool is_success = false;
        const std::wstring content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }

    // load not existing file
    {
        const std::wstring file_name          = L"log/SaveLoadTest_NotExistingFile_utf16\u0444.txt";

        TTK_DeleteFile(file_name);
        assert(IsFileExist(file_name) == false);

        bool is_success = false;
        const std::wstring content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success == false);
        assert(content == L"");
    }

    // load not existing file no success info
    {
        const std::wstring file_name          = L"log/SaveLoadTest_NotExistingFile_NoSuccessInfo_utf16\u0444.txt";

        TTK_DeleteFile(file_name);
        assert(IsFileExist(file_name) == false);

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

#endif

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

    Test_ToUTF16();
    Test_ToUTF8();
    Test_IsFileExist_ASCII();
    //Test_CreateFile_ASCII();
    //Test_IsFolderExist_ASCII();
    //TestTTK_DeleteFile();
    //TestTTK_LoadSaveFileContent();
    //TestTTK_Assert();
    //TestTTK_NotifyTest();
    //TestTTK_Trace();
    //TestTTK_RunTests();

#ifdef TTK_WIDE_ORIENTED
    wprintf(L"%ls\n", L"--- Test End ---");
#else
    puts("--- Test End ---");
#endif
}