/*
 * x11-calc-button.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * Button functions.
 *
 * Implements  the  routines needed to create the data structures  used  to
 * hold the properties and the functions needed to create, draw, and  check
 * if a button has been clicked.
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
 * 03 Jul 13         - Initial verson with new button style - MT
 * 04 Jul 13         - Optimized the number of drawing operations - MT
 * 07 Jul 13         - Tuned position of font - MT
 * 14 Jul 13         - Flush X buffer each time button is drawn - MT
 * 15 Dec 18         - Changed debug macro and added an error macro - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 30 Sep 21         - Added the additional properties for the label colour
 *                     and alternate function colour - MT
 * 10 Oct 21         - Allow use of NULL pointers - MT

 *
 * TO DO             - Add an alternate label text
 *
 */

#define VERSION        "0.1"
#define BUILD          "0006"
#define DATE           "14 Jul 13"
#define AUTHOR         "MT"

#define DEBUG 0        /* Enable/disable debug*/

#include <string.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* malloc(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-calc-font.h"
#include "x11-calc-switch.h"
#include "x11-calc-button.h"

#include "x11-calc.h"

#include "x11-calc-colour.h"
#include "gcc-debug.h"

/*
 * button_key_pressed (button, key)
 *
 * If the x and y co-ordinate are within the area of the button this
 * function returns a pointer to the button, or a NULL otherwise.
 *
 * 01 Jul 13         - Initial version - MT
 *
 */

obutton *h_button_key_pressed(obutton *h_button, char c_key){
   
   if (h_button != NULL) {
      if (h_button->key != '\000') {
         if (c_key == h_button->key)
            return(h_button);
      }
   }
   return(NULL);
}
/* button_pressed (button, x, y) */

obutton *h_button_pressed(obutton *h_button, int i_xpos, int i_ypos){

   int i_indent, i_extent, i_upper, i_lower;

   if (h_button != NULL) {
      i_indent = h_button->left;
      i_extent = h_button->left + h_button->width;
      i_upper = h_button->top;
      i_lower = h_button->top + h_button->height;

      if (((i_xpos > i_indent ) && (i_xpos < i_extent)) &&
         ((i_ypos > i_upper ) && (i_ypos < i_lower))) {
         return(h_button);
      }
   }
   return(NULL);
}

/*button_create (index, key, text, label ,alternate, font, label_font,
 *                alternate_font, left, top, width, height,
 *                state, colour)
 */

obutton *h_button_create(int i_index, char c_key, char* s_text, char* s_label ,char* s_alternate,
   XFontStruct *h_normal_font, XFontStruct *h_shift_font, XFontStruct *h_alternate_font,
   int i_left, int i_top, int i_width, int i_height, int i_state,
   unsigned int i_colour, unsigned int i_label_colour, unsigned int i_alternate_colour) {

   obutton *h_button; /* Ponter to button. */

   /* Attempt to allcoate memory for a button. */
   if ((h_button = malloc (sizeof(*h_button)))==NULL) v_error("Memory allocation failed!");

   h_button->index = i_index;
   h_button->key = c_key;
   h_button->text = s_text;
   h_button->label = s_label;
   h_button->alternate = s_alternate;
   h_button->text_font = h_normal_font;
   h_button->label_font = h_shift_font;
   h_button->alternate_font = h_alternate_font;

   h_button->left = i_left;
   h_button->top = i_top;
   h_button->width = i_width;
   h_button->height = i_height;

   h_button->state = i_state;
   h_button->colour = i_colour;
   h_button->label_colour = i_label_colour;
   h_button->alternate_colour = i_alternate_colour;
   return(h_button);
}

/* button_draw (display, window, screen, button) */

