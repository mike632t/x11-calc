/*
 * x11-calc-button.h - RPN (Reverse Polish) calculator simulator. 
 *
 * Copyright(C) 2013   MT
 * 
 * Button functions.
 *      
 * Defines the routines and data structures used to draw a button.
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
 * 02 Jul 13         - Initial version - MT
 *
 */
 
typedef struct { /* Calculator button structure. */
   int index;
   char* text;
   char* label;
   char* alternate;
   XFontStruct* text_font; /* Pointer to font. */
   XFontStruct* label_font; /* Pointer to shifted font. */
   XFontStruct* alternate_font; /* Pointer to alternate font. */
   int left;
   int top; 
   int width;
   int height; 
   int state;
   unsigned int colour;
} o_button;

o_button* h_button_pressed(o_button* h_button, int i_xpos, int i_ypos);

o_button* h_button_create(int i_index, char* s_text,char* s_label ,char* s_alternate, 
   XFontStruct* h_normal_font, XFontStruct* h_shift_font, XFontStruct* h_alternate_font, 
   int i_left, int i_top, int i_width, int i_height, int i_state,
   unsigned int i_colour);
   
int i_button_draw(Display* h_display, int x_application_window, int i_screen,o_button* h_button);
