#ifndef ASSERTFAIL_H_
#define ASSERTFAIL_H_

#include "TrivialTestKit.h"

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

inline void FailAssertMessageUTF16(bool& is_finished) {
    is_finished = false;

    TTK_AssertM(10 == 5, L"Some message\u0444.");
    TTK_AssertM(false, "");

    is_finished = true;
}

inline void TestDummyFullTrace() {
    TTK_FullTrace("Some message.");
}

inline void TestDummyFullTraceUTF16() {
    TTK_FullTrace(L"Some message\u0444.");
}

#endif // ASSERTFAIL_H_
