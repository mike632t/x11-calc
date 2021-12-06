/*
 * x11-calc-33.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2018   MT
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
 * 13 Jun 13   0.1   - Initial version - MT
 * 10 Mar 14         - Changed indexes to BCD hex values - MT
 * 10 Dec 18         - Alternate  function key now LIGHT_BLUE, allowing  it
 *                     to be a different color to the alternate text - MT
 * 31 Aug 20         - Made  version(), about(), and error() model specific
 *                     so that the name reflects the model number - MT
 *                   - Modified version() to display a leading zero in  the
 *                     date instead of a space - MT
 * 26 Sep 21         - Replaced BCD hex key codes with octal values - MT
 *                   - Added shortcut key definitions - MT
 * 18 Oct 21         - Added key codes and ROM contents - MT
 * 20 Oct 21         - Assigned space bar to single step - MT
 * 02 Nov 21         - Allows size of the window to be changed by modifying
 *                     the value of SCALE at compile time - MT
 * 23 Nov 21         - Added the alternate function text - MT
 * 06 Dec 21         - Simplified keyboard constants - MT
 */

#define VERSION        "0.1"
#define BUILD          "0011"
#define DATE           "23 Nov 21"
#define AUTHOR         "MT"

#define DEBUG 0

#include <stdarg.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* getenv(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-calc-font.h"
#include "x11-calc-button.h"
#include "x11-calc-switch.h"
#include "x11-calc-colour.h"

#include "x11-calc-cpu.h"

#include "x11-calc.h"

#include "gcc-debug.h"

oregister o_mem[MEMORY_SIZE];

void v_init_keypad(obutton *h_button[], oswitch *h_switch[]) {
   int i_left, i_top, i_height, i_width, i_count;

   /* Define the switches. */
   i_top = KBD_TOP;
   i_left = KBD_LEFT;
   i_height = SWITCH_HEIGHT;
   i_width = (2 * KEY_SMALL + KEY_GAP);
   h_switch[0] = h_switch_create(00000, "OFF", "ON ", h_alternate_font, i_left, i_top, i_width, i_height, True, MID_GREY, DARK_GREY);
   i_left += (3 * (KEY_SMALL + KEY_GAP));
   h_switch[1] = h_switch_create(00000, "PRGM", "RUN", h_alternate_font, i_left, i_top, i_width, i_height, True, MID_GREY, DARK_GREY);

   /* Define top row of keys. */
   i_top += KEY_HEIGHT;
   i_left = KBD_LEFT;
   i_height = KEY_HEIGHT;
   i_width = KEY_SMALL;
   i_count = 0;
   h_button[i_count++] = h_button_create(00244, 'a', "A", "a", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00243, 'b', "B", "b", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00242, 'c', "C", "c", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00241, 'd', "D", "d", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00240, 'e', "E", "e", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);

   /* Define second row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00224, 000, "E+", "x ", "s", "E-", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00223, 000, "GTO", "GSB", "f", "RTN", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00222, 000, "DSP", "FIX", "SCI", "ENG", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00221, 000, "(i)", "RND", "", "X-I", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLACK, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00220, 000, "SST", "LBL", "f", "BST", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);

   /* Define third row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00024, 000, "f", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, YELLOW, BACKGROUND, YELLOW, YELLOW);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00023, 000, "g", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_BLUE, BACKGROUND, LIGHT_BLUE, LIGHT_BLUE);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00022, 000, "STO", "DSZ", "(i) ", "STO I", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00021, 000, "RCL", " ISZ", "(i)", "RCL I", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00020, 000, "h", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BLACK, BLACK, BLACK);

   /* Define fourth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = 2* KEY_SMALL + KEY_GAP;
   h_button[i_count++] = h_button_create(00063, 015, "ENTER", "W/DATA", "MERGE", "        DEG", h_normal_font, h_small_font, h_alternate_font, KBD_LEFT, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (2 * (KEY_SMALL + KEY_GAP));
   i_width = KEY_SMALL;
   h_button[i_count++] = h_button_create(00062, 'c', "CHS", "P-S", "", "RAD", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLACK, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00061, 'e', "EEX", "CLREG", "", "GRD", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLACK, BLACK);
   i_left += (KEY_SMALL + KEY_GAP);
   h_button[i_count++] = h_button_create(00060, 033, "CLX", "CLPRGM", "", "DEL", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLACK, BLACK);

   /* Define fifth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = KEY_SMALL;
   h_button[i_count++] = h_button_create(00103, '-', "-", "X=0", "X=Y", "SF", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + 2 * KEY_GAP + 1);
   i_width = KEY_NUMERIC;
   h_button[i_count++] = h_button_create(00102, '7', "7", "LN", "eX", "X-Y", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP + 1);
   h_button[i_count++] = h_button_create(00101, '8', "8", "LOG", "10x", "Rv", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP + 1);
   h_button[i_count++] = h_button_create(00100, '9', "9", "V\xaf", "x\xb2", "R^", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);

   /* Define sixth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = KEY_SMALL;
   h_button[i_count++] = h_button_create(00123, '+', "+", "X\x1d\x30", "X\x1d\x59", "CF", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + 2 * KEY_GAP + 1);
   i_width = KEY_NUMERIC;
   h_button[i_count++] = h_button_create(00122, '4', "4", "SIN", "-\xb9", "1/x", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP + 1);
   h_button[i_count++] = h_button_create(00121, '5', "5", "COS", "-\xb9", "yX", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP + 1);
   h_button[i_count++] = h_button_create(00120, '6', "6", "TAN", "-\xb9", "ABS", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);

   /* Define seventh row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = KEY_SMALL;
   h_button[i_count++] = h_button_create(00143, '*', "\xd7", "X<0", "X\x1a\x59", "F?", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + 2 * KEY_GAP + 1);
   i_width = KEY_NUMERIC;
   h_button[i_count++] = h_button_create(00142, '1', "1", "-R", "-P", "PSE", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP + 1);
   h_button[i_count++] = h_button_create(00141, '2', "2", "-D", "-R", "Pi", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP + 1);
   h_button[i_count++] = h_button_create(00140, '3', "3", "-H", "-HMS", "REG", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);

   /* Define bottom row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = KEY_SMALL;
   h_button[i_count++] = h_button_create(00163, '/', "\xf7", "X>0", "X>Y", "N!", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, GREEN, YELLOW, BLUE, BLACK);
   i_left += (KEY_SMALL + 2 * KEY_GAP + 1);
   i_width = KEY_NUMERIC;
   h_button[i_count++] = h_button_create(00162, '0', "0", "%", "%CH", "LST x", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP + 1);
   h_button[i_count++] = h_button_create(00161, '.', ".", "INT", "FRAC", "HMS+", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP + 1);
   h_button[i_count++] = h_button_create(00160, 000, "R/S", "-x-", "STK", "SPACE", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, YELLOW, BLUE, BLACK);
}

int i_rom[ROM_SIZE * ROM_BANKS] = {};
