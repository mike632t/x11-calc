/*
 * x11-calc-digit.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * 7 segment display digit functions.
 *
 * Contains  the  functions needed to create and display  a  seven  segment
 * digit element based on the mask value.
 *
 *    "1" = 0x30     " " = 0x00
 *    "2" = 0x6d     "-" = 0x40
 *    "3" = 0x79     "." = 0x80
 *    "4" = 0x72     "," = 0x100
 *    "5" = 0x5b     ":" = 0x200
 *    "6" = 0x5f
 *    "7" = 0x31     "E" = 0x4f
 *    "8" = 0x7f     "P" = 0x67
 *    "9" = 0x73     "r" = 0x44
 *    "0" = 0x3f     "o" = 0x5c
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
 * 14 Jul 13         - Initial version - MT
 * 16 Jul 13         - Draws a 7 segment display based on mask - MT
 * 18 Jul 13         - Added decimal point and comma (using shading) - MT
 *                   - Reversed  the order of the segments in the bit mask,
 *                     segment A is now bit 1, and segment G bit 7 - MT
 * 19 Jul 13         - Added  shading  to the display segments and  reduced
 *                     the  number  times the foreground colour is  set  by
 *                     drawing all the background elements for each segment
 *                     before filling in the foreground - MT
 * 15 Dec 18         - Changed debug macro and added an error macro - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 08 Feb 22         - Include header for labels - MT
 * 14 Oct 23         - Don't draw a separate background behind digits on an
 *                     ARM  based system (unless it is an Apple) as the new
 *                     Wayand compositor's performance on a Raspberry Pi is
 *                     far too slow even on a Pi 4 - MT
 *                   - Changed the way the code tests to see if it is being
 *                     built on an ARM processor to make it compatible with
 *                     both tcc and clang - MT
 * 25 Feb 24         - Modified the display to show the background for each
 *                     digit for all processor types - MT
 *                   - Does not attempt draw a separate background for each
 *                     digit on alpha based systems processors - MT
 * 03 Mar 24         - Updated error handling (now passes the  error number
 *                     to the error handler) - MT
 * 09 Apr 24         - Finally renamed x11-calc-segment to the more correct
 *                     x11-calc-digit - MT
 * 23 Apr 24         - Separated out prototypes for error handlers - MT
 *
 * TO DO :           - Optimise drawing of display digits.
 ^
 */

#define NAME           "x11-calc-digit"
#define BUILD          "0013"
#define DATE           "09 Apr 24"
#define AUTHOR         "MT"

#include <errno.h>     /* errno */

#include <stdio.h>     /* fprintf(), etc */
#include <stdlib.h>    /* malloc(), etc */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc */
#include <X11/Xutil.h> /* XSizeHints etc */

#include "x11-calc-messages.h"
#include "x11-calc-errors.h"

#include "x11-calc-switch.h"
#include "x11-calc-button.h"
#include "x11-calc-label.h"
#include "x11-calc-digit.h"

#include "x11-calc-colour.h"

#include "x11-calc.h"

#include "gcc-debug.h"

/*
 * digit_create (index, text, left, top, width, height, state,
 *                colour)
 *
 * Allocates the memory for a new digit, initializes the digit's properties
 * and returns a pointer to the digit.
 *
 */

struct odigit *h_digit_create(int i_index, int i_mask, int i_left, int i_top,
   int i_width, int i_height, unsigned int i_foreground, unsigned int i_background){

   struct odigit *h_digit; /* Ponter to digit */

   /* Attempt to allocate memory for a digit */
   if ((h_digit = malloc (sizeof(*h_digit)))==NULL) v_error(errno, "Memory allocation failed!");

   h_digit->index = i_index;
   h_digit->mask = i_mask;

   h_digit->digit_position.x = i_left;
   h_digit->digit_position.y = i_top;
   h_digit->digit_position.width = i_width;
   h_digit->digit_position.height = i_height;

   h_digit->foreground = i_foreground;
   h_digit->background = i_background;

   h_digit->digit_geometry = h_digit->digit_position; /* Save current size and position */

   return (h_digit);
}

