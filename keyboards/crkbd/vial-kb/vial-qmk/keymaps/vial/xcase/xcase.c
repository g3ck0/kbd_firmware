/* Simple xcase implementation
* Replaces spaces with delimiters for snake_case, kebab-case, camelCase
*/

#include "xcase.h"

// private variables
static bool xcase_active = false;
static uint16_t xcase_delimiter = KC_UNDS;
static uint16_t last_keycode = KC_NO;

#define MAX_EXCLUSION_KEYCODES 16
static uint16_t exclusion_keycodes[MAX_EXCLUSION_KEYCODES];
static uint8_t exclusion_keycode_count = 0;


#define OS_LSFT OSM(MOD_LSFT)
#define OS_RSFT OSM(MOD_RSFT)

// public functions

/**
 * @brief Check if xcase is active.
 * @return True if xcase is active, false otherwise.
 */
 bool is_xcase_active(void) {
    return xcase_active;
}


/**
 * @brief Check if a keycode is an exclusion.
 * Exclusions are keycodes that will not trigger the end of xcase.
 * @param keycode The keycode to check.
 * @return True if the keycode is an exclusion, false otherwise.
 */
bool is_xcase_exclusion_keycode(uint16_t keycode) {
    // pass keycodes on the user's exclusion list
    for (uint8_t i = 0; i < exclusion_keycode_count; i++) {
        if (exclusion_keycodes[i] == keycode) {
            return true;
        }
    }

    // pass layering keys and one-shot mods
    if (IS_QK_MOMENTARY(keycode) ||
        IS_QK_DEF_LAYER(keycode) ||
        IS_QK_TOGGLE_LAYER(keycode) ||
        IS_QK_ONE_SHOT_LAYER(keycode) ||
        IS_QK_TO(keycode) ||
        IS_QK_LAYER_MOD(keycode) ||
        IS_QK_ONE_SHOT_MOD(keycode) ||
        is_caps_word_on()       // if caps_word is active
    ) {
        return true;
    }

    switch (keycode) {
#ifdef TRI_LAYER_ENABLE // Ignore Tri Layer keys.
        case QK_TRI_LAYER_LOWER ... QK_TRI_LAYER_UPPER:
#endif
#ifdef LAYER_LOCK_ENABLE // Ignore Layer Lock key.
        case QK_LAYER_LOCK:
#endif
        // alphanumeric keys
        case KC_A ... KC_Z:
        case KC_P1 ... KC_P0:
        // international language keys
        case KC_INTERNATIONAL_1 ... KC_LANGUAGE_9:
        // common delimiters
        case KC_UNDERSCORE:
        case KC_MINUS:
        case KC_KP_MINUS:
        // editing keys
        case KC_BACKSPACE:
        case KC_DELETE:
        case KC_LEFT:
        case KC_RIGHT:
        case KC_UP:
        case KC_DOWN:
        // modifier keys
        case KC_LEFT_SHIFT:
        case KC_RIGHT_SHIFT:
        case KC_LEFT_CTRL:
        case KC_RIGHT_CTRL:
        case KC_LEFT_GUI:
        case KC_RIGHT_GUI:
        case KC_RIGHT_ALT:
        case KC_LEFT_ALT:
        case KC_CAPS:
            return true;
        case KC_1 ... KC_0:
            if ((get_mods()|get_oneshot_mods()|get_weak_mods()) & MOD_MASK_SHIFT) {
                // do not pass symbols like !@#$ through
                return false;
            } else {
                // do pass unshifted numbers through
                return true;
            }
        default:
            return false;
    }
}


/**
 * @brief Add a keycode to the exclusion list.
 * @param keycode The keycode to add.
 */
 void add_xcase_exclusion_keycode(uint16_t keycode) {
    if (exclusion_keycode_count >= MAX_EXCLUSION_KEYCODES) {
        return;  // List is full
    }
    if (is_xcase_exclusion_keycode(keycode)) {
        return;  // Already in list
    }
    exclusion_keycodes[exclusion_keycode_count++] = keycode;
}


