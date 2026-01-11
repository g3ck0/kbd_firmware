
#define OS_LSFT OSM(MOD_LSFT)
//xcase
//https://github.com/ohshitgorillas/qmk-xcase

void leader_end_user(void) {
    if (leader_sequence_one_key(KC_S)) {
        SEND_STRING("snake");
    }

    // Leader, S, C for snake_case
    if (leader_sequence_two_keys(KC_S, KC_C)) {
        enable_xcase_with(KC_UNDS);
    }
    // Leader, K, C for kebab-case
    else if (leader_sequence_two_keys(KC_K, KC_C)) {
        enable_xcase_with(KC_MINS);
    }
    // Leader, C, C for camelCase
    else if (leader_sequence_two_keys(KC_C, KC_C)) {
        enable_xcase_with(OS_LSFT);
    }
    // LEAD → N → D (Ctrl + Shift + End)
    else if (leader_sequence_two_keys(KC_N, KC_D)) {
        tap_code16(C(S(KC_END)));
        return;
    }
    // LEAD → N → S (Ctrl + Shift + Home)
    else if (leader_sequence_two_keys(KC_N, KC_S)) {
        tap_code16(C(S(KC_HOME)));
        return;
    }
}

void leader_dictionary(void) {
    if (leader_sequence_one_key(KC_S)) {
        SEND_STRING("snake");
    }

    // Leader, S, C for snake_case
    if (leader_sequence_two_keys(KC_S, KC_C)) {
        enable_xcase_with(KC_UNDS);
    }
    // Leader, K, C for kebab-case
    else if (leader_sequence_two_keys(KC_K, KC_C)) {
        enable_xcase_with(KC_MINS);
    }
    // Leader, C, C for camelCase
    else if (leader_sequence_two_keys(KC_C, KC_C)) {
        enable_xcase_with(OS_LSFT);
    }
    // LEAD → N → D (Ctrl + Shift + End)
    else if (leader_sequence_two_keys(KC_N, KC_D)) {
        tap_code16(C(S(KC_END)));
        return;
    }
    // LEAD → N → S (Ctrl + Shift + Home)
    else if (leader_sequence_two_keys(KC_N, KC_S)) {
        tap_code16(C(S(KC_HOME)));
        return;
    }
}
