/*
 * x11-calc-45.h - RPN (Reverse Polish) calculator simulator.
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
 * 24 Dec 21         - Initial version with new button style - MT
 *
 */

#define MODEL           "45"
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
#define KBD_ROW         45 * SCALE_HEIGHT

#define KEY_HEIGHT      25 * SCALE_HEIGHT
#define KEY_WIDTH       33 * SCALE_WIDTH
#define KEY_NUMERIC     41 * SCALE_WIDTH
#define KEY_SMALL       25 * SCALE_WIDTH
#define KEY_GAP          3 * SCALE_WIDTH
#define SWITCH_HEIGHT   10 * SCALE_HEIGHT

#define ROM_SIZE        04000
#define MEMORY_SIZE     9
#define ROM_BANKS       1
#define CONTINIOUS
#define CLASSIC

int i_rom [ROM_SIZE * ROM_BANKS];

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]);
