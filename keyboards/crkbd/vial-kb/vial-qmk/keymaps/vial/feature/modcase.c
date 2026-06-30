#include QMK_KEYBOARD_H
#include "../xcase/xcase.h"
// #define OS_LSFT OSM(MOD_LSFT)
// #define OS_RSFT OSM(MOD_RSFT)

//xcase
//https://github.com/ohshitgorillas/qmk-xcase
static void tap_accented(uint16_t lower, uint16_t upper);

void leader_start_user(void) {
    // Do something when the leader key is pressed
    return;
}

void leader_end_user(void) {
    
    // Leader, S, C for snake_case
    if (leader_sequence_one_key(KC_S)) {
        enable_xcase_with(KC_UNDS);
        // SEND_STRING("sc");
    }
    // Leader, K, C for kebab-case
    if (leader_sequence_one_key(KC_K)) {
        enable_xcase_with(KC_MINS);
        // SEND_STRING("kc");
    }
    // Leader, C, C for camelCase
    if (leader_sequence_one_key(KC_C)) {
        enable_xcase_with(KC_LSFT );
    }
    
    if (leader_sequence_one_key(KC_A)){
        tap_accented(RALT(KC_A), RALT(S(KC_A)));
    }else if (leader_sequence_one_key(KC_E)){
         tap_accented(RALT(KC_E), RALT(S(KC_E)));
    }
    else if (leader_sequence_one_key(KC_I)){
         tap_accented(RALT(KC_I), RALT(S(KC_I)));
    }
    else if (leader_sequence_one_key(KC_O)){
         tap_accented(RALT(KC_O), RALT(S(KC_O)));
    }
    else if (leader_sequence_one_key(KC_U)){
         tap_accented(RALT(KC_U), RALT(S(KC_U)));
    }
    else if (leader_sequence_one_key(KC_N)){
         tap_accented(RALT(KC_N), RALT(S(KC_N)));
    }
}


static void tap_accented(uint16_t lower, uint16_t upper) {
    if (get_mods() & MOD_MASK_SHIFT) {
        del_mods(MOD_MASK_SHIFT);
        tap_code16(upper);
        add_mods(MOD_MASK_SHIFT);
    } else {
        tap_code16(lower);
    }
}