/*
 * x11-calc-colour.c - RPN (Reverse Polish) calculator simulator. 
 *
 * Copyright(C) 2013   MT
 * 
 * Colour manipulation functions.
 *           
 * Implements  the routines needed to manipulate colour values to  generate
 * different colour tints used for shading etc.
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
 * 02 Jul 13         - Initial version - MT
 * 15 Dec 18         - Changed debug macro and added an error macro - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0001"
#define DATE           "02 Jul 13"
 
#define DEBUG 0        /* Enable/disable debug*/
 
#include "gcc-debug.h"
 
/*
 * shade(colour)
 *
 * Returns a lighter shade of the specified colour.
 *
 * 13 Jun 13         - Initial version - MT
 *
 */

unsigned int i_shade(unsigned int i_colour){

   unsigned int i_red, i_green, i_blue;
   
   /* Find an approprite darker shade of blue. */
   i_blue = ((i_colour & 0xff) /3) * 2;

   /* Find an approprite darker shade of green. */
   i_colour = i_colour >> 8;
   i_green = ((i_colour & 0xff) / 3) * 2;

   /* Find an approprite darker shade of red. */
   i_colour = i_colour >> 8;
   i_red = ((i_colour & 0xff) / 3) * 2;

   return ((((i_red << 8) | i_green) << 8) | i_blue);
}


/*
 * tint(colour)
 *
 * Returns a darker shade of the specified colour.
 *
 * 13 Jun 13         - Initial version - MT
 *
 */

unsigned int i_tint(unsigned int i_colour){

   unsigned int i_red, i_green, i_blue, i_gray;
   
   /* Find an approprite lighter shade of blue. */
   i_blue = i_colour & 0xff;
   if (i_blue > 127) i_blue = (i_blue >> 1) | i_blue; else i_blue = i_blue << 1;
      
   /* Find an approprite lighter shade of green. */
   i_colour = i_colour >> 8;
   i_green = i_colour & 0xff;
   if (i_green > 127) i_green = (i_green >> 1) | i_green; else i_green = i_green << 1;
   
   /* Find an approprite lighter shade of red. */
   i_colour = i_colour >> 8;
   i_red = i_colour & 0xff;
   if (i_red > 127) i_red = (i_red >> 1) | i_red; else i_red = i_red << 1;
   
   i_gray = (i_red + i_green + i_blue)/3;
   if (i_gray > 127) i_gray = 0xff; else i_gray = 0x7f;

   /* Combine the red, blue and green values to give a lighter tint of the 
      original colour. */
   return((((i_red << 8) | i_green) << 8) | i_blue);
}



