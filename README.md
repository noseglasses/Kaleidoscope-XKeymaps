# Extended Keymaps for the Kaleidoscope Keyboard Firmware

This plugin features the following types of keymaps as a drop in replacement
for Kaleidoscope's traditional keymap definition.

* sparse (specify only those keys that differ from a given default key)
* stacked (same as Kaleidoscope's standard keymap definition)
* mirrored (references another keymap)
* shifted (references another keymap)
* shifted and wrapped (references another keymap)
* bitmasked keymaps (references another keymap; uses its keys only if corresponding bits in a keymask are non-zero)
* same key used for all keys

## Overlay layer

XKeymaps allows to define an additional overlay layer. All 
non-transparent keys on this layer are preceded by all means. This e.g.
enables fool-proof definitions of `ShiftToLayer`-keys for whose positions
otherwise a transparent key is required on all layers.

## Usage

See the header file `Kaleidoscope-XKeymaps.h` for further documentation of the 
macros that are available for defining keymaps.

The following is a minimal example sketch.

```cpp
#include "Kaleidoscope.h"
#include "Kaleidoscope-XKeymaps.h"

// Define a keymask (a bitfield wherein each bit represents one key).
// The whole bitmask is stored in PROGMEM and occupies only eight bytes.
// It is used further below for masking of keymaps.
//
KEYMAP_BITFIELD_STACKED_PROGMEM(my_keymask, 
   0, 0, 0, 0, 0, 0, 0,
   0, 1, 1, 1, 1, 1, 0,
   0, 1, 1, 1, 1, 1,
   0, 0, 0, 0, 1, 1, 1,
            0, 0, 0, 0,
                     0,
                     
   0, 0, 0, 0, 0, 0, 0, 
   0, 1, 1, 1, 1, 1, 0, 
      1, 1, 1, 1, 1, 0, 
   1, 1, 1, 0, 0, 0, 0,
   0, 0, 0, 0,
   0
);

enum Layers { L0, L1, L2, L3, L4, L5, L6, L7, L8, L9 };

XKEYMAP_OVERLAY(
   // Layer shifts should always be defined as overlays to make sure that
   // they are active on all layers.
   XKEYMAP_SPARSE(254 /* this is ignored for overlay keymaps */, Key_Transparent /* the default key */,
      XKEYMAP_SPARSE_ENTRY(0, 7, ShiftToLayer(L1)), // row = 0, col = 7
      XKEYMAP_SPARSE_ENTRY(1, 7, ShiftToLayer(L2)),
      XKEYMAP_SPARSE_ENTRY(2, 7, ShiftToLayer(L3)),
      XKEYMAP_SPARSE_ENTRY(3, 7, ShiftToLayer(L4)),
      XKEYMAP_SPARSE_ENTRY(3, 8, ShiftToLayer(L5)),
      XKEYMAP_SPARSE_ENTRY(2, 8, ShiftToLayer(L6)),
      XKEYMAP_SPARSE_ENTRY(1, 8, ShiftToLayer(L7)),
      XKEYMAP_SPARSE_ENTRY(0, 0, Key_R),
   )  
)

XKEYMAPS(XXX /* The fallback key */,
  XKEYMAP_STACKED(L0, 
     Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
     Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
     Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
     Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
     Key_A, Key_A, Key_A, Key_A,
     Key_A,

     Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
     Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
            Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
     Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
     Key_A, Key_A, Key_A, Key_A,
     Key_A 
  )  
  XKEYMAP_SPARSE(L1, Key_Transparent /* the default key */,
     XKEYMAP_SPARSE_ENTRY(1, 0, Key_C), // row = 1, col = 0
     XKEYMAP_SPARSE_ENTRY(2, 1, Key_C)
  )
  XKEYMAP_MIRRORED(L2, L1 /* mirrors L1 */)
  XKEYMAP_SHIFTED(L3, L1 /* shifts L1 */, XXX, 1 /* row-offset */, 1 /* col-offset */)
  XKEYMAP_MIRRORED(L4, L3 /* mirrors the already shifted L3 */)
  XKEYMAP_SHIFTED_WRAPPED(L5, L1 /* shifts L1 */, 0 /* row-offset */, -1 /* col-offset */)
  XKEYMAP_ALL(L6, Key_B)
  XKEYMAP_MASKED(L7, L9, my_keymask, ___, /* don't negate */)
  XKEYMAP_MASKED(L8, L9, my_keymask, ___, ! /* negate */)
  XKEYMAP_ALL(L9, Key_M) // All keys set to keycode Key_M
)

// Generate a layer stack that mimics the behavior of Kaleidoscope's
// default layer stack. We require it to be defined explicitly to
// be able to provide alternative layer handling strategies in 
// future versions of this plugin.
//
XKEYMAP_LAYER_STACK

KALEIDOSCOPE_INIT_PLUGINS(XKeymaps);

void setup() {  
   Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

```
