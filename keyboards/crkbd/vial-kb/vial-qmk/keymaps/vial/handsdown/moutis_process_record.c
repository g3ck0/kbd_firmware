/*

 Set keyboard on host to ANSI (not ISO/JIS for now. will eventually invert this.)
 
 */


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    bool return_state = true;

    saved_mods = get_mods(); // preserve mods

    // Do we need to filter multi-function keys?
    // switch (keycode) {
    //     case QK_MOD_TAP ... QK_MOD_TAP_MAX:
    //     case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
    //         if (!record->tap.count) // if not tapped yet…
    //             return true; // let QMK do that first
    //         keycode &= QK_BASIC_MAX; // mods & taps have been handled.
    // }
    
    // // Do we turn off CAPS_WORD?
    // if (caps_word_timer) {
    //     if (!process_caps_word(keycode, record)) {
    //         return false; // took care of that key
    //     }
    // }

    // Do we handle a semantic key? Combos or adaptives could have sent one.
    if (!process_semkey(keycode, record)) {
        return false; // took care of that key
    }
    
//     // only process for SHIFT/ALT & no CTRL or GUI mods
//     if (saved_mods & MOD_MASK_CG)  // CTRL or GUI/CMD?
//         return true; // do default if CTRL or GUI/CMD are down

//     if (record->event.pressed) {
// //        switch (((keycode >= SAFE_RANGE) && (keycode <= SemKeys_COUNT)) ? (keycode) : (keycode & QK_BASIC_MAX)) { // only handling normal, SHFT or ALT cases.
//         switch (keycode) { // only handling normal, SHFT or ALT cases.

//            case SK_Win: // SINCE MAC IS MY LAYOUT DEFAULT switch to windows
//                 user_config.OSIndex = OS_Win; // for Windows Semkeys
//                 keymap_config.swap_lctl_lgui = keymap_config.swap_rctl_rgui = true;
//                 return_state = false; // stop processing this record.
//                 goto storeSettings;
//             case SK_Mac: // Back to default
//                 user_config.OSIndex = OS_Mac; // for Mac Semkeys
//                 keymap_config.swap_lctl_lgui = keymap_config.swap_rctl_rgui = false;
//                 return_state = false; // stop processing this record.
//                 goto storeSettings;
// storeSettings:
//                 eeconfig_update_user(user_config.raw); // Remember platform variables after powerdown
//                 break;
                
//         } // switch (keycode) {   
//     } 

    return return_state;  // keep processing record
}


