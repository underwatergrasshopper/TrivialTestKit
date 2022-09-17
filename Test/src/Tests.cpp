#include "Tests.h"

#include "TrivialTestKit.h"
#include "AssertFails.h"

#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>

void SystemMakeFile(const std::string& file_name) {
    const std::string command = std::string() + "if not exist \"" + file_name + "\" echo. > \"" + file_name + "\"";
    system(command.c_str());
}

void SystemMakeFile(const std::wstring& file_name) {
    const std::wstring command = std::wstring() + L"if not exist \"" + file_name + L"\" echo. > \"" + file_name + L"\"";
    _wsystem(command.c_str());
}

void SystemMakeFolder(const std::string& folder_name) {
    const std::string command = std::string() + "if not exist \"" + folder_name + "\" md \"" + folder_name + "\"";
    system(command.c_str());
}

void SystemMakeFolder(const std::wstring& folder_name) {
    const std::wstring command = std::wstring() + L"if not exist \"" + folder_name + L"\" md \"" + folder_name + L"\"";
    _wsystem(command.c_str());
}

class Output {
public:
    Output() : m_out(nullptr) {}

    explicit Output(const std::wstring& file_name) : m_out(nullptr) {
        if (_wfopen_s(&m_out, file_name.c_str(), L"w") != 0) m_out = nullptr;
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

inline std::wstring GetDefSolutionBuildDirCutOff() {
#ifdef _DEBUG

#ifdef WIN32
    return L"\\Debug";
#else
    return L"\\x64\\Debug";
#endif

#else

#ifdef WIN32
    return L"\\Release";
#else
    return L"\\x64\\Release";
#endif

#endif
}

inline std::wstring GetDefSolutionDir() {
    std::wstring cwd = TTK_GetCWD_UTF16();
    auto pos = cwd.find(GetDefSolutionBuildDirCutOff());
    return cwd.substr(0, pos);
}

//==============================================================================

void TestTTK_IsFileExist() {
    puts(__func__);

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
        const std::string folder_url = "log/ExistingFolder";
        SystemMakeFolder(folder_url);
        assert(TTK_IsFileExist(folder_url) == false);
    }

    // utf16 existing
    {
        const std::wstring file_url = L"log/ExistingFile\u0444.txt";
        SystemMakeFile(file_url);
        assert(TTK_IsFileExist(file_url));
    }

    // utf16 not existing
    {
        assert(TTK_IsFileExist(L"log/NotExistingFile\u0444.txt") == false);
    }

    // utf16 wrong type
    {
        const std::wstring folder_url = L"log/ExistingFolder\u0444";
        SystemMakeFolder(folder_url);
        assert(TTK_IsFileExist(folder_url) == false);
    }
}

void TestTTK_IsFolderExist() {
    puts(__func__);

    // existing
    {
        const std::string folder_url = "log/ExistingFolder";
        SystemMakeFolder(folder_url);
        assert(TTK_IsFolderExist(folder_url));
    }

    // not existing
    {
        assert(TTK_IsFolderExist("log/NonExistingFile") == false);
    }

    // wrong type
    {
        const std::string file_url = "log/ExistingFile.txt";
        SystemMakeFile(file_url);
        assert(TTK_IsFolderExist(file_url) == false);
    }

    // utf16 existing
    {
        const std::wstring folder_url = L"log/ExistingFolder\u0444";
        SystemMakeFolder(folder_url);
        assert(TTK_IsFolderExist(folder_url));
    }

    // utf16 not existing
    {
        assert(TTK_IsFolderExist(L"log/NonExistingFile\u0444") == false);
    }

    // utf16 wrong type
    {
        const std::wstring file_url = L"log/ExistingFile.txt";
        SystemMakeFile(file_url);
        assert(TTK_IsFolderExist(file_url) == false);
    }
}

void TestTTK_DeleteFile() {
    puts(__func__);

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

    // utf16 existing
    {
        const std::wstring file_url = L"log/FileToDelete\u0444.txt";
        SystemMakeFile(file_url);
        assert(TTK_IsFileExist(file_url));
        assert(TTK_DeleteFile(file_url));
        assert(TTK_IsFileExist(file_url) == false);
    }

    // utf16 not existing
    {
        const std::wstring file_url = L"log/NotExistingFileToDelete\u0444.txt";
        assert(TTK_DeleteFile(file_url) == false);
    }
}

void TestTTK_LoadSaveFileContent() {
    puts(__func__);

    // existing file
    {
        const std::string file_name          = "log/SaveLoadTest.txt";
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
        const std::string file_name          = "log/SaveLoadTest.txt";
        const std::string expected_content  = "Some text to save.\nAnother line.\n";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, expected_content));

