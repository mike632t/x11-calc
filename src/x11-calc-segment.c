/*
 * x11-calc-segment.c - RPN (Reverse Polish) calculator simulator.
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
 * 14 Jul 13         - Initial verson - MT
 * 16 Jul 13         - Draws basic display segments based on mask - MT
 * 18 Jul 13         - Added decimal point and comma (using shading) - MT
 *                   - Reversed the order of the segments in the bit mask,
 *                     segment A is now bit 1, and segment G bit 7 - MT
 * 19 Jul 13         - Added shading to the display segments and reduced
 *                     the number times the foreground colour is set by
 *                     drawing all the background elements for every segment
 *                     before filling in the foreground - MT
 * 15 Dec 18         - Changed debug macro and added an error macro - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 *
 * TO DO :           - Optimize drawing of display segment by drawing in
 ^                     all the darker background regions before the foreground.
 ^
 */

#define VERSION        "0.0"
#define BUILD          "0006"
#define DATE           "19 Jul 13"
#define AUTHOR         "MT"

#include <stdio.h>     /* fprintf(), etc */
#include <stdlib.h>    /* malloc(), etc */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc */
#include <X11/Xutil.h> /* XSizeHints etc */

#include "x11-calc-switch.h"
#include "x11-calc-button.h"

#include "x11-calc.h"

#include "x11-calc-colour.h"
#include "x11-calc-segment.h"

#include "gcc-debug.h"

/*
 * segment_create (index, text, left, top, width, height, state,
 *                colour)
 *
 * Allocates storage for a new display segment, sets the properties and
 * returns a pointer to the segment, or exits the program if there isn't
 * enough memory available.
 *
 */

osegment *h_segment_create(int i_index, int i_mask, int i_left, int i_top,
   int i_width, int i_height, unsigned int i_foreground, unsigned int i_background){

   osegment *h_segment; /* Ponter to segment */

   /* Attempt to allocate memory for a segment */
   if ((h_segment = malloc (sizeof(*h_segment)))==NULL) v_error("Memory allocation failed!");

   h_segment->index = i_index;
   h_segment->mask = i_mask;

   h_segment->left = i_left;
   h_segment->top = i_top;
   h_segment->width = i_width;
   h_segment->height = i_height;

   h_segment->foreground = i_foreground;
   h_segment->background = i_background;
   return (h_segment);
}

int i_segment_draw(Display *h_display, int x_application_window, int i_screen, osegment *h_segment){ /* Draws all the segments */

   int i_left, i_right, i_upper, i_lower;
   int i_offset;
#if defined(HP67) || defined (HP35) || defined (HP80) || defined (HP45) || defined (HP70) || defined(HP55)
   int i_middle;
#endif

   i_upper = h_segment->height / 4;
   i_lower = h_segment->top +  h_segment->height - i_upper;
   i_upper = h_segment->top + i_upper;
   i_offset = i_upper + (i_lower - i_upper) / 2;
   i_left = h_segment->left + 2;
#if defined(HP67) || defined (HP35) || defined (HP80) || defined (HP45) || defined (HP70) || defined(HP55)
   i_right = h_segment->left + h_segment->width - 2;
   i_middle = i_left + ((i_right - i_left) / 2);
#else
   i_right = h_segment->left + h_segment->width - 7;
#endif

   debug(fprintf(stderr, "%4d,%d (%dx%d) %.1X%.1X%.1X%.1X %.1X%.1X%.1X%.1X.\n", \
      h_segment->left, h_segment->top, h_segment->height, h_segment->width, \
      h_segment->mask & SEG_DECIMAL && 1, h_segment->mask & SEG_G && 1, \
      h_segment->mask & SEG_F && 1, h_segment->mask & SEG_E && 1, \
      h_segment->mask & SEG_D && 1, h_segment->mask & SEG_C && 1, \
      h_segment->mask & SEG_B && 1, h_segment->mask & SEG_A && 1));

   /* Draw the display segment background */
   XSetForeground(h_display, DefaultGC(h_display, i_screen), h_segment->background);
   XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), h_segment->left, h_segment->top, h_segment->width, h_segment->height);
   XDrawRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), h_segment->left, h_segment->top, h_segment->width, h_segment->height);

   /* Fill in the background for each active display segment */
   XSetForeground(h_display, DefaultGC(h_display, i_screen),i_shade( h_segment->foreground));

   if (h_segment->mask & SEG_A) { /* Draw the segment A */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left, i_upper, i_right, i_upper);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_upper - 1, i_right - i_left - 1, 3);
   }

   if (h_segment->mask & SEG_B) { /* Draw the segment B */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left , i_upper, i_left, i_offset);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left - 1, i_upper + 1, 3, i_offset - i_upper - 1);
   }

   if (h_segment->mask & SEG_C) { /* Draw the segment C */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left , i_offset, i_left, i_lower);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left - 1, i_offset + 1, 3, i_lower - i_offset - 1);
   }

   if (h_segment->mask & SEG_D) { /* Draw the segment D */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left, i_lower, i_right, i_lower);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_lower - 1, i_right - i_left - 1, 3);
   }

   if (h_segment->mask & SEG_E) { /* Draw the segment E */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right , i_lower, i_right, i_offset);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right - 1, i_offset + 1, 3, i_lower - i_offset - 1);
   }

   if (h_segment->mask & SEG_F) { /* Draw the segment F */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right , i_offset, i_right, i_upper);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right - 1, i_upper + 1, 3, i_offset - i_upper - 1);
   }

   if (h_segment->mask & SEG_G) { /* Draw the segment G */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left, i_offset, i_right, i_offset);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_offset - 1, i_right - i_left - 1, 3);
   }

