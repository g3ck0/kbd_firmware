#include QMK_KEYBOARD_H
#include "../custom_keycodes.h"
#include "modcase.h"
#include "../xcase/xcase.h"
#include "../handsdown/moutis_semantickeys.h"

// Compose state: armed by the Shift+Space thumb combo, consumed by the next keypress
static bool compose_pending = false;

// Select latch state: real Shift, scoped to the EXTEND layer
static bool sel_latch_active = false;
static bool del_latch_active = false;

void sel_latch_off(void) {
    if (sel_latch_active) {
        unregister_code(KC_LSFT);
        sel_latch_active = false;
    }
}

void del_latch_off(void) {
    if (del_latch_active) {
        del_latch_active = false;
    }
}

bool del_latch_is_active(void) {
    return del_latch_active;
}


/**
 * @brief Process a keypress with xcase.
 * @param keycode The keycode of the keypress.
 * @param record The keyrecord to process.
 * @return True if the keypress is returned to the caller, false otherwise.
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
        // Extract base keycode from LT/MT transparently
    uint16_t base = keycode;
    if (IS_QK_LAYER_TAP(keycode)) base = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
    if (IS_QK_MOD_TAP(keycode))   base = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);

    if (compose_pending && record->event.pressed) {
        uint16_t kc = keycode;
        if (IS_QK_MOD_TAP(keycode)) {
            if (record->tap.count == 0) {
                return true;  // mod-tap held as modifier: keep compose pending
            }
            kc = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        }
        switch (kc) {
            case KC_LCTL ... KC_RGUI:
                return true;  // plain modifiers don't consume compose (allows shifted accents)
            case KC_ESC:
                compose_pending = false;
                return false;  // cancel
            case KC_A:
                compose_pending = false;
                tap_accented(RALT(KC_A), RALT(S(KC_A)));
                return false;
            case KC_E:
                compose_pending = false;
                tap_accented(RALT(KC_E), RALT(S(KC_E)));
                return false;
            case KC_I:
                compose_pending = false;
                tap_accented(RALT(KC_I), RALT(S(KC_I)));
                return false;
            case KC_O:
                compose_pending = false;
                tap_accented(RALT(KC_O), RALT(S(KC_O)));
                return false;
            case KC_U:
                compose_pending = false;
                tap_accented(RALT(KC_U), RALT(S(KC_U)));
                return false;
            case KC_N:
                compose_pending = false;
                tap_accented(RALT(KC_N), RALT(S(KC_N)));
                return false;
            default:
                compose_pending = false;
                return true;  // pass through unchanged
        }
    }

    if (!process_semkey(keycode, record)) {
        return false; // took care of that key
    }

    // Handle activation/deactivation keycodes first
    if (record->event.pressed) {
        switch (keycode) {
            case XCASE_SNAKE:
                caps_word_off();
                enable_xcase_with(KC_UNDS);
                return false; // Keycode handled
            case XCASE_KEBAB:
                caps_word_off();
                enable_xcase_with(KC_MINS);
                return false; // Keycode handled
            case XCASE_CAMEL:
                caps_word_off();
                enable_xcase_with(KC_LSFT);
                return false; // Keycode handled
            case XCASE_OFF:
                disable_xcase();
                return false; // Keycode handled
            case SEL_LATCH:
                if (record->event.pressed) {
                    if (sel_latch_active) {
                        sel_latch_off();
                    } else {
                        register_code(KC_LSFT);
                        sel_latch_active = true;
                    }
                }
                return false;
            case DEL_LATCH:
                if (record->event.pressed) {
                    if (del_latch_active) {
                        del_latch_off();
                    } else {
                        del_latch_active = true;
                    }
                }
                return false;

            case KC_ESC:
                // Esc bails out of an active selection latch (mirrors oneshot cancel)
                if (record->event.pressed) {
                    sel_latch_off();
                    del_latch_off();
                }
                break;
            case MOD_ACCENT:
                compose_pending = true;
                return false; // Keycode handled

        // case ARROW_OP:
        //     if (record->event.pressed) {
        //         uint8_t saved = get_mods();
        //         clear_mods();
        //         send_string((saved & MOD_MASK_SHIFT) ? "=>" : "->");
        //         set_mods(saved);
        //     }
        //     return false;
                
        }
        // Handle base keycode for tap-hold keys
        switch (base) {
            case KC_ESC:
                sel_latch_off();
                del_latch_off();
                break; // ← break, not return false, so LT still works normally
        }
    }

    // If not active, pass all keys through
    if (!is_xcase_active()) {
        return true;
    }

    // process on key down
    if (record->event.pressed) {
        // Strip mod-tap and layer-tap to get base keycode
        uint16_t base_keycode = keycode;
        if (IS_QK_MOD_TAP(keycode)) {
            base_keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        } else if (IS_QK_LAYER_TAP(keycode)) {
            base_keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
        }

        if (base_keycode == KC_SPC) {
            // check for double space to exit xcase mode
            if (get_xcase_last_keycode() == KC_SPC) {
                if (get_xcase_delimiter() != KC_LSFT &&
                    get_xcase_delimiter() != KC_CAPS)
                {
                    tap_code(KC_BSPC); // remove the trailing delimiter for non-camelCase
                }
                disable_xcase();
                return true; // Let the second space through
            }

            // replace space with delimiter
            if (get_xcase_delimiter() == KC_LSFT) {
                add_oneshot_mods(MOD_BIT(get_xcase_delimiter()));  // add one-shot shift for camelCase
            } else {
                tap_code16(get_xcase_delimiter());  // send the delimiter
            }
            set_xcase_last_keycode(KC_SPC);
            return false; // do not send space
        }

        // check if this key should continue xcase mode
        if (!is_xcase_exclusion_keycode(base_keycode)) {
            disable_xcase();
        } else {
            set_xcase_last_keycode(base_keycode);
        }
        return true;
    }
    return true;
}
