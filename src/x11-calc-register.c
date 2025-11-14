/*
 * x11-calc-register.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2025   MT
 *
 * Contains the functions to implement any register methods.
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
 * 14 Nov 25  0.1.0001  - Initial version - MT
 *
 * To Do                -
 *
 */

#define NAME           "x11-calc-register"
#define BUILD          "0001"
#define DATE           "14 Nov 2025"
#define AUTHOR         "MT"

#include <errno.h>     /* errno */

#include <string.h>    /* strlen(), memcpy() etc */
#include <stdio.h>     /* fprintf(), etc */
#include <stdlib.h>    /* malloc(), etc */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc */
#include <X11/Xutil.h> /* XSizeHints etc */

#include "x11-calc-messages.h"
#include "x11-calc-errors.h"

#include "x11-calc-register.h"

#include "gcc-debug.h"

oregister *h_register_create(int i_id)  /* Create a new register */
{
   oregister *h_register;  /* Pointer to register */
   int i_count, i_temp;

   if ((h_register = malloc (sizeof(*h_register))) == NULL)
   {
      fprintf(stdout, h_err_register_alloc, __FILE__, __LINE__);
   }
   i_temp = sizeof(h_register->nibble) / sizeof(*h_register->nibble);
   h_register->id = i_id;
   for (i_count = 0; i_count < i_temp; i_count++)
      h_register->nibble[i_count] = 0;
   return(h_register);
}

void v_fprint_register(FILE *h_file, oregister *h_register)  /* Print the contents of a register */
{
   const char c_name[8] = {'A', 'B', 'C', 'Y', 'Z', 'T', 'M', 'N'};
   int i_count;
   if (h_register != NULL)
   {
      fprintf(h_file, "\treg[");
      if (h_register->id < 0)
         fprintf(h_file, "\'%c\'", c_name[h_register->id * -1 - 1]);
      else
         fprintf(h_file, "%03d", h_register->id);
      fprintf(h_file, "] = 0x");
      for (i_count = REG_SIZE - 1; i_count >=0 ; i_count--)
         fprintf(h_file, "%1x", h_register->nibble[i_count]);
   }
}
