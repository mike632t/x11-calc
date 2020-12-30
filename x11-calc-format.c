/*
 * x11-calc-format.c - Formatting for floating point numbers.
 *
 * Copyright(C) 2018   MT
 *
 * Formats  floating point numbers according to the rules for FIX, SCI  and
 * ENG number formats. 
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
 * 20 Jul 13   0.1   - Initial version - MT
 * 22 Jul 13         - Fixed the truncation errors that were occurring when
 *                     formatting  some  fractions by rounding  the  number
 *                     before truncating it - MT
 *                   - Defined trunc() and round() locally as they are  not
 *                     necessairaly available on all platforms - MT
 *                   - Defined  display  width as a  constant  and  updated
 *                     formatting routines to allow the display width to be
 *                     adjusted at compile time - MT
 * 24 Jul 13         - Modified  code to use sprinf() instead of  asprinf()
 *                     to allow routines to be compiled using VAX C - MT
 * 25 Jul 13         - Added mant() to return the mantissa - MT
 * 26 Jul 13         - Modified  eng() and sci() to include spaces  between
 *                     the value and the exponent - MT
 *                   - Added sign() function -MT
 *                   - Modified eng and sci to remove string constants used
 *                     to store the sign character - MT
 * 27 Jul 13         - Fixed formatting errors in sci() and eng() - MT
 * 02 Aug 13         - Checks for overflow in sci() and eng() - MT
 * 17 Aug 13         - Combined sci() and eng() in format() - MT
 * 18 Aug 13         - Finally got format() working, and fixed problems
 *                     with the mantissa when rounding up - MT
 *                   - Removed sci() and eng() - MT
 * 18 Aug 13         - Fixed problem with mant() caused by rounding up when
 *                     truncating the number of digits - MT
 * 21 Aug 13   0.2   - Put test cases into an array - MT
 *                   - Added integer min() and max() macro definitions - MT
 *                   - Now checks for numeric underflow - MT
 *                   - Tidied up warning messages (errors now display value
 *                     correctly) - MT
 * 16 Dec 18         - Replaced local math functions with macro definitions
 *                     (so they can be used with floating point and integer
 *                     values) - MT
 * 20 Dec 18         - Fixed FIX formatting - MT
 * 08 Jan 19         - Added SWAP macro to exchange values - MT
 * 30 Aug 20         - Moved macro definitions to a seperate file - MT
 *                     
 */
 
#define DEBUG 1
 
#include <math.h>      /* log10(), etc. */
#include <string.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* malloc(), etc. */
 
#include "gcc-math.h"
#include "gcc-debug.h"
 
#include "x11-calc-21.h"
 
char s_string[DIGITS + 3]; /* Allowing for the sign, decimal point and terminator. */
 
