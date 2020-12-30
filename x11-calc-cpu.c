/*
 * x11-calc-display.c - RPN (Reverse Polish) calculator simulator. 
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

#define VERSION        "0.1"
#define BUILD          "0001"
#define DATE           "10 Sep 20"

#define DEBUG 1        /* Enable/disable debug*/
 
#include <stdio.h>     /* fprintf(), etc. */
#include <stdarg.h>
 
#include "gcc-debug.h"
 
unsigned char gc_a_reg[14];
unsigned char gc_b_reg[14];
unsigned char gc_c_reg[14];
unsigned char gc_d_reg[14];
unsigned char gc_e_reg[14];
unsigned char gc_f_reg[14];
unsigned char gc_m1_reg[14];
unsigned char gc_s_reg[16]; /* Processor status word */
  
/*
 * S0                Not used.
 * S1    SCI         Scientific notation (clear for fixed point notation).
 * S2    Auto_Enter  Entering digit will push X.
 * S3    RAD         Radians (clear for degrees).
 * S4    DC OK       Clear for lower power
 * S5    DECIMAL     Set if decimal point has already been entered.
 * S6    ?
 * S7    ?
 * S8    ?
 * S9    ?
 * S10   ?
 * S11   ?
 * S12   ?
 * S13   FUNCTION    Set if function key has been pressed.
 * S14   ?
 * S15   KEY         Set if key is down.
 */

int i_cpu_init(){
   int i_count;
   size_t t_upb;
 
   /* Clear registers. */
   t_upb = sizeof(gc_a_reg)/sizeof(gc_a_reg[0]);
   for (i_count = 0; i_count < t_upb; i_count++) {
      gc_a_reg[i_count]= 0; gc_b_reg[i_count]= 0; gc_c_reg[i_count]= 0; 
      gc_d_reg[i_count]= 0; gc_e_reg[i_count]= 0; gc_f_reg[i_count]= 0;
      gc_m1_reg[i_count]=0; 
   }
   /* Clear processor status word. */
   t_upb = sizeof(gc_s_reg)/sizeof(gc_s_reg[0]);
   for (i_count = 0; i_count < t_upb; i_count++)
      gc_s_reg[i_count]= 0;
}
 
int i_reg_load(unsigned char *c_reg, ...) {
   unsigned char c_temp;
   int i_count;
   size_t t_upb;
   va_list t_args;

   va_start(t_args, c_reg);
   t_upb = (sizeof(gc_a_reg)/sizeof(gc_a_reg[0])) - 1 ;
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
