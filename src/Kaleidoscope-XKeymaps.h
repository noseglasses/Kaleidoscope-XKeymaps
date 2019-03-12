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

#pragma once

#include "Kaleidoscope.h"
#include "kaleidoscope/macro_helpers.h"
#include "keymask.h"

// Usage example:
//
// KEYMASK_STACKED_PROGMEM(my_keymask, 
//    0, 0, 0, 0, 0, 0, 0,
//    0, 1, 1, 1, 1, 1, 0,
//    0, 1, 1, 1, 1, 1,
//    0, 0, 0, 0, 1, 1, 1,
//             0, 1, 1, 0,
//                      0,
//                      
//    0, 0, 0, 0, 0, 0, 0, 
//    0, 1, 1, 1, 1, 1, 0, 
//       1, 1, 1, 1, 1, 0, 
//    1, 1, 1, 0, 0, 0, 0,
//    0, 1, 1, 0,
//    0
// );
//
// /* We use enum codenames here for keymaps to prevent mixing them 
//    up with layer ids (tree node ids) */
// 
// enum Keymaps { Base, Snap, Crackle, Pop, Fish, Cupid, Storm};
// 
// XKEYMAPS(XXX /* The fallback key */,
//   XKEYMAP_STACKED(Base, 
//    ___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
//    Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
//    Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
//    Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
//    Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
//    ShiftToLayer(FUNCTION),
// 
//    M(MACRO_ANY),  Key_6, Key_7, Key_8,     Key_9,         Key_0,         LockLayer(NUMPAD),
//    Key_Enter,     Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
//                   Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
//    Key_RightAlt,  Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,
//    Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
//    ShiftToLayer(FUNCTION) 
//   )
//   XKEYMAP_SPARSE(Snap, Key_Transparent /* the default key */,
//      /* Important: Sparse entry must be listed in ascending order of
//                    row*COLS + col evaluated for their (row/col) pairs. */
//      XKEYMAP_SPARSE_ENTRY(2, 3, Key_A),
//      XKEYMAP_SPARSE_ENTRY(7, 8, Key_B)
//   )
//   XKEYMAP_MIRRORED(Crackle, Snap /* mirrors Snap */)
//   XKEYMAP_SHIFTED(Pop, Snap /* shifts Snap */, XXX, 2 /* x-offset */, 3 /* y-offset */)
//   XKEYMAP_MIRRORED(Fish, Pop /* mirrors the already shifted Pop */)
//   XKEYMAP_SHIFTED_WRAPPED(Cupid, Snap /* shifts Snap */, 2 /* x-offset */, 3 /* y-offset */)
//   XKEYMAP_MASKED(Storm, Pop, my_keymask, ___, /* don't negate */)
// )
// 
// /* When using a keymap tree, layer ids match tree node ids which are both
//    zero based. Every tree node (layer) references a keymap. 
//    While layer (node) ids are unique, the same keymap can be referenced 
//    by multiple layers (nodes) in the tree. */
// 
// /*
// 0(Base)---1(Snap)---5(Fish)
//         |
//         *-2(Crackle)---3(Pop)
//                      |
//                      *-4(Fish)
// */
// 
// XKEYMAP_TREE(
//    XKEYMAP_TREE_ROOT(Base),                                               /* node 0 */
//    XKEYMAP_TREE_NODE(0 /* parent node */, Base    /* referenced keymap */), /* node 1 */
//    XKEYMAP_TREE_NODE(0 /* parent node */, Crackle /* referenced keymap */), /* node 2 */
//    XKEYMAP_TREE_NODE(2 /* parent node */, Pop     /* referenced keymap */), /* node 3 */
//    XKEYMAP_TREE_NODE(2 /* parent node */, Fish    /* referenced keymap */), /* node 4 */
//    XKEYMAP_TREE_NODE(1 /* parent node */, Fish    /* referenced keymap */), /* node 5 */
// )
//
// /* Or alternatively ... */
// 
// XKEYMAP_STACK /* Interprets layer ids as keymap ids, which gives 
//                  an ordinary layer stack. */
//
// Important: Use either XKEYMAP_TREE or XKEYMAP_STACK

