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

enum { PRIMARY, NUMPAD, FUNCTION }; // layers

enum { MACRO_VERSION_INFO,
       MACRO_ANY
     };
     
KEYMASK_STACKED_PROGMEM(my_keymask, 
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

enum Layers { L0, L1, L2, L3, L4, L5, L6, L7, L8 };

/* We use enum codenames here for slots to prevent mixing them 
   up with layer ids (tree node ids) */

enum Slots { Base, Snap, Crackle, Pop, Fish, Cupid, Storm, Lightning };

XKEYMAP_OVERLAY(
   // Layer shifts should always be defined as overlays to make sure that
   // they are active on all layers.
   XKEYMAP_SPARSE(-1 /* this is ignored for overlay keymaps */, Key_Transparent /* the default key */,
      XKEYMAP_SPARSE_ENTRY(0, 7, ShiftToLayer(L1)),
      XKEYMAP_SPARSE_ENTRY(1, 7, ShiftToLayer(L2)),
      XKEYMAP_SPARSE_ENTRY(2, 7, ShiftToLayer(L3)),
      XKEYMAP_SPARSE_ENTRY(3, 7, ShiftToLayer(L4)),
      XKEYMAP_SPARSE_ENTRY(3, 8, ShiftToLayer(L5)),
      XKEYMAP_SPARSE_ENTRY(2, 8, ShiftToLayer(L6)),
      XKEYMAP_SPARSE_ENTRY(1, 8, ShiftToLayer(L7)),
      XKEYMAP_SPARSE_ENTRY(0, 8, ShiftToLayer(L8)),
      XKEYMAP_SPARSE_ENTRY(0, 0, Key_0),
   )  
)

XKEYMAPS(XXX /* The fallback key */,
  XKEYMAP_STACKED(Base, 
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
  XKEYMAP_SPARSE(Snap, Key_Transparent /* the default key */,
     /* Important: Sparse entry must be listed in ascending order of
                   row*COLS + col evaluated for their (row/col) pairs. */
     XKEYMAP_SPARSE_ENTRY(1, 0, Key_C),
     XKEYMAP_SPARSE_ENTRY(2, 1, Key_C)
  )
  XKEYMAP_MIRRORED(Crackle, Snap /* mirrors Snap */)
  XKEYMAP_SHIFTED(Pop, Snap /* shifts Snap */, XXX, 2 /* x-offset */, 3 /* y-offset */)
  XKEYMAP_MIRRORED(Fish, Pop /* mirrors the already shifted Pop */)
  XKEYMAP_SHIFTED_WRAPPED(Cupid, Snap /* shifts Snap */, -1 /* x-offset */, 0 /* y-offset */)
  XKEYMAP_ALL(Storm, Key_B)
  XKEYMAP_MASKED(Lightning, Snap, my_keymask, ___, /* don't negate */)
)

/* When using a keymap tree, layer ids match tree node ids which are both
   zero based. Every tree node (layer) references a slot. 
   While layer (node) ids are unique, the same slot can be referenced 
   by multiple layers (nodes) in the tree. */

/*
L0(Base)---L1(Snap)
         |
         *-L2(Crackle)
         |
         *-L3(Pop)
         |
         *-L4(Fish)
         |
         *-L5(Cupid)
         |
         *-L6(Storm)---L7(Lightning)
*/

XKEYMAP_TREE(
   XKEYMAP_TREE_ROOT(Base),                                                 /* node L0 */
   XKEYMAP_TREE_NODE(0 /* parent node */, Snap      /* referenced slot */), /* node L1 */
   XKEYMAP_TREE_NODE(0 /* parent node */, Crackle   /* referenced slot */), /* node L2 */
   XKEYMAP_TREE_NODE(0 /* parent node */, Pop       /* referenced slot */), /* node L3 */
   XKEYMAP_TREE_NODE(0 /* parent node */, Fish      /* referenced slot */), /* node L4 */
   XKEYMAP_TREE_NODE(0 /* parent node */, Cupid     /* referenced slot */), /* node L5 */
   XKEYMAP_TREE_NODE(0 /* parent node */, Storm     /* referenced slot */), /* node L6 */
   XKEYMAP_TREE_NODE(Storm /* parent node */, Lightning /* referenced slot */), /* node L7 */
)

KALEIDOSCOPE_INIT_PLUGINS(XKeymaps);

void setup() {
}

void loop() {
  Kaleidoscope.loop();
}
