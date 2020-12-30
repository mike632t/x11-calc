/*
 * x11-calc.h - RPN (Reverse Polish) calculator simulator. 
 *
 * Copyright(C) 2018   MT
 * 
 * Common function definitions.
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
 * 31 Aug 20   0.1   - Initial version - MT
 * 31 Aug 20         - Resolved dependancies between header files by moving
 *                     common function definitions to a sperate file - MT
 *                      
 * TO DO :           - 
 */
  
#include "x11-calc-21.h"

#ifndef TITLE
enum t_mode{FIX, SCI, ENG};
void v_version();
void v_about();
void v_error(const char *s_fmt, ...);
 
void v_init_calc();
void v_init_keypad(o_button* h_button[]);
#endif
