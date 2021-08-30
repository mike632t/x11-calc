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
 * Each  16-bit  processor  instruction is  retrieved  from  the  currently
 * selected  ROM using either an 8-bit or 12-bit address (depending on  the
 * calculator model) and allows each of the registers (or the selected part
 * of  each  register)  to  be  cleared, copied,  exchanged,   incremented, 
 * decremented, shifted left or right and tested.
 * 
 * Arithmetic Registers
 * 
 *  13   12  11  10  9   8   7   6   5   4   3   2   1   0
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * | s | m | m | m | m | m | m | m | m | m | m | s | e | e |
 * +---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 *
 * A, B, C:    General purpose registers.  The C register is used to access
 *             the M register and or memory as well as holding the value of
 *             the 'X' register. 
 * Y, Z, T:    Stack registers.
 * M, N:       Memory registers.
 *
 * Special purpose registers 
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
 * F0          Selects Run / Program mode.
 * F1          Carry.
 * F2          Prev Carry.
 * F3          Delayed ROM select.
 * F4          Bank switch
 * F5          Display enabled
 * F6          ???
 * F7          ???
 * F8          Timer.
 * F9          Trace enabled (implementation specific!).
 * 
 * Processor status word.
 * 
 * S0          Not used.
 * S1          Scientific notation (clear for fixed point notation).
 * S2          Auto Enter (if set entering digit will push 'X').
 * S3          Set for radians clear for degrees.
 * S4          Power OK (clear for lower power).
 * S5          Set if decimal point has already been entered.
 * S6          ?
 * S7          ?
 * S8          ?
 * S9          ?
 * S10         ?
 * S11         ?
 * S12        ?
 * S13         Set if function key has been pressed.
 * S14         ?
 * S15        Set if any key is pressed.
 *
 * Instruction set
 * 
 * Special operations - May be one or two word instructions!
 * 
 *   9   8   7   6   5   4   3   2   1   0
 * +---+---+---+---+---+---+---+---+---+---+
 * | n | n | n | n | n | n | n | n | 0 | 0 | 
 * +---+---+---+---+---+---+---+---+---+---+ 
 * 
 * Jump Subroutine
 * 
 *   9   8   7   6   5   4   3   2   1   0
 * +---+---+---+---+---+---+---+---+---+---+
 * | n | n | n | n | n | n | n | n | 1 | 1 | 
 * +---+---+---+---+---+---+---+---+---+---+
 * 
 *   7   6   5   4   3   2   1   0
 * +---+---+---+---+---+---+---+---+
 * | n | n | n | n | n | n | n | n | Address 
 * +---+---+---+---+---+---+---+---+
 * 
 * Arithmetic operations.
 * 
 *   9   8   7   6   5   4   3   2   1   0
 * +---+---+---+---+---+---+---+---+---+---+
 * | n | n | n | n | n | m | m | m | 1 | 0 |
 * +---+---+---+---+---+---+---+---+---+---+
 * 
 * Where mmm is the field modifier
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
 * Subroutine calls and long conditional jumps.
 * 
 *   9   8   7   6   5   4   3   2   1   0
 * +---+---+---+---+---+---+---+---+---+---+
 * | l | l | l | l | l | l | l | l | 0 | 1 |
 * +---+---+---+---+---+---+---+---+---+---+
 * 
 *   9   8   7   6   5   4   3   2   1   0
 * +---+---+---+---+---+---+---+---+---+---+
 * | h | h | h | h | h | h | h | h | t | t |
 * +---+---+---+---+---+---+---+---+---+---+
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
 *
 * 
 *
 * 10 Sep 20         - Initial version - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments and  added
 *                     a section describing the processor registers - MT
 *                   - Changed the status word into an integer - MT
 * 10 Aug 21         - Modified register names - MT
 *                   - Defined register size as a constant - MT
 * 16 Aug 21         - Added   tick()  to  execute  a  single  instruction,
 *                     currently this only decodes two  instructions ('nop'
 *                     and  'hi I'm woodstock')  neither of which  actually
 *                     does anything.  Any unrecognised opcodes are  simply
 *                     ignored - MT
 * 19 Aug 21         - Created  two data structures to hold  the  processor
 *                     properties and the register properties and added the
 *                     code to create a new processor instance - MT 
 * 20 Aug 21         - Updated tick() using the new data structures - MT
 * 22 Aug 21         - Can now decode the four main instruction types - MT
 * 24 Aug 21         - Added code to decode the field modifier - MT
 * 26 Aug 21         - Re-implemented load_reg() to use the processor  data
 *                     and renamed it set_register() - MT
 *                   - Reversed order of nibbles in each register to  match
 *                     the actual numbering (when the register format  says
 *                     the left hand nibble is number 13 it means it!) - MT
 *                   - Implemented my first real op code (0 -> a[f]) - MT
 * 27 Aug 21         - Added code to exchange register contents -MT 
 * 28 Aug 21         - Created a separate method to display the contents of
 *                     a  register  (replacing the existing debug  code  in 
 *                     register load and exchange routines) - MT 
 *                   - Implemented routines to add and the contents of  two
 *                     registers  and used this to increment a register  by
 *                     setting the carry flag and then calling add register
 *                     with a single parameter - MT
 *                   - Added an identifier to the register properties  which
 *                     allows  the register name to be shown when  printing
 *                     the value - MT
 * 29 Aug 21         - Tidied up trace output - MT
 *                   - Changed the register names - MT
 * 30 Aug 21         - Changed  carry to use the processor flags, and  made
 *                     the  first  and last values used when selecting  the
 *                     field in a register processor properties - MT
 *                   - Changed all flags and status bits to integers - MT
 *                   - Run mode set by default - MT
 *                   - Tidied up the comments (again) - MT
 *                   - Separated the 'special' instruction into four groups
 *                     to make it easier to parse the parameters - MT
 *                   - Added several more functions - MT
 *                   - Implemented code to handle subroutines - MT
 *                   - Fixed compiler warnings - MT
 *                   - Added rom select, and register copy - MT
 * 
 * To Do             - Return status from tick().
 *                   - Finish instruction decoder!! 
 */

#define VERSION        "0.1"
#define BUILD          "0008"
#define DATE           "20 Aug 21"
#define AUTHOR         "MT"

#define DEBUG 0        /* Enable/disable debug*/

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


/* Print the content of a register */
void v_reg_fprint(FILE *h_file, oregister *h_register) {
   const char c_name[8] = {'A', 'B', 'C', 'Y', 'Z', 'T', 'M', 'N'};
   int i_count;
   if (h_register != NULL) {
      fprintf(h_file, "reg[");
      if (h_register->id < 0) 
         fprintf(h_file, "*%c", c_name[h_register->id * -1 - 1]);
      else 
         fprintf(h_file, "%02d", h_register->id);
      fprintf(h_file, "] = 0x");
      for (i_count = REG_SIZE - 1; i_count >=0 ; i_count--) {
         fprintf(h_file, "%1x", h_register->nibble[i_count]);
      }
      fprintf(h_file, " ");
   }
}

/* Display the current processor status word */
void v_status_fprint(FILE *h_file,oprocessor *h_processor) {
   int i_count, i_temp = 0;
   if (h_processor != NULL) {
      fprintf(h_file, "(");
      for (i_count = 0; i_count < (sizeof(h_processor->status) / sizeof(*h_processor->status)); i_count++)
         i_temp = i_temp & h_processor->status[i_count];
      fprintf(h_file, "0x%04x", i_temp);
      fprintf(h_file, ") ");
   }
}

/* Display the current processor flags */
void v_flags_fprint(FILE *h_file,oprocessor *h_processor) {
   int i_count, i_temp = 0;
   if (h_processor != NULL) {
      fprintf(h_file, "(");
      for (i_count = 0; i_count < TRACE; i_count++) /* Ignore TRACE flag */
         i_temp = i_temp + (h_processor->flags[i_count] << i_count);
      fprintf(h_file, "0x%04x", i_temp);
      fprintf(h_file, ") ");
   }
}

/* Create a new register object */
oregister *h_register_create(char c_id){
   oregister *h_register; /* Pointer to register. */
   int i_count, i_temp;
   if ((h_register = malloc (sizeof(*h_register))) == NULL) {
      fprintf(stderr, "Run-time error\t: %s line : %d : %s", \
      __FILE__, __LINE__, "Memory allocation failed!\n");
   }
   i_temp = sizeof(h_register->nibble) / sizeof(*h_register->nibble);
   h_register->id = c_id;
   for (i_count = 0; i_count < i_temp; i_count++)
      h_register->nibble[i_count] = 0;
   debug(v_reg_fprint(stdout, h_register); fprintf(stdout, "\n"));
   return (h_register);
}

/* Load a register */
void v_reg_load(oregister *h_register, ...) {
   int i_count, i_temp;
   va_list t_args;
   va_start(t_args, h_register);
   i_temp = sizeof(h_register->nibble) / sizeof(*h_register->nibble) - 1;
   for (i_count = i_temp; i_count >= 0; i_count--) 
      h_register->nibble[i_count]  = va_arg(t_args, int);
}

/* Create a new processor object */
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
      h_processor->flags[i_count] = False; /* Clear the processor flags */
   h_processor->flags[MODE] = 1; /* Select RUN mode */
   h_processor->pc = 0; /* Program counter */
   h_processor->sp = 0; /* Stack pointer */
   h_processor->p = 0;  /* Pointer register*/
   h_processor->f = 0;  /* F register*/
   h_processor->data = 0; /* Data register */
   h_processor->base = 10; /* Data register */
   
   h_processor->bank = 0; /* ROM bank number */
   h_processor->rom = h_rom ; /* Address of ROM */ 
   return (h_processor);
}

