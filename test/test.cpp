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

#include <stdint.h>
#include <iostream>

uint8_t pgm_read_byte(void *) { return 0; }

#define __NL__

#include "keymask.h"

#include <cassert>

int main(int argc, char **argv) {
   
   const BITFIELD(my_bitfield, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1);
   
   assert(sizeof(my_bitfield) == 2);
   
   assert( isBitSet(my_bitfield, 0));
   assert(!isBitSet(my_bitfield, 1));
   assert(!isBitSet(my_bitfield, 2));
   assert( isBitSet(my_bitfield, 3));
   assert( isBitSet(my_bitfield, 4));
   assert( isBitSet(my_bitfield, 5));
   assert(!isBitSet(my_bitfield, 6));
   assert( isBitSet(my_bitfield, 7));
   assert( isBitSet(my_bitfield, 8));
   assert(!isBitSet(my_bitfield, 9));
   assert( isBitSet(my_bitfield, 10));
   
   assert(!isBitSet(my_bitfield, 11));
   assert(!isBitSet(my_bitfield, 12));
   assert(!isBitSet(my_bitfield, 13));
   assert(!isBitSet(my_bitfield, 14));
   assert(!isBitSet(my_bitfield, 15));
   
   BITFIELD(my_bitfield_2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
   
   assert(sizeof(my_bitfield_2) == 2);
   
   assert(!isBitSet(my_bitfield_2, 0));
   assert(!isBitSet(my_bitfield_2, 1));
   assert(!isBitSet(my_bitfield_2, 2));
   assert(!isBitSet(my_bitfield_2, 3));
   assert(!isBitSet(my_bitfield_2, 4));
   assert(!isBitSet(my_bitfield_2, 5));
   assert(!isBitSet(my_bitfield_2, 6));
   assert(!isBitSet(my_bitfield_2, 7));
   assert(!isBitSet(my_bitfield_2, 8));
   assert(!isBitSet(my_bitfield_2, 9));
   assert(!isBitSet(my_bitfield_2, 10));
   assert(!isBitSet(my_bitfield_2, 11));
   assert(!isBitSet(my_bitfield_2, 12));
   assert(!isBitSet(my_bitfield_2, 13));
   assert(!isBitSet(my_bitfield_2, 14));
   assert(!isBitSet(my_bitfield_2, 15));
   
   setBit(my_bitfield_2, 1, 1);
   setBit(my_bitfield_2, 5, 1);
   setBit(my_bitfield_2, 9, 1);
      
   assert(!isBitSet(my_bitfield_2, 0));
   assert( isBitSet(my_bitfield_2, 1));
   assert(!isBitSet(my_bitfield_2, 2));
   assert(!isBitSet(my_bitfield_2, 3));
   assert(!isBitSet(my_bitfield_2, 4));
   assert( isBitSet(my_bitfield_2, 5));
   assert(!isBitSet(my_bitfield_2, 6));
   assert(!isBitSet(my_bitfield_2, 7));
   assert(!isBitSet(my_bitfield_2, 8));
   assert( isBitSet(my_bitfield_2, 9));
   assert(!isBitSet(my_bitfield_2, 10));
   assert(!isBitSet(my_bitfield_2, 11));
   assert(!isBitSet(my_bitfield_2, 12));
   assert(!isBitSet(my_bitfield_2, 13));
   assert(!isBitSet(my_bitfield_2, 14));
   assert(!isBitSet(my_bitfield_2, 15));
   
   setBit(my_bitfield_2, 1, 0);
   setBit(my_bitfield_2, 5, 0);
   setBit(my_bitfield_2, 9, 0);
   
   assert(!isBitSet(my_bitfield_2, 0));
   assert(!isBitSet(my_bitfield_2, 1));
   assert(!isBitSet(my_bitfield_2, 2));
   assert(!isBitSet(my_bitfield_2, 3));
   assert(!isBitSet(my_bitfield_2, 4));
   assert(!isBitSet(my_bitfield_2, 5));
   assert(!isBitSet(my_bitfield_2, 6));
   assert(!isBitSet(my_bitfield_2, 7));
   assert(!isBitSet(my_bitfield_2, 8));
   assert(!isBitSet(my_bitfield_2, 9));
   assert(!isBitSet(my_bitfield_2, 10));
   assert(!isBitSet(my_bitfield_2, 11));
   assert(!isBitSet(my_bitfield_2, 12));
   assert(!isBitSet(my_bitfield_2, 13));
   assert(!isBitSet(my_bitfield_2, 14));
   assert(!isBitSet(my_bitfield_2, 15));
   
   return 0;
}