/*
 * digit_resize (display, scale)
 *
 * Resize a digit.
 *
 */

int i_digit_resize(struct odigit *h_digit, float f_scale) /* Resize digit based on original geometry */
{
   h_digit->digit_position.x = h_digit->digit_geometry.x * f_scale;
   h_digit->digit_position.y = h_digit->digit_geometry.y * f_scale;
   h_digit->digit_position.width = h_digit->digit_geometry.width * f_scale;
   h_digit->digit_position.height = h_digit->digit_geometry.height * f_scale;

   return 0;
}

int i_digit_draw(Display *h_display, int x_application_window, int i_screen, struct odigit *h_digit){ /* Draws all the digits */

   int i_left, i_right, i_upper, i_lower;
   int i_offset;
#if defined(HP10) || defined(HP67) || defined(HP35) || defined(HP80) || defined(HP45) || defined(HP70) || defined(HP55)
   int i_middle;
#endif
   i_upper = h_digit->digit_position.height / 4;
   i_lower = h_digit->digit_position.y +  h_digit->digit_position.height - i_upper;
   i_upper = h_digit->digit_position.y + i_upper;
   i_offset = i_upper + (i_lower - i_upper) / 2;
   i_left = h_digit->digit_position.x + 2;
#if defined(HP10) || defined(HP67) || defined(HP35) || defined(HP80) || defined(HP45) || defined(HP70) || defined(HP55)
   i_right = h_digit->digit_position.x + h_digit->digit_position.width - 2;
   i_middle = i_left + ((i_right - i_left) / 2);
#else
   i_right = h_digit->digit_position.x + h_digit->digit_position.width - 7;
#endif

   debug(fprintf(stderr, "%4d,%d (%dx%d) %.1X%.1X%.1X%.1X %.1X%.1X%.1X%.1X.\n", \
      h_digit->digit_position.x, h_digit->digit_position.y, h_digit->digit_position.height, h_digit->digit_position.width, \
      h_digit->mask & SEG_DECIMAL && 1, h_digit->mask & SEG_G && 1, \
      h_digit->mask & SEG_F && 1, h_digit->mask & SEG_E && 1, \
      h_digit->mask & SEG_D && 1, h_digit->mask & SEG_C && 1, \
      h_digit->mask & SEG_B && 1, h_digit->mask & SEG_A && 1));

   /* Draw the display digit background */
   XSetForeground(h_display, DefaultGC(h_display, i_screen), h_digit->background);
   XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), h_digit->digit_position.x, h_digit->digit_position.y, h_digit->digit_position.width, h_digit->digit_position.height);
   XDrawRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), h_digit->digit_position.x, h_digit->digit_position.y, h_digit->digit_position.width, h_digit->digit_position.height);

#if !(defined(__aarch64__) || defined(__aarch__) || defined(__arm__) || defined(__arm64__) || defined(__alpha)) || defined(__APPLE__)
   /* Fill in the background for each active display digit */
   XSetForeground(h_display, DefaultGC(h_display, i_screen), i_shade(h_digit->foreground));

   if (h_digit->mask & SEG_A) { /* Draw the segment A */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left, i_upper, i_right, i_upper);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_upper - 1, i_right - i_left - 1, 3);
   }

   if (h_digit->mask & SEG_B) { /* Draw the segment B */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left , i_upper, i_left, i_offset);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left - 1, i_upper + 1, 3, i_offset - i_upper - 1);
   }

   if (h_digit->mask & SEG_C) { /* Draw the segment C */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left , i_offset, i_left, i_lower);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left - 1, i_offset + 1, 3, i_lower - i_offset - 1);
   }

   if (h_digit->mask & SEG_D) { /* Draw the segment D */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left, i_lower, i_right, i_lower);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_lower - 1, i_right - i_left - 1, 3);
   }

   if (h_digit->mask & SEG_E) { /* Draw the segment E */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right , i_lower, i_right, i_offset);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right - 1, i_offset + 1, 3, i_lower - i_offset - 1);
   }

   if (h_digit->mask & SEG_F) { /* Draw the segment F */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right , i_offset, i_right, i_upper);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right - 1, i_upper + 1, 3, i_offset - i_upper - 1);
   }

   if (h_digit->mask & SEG_G) { /* Draw the segment G */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left, i_offset, i_right, i_offset);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_offset - 1, i_right - i_left - 1, 3);
   }

