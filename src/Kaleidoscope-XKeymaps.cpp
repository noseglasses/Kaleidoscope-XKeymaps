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

#include "Kaleidoscope-XKeymaps.h"

namespace kaleidoscope {
namespace xkeymaps {

void keyFromSparceKeymap(uint8_t row, uint8_t col,
                        Key &key,
                        const SparseKeymapEntry *sparce_keymap, 
                        uint8_t entry_count)
{
   uint8_t test_raw_pos = row*COLS + col;
   
   // This method does a binary search to find an entry for 
   // test_raw_pos within sparse_keymap with entry_count entries.
   
   uint8_t first = 0;
	uint8_t last = entry_count - 1;
	uint8_t middle = (first + last)/2;
   
	while (first <= last)
	{
      uint8_t raw_pos = pgm_read_word(&(sparce_keymap[middle].raw_pos_));
      
		if(raw_pos < test_raw_pos)
		{
			first = middle + 1;
		}
		else if(raw_pos == test_raw_pos)
		{
         key = Key{pgm_read_word(&(sparce_keymap[middle].raw_key_))};
         return;
		}
		else
		{
			 last = middle - 1;
		}
		
		middle = (first + last)/2;
	}
}

} // end namespace kaleidoscope
} // end namespace xkeymaps
