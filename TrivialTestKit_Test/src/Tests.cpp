#include "Tests.h"

#include "TrivialTestKit.h"
#include "Support.h"

// Not all compilers are able to accept unicode characters from command argument list.
#ifndef NO_TEST_IN_UNICODE_FOLDER
#include "InUnicodeFolder.h"
#endif

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

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
uint64_t g_test_finish_counter = 0;
uint64_t g_line[MAX_REG_LINES] = {};

void Test_TTK_NoTest() {
    Notice();

    const std::string output_file_name = "log/Out_NoTest.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_Run();
        TTK_Free();
    }

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "--- TEST SUCCESS ---\n"
        "number of executed asserts      : 0\n"
        "number of failed asserts        : 0\n"
        "number of executed tests        : 0\n"
        "number of failed tests          : 0\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void EmptyTest() {
    g_test_finish_counter += 1;
}

void Test_TTK_EmptyTest() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_EmptyTest.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(EmptyTest, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] EmptyTest\n"
        "--- TEST SUCCESS ---\n"
        "number of executed asserts      : 0\n"
        "number of failed asserts        : 0\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 0\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void AssertSuccess() {
    TTK_ASSERT(10 == 10);
    TTK_ASSERT_M(4 > 0, "Some message.");
    TTK_ASSERT(2 == 2);
    TTK_ASSERT_M(true, "Some message.");

    g_test_finish_counter += 1;
}

void Test_TTK_AssertSuccess() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_AssertSuccess.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(AssertSuccess, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);
    
    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] AssertSuccess\n"
        "--- TEST SUCCESS ---\n"
        "number of executed asserts      : 4\n"
        "number of failed asserts        : 0\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 0\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void AssertSuccessString() {
    TTK_ASSERT(10 == 10);
    TTK_ASSERT_M(4 > 0, std::string("Some message."));
    TTK_ASSERT(2 == 2);
    TTK_ASSERT_M(true, std::string("Some message."));

    g_test_finish_counter += 1;
}

void Test_TTK_AssertSuccessString() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_AssertSuccessString.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(AssertSuccessString, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] AssertSuccessString\n"
        "--- TEST SUCCESS ---\n"
        "number of executed asserts      : 4\n"
        "number of failed asserts        : 0\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 0\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void AssertSuccessUTF8() {
    TTK_ASSERT(10 == 10);
    TTK_ASSERT_M(4 > 0, u8"Some message.");
    TTK_ASSERT(2 == 2);
    TTK_ASSERT_M(true, u8"Some message.");

    g_test_finish_counter += 1;
}

void Test_TTK_AssertSuccessUTF8() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_AssertSuccessUTF8.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(AssertSuccessUTF8, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] AssertSuccessUTF8\n"
        "--- TEST SUCCESS ---\n"
        "number of executed asserts      : 4\n"
        "number of failed asserts        : 0\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 0\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void AssertFail() {
    TTK_ASSERT(4 == 4);
    g_line[0] = __LINE__ + 1;
    TTK_ASSERT(10 != 10);
    TTK_ASSERT(false);
    TTK_ASSERT(4 > 0);

    g_test_finish_counter += 1;
}

void Test_TTK_AssertFail() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_AssertFail.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(AssertFail, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 0);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] AssertFail\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:10 != 10]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 1\n"
        "number of failed asserts        : 1\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void Test_TTK_AssertFailInUnicodeFolder() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_AssertFailInUnicodeFolder.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(AssertFailInUnicodeFolder, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 0);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    // Note: Watch out. This is a hack. CMake replaces back slashed to forward slashes if assert happens in .h file instead in .cpp file. 
    // And CMake can't process unicode characters in path to .cpp files, but don't have this problem with path to include folders.
    // To disable this test, build with NO_TEST_IN_UNICODE_FOLDER flag.
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] AssertFailInUnicodeFolder\n"                    
        "    [fail] [file:" + GetSourceFileName_UTF8(!IsMSVC(), u8"\\TrivialTestKit_Test\\src\\Folder\u0444\\InUnicodeFolder.h") + "] [line:" + std::to_string(g_line[0]) + "] [condition:10 != 10]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 1\n"
        "number of failed asserts        : 1\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}