#if defined(HP10) || defined(HP67) || defined(HP35) || defined(HP80) || defined(HP45) || defined(HP70) || defined(HP55)
   if (h_digit->mask & SEG_DECIMAL) { /* Draw a decimal point separator */
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_middle - 1 , (i_upper + 3 * (i_lower - i_upper) / 4) - 1, 3, 3);
   }
#else
   if (h_digit->mask & SEG_DECIMAL) { /* Draw a decimal point separator */
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower - 1, 3, 3);
   }

   if (h_digit->mask & SEG_COMMA) { /* Draw a comma separator */
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower - 1, 3, 3);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower + 2, i_right + 3, i_lower + 2);
   }

   if (h_digit->mask & SEG_COLON) { /* Draw a colon separator */
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_offset - 4, 3, 3);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_offset + 2, 3, 3);
   }
#endif

#endif

   /* Draw the in the foreground elements */
   XSetForeground(h_display, DefaultGC(h_display, i_screen), h_digit->foreground);
   if (h_digit->mask & SEG_A) { /* Draw the segment A */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_upper, i_right - 1, i_upper);
   }

   if (h_digit->mask & SEG_B) { /* Draw the segment B */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left , i_upper + 1, i_left, i_offset - 1);
   }

   if (h_digit->mask & SEG_C) { /* Draw the segment C */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left , i_offset + 1, i_left, i_lower - 1);
   }

   if (h_digit->mask & SEG_D) { /* Draw the segment D */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_lower, i_right - 1, i_lower);
   }

   if (h_digit->mask & SEG_E) { /* Draw the segment E */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right , i_lower - 1, i_right, i_offset + 1);
   }

   if (h_digit->mask & SEG_F) { /* Draw the segment F */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right , i_offset - 1, i_right, i_upper + 1);
   }

   if (h_digit->mask & SEG_G) { /* Draw the segment G */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_offset, i_right - 1, i_offset);
   }

#if defined(HP10) || defined(HP67) || defined(HP35) || defined(HP80) || defined(HP45) || defined(HP70) || defined(HP55)
   if (h_digit->mask & SEG_DECIMAL) { /* Draw a decimal point separator */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_middle, (i_upper + 3 * (i_lower - i_upper) / 4) - 1, i_middle, (i_upper + 3 * (i_lower - i_upper) / 4) + 1);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_middle - 1, (i_upper + 3 * (i_lower - i_upper) / 4), i_middle + 1, (i_upper + 3 * (i_lower - i_upper) / 4));
   }
#else
   if (h_digit->mask & SEG_DECIMAL) { /* Draw a decimal point separator */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower, i_right + 5, i_lower);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 4, i_lower - 1, i_right + 4, i_lower + 1);
   }

   if (h_digit->mask & SEG_COMMA) { /* Draw a comma separator */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower, i_right + 5, i_lower);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 4, i_lower - 1, i_right + 4, i_lower + 1);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower + 3, i_right + 4, i_lower + 3);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 2, i_lower + 4, i_right + 3, i_lower + 4);
   }

   if (h_digit->mask & SEG_COLON) { /* Draw a decimal point separator */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_offset - 3, i_right + 5, i_offset - 3);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 4, i_offset - 4, i_right + 4, i_offset - 2);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_offset + 3, i_right + 5, i_offset + 3);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 4, i_offset + 4, i_right + 4, i_offset + 2);
   }
#endif
   return(True);
}
