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
#include "kaleidoscope/bitfields.h"

// Usage example: See the file example/test/test.ino

#define XKEYMAPS___ONE___(...) 1

// The FALLBACK_VALUE is returned if a keymap cannot be found.
//
#define XKEYMAPS_GENERIC(NAME,                                                 \
                         PER_KEY_TYPE,                                         \
                         TRANSPARENT_VALUE,                                    \
                         FALLBACK_VALUE,                                       \
                         ...)                                                  \
                                                                        __NL__ \
   /* Add an empty standard keymap to make Kaleidoscope happy */        __NL__ \
   KEYMAPS()                                                            __NL__ \
                                                                        __NL__ \
   namespace kaleidoscope {                                             __NL__ \
   namespace xkeymaps {                                                 __NL__ \
   namespace NAME {                                                     __NL__ \
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
      PER_KEY_TYPE valueFromKeymap_(uint8_t keymap,                     __NL__ \
                                  uint8_t row, uint8_t col) {           __NL__ \
         switch(keymap) {                                               __NL__ \
            __VA_ARGS__                                                 __NL__ \
         }                                                              __NL__ \
         return FALLBACK_VALUE;                                         __NL__ \
      }                                                                 __NL__ \
      typedef PER_KEY_TYPE(*ValueFromOverlayFunc)(uint8_t, uint8_t);    __NL__ \
      ValueFromOverlayFunc valueFromOverlayPtr = nullptr;               __NL__ \
                                                                        __NL__ \
      PER_KEY_TYPE valueFromKeymap(uint8_t keymap,                      __NL__ \
                                 uint8_t row, uint8_t col) {            __NL__ \
                                                                        __NL__ \
         /* The overlay keymap has precedence before all other layer    __NL__ \
          * stuff. If a key is non transparent in the overlay layer,    __NL__ \
          * it will always be used directly. This makes using layer     __NL__ \
          * toggle keys foolproof.                                      __NL__ \
          */                                                            __NL__ \
         if(valueFromOverlayPtr) {                                      __NL__ \
            PER_KEY_TYPE overlap_value                                  __NL__ \
               = (*valueFromOverlayPtr)(row, col);                      __NL__ \
            if(overlap_value != TRANSPARENT_VALUE) {                    __NL__ \
               return overlap_value;                                    __NL__ \
            }                                                           __NL__ \
         }                                                              __NL__ \
                                                                        __NL__ \
         return valueFromKeymap_(keymap, row, col);                     __NL__ \
      }                                                                 __NL__ \
   } /* end namespace NAME */                                           __NL__ \
   } /* end namespace xkeymaps */                                       __NL__ \
   } /* end namespace kaleidoscope */

// The FALLBACK_KEY is returned if a keymap cannot be found.
//
#define XKEYMAPS(FALLBACK_KEY, ...)                                            \
   XKEYMAPS_GENERIC(                                                           \
      standard,                                                                \
      Key,                                                                     \
      Key_Transparent,                                                         \
      FALLBACK_KEY,                                                            \
      __VA_ARGS__                                                              \
   )

#define XKEYMAP_OVERLAY_GENERIC(NAME,                                          \
                                PER_KEY_TYPE,                                  \
                                TRANSPARENT_VALUE,                             \
                                ...)                                           \
   namespace kaleidoscope {                                             __NL__ \
   namespace xkeymaps {                                                 __NL__ \
   namespace NAME {                                                     __NL__ \
                                                                        __NL__ \
      typedef PER_KEY_TYPE(*ValueFromOverlayFunc)(uint8_t, uint8_t);    __NL__ \
      extern ValueFromOverlayFunc valueFromOverlayPtr;                  __NL__ \
                                                                        __NL__ \
      PER_KEY_TYPE valueFromOverlay(uint8_t row, uint8_t col) {         __NL__ \
         /* This switch is just necessary as the KEYMAP definition      __NL__ \
          * macros feature case labels. By using the value 255 here and __NL__ \
          * relying on C++'s switch case fallthrough in the absence     __NL__ \
          * of a break statement, we can be sure that the overlay       __NL__ \
          * case is always executed.                                    __NL__ \
          */                                                            __NL__ \
         switch(255) {                                                  __NL__ \
            case 255:                                                   __NL__ \
            __VA_ARGS__                                                 __NL__ \
         }                                                              __NL__ \
         return TRANSPARENT_VALUE;                                      __NL__ \
      }                                                                 __NL__ \
                                                                        __NL__ \
      /* Register the overlay ptr */                                    __NL__ \
      bool initValueFromOverlayPtrHelper() {                            __NL__ \
         valueFromOverlayPtr = &valueFromOverlay;                       __NL__ \
         return true;                                                   __NL__ \
      }                                                                 __NL__ \
      bool initValueFromOverlayPtr = initValueFromOverlayPtrHelper();   __NL__ \
                                                                        __NL__ \
   } /* end namespace NAME */                                           __NL__ \
   } /* end namespace xkeymaps */                                       __NL__ \
   } /* end namespace kaleidoscope */
   