void AssertFailMessage() {
    TTK_ASSERT_M(4 == 4, "Message 1.");
    g_line[0] = __LINE__ + 1;
    TTK_ASSERT_M(10 != 10, "Message 2.");
    TTK_ASSERT_M(false, "Message 3.");
    TTK_ASSERT_M(4 > 0, "Message 4.");

    g_test_finish_counter += 1;
}

void Test_TTK_AssertFailMessage() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_AssertFailMessage.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(AssertFailMessage, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 0);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] AssertFailMessage\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:10 != 10] [message:Message 2.]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 1\n"
        "number of failed asserts        : 1\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void AssertFailMessageString() {
    TTK_ASSERT_M(4 == 4, std::string("Message 1."));
    g_line[0] = __LINE__ + 1;
    TTK_ASSERT_M(10 != 10, std::string("Message 2."));
    TTK_ASSERT_M(false, std::string("Message 3."));
    TTK_ASSERT_M(4 > 0, std::string("Message 4."));

    g_test_finish_counter += 1;
}

void Test_TTK_AssertFailMessageString() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_AssertFailMessageString.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(AssertFailMessageString, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 0);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] AssertFailMessageString\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:10 != 10] [message:Message 2.]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 1\n"
        "number of failed asserts        : 1\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void AssertFailMessageUTF8() {
    TTK_ASSERT_M(4 == 4, u8"Message 1.");
    g_line[0] = __LINE__ + 1;
    TTK_ASSERT_M(10 != 10, u8"Message 2.");
    TTK_ASSERT_M(false, u8"Message 3.");
    TTK_ASSERT_M(4 > 0, u8"Message 4.");

    g_test_finish_counter += 1;
}

void Test_TTK_AssertFailMessageUTF8() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_AssertFailMessageUTF8.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(AssertFailMessageUTF8, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 0);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] AssertFailMessageUTF8\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:10 != 10] [message:Message 2.]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 1\n"
        "number of failed asserts        : 1\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}


void ExpectSuccess() {
    TTK_EXPECT(10 == 10);
    TTK_EXPECT_M(4 > 0, "Some message.");
    TTK_EXPECT(2 == 2);
    TTK_EXPECT_M(true, "Some message.");

    g_test_finish_counter += 1;
}

void Test_TTK_ExpectSuccess() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_ExpectSuccess.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(ExpectSuccess, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] ExpectSuccess\n"
        "--- TEST SUCCESS ---\n"
        "number of executed asserts      : 4\n"
        "number of failed asserts        : 0\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 0\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void ExpectSuccessString() {
    TTK_EXPECT(10 == 10);
    TTK_EXPECT_M(4 > 0, std::string("Some message."));
    TTK_EXPECT(2 == 2);
    TTK_EXPECT_M(true, std::string("Some message."));

    g_test_finish_counter += 1;
}

void Test_TTK_ExpectSuccessString() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_ExpectSuccessString.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(ExpectSuccessString, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] ExpectSuccessString\n"
        "--- TEST SUCCESS ---\n"
        "number of executed asserts      : 4\n"
        "number of failed asserts        : 0\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 0\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void ExpectSuccessUTF8() {
    TTK_EXPECT(10 == 10);
    TTK_EXPECT_M(4 > 0, u8"Some message.");
    TTK_EXPECT(2 == 2);
    TTK_EXPECT_M(true, u8"Some message.");

    g_test_finish_counter += 1;
}

void Test_TTK_ExpectSuccessUTF8() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_ExpectSuccessUTF8.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(ExpectSuccessUTF8, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] ExpectSuccessUTF8\n"
        "--- TEST SUCCESS ---\n"
        "number of executed asserts      : 4\n"
        "number of failed asserts        : 0\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 0\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void ExpectFail() {
    TTK_EXPECT(4 == 4);
    g_line[0] = __LINE__ + 1;
    TTK_EXPECT(10 != 10);
    g_line[1] = __LINE__ + 1;
    TTK_EXPECT(false);
    TTK_EXPECT(4 > 0);

    g_test_finish_counter += 1;
}

