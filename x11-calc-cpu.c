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
 * Registers
 * 
 * A, B, C:    General purpose registers.  The C register is used to access
 *             the M register and or memory as well as holding the value of
 *             the 'X' register. 
 * Y, Z, T:    Stack registers.
 * M, N:       Memory registers.
 * 
 * F:          F register.
 * P:          A  4-bit register that is used to select which part of  each
 *             register should be used.
 * DATA:       An 8-bit register holding the memory address used to read or
 *             write to memory from the C register.
 * SP:         Stack pointer
 * 
 * Processor flags
 * 
 *    F0       Selects Run / Program mode.
 *    F1       Carry.
 *    F2       Prev Carry.
 *    F3       Delayed ROM select.
 *    F4       Bank switch
 *    F5       Display enabled
 *    F6       ???
 *    F7       ???
 *    F8       Timer.
 * 
 * Processor status word.
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
 *                   - Reversed order of nibbles in register to match actual
 *                     processor  numbering  (when the register format  says
 *                     the left hand nibble is number 13 it means it!) - MT
 *                   - Implemented my first real op code (0 -> a[f]) - MT
 * 
 * To Do             - Return status from tick().
 *                   - Finish instruction decoder!! 
 */

#define VERSION        "0.1"
#define BUILD          "0008"
#define DATE           "20 Aug 21"
#define AUTHOR         "MT"

#define DEBUG          1   /* Enable/disable debug*/

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

void v_reg_fprint(FILE *h_file, oregister *h_register) {
   const char c_name[8] = {'A', 'B', 'X', 'Y', 'Z', 'T', 'M', 'N'};
   int i_count;
   if (h_register != NULL) {
      fprintf(h_file, "reg[");
      if (h_register->id < 0) 
         fprintf(h_file, "*%c", c_name[h_register->id * -1 - 1]);
      else 
         fprintf(h_file, "%02d", c_name[h_register->id]);
      fprintf(h_file, "] = 0x");
      for (i_count = REG_SIZE - 1; i_count >=0 ; i_count--) {
         fprintf(h_file, "%1x", h_register->nibble[i_count]);
      }
      fprintf(h_file, " ");
   }
}

oregister *h_register_create(char c_id){
   oregister *h_register; /* Pointer to register. */
   int i_count, i_temp;
   if ((h_register = malloc (sizeof(*h_register))) == NULL) v_error("Memory allocation failed!"); /* Attempt to allocate memory to register. */
   i_temp = sizeof(h_register->nibble) / sizeof(*h_register->nibble);
   h_register->id = c_id;
   for (i_count = 0; i_count < i_temp; i_count++)
      h_register->nibble[i_count] = 0;
   debug(v_reg_fprint(stdout, h_register); fprintf(stdout, "\n"));
   return (h_register);
}

oprocessor *h_processor_create(int *h_rom)
{
   oprocessor *h_processor;
   int i_count;
   if ((h_processor = malloc(sizeof(*h_processor)))==NULL) v_error("Memory allocation failed!"); /* Attempt to allocate memory to hold the processor structure. */
   for (i_count = 0; i_count < (sizeof(h_processor->reg) / sizeof(h_processor->reg[0])); i_count++)
      h_processor->reg[i_count] = h_register_create(- (i_count + 1)); /* Allocate storage for the registers */
   for (i_count = 0; i_count < (sizeof(h_processor->ram) / sizeof(h_processor->ram[0])); i_count++)
      h_processor->reg[i_count] = h_register_create(i_count); /* Allocate storage for the RAM */
   for (i_count = 0; i_count < (sizeof(h_processor->status) / sizeof(h_processor->status[0]) ); i_count++)
      h_processor->status[i_count] = 0; /* Clear the processor status word */
   for (i_count = 0; i_count < (sizeof(h_processor->stack) / sizeof(h_processor->stack[0]) ); i_count++)
      h_processor->stack[i_count] = 0; /* Clear the processor stack */
   for (i_count = 0; i_count < (sizeof(h_processor->flags) / sizeof(h_processor->flags[TRACE]) ); i_count++)
      h_processor->flags[i_count] = True; /* Clear the processor flags */
   h_processor->pc = 0; /* Program counter */
   h_processor->sp = 0; /* Stack pointer */
   h_processor->p = 0;  /* Pointer register*/
   h_processor->f = 0;  /* F register*/
   h_processor->data = 0; /* Data register */
   h_processor->base = 10; /* Data register */
   h_processor->carry = 0; /* Carry */
   
   h_processor->bank = 0; /* ROM bank number */
   h_processor->rom = h_rom ; /* Address of ROM */ 
   return (h_processor);
}

