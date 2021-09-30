/*
 * x11-calc-switch.h - RPN (Reverse Polish) calculator simulator.
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
 * 29 Jul 13         - Initial version - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 *
 */

typedef struct { /* Calculator switch structure */
   int index;
   char* text; /* Text */ 
   char* alternate; /* Alternate text */
   XFontStruct* text_font; /* Pointer to font */
   int left;
   int top;
   int width;
   int height;
   char state;
   char inverted;
   unsigned int colour; /* Colour */
   unsigned int alternate_colour; /* Colour */
} oswitch;

oswitch *h_switch_pressed(oswitch *h_switch, int i_xpos, int i_ypos);

oswitch *h_switch_create(int i_index, char* s_text, char* s_alternate_text,
   XFontStruct *h_normal_font,
   int i_left, int i_top, int i_width, int i_height, char b_state,
   unsigned int i_colour, unsigned int i_alternate_colour);

int i_switch_draw(Display *h_display, int x_application_window, int i_screen, oswitch *h_switch);
