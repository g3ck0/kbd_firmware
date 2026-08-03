#include QMK_KEYBOARD_H
#include "../xcase/xcase.h"
// #define OS_LSFT OSM(MOD_LSFT)
// #define OS_RSFT OSM(MOD_RSFT)

//xcase
//https://github.com/ohshitgorillas/qmk-xcase


void tap_accented(uint16_t lower, uint16_t upper) {
    if (get_mods() & MOD_MASK_SHIFT) {
        del_mods(MOD_MASK_SHIFT);
        tap_code16(upper);
        add_mods(MOD_MASK_SHIFT);
    } else {
        tap_code16(lower);
    }
}

void leader_start_user(void) {
    // Do something when the leader key is pressed
    return;
}

void leader_end_user(void) {
    
    // Leader, S, C for snake_case
    if (leader_sequence_one_key(KC_S)) {
        caps_word_off();
        enable_xcase_with(KC_UNDS);
        // SEND_STRING("sc");
    }
    // Leader, K, C for kebab-case
    if (leader_sequence_one_key(KC_K)) {
        caps_word_off();
        enable_xcase_with(KC_MINS);
        // SEND_STRING("kc");
    }
    // Leader, C, C for camelCase
    if (leader_sequence_one_key(KC_C)) {
        caps_word_off();
        enable_xcase_with(KC_LSFT );
    }
    
}

