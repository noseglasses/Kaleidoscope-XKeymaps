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

// To store the bitfield we use a recursively defined template struct.
// It contains one byte of storage and a nested template struct
// that wraps the remaining bytes.
// This struct is then cast to a simple array of uint8_t. 
// Please note that this (non-standard cast) is only possible because 
// we instruct gcc not to add any padding bytes within Bitfield
// by using the __attribute__((packed)) directive.

template<uint8_t Byte_Count__>
struct Bitfield_ {
   
   uint8_t byte_;
   
   Bitfield_<Byte_Count__ - 1> more_bytes_;
   
   // A constructor to initialize this class and to pass the remaining
   // bits on to the nested more_bytes_ struct
   //
   template<typename ... Bits__>
   constexpr Bitfield_(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, 
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
struct Bitfield_<1> {   
   
   uint8_t byte_;
   
   constexpr Bitfield_(uint8_t b0 = 0, uint8_t b1 = 0, uint8_t b2 = 0, uint8_t b3 = 0, 
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

class BaseBitfield {
   
   protected:
      
      static bool isBitSetP(const void *bit_field, uint8_t raw_pos);
      static void setBitP(void *bit_field, uint8_t raw_pos, uint8_t val);
      static bool isBitSetPROGMEM_P(const void *bit_field, uint8_t raw_pos);
};

template<size_t BitCount__>
class Bitfield : public BaseBitfield {
   
   public:

      static constexpr size_t nBytesForBits(size_t n_bits) {
        return (n_bits % 8) ? n_bits/8 + 1 : n_bits/8;
      }
      
      static constexpr size_t n_bits_ = BitCount__;
      static constexpr size_t n_bytes_ = nBytesForBits(BitCount__);
         
      template<typename ... Bits__>
      constexpr Bitfield(Bits__...bits) : bits_(bits...) {
          static_assert(sizeof...(Bits__) == n_bits_, "Invalid number of bits supplied to constructor");
      }

      bool isBitSet(uint8_t raw_pos) const {
         return isBitSetP(&bits_, raw_pos);
      }

      bool isBitSetPROGMEM(uint8_t raw_pos) const {
         return isBitSetPROGMEM_P(&bits_, raw_pos);
      }

      void setBit(uint8_t raw_pos, uint8_t val) {
         setBitP(&bits_, raw_pos, val);
      }
      
      // An operator to query bits. Only applicable if 
      // bitfield is not stored in PROGMEM.
      //
      constexpr bool operator[](uint8_t raw_pos) const {
         return this->isBitSet(raw_pos);
      }
      
   private:
      
      Bitfield_<n_bytes_> bits_;
};

// This method could be used to create bitfields like.
//
// Note: Due to a restriction in all gcc versions < 6.0
//       this function cannot be called with the output of KEYMAP_STACKED(...)
//       or KEYMAP(...) as argument
//
// auto my_bitfield = generateBitfield(1, 2, 3);
//
template<typename...Bits__>
constexpr Bitfield<sizeof...(Bits__)> generateBitfield(Bits__...bits) {
   return Bitfield<sizeof...(Bits__)>(bits...);
}

template<typename...Args__>
constexpr size_t getNArgs(Args__...) { return sizeof...(Args__); }

// Defines a bitfield named VAR_NAME. Applies MODIFIER (could e.g. set to const PROGMEM).
//
#define BITFIELD__(VAR_NAME, MODIFIER, ...)                                    \
    constexpr size_t VAR_NAME##_n_bits                                  __NL__ \
         = kaleidoscope::bitfields::getNArgs(__VA_ARGS__);              __NL__ \
                                                                        __NL__ \
    MODIFIER kaleidoscope::bitfields::Bitfield<VAR_NAME##_n_bits>       __NL__ \
         VAR_NAME{__VA_ARGS__};
    
#define BITFIELD(VAR_NAME, ...) BITFIELD__(VAR_NAME,, __VA_ARGS__)   
#define BITFIELD_PROGMEM(VAR_NAME, ...) BITFIELD__(VAR_NAME, const PROGMEM, __VA_ARGS__)           
    
#ifdef KEYMAP_LIST
#define KEYMASK(VAR_NAME, ...) \
    BITFIELD(VAR_NAME, KEYMAP_LIST(0 /*default for non-existent keys*/, __VA_ARGS__))
    
#define KEYMASK_PROGMEM(VAR_NAME, ...) \
    BITFIELD_PROGMEM(VAR_NAME, KEYMAP_LIST(0 /*default for non-existent keys*/, __VA_ARGS__))
#endif
    
#ifdef KEYMAP_STACKED_LIST
#define KEYMASK_STACKED(VAR_NAME, ...) \
    BITFIELD(VAR_NAME, KEYMAP_STACKED_LIST(0 /*default for non-existent keys*/, __VA_ARGS__))
    
#define KEYMASK_STACKED_PROGMEM(VAR_NAME, ...) \
    BITFIELD_PROGMEM(VAR_NAME, KEYMAP_STACKED_LIST(0 /*default for non-existent keys*/, __VA_ARGS__))
#endif

} // end namespace bitfields
} // end namespace kaleidoscope