void Test_TTK_ExpectFail() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_ExpectFail.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(ExpectFail, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] ExpectFail\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:10 != 10]\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[1]) + "] [condition:false]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 4\n"
        "number of failed asserts        : 2\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void Test_TTK_ExpectFailInUnicodeFolder() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_ExpectFailInUnicodeFolder.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(ExpectFailInUnicodeFolder, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    // Note: Watch out. This is a hack. CMake replaces back slashed to forward slashes if assert happens in .h file instead in .cpp file. 
    // And CMake can't process unicode characters in path to .cpp files, but don't have this problem with path to include folders.
    // To disable this test, run test with third flag as NO_TEST_IN_UNICODE_FOLDER. Example: TrivialTestKit_Test.exe - - NO_TEST_IN_UNICODE_FOLDER.
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] ExpectFailInUnicodeFolder\n"
        "    [fail] [file:" + GetSourceFileName_UTF8(!IsMSVC(), u8"\\TrivialTestKit_Test\\src\\Folder\u0444\\InUnicodeFolder.h") + "] [line:" + std::to_string(g_line[0]) + "] [condition:10 != 10]\n"
        "    [fail] [file:" + GetSourceFileName_UTF8(!IsMSVC(), u8"\\TrivialTestKit_Test\\src\\Folder\u0444\\InUnicodeFolder.h") + "] [line:" + std::to_string(g_line[1]) + "] [condition:false]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 3\n"
        "number of failed asserts        : 2\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void ExpectFailMessage() {
    TTK_EXPECT_M(4 == 4, "Message 1.");
    g_line[0] = __LINE__ + 1;
    TTK_EXPECT_M(10 != 10, "Message 2.");
    g_line[1] = __LINE__ + 1;
    TTK_EXPECT_M(false, "Message 3.");
    TTK_EXPECT_M(4 > 0, "Message 4.");

    g_test_finish_counter += 1;
}

void Test_TTK_ExpectFailMessage() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_ExpectFailMessage.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(ExpectFailMessage, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] ExpectFailMessage\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:10 != 10] [message:Message 2.]\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[1]) + "] [condition:false] [message:Message 3.]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 4\n"
        "number of failed asserts        : 2\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void ExpectFailMessageString() {
    TTK_EXPECT_M(4 == 4, std::string("Message 1."));
    g_line[0] = __LINE__ + 1;
    TTK_EXPECT_M(10 != 10, std::string("Message 2."));
    g_line[1] = __LINE__ + 1;
    TTK_EXPECT_M(false, std::string("Message 3."));
    TTK_EXPECT_M(4 > 0, std::string("Message 4."));

    g_test_finish_counter += 1;
}

void Test_TTK_ExpectFailMessageString() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_ExpectFailMessageString.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(ExpectFailMessageString, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] ExpectFailMessageString\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:10 != 10] [message:Message 2.]\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[1]) + "] [condition:false] [message:Message 3.]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 4\n"
        "number of failed asserts        : 2\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void ExpectFailMessageUTF8() {
    TTK_EXPECT_M(4 == 4, u8"Message 1.");
    g_line[0] = __LINE__ + 1;
    TTK_EXPECT_M(10 != 10, u8"Message 2.");
    g_line[1] = __LINE__ + 1;
    TTK_EXPECT_M(false, u8"Message 3.");
    TTK_EXPECT_M(4 > 0, u8"Message 4.");

    g_test_finish_counter += 1;
}

void Test_TTK_ExpectFailMessageUTF8() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_ExpectFailMessageUTF8.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(ExpectFailMessageUTF8, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] ExpectFailMessageUTF8\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:10 != 10] [message:Message 2.]\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[1]) + "] [condition:false] [message:Message 3.]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 4\n"
        "number of failed asserts        : 2\n"
        "number of executed tests        : 1\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}


