# KBD firmware

## How to build

## 1. Setting Up Your QMK Environment

Please see https://docs.qmk.fm/#/newbs_getting_started and set up 1 to 3.

## 2. Getting source files

Please get source files of `qmk/qmk_firmware` and `vial-kb/vial-qmk`
```sh
make git-submodule
```

## 3. Building firmwares

### for VIA

```sh
make qmk-clean
kb=crkbd make qmk-init
kb=crkbd kr=rev4_1/standard km=via make qmk-compile
```
A built data will be stored on `keyboards/crkbd/qmk/qmk_firmware/.build`\
Please change `kb`, `kr` and `km` when build other.

### for Vial
```sh
make vial-qmk-clean
kb=crkbd make vial-qmk-init
kb=crkbd kr=rev4_1/standard km=vial make vial-qmk-compile
```
A built data will be stored on `keyboards/crkbd/vial-kb/vial-qmk/.build`\
Please change `kb`, `kr` and `km` when build other.

### All cleaning and building
```sh
make update-all
```


## 4. Lighting

To be able to use ligting per layer, and also to identify when Caps Locks/Caps Words is active:

This keyboard uses RGB MATRIX that is why we enable it.

>rules.mk

```
#to enable rgb matrix to use them
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = yes
```

>config.h

With this configuration we define 10 layers to use

```
// Define Layers to 10
#undef DYNAMIC_KEYMAP_LAYER_COUNT
#define DYNAMIC_KEYMAP_LAYER_COUNT 10
```

In this part (it is not the complete configuration of the file)

```
#ifdef RGB_MATRIX_ENABLE

#   undef RGB_MATRIX_SPLIT
#   define RGB_MATRIX_SPLIT { 17, 17 }
#   define SPLIT_TRANSPORT_MIRROR
#   define SPLIT_LAYER_STATE_ENABLE
#   define SPLIT_MODS_ENABLE


#    define RGB_DISABLE_WHEN_USB_SUSPENDED true  // turn off effects when suspended
#    undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 100
#    define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS // Sets the default brightness value, if none has been set
```

>keymap.c

These functions help us to identify when the Caps Lock/Caps Word are active or we change of layer and then call the function that apply the colors/effects

```
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
```

This is the function that applies the color changes:

```
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
```
For this version i used the same effect for all the layers, just changed the color used for that effect.