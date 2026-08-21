# Implementar caps_word extendido — **sin xcase ni modcase**

## Contexto y objetivo

Eliminar completamente `xcase/` y `feature/modcase.c` y absorber toda su funcionalidad dentro de los **modos extendidos de caps_word** inspirados en el repo `Ga68/sofle`.

---

## ¿Qué aporta cada archivo a eliminar?

### `xcase/xcase.c` + `xcase/xcase.h`
| Función | Dónde se usa actualmente | Reemplazo |
|---|---|---|
| `enable_xcase_with(KC_UNDS)` | `modprocessrecord.c`, `modcase.c` | `CW_SNAKE` → `caps_word_set_mode(CWMODE_SPACE_SUB) + g_caps_word_space_substitute = KC_UNDS` |
| `enable_xcase_with(KC_MINS)` | `modprocessrecord.c`, `modcase.c` | `CW_KEBAB` → `caps_word_set_mode(CWMODE_SPACE_SUB) + g_caps_word_space_substitute = KC_MINS` |
| `enable_xcase_with(KC_LSFT)` | `modprocessrecord.c`, `modcase.c` | `CW_CAMEL` → `caps_word_set_mode(CWMODE_CAMEL_CASE)` |
| `disable_xcase()` | `modprocessrecord.c` | automático al salir de caps_word |
| `is_xcase_active()` | `modprocessrecord.c` | eliminado (toda la lógica se mueve a `caps_word_press_user`) |
| `get_xcase_delimiter()`, `set_xcase_last_keycode()`, etc. | `modprocessrecord.c` | internalizados en `caps_word_modes.c` |

### `feature/modcase.c` + `feature/modcase.h`
| Función | Dónde se usa | Reemplazo |
|---|---|---|
| `tap_accented(lower, upper)` | `modprocessrecord.c` | **se mueve** a `caps_word_modes.c` (o se queda en `modprocessrecord.c`) |
| `leader_start_user()` | QMK hook | ❌ **se elimina** (sin Leader key) |
| `leader_end_user()` (Leader+S/K/C) | QMK hook | ❌ **se elimina** — activación solo por keycodes directos |

---

## Archivos afectados

```
[DELETE]  xcase/xcase.c
[DELETE]  xcase/xcase.h
[DELETE]  feature/modcase.c
[DELETE]  feature/modcase.h
[MODIFY]  config.h                           ← eliminar LEADER_TIMEOUT
[NEW]     feature/caps_word_modes.h
[NEW]     feature/caps_word_modes.c
[MODIFY]  feature/modprocessrecord.c   ← reescribir (eliminar toda referencia a xcase)
[MODIFY]  keymap.c                     ← eliminar #include xcase/xcase.h y modcase.h
[MODIFY]  custom_keycodes.h            ← eliminar XCASE_* , agregar CW_* 
[MODIFY]  rules.mk                     ← eliminar xcase/xcase.c y modcase.c, agregar caps_word_modes.c
```

---

## Proposed Changes

---

### 1 — `feature/caps_word_modes.h` [NEW]

```c
#pragma once
#include "quantum.h"

typedef enum {
    CWMODE_NORMAL = 0,      // Caps Word estándar (letras en MAYÚS)
    CWMODE_ARROW_SHIFT,     // Flechas + Shift automático (selección de texto)
    CWMODE_SPACE_SUB,       // Sustituye espacio por carácter (snake/kebab/slash)
    CWMODE_CAMEL_CASE,      // camelCase: capitaliza letra tras espacio
} caps_word_mode_t;

#define CAPS_WORD_MODE_DEFAULT      CWMODE_NORMAL
#define CAPS_WORD_SPACE_SUB_DEFAULT KC_UNDS

extern caps_word_mode_t g_caps_word_mode;
extern uint16_t         g_caps_word_space_substitute;

void caps_word_set_mode(caps_word_mode_t mode);

// Migrado desde modcase.h (sin dependencia de leader)
void tap_accented(uint16_t lower, uint16_t upper);
```

---

### 2 — `feature/caps_word_modes.c` [NEW]

Concentra:
- Variables globales de modo
- `caps_word_press_user()` con lógica de los 4 modos
- `tap_accented()` migrado de `modcase.c`
- ~~`leader_start_user()` / `leader_end_user()`~~ → **eliminados**, la activación de modo es solo por keycodes directos en el keymap

**`caps_word_press_user` — lógica de modos:**

```c
// CWMODE_NORMAL: comportamiento estándar QMK
//   A–Z → con Shift, 0–9/BSPC/MINS/UNDS → sin shift, resto desactiva

// CWMODE_SPACE_SUB:
//   A–Z → con Shift (MAYÚSCULAS)
//   Espacio → tap_code16(g_caps_word_space_substitute), return false (no envía espacio)
//   BSPC, 0–9, UNDS → continúan sin shift
//   Doble espacio → desactiva

// CWMODE_CAMEL_CASE:
//   A–Z → con Shift (primera letra de cada palabra)
//   Espacio → add_oneshot_mods(MOD_BIT(KC_LSFT)), return false
//   BSPC, 0–9 → continúan

// CWMODE_ARROW_SHIFT:
//   ←↑→↓ y variantes Alt/Gui → con Shift (selección)
//   resto desactiva
```

> [!NOTE]
> **Diferencia vs xcase original**: xcase usaba sus propias variables `xcase_active`, `last_keycode`, etc. El nuevo sistema usa las variables internas del QMK native caps_word (`caps_word_on()` / `caps_word_off()` / `is_caps_word_on()`) como estado único, y `g_caps_word_mode` como discriminador.