#if defined(HP67) || defined (HP35) || defined (HP80) || defined (HP45) || defined (HP70) || defined(HP55)
   if (h_segment->mask & SEG_DECIMAL) { /* Draw a decimal point separator */
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_middle - 1 , (i_upper + 3 * (i_lower - i_upper) / 4) - 1, 3, 3);
   }
#else
   if (h_segment->mask & SEG_DECIMAL) { /* Draw a decimal point separator */
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower - 1, 3, 3);
   }

   if (h_segment->mask & SEG_COMMA) { /* Draw a comma separator */
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower - 1, 3, 3);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower + 2, i_right + 3, i_lower + 2);
   }

   if (h_segment->mask & SEG_COLON) { /* Draw a colon separator */
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_offset - 4, 3, 3);
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_offset + 2, 3, 3);
   }
#endif

   /* Draw the in the foreground elements */
   XSetForeground(h_display, DefaultGC(h_display, i_screen), h_segment->foreground);
   if (h_segment->mask & SEG_A) { /* Draw the segment A */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_upper, i_right - 1, i_upper);
   }

   if (h_segment->mask & SEG_B) { /* Draw the segment B */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left , i_upper + 1, i_left, i_offset - 1);
   }

   if (h_segment->mask & SEG_C) { /* Draw the segment C */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left , i_offset + 1, i_left, i_lower - 1);
   }

   if (h_segment->mask & SEG_D) { /* Draw the segment D */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_lower, i_right - 1, i_lower);
   }

   if (h_segment->mask & SEG_E) { /* Draw the segment E */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right , i_lower - 1, i_right, i_offset + 1);
   }

   if (h_segment->mask & SEG_F) { /* Draw the segment F */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right , i_offset - 1, i_right, i_upper + 1);
   }

   if (h_segment->mask & SEG_G) { /* Draw the segment G */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_left + 1, i_offset, i_right - 1, i_offset);
   }

#if defined(HP67) || defined (HP35) || defined (HP80) || defined (HP45) || defined (HP70) || defined(HP55)
   if (h_segment->mask & SEG_DECIMAL) { /* Draw a decimal point separator */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_middle, (i_upper + 3 * (i_lower - i_upper) / 4) - 1, i_middle, (i_upper + 3 * (i_lower - i_upper) / 4) + 1);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_middle - 1, (i_upper + 3 * (i_lower - i_upper) / 4), i_middle + 1, (i_upper + 3 * (i_lower - i_upper) / 4));
   }
#else
   if (h_segment->mask & SEG_DECIMAL) { /* Draw a decimal point separator */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower, i_right + 5, i_lower);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 4, i_lower - 1, i_right + 4, i_lower + 1);
   }

   if (h_segment->mask & SEG_COMMA) { /* Draw a comma separator */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower, i_right + 5, i_lower);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 4, i_lower - 1, i_right + 4, i_lower + 1);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_lower + 3, i_right + 4, i_lower + 3);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 2, i_lower + 4, i_right + 3, i_lower + 4);
   }

   if (h_segment->mask & SEG_COLON) { /* Draw a decimal point separator */
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_offset - 3, i_right + 5, i_offset - 3);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 4, i_offset - 4, i_right + 4, i_offset - 2);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 3, i_offset + 3, i_right + 5, i_offset + 3);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_right + 4, i_offset + 4, i_right + 4, i_offset + 2);
   }
#endif
   return(True);
}
