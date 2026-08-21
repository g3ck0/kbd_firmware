# Migrar `del_latch` a un modo de caps_word

## Factibilidad

**Sí, es posible.** El enum `caps_word_mode_t` puede recibir un nuevo valor `CWMODE_DELETE_NAV` que reemplaza el booleano `del_latch_active`. La lógica de `process_semkey` en `moutis_semantickeys.c` cambia de leer `del_latch_is_active()` a comprobar `is_caps_word_on() && g_caps_word_mode == CWMODE_DELETE_NAV`.

---

## Comportamiento actual de `del_latch_active`

```
DEL_LATCH presionado
  → del_latch_active = true
  → SK_WORDPRV → SK_DELWDL (Delete Word Left)
  → SK_WORDNXT → SK_DELWDR (Delete Word Right)
  → SK_LINEBEG → SK_DELLNL (Delete Line Left)
  → SK_LINEEND → SK_DELLNR (Delete Line Right)

DEL_LATCH presionado de nuevo, o Esc
  → del_latch_active = false
  → teclas SK_* vuelven a su comportamiento normal
```

El latch **no se desactiva** al presionar teclas SK — es un toggle persistente.

---

## Diferencia semántica importante — caps_word vs. latch

| Aspecto | `del_latch_active` actual | `CWMODE_DELETE_NAV` en caps_word |
|---|---|---|
| Activación | Toggle con `DEL_LATCH` | `DEL_LATCH` → `caps_word_on()` |
| Desactivación | Solo `DEL_LATCH` de nuevo o `Esc` | `DEL_LATCH` de nuevo, `Esc`, **o timeout de caps_word** |
| Teclas que lo mantienen activo | Cualquier tecla (no se desactiva solo) | Solo las que `caps_word_press_user` deja pasar |
| Esc | Lo cancela | caps_word ya cancela automáticamente |

> [!IMPORTANT]
> La diferencia clave está en `caps_word_press_user`: necesita retornar `true` para **todos** los SK_* keys y también para modificadores y teclas de navegación, de lo contrario caps_word se desactivaría sola al presionar cualquier tecla no listada.

---

## Proposed Changes

### `feature/caps_word_modes.h` [MODIFY]

Agregar el nuevo modo al enum:

```diff
 typedef enum {
     CWMODE_NORMAL = 0,
     CWMODE_ARROW_SHIFT,
     CWMODE_SPACE_SUB,
     CWMODE_CAMEL_CASE,
+    CWMODE_DELETE_NAV,   // del_latch: SK_WORD*/SK_LINE* → borrar en lugar de navegar
 } caps_word_mode_t;
```

---

### `feature/caps_word_modes.c` [MODIFY]

Agregar el caso en `caps_word_press_user`:

```c
case CWMODE_DELETE_NAV:
    // Dejar pasar todas las SK_* keys y modificadores
    // caps_word permanece activo mientras se usen estas teclas
    switch (keycode) {
        case SK_WORDPRV:
        case SK_WORDNXT:
        case SK_LINEBEG:
        case SK_LINEEND:
        // también flechas, BSPC, DEL para no interrumpir el flujo
        case KC_LEFT:
        case KC_RIGHT:
        case KC_UP:
        case KC_DOWN:
        case KC_BSPC:
        case KC_DEL:
        // modificadores (para no romper combos)
        case KC_LSFT ... KC_RGUI:
            return true;
        default:
            return false; // cualquier otra tecla desactiva
    }
```

---

### `feature/modprocessrecord.c` [MODIFY]

**Agregar** el caso para `DEL_LATCH`:

```diff
 case DEL_LATCH:
-    if (del_latch_active) {
-        del_latch_off();
-    } else {
-        del_latch_active = true;
-    }
+    if (is_caps_word_on() && g_caps_word_mode == CWMODE_DELETE_NAV) {
+        caps_word_off();  // toggle: desactivar si ya está en ese modo
+    } else {
+        caps_word_set_mode(CWMODE_DELETE_NAV);
+        caps_word_on();
+    }
    return false;
```

**Eliminar** las variables y funciones de `del_latch`:

```diff
-static bool del_latch_active = false;
-
-void del_latch_off(void) { ... }
-bool del_latch_is_active(void) { ... }
```

**Actualizar** el bloque de `KC_ESC` (caps_word ya lo cancela, pero por si acaso):

```diff
 case KC_ESC:
     sel_latch_off();
-    del_latch_off();
     break;
```

---

### `handsdown/moutis_semantickeys.c` [MODIFY]

**Agregar** include de `caps_word_modes.h`:

```diff
 #include "../feature/modprocessrecord.h"
+#include "../feature/caps_word_modes.h"
```

**Reemplazar** la comprobación de `del_latch_is_active()`:

```diff
-bool del_latch_active = del_latch_is_active();
+bool del_latch_active = is_caps_word_on() &&
+                        g_caps_word_mode == CWMODE_DELETE_NAV;
```

El resto de la lógica del `if (del_latch_active && ...)` queda **exactamente igual**.

---

### `feature/modprocessrecord.h` [MODIFY]

Eliminar las declaraciones que ya no existen:

```diff
-void del_latch_off(void);
-bool del_latch_is_active(void);
```

---

### `feature/ligth.c` [MODIFY] — opcional

Agregar color para el nuevo modo:

```diff
 case CWMODE_ARROW_SHIFT: rgb_matrix_sethsv_noeeprom(HSV_CYAN);    break;
+case CWMODE_DELETE_NAV:  rgb_matrix_sethsv_noeeprom(HSV_RED);     break;
```

---

## Ventajas del cambio

- **Un solo sistema de estado**: `g_caps_word_mode` controla todo — no más booleans sueltos
- **Feedback visual gratis**: el RGB ya muestra el modo activo
- **Esc cancela automáticamente**: caps_word ya tiene ese comportamiento
- **Timeout opcional**: si se quiere que el delete-nav expire automáticamente, `CAPS_WORD_IDLE_TIMEOUT` en `config.h` lo maneja sin código extra

## Open Questions

> [!NOTE]
> **¿Quieres que `CWMODE_DELETE_NAV` expire por timeout o que sea persistente como el latch actual?**
> - Si **persistente**: agregar `#define CAPS_WORD_IDLE_TIMEOUT 0` en `config.h` (deshabilita el timeout) o hacer `caps_word_press_user` retornar `true` para **todas** las teclas en ese modo.
> - Si **con timeout**: el comportamiento existente de caps_word aplica automáticamente.

> [!NOTE]
> **¿El `DEL_LATCH` keycode permanece o se renombra?**
> El keycode puede quedarse con el mismo nombre — solo cambia su implementación interna. No requiere cambios en `custom_keycodes.h` ni en `vial.json`.
