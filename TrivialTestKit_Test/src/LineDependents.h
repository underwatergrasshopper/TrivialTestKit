#ifndef LINEDEPENDENTS_H_
#define LINEDEPENDENTS_H_

#include "TrivialTestKit.h"
#include <string>
// NOTE: !!! Success TestTTK_Assert function in Test.h depends on position of TTK_ASSERT_{M} in this header file. 
// If position of any of those functions change, don't forget update corresponding tests in TTK_ASSERT with new positions. 

enum { ASSERT_FAIL_RETURN = 12 };

inline void FailAssert(bool& is_finished) {
    is_finished = false;

    TTK_ASSERT(10 == 5);
    TTK_ASSERT(false);

    is_finished = true;
}

inline void FailAssertMessage(bool& is_finished) {
    is_finished = false;

    TTK_ASSERT_M(10 == 5, "Some message.");
    TTK_ASSERT_M(false, "");

    is_finished = true;
}























inline void SuccessAssert(bool& is_finished) {
    is_finished = false;

    TTK_ASSERT(10 == 10);
    TTK_ASSERT_M(10 == 10, "Some message.");

    is_finished = true;
}












inline void FailAssertMessageString(bool& is_finished) {
    is_finished = false;

    TTK_ASSERT_M(10 == 5, std::string("Some message."));
    TTK_ASSERT_M(false, "");

    is_finished = true;
}









inline void TestD_Fail() {
    //TTK_NotifyTest();

    TTK_ASSERT(true);
    TTK_ASSERT(5 == 7); // will fail
    TTK_ASSERT(10 == 10);
}

inline void FailAssertMessageUTF8(bool& is_finished) {
    is_finished = false;

    TTK_ASSERT_M(10 == 5, u8"Some message\u0444.");
    TTK_ASSERT_M(false, "");

    is_finished = true;
}

#endif // LINEDEPENDENTS_H_
