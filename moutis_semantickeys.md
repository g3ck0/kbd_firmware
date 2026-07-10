# Semantic Keys — `moutis_semantickeys.c`

## Modificadores (QMK)

| Macro | Combinación | Mac | Windows |
|-------|------------|-----|---------|
| `G(kc)` | GUI | ⌘ | ⊞ Win |
| `C(kc)` | Control | ^ | ^ |
| `A(kc)` | Alt | ⌥ | Alt |
| `S(kc)` | Shift | ⇧ | ⇧ |
| `LAG(kc)` | LAlt + LGUI | ⌥⌘ | Alt+⊞ |
| `LCA(kc)` | LCtrl + LAlt | ^⌥ | ^Alt |
| `LSA(kc)` | LShift + LAlt | ⇧⌥ | ⇧Alt |
| `LSG(kc)` | LShift + LGUI | ⇧⌘ | ⇧⊞ |
| `RSG(kc)` | RShift + RGUI | ⇧⌘ | ⇧⊞ |
| `ALGR(kc)` | AltGr (RAlt) | (n/a) | AltGr |

---

## Tabla completa de SemKeys

### System-wide controls

| Key | Mac | Windows | Acción |
|-----|-----|---------|--------|
| `SK_KILL` | ⌥⌘⎋ | ^⌅Del | Force quit / Ctrl-Alt-Del |
| `SK_HENK` | 変換 | ^⇧1 | 変換/かな |
| `SK_MHEN` | 無変換 | ^⇧0 | 無変換/英数 |
| `SK_DKT8` | ^⇧3 | ⌘H | Dictate speech to text |
| `SK_AIVC` | ^⇧4 | ⌘C | AI voice control (Siri/Cortana) |

### Extended characters / editing commands

| Key | Mac | Windows | Acción |
|-----|-----|---------|--------|
| `SK_HENT` | ⌘⏎ | ^⏎ | Hard ENTER |
| `SK_UNDO` | ⌘Z | ^Z | Undo |
| `SK_REDO` | ⌘⇧Z | ^⇧Z | Redo |
| `SK_CUT` | ⌘X | ^X | Cut |
| `SK_COPY` | ⌘C | ^C | Copy |
| `SK_PSTE` | ⌘V | ^V | Paste |
| `SK_PSTM` | ⌘⇧⌥V | ^⇧⌥V | Paste Match Style |
| `SK_SALL` | ⌘A | ^A | Select All |
| `SK_CLOZ` | ⌘W | ^W | Close tab/window |
| `SK_QUIT` | ⌘Q | ^Q | Quit app |
| `SK_NEW` | ⌘N | ^N | New |
| `SK_OPEN` | ⌘O | ^O | Open |
| `SK_FIND` | ⌘F | ^F | Find |
| `SK_FAGN` | ⌘G | ^G | Find Again |
| `SK_SCAP` | ⇧⌘4 | PrtSc | Screen Capture |
| `SK_SCLP` | ^⇧⌘4 | ⌥PrtSc | Selection Capture |
| `SK_SRCH` | ⌘␣ | ⌘S | Platform search (Siri/Cortana) |
| `SK_DELWDL` | ⌥⌫ | ^⌫ | Delete Word Left |
| `SK_DELWDR` | ⌥⌦ | ^⌦ | Delete Word Right |
| `SK_DELLNL` | ⌘⌫ | ^⌫ | Delete line left of cursor |
| `SK_DELLNR` | ⌘⌦ | ^⌦ | Delete line right of cursor |

### Extended navigation

| Key | Mac | Windows | Acción |
|-----|-----|---------|--------|
| `SK_WORDPRV` | ⌥← | ^← | Word Left |
| `SK_WORDNXT` | ⌥→ | ^→ | Word Right |
| `SK_DOCBEG` | ⌘↑ | ^Home | Start of document |
| `SK_DOCEND` | ⌘↓ | ^End | End of document |
| `SK_LINEBEG` | ⌘↓ | ^End | ⚠ **Bug**: mismo mapping que `SK_DOCEND` |
| `SK_LINEEND` | ⌘↓ | ^End | ⚠ **Bug**: mismo mapping que `SK_DOCEND`/`SK_LINEBEG` |
| `SK_PARAPRV` | ⌥↑ | ^↑ | Previous paragraph |
| `SK_PARANXT` | ⌥↓ | ^↓ | Next paragraph |
| `SK_HISTPRV` | ⌘[ | ⌥← | Browser Back |
| `SK_HISTNXT` | ⌘] | ⌥→ | Browser Forward |
| `SK_ZOOMIN` | ⌘= | ^= | Zoom In |
| `SK_ZOOMOUT` | ⌘- | ^- | Zoom Out |
| `SK_ZOOMRST` | ⌘0 | ^0 | Zoom Reset |
| `SK_APPNXT` | ⌘⇥ | ⌥⇥ | App switcher (last used) |
| `SK_APPPRV` | ⇧⌘⇥ | ⇧⌥⇥ | App switcher (least recent) |
| `SK_WINNXT` | ^⇥ | ^⇥ | Window/tab switcher next |
| `SK_WINPRV` | ^⇧⇥ | ^⇧⇥ | Window/tab switcher previous |

### Punctuation & typography

