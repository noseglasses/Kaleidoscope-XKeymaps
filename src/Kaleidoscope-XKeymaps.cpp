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

#include "Kaleidoscope.h"
#include "Kaleidoscope-XKeymaps.h"

namespace kaleidoscope {
namespace xkeymaps {
   
// __attribute__((weak))
// extern Key keyFromOverlay(uint8_t row, uint8_t col)
// {
//    return Key_Transparent;
// }

// A binary search that could be efficient when a greater number of 
// sparse entries are defined. It comes with the disadvantage 
// that the sparce keys need to be defined in ascending order
// of row*COLS + col.
//
// void keyFromSparseKeymap(uint8_t row, uint8_t col,
//                         Key &key,
//                         const SparseKeymapEntry *sparse_keymap, 
//                         uint8_t entry_count)
// {
//    uint8_t test_raw_pos = row*COLS + col;
//    
//    // This method does a binary search to find an entry for 
//    // test_raw_pos within sparse_keymap with entry_count entries.
//    
//    uint8_t first = 0;
// 	uint8_t last = entry_count - 1;
// 	uint8_t middle = (first + last)/2;
//    
// 	while (first <= last)
// 	{
//       uint8_t raw_pos = pgm_read_byte(&(sparse_keymap[middle].raw_pos_));
//       
// 		if(raw_pos < test_raw_pos)
// 		{
// 			first = middle + 1;
// 		}
// 		else if(raw_pos == test_raw_pos)
// 		{
//          key = Key{pgm_read_word(&(sparse_keymap[middle].raw_key_))};
//          return;
// 		}
// 		else
// 		{
// 			 last = middle - 1;
// 		}
// 		
// 		middle = (first + last)/2;
// 	}
// }

// This is doing a linear search which is probably faster than
// the binary search if not too many keys are defined. It also 
// has the great advantage that keys do not need to be defined in
// ascending order of row*COLS + col.
//
// void keyFromSparseKeymap(uint8_t row, uint8_t col,
//                         Key &key,
//                         const SparseKeymapEntry *sparse_keymap, 
//                         uint8_t entry_count)
// {
//    uint8_t test_offset = row*COLS + col;
//    
//    for(uint8_t i = 0; i < entry_count; ++i) {
//       uint8_t offset = pgm_read_byte(&(sparse_keymap[i].offset_));
//       if(test_offset == offset) {
//          key = Key{pgm_read_word(&(sparse_keymap[i].key_))};
//          return;
//       }
//    }
// }


} // end namespace kaleidoscope
} // end namespace xkeymaps
