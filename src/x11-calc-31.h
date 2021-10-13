/*
 * x11-calc-31.h - RPN (Reverse Polish) calculator simulator.
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
 * 21 Sep 21   0.1   - Initial version - MT
 * 12 Oct 21         - Removed Title and replaced with model number - MT
 *                   - Added macro definition for continuous memory - MT
 *
 */

#define MODEL        "31"
#define HEIGHT       385
#define WIDTH        201   /* Yes really! */
#define BUTTONS      30

#define DIGITS       11

#define ROM_SIZE     04000
#define MEMORY_SIZE  4
#define ROM_BANKS    1
#define CONTINIOUS   False

int i_rom [ROM_SIZE * ROM_BANKS];

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]);
