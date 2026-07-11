
//#include QMK_KEYBOARD_H

#include <stdio.h>

#include "moutis.h"


/*
    My globals
*/
user_config_t user_config;
uint8_t  saved_mods = 0; // to pass state between process_record_user and matrix_scan_user
uint16_t record_keycode = 0; // the keykcode we poke at in process_record
// key_trap is used to intercept keycodes to do KEY_OVERRIDE stuff in userspace.
// since QMK's KEY_OVERRIDE can't integrate Semantic Keys or lingers.
uint16_t key_trap = 0; // the actual keycode registered (need to unregister)

uint16_t linger_key = 0;  // keycode for linger actions (ex. "Qu")
uint32_t linger_timer = 0; // time to hold a key before something else happens.
uint32_t appmenu_timer = 0;  // time to leave appmenu active before shutting it down automatically.
bool appmenu_on = false;  // appmenu triggered (after holding key)
bool mods_held = false;  // need to remember how we entered the appmenu state

uint16_t L_quote = KC_DQUO; // default " (「 in Japanese mode)
uint16_t R_quote = KC_QUOT; // default ' (」 in Japanese mode)


uint8_t  combo_on = 0;           // for combo actions with hold behaviors
bool  combo_triggered = false;   // for one-shot-combo-actions

void eeconfig_init_user(void) {
    // Llamado cuando el EEPROM se resetea (ej. primer arranque o EE_CLR)
    user_config.raw = 0;
    user_config.OSIndex = OS_Win; // Windows como default
    user_config.AdaptiveKeys = false; // o tu default preferido
    eeconfig_update_user(user_config.raw);
}

void keyboard_post_init_user(void) {
    // Read the user config from EEPROM to facilitate
    // appropriate platform support

    // user_config.raw = 0; // reset
    // eeconfig_update_user(user_config.raw); // write the setings to EEPROM
    /*
    user_config.OSIndex use 0 for Mac; 1 for Win
     */
    user_config.raw = eeconfig_read_user();
    eeconfig_read_default_layer(); // get the default layer from eeprom.

}