// The FALLBACK_KEY is returned if a keymap cannot be found.
//
#define XKEYMAPS(FALLBACK_KEY, ...)                                            \
   namespace kaleidoscope {                                             __NL__ \
   namespace xkeymaps {                                                 __NL__ \
      Key keyFromKeymap(uint8_t keymap, uint8_t row, uint8_t col) {     __NL__ \
         switch(keymap) {                                               __NL__ \
            __VA_ARGS__                                                 __NL__ \
         }                                                              __NL__ \
         return FALLBACK_KEY;                                           __NL__ \
      }                                                                 __NL__ \
   } /* end namespace xkeymaps */                                       __NL__ \
   } /* end namespace kaleidoscope */
   
//******************************************************************************
// Macros to define keymaps
//******************************************************************************

// Generates a stacked keymap and reuses the KEYMAP_STACKED macro that 
// is provided by the keyboard hardware.
//
#define XKEYMAP_STACKED(KEYMAP, ...)                                           \
            case KEYMAP:                                                __NL__ \
            {                                                           __NL__ \
               static const Key keys[ROWS][COLS] PROGMEM =              __NL__ \
                  KEYMAP_STACKED(__VA_ARGS__);                          __NL__ \
                                                                        __NL__ \
               return Key{pgm_read_word(&(keys[row][col]))};            __NL__ \
            }

// Defines a key at a given position in a sparse keymap.
//
#define XKEYMAP_SPARSE_ENTRY(ROW, COL, KEY)                                    \
   SparseKeymapEntry{uint8_t(ROW*COLS + COL), KEY.raw}
   
// Defines a sparse keymap.
//
#define XKEYMAP_SPARSE(KEYMAP, DEFAULT_KEY, ...)                               \
            case KEYMAP:                                                __NL__ \
            {                                                           __NL__ \
               const static SparseKeymapEntry                           __NL__ \
                     sparseKeymapEntries[] PROGMEM                      __NL__ \
               = {                                                      __NL__ \
                  __VA_ARGS__                                           __NL__ \
               };                                                       __NL__ \
                                                                        __NL__ \
               constexpr uint8_t entry_count                            __NL__ \
                  = sizeof(sparseKeymapEntries)                         __NL__ \
                      / sizeof(*sparseKeymapEntries);                   __NL__ \
                                                                        __NL__ \
               Key key = DEFAULT_KEY;                                   __NL__ \
                                                                        __NL__ \
               keyFromSparseKeymap(row, col, key,                       __NL__ \
                                   sparseKeymapEntries, entry_count);   __NL__ \
                                                                        __NL__ \
               return key;                                              __NL__ \
            }
            
// Mirrors a given keymap.
//
#define XKEYMAP_MIRRORED(KEYMAP, SOURCE_KEYMAP)                                \
            case KEYMAP:                                                __NL__ \
               return keyFromKeymap(SOURCE_KEYMAP, row, COLS - 1 - col);
  
// Shifts a given keymap.
//
#define XKEYMAP_SHIFTED(KEYMAP, SOURCE_KEYMAP, DEFAULT_KEY, ROW_OFFSET, COL_OFFSET) \
            case KEYMAP:                                                __NL__ \
            {                                                           __NL__ \
               int8_t shifted_row = row + ROW_OFFSET;                   __NL__ \
               int8_t shifted_col = col + COL_OFFSET;                   __NL__ \
                                                                        __NL__ \
               if(   shifted_row < 0 || shifted_row >= ROWS             __NL__ \
                  || shifted_col < 0 || shifted_col >= COLS) {          __NL__ \
                  return DEFAULT_KEY;                                   __NL__ \
               }                                                        __NL__ \
                                                                        __NL__ \
               return keyFromKeymap(SOURCE_KEYMAP,                      __NL__ \
                                 uint8_t(shifted_row),                  __NL__ \
                                 uint8_t(shifted_col));                 __NL__ \
            }
               
// Shifts a given keymap, thereby wrapping keys around.
//
#define XKEYMAP_SHIFTED_WRAPPED(KEYMAP, SOURCE_KEYMAP, ROW_OFFSET, COL_OFFSET) \
            case KEYMAP:                                                __NL__ \
               return keyFromKeymap(SOURCE_KEYMAP,                      __NL__ \
                                  (row + ROWS + ROW_OFFSET) % ROWS,     __NL__ \
                                  (col + COLS + COL_OFFSET) % COLS);