/* Clear the contents of a register */
static void v_reg_zero(oprocessor *h_processor, oregister *h_register) {
   int i_count;
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++)
      h_register->nibble[i_count] = 0;
}

/* Exchange the contents of two registers */
static void v_reg_exch (oprocessor *h_processor, oregister *h_destination, oregister *h_source) {
   int i_count, i_temp;
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++) {
      i_temp = h_destination->nibble[i_count];
      h_destination->nibble[i_count] = h_source->nibble[i_count];
      h_source->nibble[i_count] = i_temp;
   }
}

/* Copy the contents of a register */
static void v_reg_copy (oprocessor *h_processor, oregister *h_destination, oregister *h_source) {
   int i_count;
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++) {
      h_destination->nibble[i_count] = h_source->nibble[i_count];
   }
}

/* Add the contents of two registers */
static void v_reg_add (oprocessor *h_processor, oregister *h_destination, oregister *h_source){
   int i_count, i_temp;
   for (i_count = h_processor->first; i_count <= h_processor->last; i_count++){
      if (h_source != NULL) i_temp = h_source->nibble[i_count]; else i_temp = 0;
      i_temp = h_destination->nibble[i_count] + i_temp + h_processor->flags[CARRY];
      if (i_temp >= h_processor->base){
         i_temp -= h_processor->base;
         h_processor->flags[CARRY] = 1;
      } else {
         h_processor->flags[CARRY] = 0;
      }
      h_destination->nibble[i_count] = i_temp;
   }
}

