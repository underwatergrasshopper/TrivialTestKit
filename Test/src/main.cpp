#include "TrivialTestKit.h"

#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>

void SystemMakeFile(const std::string& filename) {
    const std::string command = std::string() + "if not exist \"" + filename + "\" echo. > \"" + filename + "\"";
    system(command.c_str());
}

void SystemMakeFile(const std::wstring& filename) {
    const std::wstring command = std::wstring() + L"if not exist \"" + filename + L"\" echo. > \"" + filename + L"\"";
    _wsystem(command.c_str());
}

void SystemMakeFolder(const std::string& foldername) {
    const std::string command = std::string() + "if not exist \"" + foldername + "\" md \"" + foldername + "\"";
    system(command.c_str());
}

void SystemMakeFolder(const std::wstring& foldername) {
    const std::wstring command = std::wstring() + L"if not exist \"" + foldername + L"\" md \"" + foldername + L"\"";
    _wsystem(command.c_str());
}

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
        const std::string filename          = "log/SaveLoadTest.txt";
        const std::string expected_content  = "Some text to save.\nAnother line.\n";

        TTK_DeleteFile(filename);
        assert(TTK_IsFileExist(filename) == false);

        assert(TTK_SaveContentToFile(filename, expected_content));

        bool is_success = false;
        const std::string content = TKK_LoadContentFromFile(filename, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }

    // existing file no success info
    {
        const std::string filename          = "log/SaveLoadTest.txt";
        const std::string expected_content  = "Some text to save.\nAnother line.\n";

        TTK_DeleteFile(filename);
        assert(TTK_IsFileExist(filename) == false);

        assert(TTK_SaveContentToFile(filename, expected_content));

        const std::string content = TKK_LoadContentFromFile(filename);
        assert(content == expected_content);
    }

    // overriding file
    {
        const std::string filename          = "log/SaveLoadTest.txt";
        const std::string initial_content   = "Initial text to save.";
        const std::string expected_content  = "Some text to save.\nAnother line.\n";

        TTK_DeleteFile(filename);
        assert(TTK_IsFileExist(filename) == false);

        assert(TTK_SaveContentToFile(filename, initial_content));
        assert(TTK_SaveContentToFile(filename, expected_content));

        bool is_success = false;
        const std::string content = TKK_LoadContentFromFile(filename, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }

    // load not existing file
    {
        const std::string filename          = "log/NotExistingSaveLoadTest.txt";

        TTK_DeleteFile(filename);
        assert(TTK_IsFileExist(filename) == false);

        bool is_success = false;
        const std::string content = TKK_LoadContentFromFile(filename, &is_success);
        assert(is_success == false);
        assert(content == "");
    }

    // load not existing file no success info
    {
        const std::string filename          = "log/NotExistingSaveLoadTest.txt";

        TTK_DeleteFile(filename);
        assert(TTK_IsFileExist(filename) == false);

        const std::string content = TKK_LoadContentFromFile(filename);
        assert(content == "");
    }
}

void RunAllTests() {
    system("if not exist log mkdir log");

    TestTTK_IsFileExist();
    TestTTK_IsFolderExist();
    TestTTK_DeleteFile();
    TestTTK_LoadSaveFileContent();
}

int main() {
    RunAllTests();
    return 0;
}