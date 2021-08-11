/*
 * x11-calc-cpu.h - RPN (Reverse Polish) calculator simulator. 
 *
 * Copyright(C) 2020   MT
 * 
 * Emulates processor operation.
 * 
 * Contains  the type definitions and functions definitions used to  emulate
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

#define REG_SIZE 14
  
extern unsigned char c_Areg[REG_SIZE], c_Breg[REG_SIZE], c_Creg[REG_SIZE];
extern unsigned char c_Dreg[REG_SIZE], c_Ereg[REG_SIZE], c_Freg[REG_SIZE];
extern unsigned char c_M1reg[REG_SIZE];
extern unsigned int i_Sreg; /* Processor status word */
extern unsigned int i_Preg; /* Prgram counter */

int i_cpu_init();
int i_reg_load(unsigned char *c_reg, ...);
