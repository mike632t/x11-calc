/*
 * x11-calc-22.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2018    MT
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
 * 21 Sep 21   0.1   - Initial version - MT
 * 12 Oct 21         - Removed Title and replaced with model number - MT
 *                   - Added macro definition for continuous memory - MT
 * 04 Nov 21         - Allows size of the window to be changed by modifying
 *                     the value of SCALE at compile time - MT
 */

#define MODEL           "22"
#define SCALE           1.33
#define HEIGHT          385 * SCALE
#define WIDTH           200 * SCALE
#define BUTTONS         30

#define DIGITS          12

#define DISPLAY_LEFT    0
#define DISPLAY_TOP     4 * SCALE
#define DISPLAY_WIDTH   200 * SCALE
#define DISPLAY_HEIGHT  61 * SCALE

#define KEYBOARD_ROW_0  67 * SCALE
#define KEYBOARD_ROW_1  89 * SCALE
#define KEYBOARD_ROW_2  132 * SCALE
#define KEYBOARD_ROW_3  175 * SCALE
#define KEYBOARD_ROW_4  218 * SCALE
#define KEYBOARD_ROW_5  261 * SCALE
#define KEYBOARD_ROW_6  304 * SCALE
#define KEYBOARD_ROW_7  347 * SCALE

#define KEYBOARD_COL_A  12 * SCALE
#define KEYBOARD_COL_B  48 * SCALE
#define KEYBOARD_COL_C  84 * SCALE
#define KEYBOARD_COL_D  120 * SCALE
#define KEYBOARD_COL_E  156 * SCALE

#define KEYBOARD_COL_1  12 * SCALE
#define KEYBOARD_COL_2  52 * SCALE
#define KEYBOARD_COL_3  100 * SCALE
#define KEYBOARD_COL_4  147 * SCALE

#define SMALL_KEY_WIDTH 33 * SCALE
#define NUM_KEY_WIDTH   41 * SCALE
#define ENTER_KEY_WIDTH 69 * SCALE
#define KEY_HEIGHT      30 * SCALE
#define SWITCH_HEIGHT   10 * SCALE

/* Reserve enough storage in memory to hold ten registers (0 - 9), and  the
 * six financial registers.  */
#define MEMORY_SIZE     (10 + 6) /* 0 - 15 */
#define ROM_SIZE        04000
#define ROM_BANKS       1
#define CONTINIOUS      False

int i_rom [ROM_SIZE * ROM_BANKS];

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]);
