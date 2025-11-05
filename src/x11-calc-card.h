/*
 * x11-calc-card.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2025   MT
 *
 * Card functions (not including read record).
 *
 * Contains  the  functions needed to create a card object and render it.
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
 * 31 Oct 25  0.1.0001  - Initial version - MT
 *
 */
struct onode /* Linked list node */
{
    void  *data;
    struct onode *next;
};

struct ocard /* Card structure */
{
   int index;
   FILE * file;                  /* Open card file (NULL = not opened) - KJC */
   char* filename;               /* Filename */
   int records;                  /* Number of records read/written, cleared at open - KJC */
   char* text;                   /* Label */
   XRectangle position;          /* Current position */
   XRectangle geometry;          /* Original position */
   XFontStruct* font;            /* Font */
   struct onode *list;           /* List */
   unsigned int foreground;      /* Foreground colour */
   unsigned int background;      /* Background colour */
   int state;                    /* Enabled */
};

struct ocard *h_card_pressed(struct ocard *h_card, int i_xpos, int i_ypos);

struct ocard *h_card_create(int i_index, char* s_text, XFontStruct *h_font,
   int i_left, int i_top, int i_width, int i_height,
   unsigned int i_foreground, unsigned int i_background, int i_state);

int i_card_resize(struct ocard *h_card, float f_scale);

int i_card_draw(Display *h_display, int x_application_window, int i_screen, struct ocard *h_card);
