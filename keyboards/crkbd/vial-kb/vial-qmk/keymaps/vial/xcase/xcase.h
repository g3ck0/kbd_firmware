#pragma once

#include "quantum.h"

#define XCASE_SNAKE QK_KB_3
#define XCASE_KEBAB QK_KB_4   
#define XCASE_CAMEL QK_KB_5  
#define XCASE_OFF QK_KB_6
/**
 * @brief Enable xcase with a given delimiter.
 * @param delimiter The keycode to use as a delimiter.
 */
void enable_xcase_with(uint16_t delimiter);

/**
 * @brief Disable xcase.
 */
void disable_xcase(void);

/**
 * @brief Check if xcase is active.
 * @return True if xcase is active, false otherwise.
 */
bool is_xcase_active(void);

/**
 * @brief Add a keycode to the exclusion list
 * (keys that will not trigger the end of xcase).
 * @param keycode The keycode to add.
 */
void add_xcase_exclusion_keycode(uint16_t keycode);

/**
 * @brief Remove a keycode from the exclusion list
 * (keys that will not trigger the end of xcase).
 * @param keycode The keycode to remove.
 */
void remove_xcase_exclusion_keycode(uint16_t keycode);

/**
 * @brief Check if a keycode is on the user's exclusion list
 * (keys that will not trigger the end of xcase).
 * @param keycode The keycode to check.
 * @return True if the keycode is on the user's exclusion list, false otherwise.
 */
bool is_xcase_exclusion_keycode(uint16_t keycode);

/**
 * @brief Get the current xcase delimiter keycode.
 * @return The current delimiter (e.g. KC_UNDS, KC_MINS, KC_LSFT).
 */
uint16_t get_xcase_delimiter(void);

/**
 * @brief Get the last keycode processed while xcase was active.
 * @return The last base keycode (used for double-space detection).
 */
uint16_t get_xcase_last_keycode(void);

/**
 * @brief Set the last keycode processed while xcase was active.
 * @param kc The keycode to store as last processed.
 */
void set_xcase_last_keycode(uint16_t kc);

/**
 * @brief Process a keypress with xcase.
 * @param keycode The keycode of the keypress.
 * @param record The keyrecord to process.
 * @return True if the keypress is returned to the caller, false otherwise.
 */
// bool process_record_kb(uint16_t keycode, keyrecord_t *record);
