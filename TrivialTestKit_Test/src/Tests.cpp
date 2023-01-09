#include "Tests.h"

#include "TrivialTestKit.h"
#include "Support.h"

#include "LineDependents.h"

// Not all compilers are able to accept unicode characters from command argument list.
#ifndef NO_TEST_IN_UNICODE_FOLDER
#include "InUnicodeFolder.h"
#endif

#include <stdio.h>
#include <stdlib.h> 

#define _USE_MATH_DEFINES 
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef NDEBUG
#undef NDEBUG
#include <assert.h>
#define NDEBUG
#else
#include <assert.h>
#endif

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

void Test_LoadAndSaveToFile() {
    Notice();

    // existing file
    {
        const std::string file_name        = "log/SaveLoadTest_ExistingFile.txt";
        const std::string expected_content = u8"Some text to save.\nAnother line.\u0444\n";

        assert(IsFileExist_ASCII(file_name) == false);

        assert(SaveToFile_UTF8(file_name, expected_content));

        bool is_success = false;
        const std::string content = LoadFromFile_UTF8(file_name, &is_success);

        assert(is_success);
        assert(content == expected_content);
    }

    // existing file no success info
    {
        const std::string file_name         = "log/SaveLoadTest_ExistingFile_NoSuccessInfoe.txt";
        const std::string expected_content  = u8"Some text to save.\nAnother line.\u0444\n";

        assert(IsFileExist_ASCII(file_name) == false);

        assert(SaveToFile_UTF8(file_name, expected_content));

        const std::string content = LoadFromFile_UTF8(file_name);
        assert(content == expected_content);
    }

    // overriding file
    {
        const std::string file_name         = "log/SaveLoadTest_OverridingFile.txt";
        const std::string initial_content   = u8"Initial text to save.\u0444";
        const std::string expected_content  = u8"Some text to save.\nAnother line.\u0444\n";

        assert(IsFileExist_ASCII(file_name) == false);

        assert(SaveToFile_UTF8(file_name, initial_content));
        assert(SaveToFile_UTF8(file_name, expected_content));

        bool is_success = false;
        const std::string content = LoadFromFile_UTF8(file_name, &is_success);

        assert(is_success);
        assert(content == expected_content);
    }

    // load not existing file
    {
        const std::string file_name          = "log/SaveLoadTest_NotExistingFile.txt";

        assert(IsFileExist_ASCII(file_name) == false);

        bool is_success = false;
        const std::string content = LoadFromFile_UTF8(file_name, &is_success);

        assert(is_success == false);
        assert(content == "");
    }

    // load not existing file no success info
    {
        const std::string file_name          = "log/SaveLoadTest_NotExistingFile_NoSuccessInfo.txt";

        assert(IsFileExist_ASCII(file_name) == false);

        const std::string content = LoadFromFile_UTF8(file_name);
        assert(content == "");
    }
}

//==============================================================================