/**
 * @brief Remove a keycode from the exclusion list.
 * @param keycode The keycode to remove.
 */
 void remove_xcase_exclusion_keycode(uint16_t keycode) {
    for (uint8_t i = 0; i < exclusion_keycode_count; i++) {
        if (exclusion_keycodes[i] == keycode) {
            // Shift remaining elements down
            for (uint8_t j = i; j < exclusion_keycode_count - 1; j++) {
                exclusion_keycodes[j] = exclusion_keycodes[j + 1];
            }
            exclusion_keycode_count--;
            return;
        }
    }
}


/**
 * @brief Enable xcase with a given delimiter.
 * @param delimiter The keycode to use as a delimiter.
 */
void enable_xcase_with(uint16_t delimiter) {
    // 1. Analyze Modifiers
    // Extract the high bits (modifiers) from the keycode
    uint16_t mods = delimiter & ~0xFF;

    // Check if the modifiers are "Visual" (Shift, Option, or Shift+Option)
    // QK_LALT = Left Option (Mac)
    // QK_RALT = Right Option (Mac)
    // QK_LSFT = Shift
    bool is_visual_mod = (mods == QK_LSFT) ||
                         (mods == QK_LALT) ||
                         (mods == QK_RALT) ||           // also AltGr
                         (mods == (QK_LSFT | QK_LALT)); // Shift+Opt (e.g. )

    // 2. Rejection Logic: Stage 1
    // If it's NOT a basic key AND it doesn't have valid visual modifiers, reject it.
    if (!IS_QK_BASIC(delimiter) && !is_visual_mod) {
        return;
    }

    // 2. Rejection Logic: Stage 2
    // Specific keycode rejections and handling
    switch (delimiter) {
        // special handling for camelCase
        case KC_LSFT:
        case KC_RSFT:
        case OS_LSFT:
        case OS_RSFT:
            // simplify shifts to KC_LSFT
            xcase_delimiter = KC_LSFT;
            break;

        // unacceptable delimiters
#ifdef TRI_LAYER_ENABLE // Ignore Tri Layer keys.
        case QK_TRI_LAYER_LOWER ... QK_TRI_LAYER_UPPER:
#endif
#ifdef LAYER_LOCK_ENABLE // Ignore Layer Lock key.
        case QK_LAYER_LOCK:
#endif
        case KC_NO:
        case KC_F1 ... KC_F12:
        case KC_F13 ... KC_LPAD:
        case KC_BACKSPACE:
        case KC_DEL:
        case KC_HOME:
        case KC_END:
        case KC_PGUP:
        case KC_PGDN:
        case KC_PSCR:
        case KC_SCRL:
        case KC_PAUS:
        case KC_INS:
        case KC_NUM:
        case KC_ESC:
        case KC_LCTL:
        case KC_RCTL:
        case KC_LCMD:
        case KC_RCMD:
        case KC_SPACE:  // ...seriously?
            return;  // do not pass go, do not col-lect $200

        // use the provided delimiter directly
        default:
            xcase_delimiter = delimiter;
            break;
        }

    last_keycode = KC_NO;
    xcase_active = true;
    add_xcase_exclusion_keycode(delimiter);
}


/**
 * @brief Disable xcase.
 */
void disable_xcase(void) {
    xcase_active = false;
    last_keycode = KC_NO;
    remove_xcase_exclusion_keycode(xcase_delimiter);
}


/**
 * @brief Get the current xcase delimiter keycode.
 */
uint16_t get_xcase_delimiter(void) {
    return xcase_delimiter;
}

/**
 * @brief Get the last keycode processed while xcase was active.
 */
uint16_t get_xcase_last_keycode(void) {
    return last_keycode;
}

/**
 * @brief Set the last keycode processed while xcase was active.
 */
void set_xcase_last_keycode(uint16_t kc) {
    last_keycode = kc;
}
