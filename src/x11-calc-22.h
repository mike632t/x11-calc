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
 *
 */

#define MODEL        "22"
#define HEIGHT       385
#define WIDTH        201 /* Yes really! */
#define BUTTONS      30

#define DIGITS       12

/* Reserve enough storage in memory to hold ten registers (0 - 9), and  the
 * six financial registers.  */
#define MEMORY_SIZE  (10 + 6) /* 0 - 15 */
#define ROM_SIZE     04000
#define ROM_BANKS    1
#define CONTINIOUS   False

int i_rom [ROM_SIZE * ROM_BANKS];

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]);
