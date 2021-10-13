/*
 * x11-calc-29.h - RPN (Reverse Polish) calculator simulator.
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
 * 26 Sep 21         - Initial version - MEJT
 * 12 Oct 21         - Added macro definition for continuous memory - MT
 *
 */

#define MODEL        "27"
#define HEIGHT       385
#define WIDTH        201 /* Yes really! */
#define BUTTONS      30

#define DIGITS       12

#define MEMORY_SIZE  16
#define ROM_SIZE     04000
#define ROM_BANKS    1
#define CONTINIOUS   False

int i_rom [ROM_SIZE * ROM_BANKS];

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]);
