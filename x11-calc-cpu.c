/*
 * x11-calc-cpu.c - RPN (Reverse Polish) calculator simulator. 
 *
 * Emulates processor operation.
 * 
 * Copyright(C) 2020    MT
 * 
 * This processor simulator is based on the work of a number of individuals
 * including  Jacques LAPORTE, David HICKS, Greg SYDNEY-SMITH, Eric  SMITH,
 * Tony  NIXON and Bernhard EMESE.  Without their efforts and in some cases
 * assistance and encouragement this simulator not have been possible.
 * 
 * Thank you.  
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
 * Contains the type definitions and functions definitions used to  emulate
 * the  ACT processor with seven 56-bit Registers.  Each register  consists
 * of 14 4-bit nibbles capable of storing a 10 digit mantissa and a 2 digit
 * exponent with separate signs for both the mantissa and exponent. 
 * 
 * A, B, C:    General purpose registers.  The C register is used to access
 *             the M register and or memory as well as holding the value of
 *             the 'X' register. 
 * D, E, F:    Stack registers use to hold the values of 'Y', 'Z', and 'T'.
 * M, N:       Memory registers.
 * 
 * P:          A  4-bit register that is used to select which part of  each
 *             register should be used.
 * DATA:       An 8-bit register holding the memory address used to read or
 *             write to memory from the C register.
 * 
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
 * 20 Aug 21         - Re-implemented tick using the new data structures- 
 * 22 Aug 21         - Implemented the decoder for the four main instruction
 *                     types - MT
 * 24 Aug 21         - Added code to decode the field modifier - MT
 * 26 Aug 21         - Re-implemented  load_reg() to use the processor  data
 *                     and renamed it set_register() - MT
 * 
 * To Do             - Return status from tick().
 *                   - Finish instruction decoder!! 
 */

#define VERSION      "0.1"
#define BUILD        "0008"
#define DATE         "20 Aug 21"
#define AUTHOR       "MT"

#define DEBUG 0      /* Enable/disable debug*/

#define trace(code) do {if(flags[0]){code;}} while(0)
 
#include <stdlib.h>    /* malloc(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdarg.h>
 
#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */
 
#include "x11-calc-font.h" 
#include "x11-calc-button.h"
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
   if ((h_processor = malloc(sizeof(*h_processor)))==NULL) v_error("Memory allocation failed!"); /* Attempt to allocate memory to hold the processor structure. */
   for (i_count = 0; i_count < (sizeof(h_processor->reg) / sizeof(h_processor->reg[0])); i_count++)
      h_processor->reg[i_count] = h_register_create(0); /* Allocate storage for the registers */
   for (i_count = 0; i_count < (sizeof(h_processor->ram) / sizeof(h_processor->ram[0])); i_count++)
      h_processor->reg[i_count] = h_register_create(0); /* Allocate storage for the RAM */
   for (i_count = 0; i_count < (sizeof(h_processor->status) / sizeof(h_processor->status[0]) ); i_count++)
      h_processor->status[i_count] = 0; /* Clear the processor status word */
   for (i_count = 0; i_count < (sizeof(h_processor->stack) / sizeof(h_processor->stack[0]) ); i_count++)
      h_processor->stack[i_count] = 0; /* Clear the processor stack */
   for (i_count = 0; i_count < (sizeof(h_processor->flags) / sizeof(h_processor->flags[0]) ); i_count++)
      h_processor->flags[i_count] = True; /* Clear the processor flags */
   h_processor->pc = 0; /* Program counter */
   h_processor->sp = 0; /* Stack pointer */
   h_processor->p = 0;  /* Pointer register*/
   h_processor->data = 0; /* Data register */
   
   h_processor->bank = 0; /* ROM bank number */
   h_processor->rom = h_rom ; /* Address of ROM */ 
   return (h_processor);
}

