#pragma once

#include <quantum.h>


typedef union {
    uint32_t raw;
    struct {
        uint8_t OSIndex; // index of platforms (0=mac, 1=win, 2=lux)? // used by semantickeys
        bool AdaptiveKeys; // Adaptive Keys On? (and advanced combos)
    };
} user_config_t; // used for persistent memory of settings (only 16 bytes avail on AVR?)


enum OS_Platform { // Used for platform support via SemKeys
    OS_Mac,     // Mac with ANSI_US_EXTENDED layout
    OS_Win,     // Win with default English/ANSI layout?
    OS_count
};

#include "moutis_semantickeys.h"

#define tap_HDkey(kc) ({is_SemKey(kc) ? tap_SemKey(kc) : tap_code16(kc);})
#define register_HDkey(kc) ({is_SemKey(kc) ? register_SemKey(kc) : register_code16(kc);})
#define unregister_HDkey(kc) ({is_SemKey(kc) ? unregister_SemKey(kc) : unregister_code16(kc);})

#define register_linger_key(kc) ({register_HDkey(kc);linger_key = kc;linger_timer = timer_read();})
#define unregister_linger_key() ({unregister_HDkey(linger_key) ;linger_key = 0;})

// void (void);

#include "../feature/modprocessrecord.h"