---

### 3 — `feature/modprocessrecord.c` [MODIFY] — Reescritura

**Eliminar:**
- `#include "../xcase/xcase.h"` 
- Todo el bloque `if (!is_xcase_active()) { return true; }` al final
- Todo el bloque de lógica xcase (detección de doble espacio, send delimiter, `set_xcase_last_keycode`, etc.)
- `XCASE_SNAKE`, `XCASE_KEBAB`, `XCASE_CAMEL`, `XCASE_OFF` del switch de keycodes

**Agregar:**
- `#include "caps_word_modes.h"`
- Nuevos casos en el switch:

```c
case CW_NORMAL:
    caps_word_set_mode(CWMODE_NORMAL);
    caps_word_on();
    return false;
case CW_ARW_SFT:
    caps_word_set_mode(CWMODE_ARROW_SHIFT);
    caps_word_on();
    return false;
case CW_SNAKE:
    g_caps_word_space_substitute = KC_UNDS;
    caps_word_set_mode(CWMODE_SPACE_SUB);
    caps_word_on();
    return false;
case CW_KEBAB:
    g_caps_word_space_substitute = KC_MINS;
    caps_word_set_mode(CWMODE_SPACE_SUB);
    caps_word_on();
    return false;
case CW_CAMEL:
    caps_word_set_mode(CWMODE_CAMEL_CASE);
    caps_word_on();
    return false;
```

**`process_record_user` queda mucho más corto**: solo maneja compose, latch keys, Esc, MOD_ACCENT y los nuevos CW_* keycodes.

---

### 4 — `custom_keycodes.h` [MODIFY]

```diff
 enum custom_keycodes {
-    XCASE_SNAKE = QK_KB_3,
-    XCASE_KEBAB,
-    XCASE_CAMEL,
-    XCASE_OFF,
+    CW_NORMAL = QK_KB_3,  // Caps Word modo normal
+    CW_ARW_SFT,           // Caps Word modo arrow-shift
+    CW_SNAKE,             // Caps Word snake_case (space → _)
+    CW_KEBAB,             // Caps Word kebab-case (space → -)
+    CW_CAMEL,             // Caps Word camelCase
     SEL_LATCH,
     DEL_LATCH,
     MOD_ACCENT,
     // ... resto sin cambios
 };
```

---

### 5 — `keymap.c` [MODIFY]

```diff
-#include "feature/modcase.h"
-#include "xcase/xcase.h"
+#include "feature/caps_word_modes.h"
```

Los keycodes `XCASE_SNAKE/KEBAB/CAMEL/OFF` en el keymap se reemplazan por `CW_SNAKE/KEBAB/CAMEL/NORMAL`.

> [!IMPORTANT]
> **¿Dónde poner los CW_* en el layout?**
> Actualmente los `XCASE_*` están en la Layer 2 (símbolos), según el vial.json. ¿Los mantenemos ahí, o los movemos a Layer 6 (vacía) como "capa de modos de escritura"?

---

### 6 — `rules.mk` [MODIFY]

```diff
+CAPS_WORD_ENABLE = yes
-LEADER_ENABLE         = yes
 SRC += feature/caps_word_modes.c
-SRC += xcase/xcase.c
-SRC += feature/modcase.c
 SRC += feature/ligth.c
 SRC += feature/modprocessrecord.c
 SRC += handsdown/moutis_semantickeys.c
```

---

### 7 — `config.h` [MODIFY]

Eliminar la directiva de Leader key:

```diff
-#define LEADER_TIMEOUT 350
-// #define LEADER_PER_KEY_TIMING
```

---

### 8 — `feature/ligth.c` [MODIFY] (opcional — colores por modo)

```c
if (caps_word_active) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_PINWHEEL);
    switch (g_caps_word_mode) {
        case CWMODE_NORMAL:     rgb_matrix_sethsv_noeeprom(HSV_YELLOW); break;
        case CWMODE_ARROW_SHIFT:rgb_matrix_sethsv_noeeprom(HSV_CYAN);   break;
        case CWMODE_SPACE_SUB:  rgb_matrix_sethsv_noeeprom(HSV_ORANGE); break;  // snake/kebab
        case CWMODE_CAMEL_CASE: rgb_matrix_sethsv_noeeprom(HSV_GREEN);  break;
    }
}
```

---

## Verification Plan

### Build
```bash
make crkbd/vial-kb/vial-qmk:vial
```

### Manual
1. **CW_SNAKE** → escribe `hello world` → debe producir `HELLO_WORLD`
2. **CW_KEBAB** → escribe `hello world` → debe producir `HELLO-WORLD`
3. **CW_CAMEL** → escribe `hello world` → debe producir `helloWorld`
4. **CW_NORMAL** → mayúsculas normales, Esc desactiva
5. `MOD_ACCENT` + vocal → tilde correcta (verifica que `tap_accented` migró bien)
6. `SEL_LATCH` sigue funcionando en la capa de navegación
7. Verificar que la tecla Leader (QK_LEAD) ya **no está** en ninguna capa del keymap

> [!WARNING]
> `vial.json` también referencia keycodes por nombre/valor numérico. Si cambias de `XCASE_*` a `CW_*` en `custom_keycodes.h`, el `.json` puede quedar desincronizado con los valores de `QK_KB_X`. Hay que verificar que el orden en el enum coincida o actualizar el JSON.
