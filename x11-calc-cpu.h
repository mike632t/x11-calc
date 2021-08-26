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
 * 24 Aug 21         - Added  properties for the p and data registers,  and
 *                     an  array of flags to keep track of things like  the
 *                     arithmetic mode and instruction tracing - MT 
 *
 */

#ifndef REGISTERS
#include "x11-calc.h"

#define REGISTERS    8     /* A, B , C(X), D(Y), E(Z), F(T), M, N(M2) */
#define REG_SIZE     14   
#define EXP_SIZE     3     /* Two digit exponent plus a sign digit */
#define STACK_DEPTH  2

#define A_REG        0
#define B_REG        1
#define C_REG        2
#define Y_REG        3
#define Z_REG        4
#define T_REG        5
#define M_REG        6
#define N_REG        7

typedef struct {
   unsigned char nibble[REG_SIZE];
} o_register;

typedef struct { 
   int index; 
   o_register* reg[REGISTERS];
   o_register* ram[RAM_SIZE]; /* Pointer to the ROM contents */
   unsigned int stack[STACK_DEPTH]; /* Call stack */
   unsigned int pc;  /* Program counter */
   unsigned int sp;  /* Stack pointer */
   unsigned int p;   /* P register */
   unsigned int data;   /* Data register */
   unsigned char status[16];  /* Processor status word */

   int flags[4] ; /* Processor flags */ 
   unsigned int bank; /* ROM bank number */
   int* rom; /* Pointer to the ROM contents */
} o_processor;  

o_processor* h_processor_create(int i_index, int *h_rom);
   
int i_processor_tick(o_processor* h_procesor);

int i_set_register(o_register* c_reg, ...);
#endif