| Key | Mac | Windows | Acción |
|-----|-----|---------|--------|
| `SK_NDSH` | ⌥- | Alt+0150 | – N-Dash |
| `SK_MDSH` | ⇧⌥- | Alt+0151 | — M-Dash |
| `SK_ELPS` | ⌥; | Alt+0133 | … |
| `SK_SCRS` | ⇧⌥5 | Alt+0134 | † Single Cross |
| `SK_DCRS` | ⇧⌥7 | Alt+0135 | ‡ Double Cross |
| `SK_BBLT` | ⌥8 | Alt+0149 | • Bold Bullet |
| `SK_SBLT` | ⇧⌥9 | Alt+0183 | · Small Bullet |
| `SK_PARA` | ⌥7 | Alt+0182 | ¶ |
| `SK_SECT` | ⌥5 | Alt+0167 | § |

### Number & Math symbols

| Key | Mac | Windows | Acción |
|-----|-----|---------|--------|
| `SK_PERM` | ⇧⌥R | Alt+0137 | ‰ Per Mille |
| `SK_DEGR` | ⇧⌥8 | Alt+0176 | ° Degree |
| `SK_GTEQ` | ⌥. | Alt+8805 (0x4242) | ≥ |
| `SK_LTEQ` | ⌥, | Alt+8804 (0x4243) | ≤ |
| `SK_PLMN` | ⇧⌥= | Alt+0177 | ± Plus/Minus |
| `SK_DIV` | ⌥/ | Alt+0247 | ÷ Divide |
| `SK_FRAC` | ⇧⌥1 | Alt+0189 (0x4246) | ⁄ (⚠ BCD 0x4246 es ÷, no ½) |
| `SK_NOTEQ` | ⌥= | ⌥⌥= | ≠ Not Equal |
| `SK_APXEQ` | ⌥X | Alt+8776 (0x4247) | ≈ |
| `SK_OMEGA` | ⌥Z | Alt+937 (0x4234) | Ω |

### Currency

| Key | Mac | Windows | Acción |
|-----|-----|---------|--------|
| `SK_EURO` | ⇧⌥2 | Alt+0128 | € |
| `SK_CENT` | ⌥4 | Alt+0162 | ¢ |
| `SK_BPND` | ⌥3 | Alt+0163 | £ |
| `SK_JPY` | ⌥Y | Alt+0165 | ¥ |
| `SK_No` | ⇧⌥; | Alt+8470 | № |

### Quotations

| Key | Mac | Windows | Acción |
|-----|-----|---------|--------|
| `SK_SQUL` | ⌥] | Alt+0145 | ' Single quote left |
| `SK_SQUR` | ⇧⌥] | Alt+0146 | ' Single quote right |
| `SK_SDQL` | ⌥[ | Alt+0147 | " Double quote left |
| `SK_SDQR` | ⇧⌥[ | Alt+0148 | " Double quote right |
| `SK_FDQL` | ⌥\ | Alt+0171 | « French quote left |
| `SK_FDQR` | ⇧⌥\ | Alt+0187 | » French quote right |
| `SK_FSQL` | ⇧⌥3 | Alt+0139 | ‹ Single French left |
| `SK_FSQR` | ⇧⌥4 | Alt+0155 | › Single French right |
| `SK_IQUE` | ⇧⌥/ | Alt+0191 | ¿ Inv. Question Mark |
| `SK_IEXC` | ⌥1 | Alt+0161 | ¡ Inv. Exclamation Mark |

### Diacritics

| Key | Mac | Windows | Acción |
|-----|-----|---------|--------|
| `SK_ENYE` | ⌥N | AltGr+N | ñ (procesamiento especial, ver abajo) |

---

## Casos especiales en `process_semkey()`

| Key | Condición | Comportamiento |
|-----|-----------|----------------|
| **`SK_SWRD`** | — | Multi-keystroke: `SK_WORDPRV` → Shift down → `SK_WORDNXT` → Shift up (selecciona palabra actual) |
| **`SK_ENYE`** | AdaptiveKeys + English mode | `SK_ENYE` tap + Shift preservado + `KC_N` tap |
| **`SK_ENYE`** | Japanese mode | `KC_N` + `KC_N` (envía ん) |
| **`SK_HENK`** | — | Activa Japanese mode + `SK_HENK` tap |
| **`SK_MHEN`** | — | Activa English mode + `SK_MHEN` tap |
| **`SK_WORDPRV`** | GUI held (`#ifdef SK_DELLINE`) | Redirige a `SK_LINEBEG` |
| **`SK_WORDNXT`** | GUI held (`#ifdef SK_DELLINE`) | Redirige a `SK_LINEEND` |
| **`SK_DELWDL`** | Siempre (`#ifdef SK_DELLINE`) | Redirige a `SK_DELLNL` |
| **`SK_DELWDR`** | Siempre (`#ifdef SK_DELLINE`) | Redirige a `SK_DELLNR` |
| **Default** | — | `register_SemKey(keycode)` directo |

---

## Notas

- **`SK_LINEBEG`** (línea 92) y **`SK_LINEEND`** (línea 93) mapean a `{G(KC_DOWN), C(KC_END)}` — idéntico a `SK_DOCEND`. Parece un **bug**: probablemente deberían ser `Cmd+Left`/`Cmd+Right` en Mac y `Home`/`End` en Windows.
- Los códigos Windows con valores > 0x7FFF son códigos BCD de 3-4 dígitos para entrada Alt+numérico. El MSB (0x8000) indica que se debe presionar `KC_P0` al inicio de la secuencia.
