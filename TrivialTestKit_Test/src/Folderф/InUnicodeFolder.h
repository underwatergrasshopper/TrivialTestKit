#ifndef INUNICODEFOLDER_H_
#define INUNICODEFOLDER_H_

#include "TrivialTestKit.h"
#include <string>

// NOTE: !!! Success TestTTK_Assert function in Test.h depends on position of TTK_Assert{M|R} in this header file. 
// If position of any of those functions change, don't forget update corresponding tests in TestTTK_Assert with new positions. 

inline void FailAssertInUnicodeFolder(bool& is_finished) {
    is_finished = false;

    TTK_ASSERT(10 == 5);
    TTK_ASSERT(false);

    is_finished = true;
}

#endif // INUNICODEFOLDER_H_
