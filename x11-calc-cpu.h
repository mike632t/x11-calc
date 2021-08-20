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

#define REG_SIZE     14
#define REGISTERS    9     /* A, B ,C , D, E, F, M1, PSW, PC */


#define A_REG        0
#define B_REG        1
#define C_REG        2
#define D_REG        3
#define E_REG        4
#define F_REG        5
#define M_REG        6
#define M2_REG       7

typedef struct {
   unsigned char nibble[REG_SIZE];
} o_register;

typedef struct { 
   int index; 
   o_register* processor_register[REGISTERS];
   unsigned int processor_status; /* Processor status word */
   unsigned int program_counter; /* Program counter */
   unsigned int current_rom_bank; /* ROM bank number */
   int trace_flag; /* Controls CPU trace output */ 
   int* rom; /* Pointer to the ROM contents */
} o_processor;  

o_processor* h_processor_create(int i_index, int *h_rom);
   
//int i_processor_init(o_processor* h_procesor);

int i_processor_tick(o_processor* h_procesor);

int i_reg_load(unsigned char *c_reg, ...);
