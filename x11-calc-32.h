/*
 *
 * x11-calc-33.h
 *
 * Copyright(C) 2018   MT
 * 
 * Model specific functions
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
 *                      
 * TO DO :           - 
 */

#ifndef TITLE
#define TITLE        "RPN calc 32"
#define HEIGHT       385
#define WIDTH        201   /* Yes really! */
#define BUTTONS      30

#define DIGITS       12

#define ROM_SIZE     02000  
#define RAM_SIZE     00000
#define ROM_BANKS    1

#define True         1
#define False        0

int i_rom [ROM_SIZE * ROM_BANKS];

void v_init_calc();

int v_init_keypad(o_button* h_button[]);
#endif