/* Increment the contents of a register */
static void v_reg_inc (oprocessor *h_processor, oregister *h_register){
   h_processor->flags[CARRY] = 1; /* Set carry */
   v_reg_add (h_processor, h_register, NULL); /* Add carry to register */
}

/* Set the specified status bit  */
static void op_set_s (oprocessor *h_processor, int i_count) {
   if (i_count < sizeof(h_processor->status) / sizeof(*h_processor->status))
      h_processor->status[i_count] = 1;
   else
      v_error("Unexpected error in  %s line : %d\n", __FILE__, __LINE__);
}

/* Clear the specified status bit */
static void op_clr_s (oprocessor *h_processor, int i_count) {
   if (i_count < sizeof(h_processor->status) / sizeof(*h_processor->status))
      h_processor->status[i_count] = 1;
   else
      v_error("Unexpected error in  %s line : %d\n", __FILE__, __LINE__);
}

/* Test the specified status bit */
static void op_test_s (oprocessor *h_processor, int i_count) {
   if ((i_count) < sizeof(h_processor->status) / sizeof(*h_processor->status)) {
      h_processor->flags[CARRY] = h_processor->status[i_count];
      h_processor->status[i_count] = 0; /* Testing the status clears it */
   }
   else
      v_error("Unexpected error in  %s line : %d\n", __FILE__, __LINE__);
}