void Test_TTK_Assert() {
    Notice();
    // assert success
    {
        const std::string output_file_name          = "log/Out_AssertSuccess.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            SuccessAssert(is_finished);
        }
        assert(is_finished);

        const std::string fail_information = LoadFromFile_UTF8(output_file_name);

        assert(fail_information == "");
    }

    // assert success with return
    {
        const std::string output_file_name          = "log/Out_AssertSuccessWithReturn.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            assert(SuccessAssertReturn(is_finished) == 0);
        }
        assert(is_finished);

        const std::string fail_information = LoadFromFile_UTF8(output_file_name);

        assert(fail_information == "");
    }

    // assert fail
    {
        const std::string output_file_name          = "log/Out_AssertFail.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            FailAssert(is_finished);
        }
        assert(is_finished == false);

        const std::string fail_information          = LoadFromFile_UTF8(output_file_name);
        const std::string expected_fail_information = "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:14] [condition:10 == 5]\n";

        assert(fail_information == expected_fail_information);
    }

    // assert fail with message
    {
        const std::string output_file_name          = "log/Out_AssertFailWithMessage.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            FailAssertMessage(is_finished);
        }
        assert(is_finished == false);

        const std::string fail_information          = LoadFromFile_UTF8(output_file_name);
        const std::string expected_fail_information = "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:23] [condition:10 == 5] [message:Some message.]\n";

        assert(fail_information == expected_fail_information);
    }

    // assert fail with message std::string
    {
        const std::string output_file_name          = "log/Out_AssertFailWithMessage.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            FailAssertMessageString(is_finished);
        }
        assert(is_finished == false);

        const std::string fail_information          = LoadFromFile_UTF8(output_file_name);
        const std::string expected_fail_information = "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:74] [condition:10 == 5] [message:Some message.]\n";

        assert(fail_information == expected_fail_information);
    }

    // assert fail with return
    {
        const std::string output_file_name          = "log/Out_AssertFailWithReturn.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            assert(FailAssertReturn(is_finished) == ASSERT_FAIL_RETURN);
        }
        assert(is_finished == false);

        const std::string fail_information          = LoadFromFile_UTF8(output_file_name);
        const std::string expected_fail_information = "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:32] [condition:10 == 5]\n";

        assert(fail_information == expected_fail_information);
    }

    // assert fail with message and return
    {
        const std::string output_file_name          = "log/Out_AssertFailWithMessageAndReturn.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            assert(FailAssertMessageReturn(is_finished) == ASSERT_FAIL_RETURN);
        }
        assert(is_finished == false);

        const std::string fail_information          = LoadFromFile_UTF8(output_file_name);
        const std::string expected_fail_information = "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:43] [condition:10 == 5] [message:Some message.]\n";

        assert(fail_information == expected_fail_information);
    }

    // assert fail with message utf8
    {
        const std::string output_file_name          = "log/Out_AssertFailWithMessage_utf8.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            FailAssertMessageUTF8(is_finished);
        }
        assert(is_finished == false);

        const std::string fail_information          = LoadFromFile_UTF8(output_file_name);
        const std::string expected_fail_information = u8"    [fail] [file:" + GetSourceFileName_UTF8() + u8"] [line:99] [condition:10 == 5] [message:Some message\u0444.]\n";

        assert(fail_information == expected_fail_information);
    }
}

void Test_TTK_Assert_WithCodeInUnicodeFolder() {
    Notice();

    // assert fail in unicode folder
    {
        const std::string output_file_name          = "log/Out_AssertFail_InUnicodeFolder.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            FailAssertInUnicodeFolder(is_finished);
        }
        assert(is_finished == false);

        const std::string fail_information          = LoadFromFile_UTF8(output_file_name);
        const std::string file_name                 = GetSourceFileName_UTF8(u8"\\TrivialTestKit_Test\\src\\Folder\u0444\\InUnicodeFolder.h", u8"/src/Folder\u0444/InUnicodeFolder.h");
        const std::string expected_fail_information = "    [fail] [file:" + file_name + "] [line:13] [condition:10 == 5]\n";

        assert(fail_information == expected_fail_information);
    }
}

void TestDummy() {
    TTK_NotifyTest();
}