#define XKEYMAP_OVERLAY(...)                                                   \
   XKEYMAP_OVERLAY_GENERIC(                                                    \
      standard,                                                                \
      Key,                                                                     \
      Key_Transparent,                                                         \
      __VA_ARGS__                                                              \
   )
      
//******************************************************************************
// Macros to define keymaps
//******************************************************************************

// Generates a stacked keymap and reuses the KEYMAP_STACKED macro that 
// is provided by the keyboard hardware.
//
#define XKEYMAP_STACKED_GENERIC(PER_KEY_TYPE,                                  \
                                KEYMAP,                                        \
                                ...)                                           \
            case KEYMAP:                                                __NL__ \
            {                                                           __NL__ \
               static const PER_KEY_TYPE values[ROWS*COLS] PROGMEM =               __NL__ \
                  KEYMAP_STACKED(__VA_ARGS__);                          __NL__ \
                                                                        __NL__ \
               return PER_KEY_TYPE{pgm_read_word(&(values[row*COLS + col]))};      __NL__ \
            }
            
#define XKEYMAP_STACKED(KEYMAP, ...)                                           \
   XKEYMAP_STACKED_GENERIC(                                                    \
      Key,                                                                     \
      KEYMAP,                                                                  \
      __VA_ARGS__                                                              \
   )

// Defines a key at a given position in a sparse keymap.
//
#define XKEYMAP_SPARSE_ENTRY(ROW, COL, KEY)                                    \
   SparseKeymapEntry<decltype(KEY)>{uint8_t(ROW*COLS + COL), KEY}
   
// Defines a sparse keymap.
//
#define XKEYMAP_SPARSE_GENERIC(PER_KEY_TYPE,                                   \
                               KEYMAP,                                         \
                               DEFAULT_VALUE,                                  \
                               ...)                                            \
            case KEYMAP:                                                __NL__ \
            {                                                           __NL__ \
               const static SparseKeymapEntry<PER_KEY_TYPE>             __NL__ \
                     sparseKeymapEntries[] PROGMEM                      __NL__ \
               = {                                                      __NL__ \
                  __VA_ARGS__                                           __NL__ \
               };                                                       __NL__ \
                                                                        __NL__ \
               constexpr uint8_t entry_count                            __NL__ \
                  = sizeof(sparseKeymapEntries)                         __NL__ \
                      / sizeof(*sparseKeymapEntries);                   __NL__ \
                                                                        __NL__ \
               PER_KEY_TYPE value = DEFAULT_VALUE;                      __NL__ \
                                                                        __NL__ \
               valueFromSparseKeymap(row, col, value,                   __NL__ \
                                   sparseKeymapEntries, entry_count);   __NL__ \
                                                                        __NL__ \
               return value;                                            __NL__ \
            }
            
#define XKEYMAP_SPARSE(KEYMAP, DEFAULT_KEYCODE, ...)                           \
   XKEYMAP_SPARSE_GENERIC(                                                     \
      Key,                                                                     \
      KEYMAP,                                                                  \
      DEFAULT_KEYCODE,                                                         \
      __VA_ARGS__                                                              \
   )
     
// Mirrors a given keymap.
//
#define XKEYMAP_MIRRORED_GENERIC(KEYMAP,                                       \
                                 SOURCE_KEYMAP)                                \
            case KEYMAP:                                                __NL__ \
               return valueFromKeymap_(SOURCE_KEYMAP, row, COLS - 1 - col);  
               
#define XKEYMAP_MIRRORED(KEYMAP, SOURCE_KEYMAP)                                \
   XKEYMAP_MIRRORED_GENERIC(                                                   \
      KEYMAP,                                                                  \
      SOURCE_KEYMAP                                                            \
   )
  
// Shifts a given keymap.
//
#define XKEYMAP_SHIFTED_GENERIC(KEYMAP,                                        \
                                SOURCE_KEYMAP,                                 \
                                DEFAULT_KEYCODE,                               \
                                ROW_OFFSET,                                    \
                                COL_OFFSET)                                    \
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
               return valueFromKeymap_(SOURCE_KEYMAP,                   __NL__ \
                                 uint8_t(shifted_row),                  __NL__ \
                                 uint8_t(shifted_col));                 __NL__ \
            }
            
#define XKEYMAP_SHIFTED(KEYMAP,                                                \
                        SOURCE_KEYMAP,                                         \
                        DEFAULT_KEYCODE,                                       \
                        ROW_OFFSET,                                            \
                        COL_OFFSET)                                            \
   XKEYMAP_SHIFTED_GENERIC(                                                    \
      KEYMAP,                                                                  \
      SOURCE_KEYMAP,                                                           \
      DEFAULT_KEYCODE,                                                         \
      ROW_OFFSET,                                                              \
      COL_OFFSET                                                               \
   )
               
