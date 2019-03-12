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

#include <stdint.h>

// TODO: Move this to the kaleidoscope core (if accepted)

namespace kaleidoscope {
namespace bitfields {

// Compute the number of bytes that are necessary to store
// n_bits bits
//
constexpr uint8_t nBytesForBits(uint8_t n_bits) {
   return (n_bits % 8) ? n_bits/8 + 1 : n_bits/8;
}

// To store the bitfield we use a recursively defined template struct.
// It contains one byte of storage and a nested template struct
// that wraps the remaining bytes.
// This struct is then cast to a simple array of uint8_t. 
// Please note that this (non-standard cast) is only possible because 
// we instruct gcc not to add any padding bytes within Bitfield
// by using the __attribute__((packed)) directive.

template<uint8_t Byte_Count__>
struct Bitfield {
   
   uint8_t byte_;
   
   Bitfield<Byte_Count__ - 1> more_bytes_;
   
   // A constructor to initialize this class and to pass the remaining
   // bits on to the nested more_bytes_ struct
   //
   template<typename ... Bits__>
   constexpr Bitfield(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, 
                      uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, 
                      Bits__ ... bits) 
      :  byte_( b0 << 0
            |   b1 << 1
            |   b2 << 2
            |   b3 << 3
            |   b4 << 4
            |   b5 << 5
            |   b6 << 6
            |   b7 << 7),
         more_bytes_(bits...)
   {}

} __attribute__((packed));

// A specialization that stores any remaining 1 to 8 bits. Any bits that
// are unused are autommatically zeroed.
//
template<>
struct Bitfield<1> {   
   
   uint8_t byte_;
   
   constexpr Bitfield(uint8_t b0 = 0, uint8_t b1 = 0, uint8_t b2 = 0, uint8_t b3 = 0, 
                      uint8_t b4 = 0, uint8_t b5 = 0, uint8_t b6 = 0, uint8_t b7 = 0) 
      :  byte_( b0 << 0
            |   b1 << 1
            |   b2 << 2
            |   b3 << 3
            |   b4 << 4
            |   b5 << 5
            |   b6 << 6
            |   b7 << 7)
   {}
} __attribute__((packed));

// Defines a bitfield named VAR_NAME. Applies MODIFIER (could e.g. set to PROGMEM).
//
#define BITFIELD__(VAR_NAME, MODIFIER, ...)                                    \
    /* Generate an auxiliary array that stores the bit information.     __NL__ \
     * It is only used to determine the number of bits that are passed  __NL__ \
     * to macro BITFIELD.                                               __NL__ \
     */                                                                 __NL__ \
    static constexpr uint8_t bits_in_##VAR_NAME[] = {__VA_ARGS__};      __NL__ \
    static constexpr uint8_t n_bits_in_##VAR_NAME                       __NL__ \
       = sizeof(bits_in_##VAR_NAME) / sizeof(*bits_in_##VAR_NAME);      __NL__ \
                                                                        __NL__ \
    MODIFIER kaleidoscope::bitfields::Bitfield<                         __NL__ \
       kaleidoscope::bitfields::nBytesForBits(n_bits_in_##VAR_NAME)     __NL__ \
    > VAR_NAME(__VA_ARGS__);
    
#define BITFIELD(VAR_NAME, ...) BITFIELD__(VAR_NAME,, __VA_ARGS__)   
#define BITFIELD_PROGMEM(VAR_NAME, ...) BITFIELD__(VAR_NAME, const PROGMEM, __VA_ARGS__)           
    
#define KEYMASK_STACKED(VAR_NAME, ...) \
    BITFIELD(VAR_NAME, KEYMAP_STACKED_LIST(__VA_ARGS__))
    
#define KEYMASK_STACKED_PROGMEM(VAR_NAME, ...) \
    BITFIELD_PROGMEM(VAR_NAME, KEYMAP_STACKED_LIST(__VA_ARGS__))
    
bool isBitSetP(const void *bit_field, uint8_t raw_pos);
template<typename Bitfield__>
bool isBitSet(const Bitfield__ &bit_field, uint8_t raw_pos) {
   return isBitSetP(&bit_field, raw_pos);
}

void setBitP(void *bit_field, uint8_t raw_pos, uint8_t val);
template<typename Bitfield__>
void setBit(Bitfield__ &bit_field, uint8_t raw_pos, uint8_t val) {
   setBitP(&bit_field, raw_pos, val);
}

bool isBitSetPROGMEM_P(const void *bit_field, uint8_t raw_pos);
template<typename Bitfield__>
bool isBitSetPROGMEM(const Bitfield__ &bit_field, uint8_t raw_pos) {
   return isBitSetPROGMEM_P(&bit_field, raw_pos);
}


} // end namespace bitfields
} // end namespace kaleidoscope