void Test_TTK_NotifyTest() {
    Notice();

    {
        const std::string output_file_name          = "log/Out_NotifyTest.txt";

        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TestDummy();
        }

        const std::string expected_fail_information = "[test] TestDummy\n";
        const std::string fail_information          = LoadFromFile_UTF8(output_file_name);

        assert(expected_fail_information == fail_information);
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

void Test_TTK_RunTests() {
    Notice();

    // success
    {    
        const std::string output_file_name          = "log/Out_RunTestsSuccess.txt";

        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TTK_SetIsAbortAtFail(true);

            const bool is_success = TTK_RunTests({
                TestA,
                TestB,
                TestC
            });
            assert(is_success);
        }

        const std::string expected_fail_information = 
            "--- TEST ---\n"
            "[test] TestA\n"
            "[test] TestB\n"
            "[test] TestC\n"
            "--- TEST SUCCESS ---\n"
            "number of runned notified tests : 3\n"
            "number of failed tests          : 0\n";
        const std::string fail_information          = LoadFromFile_UTF8(output_file_name);

        assert(expected_fail_information == fail_information);
    }

    // fail
    {   
        const std::string output_file_name          = "log/Out_RunTestsFail.txt";

        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TTK_SetIsAbortAtFail(true);

            const bool is_success = TTK_RunTests({
                TestA,
                TestB,
                TestD_Fail,
                TestC
                });
            assert(!is_success);
        }

        const std::string expected_fail_information  = 
            "--- TEST ---\n"
            "[test] TestA\n"
            "[test] TestB\n"
            "[test] TestD_Fail\n"
            "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:92] [condition:5 == 7]\n"
            "--- TEST FAIL ---\n"
            "number of runned notified tests : 3\n"
            "number of failed tests          : 1\n";

        const std::string fail_information      = LoadFromFile_UTF8(output_file_name);

        assert(expected_fail_information == fail_information);
    }

    // fail without abort
    {   
        const std::string output_file_name      = "log/Out_RunTestsFailNoAbort.txt";

        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TTK_SetIsAbortAtFail(false);

            const bool is_success = TTK_RunTests({
                TestA,
                TestB,
                TestD_Fail,
                TestC
                });
            assert(!is_success);
        }

        const std::string expected_fail_information  = 
            "--- TEST ---\n"
            "[test] TestA\n"
            "[test] TestB\n"
            "[test] TestD_Fail\n"
            "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:92] [condition:5 == 7]\n"
            "[test] TestC\n"
            "--- TEST FAIL ---\n"
            "number of runned notified tests : 4\n"
            "number of failed tests          : 1\n";

        const std::string fail_information           = LoadFromFile_UTF8(output_file_name);

        assert(expected_fail_information == fail_information);
    }

    // fail mixed
    {   
        const std::string output_file_name      = "log/Out_RunTestsFailMixed.txt";

        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetOutput(output.Access());

            TTK_SetIsAbortAtFail(true);

            const bool is_success = TTK_RunTests({
                TTK_DisableAbortAtFail,     // <- disable abort
                TestA,
                TestD_Fail,
                TTK_EnableAbortAtFail,      // <- enable abort
                TestB,
                TestD_Fail,
                TestC
                });
            assert(!is_success);
        }

        const std::string expected_fail_information  = 
            "--- TEST ---\n"
            "[test] TestA\n"
            "[test] TestD_Fail\n"
            "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:92] [condition:5 == 7]\n"
            "[test] TestB\n"
            "[test] TestD_Fail\n"
            "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:92] [condition:5 == 7]\n"
            "--- TEST FAIL ---\n"
            "number of runned notified tests : 4\n"
            "number of failed tests          : 2\n";

        const std::string fail_information           = LoadFromFile_UTF8(output_file_name);

        assert(expected_fail_information == fail_information);
    }
}

void RunAllTests() {
    if (IsWideOriented()) {
        _wsystem(L"if exist log @rd /S /Q log");
        _wsystem(L"mkdir log");

        wprintf(L"%hs\n", "--- Wide Oriented ---");
        wprintf(L"sizeof(void*)=%d\n", (int)sizeof(void*));

        wprintf(L"%hs\n", "--- Test Begin ---");

        TTK_ForceOutputOrientation(1);
    } else {
        system("if exist log @rd /S /Q log");
        system("mkdir log");

        puts("--- Narrow Oriented  ---");
        printf("sizeof(void*)=%d\n", (int)sizeof(void*));

        puts("--- Test Begin ---");

        TTK_ForceOutputOrientation(-1);
    }

    Test_ToUTF16();
    Test_ToUTF8();
    Test_IsFileExist_ASCII();
    Test_LoadAndSaveToFile();
    Test_TTK_Assert();
#ifndef NO_TEST_IN_UNICODE_FOLDER
    Test_TTK_Assert_WithCodeInUnicodeFolder();
#endif
    Test_TTK_NotifyTest();
    Test_TTK_RunTests();

    if (IsWideOriented()) {
        wprintf(L"%hs\n", "--- Test End ---");
    } else {
        puts("--- Test End ---");
    }
}