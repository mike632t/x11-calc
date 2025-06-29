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
 * 29 Sep 20         - Initial version - MT
 * 10 Oct 21         - Allow use of NULL pointers - MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 21 Oct 23         - Updated switch parameters to accomodate a 3 position
 *                     switch - MT
 * 22 Oct 23         - Added method to update state when clicked - MT
 * 23 Oct 23         - Added code to draw three position switches - MT
 * 24 Feb 24         - Do not need to include "x11-font.h" - MT
 * 03 Mar 24         - Updated error handling (now passes the  error number
 *                     to the error handler) - MT
 * 29 Mar 24         - Fixed  compiler 'vairable may be used uninitialized'
 *                     warnings - MT
 * 23 Apr 24         - Separated out prototypes for error handlers - MT
 *                   - Removed unnecessary includes - MT
 * 20 May 25         - Tidied up data structure definitions - MT
 *
 */

#define NAME           "x11-calc-switch"
#define BUILD          "0012"
#define DATE           "20 May 25"
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

/* switch_pressed (switch, x, y) */

struct oswitch *h_switch_pressed(struct oswitch *h_switch, int i_xpos, int i_ypos)
{
   int i_indent, i_extent, i_upper, i_lower;

   if (h_switch != NULL)
   {
      i_indent = h_switch->switch_position.x;
      i_extent = h_switch->switch_position.x + h_switch->switch_position.width;
      i_upper = h_switch->switch_position.y;
      i_lower = h_switch->switch_position.y + h_switch->switch_position.height;

      if (((i_xpos > i_indent ) && (i_xpos < i_extent)) &&
         ((i_ypos > i_upper ) && (i_ypos < i_lower)))
         return(h_switch);
   }
   return(NULL);
}

/* switch_create (index, text, font, left, top, width, height, colour) */

struct oswitch *h_switch_create(int i_index, char* s_on, char* s_mid, char* s_off,
   XFontStruct *h_normal_font,
   int i_left, int i_top, int i_width, int i_height, char b_state,
   unsigned int i_colour, unsigned int i_alternate_colour)
{
   struct oswitch *h_switch; /* Ponter to switch. */

   /* Attempt to allcoate memory for a switch. */
   if ((h_switch = malloc (sizeof(*h_switch)))==NULL) v_error(errno, h_err_memmory_alloc, __FILE__, __LINE__);

   h_switch->index = i_index;
   h_switch->on = s_on;
   h_switch->mid = s_mid;
   h_switch->off = s_off;
   h_switch->text_font = h_normal_font;

   h_switch->switch_position.x = i_left;
   h_switch->switch_position.y = i_top;
   h_switch->switch_position.width = i_width;
   h_switch->switch_position.height = i_height;

   h_switch->switch_geometry = h_switch->switch_position;

   h_switch->state = b_state;
   h_switch->colour = i_colour;
   h_switch->alternate_colour = i_alternate_colour;
   return(h_switch);
}

/*
 * switch_resize (display, scale)
 *
 * Resize switch based on original geometery
 *
 */

int i_switch_resize(struct oswitch *h_switch, float f_scale)
{
   h_switch->switch_position.x = h_switch->switch_geometry.x * f_scale;
   h_switch->switch_position.y = h_switch->switch_geometry.y * f_scale;
   h_switch->switch_position.width = h_switch->switch_geometry.width * f_scale;
   h_switch->switch_position.height = h_switch->switch_geometry.height * f_scale;
   return(True);
}

/* switch_draw (display, window, screen, switch) */

int i_switch_draw(Display *h_display, int x_application_window, int i_screen, struct oswitch *h_switch)
{
   int i_indent, i_upper;
   int i_on_colour = h_switch->colour, i_mid_colour = h_switch->alternate_colour, i_off_colour = h_switch->alternate_colour;

   if (h_switch != NULL)
   {
      XSetFont(h_display, DefaultGC(h_display, i_screen), h_switch->text_font->fid); /* Set the text font. */
      if ((h_switch->mid != NULL) && (h_switch->mid[0] != '\0'))
      {
         i_upper = h_switch->switch_position.y + (h_switch->text_font->ascent) + (h_switch->switch_position.height / 2 - (h_switch->text_font->ascent + h_switch->text_font->descent)) / 2;
         switch (h_switch->state)
         {
            case 0:
               i_on_colour = h_switch->colour;
               i_mid_colour = h_switch->alternate_colour;
               i_off_colour = h_switch->alternate_colour;
               break;
            case 1:
            case 3:
               i_on_colour = h_switch->alternate_colour;
               i_mid_colour = h_switch->colour;
               i_off_colour = h_switch->alternate_colour;
               break;
            case 2:
               i_on_colour = h_switch->alternate_colour;
               i_mid_colour = h_switch->alternate_colour;
               i_off_colour = h_switch->colour;
               break;
         }
      }
      else
      {
         i_upper = h_switch->switch_position.y + (h_switch->text_font->ascent) + (h_switch->switch_position.height - (h_switch->text_font->ascent + h_switch->text_font->descent)) / 2;
         switch (h_switch->state)
         {
            case 0:
               i_on_colour = h_switch->colour;
               i_mid_colour = h_switch->alternate_colour;
               i_off_colour = h_switch->alternate_colour;
               break;
            default:
               i_on_colour = h_switch->alternate_colour;
               i_mid_colour = h_switch->alternate_colour;
               i_off_colour = h_switch->colour;
               break;
         }
      }

      XSetForeground(h_display, DefaultGC(h_display, i_screen), i_on_colour);
      i_indent = 1 + h_switch->switch_position.x + ((h_switch->switch_position.width / 2) - XTextWidth(h_switch->text_font, h_switch->on, strlen(h_switch->on))) / 2; /* Find position of the text */
      XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper, h_switch->on, strlen(h_switch->on)); /* Draw the main text */

      XSetForeground(h_display, DefaultGC(h_display, i_screen), i_off_colour);
      i_indent = 1 + h_switch->switch_position.x + (h_switch->switch_position.width / 2) + ((h_switch->switch_position.width / 2) - XTextWidth(h_switch->text_font, h_switch->on, strlen(h_switch->on))) / 2; /* Find position of the text */
      XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper, h_switch->off, strlen(h_switch->off)); /* Draw the main text */

      if ((h_switch->mid != NULL) && (h_switch->mid[0] != '\0'))
      {
         XSetForeground(h_display, DefaultGC(h_display, i_screen), i_mid_colour);
         i_indent = h_switch->switch_position.x + h_switch->switch_position.width / 2 - XTextWidth(h_switch->text_font, h_switch->mid, strlen(h_switch->mid)) / 2; /* Find position of the text */
         i_upper = i_upper + (h_switch->text_font->ascent + h_switch->text_font->descent);
         XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper, h_switch->mid, strlen(h_switch->mid)); /* Draw the main text */
      }
   }
   return(True);
}

/* switch_click (display, window, screen, switch) - Updates switch status */

int i_switch_click(struct oswitch *h_switch)
{
   if (h_switch != NULL)
   {
      if ((h_switch->mid != NULL) && (h_switch->mid[0] != '\0'))
      {
         h_switch->state++; /* Update switch state */
         if (h_switch->state > 3) h_switch->state = 0;
      }
      else
         h_switch->state = !(h_switch->state); /* Toggle prgm/run switch */
   }
   return(h_switch->state);
}