// Checks a keymask and only uses the key from the source keymap if the
// respective bit in the keymask is set or returns the default key otherwise.
// Setting NEGATE to ! inverts the meaning of the keymask.
//
#define XKEYMAP_MASKED(KEYMAP, SOURCE_KEYMAP, KEYMASK, DEFAULT_KEY, NEGATE)    \
            case KEYMAP:                                                __NL__ \
            {                                                           __NL__ \
               if(NEGATE isBitSetPROGMEM(KEYMASK, row*COLS + col)) {    __NL__ \
                  return keyFromKeymap(SOURCE_KEYMAP, row, col);        __NL__ \
               }                                                        __NL__ \
               return DEFAULT_KEY;                                      __NL__ \
            }
            
//******************************************************************************
// Keymap tree handling macros
//******************************************************************************
             
#define XKEYMAP_TREE_NODE(PARENT_LAYER, KEYMAP)                                \
   kaleidoscope::xkeymaps::TreeNode{PARENT_LAYER, KEYMAP}
   
#define XKEYMAP_TREE_ROOT(KEYMAP) XKEYMAP_TREE_NODE(0, KEYMAP)
               
#define XKEYMAP_TREE(...)                                                      \
   namespace kaleidoscope {                                             __NL__ \
   namespace xkeymaps {                                                 __NL__ \
   /* The array position matches the layer id */                        __NL__ \
   const TreeNode layer_tree[] PROGMEM = {                              __NL__ \
      __VA_ARGS__                                                       __NL__ \
   };                                                                   __NL__ \
                                                                        __NL__ \
   Key keyFromTreeNodeLayer(uint8_t layer, uint8_t row, uint8_t col) {  __NL__ \
      uint8_t keymap = pgm_read_byte(&(layer_tree[layer].keymap_));     __NL__ \
      return keyFromKeymap(keymap, row, col);                           __NL__ \
   }                                                                    __NL__ \
                                                                        __NL__ \
   uint8_t nextLowerLayer(uint8_t layer) {                              __NL__ \
      return pgm_read_byte(&(layer_tree[layer].parent_layer_));         __NL__ \
   }                                                                    __NL__ \
                                                                        __NL__ \
   class XKeymaps : public kaleidoscope::Plugin {                       __NL__ \
      public:                                                           __NL__ \
         EventHandlerResult onSetup() {                                 __NL__ \
            Layer.getKey = kaleidoscope::xkeymaps::keyFromTreeNodeLayer;__NL__ \
            return EventHandlerResult::OK;                              __NL__ \
         }                                                              __NL__ \
   };                                                                   __NL__ \
   } /* end namespace xkeymaps */                                       __NL__ \
   } /* end namespace kaleidoscope */                                   __NL__ \
                                                                        __NL__ \
   kaleidoscope::xkeymaps::XKeymaps XKeymaps;
   
//******************************************************************************
// Keymap stack handling macros
//******************************************************************************
   
#define XKEYMAP_STACK                                                          \
   namespace kaleidoscope {                                             __NL__ \
   namespace xkeymaps {                                                 __NL__ \
   class XKeymaps : public kaleidoscope::Plugin {                       __NL__ \
      public:                                                           __NL__ \
         EventHandlerResult onSetup() {                                 __NL__ \
            Layer.getKey = kaleidoscope::xkeymaps::keyFromKeymap;       __NL__ \
            return EventHandlerResult::OK;                              __NL__ \
         }                                                              __NL__ \
   };                                                                   __NL__ \
   } /* end namespace xkeymaps */                                       __NL__ \
   } /* end namespace kaleidoscope */                                   __NL__ \
                                                                        __NL__ \
   kaleidscope::xkeymaps::XKeymaps XKeymaps;
       
namespace kaleidoscope {
namespace xkeymaps {
   
struct SparseKeymapEntry {
   uint8_t raw_pos_;
   uint16_t raw_key_;
};

void keyFromSparseKeymap(uint8_t row, uint8_t col,
                        Key &key,
                        const SparseKeymapEntry *sparse_keymap, 
                        uint8_t entry_count);

struct TreeNode {
   uint8_t parent_layer_;
   uint8_t keymap_;
};  

} // end namespace xkeymaps
} // end namespace kaleidoscope