// Shifts a given keymap, thereby wrapping keys around.
//
#define XKEYMAP_SHIFTED_WRAPPED_GENERIC(KEYMAP,                                \
                                        SOURCE_KEYMAP,                         \
                                        ROW_OFFSET,                            \
                                        COL_OFFSET)                            \
            case KEYMAP:                                                __NL__ \
               return valueFromKeymap_(SOURCE_KEYMAP,                   __NL__ \
                                  (row + ROWS - ROW_OFFSET) % ROWS,     __NL__ \
                                  (col + COLS - COL_OFFSET) % COLS);
               
#define XKEYMAP_SHIFTED_WRAPPED(KEYMAP, SOURCE_KEYMAP, ROW_OFFSET, COL_OFFSET) \
   XKEYMAP_SHIFTED_WRAPPED_GENERIC(                                            \
      KEYMAP,                                                                  \
      SOURCE_KEYMAP,                                                           \
      ROW_OFFSET,                                                              \
      COL_OFFSET                                                               \
   )

// Checks a keymask and only uses the key from the source keymap if the
// respective bit in the keymask is set or returns the default key otherwise.
// Setting NEGATE to ! inverts the meaning of the keymask.
//
#define XKEYMAP_MASKED_GENERIC(KEYMAP,                                         \
                               SOURCE_KEYMAP,                                  \
                               KEYMASK,                                        \
                               DEFAULT_VALUE,                                  \
                               NEGATE)                                         \
            case KEYMAP:                                                __NL__ \
            {                                                           __NL__ \
               if(NEGATE KEYMASK.isBitSetPROGMEM(row*COLS + col)) {     __NL__ \
                  return valueFromKeymap_(SOURCE_KEYMAP, row, col);     __NL__ \
               }                                                        __NL__ \
               return DEFAULT_VALUE;                                    __NL__ \
            }
            
#define XKEYMAP_MASKED(KEYMAP, SOURCE_KEYMAP, KEYMASK, DEFAULT_KEYCODE, NEGATE)\
   XKEYMAP_MASKED_GENERIC(                                                     \
      KEYMAP,                                                                  \
      SOURCE_KEYMAP,                                                           \
      KEYMASK,                                                                 \
      DEFAULT_KEYCODE,                                                         \
      NEGATE                                                                   \
   )
            
// Returns the same keycode for all keys
//
#define XKEYMAP_ALL_GENERIC(KEYMAP,                                            \
                            KEY_VALUE)                                         \
            case KEYMAP:                                                __NL__ \
               return KEY_VALUE;
               
#define XKEYMAP_ALL(KEYMAP, KEYCODE)                                           \
   XKEYMAP_ALL_GENERIC(                                                        \
      KEYMAP,                                                                  \
      KEYCODE                                                                  \
   )
   
//******************************************************************************
// Keymap stack handling macros
//******************************************************************************
   
#define XKEYMAP_LAYER_STACK                                                    \
   namespace kaleidoscope {                                             __NL__ \
   namespace xkeymaps {                                                 __NL__ \
   class XKeymaps : public kaleidoscope::Plugin {                       __NL__ \
      public:                                                           __NL__ \
         EventHandlerResult onSetup() {                                 __NL__ \
            Layer.getKey = kaleidoscope::xkeymaps::standard::valueFromKeymap; __NL__ \
            layer_count = kaleidoscope::xkeymaps::standard::n_xkeymaps; __NL__ \
            return EventHandlerResult::OK;                              __NL__ \
         }                                                              __NL__ \
   };                                                                   __NL__ \
   } /* end namespace xkeymaps */                                       __NL__ \
   } /* end namespace kaleidoscope */                                   __NL__ \
                                                                        __NL__ \
   kaleidoscope::xkeymaps::XKeymaps XKeymaps;
       
namespace kaleidoscope {
namespace xkeymaps {

template<typename _PerKeyType>
struct SparseKeymapEntry {
   uint8_t offset_;
   _PerKeyType value_;
};

template<typename _PerKeyType>
void valueFromSparseKeymap(uint8_t row, uint8_t col,
                        _PerKeyType &value,
                        const SparseKeymapEntry<_PerKeyType> *sparse_keymap, 
                        uint8_t entry_count)
{
   uint8_t test_offset = row*COLS + col;
   
   for(uint8_t i = 0; i < entry_count; ++i) {
      uint8_t offset = pgm_read_byte(&(sparse_keymap[i].offset_));
      if(test_offset == offset) {
         memcpy_P(&value, &(sparse_keymap[i].value_), sizeof(_PerKeyType));
         return;
      }
   }
}

} // end namespace xkeymaps
} // end namespace kaleidoscope
