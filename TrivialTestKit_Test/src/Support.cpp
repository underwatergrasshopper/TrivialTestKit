#include "Support.h"

static bool s_is_wide_oriented = false;

void SwitchStdOutToWideOriented() {
    s_is_wide_oriented = true;
}

bool IsStdOutWideOriented() {
    return s_is_wide_oriented;
}

