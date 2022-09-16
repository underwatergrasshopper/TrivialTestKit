#include "TrivialTestKit.h"

#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>

int main() {
    system("if not exist log mkdir log");

    {
        const std::string filename          = "log/SaveLoadTest.txt";
        const std::string expected_content  = "Some text to save.";

        assert(TTK_SaveContentToFile(filename, expected_content));

        bool is_success = false;
        const std::string content = TKK_LoadContentFromFile(filename, &is_success);
        assert(is_success);
        assert(content == expected_content);
    }

    TTK_Assert(1 == 2);
    TTK_AssertM(3 == 5, "This is stupid.");

    return 0;
}