void v_set_register(oregister *h_register, ...) {

   int i_count, i_temp;
   unsigned char c_temp;
   va_list t_args;

   va_start(t_args, h_register);
   i_temp = sizeof(h_register->nibble) / sizeof(*h_register->nibble) - 1;
   for (i_count = i_temp; i_count >= 0; i_count--) 
      h_register->nibble[i_count]  = va_arg(t_args, int);
}

void v_reg_zero(oregister *h_register, int i_first, int i_last) {
   int i_count;
   for (i_count = i_first; i_count <= i_last; i_count++)
      h_register->nibble[i_count] = 0;
}

void v_reg_exch (oregister *h_destination, oregister *h_source, int i_first, int i_last) {
   int i_count, i_temp;
   for (i_count = i_first; i_count <= i_last; i_count++) {
      i_temp = h_destination->nibble[i_count];
      h_destination->nibble[i_count] = h_source->nibble[i_count];
      h_source->nibble[i_count] = i_temp;
   }
}

void v_reg_add (oregister *h_destination, oregister *h_source, int i_first, int i_last, char *carry, unsigned int i_base){
   int i_count, i_temp;
   for (i_count = i_first; i_count <= i_last; i_count++){
      if (h_source != NULL) i_temp = h_source->nibble[i_count]; else i_temp = 0;
      i_temp = h_destination->nibble[i_count] + i_temp + *carry;
      if (i_temp >= i_base){
         i_temp -= i_base;
         *carry = 1;
      } else {
         *carry = 0;
      }
      h_destination->nibble[i_count] = i_temp;
   }
}

void v_reg_inc (oregister *h_register, int i_first, int i_last, char *carry, unsigned int i_base){
   *carry = 1; /* Set carry */
   v_reg_add (h_register, NULL, i_first, i_last, carry, i_base); /* Add carry to register */
}

