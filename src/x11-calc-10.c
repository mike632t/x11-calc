/*
 * x11-calc-10.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2021   MT
 *
 * Model specific functions.
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
 * 10 Oct 21   0.1   - Initial version - MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 31 Jan 22         - Added ROM - MT
 *
 * TO DO :           -
 */

#define VERSION        "0.1"
#define BUILD          "0002"
#define DATE           "10 Oct 21"
#define AUTHOR         "MT"

#include <stdarg.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* getenv(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-calc-font.h"
#include "x11-calc-button.h"
#include "x11-calc-switch.h"
#include "x11-calc-label.h"
#include "x11-calc-colour.h"

#include "x11-calc-cpu.h"

#include "x11-calc.h"

#include "gcc-debug.h"

oregister o_mem[MEMORY_SIZE];

int i_rom[ROM_SIZE];

void v_init_labels(olabel *h_label[]) {
   int i_height = h_small_font->ascent + h_small_font->descent;
   h_label[0] = h_label_create(001, "CLEAR" , h_alternate_font, KBD_LEFT + 2 * (KEY_WIDTH + KEY_GAP),
      KBD_TOP + KEY_HEIGHT +  2 * (KBD_ROW - i_height) + h_small_font->descent,
      2 * (KEY_WIDTH + KEY_GAP) + KEY_WIDTH, i_height, YELLOW, BACKGROUND);
}

void v_init_buttons(obutton *h_button[]) {
   int i_left, i_top, i_count;

   /* Define top row of keys. */
   i_top = KBD_TOP + KEY_HEIGHT;
   i_left = KBD_LEFT;
   i_count = 0;
   h_button[i_count++] = h_button_create(00023, 000, "/\xaf", "x\xb2", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00063, 000, "eX", "ln", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00163, 000, "10x", "log", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00303, 000, "yX", "-HMS", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00203, 000, "1/x", "-H", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00202, 'c', "CHS", "\x1c", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00302, '7', "7", "FIX", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00162, '8', "8", "SCI", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00062, '9', "9", "ENG", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00022, '/', "\xf7", "X\x1a\x59", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);

   /* Define second row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00020, 000, "%", "-R", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00060, 000, "GTO", "-P", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00160, 000, "SIN", "SIN\xaf\xb9", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00300, 000, "COS", "COS\xaf\xb9", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00200, 000, "TAN", "TAN\xaf\xb9", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00207, 'e', "EEX", "n!", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00307, '4', "4", "DEG", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00167, '5', "5", "RAD", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00067, '6', "6", "GRD", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00027, '*', "\xd7", "X=0", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);

   /* Define third row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00021, 000, "R/S", "PSE", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00061, 000, "SST", "BST", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00161, 000, "Rv", "PRGM", "", "", h_normal_font, h_alternate_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00301, 000, "X-Y", "REG", "", "", h_normal_font, h_alternate_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00201, 033, "CLx", "PREFIX", "", "", h_normal_font, h_alternate_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00204, 015, "ENTER", "LSTx", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT + KBD_ROW, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00304, '1', "1", "x,r", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00164, '2', "2", "y,r", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00064, '3', "3", "L.R", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00024, '-', "-", "-RAD", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);


   /* Define fourth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00030, 000, "ON", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00070, 'f', "f", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, YELLOW, BACKGROUND, YELLOW, YELLOW);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00170, 000, "P/R", "MEM", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00310, 000, "STO", "INT", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00210, 000, "RCL", "FRAC", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00305, '0', "0", "x", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00165, '.', ".", "s", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00065, 000, "E+", "E-", "", "",  h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00025, '+', "+", "-DEG", "", "",  h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
}
