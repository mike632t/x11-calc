/*
 * x11-calc-display.h - RPN (Reverse Polish) calculator simulator. 
 *
 * Copyright(C) 2020   MT
 * 
 * Emulates woodstock processor.
 * 
 * Contains  the type definations and functions definitions used to emulate
 * the CPU microcode.
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
 * 10 Sep 20         - Initial version - MT
 *
 */
  
extern unsigned char gc_a_reg[14];
extern unsigned char gc_b_reg[14];
extern unsigned char gc_c_reg[14];
extern unsigned char gc_d_reg[14];
extern unsigned char gc_e_reg[14];
extern unsigned char gc_f_reg[14];
extern unsigned char gc_m1_reg[14];
extern unsigned char gc_m2_reg[14];
extern unsigned char gc_s_reg[16]; /* Processor status word */

