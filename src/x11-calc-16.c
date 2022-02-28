/*
 * x11-calc-16.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2022   MT
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
 * 30 Jan 22   0.1   - Initial version (derived from x11-calc-10.c) - MT
 *
 * TO DO :           -
 */

#define VERSION        "0.1"
#define BUILD          "0001"
#define DATE           "30 Jan 22"
#define AUTHOR         "MT"

#include <stdarg.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* getenv(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-calc-font.h"
#include "x11-calc-button.h"
#include "x11-calc-label.h"
#include "x11-calc-colour.h"

#include "x11-calc-cpu.h"

#include "x11-calc.h"

#include "gcc-debug.h"

oregister o_mem[MEMORY_SIZE];

int i_rom[ROM_SIZE];

void v_init_labels(olabel *h_label[]) {
   int i_height = h_small_font->ascent + h_small_font->descent;
   h_label[0] = h_label_create(000, "SHOW" , h_small_font, KBD_LEFT + 2 * (KEY_WIDTH + KEY_GAP),
      KBD_TOP + KEY_HEIGHT +  KBD_ROW - i_height - 1,
      3 * (KEY_WIDTH + KEY_GAP) + KEY_WIDTH, i_height, YELLOW, BACKGROUND);

   h_label[1] = h_label_create(001, "CLEAR" , h_alternate_font, KBD_LEFT + 2 * (KEY_WIDTH + KEY_GAP),
      KBD_TOP + KEY_HEIGHT +  2 * (KBD_ROW - i_height) + h_alternate_font->descent,
      2 * (KEY_WIDTH + KEY_GAP) + KEY_WIDTH, i_height, YELLOW, BACKGROUND);

   h_label[2] = h_label_create(002, "SET COMPL" , h_alternate_font, KBD_LEFT + 6 * (KEY_WIDTH + KEY_GAP),
      KBD_TOP + KEY_HEIGHT +  2 * (KBD_ROW - i_height) + h_alternate_font->descent,
      2 * (KEY_WIDTH + KEY_GAP) + KEY_WIDTH, i_height, YELLOW, BACKGROUND);

}

void v_init_buttons(obutton *h_button[]) {
   int i_left, i_top, i_count;

   /* Define top row of keys. */
   i_top = KBD_TOP + KEY_HEIGHT;
   i_left = KBD_LEFT;
   i_count = 0;
   h_button[i_count++] = h_button_create(00023, 'A', "A", "SL", "", "LJ", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00063, 'B', "B", "SR", "", "ASR", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00163, 'C', "C", "RL", "", "RLC", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00303, 'D', "D", "RR", "", "RRC", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00203, 'E', "E", "RLn", "", "RLCn", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00202, 'F', "F", "RRn", "", "RRCn", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00302, '7', "7", "MASKL", "", "# B", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00162, '8', "8", "MASKR", "", "ABS", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00062, '9', "9", "RMD", "", "DBLR", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00022, '/', "\xf7", "XOR", "", "DBL\xf7", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);

   /* Define second row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00020, 000, "GSB", "X-(i)", "", "RTN", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00060, 000, "GTO", "X-I", "", "LBL", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00160, 000, "HEX", "", "", "DSZ", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00300, 000, "DEC", "", "", "ISZ", h_normal_font, h_alternate_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00200, 000, "OCT", "", "", "/\xaf", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00207, 'e', "BIN", "", "", "1/x", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00307, '4', "4", "SB", "", "SF", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00167, '5', "5", "CB", "", "CF", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00067, '6', "6", "B ?", "", "F ?", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00027, '*', "\xd7", "AND", "", "DBL\xd7", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);

   /* Define third row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00021, 000, "R/S", "(i)", "", "P/R", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00061, ' ', "SST", "I", "", "BST", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00161, 000, "Rv", "PRGM", "", "R^", h_normal_font, h_alternate_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00301, 000, "X-Y", "REG", "", "PSE", h_normal_font, h_alternate_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00201, 033, "BSP", "PREFIX", "", "CLx", h_normal_font, h_alternate_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00204, 015, "ENTER", "WINDOW", "", "LSTx", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT + KBD_ROW, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00304, '1', "1", "1's", "", "X\x1a\x59", h_normal_font, h_alternate_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00164, '2', "2", "2's", "", "X<0", h_normal_font, h_alternate_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00064, '3', "3", "UNSIGN", "", "X>Y", h_normal_font, h_alternate_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00024, '-', "-", "NOT", "", "X>0", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);


   /* Define fourth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00030, 000, "ON", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00070, 'f', "f", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, YELLOW, BACKGROUND, YELLOW, YELLOW);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00170, 'g', "g", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, LIGHT_BLUE, BACKGROUND, LIGHT_BLUE, LIGHT_BLUE);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00310, 000, "STO", "WSIZE", "", "<", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00210, 000, "RCL", "FLOAT", "", ">", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00305, '0', "0", "MEM", "", "X\x1d\x59", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00165, '.', ".", "STATUS", "", "X\x1d\x30", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00065, 'c', "CHS", "EEX", "", "X=Y",  h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00025, '+', "+", "OR", "", "X=0",  h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
}