static void op_reset_s (oprocessor *h_processor) {
   int i_count;
   for (i_count = 0; i_count < sizeof(h_processor->status) / sizeof(*h_processor->status); i_count++)
      if ((i_count != 1) && (i_count != 2) && 
         (i_count != 5) && (i_count != 15)) h_processor->status[i_count] = 0;
}

/* Note - The bit to be tested is derived from the value of the argument 
   int p_set_map [16] = { 14,  4,  7,  8, 11,  2, 10, 12,  1,  3, 13,  6,  0,  9,  5, 14 };
   int p_set_map [16] = { 14,  4,  7,  8, 11,  2, 10, 12,  1,  3, 13,  6,  0,  9,  5, 14 };
   int p_test_map [16] = {  4,  8, 12,  2,  9,  1,  6,  3,  1, 13,  5,  0, 11, 10,  7,  4 };
*/

/* Jump to subroutine */
void op_jsb(oprocessor *h_processor, int i_count){
   h_processor->stack[h_processor->sp] = h_processor->pc; /* Push program counter on the stack */
   h_processor->sp = (h_processor->sp + 1) & (STACK_SIZE - 1); /* Update stack pointer */
   h_processor->pc = i_count - 1; /* Program counter will be auto incremented before next fetch */
   /* TO DO handle_del_rom(); */
}

/* Return from subroutine */
void op_rtn(oprocessor *h_processor) {
   h_processor->sp = (h_processor->sp - 1) & (STACK_SIZE - 1); /* Update stack pointer */
   h_processor->pc = h_processor->stack[h_processor->sp]; /* Pop program counter on the stack */
}

