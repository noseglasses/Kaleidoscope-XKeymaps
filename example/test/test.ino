/* Kaleidoscope-XKeymaps
 * Copyright (C) 2019 noseglasses (shinynoseglasses@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

#include "Kaleidoscope.h"

#include "Kaleidoscope-XKeymaps.h"

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
      XKEYMAP_SPARSE_ENTRY(0, 7, ShiftToLayer(L1)),
      XKEYMAP_SPARSE_ENTRY(1, 7, ShiftToLayer(L2)),
      XKEYMAP_SPARSE_ENTRY(2, 7, ShiftToLayer(L3)),
      XKEYMAP_SPARSE_ENTRY(3, 7, ShiftToLayer(L4)),
      XKEYMAP_SPARSE_ENTRY(3, 8, ShiftToLayer(L5)),
      XKEYMAP_SPARSE_ENTRY(2, 8, ShiftToLayer(L6)),
      XKEYMAP_SPARSE_ENTRY(1, 8, ShiftToLayer(L7)),
      XKEYMAP_SPARSE_ENTRY(0, 8, ShiftToLayer(L8)),
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
     XKEYMAP_SPARSE_ENTRY(1, 0, Key_C),
     XKEYMAP_SPARSE_ENTRY(2, 1, Key_C)
  )
  XKEYMAP_MIRRORED(L2, L1 /* mirrors L1 */)
  XKEYMAP_SHIFTED(L3, L1 /* shifts L1 */, XXX, 1 /* x-offset */, 1 /* y-offset */)
  XKEYMAP_MIRRORED(L4, L3 /* mirrors the already shifted L3 */)
  XKEYMAP_SHIFTED_WRAPPED(L5, L1 /* shifts L1 */, 0 /* x-offset */, -1 /* y-offset */)
  XKEYMAP_ALL(L6, Key_B)
  XKEYMAP_MASKED(L7, L9, my_keymask, ___, /* don't negate */)
  XKEYMAP_MASKED(L8, L9, my_keymask, ___, ! /* negate */)
  XKEYMAP_ALL(L9, Key_M)
)

XKEYMAP_LAYER_STACK

KALEIDOSCOPE_INIT_PLUGINS(XKeymaps);

void setup() {  
   Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
