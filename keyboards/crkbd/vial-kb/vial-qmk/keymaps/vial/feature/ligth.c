
// Function to set RGB based on current state
void update_rgb_state(void) {
    bool caps_word_active = is_caps_word_on();
    bool caps_lock_active = host_keyboard_led_state().caps_lock;

    if (caps_word_active || caps_lock_active) {
        // Override with caps indication
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
        if (caps_word_active) {
            rgb_matrix_sethsv_noeeprom(HSV_YELLOW); // Yellow for Caps Word
        } else {
            rgb_matrix_sethsv_noeeprom(HSV_WHITE); // White for Caps Lock
        }
        return;
    }

    // Set RGB based on current layer
    switch (get_highest_layer(layer_state)) {
        case 0:// Default layer
            rgb_matrix_mode_noeeprom(RGB_MATRIX_TYPING_HEATMAP);
            rgb_matrix_sethsv_noeeprom(HSV_BLUE);
            break;
        case 1:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_SMOOTH);
            rgb_matrix_sethsv_noeeprom(HSV_RED);
            break;
        case 2:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_SMOOTH);
            rgb_matrix_sethsv_noeeprom(HSV_BLUE);
            break;
        case 3:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_SMOOTH);
            rgb_matrix_sethsv_noeeprom(HSV_PURPLE);
            break;
        case 4:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_SMOOTH);
            rgb_matrix_sethsv_noeeprom(HSV_MAGENTA);
            break;
        case 5:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_SMOOTH);
            rgb_matrix_sethsv_noeeprom(HSV_AZURE);
            break;
        case 6:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_SMOOTH);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
            break;
        case 7:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_SMOOTH);
            rgb_matrix_sethsv_noeeprom(HSV_CHARTREUSE);
            break;
        case 8:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_SMOOTH);
            rgb_matrix_sethsv_noeeprom(HSV_SPRINGGREEN);
            break;
        case 9:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_SMOOTH);
            rgb_matrix_sethsv_noeeprom(HSV_TEAL);
            break;
    }
}

bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) {
        return false;
    }
    return false;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    update_rgb_state();
    return state;
}

// Handle caps lock state changes
bool led_update_user(led_t led_state) {
    update_rgb_state();
    return true;
}

// Handle caps word state changes
void caps_word_set_user(bool active) {
    update_rgb_state();
}