/* Decode and execute a single instruction */
int i_processor_tick(oprocessor *h_processor) {

   unsigned int i_opcode; 
   unsigned int i_counter; /* New program counter */
   unsigned int i_field; /* Field modifier */
   const char *s_field; /* Holds pointer to field name */
   
   i_opcode = h_processor->rom[h_processor->pc];

   if (h_processor->flags[TRACE])
      fprintf(stdout, "%1o-%04o\t %04o\t ", h_processor->bank, h_processor->pc, h_processor->rom[h_processor->pc]);

   switch (i_opcode & 03) {
   case 00: /* Special operations */
      switch ((i_opcode >> 2) & 03) {
      case 00: /* Group 0 */
         switch ((i_opcode >> 4) & 03) {
         case 00: /* nop */
            if (h_processor->flags[TRACE]) fprintf(stdout, "nop");
            break;
         case 01:
            switch (i_opcode){
            case 01020: /* return */
               op_rtn (h_processor);
               if (h_processor->flags[TRACE]) fprintf(stdout, "return");
               break;
            default:
               if (h_processor->flags[TRACE]) fprintf(stdout, "special group 01");
            }
            break;
         case 02: /* select rom */
            h_processor->pc = (i_opcode >> 6) * 256 + (h_processor->pc % 256);
            if (h_processor->flags[TRACE]) fprintf(stdout, "select rom %02d", i_opcode >> 6);
            break;
         case 03:
            switch (i_opcode) {
            case 01760: /* hi I'm woodstock */
               if (h_processor->flags[TRACE]) fprintf(stdout, "hi I'm woodstock");
               break;
            default: 
               if (h_processor->flags[TRACE]) fprintf(stdout, "special group 03");
            }
            break;
         default: 
            v_error("Unexpected error in  %s line : %d\n", __FILE__, __LINE__);
         }
         break;
      case 01: /* Group 1 */ 
         switch ((i_opcode >> 4) & 03) {
         case 00: /* 1 -> s(n) */
            op_set_s (h_processor, i_opcode >> 6);
            if (h_processor->flags[TRACE]) fprintf(stdout, "1 -> s(%d)", i_opcode >> 6);
            break;
         case 01: /* if 1 = s(n) */
            op_test_s (h_processor, i_opcode >> 6);
            if (h_processor->flags[TRACE]) fprintf(stdout, "if 1 = s(%d)", i_opcode >> 6);
            break;
         case 02: /* if p = n */
            if (h_processor->flags[TRACE]) fprintf(stdout, "** if p = %d", i_opcode >> 6);
            break;
         case 03: /* delayed rom n */
            if (h_processor->flags[TRACE]) fprintf(stdout, "** delayed rom %d", i_opcode >> 6);
            break;
         default: 
            v_error("Unexpected error in  %s line : %d\n", __FILE__, __LINE__);
         }
         break;
      case 02: /* Group 2 */ 
         switch (i_opcode) {
         case 00110: /* clear s */
            op_reset_s (h_processor);
            if (h_processor->flags[TRACE]) fprintf(stdout, "clear s");
            break;
         case 00210: /* display toggle */
            if (h_processor->flags[DISPLAY_ENABLE] == 0)
               h_processor->flags[DISPLAY_ENABLE] = 1;
            else
               h_processor->flags[DISPLAY_ENABLE] = 0;
            if (h_processor->flags[TRACE]) {
               fprintf(stdout, "display toggle\t");
               v_flags_fprint(stdout, h_processor);
            }
            break;
         case 00310: /* display off */
            h_processor->flags[DISPLAY_ENABLE] = 0;
            if (h_processor->flags[TRACE]) {
               fprintf(stdout, "display off\t");
               v_flags_fprint(stdout, h_processor);
            }
            break;
         case 00410: /* m exch c */ 
            h_processor->first = 0; h_processor->last = REG_SIZE - 1;
            v_reg_exch(h_processor, h_processor->reg[M_REG], h_processor->reg[C_REG]);               
            if (h_processor->flags[TRACE]) {
               fprintf(stdout, "m exch c \t");
               v_reg_fprint(stdout, h_processor->reg[M_REG]);
               v_reg_fprint(stdout, h_processor->reg[C_REG]);
               v_flags_fprint(stdout, h_processor);
            }
            break;
         case 01710: /* f exch a */
            {
               int i_temp;
               i_temp = h_processor->reg[A_REG]->nibble[0];
               h_processor->reg[A_REG]->nibble[0] = h_processor->f;
               h_processor->f = i_temp;
            }
            if (h_processor->flags[TRACE]) {
               fprintf(stdout, "f exch a \t");
               v_reg_fprint(stdout, h_processor->reg[A_REG]);
               v_flags_fprint(stdout, h_processor);
            }
            break;
         default: 
            if (h_processor->flags[TRACE]) fprintf(stdout, "special group 2");
         }
         break;
      case 03: /* Group 3 */ 
         switch ((i_opcode >> 4) & 03) {
         case 00: /* 0 -> s(n) */
            op_clr_s (h_processor, i_opcode >> 6);
            if (h_processor->flags[TRACE]) fprintf(stdout, "0 -> s(%d)", i_opcode >> 6);
            break;
         case 01: /* if 0 = s(n) */
            if (h_processor->flags[TRACE]) fprintf(stdout, "TODO if 1 = s(%d)", i_opcode >> 6);
            break;
         case 02: /* if p # n */
            if (h_processor->flags[TRACE]) fprintf(stdout, "** if p # n");
            break;
         case 03: /* p = n */
            if (h_processor->flags[TRACE]) fprintf(stdout, "TODO p = n");
            break;
         default: 
            v_error("Unexpected error in  %s line : %d\n", __FILE__, __LINE__);
         }
         break;
      }
      if (h_processor->flags[TRACE]) fprintf(stdout, "\n");
      break;

   case 01: /* Jump subroutine */
      i_counter = i_opcode >> 2; /* Get target address */
      op_jsb (h_processor, i_counter);
      if (h_processor->flags[TRACE]) fprintf(stdout, "jsb\t %01o-%04o\n", h_processor->bank, i_counter);
      break;

   case 02: /* Arithmetic operations */
      i_field = (i_opcode >> 2) & 7;
     
      switch (i_field) {
      case 0:  /* 000   P  : determined by P register             ([P]) */
         h_processor->first = h_processor->p; h_processor->last = h_processor->p;
         s_field = "p";
         if (h_processor->p >= REG_SIZE) {
            fprintf(stderr, "Run-time error\t: %s line : %d : %s", \
            __FILE__, __LINE__, "Invalid register pointer\n");
            h_processor->last = 0;
         }
         break;
      case 1:  /* 100  WP  : word up to and including P register  ([0 .. P])  */
         h_processor->first =  0; h_processor->last =  h_processor->p; /* break; bug in orig??? */
         s_field = "wp";
         s_field = "wp";
         if (h_processor->p >= REG_SIZE) {
            fprintf(stderr, "Run-time error\t: %s line : %d : %s", \
            __FILE__, __LINE__, "Invalid register pointer\n");
            h_processor->last = REG_SIZE - 1;
         }
         break;
      case 2:  /* 110  XS  : exponent and sign                    ([0 .. 2])  */
         h_processor->first = EXP_SIZE - 1; h_processor->last = EXP_SIZE - 1;
         s_field = "xs";
         break;
      case 3:  /* 010   X  : exponent                             ([0 .. 1])  */
         h_processor->first = 0; h_processor->last = EXP_SIZE - 1;
         s_field = "x";
         break;
      case 4:  /* 111   S  : sign                                 ([13])      */  
         h_processor->first = REG_SIZE - 1; h_processor->last = REG_SIZE - 1;
         s_field = "s";
         break;
      case 5:  /* 001   M  : mantissa                             ([3 .. 12]) */
         h_processor->first = EXP_SIZE; h_processor->last = REG_SIZE - 2;
         s_field = "m";
         break;
      case 6:  /* 011   W  : word                                 ([0 .. 13]) */
         h_processor->first = 0; h_processor->last = REG_SIZE - 1;
         s_field = "w";
         break;
      case 7:  /* 101  MS  : mantissa and sign                    ([3 .. 13]) */
         h_processor->first = EXP_SIZE; h_processor->last = REG_SIZE - 1;
         s_field = "ms";
         break;
      }         
             
      /* TO DO - Clear CARRY?  */
      
      switch (i_opcode >> 5)
      {
      case 0000:  /* 0 -> a[f] */
         v_reg_zero(h_processor, h_processor->reg[A_REG]);
         if (h_processor->flags[TRACE]) {
            fprintf(stdout, "0 -> a[%s]\t", s_field);
            v_reg_fprint(stdout, h_processor->reg[A_REG]);                  
            v_flags_fprint(stdout, h_processor);
         }
         break;
      case 0001:  /* 0 -> b[f] */
         v_reg_zero(h_processor, h_processor->reg[B_REG]);
         if (h_processor->flags[TRACE]) {
            fprintf(stdout, "0 -> b[%s]\t", s_field);
            v_reg_fprint(stdout, h_processor->reg[B_REG]);                  
            v_flags_fprint(stdout, h_processor);
         }
         break;
      case 0002:  /* a exch b[f] */
         v_reg_exch(h_processor, h_processor->reg[A_REG], h_processor->reg[B_REG]);
         if (h_processor->flags[TRACE]) {
            fprintf(stdout, "a exch b[%s]\t", s_field);
            v_reg_fprint(stdout, h_processor->reg[A_REG]);                  
            v_reg_fprint(stdout, h_processor->reg[B_REG]);                  
            v_flags_fprint(stdout, h_processor);
         }
         break;            
      /* case 0003:  /* a -> b[f] */
      /*    src = act_a; dest = act_b; reg_copy(); break; */
      case 0004:  /* a exch c[f] */
         v_reg_exch(h_processor, h_processor->reg[A_REG], h_processor->reg[C_REG]);
         if (h_processor->flags[TRACE]) {
            fprintf(stdout, "a exch c[%s]\t", s_field);
            v_reg_fprint(stdout, h_processor->reg[A_REG]);                  
            v_reg_fprint(stdout, h_processor->reg[C_REG]);                  
            v_flags_fprint(stdout, h_processor);
         }
         break;            
      /* case 0005:  /* c -> a[f] */
      /*    src = act_c; dest = act_a; reg_copy(); break; */
      /* case 0006:  /* b -> c[f] */
      /*    src = act_b; dest = act_c; reg_copy(); break; */
      case 0007:  /* b exchange c[f] */
         v_reg_exch(h_processor, h_processor->reg[B_REG], h_processor->reg[C_REG]);
         if (h_processor->flags[TRACE]) {
            fprintf(stdout, "b exch c[%s]\t", s_field);
            v_reg_fprint(stdout, h_processor->reg[B_REG]);                  
            v_reg_fprint(stdout, h_processor->reg[C_REG]);                  
            v_flags_fprint(stdout, h_processor);
         }
         break;            
      case 0010:  /* 0 -> c[f] */
         v_reg_zero(h_processor, h_processor->reg[C_REG]);
         if (h_processor->flags[TRACE]) {
            fprintf(stdout, "0 -> c[%s]\t", s_field);
            v_reg_fprint(stdout, h_processor->reg[C_REG]);                  
            v_flags_fprint(stdout, h_processor);
         }
         break;
      /* case 0011:  /* a + b -> a[f] */
      /*    dest = act_a; src = act_b; reg_add(); break; */
      /* case 0012:  /* a + c -> a[f] */
      /*    dest = act_a; src = act_c; reg_add(); break; */
      /* case 0013:  /* c + c -> c[f] */
      /*    dest = act_c; src = act_c; reg_add(); break; */
      /* case 0014:  /* a + c -> c[f] */
      /*    dest = act_c; src = act_a; reg_add(); break; */
      case 0015:  /* a + 1 -> a[f] */
         v_reg_inc (h_processor, h_processor->reg[A_REG]);
         if (h_processor->flags[TRACE]) {
            fprintf(stdout, "a + 1 -> a[%s]\t", s_field);
            v_reg_fprint(stdout, h_processor->reg[A_REG]);                  
            v_flags_fprint(stdout, h_processor);
         }
         break;
      /* case 0016:  /* shift left a[f] */
      /*    dest = act_a; reg_shift_left(); break; */
      /* case 0017:  /* c + 1 -> c[f] */
      /*    dest = act_c; reg_inc(); break; */
      /* case 0020:  /* a - b -> a[f] */
      /*    dest = act_a; src = act_a; src2 = act_b; reg_sub(); break; */
      /* case 0021:  /* a - c -> c[f] */
      /*    dest = act_c; src = act_a; src2 = act_c; reg_sub(); break; */
      /* case 0022:  /* a - 1 -> a[f] */
      /*    act_flags |= F.CARRY; dest = act_a; src = act_a; src2 = null; reg_sub(); break; */
      /* case 0023:  /* c - 1 -> c[f] */
      /*    act_flags |= F.CARRY; dest = act_c; src = act_c; src2 = null; reg_sub(); break; */
      /* case 0024:  /* 0 - c -> c[f] */
      /*    dest = act_c; src = null; src2 = act_c; reg_sub(); break; */
      /* case 0025:  /* 0 - c - 1 -> c[f] */
      /*    act_flags |= F.CARRY; dest = act_c; src = null; src2 = act_c; reg_sub(); break; */
      /* case 0026:  /* if b[f] = 0 */
      /*    act_inst_state = ST.branch; src = act_b;  dest=null; reg_test_nonequal(); break; */
      /* case 0027:  /* if c[f] = 0 */
      /*    act_inst_state = ST.branch; src = act_c;  dest=null; reg_test_nonequal(); break; */
      /* case 0030:  /* if a >= c[f] */
      /*    act_inst_state = ST.branch; dest = null; src = act_a; src2 = act_c; reg_sub(); break; */
      /* case 0031:  /* if a >= b[f] */
      /*    act_inst_state = ST.branch; dest = null; src = act_a; src2 = act_b; reg_sub(); break; */
      /* case 0032:  /* if a[f] # 0 */
      /*    act_inst_state = ST.branch; src = act_a; dest=null; reg_test_equal(); break; */
      /* case 0033:  /* if c[f] # 0 */
      /*    act_inst_state = ST.branch; src = act_c;  dest=null; reg_test_equal(); break; */
      /* case 0034:  /* a - c -> a[f] */
      /*    dest = act_a; src = act_a; src2 = act_c; reg_sub(); break; */
      /* case 0035:  /* shift right a[f] */
      /*    dest = act_a; reg_shift_right(); break; */
      /* case 0036:  /* shift right b[f] */
      /*    dest = act_b; reg_shift_right(); break; */
      /* case 0037:  /* shift right c[f] */
      /*    dest = act_c; reg_shift_right(); break; */
      default:
         if (h_processor->flags[TRACE]) fprintf(stdout, "arithmetic");
         break;
      }
      if (h_processor->flags[TRACE]) fprintf(stdout, "\n");
      break;

   case 03:/* Subroutine calls and long conditional jumps */
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
   }

   if (h_processor->pc++ >= (ROM_SIZE - 1)) h_processor->pc = 0; /* Increment program counter */   
   return(0); /* TO DO: Return status */
}
