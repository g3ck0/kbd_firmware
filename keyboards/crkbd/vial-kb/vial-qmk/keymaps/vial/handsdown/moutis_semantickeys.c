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
// Mac, Win, (Phase 3, add others if necessary, expand to multi-key?)
        // System-wide controls

    [SK_ndx(SK_KILL)] = {{LCA(KC_DEL), KC_NO}},        // Force quit / ctrl-alt-del
    // [SK_ndx(SK_HENK)] = {KC_LNG1, C(S(KC_1))},            // 変換/かな
    // [SK_ndx(SK_MHEN)] = {KC_LNG2, C(S(KC_0))},            // 無変換/英数
    // [SK_ndx(SK_DKT8)] = {C(S(KC_3)),G(KC_H)},             // Dictate speech to text
    // [SK_ndx(SK_AIVC)] = {C(S(KC_4)),G(KC_C)},             // AI voice control (mac Siri/Win Cortana)
    //     // extended characters/ editing commands
    // [SK_ndx(SK_HENT)] = {G(KC_ENT),C(KC_ENT)},            // Hard ENTER
    // [SK_ndx(SK_CUT )] = {G(KC_X),C(KC_X)},                // cut
    // [SK_ndx(SK_COPY)] = {G(KC_C),C(KC_C)},                // copy
    // [SK_ndx(SK_PSTE)] = {G(KC_V),C(KC_V)},                // paste
    // [SK_ndx(SK_PSTM)] = {G(S(A(KC_V))),C(S(A(KC_V)))},    // paste_match
    // [SK_ndx(SK_SALL)] = {G(KC_A),C(KC_A)},                // select all
    // [SK_ndx(SK_CLOZ)] = {G(KC_W),C(KC_W)},                // close
    // [SK_ndx(SK_QUIT)] = {G(KC_Q),C(KC_Q)},                // quit
    // [SK_ndx(SK_NEW)]  = {G(KC_N),C(KC_N)},                  // new
    // [SK_ndx(SK_OPEN)] = {G(KC_O),C(KC_O)},                // open
    [SK_ndx(SK_FIND)] = {{C(KC_F), KC_NO}},                // find
    // [SK_ndx(SK_FAGN)] = {G(KC_G),C(KC_G)},                // find again
    // [SK_ndx(SK_SCAP)] = {LSG(KC_4),KC_PSCR},              // Screen Capture
    // [SK_ndx(SK_SCLP)] = {C(S(G(KC_4))),A(KC_PSCR)},       // Selection Capture
    // [SK_ndx(SK_SRCH)] = {G(KC_SPC),G(KC_S)},              // platform search (siri/cortana, etc.)
    [SK_ndx(SK_DELWDL)] = {{C(KC_BSPC), KC_NO}},        // DELETE WORD LEFT
    [SK_ndx(SK_DELWDR)] = {{C(KC_DEL), KC_NO}},          // DELETE WORD RIGHT
    [SK_ndx(SK_DELLNL)] = {{S(KC_HOME), KC_BSPC, KC_NO}},        // Delete line left of cursor
    [SK_ndx(SK_DELLNR)] = {{S(KC_END), KC_BSPC, KC_NO}},          // Delete line right of cursor
        // extended navigation
    [SK_ndx(SK_WORDPRV)]        = {{C(KC_LEFT), KC_NO}},        // WORD LEFT
    [SK_ndx(SK_WORDNXT)]        = {{C(KC_RIGHT), KC_NO}},       // WORD RIGHT
    [SK_ndx(SK_DOCBEG)]         = {{C(KC_HOME), KC_NO}},        // Go to start of document
    [SK_ndx(SK_DOCEND)]         = {{C(KC_END), KC_NO}},         // Go to end of document
    [SK_ndx(SK_LINEBEG)]        = {{KC_HOME, KC_NO}},           // Go to beg of line
    [SK_ndx(SK_LINEEND)]        = {{KC_END, KC_NO}},            // Go to end of line
    //
    // [SK_ndx(SK_PARAPRV)] = {A(KC_UP),C(KC_UP)},           // Go to previous paragraph
    // [SK_ndx(SK_PARANXT)] = {A(KC_DOWN),C(KC_DOWN)},       // Go to next paragraph
    // [SK_ndx(SK_HISTPRV)] = {G(KC_LBRC),A(KC_LEFT)},       // BROWSER BACK
    // [SK_ndx(SK_HISTNXT)] = {G(KC_RBRC),A(KC_RIGHT)},      // BROWSER FWD
    // [SK_ndx(SK_ZOOMIN)] = {G(KC_EQL),C(KC_EQL)},          // ZOOM IN
    // [SK_ndx(SK_ZOOMOUT)] = {G(KC_MINS),C(KC_MINS)},       // ZOOM OUT
    // [SK_ndx(SK_ZOOMRST)] = {G(KC_0),C(KC_0)},             // ZOOM RESET
    // [SK_ndx(SK_APPNXT)] = {G(KC_TAB),A(KC_TAB)},          // APP switcher Next (last used)
    // [SK_ndx(SK_APPPRV)] = {RSG(KC_TAB),LSA(KC_TAB)},      // APP switcher Prev (least recently used)
    // [SK_ndx(SK_WINNXT)] = {C(KC_TAB),C(KC_TAB)},          // Window/tab switcher Next
    // [SK_ndx(SK_WINPRV)] = {C(S(KC_TAB)),C(S(KC_TAB))},    // Window/tab switcher Prev
    //     // Punctuation & typography
    // [SK_ndx(SK_NDSH)] = {A(KC_MINS),0x8150},              // – N-Dash 
    // [SK_ndx(SK_MDSH)] = {LSA(KC_MINS),0x8151},            // — M-Dash
    // [SK_ndx(SK_ELPS)] = {A(KC_SCLN),0x8133},              // … 
    // [SK_ndx(SK_SCRS)] = {LSA(KC_5),0x8134},               // † Single Cross 
    // [SK_ndx(SK_DCRS)] = {LSA(KC_7),0x8135},               // ‡ Double Cross 
    // [SK_ndx(SK_BBLT)] = {A(KC_8),0x8149},                 // • Bold Bullet 
    // [SK_ndx(SK_SBLT)] = {LSA(KC_9),0x8183},               // · Small Bullet 
    // [SK_ndx(SK_PARA)] = {A(KC_7),0x8182},                 // ¶ 
    // [SK_ndx(SK_SECT)] = {A(KC_5),0x8167},                 // §
    //     // Number & Math symbols
    // [SK_ndx(SK_PERM)] = {LSA(KC_R),0x8137},               // ‰ Per Mille
    // [SK_ndx(SK_DEGR)] = {LSA(KC_8),0x8176},               // ° DEGREE
    // [SK_ndx(SK_GTEQ)] = {A(KC_DOT),0x4242},               // ≥ Greater Than or Equal to
    // [SK_ndx(SK_LTEQ)] = {A(KC_COMM),0x4243},              // ≤ Less Than or Equal to
    // [SK_ndx(SK_PLMN)] = {LSA(KC_EQL),0x8177},             // ± Plus/Minus
    // [SK_ndx(SK_DIV) ] = {A(KC_SLSH),0x4246},              // ÷ Divide
    // [SK_ndx(SK_FRAC)] = {LSA(KC_1),0x4246},               // ⁄ Fraction "Solidus" symbol
    // [SK_ndx(SK_NOTEQ)] = {A(KC_EQL),ALGR(KC_EQL)},        // ≠ NOT Equal to
    // [SK_ndx(SK_APXEQ)] = {A(KC_X),0x4247},                // ≈ APPROX Equal to
    // [SK_ndx(SK_OMEGA)] = {A(KC_Z),0x4234},                // Ω OMEGA
    //     // Currency
    // [SK_ndx(SK_EURO)] = {LSA(KC_2),0x8128},               // € 
    // [SK_ndx(SK_CENT)] = {A(KC_4),0x8162},                 // ¢ 
    // [SK_ndx(SK_BPND)] = {A(KC_3),0x8163},                 // £ 
    // [SK_ndx(SK_JPY )] = {A(KC_Y),0x8165},                 // ¥
    // [SK_ndx(SK_No  )] = {LSA(KC_SCLN),0x8470},            // № ordinal number symbol *wrong alt code*
    //     // Quotations
    // [SK_ndx(SK_SQUL)] = {A(KC_RBRC),0x8145},              // ’ ** Left single quote 
    // [SK_ndx(SK_SQUR)] = {LSA(KC_RBRC),0x8146},            // ’ ** Right single quote 
    // [SK_ndx(SK_SDQL)] = {A(KC_LBRC),0x8147},              // “ ** Left double quote 
    // [SK_ndx(SK_SDQR)] = {LSA(KC_LBRC),0x8148},            // ” ** Right double quote 
    // [SK_ndx(SK_FDQL)] = {A(KC_BSLS),0x8171},              // « Left double French quote 
    // [SK_ndx(SK_FDQR)] = {LSA(KC_BSLS),0x8187},            // » Right double French quote 
    // [SK_ndx(SK_FSQL)] = {LSA(KC_3),0x8139},               // ‹ Left single French quote 
    // [SK_ndx(SK_FSQR)] = {LSA(KC_4),0x8155},               // › Right single French quote 
    // [SK_ndx(SK_IQUE)] = {LSA(KC_SLASH),0x8191},           // ¿ Spanish inverted Question Mark
    // [SK_ndx(SK_IEXC)] = {A(KC_1),0x8161},                 // ¡ Spanish inverted Exclamation Mark
    //     // Composed letters with diacritics
    // [SK_ndx(SK_ENYE)] = {A(KC_N),ALGR(KC_N)}             // ñ/Ñ 
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