char* s_format(double d_value, int i_precision, int i_mode) {
 
   double d_number = d_value;
   int i_sign, i_exponent, i_decimals, i_digits;
   char c_sign = ' ';
   
   i_exponent = 0; i_digits = 1; i_decimals = i_precision;
   i_sign = SIGN(d_number);

   if (ABS(i_sign) > 0) { 

      d_number *= i_sign; /* Make number positive before formatting it and restore the sign at the end! */
      i_exponent = (int) ROUND(floor(log10(d_number))); /* Find exponent. */
                  
      debug(
         if ((ROUND(d_number) > d_number) && i_precision < 1 && i_mode == 0) 
         fprintf(stderr, "Debug\t: %s line : %d : Mantessa = %e, Rounded = %e, Decimals = %d, Exponent = %d.\n", 
         __FILE__, __LINE__, d_number, ROUND(d_number), i_decimals, i_exponent)
      );      

      if (i_mode > 0 || (ABS(i_exponent) > (DIGITS)) || ((i_decimals + i_exponent) < 0)) { /* Scientific (or Engineering) mode. */

         d_number /= pow(10.0, i_exponent); /*  Find mantissa. */
         d_number = ROUND(d_number * pow(10.0, i_decimals)) / pow(10.0, i_decimals); /* Round up the the desired number of decimal places. */

         while (d_number >= 10.0) {d_number /= 10.0; i_exponent++;} /* Fix up value if necessary. */

         if (i_exponent + (int) ROUND(floor(log10(d_number))) < -99) { /* Check for numeric underflow. */
            fprintf(stderr, "Warning\t: %s line: %d - Underflow\t(%- fe%+-2d).\n",
              __FILE__, __LINE__, d_number * i_sign, i_exponent);
            d_number = 0.0; i_exponent = 0; i_sign = 0;
         }

         if (i_mode > 1) { /* Adjust exponent for Engineering mode. */
            i_digits += i_decimals;
            i_decimals -= i_exponent;
            if ((i_exponent / 3) * 3 - i_exponent != 0) {/* Is the exponent already a multiple of 3. */
               if(i_exponent >= 0) i_exponent = (i_exponent / 3) * 3; else i_exponent = ((-i_exponent + 3) / 3) * (-3); /* Round the exponent up/down as needed. */
            }
            i_decimals += i_exponent; /* Work out the change in the number of decimal places */
            i_digits -= i_decimals;
            if (i_decimals < 0) i_decimals = 0;
            d_number = ROUND(d_number * pow(10.0, i_digits - 1 + i_decimals))/ pow(10.0, i_decimals);
         }

         if (i_exponent + (int) ROUND(floor(log10(d_number))) > 99) { /* Check for numeric overflow. */
            while (d_number >= 10.0) {d_number /= 10.0; i_exponent++;} /* Fix up value if necessary. */
            fprintf(stderr, "Warning\t: %s line: %d - Overflow \t(%- fe%+-2d).\n",
              __FILE__, __LINE__, d_number * i_sign, i_exponent);
            d_number = 9.999999999999999; i_exponent = 99;
            i_decimals = DIGITS - 1; i_digits = 1; /* Force all decimal places to be shown. */
         }
      }
      else {
         if ((i_decimals + i_exponent) >= DIGITS) i_decimals = DIGITS - i_exponent -1; /* Round up the the desired number of decimal places. */
         d_number = ROUND(d_number * pow(10.0, i_decimals)) / pow(10.0, i_decimals); 
         i_exponent = (int) ROUND(floor(log10(d_number))); /* Find exponent again - it may have changed when the number was rounded up. */
         if (i_exponent > 0) i_digits += i_exponent;
      }
      d_number *= i_sign; /* Fix up the sign. */
   }
   
   if (i_mode > 0 || (ABS(i_exponent) > (DIGITS)) || ((i_decimals + i_exponent) < 0)) { /* SCI or ENG format */
      if (i_digits + i_decimals > DIGITS - 3){ /* Truncate mantissa if necessary. */
         d_number = TRUNC(d_number * pow(10.0, DIGITS - 3 - i_digits)) / pow(10.0, DIGITS - 3 - i_digits); /* Truncate. */
         i_decimals = DIGITS - 3 - i_digits; /* Adjust number of decimal places. */
      }
      if (i_exponent < 0) {c_sign = '-'; i_exponent = -i_exponent;} /* Is exponent negative? */
      sprintf(s_string, "% #*.*f%*c%02d%c", i_digits + i_decimals + 2,  i_decimals, d_number, DIGITS - 2 - i_digits - i_decimals, c_sign, i_exponent,' '); /* Allowing for the sign and decimal point. */
      return s_string;
   }
   else { /* FIX format */
      if (i_digits + i_decimals > DIGITS) i_decimals = DIGITS - i_digits; /* Truncate mantessa if necessary. */
      sprintf(s_string, "% #*.*f%*c", i_digits + i_decimals + 2,  i_decimals, d_number, DIGITS - (i_digits + i_decimals) + 1, ' '); /* Allowing for the sign and decimal point. */
      return s_string;
   }
}

char* s_mant(const double d_value) {

   double d_number = d_value;
   int i_exponent = 0;
   switch (SIGN(d_number)) {
   case -1:
      d_number = -d_number; /* Change sign of number and fall through. */
   case 1:
      i_exponent = (int) ROUND(floor(log10(d_number))); /* Find exponent. */
      d_number /= pow(10.0, i_exponent); /*  Find mantessa. */

      /* Round up the the desired number of decimal places. */
      d_number = ROUND(d_number * pow(10.0, DIGITS - 1)) / pow(10.0, DIGITS - 1); 
      if (d_number >= 10.0) {d_number /= 10.0; i_exponent++;}
      d_number = ROUND(d_number * pow(10.0, DIGITS - 1));
   }
   if ((i_exponent < -99)) d_number = 0.;  /* Check for numeric underflow. */
   if ((i_exponent >= 99) && (d_number >= 10)) d_number = 9999999999.; /* Check for numeric overflow. */
   sprintf(s_string, " %0*.0f", DIGITS, d_number);
   return s_string;
}  
