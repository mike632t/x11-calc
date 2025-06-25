/*
 * x11-calc-label.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * Defines the routines and data structures used to draw labels.
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
 * 29 Sep 20         - Initial version - MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 10 Feb 22         - Added background shading and horizontal line - MT
 * 12 Mar 22         - Implemented a state property allowing the appearance
 *                     of the label to be changed (hidden, or no line) - MT
 * 24 Feb 24         - Do not need to include "x11-font.h" - MT
 * 03 Mar 24         - Updated error handling (now passes the  error number
 *                     to the error handler) - MT
 * 22 Apr 24         - Shortened long lines - MT
 * 23 Apr 24         - Separated out prototypes for error handlers - MT
 *                   - Removed unnecessary includes - MT
 *
 * TO DO:            - Implement ability to align text in a label using the
 *                     style property to modify the position and appearance
 *                     of the text.
 *
 */

#define NAME           "x11-calc-label"
#define BUILD          "0009"
#define DATE           "23 Apr 24"
#define AUTHOR         "MT"

#include <errno.h>     /* errno */

#include <string.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* malloc(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-calc-messages.h"
#include "x11-calc-errors.h"

#include "x11-calc-label.h"
#include "x11-calc-switch.h"
#include "x11-calc-button.h"

#include "gcc-debug.h"

/* label_pressed (label, x, y) */

struct olabel *h_label_pressed(struct olabel *h_label, int i_xpos, int i_ypos)
{

   int i_indent, i_extent, i_upper, i_lower;

   i_indent = h_label->label_position.x;
   i_extent = h_label->label_position.x + h_label->label_position.width;
   i_upper = h_label->label_position.y;
   i_lower = h_label->label_position.y + h_label->label_position.height;

   if (((i_xpos > i_indent ) && (i_xpos < i_extent)) &&
      ((i_ypos > i_upper ) && (i_ypos < i_lower)))
      return(h_label);
   return(NULL);
}

/* label_create (index, text, font, left, top, width, height, colour) */

struct olabel *h_label_create(int i_index, char* s_text, XFontStruct *h_font,
   int i_left, int i_top, int i_width, int i_height,
   unsigned int i_colour, unsigned int i_background, int i_state)
{

   struct olabel *h_label; /* Ponter to label. */

   /* Attempt to allcoate memory for a label. */
   if ((h_label = malloc (sizeof(*h_label)))==NULL) v_error(errno, h_err_memmory_alloc, __FILE__, __LINE__);

   h_label->index = i_index;
   h_label->text = s_text;
   h_label->text_font = h_font;

   h_label->label_position.x = i_left;

   h_label->label_position.x = i_left;
   h_label->label_position.y = i_top;
   h_label->label_position.width = i_width;
   h_label->label_position.height = i_height;

   h_label->label_geometry = h_label->label_position; /* Save position */

   h_label->colour = i_colour;
   h_label->background = i_background;
   h_label->state = i_state;
   return(h_label);
}

/*
 * label_resize (display, scale)
 *
 * Resize label based on original geometery
 *
 */

int i_label_resize(struct olabel *h_label, float f_scale)
{
   h_label->label_position.x = h_label->label_geometry.x * f_scale;
   h_label->label_position.y = h_label->label_geometry.y * f_scale;
   h_label->label_position.width = h_label->label_geometry.width * f_scale;
   h_label->label_position.height = h_label->label_geometry.height * f_scale;

   return 0;
}

/*
 * label_draw (display, window, screen, label)
 *
 * Draw a label.
 *
 */

int i_label_draw(Display *h_display, int x_application_window, int i_screen, struct olabel *h_label)
{

   int i_indent, i_upper, i_offset;
   if (h_label != NULL)
   {
      if (h_label->state)
      {
         i_offset = h_label->label_position.y + h_label->label_position.height / 2;
         XSetFont(h_display, DefaultGC(h_display, i_screen), h_label->text_font->fid); /* Set the text font. */
         i_indent = 1 + h_label->label_position.x + (h_label->label_position.width - XTextWidth(h_label->text_font, h_label->text, strlen(h_label->text))) / 2; /* Find position of the text. */
         i_upper = h_label->label_position.y + (h_label->text_font->ascent) + (h_label->label_position.height - (h_label->text_font->ascent + h_label->text_font->descent)) / 2; /* Position text in middle of label. */

         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->background);
         XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen),
            h_label->label_position.x, h_label->label_position.y , h_label->label_position.width, h_label->label_position.height); /* Fill in label background. */

         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->colour); /* Set the text colour. */
         if (h_label->state < 0)
            XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen),
            h_label->label_position.x , i_offset, h_label->label_position.x + h_label->label_position.width - 2, i_offset); /* Draw line through middle of label. */

         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->background);
         XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen),
            (i_indent - 3), h_label->label_position.y , XTextWidth(h_label->text_font, h_label->text, strlen(h_label->text)) + 5, h_label->label_position.height); /* Fill in label background. */

         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->colour); /* Set the background colour. */
         XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper, h_label->text, strlen(h_label->text)); /* Draw the text. */
      }
   }
   return(True);
}