void TestAssertSuccessA() {
    TTK_ASSERT(1 == 1);
    TTK_ASSERT_M(2 == 2, "Message 1.");
    TTK_ASSERT_M(3 == 3, u8"Message 2.");
    TTK_ASSERT_M(4 == 4, std::string("Message 3."));

    g_test_finish_counter += 1;
}

void TestAssertSuccessB() {
    TTK_ASSERT(10 == 10);
    TTK_ASSERT_M(20 == 20, "Message 1.");
    TTK_ASSERT_M(30 == 30, u8"Message 2.");
    TTK_ASSERT_M(40 == 40, std::string("Message 3."));

    g_test_finish_counter += 1;
}

void TestAssertFail() {
    TTK_EXPECT(100 == 100);
    g_line[0] = __LINE__ + 1;
    TTK_ASSERT_M(200 > 200, "Message 1.");
    g_line[1] = __LINE__ + 1;
    TTK_ASSERT_M(300 > 300, u8"Message 2.");
    TTK_ASSERT_M(400 == 400, std::string("Message 3."));

    g_test_finish_counter += 1;
}

void TestExpectSuccessA() {
    TTK_EXPECT(1 == 1);
    TTK_EXPECT_M(2 == 2, "Message 1.");
    TTK_EXPECT_M(3 == 3, u8"Message 2.");
    TTK_EXPECT_M(4 == 4, std::string("Message 3."));

    g_test_finish_counter += 1;
}

void TestExpectSuccessB() {
    TTK_EXPECT(10 == 10);
    TTK_EXPECT_M(20 == 20, "Message 1.");
    TTK_EXPECT_M(30 == 30, u8"Message 2.");
    TTK_EXPECT_M(40 == 40, std::string("Message 3."));

    g_test_finish_counter += 1;
}

void TestExpectFail() {
    TTK_EXPECT(100 == 100);
    g_line[10] = __LINE__ + 1;
    TTK_EXPECT_M(200 > 200, "Message 1.");
    g_line[11] = __LINE__ + 1;
    TTK_EXPECT_M(300 > 300, u8"Message 2.");
    TTK_EXPECT_M(400 == 400, std::string("Message 3."));

    g_test_finish_counter += 1;
}