int i_processor_tick(oprocessor *h_processor) {

   unsigned int i_opcode; 
   unsigned int i_counter; /* New program counter */
   unsigned int i_field; /* Field modifier */
   unsigned int i_first; /* First nibble selected by field modifier */
   unsigned int i_last; /* Last nibble selected by field modifier */
   const char *s_field; /* Holds pointer to field name */
   
   i_opcode = h_processor->rom[h_processor->pc];

   if (h_processor->flags[TRACE])
      fprintf(stdout, "%1o-%04o\t %04o\t ", h_processor->bank, h_processor->pc, h_processor->rom[h_processor->pc]);

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
               if (h_processor->flags[TRACE]) fprintf(stdout, "nop");
               break;
            case 00410: /* m exch c */ 
               if (h_processor->flags[TRACE]) {
                  fprintf(stdout, "m exch c \t");
                  v_reg_fprint(stdout, h_processor->reg[M_REG]);
                  v_reg_fprint(stdout, h_processor->reg[C_REG]);
                  fprintf(stdout, "\t");
               }
               v_reg_exch(h_processor->reg[M_REG], h_processor->reg[C_REG], 0, sizeof(h_processor->reg[C_REG]->nibble));               
               if (h_processor->flags[TRACE]) {
                  fprintf(stdout, "m exch c \t");
                  v_reg_fprint(stdout, h_processor->reg[M_REG]);
                  v_reg_fprint(stdout, h_processor->reg[C_REG]);
               }
               break;
            case 01710: /* f exchange a */
               {
                  int i_temp;
                  i_temp = h_processor->reg[A_REG]->nibble[0];
                  h_processor->reg[A_REG]->nibble[0] = h_processor->f;
                  h_processor->f = i_temp;
               }
               if (h_processor->flags[TRACE]) {
                  fprintf(stdout, "f exch a \t");
                  v_reg_fprint(stdout, h_processor->reg[A_REG]);
               }
               break;
            case 01760: /* hi I'm woodstock */
               if (h_processor->flags[TRACE]) fprintf(stdout, "hi I'm woodstock");
               break;
            default: 
               break;
         }
         if (h_processor->flags[TRACE]) fprintf(stdout, "\n");
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
         if (h_processor->flags[TRACE]) fprintf(stdout, "jsb\t %01o-%04o\n", h_processor->bank, i_counter);
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
          * 001  WP  : word up to and including P register  ([0 .. P])
          * 010  XS  : exponent and sign                    ([0 .. 2])
          * 011   X  : exponent                             ([0 .. 1])
          * 100   S  : sign                                 ([13])
          * 101   M  : mantissa                             ([3 .. 12])
          * 110   W  : word                                 ([0 .. 13])
          * 111  MS  : mantissa and sign                    ([3 .. 13])
          *
          */
          
         i_field = (i_opcode >> 2) & 7;
        
         switch (i_field) {
            case 0:  /* 000   P  : determined by P register             ([P]) */
               i_first = h_processor->p; i_last = h_processor->p;
               s_field = "p";
               if (h_processor->p >= REG_SIZE) {
                  fprintf(stderr, "Run-time error\t: %s line : %d : %s", \
                  __FILE__, __LINE__, "Invalid register pointer\n");
                  i_last = 0;
               }
               break;
            case 1:  /* 100  WP  : word up to and including P register  ([0 .. P])  */
               i_first =  0; i_last =  h_processor->p; /* break; bug in orig??? */
               s_field = "wp";
               if (h_processor->p >= REG_SIZE) {
                  fprintf(stderr, "Run-time error\t: %s line : %d : %s", \
                  __FILE__, __LINE__, "Invalid register pointer\n");
                  i_last = REG_SIZE - 1;
               }
               break;
            case 2:  /* 110  XS  : exponent and sign                    ([0 .. 2])  */
               i_first = EXP_SIZE - 1; i_last = EXP_SIZE - 1;
               s_field = "xs";
               break;
            case 3:  /* 010   X  : exponent                             ([0 .. 1])  */
               i_first = 0; i_last = EXP_SIZE - 1;
               s_field = "x";
               break;
            case 4:  /* 111   S  : sign                                 ([13])      */  
               i_first = REG_SIZE - 1; i_last = REG_SIZE - 1;
               s_field = "s";
               break;
            case 5:  /* 001   M  : mantissa                             ([3 .. 12]) */
               i_first = EXP_SIZE; i_last = REG_SIZE - 2;
               s_field = "m";
               break;
            case 6:  /* 011   W  : word                                 ([0 .. 13]) */
               i_first = 0; i_last = REG_SIZE - 1;
               s_field = "w";
               break;
            case 7:  /* 101  MS  : mantissa and sign                    ([3 .. 13]) */
               i_first = EXP_SIZE; i_last = REG_SIZE - 1;
               s_field = "ms";
               break;
         }         
                
         /* act_flags &= ~F.CARRY; /* is already cleared by execute_instruction */
         
         switch (i_opcode >> 5)
         {
            case 0000:  /* 0 -> a[f] */
               v_reg_zero(h_processor->reg[A_REG], i_first, i_last);
               if (h_processor->flags[TRACE]) {
                  fprintf(stdout, "0 -> a[%s]\t", s_field);
                  v_reg_fprint(stdout, h_processor->reg[A_REG]);                  
               }
               break;
            case 0001:  /* 0 -> b[f] */
               v_reg_zero(h_processor->reg[B_REG], i_first, i_last);
               if (h_processor->flags[TRACE]) {
                  fprintf(stdout, "0 -> b[%s]\t", s_field);
                  v_reg_fprint(stdout, h_processor->reg[B_REG]);                  
               }
               break;
            case 0002:  /* a exch b[f] */
               v_reg_exch(h_processor->reg[A_REG], h_processor->reg[B_REG], i_first, i_last);
               if (h_processor->flags[TRACE]) {
                  fprintf(stdout, "a exch b[%s]\t", s_field);
                  v_reg_fprint(stdout, h_processor->reg[A_REG]);                  
                  v_reg_fprint(stdout, h_processor->reg[B_REG]);                  
               }
               break;            
            //case 0003:  /* a -> b[f] */
               //src = act_a; dest = act_b; reg_copy(); break;
            //case 0004:  /* a exchange c[f] */
               v_reg_exch(h_processor->reg[A_REG], h_processor->reg[C_REG], i_first, i_last);
               if (h_processor->flags[TRACE]) {
                  fprintf(stdout, "a exch c[%s]\t", s_field);
                  v_reg_fprint(stdout, h_processor->reg[A_REG]);                  
                  v_reg_fprint(stdout, h_processor->reg[C_REG]);                  
               }
               break;            
            //case 0005:  /* c -> a[f] */
               //src = act_c; dest = act_a; reg_copy(); break;
            //case 0006:  /* b -> c[f] */
               //src = act_b; dest = act_c; reg_copy(); break;
            //case 0007:  /* b exchange c[f] */
               v_reg_exch(h_processor->reg[B_REG], h_processor->reg[C_REG], i_first, i_last);
               if (h_processor->flags[TRACE]) {
                  fprintf(stdout, "b exch c[%s]\t", s_field);
                  v_reg_fprint(stdout, h_processor->reg[B_REG]);                  
                  v_reg_fprint(stdout, h_processor->reg[C_REG]);                  
               }
               break;            
            case 0010:  /* 0 -> c[f] */
               v_reg_zero(h_processor->reg[C_REG], i_first, i_last);
               if (h_processor->flags[TRACE]) {
                  fprintf(stdout, "0 -> c[%s]\t", s_field);
                  v_reg_fprint(stdout, h_processor->reg[C_REG]);                  
               }
               break;
            //case 0011:  /* a + b -> a[f] */
               //dest = act_a; src = act_b; reg_add(); break;
            //case 0012:  /* a + c -> a[f] */
               //dest = act_a; src = act_c; reg_add(); break;
            //case 0013:  /* c + c -> c[f] */
               //dest = act_c; src = act_c; reg_add(); break;
            //case 0014:  /* a + c -> c[f] */
               //dest = act_c; src = act_a; reg_add(); break;
            case 0015:  /* a + 1 -> a[f] */
               v_reg_inc (h_processor->reg[A_REG], i_first, i_last, &h_processor->carry, h_processor->base);
               if (h_processor->flags[TRACE]) {
                  fprintf(stdout, "a + 1 -> a[%s]\t", s_field);
                  v_reg_fprint(stdout, h_processor->reg[A_REG]);                  
               }
               break;
            //case 0016:  /* shift left a[f] */
               //dest = act_a; reg_shift_left(); break;
            //case 0017:  /* c + 1 -> c[f] */
               //dest = act_c; reg_inc(); break;
            //case 0020:  /* a - b -> a[f] */
               //dest = act_a; src = act_a; src2 = act_b; reg_sub(); break;
            //case 0021:  /* a - c -> c[f] */
               //dest = act_c; src = act_a; src2 = act_c; reg_sub(); break;
            //case 0022:  /* a - 1 -> a[f] */
               //act_flags |= F.CARRY; dest = act_a; src = act_a; src2 = null; reg_sub(); break;
            //case 0023:  /* c - 1 -> c[f] */
               //act_flags |= F.CARRY; dest = act_c; src = act_c; src2 = null; reg_sub(); break;
            //case 0024:  /* 0 - c -> c[f] */
               //dest = act_c; src = null; src2 = act_c; reg_sub(); break;
            //case 0025:  /* 0 - c - 1 -> c[f] */
               //act_flags |= F.CARRY; dest = act_c; src = null; src2 = act_c; reg_sub(); break;
            //case 0026:  /* if b[f] = 0 */
               //act_inst_state = ST.branch; src = act_b;  dest=null; reg_test_nonequal(); break;
            //case 0027:  /* if c[f] = 0 */
               //act_inst_state = ST.branch; src = act_c;  dest=null; reg_test_nonequal(); break;
            //case 0030:  /* if a >= c[f] */
               //act_inst_state = ST.branch; dest = null; src = act_a; src2 = act_c; reg_sub(); break;
            //case 0031:  /* if a >= b[f] */
               //act_inst_state = ST.branch; dest = null; src = act_a; src2 = act_b; reg_sub(); break;
            //case 0032:  /* if a[f] # 0 */
               //act_inst_state = ST.branch; src = act_a; dest=null; reg_test_equal(); break;
            //case 0033:  /* if c[f] # 0 */
               //act_inst_state = ST.branch; src = act_c;  dest=null; reg_test_equal(); break;
            //case 0034:  /* a - c -> a[f] */
               //dest = act_a; src = act_a; src2 = act_c; reg_sub(); break;
            //case 0035:  /* shift right a[f] */
               //dest = act_a; reg_shift_right(); break;
            //case 0036:  /* shift right b[f] */
               //dest = act_b; reg_shift_right(); break;
            //case 0037:  /* shift right c[f] */
               //dest = act_c; reg_shift_right(); break;
            default:
               if (h_processor->flags[TRACE]) fprintf(stdout, "arithmetic");
               break;
         }
         if (h_processor->flags[TRACE]) fprintf(stdout, "\n");
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
               if (h_processor->flags[TRACE])
                  fprintf(stdout, "call\t %01o-%04o\n", h_processor->bank, i_counter);
               break;
            case 01:
               if (h_processor->flags[TRACE])
                  fprintf(stdout, "call\t %01o-%04o\n", h_processor->bank, i_counter);
               break;
            case 02:
               if (h_processor->flags[TRACE])
                  fprintf(stdout, "jump\t %01o-%04o\n", h_processor->bank, i_counter);
               break;
            case 03:
               if (h_processor->flags[TRACE])
                  fprintf(stdout, "jump\t %01o-%04o\n", h_processor->bank, i_counter);
               break;
         }
         break;
   }

   if (h_processor->pc++ >= (ROM_SIZE - 1)) h_processor->pc = 0; /* Increment program counter */   
   return(0); /* TO DO: Return status */
}
