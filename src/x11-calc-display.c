/*
 * x11-calc-display.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * 10 Digit digit display functions.
 *
 * Contains  the functions needed to create and display a 10 digit 7 seven
 * segment display element.
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
 * On VMS compile with 'cc x11-calc-display'
 *
 * On UNIX Compile with 'gcc x11-calc-display.c -c'
 *
 * 14 Jul 13         - Initial verson - MT
 * 15 Dec 18         - Changed debug macro and added an error macro - MT
 * 23 Aug 20         - Removed the error macro - MT
 * 30 Aug 20         - Base  the number of display segments on the  maximum
 *                     number of digits in the display - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 26 Aug 21         - Updated  the display routines to use the contents of
 *                     the A and B registers - MT
 *                   - Reversed  order of nibbles in the registers to match
 *                     the  actual processor numbering (when  the  register
 *                     format  says  the  left  hand  nibble  is  the  most
 *                     significant means it!) - MT
 * 30 Aug 21         - Checks that the display is enabled when updating the
 *                     display- MT
 *                   - Removed unused variables - MT
 * 10 Sep 21         - Updated  lookup  table with codes  used  to  display
 *                     error messages - MT
 *  1 Oct 21         - Converted flags to Boolean variables - MT
 * 17 Oct 21         - Size of each digit depends on the number digits - MT
 *                   - Uses separate display decoders used depending on the
 *                     number of digits - MT
 * 19 Oct 21         - Fixed display of program steps Spice series - MT
 * 20 Oct 21         - Conditionally  compiles the display formatting  code
 *                     for the SPICE or WOODSTOCK series machines - MT
 * 29 Oct 21         - Fixed bug is SPICE display, only shows minus sign if
 *                     there are two consecutive commas in the display - MT
 * 16 Nov 21         - Can now define the horizontal and vertical scales to
 *                     independently of each other - MT
 * 20 Dec 21         - Updated display for HP67 - MT
 * 21 Dec 21         - Only displays warning messages if DEBUG is true - MT
 * 22 Dec 21         - Uses model numbers for conditional compilation - MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 29 Jan 22         - Added an optional bezel to the display - MT
 * 20 Feb 22         - Now supports the 10C, 11C, 12C, 15C and 16C - MT
 * 01 Mar 22         - Fixed digit 3 (11C, 12C, 15C and 16C) - MT
 * 01 Mar 22         - Fixed digits 4 and 10 (11C, 12C, 15C and 16C) tested
 *                     all digits now work as expected - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0020"
#define DATE           "29 Jan 22"
#define AUTHOR         "MT"

#include <stdlib.h>    /* malloc(), etc. */
#include <stdio.h>     /* fprintf(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-calc-button.h"
#include "x11-calc-switch.h"
#include "x11-calc-label.h"

#include "x11-calc.h"

#include "x11-calc-colour.h"
#include "x11-calc-segment.h"
#include "x11-calc-display.h"
#include "x11-calc-cpu.h"

#include "gcc-debug.h"

/*
 * display_create (index, text, left, top, width, height,
 *                margin, header, footer,
 *                foreground, backgroind, border)
 *
 * Allocates storage for a seven segment display, sets the properties and
 * returns a pointer to the display, or exits the program if there isn't
 * enough memory available.
 *
 */

