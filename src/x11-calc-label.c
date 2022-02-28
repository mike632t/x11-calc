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
 * 29 Sep 20   0.1   - Initial version - MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 10 Feb 22         - Added background shading and horizontal line - MT
 *
 * TO DO:            - Implement ability to align text in a label using the
 *                     style property to modify the position and appearance
 *                     of the text.
 *
 */

#define VERSION        "0.1"
#define BUILD          "0001"
#define DATE           "29 Sep 21"
#define AUTHOR         "MT"

#include <string.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* malloc(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-calc-font.h"
#include "x11-calc-label.h"
#include "x11-calc-switch.h"
#include "x11-calc-button.h"

#include "x11-calc.h"

#include "x11-calc-colour.h"
#include "gcc-debug.h"

/* label_pressed (label, x, y) */

olabel *h_label_pressed(olabel *h_label, int i_xpos, int i_ypos){

   int i_indent, i_extent, i_upper, i_lower;

   i_indent = h_label->left;
   i_extent = h_label->left + h_label->width;
   i_upper = h_label->top;
   i_lower = h_label->top + h_label->height;

   if (((i_xpos > i_indent ) && (i_xpos < i_extent)) &&
      ((i_ypos > i_upper ) && (i_ypos < i_lower))) {
      return(h_label);
   }
   return(NULL);
}

/* label_create (index, text, font, left, top, width, height, colour) */

olabel *h_label_create(int i_index, char* s_text, XFontStruct *h_font,
   int i_left, int i_top, int i_width, int i_height,
   unsigned int i_colour, unsigned int i_background) {

   olabel *h_label; /* Ponter to label. */

   /* Attempt to allcoate memory for a label. */
   if ((h_label = malloc (sizeof(*h_label)))==NULL) v_error("Memory allocation failed!");

   h_label->index = i_index;
   h_label->text = s_text;
   h_label->text_font = h_font;

   h_label->left = i_left;
   h_label->top = i_top;
   h_label->width = i_width;
   h_label->height = i_height;

   h_label->colour = i_colour;
   h_label->background = i_background;
   return(h_label);
}

/* label_draw (display, window, screen, label) */

int i_label_draw(Display *h_display, int x_application_window, int i_screen, olabel *h_label){

   int i_indent, i_upper, i_offset;
   i_offset = h_label->top + h_label->height / 2;
   XSetFont(h_display, DefaultGC(h_display, i_screen), h_label->text_font->fid); /* Set the text font. */
   i_indent = 1 + h_label->left + (h_label->width - XTextWidth(h_label->text_font, h_label->text, strlen(h_label->text))) / 2; /* Find position of the text. */
   i_upper = h_label->top + (h_label->text_font->ascent) + (h_label->height - (h_label->text_font->ascent + h_label->text_font->descent)) / 2; /* Position text in middle of label. */

   XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->background);
   XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), h_label->left, h_label->top , h_label->width, h_label->height); /* Fill in label background. */

   XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->colour); /* Set the text colour. */
   XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen),h_label->left , i_offset, h_label->left + h_label->width - 2, i_offset); /* Draw line through middle of label. */

   XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->background);
   XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), (i_indent - 3), h_label->top , XTextWidth(h_label->text_font, h_label->text, strlen(h_label->text)) + 5, h_label->height); /* Fill in label background. */

   XSetForeground(h_display, DefaultGC(h_display, i_screen), h_label->colour); /* Set the background colour. */
   XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper, h_label->text, strlen(h_label->text)); /* Draw the text. */
   return(True);
}