void Test_TTK_RunSuccess() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_RunSuccess.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(TestAssertSuccessA, 0);
        TTK_ADD_TEST(TestAssertSuccessB, 0);
        TTK_ADD_TEST(TestExpectSuccessA, 0);
        TTK_ADD_TEST(TestExpectSuccessB, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 4);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] TestAssertSuccessA\n"
        "[test] TestAssertSuccessB\n"
        "[test] TestExpectSuccessA\n"
        "[test] TestExpectSuccessB\n"
        "--- TEST SUCCESS ---\n"
        "number of executed asserts      : 16\n"
        "number of failed asserts        : 0\n"
        "number of executed tests        : 4\n"
        "number of failed tests          : 0\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void Test_TTK_RunFail() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_RunSuccess.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(TestAssertSuccessA, 0);
        TTK_ADD_TEST(TestAssertFail, 0);
        TTK_ADD_TEST(TestAssertSuccessB, 0);
        TTK_ADD_TEST(TestExpectSuccessA, 0);
        TTK_ADD_TEST(TestExpectSuccessB, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 1);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] TestAssertSuccessA\n"
        "[test] TestAssertFail\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:200 > 200] [message:Message 1.]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 5\n"
        "number of failed asserts        : 1\n"
        "number of executed tests        : 2\n"
        "number of failed tests          : 1\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void Test_TTK_RunFailNoAbort() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_RunSuccess.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(TestAssertSuccessA, 0);
        TTK_ADD_TEST(TestAssertFail, TTK_NO_ABORT);
        TTK_ADD_TEST(TestAssertSuccessB, 0);
        TTK_ADD_TEST(TestAssertFail, 0);
        TTK_ADD_TEST(TestExpectSuccessA, 0);
        TTK_ADD_TEST(TestExpectSuccessB, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 2);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] TestAssertSuccessA\n"
        "[test] TestAssertFail\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:200 > 200] [message:Message 1.]\n"
        "[test] TestAssertSuccessB\n"
        "[test] TestAssertFail\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:200 > 200] [message:Message 1.]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 10\n"
        "number of failed asserts        : 2\n"
        "number of executed tests        : 4\n"
        "number of failed tests          : 2\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void Test_TTK_RunFailNoAbort2() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_RunSuccess.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(TestAssertSuccessA, 0);
        TTK_ADD_TEST(TestExpectFail, TTK_NO_ABORT);
        TTK_ADD_TEST(TestAssertSuccessB, 0);
        TTK_ADD_TEST(TestExpectFail, 0);
        TTK_ADD_TEST(TestExpectSuccessA, 0);
        TTK_ADD_TEST(TestExpectSuccessB, 0);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 4);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] TestAssertSuccessA\n"
        "[test] TestExpectFail\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[10]) + "] [condition:200 > 200] [message:Message 1.]\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[11]) + "] [condition:300 > 300] [message:Message 2.]\n"
        "[test] TestAssertSuccessB\n"
        "[test] TestExpectFail\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[10]) + "] [condition:200 > 200] [message:Message 1.]\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[11]) + "] [condition:300 > 300] [message:Message 2.]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 16\n"
        "number of failed asserts        : 4\n"
        "number of executed tests        : 4\n"
        "number of failed tests          : 2\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void Test_TTK_RunSuccessDisable() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_RunSuccess.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_ADD_TEST(TestAssertSuccessA, 0);
        TTK_ADD_TEST(TestAssertSuccessB, TTK_DISABLE);
        TTK_ADD_TEST(TestExpectSuccessA, 0);
        TTK_ADD_TEST(TestExpectSuccessB, TTK_DISABLE);
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 2);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] TestAssertSuccessA\n"
        "[test] TestExpectSuccessA\n"
        "--- TEST SUCCESS ---\n"
        "number of executed asserts      : 8\n"
        "number of failed asserts        : 0\n"
        "number of executed tests        : 2\n"
        "number of failed tests          : 0\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}


TTK_TEST(TestAssertSuccessA_InPlace, 0) {
    TTK_ASSERT(1 == 1);
    TTK_ASSERT_M(2 == 2, "Message 1.");
    TTK_ASSERT_M(3 == 3, u8"Message 2.");
    TTK_ASSERT_M(4 == 4, std::string("Message 3."));

    g_test_finish_counter += 1;
}

TTK_TEST(TestAssertSuccessB_InPlace, TTK_NO_ABORT | TTK_DISABLE) {
    TTK_ASSERT(10 == 10);
    TTK_ASSERT_M(20 == 20, "Message 1.");
    TTK_ASSERT_M(30 == 30, u8"Message 2.");
    TTK_ASSERT_M(40 == 40, std::string("Message 3."));

    g_test_finish_counter += 1;
}

TTK_TEST(TestAssertFail_InPlace, TTK_NO_ABORT) {
    TTK_EXPECT(100 == 100);
    g_line[0] = __LINE__ + 1;
    TTK_ASSERT_M(200 > 200, "Message 1.");
    g_line[1] = __LINE__ + 1;
    TTK_ASSERT_M(300 > 300, u8"Message 2.");
    TTK_ASSERT_M(400 == 400, std::string("Message 3."));

    g_test_finish_counter += 1;
}

TTK_TEST(TestExpectSuccessA_InPlace, 0) {
    TTK_EXPECT(1 == 1);
    TTK_EXPECT_M(2 == 2, "Message 1.");
    TTK_EXPECT_M(3 == 3, u8"Message 2.");
    TTK_EXPECT_M(4 == 4, std::string("Message 3."));

    g_test_finish_counter += 1;
}

TTK_TEST(TestExpectSuccessB_InPlace, 0) {
    TTK_EXPECT(10 == 10);
    TTK_EXPECT_M(20 == 20, "Message 1.");
    TTK_EXPECT_M(30 == 30, u8"Message 2.");
    TTK_EXPECT_M(40 == 40, std::string("Message 3."));

    g_test_finish_counter += 1;
}

