#include <quantum.h>
#include <stdio.h>
#include "../feature/modprocessrecord.h"
#include "moutis_semantickeys.h"

// Maximum number of keycodes in a sequence
#define MAX_SEMKEY_SEQUENCE 3

// Semantic key data structure: holds platform-specific keycode sequences
// Sequences are terminated by KC_NO
typedef struct {
    // uint16_t mac_sequence[MAX_SEMKEY_SEQUENCE];
    uint16_t win_sequence[MAX_SEMKEY_SEQUENCE];
} semkey_map_t;

// Track the registered semantic key for proper release
static uint16_t registered_semkey_code = KC_NO;

static const semkey_map_t semkey_table[SK_count] = {
    [SK_ndx(SK_DELWDL)]         = {{C(KC_BSPC), KC_NO}},            // DELETE WORD LEFT
    [SK_ndx(SK_DELWDR)]         = {{C(KC_DEL), KC_NO}},             // DELETE WORD RIGHT
    [SK_ndx(SK_DELLNL)]         = {{S(KC_HOME), KC_BSPC, KC_NO}},   // Delete line left of cursor
    [SK_ndx(SK_DELLNR)]         = {{S(KC_END), KC_BSPC, KC_NO}},    // Delete line right of cursor
    [SK_ndx(SK_WORDPRV)]        = {{C(KC_LEFT), KC_NO}},            // WORD LEFT
    [SK_ndx(SK_WORDNXT)]        = {{C(KC_RIGHT), KC_NO}},           // WORD RIGHT
    [SK_ndx(SK_DOCBEG)]         = {{C(KC_HOME), KC_NO}},            // Go to start of document
    [SK_ndx(SK_DOCEND)]         = {{C(KC_END), KC_NO}},             // Go to end of document
    [SK_ndx(SK_LINEBEG)]        = {{KC_HOME, KC_NO}},               // Go to beg of line
    [SK_ndx(SK_LINEEND)]        = {{KC_END, KC_NO}},                // Go to end of line
};
// Tap a sequence of keycodes for a semantic key (public function)
void tap_semkey_code(uint16_t sk) {
    if (!is_SemKey(sk)) {
        return;
    }

    uint16_t idx = SK_ndx(sk);
    if (idx >= SK_count) {
        return;
    }

    const semkey_map_t *entry = &semkey_table[idx];
    const uint16_t *sequence = entry->win_sequence;

    uint8_t held_mods = get_mods();
    uint8_t held_weak_mods = get_weak_mods();

    clear_mods();
    clear_weak_mods();
    send_keyboard_report();
    wait_ms(10);

    // Tap each keycode in the sequence until we hit KC_NO
    for (int i = 0; i < MAX_SEMKEY_SEQUENCE && sequence[i] != KC_NO; i++) {
        tap_code16(sequence[i]);
    }

    set_mods(held_mods);
    set_weak_mods(held_weak_mods);
    send_keyboard_report();
}

// Get the first platform-specific keycode for a semantic key (for backwards compatibility)
// Returns KC_NO for multi-keycode sequences
uint16_t get_semkey_code(uint16_t sk) {
    if (!is_SemKey(sk)) {
        return KC_NO;
    }

    uint16_t idx = SK_ndx(sk);
    if (idx >= SK_count) {
        return KC_NO;
    }

    const semkey_map_t *entry = &semkey_table[idx];
    const uint16_t *sequence = entry->win_sequence;

    // Return first keycode if it's the only one, otherwise KC_NO (use tap_semkey_code instead)
    if (sequence[0] != KC_NO && sequence[1] == KC_NO) {
        return sequence[0];
    }

    return KC_NO;  // Multi-keycode sequence, can't return a single code
}

void register_SemKey(uint16_t sk) {
    if (!is_SemKey(sk)) {
        return;
    }

    // Get the platform-specific keycode
    uint16_t platform_keycode = get_semkey_code(sk);

    // If it's a single keycode, use register/unregister for proper hold behavior
    if (platform_keycode != KC_NO) {
        register_code16(platform_keycode);
        registered_semkey_code = platform_keycode;
    } else {
        // Multi-keycode sequence - just tap it
        tap_semkey_code(sk);
    }
}


bool process_semkey(uint16_t keycode, const keyrecord_t *record) {
    // custom processing could hapen here
    uint8_t  held_mods;

    bool del_latch_active = del_latch_is_active();

    if (!(is_SemKey(keycode)))
        return true; // nothing to do. continue processing this record
    
    held_mods = get_mods() |
                get_weak_mods() |
                get_oneshot_mods();

    if (del_latch_active && record->event.pressed) {
        switch (keycode) {
            case SK_WORDPRV:
                register_SemKey(SK_DELWDL);
                return false;
            case SK_WORDNXT:
                register_SemKey(SK_DELWDR);
                return false;
            case SK_LINEBEG:
                register_SemKey(SK_DELLNL);
                return false;
            case SK_LINEEND:
                register_SemKey(SK_DELLNR);
                return false;
        }
    }

    if (record->event.pressed) {
        switch (keycode) {
            case SK_LINEBEG: //
                if (!(held_mods & ~MOD_MASK_SHIFT)) {
                    register_SemKey(SK_LINEBEG);
                } else if (held_mods & MOD_MASK_CTRL) {
                    register_SemKey(SK_DOCBEG);
                } else {
                    register_SemKey(SK_LINEBEG);
                }
                break;
            case SK_LINEEND: //
                if (!(held_mods & ~MOD_MASK_SHIFT)) {
                    register_SemKey(SK_LINEEND);
                } else if (held_mods & MOD_MASK_CTRL) {
                    register_SemKey(SK_DOCEND);
                } else {
                    register_SemKey(SK_LINEEND);
                }
                break;

            default: // default keydown event (from the semkey table)
                register_SemKey(keycode);
                break;
        }
    } else { // The keyup event
        // Key release: unregister whatever we sent on keydown
        if (registered_semkey_code != KC_NO) {
            unregister_code16(registered_semkey_code);
            registered_semkey_code = KC_NO;
        }

    }
    return false; // don't do more with this record.
};
