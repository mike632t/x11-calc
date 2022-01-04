/*
 * x11-calc-25.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2018   MEJT
 *
 * Model specific constants and function prototypes.
 *
 * This  program is free software: you can redistribute it and/or modify  it
 * under  the  terms of the GNU General Public License as published  by  the
 * Free  Software  Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is  distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without  even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License  along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 09 Mar 14         - Initial verson - MEJT
 * 06 Oct 21         - Corrected number of registers - MT
 * 12 Oct 21         - Removed Title and replaced with model number - MT
 *                   - Added macro definition for continuous memory - MT
 * 04 Nov 21         - Allows size of the window to be changed by modifying
 *                     the value of SCALE at compile time - MT
 * 16 Nov 21         - Can now define the horizontal and vertical scales to
 *                     independently of each other - MT
 * 22 Nov 21         - Only saves the state of continuous registers - MT
 * 30 Dec 21         - Does NOT have continuous memory - MT
 *
 */

#define MODEL           "25"
#define HEIGHT          385 * SCALE_HEIGHT
#define WIDTH           200 * SCALE_WIDTH
#define BUTTONS         30

#define DIGITS          12

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

/* Reserve enough storage in memory to hold eight registers (0 - 7), Lastx,
 * and forty nine program steps */
#define MEMORY_SIZE     (8 + 1 + (49 / 7))
#define ROM_SIZE        04000
#define ROM_BANKS       1
#define CONTINIOUS

int i_rom [ROM_SIZE * ROM_BANKS];

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]);
