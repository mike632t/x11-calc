/*
 * x11-calc-label.h - RPN (Reverse Polish) calculator simulator.
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
 * 29 Jul 13         - Initial version - MT
 *
 */

typedef struct { /* Calculator label structure */
   int index;
   char* text; /* Text */
   XFontStruct* text_font; /* Pointer to font */
   int left;
   int top;
   int width;
   int height;
   unsigned int colour; /* Colour */
   unsigned int background; /* Background colour */
} olabel;

olabel *h_label_pressed(olabel *h_label, int i_xpos, int i_ypos);

olabel *h_label_create(int i_index, char* s_text, XFontStruct *h_font,
   int i_left, int i_top, int i_width, int i_height,
   unsigned int i_colour, unsigned int i_background);

int i_label_draw(Display *h_display, int x_application_window, int i_screen, olabel *h_label);
