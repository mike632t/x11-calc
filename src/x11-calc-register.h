/*
 * x11-calc-register.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2025   MT
 *
 * Contains the type definitions for a processor register .
 *
 * This  program is free software: you can redistribute it and/or modify it
 * under  the terms of the GNU General Public License as published  by  the
 * Free  Software Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is distributed in the hope that it will  be  useful, but
 * WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 14 Nov 25         - Initial version - MT
 *
 */

#ifndef REGISTER_H
#define REGISTER_H

#define REG_SIZE        14
#define EXP_SIZE        3              /* Two digit exponent plus a sign digit */

typedef struct {
   int id;
   unsigned char nibble[REG_SIZE];
} oregister;

oregister *h_register_create(int i_id);  /* Create a new register */

void v_fprint_register(FILE *h_file, oregister *h_register);  /* Print the contents of a register */

#endif
