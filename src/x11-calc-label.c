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
 * 27 Oct 26         - Always  fills in the background (allowing a label to
 *                     be hidden by disabling it) - NT
 *                   - Allow use of NULL string - MT
 * 28 Oct 25         - Added alignment and text attributes - MT
 * 31 Oct 25         - Tidied up label object properties - MT
 *
 * TO DO:            - Implement ability to align text in a label using the
 *                     style property to modify the position and appearance
 *                     of the text.
 *
 */

#define NAME           "x11-calc-label"
#define BUILD          "0013"
#define DATE           "31 Oct 25"
#define AUTHOR         "MT"

#include <errno.h>     /* errno */

#include <string.h>    /* strlen(), etc */
#include <stdio.h>     /* fprintf(), etc */
#include <stdlib.h>    /* malloc(), etc */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc */
#include <X11/Xutil.h> /* XSizeHints etc */

#include "x11-calc-messages.h"
#include "x11-calc-errors.h"

#include "x11-calc-label.h"
#include "x11-calc-switch.h"
#include "x11-calc-button.h"

#include "gcc-debug.h"

/* label_pressed(label, x, y) */

struct olabel *h_label_pressed(struct olabel *h_label, int i_xcordinate, int i_ycordinate)
{
   int i_indent, i_extent, i_upper, i_lower;

   i_indent = h_label->position.x;
   i_extent = h_label->position.x + h_label->position.width;
   i_upper = h_label->position.y;
   i_lower = h_label->position.y + h_label->position.height;

   if (((i_xcordinate > i_indent ) && (i_xcordinate < i_extent)) &&
      ((i_ycordinate > i_upper ) && (i_ycordinate < i_lower)))
      return(h_label);
   return(NULL);
}

/* label_create (index, text, font, left, top, width, height, colour) */

struct olabel *h_label_create(int i_index, char* s_text, XFontStruct *h_font,
   int i_left, int i_top, int i_width, int i_height,
   unsigned int i_foreground, unsigned int i_background, int i_alignment,
   int i_attributes, int i_state)
{
   struct olabel *h_label;  /* Pointer to label */

   if ((h_label = malloc (sizeof(*h_label)))==NULL) v_error(errno, h_err_memmory_alloc, __FILE__, __LINE__);  /* Attempt to allocate memory for a label */

   h_label->index = i_index;
   h_label->text = s_text;
   h_label->font = h_font;
   h_label->position.x = i_left;
   h_label->position.y = i_top;
   h_label->position.width = i_width;
   h_label->position.height = i_height;

   h_label->geometry = h_label->position;  /* Save position */

   h_label->foreground = i_foreground;
   h_label->background = i_background;
   h_label->attributes = i_attributes;
   h_label->alignment = i_alignment;
   h_label->state = i_state;
   return(h_label);
}

/* label_resize (display, scale) */

int i_label_resize(struct olabel *h_label, float f_scale)
{
   h_label->position.x = h_label->geometry.x * f_scale;
   h_label->position.y = h_label->geometry.y * f_scale;
   h_label->position.width = h_label->geometry.width * f_scale;
   h_label->position.height = h_label->geometry.height * f_scale;

   return 0;
}

/* label_draw (display, window, screen, label) */

int i_label_draw(Display *h_display, int x_application_window, int i_screen, struct olabel *h_label)
{

   int i_indent, i_upper, i_offset, i_margin = 2;
   if (h_label != NULL)
   {
      XSetFont(h_display, DefaultGC(h_display, i_screen), h_label->font->fid);  /* Set the text font */
      XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->background); /* Always fill in label background - 'hides' labels when disabled */
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen),
         h_label->position.x, h_label->position.y , h_label->position.width, h_label->position.height);
      if ((h_label->state) && (h_label->text))  /* Only draw text if enabled and not blank */
      {
         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->foreground);  /* Set the text colour */
         if (h_label->attributes)
         {
            i_offset = h_label->position.y + h_label->position.height / 2;  /* Find offset to middle of label */
            XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen),
            h_label->position.x , i_offset, h_label->position.x + h_label->position.width - 2, i_offset);  /* Draw line through middle of label */
         }
         switch (h_label->alignment)
         {
            case LABEL_ALIGN_LEFT:
               i_indent = 1 + h_label->position.x + XTextWidth(h_label->font, " ", 1) + i_margin;  /* Text left aligned */
               break;
            case LABEL_ALIGN_RIGHT:
               i_indent = 1 + h_label->position.x + (h_label->position.width - XTextWidth(h_label->font, h_label->text, strlen(h_label->text)) - XTextWidth(h_label->font, " ", 1)) - i_margin;  /* Text right aligned */
               break;
            default:
               i_indent = 1 + h_label->position.x + (h_label->position.width - XTextWidth(h_label->font, h_label->text, strlen(h_label->text))) / 2;  /* Text centre aligned */
         }
         i_upper = h_label->position.y + (h_label->font->ascent) + (h_label->position.height - (h_label->font->ascent + h_label->font->descent)) / 2;  /* Position text in middle of label */
         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->background);
         XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen),
            (i_indent - 3), h_label->position.y , XTextWidth(h_label->font, h_label->text, strlen(h_label->text)) + 5, h_label->position.height);  /* Fill in label background */
         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->foreground);  /* Set the background colour */
         XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper, h_label->text, strlen(h_label->text));  /* Draw the text */
      }
   }
   return(True);
}
