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
 * 12 Mar 22         - Implemented a state property allowing the appearance
 *                     of the label to be changed (hidden, or no line) - MT
 * 28 Oct 25         - Added alignment and text attributes - MT
 * 31 Oct 25         - Tidied up label object properties - MT
 *
 */

#define LABEL_ALIGN_CENTER 0
#define LABEL_ALIGN_LEFT   1
#define LABEL_ALIGN_RIGHT  2

struct olabel  /* Calculator label structure */
{
   int index;
   XRectangle position;          /* Current label position */
   XRectangle geometry;          /* Original label position */
   char* text;                   /* Text */
   XFontStruct* font;            /* Font */
   unsigned int foreground;      /* Foreground colour */
   unsigned int background;      /* Background colour */
   int attributes;               /* Strike-through */
   int alignment;                /* Centre left or right */
   int state;                    /* Enabled */
};

struct olabel *h_label_pressed(struct olabel *h_label, int i_xpos, int i_ypos);

struct olabel *h_label_create(int i_index, char* s_text, XFontStruct *h_font,
   int i_left, int i_top, int i_width, int i_height,
   unsigned int i_colour, unsigned int i_background, int i_alignmanet,
   int i_attributes, int i_state);

int i_label_resize(struct olabel *h_label, float f_scale);

int i_label_draw(Display *h_display, int x_application_window, int i_screen, struct olabel *h_label);