TTK_TEST(TestExpectFail_InPlace, 0) {
    TTK_EXPECT(100 == 100);
    g_line[10] = __LINE__ + 1;
    TTK_EXPECT_M(200 > 200, "Message 1.");
    g_line[11] = __LINE__ + 1;
    TTK_EXPECT_M(300 > 300, u8"Message 2.");
    TTK_EXPECT_M(400 == 400, std::string("Message 3."));

    g_test_finish_counter += 1;
}

void Test_TTK_RunFail_InPlace() {
    Notice();

    g_test_finish_counter = 0;
    const std::string output_file_name = "log/Out_RunFail_InPlace.txt";
    {
        Output output = Output(output_file_name);

        TTK_SetOutput(output.Access());
        TTK_Run();
        TTK_Free();
    }
    assert(g_test_finish_counter == 4);

    const std::string output_contnet = LoadFromFile_UTF8(output_file_name);
    const std::string expected_output_contnet = 
        "--- TEST ---\n"
        "[test] TestAssertSuccessA_InPlace\n"
        "[test] TestAssertFail_InPlace\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[0]) + "] [condition:200 > 200] [message:Message 1.]\n"
        "[test] TestExpectSuccessA_InPlace\n"
        "[test] TestExpectSuccessB_InPlace\n"
        "[test] TestExpectFail_InPlace\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[10]) + "] [condition:200 > 200] [message:Message 1.]\n"
        "    [fail] [file:" + GetSourceFileName_UTF8() + "] [line:" + std::to_string(g_line[11]) + "] [condition:300 > 300] [message:Message 2.]\n"
        "--- TEST FAIL ---\n"
        "number of executed asserts      : 17\n"
        "number of failed asserts        : 3\n"
        "number of executed tests        : 5\n"
        "number of failed tests          : 2\n";
    PrintIfMissmatch(output_contnet, expected_output_contnet);
    assert(output_contnet == expected_output_contnet);
}

void RunAllTests(int argc, char* argv[]) {

    for (int ix = 0; ix < argc; ++ix) puts(argv[ix]);

    auto IsFlag = [&argc, &argv](int index, const char* flag_name) {
        return argc > index && strcmp(argv[index], flag_name) == 0;
    };

    if (IsFlag(1, "WIDE")) SwitchStdOutToWideOriented();

    if (IsStdOutWideOriented()) {
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

    if (IsFlag(2, "IN_PLACE")) {
        Test_TTK_RunFail_InPlace();
    } else {
        TTK_Free();

        Test_TTK_NoTest();
        Test_TTK_EmptyTest();
        Test_TTK_AssertSuccess();
        Test_TTK_AssertSuccessString();
        Test_TTK_AssertSuccessUTF8();
        Test_TTK_AssertFail();

        if (!IsFlag(3, "NO_TEST_IN_UNICODE_FOLDER")) Test_TTK_AssertFailInUnicodeFolder();

        Test_TTK_AssertFailMessage();
        Test_TTK_AssertFailMessageString();
        Test_TTK_AssertFailMessageUTF8();
        Test_TTK_ExpectSuccess();
        Test_TTK_ExpectSuccessString();
        Test_TTK_ExpectSuccessUTF8();
        Test_TTK_ExpectFail();

        if (!IsFlag(3, "NO_TEST_IN_UNICODE_FOLDER")) Test_TTK_ExpectFailInUnicodeFolder();

        Test_TTK_ExpectFailMessage();
        Test_TTK_ExpectFailMessageString();
        Test_TTK_ExpectFailMessageUTF8();
        Test_TTK_RunSuccess();
        Test_TTK_RunFail();
        Test_TTK_RunFailNoAbort();
        Test_TTK_RunFailNoAbort2();
        Test_TTK_RunSuccessDisable();
    }

    if (IsStdOutWideOriented()) {
        wprintf(L"%hs\n", "--- Test End ---");
    } else {
        puts("--- Test End ---");
    }
}