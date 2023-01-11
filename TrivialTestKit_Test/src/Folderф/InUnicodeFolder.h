#ifndef INUNICODEFOLDER_H_
#define INUNICODEFOLDER_H_

#include "TrivialTestKit.h"

extern uint64_t g_test_finish_counter;
extern uint64_t g_line[MAX_REG_LINES];

inline void AssertFailInUnicodeFolder() {
    TTK_ASSERT(true);
    g_line[0] = __LINE__ + 1;
    TTK_ASSERT(10 != 10);
    TTK_ASSERT(false);

    g_test_finish_counter += 1;
}

inline void ExpectFailInUnicodeFolder() {
    TTK_EXPECT(true);
    g_line[0] = __LINE__ + 1;
    TTK_EXPECT(10 != 10);
    g_line[1] = __LINE__ + 1;
    TTK_EXPECT(false);

    g_test_finish_counter += 1;
}

#endif // INUNICODEFOLDER_H_
