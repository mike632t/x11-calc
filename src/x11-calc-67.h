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
 * 13 Jun 13   0.1   - Initial version - MT
 * 30 Aug 20         - Moved functions to separate source file - MT
 * 12 Oct 21         - Removed Title and replaced with model number - MT
 *                   - Added macro definition for continuous memory - MT
 * 18 Oct 21         - Updated number of memory registers and ROM size - MT
 * 20 Oct 21         - Defined SPICE symbol, allows conditional compilation
 *                     of model dependent code - MT
 * 02 Nov 21         - Allows size of the window to be changed by modifying
 *                     the value of SCALE at compile time - MT
 * 16 Nov 21         - Can now define the horizontal and vertical scales to
 *                     independently of each other - MT
 * 22 Nov 21         - Only saves the state of continuous registers - MT
 * 06 Dec 21         - Simplified keyboard constants - MT
 *                   - Fixed number of digits and added new CLASSIC display
 *                     type definition - MT
 */

#define MODEL           "67"
#define HEIGHT          450 * SCALE_HEIGHT
#define WIDTH           200 * SCALE_WIDTH
#define BUTTONS         35

#define DIGITS          15

#define DISPLAY_LEFT    0 * SCALE_WIDTH
#define DISPLAY_TOP     4 * SCALE_HEIGHT
#define DISPLAY_WIDTH   200 * SCALE_WIDTH
#define DISPLAY_HEIGHT  61 * SCALE_HEIGHT

#define KBD_LEFT        12 * SCALE_WIDTH
#define KBD_TOP         67 * SCALE_HEIGHT
#define KBD_ROW         43 * SCALE_HEIGHT

#define KEY_HEIGHT      30 * SCALE_HEIGHT
#define KEY_SMALL       33 * SCALE_WIDTH
#define KEY_NUMERIC     41 * SCALE_WIDTH
#define KEY_GAP          3 * SCALE_WIDTH
#define SWITCH_HEIGHT   10 * SCALE_HEIGHT

#define ROM_SIZE        010000
#define MEMORY_SIZE     72
#define ROM_BANKS       2
#define CLASSIC         True
#define CONTINIOUS      False

int i_rom [ROM_SIZE * ROM_BANKS];

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]);
