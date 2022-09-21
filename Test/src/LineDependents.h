#ifndef LINEDEPENDENTS_H_
#define LINEDEPENDENTS_H_

#include "TrivialTestKit.h"
#include <string>
// NOTE: !!! Success TestTTK_Assert function in Test.h depends on position of TTK_Assert{M|R} in this header file. 
// If position of any of those functions change, don't forget update corresponding tests in TestTTK_Assert with new positions. 

enum { ASSERT_FAIL_RETURN = 12 };

inline void FailAssert(bool& is_finished) {
    is_finished = false;

    TTK_Assert(10 == 5);
    TTK_Assert(false);

    is_finished = true;
}

inline void FailAssertMessage(bool& is_finished) {
    is_finished = false;

    TTK_AssertM(10 == 5, "Some message.");
    TTK_AssertM(false, "");

    is_finished = true;
}

inline int FailAssertReturn(bool& is_finished) {
    is_finished = false;

    TTK_AssertR(10 == 5, ASSERT_FAIL_RETURN);
    TTK_AssertR(false, 0);

    is_finished = true;

    return 0;
}

inline int FailAssertMessageReturn(bool& is_finished) {
    is_finished = false;

    TTK_AssertMR(10 == 5, "Some message.", ASSERT_FAIL_RETURN);
    TTK_AssertMR(false, "", 0);

    is_finished = true;

    return 0;
}

inline void SuccessAssert(bool& is_finished) {
    is_finished = false;

    TTK_Assert(10 == 10);
    TTK_AssertM(10 == 10, "Some message.");

    is_finished = true;
}

inline int SuccessAssertReturn(bool& is_finished) {
    is_finished = false;

    TTK_AssertR(10 == 10, ASSERT_FAIL_RETURN);
    TTK_AssertMR(10 == 10, "Some message.", ASSERT_FAIL_RETURN);

    is_finished = true;

    return 0;
}

inline void FailAssertMessageString(bool& is_finished) {
    is_finished = false;

    TTK_AssertM(10 == 5, std::string("Some message."));
    TTK_AssertM(false, "");

    is_finished = true;
}









inline void TestD_Fail() {
    TTK_NotifyTest();

    TTK_Assert(true);
    TTK_Assert(5 == 7); // will fail
    TTK_Assert(10 == 10);
}

inline void FailAssertMessageUTF8(bool& is_finished) {
    is_finished = false;

    TTK_AssertM(10 == 5, u8"Some message\u0444.");
    TTK_AssertM(false, "");

    is_finished = true;
}

#endif // LINEDEPENDENTS_H_
