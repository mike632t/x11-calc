/*
 * x11-calc-33.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2018   MT
 *
 * Model specific constants and function prototypes.
 *
 * This  program is free software: you can redistribute it and/or modify it
 * under  the terms of the GNU General Public License as published  by  the
 * Free  Software Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 23 Nov 21   0.1   - Initial version - MT
 * 22 Dec 21         - Removed SPICE symbol - MT
 * 11 Jan 22         - Removed ROM_BANKS - MT
 *
 */

#define MODEL           "34"
#define HEIGHT          385 * SCALE_HEIGHT
#define WIDTH           200 * SCALE_WIDTH
#define BUTTONS         30

#define DIGITS          11

#define DISPLAY_LEFT    0
#define DISPLAY_TOP     4 * SCALE_HEIGHT
#define DISPLAY_WIDTH   200 * SCALE_WIDTH
#define DISPLAY_HEIGHT  61 * SCALE_HEIGHT

#define KEYBOARD_ROW_0  67 * SCALE_HEIGHT
#define KEYBOARD_ROW_1  89 * SCALE_HEIGHT
#define KEYBOARD_ROW_2  132 * SCALE_HEIGHT
#define KEYBOARD_ROW_3  175 * SCALE_HEIGHT
#define KEYBOARD_ROW_4  218 * SCALE_HEIGHT
#define KEYBOARD_ROW_5  261 * SCALE_HEIGHT
#define KEYBOARD_ROW_6  304 * SCALE_HEIGHT
#define KEYBOARD_ROW_7  347 * SCALE_HEIGHT

#define KEYBOARD_COL_A  12 * SCALE_WIDTH
#define KEYBOARD_COL_B  48 * SCALE_WIDTH
#define KEYBOARD_COL_C  84 * SCALE_WIDTH
#define KEYBOARD_COL_D  120 * SCALE_WIDTH
#define KEYBOARD_COL_E  156 * SCALE_WIDTH

#define KEYBOARD_COL_1  12 * SCALE_WIDTH
#define KEYBOARD_COL_2  52 * SCALE_WIDTH
#define KEYBOARD_COL_3  100 * SCALE_WIDTH
#define KEYBOARD_COL_4  147 * SCALE_WIDTH

#define SMALL_KEY_WIDTH 33 * SCALE_WIDTH
#define NUM_KEY_WIDTH   41 * SCALE_WIDTH
#define ENTER_KEY_WIDTH 69 * SCALE_WIDTH
#define KEY_HEIGHT      30 * SCALE_HEIGHT
#define SWITCH_HEIGHT   10 * SCALE_HEIGHT

#define ROM_SIZE        020000
#define MEMORY_SIZE     78       /** Should be 64 ? */
#define CONTINIOUS

int i_rom [ROM_SIZE];

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]);
