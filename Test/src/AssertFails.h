#ifndef ASSERTFAIL_H_
#define ASSERTFAIL_H_

#include "TrivialTestKit.h"

// NOTE: !!! Success TestTTK_Assert function in Test.h depends on position of TTK_Assert{M|R} in this header file. 
// If position of any of those functions change, don't forget update corresponding tests in TestTTK_Assert with new positions. 

inline void FailAssert(bool& is_finished) {
    is_finished = false;

    TTK_Assert(10 == 5);
    TTK_Assert(false);

    is_finished = true;
}

inline void FailAssertMessage(bool& is_finished) {
    is_finished = false;

    TTK_AssertM(10 == 5, "Some message.");
    TTK_Assert(false);

    is_finished = true;
}

inline int FailAssertReturn(bool& is_finished) {
    is_finished = false;

    TTK_AssertR(10 == 5, 12);
    TTK_Assert(false);

    is_finished = true;

    return 0;
}

inline int FailAssertMessageReturn(bool& is_finished) {
    is_finished = false;

    TTK_AssertMR(10 == 5, "Some message.", 12);
    TTK_Assert(false);

    is_finished = true;

    return 0;
}

#endif // ASSERTFAIL_H_
