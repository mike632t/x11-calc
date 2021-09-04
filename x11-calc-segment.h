/*
 * x11-calc-segment.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * 7 segment display digit functions.
 *
 * Contains  the  functions needed to create and display  a  seven  segment
 * digit element based on the mask value.
 *
 * Contains the type definations and function prototypes used to create and
 * display a seven segment display element.
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
 * 14 Jul 13         - Initial version - MT
 *
 */

#define SEG_A          0x0001
#define SEG_B          0x0002
#define SEG_C          0x0004
#define SEG_D          0x0008
#define SEG_E          0x0010
#define SEG_F          0x0020
#define SEG_G          0x0040
#define SEG_DECIMAL    0x0080
#define SEG_COMMA      0x0100
#define SEG_COLON      0x0200

typedef struct { /* Calculator segment structure. */
   int index;
   int mask;
   int left;
   int top;
   int width;
   int height;
   unsigned int foreground;
   unsigned int background;
} osegment;

osegment *h_segment_create(int i_index, int i_mask, int i_left, int i_top,
   int i_width, int i_height, unsigned int i_foreground, unsigned int i_background);

int i_segment_draw(Display *h_display, int x_application_window, int i_screen,osegment *h_segment);


