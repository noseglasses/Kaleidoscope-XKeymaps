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

// Usage example: See the file example/test/test.ino

#define XKEYMAPS___ONE___(...) 1

// The FALLBACK_KEY is returned if a keymap cannot be found.
//
#define XKEYMAPS(FALLBACK_KEY, ...)                                            \
                                                                        __NL__ \
   /* Add an empty standard keymap to make Kaleidoscope happy */        __NL__ \
   KEYMAPS()                                                            __NL__ \
                                                                        __NL__ \
   namespace kaleidoscope {                                             __NL__ \
   namespace xkeymaps {                                                 __NL__ \
                                                                        __NL__ \
      /* This gets the number of keymap definitions by wrapping them    __NL__ \
       * all with a function macro that evaluates to one. The result    __NL__ \
       * is a list of ones whose length is counted via casting it       __NL__ \
       * to an integer array whose lenght can be determined via         __NL__ \
       * the sizeof operator. All this is done at compile time and      __NL__ \
       * results in a compile time constant being initialized that      __NL__ \
       * can later on be used to initialize the global layer_count      __NL__ \
       * variable.                                                      __NL__ \
       */                                                               __NL__ \
      static constexpr uint8_t n_xkeymaps                               __NL__ \
         = sizeof((int[]) { MAP_LIST(XKEYMAPS___ONE___, __VA_ARGS__) }) __NL__ \
          /sizeof(int);                                                 __NL__ \
                                                                        __NL__ \
      Key keyFromKeymap_(uint8_t keymap, uint8_t row, uint8_t col) {    __NL__ \
         switch(keymap) {                                               __NL__ \
            __VA_ARGS__                                                 __NL__ \
         }                                                              __NL__ \
         return FALLBACK_KEY;                                           __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      Key keyFromKeymap(uint8_t keymap, uint8_t row, uint8_t col) {     __NL__ \
                                                                        __NL__ \
         /* The overlay keymap has precedence before all other layer    __NL__ \
          * stuff. If a key is non transparent in the overlay layer,    __NL__ \
          * it will always be used directly. This makes using layer     __NL__ \
          * toggle keys foolproof.                                      __NL__ \
          */                                                            __NL__ \
         Key o = keyFromOverlay(row, col);                              __NL__ \
         if(o != Key_Transparent) {                                     __NL__ \
            return o;                                                   __NL__ \
         }                                                              __NL__ \
                                                                        __NL__ \
         return keyFromKeymap_(keymap, row, col);                       __NL__ \
      }                                                                 __NL__ \
   } /* end namespace xkeymaps */                                       __NL__ \
   } /* end namespace kaleidoscope */

#define XKEYMAP_OVERLAY(...)                                                   \
   namespace kaleidoscope {                                             __NL__ \
   namespace xkeymaps {                                                 __NL__ \
                                                                        __NL__ \
      Key keyFromOverlay(uint8_t row, uint8_t col) {                    __NL__ \
         /* This switch is just necessary as the KEYMAP definition      __NL__ \
          * macros feature case labels. By usign 255 here and           __NL__ \
          * relying on C++'s switch case fallthrough in the absence     __NL__ \
          * of a break statement, we can be sure that the overlay       __NL__ \
          * case is always executed.                                    __NL__ \
          */                                                            __NL__ \
         switch(255) {                                                  __NL__ \
            case 255:                                                   __NL__ \
            __VA_ARGS__                                                 __NL__ \
         }                                                              __NL__ \
         return Key_Transparent;                                        __NL__ \
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
   SparseKeymapEntry{uint8_t(ROW*COLS + COL), KEY}
   
// Defines a sparse keymap.
//
#define XKEYMAP_SPARSE(KEYMAP, DEFAULT_KEYCODE, ...)                           \
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
               Key key = DEFAULT_KEYCODE;                               __NL__ \
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
               return keyFromKeymap_(SOURCE_KEYMAP, row, COLS - 1 - col);
  
// Shifts a given keymap.
//
#define XKEYMAP_SHIFTED(KEYMAP, SOURCE_KEYMAP, DEFAULT_KEYCODE, ROW_OFFSET, COL_OFFSET) \
            case KEYMAP:                                                __NL__ \
            {                                                           __NL__ \
               int8_t shifted_row = row - ROW_OFFSET;                   __NL__ \
               int8_t shifted_col = col - COL_OFFSET;                   __NL__ \
                                                                        __NL__ \
               if(   shifted_row < 0 || shifted_row >= ROWS             __NL__ \
                  || shifted_col < 0 || shifted_col >= COLS) {          __NL__ \
                  return DEFAULT_KEYCODE;                               __NL__ \
               }                                                        __NL__ \
                                                                        __NL__ \
               return keyFromKeymap_(SOURCE_KEYMAP,                     __NL__ \
                                 uint8_t(shifted_row),                  __NL__ \
                                 uint8_t(shifted_col));                 __NL__ \
            }
               
// Shifts a given keymap, thereby wrapping keys around.
//
#define XKEYMAP_SHIFTED_WRAPPED(KEYMAP, SOURCE_KEYMAP, ROW_OFFSET, COL_OFFSET) \
            case KEYMAP:                                                __NL__ \
               return keyFromKeymap_(SOURCE_KEYMAP,                     __NL__ \
                                  (row + ROWS - ROW_OFFSET) % ROWS,     __NL__ \
                                  (col + COLS - COL_OFFSET) % COLS);

// Checks a keymask and only uses the key from the source keymap if the
// respective bit in the keymask is set or returns the default key otherwise.
// Setting NEGATE to ! inverts the meaning of the keymask.
//
#define XKEYMAP_MASKED(KEYMAP, SOURCE_KEYMAP, KEYMASK, DEFAULT_KEYCODE, NEGATE)\
            case KEYMAP:                                                __NL__ \
            {                                                           __NL__ \
               if(NEGATE isBitSetPROGMEM(KEYMASK, row*COLS + col)) {    __NL__ \
                  return keyFromKeymap_(SOURCE_KEYMAP, row, col);       __NL__ \
               }                                                        __NL__ \
               return DEFAULT_KEYCODE;                                  __NL__ \
            }
            
// Returns the same keycode for all keys
//
#define XKEYMAP_ALL(KEYMAP, KEYCODE)                                           \
            case KEYMAP:                                                __NL__ \
               return KEYCODE;
            
//******************************************************************************
// Keymap tree handling macros
//******************************************************************************
         
// This section is commented as it is currently not possible to 
// implement a proper layer tree with Kaleidoscope's current layer class
// implementation.
//
#if 0
               
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
#endif
   
//******************************************************************************
// Keymap stack handling macros
//******************************************************************************
   
#define XKEYMAP_LAYER_STACK                                                          \
   namespace kaleidoscope {                                             __NL__ \
   namespace xkeymaps {                                                 __NL__ \
   class XKeymaps : public kaleidoscope::Plugin {                       __NL__ \
      public:                                                           __NL__ \
         EventHandlerResult onSetup() {                                 __NL__ \
            Layer.getKey = kaleidoscope::xkeymaps::keyFromKeymap;       __NL__ \
            layer_count = n_xkeymaps;                                   __NL__ \
            return EventHandlerResult::OK;                              __NL__ \
         }                                                              __NL__ \
   };                                                                   __NL__ \
   } /* end namespace xkeymaps */                                       __NL__ \
   } /* end namespace kaleidoscope */                                   __NL__ \
                                                                        __NL__ \
   kaleidoscope::xkeymaps::XKeymaps XKeymaps;
       
namespace kaleidoscope {
namespace xkeymaps {
   
extern Key keyFromOverlay(uint8_t row, uint8_t col);
   
struct SparseKeymapEntry {
   uint8_t offset_;
   Key key_;
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