odisplay *h_display_create(int i_index, int i_left, int i_top, int i_width, int i_height,
   int i_display_left, int i_display_top, int i_display_width, int i_display_height,
   unsigned int i_foreground, unsigned int i_background, unsigned int i_fill, unsigned int i_border){

   odisplay *h_display; /* Pointer to display. */
   int i_count;

   /* Attempt to allocate memory for a display. */
   if ((h_display = malloc(sizeof(*h_display)))==NULL) v_error("Memory allocation failed!");
   h_display->index = i_index;
   h_display->left = i_left;
   h_display->top = i_top;
   h_display->width = i_width;
   h_display->height = i_height;
   h_display->display_left = i_display_left;
   h_display->display_top = i_display_top;
   h_display->display_width = i_display_width;
   h_display->display_height = i_display_height;

#if defined(HP67) || defined (HP35) || defined (HP80) || defined (HP45) || defined (HP70) || defined(HP55)
   for (i_count = 0; i_count < DIGITS; i_count++) {
      //h_display->segment[i_count] = h_segment_create(0, 0, i_display_left + ((4 + 13 * i_count) * SCALE_WIDTH), i_display_top + (21 * SCALE_HEIGHT), 11 * SCALE_WIDTH, 33 * SCALE_HEIGHT, i_foreground, i_background); /* 15 Digit display */
      h_display->segment[i_count] = h_segment_create(0, 0, i_left + i_display_left + ((4 + 13 * i_count) * SCALE_WIDTH), i_top + i_display_top + (i_display_height - 33 * SCALE_HEIGHT) / 2, 11 * SCALE_WIDTH, 33 * SCALE_HEIGHT, i_foreground, i_background); /* 15 Digit display */
   }
#elif defined(HP31) || defined(HP32) || defined(HP33) || defined(HP34) || defined(HP37) || defined(HP38) ||  defined(HP10) ||  defined(HP11) ||  defined(HP12) ||  defined(HP15) ||  defined(HP16)
   for (i_count = 0; i_count < DIGITS; i_count++) {
      //h_display->segment[i_count] = h_segment_create(0, 0, i_display_left + ((3 + 18 * i_count) * SCALE_WIDTH) - 2, i_display_top + (18 * SCALE_HEIGHT), 16 * SCALE_WIDTH, 33 * SCALE_HEIGHT, i_foreground, i_background); /* 11 Digit display */
      h_display->segment[i_count] = h_segment_create(0, 0, i_left + i_display_left + ((3 + 18 * i_count) * SCALE_WIDTH) - 2, i_top + i_display_top + (i_display_height - 33 * SCALE_HEIGHT) / 2, 16 * SCALE_WIDTH, 33 * SCALE_HEIGHT, i_foreground, i_background); /* 11 Digit display */
   }
#else
   for (i_count = 0; i_count < DIGITS; i_count++) {
      h_display->segment[i_count] = h_segment_create(0, 0, i_left + i_display_left + ((5 + 16 * i_count) * SCALE_WIDTH), i_top + i_display_top + (i_display_height - 29 * SCALE_HEIGHT) /2, 14 * SCALE_WIDTH, 29 * SCALE_HEIGHT, i_foreground, i_background); /* 12 Digit display */
   }
#endif
   for (i_count = 0; i_count < DIGITS; i_count++) {
      h_display->segment[i_count]->mask = DISPLAY_SPACE;
   }
   h_display->foreground = i_foreground;
   h_display->background = i_background;
   h_display->fill = i_fill;
   h_display->border = i_border;
   return (h_display);
}

/*
 * display_draw (display, window, screen, display)
 *
 * Draws a seven segment display on the screen.
 *
 */

int i_display_draw(Display* x_display, int x_application_window, int i_screen, odisplay *h_display){

   int i_count;

   if (h_display->display_top != 0 || h_display->display_left != 0 || h_display->display_width != h_display->width || h_display->height != h_display->display_height)
   {
      XSetForeground(x_display, DefaultGC(x_display, i_screen), h_display->border); /* Set the border colour. */
      XFillRectangle(x_display, x_application_window, DefaultGC(x_display, i_screen), h_display->left, h_display->top, h_display->width, h_display->height); /* Fill in the border. */
   }

   XSetForeground(x_display, DefaultGC(x_display, i_screen), h_display->fill); /* Set the background colour. */
   XFillRectangle(x_display, x_application_window, DefaultGC(x_display, i_screen), h_display->left + h_display->display_left, h_display->top + h_display->display_top, h_display->display_width, h_display->display_height); /* Fill in the background. */

   /* Draw display segments. */
   for (i_count = 0; i_count < DIGITS; i_count++)
      if (!(h_display->segment[i_count] == NULL)) i_segment_draw(x_display, x_application_window, i_screen, h_display->segment[i_count]);
  return (True);
}

/*
 * display_update (display, window, screen, display)
 *
 * Updates the display based on the contents of the A and B registers.
 *
 */