int i_button_draw(Display *h_display, int x_application_window, int i_screen, obutton *h_button) {

   int i_indent, i_extent, i_upper, i_lower;
   int i_offset;

   if (h_button != NULL) {
      /* Set label foreground colour and font. */
      XSetForeground(h_display, DefaultGC(h_display, i_screen), h_button->label_colour);
      XSetFont(h_display, DefaultGC(h_display, i_screen), h_button->label_font->fid);

      /* Find position of the label. */
      i_indent = 1 + h_button->left + (h_button->width - XTextWidth(h_button->label_font, h_button->label, strlen(h_button->label))) / 2;
      i_upper = h_button->top - (h_button->label_font->descent + 1); /* Draw label 1 pixel above button. */

      /* Draw the label. */
      XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper ,h_button->label, strlen(h_button->label));

      /* Set button background colour - as forground colour! */
      XSetForeground(h_display, DefaultGC(h_display, i_screen), DARK_TEXT);

      /* Draw the button background on the window. */
      i_upper = h_button->top;
      i_lower = h_button->top + h_button->height - 2;
      i_indent = h_button->left + 1;
      i_extent = h_button->left + h_button->width - 2;
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent , h_button->top , i_extent, h_button->top);
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent , i_lower, i_extent, i_lower);
      i_upper++;
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), h_button->left, i_upper , h_button->width, h_button->height - 3);
      i_indent = i_indent + 2;
      i_extent = i_extent - 2;
      i_lower++;
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent , i_lower, i_extent, i_lower);

      /* Set the foreground colour. */
      XSetForeground(h_display, DefaultGC(h_display, i_screen), h_button->colour);

      /* Draw the button face on the background.  */
      i_lower = i_lower - 3;
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent , i_lower, i_extent, i_lower);
      i_upper = i_upper + 2;
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent , i_upper, i_extent, i_upper);
      i_indent--; i_extent++; i_upper++; i_lower--;
      XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper, h_button->width - 4, h_button->height - 8 );
      i_lower = i_lower + 2;
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent + 3, i_lower, i_extent -3 , i_lower);

      /* Set the highlight colour. */
      /* Select approprite colour for highlight (or lowlight) depending on button state. */
      if ((h_button->state)) { /* Set the foreground colour to darker tint of the base colour. */
         if (((h_button->colour & 0xff) + (h_button->colour >> 8 & 0xff) + (h_button->colour >> 16 & 0xff)) > 384)
            XSetForeground(h_display, DefaultGC(h_display, i_screen), i_shade(h_button->colour));
      }
      else  /* Set the foreground colour to lighter tint of the base colour. */
         XSetForeground(h_display, DefaultGC(h_display, i_screen), i_tint(h_button->colour));
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent , i_lower - 2, i_indent , i_upper);
      i_indent++; i_extent--; i_upper--;
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent , i_upper, i_extent , i_upper);

      /* Set the foreground colour to lighter tint of the base colour. */
      XSetForeground(h_display, DefaultGC(h_display, i_screen), i_tint(h_button->colour));

      /* Find middle of button. */
      i_offset = h_button->top + 2 + h_button->height /2;

      /* Find vertical position of text */
      i_upper = i_upper + (1 + i_offset - i_upper + h_button->text_font->ascent + h_button->text_font->descent) / 2 - h_button->text_font->descent;
      i_lower = i_offset + (i_lower - i_offset + h_button->alternate_font->ascent + h_button->alternate_font->descent) / 2 - h_button->alternate_font->descent;

      if ((h_button->state)){
         i_offset--;
         i_upper--;
      }
      else {
         i_lower++;
      }
      XDrawLine(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent , i_offset, i_extent , i_offset); /* Horizontal highlight. */

      /* Set text foreground colour based on the the brightness of the button colour and font. */
      if (((h_button->colour & 0xff) + (h_button->colour >> 8 & 0xff) + (h_button->colour >> 16 & 0xff))  > 384)
         XSetForeground(h_display, DefaultGC(h_display, i_screen), DARK_TEXT);
      else
         XSetForeground(h_display, DefaultGC(h_display, i_screen), LIGHT_TEXT);

      /* Set the text font. */
      XSetFont(h_display, DefaultGC(h_display, i_screen), h_button->text_font->fid);

      /* Find position of the text. */
      i_indent = 1 + h_button->left + (h_button->width - XTextWidth(h_button->text_font, h_button->text, strlen(h_button->text))) / 2;

      /* Draw the main text. */
      XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_upper ,h_button->text, strlen(h_button->text));

      /* Set alternate text foreground colour and font. */
      XSetForeground(h_display, DefaultGC(h_display, i_screen), h_button->alternate_colour);
      XSetFont(h_display, DefaultGC(h_display, i_screen), h_button->alternate_font->fid);

      /* Find position of the alternate text. */
      i_indent = 1 + h_button->left + (h_button->width - XTextWidth(h_button->alternate_font, h_button->alternate, strlen(h_button->alternate))) / 2;

      /* Draw the text. */
      XDrawString(h_display, x_application_window, DefaultGC(h_display, i_screen), i_indent, i_lower, h_button->alternate, strlen(h_button->alternate));
   }
   return(True);
}


