#pragma once

#include "../custom_keycodes.h"

#define SK_beg SK_KILL
#define SK_end (SK_LINEEND + 1)
#define SK_count (SK_end - SK_beg)
#define SK_ndx(sk) (sk - SK_beg)

#define is_SemKey(sk) ((sk >= (uint16_t)(SK_beg)) && (sk < (uint16_t)SK_end))

// Forward declarations
bool process_semkey(uint16_t keycode, const keyrecord_t *record);