int i_display_update(Display* x_display, int x_application_window, int i_screen, odisplay *h_display, oprocessor *h_processor){
#if defined(HP67)
   int i_count;
   static int c_digits [] =
   {
      DISPLAY_ZERO, DISPLAY_ONE, DISPLAY_TWO, DISPLAY_THREE, DISPLAY_FOUR, DISPLAY_FIVE, DISPLAY_SIX, DISPLAY_SEVEN,
      DISPLAY_EIGHT, DISPLAY_NINE, DISPLAY_r, DISPLAY_C, DISPLAY_o, DISPLAY_d, DISPLAY_E, DISPLAY_SPACE
   };

   for (i_count = 0; i_count < DIGITS; i_count++) {
      if (h_display->segment[i_count] != NULL) {
         if (h_processor->flags[DISPLAY_ENABLE] && h_processor->enabled)
         {
            switch (i_count) {
            case 0: /* Ignore */
               break;
            case 12: /* Sign */
               if (h_processor->reg[A_REG]->nibble[REG_SIZE - i_count] == 0x0F)
                  h_display->segment[i_count]->mask = DISPLAY_SPACE;
               else {
                  if (h_processor->reg[A_REG]->nibble[REG_SIZE - i_count] & 0x01)
                     h_display->segment[0]->mask = DISPLAY_SPACE;
                  else
                     h_display->segment[0]->mask = DISPLAY_MINUS;
                  if (h_processor->reg[A_REG]->nibble[REG_SIZE - i_count] >> 1)
                     h_display->segment[i_count]->mask = DISPLAY_MINUS;
                  else
                     h_display->segment[i_count]->mask = DISPLAY_SPACE;
               }
               break;
            default:
               switch (h_processor->reg[B_REG]->nibble[REG_SIZE - i_count] & 0x0F) {
               case 0x03: /* Decimal point */
                  h_display->segment[i_count]->mask = DISPLAY_DECIMAL;
                  break;
               case 0x0F:
               case 0x02:
               case 0x01: /* Space */
                     h_display->segment[i_count]->mask = DISPLAY_SPACE;
                  break;
               case 0x09:
               case 0x04:
               case 0x00: /* Number */
                  h_display->segment[i_count]->mask = c_digits[h_processor->reg[A_REG]->nibble[REG_SIZE - i_count]];
                  break;
               default:
                  debug(v_fprint_registers(stderr, h_processor);
                  v_warning("Unexpected output format specified in %s line : %d\n", __FILE__, __LINE__));
               }
            }
         }
         else
            h_display->segment[i_count]->mask = DISPLAY_SPACE;
      }
   }
#elif defined (HP35) || defined (HP80) || defined (HP45) || defined (HP70) || defined(HP55)
   int i_count, i_offset;
   static int c_digits [] =
   {
      DISPLAY_ZERO, DISPLAY_ONE, DISPLAY_TWO, DISPLAY_THREE, DISPLAY_FOUR, DISPLAY_FIVE,
      DISPLAY_SIX, DISPLAY_SEVEN, DISPLAY_EIGHT, DISPLAY_NINE
   };

   i_offset = REG_SIZE - 1;
   for (i_count = 0; i_count < DIGITS; i_count++) {
      if (h_display->segment[i_count] != NULL) {
         if (h_processor->flags[DISPLAY_ENABLE] && h_processor->enabled)
         {
            switch (i_count) {
            case 0:
               if (h_processor->reg[A_REG]->nibble[REG_SIZE - i_count - 1] == 9)
                  h_display->segment[i_count]->mask = DISPLAY_MINUS;
               else
                  h_display->segment[i_count]->mask = DISPLAY_SPACE;
            break;
            case 12:
               i_offset = 2;
               if ((h_processor->reg[A_REG]->nibble[REG_SIZE - i_count] == 9) &&
                  (h_processor->reg[B_REG]->nibble[REG_SIZE - i_count] == 0))
                  h_display->segment[i_count]->mask = DISPLAY_MINUS;
               else
                  h_display->segment[i_count]->mask = DISPLAY_SPACE;
            break;
            default:
               switch (h_processor->reg[B_REG]->nibble[REG_SIZE - i_count] & 0x0F) {
               case 2: /* Decimal point */
                  h_display->segment[i_count]->mask = DISPLAY_DECIMAL;
                  i_offset++;
                  break;
               case 9: /* Space */
                  h_display->segment[i_count]->mask = DISPLAY_SPACE;
                  break;
               case 0: /* Number */
                  if (i_offset >= 0) h_display->segment[i_count]->mask = c_digits[h_processor->reg[A_REG]->nibble[i_offset]];
                  break;
               default:
                  debug(v_fprint_registers(stderr, h_processor);
                  v_warning("Unexpected output format specified in %s line : %d\n", __FILE__, __LINE__));
               }
            }
         }
         else
            h_display->segment[i_count]->mask = DISPLAY_SPACE;
      }
      i_offset--;
   }
#elif defined(HP31) || defined(HP32) || defined(HP33) || defined(HP34) || defined(HP37) || defined(HP38)
   int i_count;
   static int c_digits [] =
   {
      DISPLAY_ZERO, DISPLAY_ONE, DISPLAY_TWO, DISPLAY_THREE, DISPLAY_FOUR, DISPLAY_FIVE, DISPLAY_SIX, DISPLAY_SEVEN,
      DISPLAY_EIGHT, DISPLAY_NINE, DISPLAY_r, DISPLAY_c, DISPLAY_o, DISPLAY_P, DISPLAY_E, DISPLAY_SPACE
   };

   for (i_count = 0; i_count < DIGITS; i_count++)
   {
      if (h_display->segment[i_count] != NULL)
      {
         if (h_processor->flags[DISPLAY_ENABLE] && h_processor->enabled)
         {
            if (i_count == 0)
               h_display->segment[i_count]->mask = DISPLAY_SPACE;
            else
               h_display->segment[i_count]->mask = c_digits[h_processor->reg[A_REG]->nibble[REG_SIZE - 1 - i_count]];
            if ((h_processor->reg[B_REG]->nibble[REG_SIZE - 1 - i_count] & 0x04) != 0)
            {
               if (i_count > 1)
               {
                  if (h_processor->reg[A_REG]->nibble[REG_SIZE - 1 - i_count]== 0x9)
                     h_display->segment[i_count]->mask = DISPLAY_MINUS;
                  else
                     h_display->segment[i_count]->mask = DISPLAY_SPACE;
               }
               else
                  if (i_count == 1) h_display->segment[0]->mask = DISPLAY_MINUS; /* Negative value */
            }
            if ((h_processor->reg[B_REG]->nibble[REG_SIZE - 1 - i_count] & 0x02) != 0)
            {
               if ((h_display->segment[i_count]->mask != DISPLAY_COMMA)
                  && (h_display->segment[i_count]->mask != DISPLAY_SPACE)
                  && (h_display->segment[i_count]->mask != DISPLAY_MINUS))
                  h_display->segment[i_count]->mask = h_display->segment[i_count]->mask | DISPLAY_COMMA;
               if ((i_count == 1) && ((h_processor->reg[B_REG]->nibble[REG_SIZE - 3] & 0x02) != 0))
                  h_display->segment[0]->mask = DISPLAY_MINUS; /* Self test */

            }
            if ((h_processor->reg[B_REG]->nibble[REG_SIZE - 1 - i_count] & 0x01) != 0)
            {
               if (i_count == 0)
                  h_display->segment[i_count]->mask = DISPLAY_SPACE;
               else
                  h_display->segment[i_count]->mask = h_display->segment[i_count]->mask | DISPLAY_DECIMAL;
            }
         }
         else
            h_display->segment[i_count]->mask = DISPLAY_SPACE;
      }
   }
#elif defined(HP10) || defined(HP11) || defined(HP12) || defined(HP15) || defined(HP16)
   /*
    * Unlike the earlier models which display the contents of the processor
    * registers  directly, the voyager series use two memory registers ([9]
    * and [10]) to hold that state of every segment (and annunciator).
    *
    * When updating the display it is necessary to look up the state of all
    * the segments individually, using the table below to map the values in
    * the two memory registers to each segment in each digit...
    *
    */
   int i_count, i_counter;
#if defined(HP10) /* Uses a different display to the earlier models so the look up table is different */
   static int i_map [DIGITS][9][3] =
   { /*      A             B             C             D             E             F             G             H             I                    */
     /*  r   n   b     r   n   b     r   n   b     r   n   b     r   n   b     r   n   b     r   n   b     r   n   b     r   n   b                */
      {{ 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, {10, 11,  4}, { 0,  0,  0}, { 0,  0,  0}}, /* Digit 0  */
      {{10, 10,  8}, {10, 10,  4}, {10, 11,  1}, {10, 11,  8}, {10, 11,  2}, {10, 10,  2}, {10, 10,  1}, {10, 12,  2}, {10, 12,  1}}, /* Digit 1  */
      {{10,  3,  8}, {10,  3,  4}, {10,  4,  1}, {10, 12,  8}, {10,  4,  2}, {10,  3,  2}, {10,  3,  1}, {10, 13,  2}, {10, 13,  1}}, /* Digit 2  */
      {{10,  2,  2}, {10,  2,  1}, {10,  2,  4}, {10, 13,  8}, {10,  2,  8}, {10,  1,  8}, {10,  1,  4}, {10,  6,  2}, {10,  6,  1}}, /* Digit 3  */
      {{10,  7,  2}, {10,  7,  1}, {10,  7,  4}, {10,  4,  8}, {10,  7,  8}, {10,  6,  8}, {10,  6,  4}, {10,  8,  2}, {10,  8,  1}}, /* Digit 4  */
      {{10,  9,  2}, {10,  9,  1}, {10,  9,  4}, {10,  5,  2}, {10,  9,  8}, {10,  8,  8}, {10,  8,  4}, {10,  5,  8}, {10,  5,  4}}, /* Digit 5  */
      {{ 9, 13,  2}, { 9, 13,  1}, { 9, 13,  4}, { 9,  1,  8}, { 9, 13,  8}, { 9, 12,  8}, { 9, 12,  4}, { 9, 12,  2}, { 9, 12,  1}}, /* Digit 6  */
      {{ 9, 11,  2}, { 9, 11,  1}, { 9, 11,  4}, { 9,  2,  2}, { 9, 11,  8}, { 9, 10,  8}, { 9, 10,  4}, { 9, 10,  2}, { 9, 10,  1}}, /* Digit 7  */
      {{ 9,  9,  2}, { 9,  9,  1}, { 9,  9,  4}, { 9,  2,  8}, { 9,  9,  8}, { 9,  8,  8}, { 9,  8,  4}, { 9,  4,  2}, { 9,  4,  1}}, /* Digit 8  */
      {{ 9,  7,  8}, { 9,  7,  4}, { 9,  8,  1}, { 9,  3,  8}, { 9,  8,  2}, { 9,  7,  2}, { 9,  7,  1}, { 9,  3,  2}, { 9,  3,  1}}, /* Digit 9  */
      {{ 9,  6,  2}, { 9,  6,  1}, { 9,  6,  4}, { 9,  4,  8}, { 9,  6,  8}, { 9,  5,  8}, { 9,  5,  4}, { 9,  5,  2}, { 9,  5,  1}}  /* Digit 10 */
   };
#else
   static int i_map [DIGITS][9][3] =
   { /*      A             B             C             D             E             F             G             H             I                    */
     /*  r   n   b     r   n   b     r   n   b     r   n   b     r   n   b     r   n   b     r   n   b     r   n   b     r   n   b                */
      {{ 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 9, 11,  4}, { 0,  0,  0}, { 0,  0,  0}}, /* Digit 0  */
      {{ 9,  5,  2}, { 9,  5,  1}, { 9,  4,  4}, { 9, 11,  8}, { 9,  4,  8}, { 9,  5,  8}, { 9,  5,  4}, { 9,  9,  8}, { 9,  9,  4}}, /* Digit 1  */
      {{ 9,  6,  8}, { 9,  6,  4}, { 9,  6,  1}, { 9,  4,  2}, { 9,  6,  2}, { 9,  7,  2}, { 9,  7,  1}, { 9,  3,  8}, { 9,  3,  4}}, /* Digit 2  */
      {{ 9, 12,  8}, { 9, 12,  4}, { 9, 12,  1}, { 9,  3,  2}, { 9, 12,  2}, { 9, 13,  2}, { 9, 13,  1}, { 9, 13,  8}, { 9, 13,  4}}, /* Digit 3  */
      {{ 9,  8,  2}, { 9,  8,  1}, { 9,  7,  4}, { 9,  2,  2}, { 9,  7,  8}, { 9,  8,  8}, { 9,  8,  4}, { 9,  9,  2}, { 9,  9,  1}}, /* Digit 4  */
      {{ 9, 10,  8}, { 9, 10,  4}, { 9, 10,  1}, { 9,  1,  8}, { 9, 10,  2}, { 9, 11,  2}, { 9, 11,  1}, { 9,  2,  8}, { 9,  2,  4}}, /* Digit 5  */
      {{10,  2,  8}, {10,  2,  4}, {10,  2,  1}, {10,  3,  8}, {10,  2,  2}, {10,  3,  2}, {10,  3,  1}, {10,  4,  2}, {10,  4,  1}}, /* Digit 6  */
      {{10,  5,  2}, {10,  5,  1}, {10,  4,  4}, {10,  1,  8}, {10,  4,  8}, {10,  5,  8}, {10,  5,  4}, {10,  6,  2}, {10,  6,  1}}, /* Digit 7  */
      {{10,  7,  2}, {10,  7,  1}, {10,  6,  4}, {10,  9,  8}, {10,  6,  8}, {10,  7,  8}, {10,  7,  4}, {10,  9,  2}, {10,  9,  1}}, /* Digit 8  */
      {{10, 11,  8}, {10, 11,  4}, {10, 11,  1}, {10,  8,  2}, {10, 11,  2}, {10, 12,  2}, {10, 12,  1}, {10,  8,  8}, {10,  8,  4}}, /* Digit 9  */
      {{10, 13,  2}, {10, 13,  1}, {10, 12,  4}, {10, 10,  2}, {10, 12,  8}, {10, 13,  8}, {10, 13,  4}, {10, 10,  8}, {10, 10,  4}}  /* Digit 10 */
   };
#endif
   for (i_count = 0; i_count < DIGITS; i_count++)
   {
      if (h_display->segment[i_count] != NULL)
      {
         h_display->segment[i_count]->mask = DISPLAY_SPACE;
         if (h_processor->flags[DISPLAY_ENABLE] && h_processor->enabled)
         {
            for (i_counter = 0; i_counter < 9; i_counter++)
            {
               if (i_map[i_count][i_counter][0] > 0)
                  if ((h_processor->mem[i_map[i_count][i_counter][0]]->nibble[i_map[i_count][i_counter][1]] & i_map[i_count][i_counter][2]))
                     h_display->segment[i_count]->mask |= (1 << i_counter); /* Mask determines which segments are on */
            }
         }
      }
   }
#else
   int i_count;
   static int c_digits [] =
   {
      DISPLAY_ZERO, DISPLAY_ONE, DISPLAY_TWO, DISPLAY_THREE, DISPLAY_FOUR, DISPLAY_FIVE, DISPLAY_SIX, DISPLAY_SEVEN,
      DISPLAY_EIGHT, DISPLAY_NINE, DISPLAY_r, DISPLAY_c, DISPLAY_o, DISPLAY_P, DISPLAY_E, DISPLAY_SPACE
   };

   for (i_count = 0; i_count < DIGITS; i_count++) {
      if (h_display->segment[i_count] != NULL) {
         if (h_processor->flags[DISPLAY_ENABLE] && h_processor->enabled) {
            h_display->segment[i_count]->mask = c_digits[h_processor->reg[A_REG]->nibble[REG_SIZE - 1 - i_count]];
            switch (h_processor->reg[B_REG]->nibble[REG_SIZE - 1 - i_count] & 0x07) {
            case 0x02: /* Sign */
               if (h_processor->reg[A_REG]->nibble[REG_SIZE - 1 - i_count])
                  h_display->segment[i_count]->mask = DISPLAY_MINUS;
               else
                  h_display->segment[i_count]->mask = DISPLAY_SPACE;
               break;
            case 0x01: /* Number and decimal point */
               h_display->segment[i_count]->mask = h_display->segment[i_count]->mask | DISPLAY_DECIMAL;
            }
         }
         else
            h_display->segment[i_count]->mask = DISPLAY_SPACE;
      }
   }
#endif
   return (True);
}
