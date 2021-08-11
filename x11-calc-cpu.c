/*
 * x11-calc-cpu.c - RPN (Reverse Polish) calculator simulator. 
 *
 * Emulates processor operation.
 * 
 * Copyright(C) 2020   MT
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
 * Contains  the type definitions and functions definitions used to  emulate
 * the ACT processor with seven 56-bit Registers.  Each register consists of
 * 14  4-bit nibbles allowing it to hold a 10 digit mantissa with a 2  digit
 * exponent and signs for both the mantissa and exponent. 
 * 
 * A, B, C:    General purpose registers.  The C register contains the value
 *             shown on the display as 'X', and is also used to access the M
 *             register and or memory.
 * D, E, F:    Stack registers use to hold the values of 'Y', 'Z', and 'T'.
 * M:          Memory register.
 * 
 * P:          A 4-bit register used with the P and WP field selects (below)
 *             that allows subsets of the registers to be accessed.
 * DATA:       An 8-bit register holding the memory address used to read  or
 *             write to memory from the C register.
 * S0-S11:     Processor status bits.
 * 
 *    S0       Not used.
 *    S1       Scientific notation (clear for fixed point notation).
 *    S2       Auto Enter (if set entering digit will push 'X').
 *    S3       Set for radians clear for degrees.
 *    S4       Power OK (clear for lower power).
 *    S5       Set if decimal point has already been entered.
 *    S6       ?
 *    S7       ?
 *    S8       ?
 *    S9       ?
 *    S10      ?
 *    S11      ?
 *    S12      ?
 *    S13      Set if function key has been pressed.
 *    S14      ?
 *    S15      Set if any key is pressed.
 *
 * Each  16-bit  processor  instruction  is  retrieved  from  the  currently
 * selected  ROM  using either an 8-bit or 12-bit address (depending on  the
 * calculator  model) and allows each of the registers (or the selected part
 * of  each  register)  to  be  cleared,  copied,  exchanged,   incremented, 
 * decremented, shifted left or right and tested.
 *
 * 10 Sep 20         - Initial version - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments and added a
 *                     section describing the processor registers - MT
 *                   - Changed the status word into an integer - MT
 * 10 Aug 21         - Modified register names - MT
 *                   - Defined register size as a constant - MT
 *
 */

#define VERSION      "0.1"
#define BUILD        "0005"
#define DATE         "10 Aug 21"
#define AUTHOR       "MT"

#define DEBUG 1      /* Enable/disable debug*/
 
#include <stdio.h>   /* fprintf(), etc. */
#include <stdarg.h>

#include "x11-calc-cpu.h"
 
#include "gcc-debug.h"
 
unsigned char c_Areg[REG_SIZE], c_Breg[REG_SIZE], c_Creg[REG_SIZE];
unsigned char c_Dreg[REG_SIZE], c_Ereg[REG_SIZE], c_Freg[REG_SIZE];
unsigned char c_M1reg[REG_SIZE];
unsigned int i_Sreg; /* Processor status word */
unsigned int i_Preg; /* Prgram counter */

int i_cpu_init(){
   int i_count;
   size_t t_upb;
 
   /* Clear registers. */
   t_upb = sizeof(c_Areg)/sizeof(c_Areg[0]);
   for (i_count = 0; i_count < t_upb; i_count++) {
      c_Areg[i_count]= 0; c_Breg[i_count]= 0; c_Creg[i_count]= 0; 
      c_Dreg[i_count]= 0; c_Ereg[i_count]= 0; c_Freg[i_count]= 0;
      c_M1reg[i_count]=0; 
   }
   i_Preg = 0; /* Clear program counter */
   i_Sreg = 0; /* Clear processor status word */
}
 
int i_reg_load(unsigned char *c_reg, ...) {
   unsigned char c_temp;
   int i_count;
   size_t t_upb;
   va_list t_args;

   va_start(t_args, c_reg);
   t_upb = (sizeof(c_Areg)/sizeof(c_Areg[0])) - 1 ;
   for (i_count = 0; i_count <= t_upb; i_count++) {
      unsigned char c_temp = va_arg(t_args, int);
      c_reg[i_count]  = c_temp;
   }
   debug(fprintf(stderr, "Debug\t: %s line : %d : Load (", __FILE__, __LINE__);
      for (i_count = 0; i_count <= t_upb; i_count++) {
         fprintf(stderr, "0x%1x", c_reg[i_count]);
         if (i_count < t_upb) fprintf(stderr, ", ");
      }
      fprintf(stderr, ")\n"));
}

