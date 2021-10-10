/*
 * x11-calc-switch.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * Defines the routines and data structures used to draw switches.
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
 * 10 Oct 21         - Allow use of NULL pointers - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0001"
#define DATE           "29 Sep 21"
#define AUTHOR         "MT"

#define DEBUG 0        /* Enable/disable debug*/

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

/* switch_pressed (switch, x, y) */

oswitch *h_switch_pressed(oswitch *h_switch, int i_xpos, int i_ypos){

   int i_indent, i_extent, i_upper, i_lower;

   if (h_switch != NULL) {
      i_indent = h_switch->left;
      i_extent = h_switch->left + h_switch->width;
      i_upper = h_switch->top;
      i_lower = h_switch->top + h_switch->height;

      if (((i_xpos > i_indent ) && (i_xpos < i_extent)) &&
         ((i_ypos > i_upper ) && (i_ypos < i_lower))) {
         return(h_switch);
      }
   }
   return(NULL);
}

/* switch_create (index, text, font, left, top, width, height, colour) */

oswitch *h_switch_create(int i_index, char* s_text, char* s_alternate,
   XFontStruct *h_normal_font,
   int i_left, int i_top, int i_width, int i_height, char b_state,
   unsigned int i_colour, unsigned int i_alternate_colour) {

   oswitch *h_switch; /* Ponter to switch. */

   /* Attempt to allcoate memory for a switch. */
   if ((h_switch = malloc (sizeof(*h_switch)))==NULL) v_error("Memory allocation failed!");

   h_switch->index = i_index;
   h_switch->text = s_text;
   h_switch->alternate = s_alternate;
   h_switch->text_font = h_normal_font;

   h_switch->left = i_left;
   h_switch->top = i_top;
   h_switch->width = i_width;
   h_switch->height = i_height;

   h_switch->state = b_state;
   h_switch->colour = i_colour;
   h_switch->alternate_colour = i_alternate_colour;
   return(h_switch);
}

/* switch_draw (display, window, screen, switch) */

int i_switch_draw(Display *h_display, int x_application_window, int i_screen, oswitch *h_switch){

   int i_indent, i_upper;
   
   if (h_switch != NULL) {
      if (h_switch->state) /* Select the text colour */
         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_switch->alternate_colour); 
      else
         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_switch->colour); 
      
      XSetFont(h_display, DefaultGC(h_display, i_screen), h_switch->text_font->fid); /* Set the text font. */
      i_upper = h_switch->top + (h_switch->text_font->ascent) + (h_switch->height - (h_switch->text_font->ascent + h_switch->text_font->descent)) / 2; 
      i_indent = 1 + h_switch->left + ((h_switch->width / 2) - XTextWidth(h_switch->text_font, h_switch->text, strlen(h_switch->text))) / 2; /* Find position of the text */
      XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper, h_switch->text, strlen(h_switch->text)); /* Draw the main text */

      if (h_switch->state) /* Select the alternate text colour */
         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_switch->colour); 
      else
         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_switch->alternate_colour); 

      i_indent = 1 + h_switch->left + (h_switch->width / 2) + ((h_switch->width / 2) - XTextWidth(h_switch->text_font, h_switch->text, strlen(h_switch->text))) / 2; /* Find position of the text */
      XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper, h_switch->alternate, strlen(h_switch->alternate)); /* Draw the main text */
   }
   return(True);
}