int i_processor_tick(o_processor* h_processor) {

   unsigned int i_opcode; 
   unsigned int i_counter; /* New program counter */
   unsigned int i_field; /* Field modifier */
   unsigned int i_first; /* First nibble selected by field modifier */
   unsigned int i_last; /* Last nibble selected by field modifier */
   
   i_opcode = h_processor->rom[h_processor->pc];

   if (h_processor->flags[0])
      fprintf(stdout, "%1o-%04o \t %04o \t", h_processor->bank, h_processor->pc, h_processor->rom[h_processor->pc]);

   switch (i_opcode & 03) {
      case 00:
         /* 
          * Special operations - May be one or two word instructions!
          * 
          *   9   8   7   6   5   4   3   2   1   0
          * +---+---+---+---+---+---+---+---+---+---+
          * | n | n | n | n | n | n | n | n | 0 | 0 | 
          * +---+---+---+---+---+---+---+---+---+---+ 
          * 
          */
         switch (i_opcode) {
            case 00000: /* nop */
               if (h_processor->flags[0]) fprintf(stdout, "nop");
               break;
            case 01760: /* hi I'm woodstock */
               if (h_processor->flags[0]) fprintf(stdout, "reserved");
               break;
            default: 
               if (h_processor->flags[0]) fprintf(stdout, "special");
               break;
         }
         break;

      case 01:
         /* 
          * Jump Subroutine
          * 
          *   9   8   7   6   5   4   3   2   1   0
          * +---+---+---+---+---+---+---+---+---+---+
          * | n | n | n | n | n | n | n | n | 1 | 1 | 
          * +---+---+---+---+---+---+---+---+---+---+
          * 
          * Address format:
          * 
          *   7   6   5   4   3   2   1   0
          * +---+---+---+---+---+---+---+---+
          * | n | n | n | n | n | n | n | n |
          * +---+---+---+---+---+---+---+---+
          * 
          */

         i_counter = i_opcode >> 2; /* Get target address */
         if (h_processor->flags[0]) fprintf(stdout, "jsb\t %01o-%04o \t", h_processor->bank, i_counter);
         break;

      case 02:
         /*
          * Arithmetic operations.
          * 
          *   9   8   7   6   5   4   3   2   1   0
          * +---+---+---+---+---+---+---+---+---+---+
          * | n | n | n | n | n | m | m | m | 1 | 0 |
          * +---+---+---+---+---+---+---+---+---+---+
          * 
          * Where mmm is the field modifier.
          * 
          *  13   12  11  10  9   8   7   6   5   4   3   2   1   0
          * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
          * | s | m | m | m | m | m | m | m | m | m | m | s | e | e |
          * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
          * 
          * 000   P  : determined by P register             ([P])
          * 001   M  : mantissa                             ([3 .. 12])
          * 010   X  : exponent                             ([0 .. 1])
          * 011   W  : word                                 ([0 .. 13])
          * 100  WP  : word up to and including P register  ([0 .. P])
          * 101  MS  : mantissa and sign                    ([3 .. 13])
          * 110  XS  : exponent and sign                    ([0 .. 2])
          * 111   S  : sign                                 ([13])
          *
          */

         if (h_processor->flags[0]) fprintf(stdout, "arithmetic");
         
         i_field = (i_opcode >> 2) & 7;
         i_opcode = i_opcode >> 5;
         
         switch (i_field) {
            case 0:  /* 000   P  : determined by P register             ([P]) */
               i_first = h_processor->p; i_last = h_processor->p;
               if (h_processor->p >= REG_SIZE) {
                  fprintf(stderr, "Run-time error\t: %s line : %d : %s", \
                  __FILE__, __LINE__, "Invalid register pointer\n");
                  i_last = 0;
               }
               break;
            case 1:  /* 001   M  : mantissa                             ([3 .. 12]) */
               i_first = EXP_SIZE; i_last = REG_SIZE - 2;
               break;
            case 2:  /* 010   X  : exponent                             ([0 .. 1])  */
               i_first = 0; i_last = EXP_SIZE - 1;
               break;
            case 3:  /* 011   W  : word                                 ([0 .. 13]) */
               i_first = 0; i_last = REG_SIZE - 1;
               break;
            case 4:  /* 100  WP  : word up to and including P register  ([0 .. P])  */
               i_first =  0; i_last =  h_processor->p; /* break; bug in orig??? */
               if (h_processor->p >= REG_SIZE) {
                  fprintf(stderr, "Run-time error\t: %s line : %d : %s", \
                  __FILE__, __LINE__, "Invalid register pointer\n");
                  i_last = REG_SIZE - 1;
               }
               break;
            case 5:  /* 101  MS  : mantissa and sign                    ([3 .. 13]) */
               i_first = EXP_SIZE; i_last = REG_SIZE - 1;
               break;
            case 6:  /* 110  XS  : exponent and sign                    ([0 .. 2])  */
               i_first = EXP_SIZE - 1; i_last = EXP_SIZE - 1;
               break;
            case 7:  /* 111   S  : sign                                 ([13])      */  
               i_first = REG_SIZE - 1; i_last = REG_SIZE - 1;
               break;
         }         
                
         // TO DO : Use lookup table ... (only 32 permutations..)
             
         switch (i_opcode) {
            default: 
               break;
         }
         break;
   
      case 03:
         /*
          * Subroutine and long conditional jumps.
          * 
          *   9   8   7   6   5   4   3   2   1   0       9   8   7   6   5   4   3   2   1   0
          * +---+---+---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+---+---+
          * | l | l | l | l | l | l | l | l | 0 | 1 |   | h | h | h | h | h | h | h | h | t | t |
          * +---+---+---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+---+---+
          * 
          * Where tt is the type of jump:
          * 
          * 00       : subroutine call if carry clear
          * 01       : subroutine call if carry set
          * 10       : jump if carry clear
          * 11       : jump if carry set
          * 
          * Address format:
          * 
          *  15  14  13  12  11  10   9   8       7   6   5   4   3   2   1   0
          * +---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+
          * | h | h | h | h | h | h | h | h |   | l | l | l | l | l | l | l | l |
          * +---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+
          * 
          */
         i_counter = i_opcode >> 2; 
         debug(fprintf(stderr,"%06o %06o\n", i_opcode, i_counter));
         if (h_processor->pc++ >= (ROM_SIZE - 1)) h_processor->pc = 0; /* Increment program counter */   
         i_opcode = h_processor->rom[h_processor->pc];
         /* TO DO - Fix Counter !! */
         debug(fprintf(stderr,"%06o %06o\n", i_opcode, i_counter));
         i_counter = i_counter | ((i_opcode >> 2) << 8); /* Should not be bigger than 4095 .*/
         switch (i_opcode & 00003) {
            case 00:
               if (h_processor->flags[0])
                  fprintf(stdout, "call\t %01o-%04o \t", h_processor->bank, i_counter);
               break;
            case 01:
               if (h_processor->flags[0])
                  fprintf(stdout, "call\t %01o-%04o \t", h_processor->bank, i_counter);
               break;
            case 02:
               if (h_processor->flags[0])
                  fprintf(stdout, "jump\t %01o-%04o \t", h_processor->bank, i_counter);
               break;
            case 03:
               if (h_processor->flags[0])
                  fprintf(stdout, "jump\t %01o-%04o \t", h_processor->bank, i_counter);
               break;
         }
         break;

   }

   if (h_processor->flags[0]) fprintf(stdout, "\n");
   
   if (h_processor->pc++ >= (ROM_SIZE - 1)) h_processor->pc = 0; /* Increment program counter */   
   return(0); /* TO DO: Return status */
}

char c_add_nibble(char a, char b, char carry, int base) {
   char c = 0;
   c = a + a + carry;
   carry = 0;
   if (c >= base) {
      c -= base;
      carry = 1;
   } 
   return(c);
}

char c_sub_nibble(char a, char b, char carry, int base) {
   char c = 0;
   c = (a - c) - carry;
   carry = 0;
   if (c < 0) {
      c += base;
      carry = 1;
   }
   return(c);
}

int i_set_register(o_register* h_register, ...) {

   int i_count;
   va_list t_args;

   va_start(t_args, h_register);
   for (i_count = 0; i_count <= REG_SIZE; i_count++) {
      unsigned char c_temp = va_arg(t_args, int);
      h_register->nibble[i_count]  = c_temp;
   }
   debug(fprintf(stderr, "Load (");
      for (i_count = 0; i_count <= REG_SIZE; i_count++) {
         fprintf(stderr, "0x%02x", h_register->nibble[i_count]);
         if (i_count < REG_SIZE) fprintf(stderr, ", ");
      }
      fprintf(stderr, ")\n"));
   return(0);  /* Return status */
}

