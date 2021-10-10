/*
 * x11-calc-display.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * 10 Digit digit display functions.
 *
 * Contains  the functions needed to create and display a 10 digit 7 seven
 * segment display element.
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
 * On VMS compile with 'cc x11-calc-display'
 *
 * On UNIX Compile with 'gcc x11-calc-display.c -c'
 *
 * 14 Jul 13         - Initial verson - MT
 * 15 Dec 18         - Changed debug macro and added an error macro - MT
 * 23 Aug 20         - Removed the error macro - MT
 * 30 Aug 20         - Base  the number of display segments on the  maximum
 *                     number of digits in the display - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 26 Aug 21         - Updated  the display routines to use the contents of
 *                     the A and B registers - MT
 *                   - Reversed  order of nibbles in the registers to match
 *                     the  actual processor numbering (when  the  register
 *                     format  says  the  left  hand  nibble  is  the  most
 *                     significant means it!) - MT
 * 30 Aug 21         - Checks that the display is enabled when updating the
 *                     display- MT
 *                   - Removed unused variables - MT
 * 10 Sep 21         - Updated  lookup  table with codes  used  to  display
 *                     error messages - MT
 *  1 Oct 21         - Converted flags to Boolean variables - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0005"
#define DATE           "30 Aug 20"
#define AUTHOR         "MT"

#define DEBUG 0        /* Enable/disable debug*/

#include <stdlib.h>    /* malloc(), etc. */
#include <stdio.h>     /* fprintf(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-calc-button.h"
#include "x11-calc-switch.h"

#include "x11-calc.h"

#include "x11-calc-colour.h"
#include "x11-calc-segment.h"
#include "x11-calc-display.h"
#include "x11-calc-cpu.h"

#include "gcc-debug.h"


/*
 * display_create (index, text, left, top, width, height,
 *                margin, header, footer,
 *                foreground, backgroind, border)
 *
 * Allocates storage for a seven segment display, sets the properties and
 * returns a pointer to the display, or exits the program if there isn't
 * enough memory available.  The height and width of the display are based on
 * the margins, size of the header and footer, number of digits, and the
 * width and height of the individual display segments.
 *
 */

odisplay *h_display_create(int i_index, int i_left, int i_top, int i_width,
   int i_height, unsigned int i_foreground, unsigned int i_background, unsigned int i_border){

   odisplay *h_display; /* Ponter to display. */
   int i_count;

   /* Attempt to allocate memory for a display. */
   if ((h_display = malloc (sizeof(*h_display)))==NULL) v_error("Memory allocation failed!");
   h_display->index = i_index;
   h_display->left = i_left;
   h_display->top = i_top;
   h_display->width = i_width;
   h_display->height = i_height;
   for (i_count = 0; i_count < DIGITS; i_count++) {
      /*h_display->segment[i_count] = h_segment_create(0, 0,  7 + 17 * i_count, 21, 17, 29, i_foreground, i_background); /* Spice  - 11 Digit display */
      h_display->segment[i_count] = h_segment_create(0, 0,  5 + 16 * i_count, 21, 14, 29, i_foreground, i_background); /* Woodstock - 12 Digit display */
      h_display->segment[i_count]->mask = DISPLAY_SPACE;
   }
   h_display->foreground = i_foreground;
   h_display->background = i_background;
   h_display->border = i_border;
   return (h_display);
}

/*
 * display_draw (display, window, screen, display)
 *
 * Draws a seven segment display on the screen.
 *
 */

int i_display_draw(Display* x_display, int x_application_window, int i_screen, odisplay *h_display){

   int i_count;

   /* Set the foreground colour. */
   XSetForeground(x_display, DefaultGC(x_display, i_screen), h_display->border);
   XFillRectangle(x_display, x_application_window, DefaultGC(x_display, i_screen), h_display->left, h_display->top, h_display->width, h_display->height);

   /* Draw display segments. */
   for (i_count = 0; i_count < DIGITS; i_count++)
      if (!(h_display->segment[i_count] == NULL)) i_segment_draw(x_display, x_application_window, i_screen, h_display->segment[i_count]);
  return (True);
}

/*
 * display_update (display, window, screen, display)
 *
 * Updates the display based on the contents of the A and B registers.
 *
 */

int i_display_update(Display* x_display, int x_application_window, int i_screen, odisplay *h_display, oprocessor *h_processor){

   static int c_digits [] = { DISPLAY_ZERO,
                              DISPLAY_ONE,
                              DISPLAY_TWO,
                              DISPLAY_THREE,
                              DISPLAY_FOUR,
                              DISPLAY_FIVE,
                              DISPLAY_SIX,
                              DISPLAY_SEVEN,
                              DISPLAY_EIGHT,
                              DISPLAY_NINE,
                              DISPLAY_r,
                              DISPLAY_SPACE,
                              DISPLAY_o,
                              DISPLAY_SPACE,
                              DISPLAY_E,
                              DISPLAY_SPACE };
   int i_count;

   /* Draw display segments. */
   for (i_count = 0; i_count < DIGITS; i_count++)
      if (!(h_display->segment[i_count] == NULL)) {
         if (h_processor->flags[DISPLAY_ENABLE] && h_processor->enabled) {
            h_display->segment[i_count]->mask = c_digits[h_processor->reg[A_REG]->nibble[REG_SIZE - 1 - i_count]];
            switch (h_processor->reg[B_REG]->nibble[REG_SIZE - 1 - i_count] & 0x07) {
            case 0x02: /* Sign */
               if (h_processor->reg[A_REG]->nibble[REG_SIZE -1 - i_count])
                  h_display->segment[i_count]->mask = DISPLAY_MINUS;
               else
                  h_display->segment[i_count]->mask = DISPLAY_SPACE;
               break;
            case 0x01: /* Number and decimal point */
               h_display->segment[i_count]->mask = h_display->segment[i_count]->mask | DISPLAY_DECIMAL;
            }
         }
         else
            h_display->segment[i_count]->mask = DISPLAY_SPACE;
      }
   return (True);
}
