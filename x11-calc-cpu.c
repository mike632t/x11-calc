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
 * 16 Aug 21         - Added a tick routine to execute a single instruction,
 *                     currently this only decodes two instructions (nop and
 *                     hi I'm woodstock) neither of which does anything. Any
 *                     unrecognised opcodes are simply ignored - MT
 * 19 Aug 21         - Created two new data structures to hold the processor
 *                     properties  and the register properties and added the
 *                     code to create a new processor instance in order to 
 *                     making it more object orientated - MT 
 * 20 Aug 21         - Re-implemented tick using the new data structures- MT
 *
 */

#define VERSION      "0.1"
#define BUILD        "0008"
#define DATE         "20 Aug 21"
#define AUTHOR       "MT"

#define DEBUG 1      /* Enable/disable debug*/

#define trace(code) do {if(f_trace){code;}} while(0)
 
#include <stdlib.h>    /* malloc(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
 
#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */
 
#include "x11-calc-font.h" 
#include "x11-calc-button.h"
//#include "x11-calc-colour.h"
#include "x11-calc-cpu.h"

#include "x11-calc.h"

#include "gcc-debug.h" /* print() */
#include "gcc-wait.h"  /* i_wait() */

o_register* h_register_create(int i_index){
   o_register* h_register; /* Pointer to register. */
   int i_count;
   if ((h_register = malloc (sizeof(*h_register)))==NULL) v_error("Memory allocation failed!"); /* Attempt to allocate memory to register. */
   for (i_count = 0; i_count < REG_SIZE; i_count++)
      h_register->nibble[i_count] = 0;
   return (h_register);
}

o_processor* h_processor_create(int i_index, int *h_rom)
{
   o_processor* h_processor;
   int i_count;
   if ((h_processor = malloc (sizeof(*h_processor)))==NULL) v_error("Memory allocation failed!"); /* Attempt to allocate memory to hold the processor structure. */
   for (i_count = 0; i_count < REGISTERS; i_count++)
      h_processor->processor_register[i_count] = h_register_create(0);
   h_processor->processor_status = 0; /* Processor status word */
   h_processor->program_counter = 0; /* Program counter */
   h_processor->current_rom_bank = 0; /* ROM bank number */
   h_processor->trace_flag = False ; /* Controls CPU trace output */ 
   h_processor->rom = h_rom ; /* Controls CPU trace output */ 
   debug(fprintf(stderr, "Debug\t: %s line : %d : ", __FILE__, __LINE__);
      fprintf(stderr, "ROM Size **: %4u words (%04o)\n",  sizeof(h_processor->rom), h_processor->rom[0]));
   return (h_processor);
}

int i_processor_tick(o_processor* h_processor) {

   int i_opcode;
   
   i_opcode = h_processor->rom[h_processor->program_counter];

   if (h_processor->trace_flag)
      fprintf(stdout, "%1o-%04o \t %04o \t", 0, h_processor->program_counter, h_processor->rom[h_processor->program_counter]);
   switch (i_opcode) {
      case 00000: /* nop */
         if (h_processor->trace_flag)
            fprintf(stdout, "nop");
         break;
      case 01760: /* hi I'm woodstock */
         if (h_processor->trace_flag)
            fprintf(stdout, "reserved");
         break;
      default: 
         break;
   }
      if (h_processor->trace_flag) fprintf(stdout, "\n");
   
   if (h_processor->program_counter++ >= (ROM_SIZE - 1)) h_processor->program_counter = 0; /* Increment program counter */   
   return(0);
}
 
//int i_reg_load(unsigned char *c_reg, ...) {
//   int i_count;
//   size_t t_upb;
//   va_list t_args;
//
//   va_start(t_args, c_reg);
//   t_upb = (sizeof(c_reg[A_REG])/sizeof(c_reg[A_REG][0])) - 1 ;
//   for (i_count = 0; i_count <= t_upb; i_count++) {
//      unsigned char c_temp = va_arg(t_args, int);
//      c_reg[i_count]  = c_temp;
//   }
//   debug(fprintf(stderr, "Debug\t: %s line : %d : ", __FILE__, __LINE__);
//      fprintf(stderr, "Load (");
//      for (i_count = 0; i_count <= t_upb; i_count++) {
//         fprintf(stderr, "0x%4o", c_reg[i_count]);
//         if (i_count < t_upb) fprintf(stderr, ", ");
//      }
//      fprintf(stderr, ")\n"));
//   return(0);
//}