        const std::string content = TTK_LoadFromFile(file_name);
        assert(content == expected_content);
    }

    // overriding file
    {
        const std::string file_name          = "log/SaveLoadTest.txt";
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
        const std::string file_name          = "log/NotExistingSaveLoadTest.txt";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        bool is_success = false;
        const std::string content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success == false);
        assert(content == "");
    }

    // load not existing file no success info
    {
        const std::string file_name          = "log/NotExistingSaveLoadTest.txt";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        const std::string content = TTK_LoadFromFile(file_name);
        assert(content == "");
    }

    // === utf16 === //

    // existing file
    {
        const std::wstring file_name          = L"log/SaveLoadTest\u0444.txt";
        const std::wstring expected_content  = L"Some text to save.\nAnother line.\n";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, expected_content));

        bool is_success = false;
        const std::wstring content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }

    // existing file no success info
    {
        const std::wstring file_name          = L"log/SaveLoadTest\u0444.txt";
        const std::wstring expected_content  = L"Some text to save.\nAnother line.\n";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, expected_content));

        const std::wstring content = TTK_LoadFromFile(file_name);
        assert(content == expected_content);
    }

    // overriding file
    {
        const std::wstring file_name          = L"log/SaveLoadTest\u0444.txt";
        const std::wstring initial_content   = L"Initial text to save.";
        const std::wstring expected_content  = L"Some text to save.\nAnother line.\n";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        assert(TTK_SaveToFile(file_name, initial_content));
        assert(TTK_SaveToFile(file_name, expected_content));

        bool is_success = false;
        const std::wstring content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }

    // load not existing file
    {
        const std::wstring file_name          = L"log/NotExistingSaveLoadTest\u0444.txt";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        bool is_success = false;
        const std::wstring content = TTK_LoadFromFile(file_name, &is_success);
        assert(is_success == false);
        assert(content == L"");
    }

    // load not existing file no success info
    {
        const std::wstring file_name          = L"log/NotExistingSaveLoadTest\u0444.txt";

        TTK_DeleteFile(file_name);
        assert(TTK_IsFileExist(file_name) == false);

        const std::wstring content = TTK_LoadFromFile(file_name);
        assert(content == L"");
    }
}

void TestTTK_Assert() {
    puts(__func__);

    const std::wstring output_file_name = L"log/Out.txt";

    // assert success
    {
        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetCommunitationOutput(output.Access());

            SuccessAssert(is_finished);
        }
        assert(is_finished);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(communitate == L"");
    }

    // assert success with return
    {
        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetCommunitationOutput(output.Access());

            assert(SuccessAssertReturn(is_finished) == 0);
        }
        assert(is_finished);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        assert(communitate == L"");
    }

    // assert fail
    {
        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetCommunitationOutput(output.Access());

            FailAssert(is_finished);
        }
        assert(is_finished == false);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        const std::wstring source_file_name = GetDefSolutionDir() + L"\\Test\\src\\AssertFails.h";
        const std::wstring expected_communitate = L"    [fail] [line:14] [file:" + source_file_name + L"] [condition:10 == 5]\n";

        assert(communitate == expected_communitate);
    }

    // assert fail message
    {
        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetCommunitationOutput(output.Access());

            FailAssertMessage(is_finished);
        }
        assert(is_finished == false);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        const std::wstring source_file_name = GetDefSolutionDir() + L"\\Test\\src\\AssertFails.h";
        const std::wstring expected_communitate = L"    [fail] [line:23] [file:" + source_file_name + L"] [condition:10 == 5] [message:Some message.]\n";

        assert(communitate == expected_communitate);
    }

    // assert fail return
    {
        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetCommunitationOutput(output.Access());

            assert(FailAssertReturn(is_finished) == ASSERT_FAIL_RETURN);
        }
        assert(is_finished == false);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        const std::wstring source_file_name = GetDefSolutionDir() + L"\\Test\\src\\AssertFails.h";
        const std::wstring expected_communitate = L"    [fail] [line:32] [file:" + source_file_name + L"] [condition:10 == 5]\n";

        assert(communitate == expected_communitate);
    }

    // assert fail message return
    {
        bool is_finished = true;
        {
            Output output = Output(output_file_name);
            assert(output.Access());

            TTK_SetCommunitationOutput(output.Access());

            assert(FailAssertMessageReturn(is_finished) == ASSERT_FAIL_RETURN);
        }
        assert(is_finished == false);

        const std::wstring communitate = TTK_LoadFromFile(output_file_name);

        const std::wstring source_file_name = GetDefSolutionDir() + L"\\Test\\src\\AssertFails.h";
        const std::wstring expected_communitate = L"    [fail] [line:43] [file:" + source_file_name + L"] [condition:10 == 5] [message:Some message.]\n";

        assert(communitate == expected_communitate);
    }
}

void RunAllTests() {
    system("if not exist log mkdir log");

    TestTTK_IsFileExist();
    TestTTK_IsFolderExist();
    TestTTK_DeleteFile();
    TestTTK_LoadSaveFileContent();
    TestTTK_Assert();
}