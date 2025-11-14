/*
 * x11-calc-card.c - RPN (Reverse Polish) calculator simulator.
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
 * 08 Nov 25            - Added function labels - MT
 *                      - Modified names of colour properties and added two
 *                        more to store the default colours - MT
 *                      - Added card_reset() to restore the default colours
 *                        and reset label text - MT
 *                      - Fixed resize() - MT
 * 09 Nov 25            - Removed redundant linked list routines - MT
 * 11 Nov 25            - Tidied up layout - MT
 * 12 Nov 25            - Use a label to display program name - MT
 *                      - Allow colour of all labels to be modified - MT
 *
 * To Do                - Use a label to display the file name (allows each
 *                        model some control over the position and style).
 *
 */

#define NAME           "x11-calc-card"
#define BUILD          "0009"
#define DATE           "12 Nov 2025"
#define AUTHOR         "MT"

#include <errno.h>     /* errno */

#include <string.h>    /* strlen(), memcpy() etc */
#include <stdio.h>     /* fprintf(), etc */
#include <stdlib.h>    /* malloc(), etc */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc */
#include <X11/Xutil.h> /* XSizeHints etc */

#include "x11-calc-messages.h"
#include "x11-calc-errors.h"

#include "x11-calc-font.h"
#include "x11-calc-label.h"
#include "x11-calc-switch.h"
#include "x11-calc-button.h"
#include "x11-calc-card.h"

#include "x11-calc-cpu.h"

#include "gcc-debug.h"

/*
 * card_create (index, text, left, top, width, height, foreground,
 *             background, state)
 *
 * Allocates  storage for a card object, sets the properties and returns  a
 * pointer  to the card, or exits the program if there isn't enough  memory
 * available.
 *
 * Does NOT create the labels - these are created by model specific init_card() routine.
 *
 */

#if defined(HP67)
struct ocard *h_card_create(int i_index, XFontStruct *h_font, int i_left, int i_top, int i_width, int i_height,
   unsigned int i_colour, unsigned int i_label_colour, unsigned int i_function_colour, int i_state)
{
   struct ocard *h_card;  /* Pointer to card */
   int i_count;

   if ((h_card = malloc(sizeof(*h_card)))==NULL)  /* Attempt to allocate memory for a display */
      v_error(errno, h_err_memmory_alloc, __FILE__, __LINE__);
   h_card->index = i_index;
   h_card->file = NULL;
   h_card->filename = NULL;
   h_card->records = 0;
   h_card->font = h_font;
   h_card->position.x = i_left;
   h_card->position.y = i_top;
   h_card->position.width = i_width;
   h_card->position.height = i_height;

   h_card->geometry = h_card->position;  /* Save position */

   for (i_count = 1; i_count < sizeof(h_card->label) / sizeof(h_card->label[0]); i_count++)
      h_card->label[i_count] = NULL;

   h_card->colour = i_colour;
   h_card->label_colour = i_label_colour;
   h_card->function_colour = i_function_colour;
   h_card->background = h_card->colour;
   h_card->foreground = h_card->label_colour;
   h_card->alternate = h_card->function_colour;
   h_card->state = i_state;
   return(h_card);
}

void i_card_reset(struct ocard *h_card)
{
   int i_label;

   for (i_label = 0; i_label < sizeof(h_card->label) / sizeof(h_card->label[0]); i_label++)
   {
      if (h_card->label[i_label])
      {
         free(h_card->label[i_label]->text);
         if (i_label > 0) h_card->label[i_label]->text = NULL;
         h_card->label[i_label]->state = True;
      }
   }
   h_card->colour = h_card->background;  /* Reset colours */
   h_card->label_colour = h_card->foreground;
   h_card->function_colour = h_card->alternate;
}

void i_card_resize(struct ocard *h_card, float f_scale)
{
   int i_label;

   h_card->position.x = h_card->geometry.x * f_scale;
   h_card->position.y = h_card->geometry.y * f_scale;
   h_card->position.width = h_card->geometry.width * f_scale;
   h_card->position.height = h_card->geometry.height * f_scale;

   for (i_label = 0; i_label < sizeof(h_card->label) / sizeof(h_card->label[0]); i_label++)
      if (h_card->label[i_label])
         i_label_resize(h_card->label[i_label], f_scale);
}

int i_card_draw(Display *h_display, int x_application_window, int i_screen, struct ocard *h_card)
{
   int i_count;

   if (h_card != NULL)
   {
      if (h_card->state)  /* Only draw text if enabled */
      {
         XSetForeground(h_display, DefaultGC(h_display, i_screen), h_card->colour);
         XFillRectangle(h_display, x_application_window, DefaultGC(h_display, i_screen),
         h_card->position.x, h_card->position.y , h_card->position.width, h_card->position.height);  /* Always fill in background */
         for (i_count = 0; i_count < sizeof(h_card->label) / sizeof(h_card->label[0]); i_count++)
         {
            if (h_card->label[i_count] != NULL)  /* Only draw each labels if not blank */
            {
               i_label_draw(h_display, x_application_window,i_screen, h_card->label[i_count]);
            }
         }
      }
   }
   return(True);
}
#endif
