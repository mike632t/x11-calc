/*
 * x11-calc-35.c - RPN (Reverse Polish) calculator simulator.
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
 * 04 Jan 22         - Initial version (50th anniversary) - MT
 * 05 Jan 22         - Changed the square root key legend to SQRT - MT
 * 11 Jan 22         - Removed ROM_BANKS - MT
 * 12 Jan 22         - Added the option to build an early 'Red Dot' version
 *                     with ROM bugs - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0002"
#define DATE           "11 Jan 22"
#define AUTHOR         "MT"

/** #define RED_DOT */

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
   i_width = (2 * KEY_WIDTH + KEY_GAP);
   h_switch[0] = h_switch_create(00000, "OFF", "ON ", h_alternate_font, i_left, i_top, i_width, i_height, True, MID_GREY, DARK_GREY);
   h_switch[0]->state = True;
   h_switch[1] = NULL;

   /* Define top row of keys. */
   i_top += KEY_HEIGHT;
   i_left = KBD_LEFT;
   i_height = KEY_HEIGHT;
   i_width = KEY_WIDTH;
   i_count = 0;

#if defined(RED_DOT)
   h_button[i_count++] = h_button_create(00006, 000, "", "xY", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00004, 000, "", "log", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00003, 000, "", "ln", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00002, 000, "", "eX", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00000, 000, "", "CLR", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, LIGHT_GREY, BACKGROUND, BACKGROUND);

   /* Define second row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   /** h_button[i_count++] = h_button_create(00056, 000, "", "/\xaf", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND); /* Original */
   h_button[i_count++] = h_button_create(00056, 000, "", "sqrt", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND); /* Modified */
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00054, 000, "", "arc", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00053, 000, "", "sin", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00052, 000, "", "cos", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00050, 000, "", "tan", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);

   /* Define third row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00016, 000, "", "1/x", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00014, 000, "", "X-Y", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00013, 000, "", "Rv", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND); /* Original */
   /** h_button[i_count++] = h_button_create(00013, 000, "", "RDN", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND); /* Modified */
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00012, 000, "", "STO", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00010, 000, "", "RCL", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);

   /* Define fourth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = 2* KEY_WIDTH + KEY_GAP;
   h_button[i_count++] = h_button_create(00076, 015, "ENTER", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (2 * (KEY_WIDTH + KEY_GAP));
   i_width = KEY_WIDTH;
   h_button[i_count++] = h_button_create(00073, 'c', "", "CHS", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00072, 'e', "", "EEX", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00070, 033, "", "CLX", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, LIGHT_GREY, BACKGROUND, BACKGROUND);
#else
   h_button[i_count++] = h_button_create(00006, 000, "xY", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00004, 000, "LOG", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00003, 000, "LN", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00002, 000, "eX", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00000, 000, "CLR", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, BACKGROUND, BACKGROUND, BACKGROUND);

   /* Define second row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   /** h_button[i_count++] = h_button_create(00056, 000, "/\xaf", "" , "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND); /* Original */
   h_button[i_count++] = h_button_create(00056, 000, "SQRT", "" , "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND); /* Modified */
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00054, 000, "ARC", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00053, 000, "SIN", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00052, 000, "COS", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00050, 000, "TAN", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);

   /* Define third row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00016, 000, "1/x", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, LIGHT_GREY, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00014, 000, "X-Y", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00013, 000, "Rv", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND); /* Original */
   /** h_button[i_count++] = h_button_create(00013, 000, "RDN", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND); /* Modified */
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00012, 000, "STO", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00010, 000, "RCL", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);

   /* Define fourth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = 2* KEY_WIDTH + KEY_GAP;
   h_button[i_count++] = h_button_create(00076, 015, "ENTER", "      ", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (2 * (KEY_WIDTH + KEY_GAP));
   i_width = KEY_WIDTH;
   h_button[i_count++] = h_button_create(00073, 'c', "CHS", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00072, 'e', "EEX", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00070, 033, "CLX", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, BACKGROUND, BACKGROUND, BACKGROUND);
#endif

   /* Define fifth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = KEY_SMALL;
   h_button[i_count++] = h_button_create(00066, '-', "-", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_SMALL + 3 * KEY_GAP);
   i_width = KEY_NUMERIC;
   h_button[i_count++] = h_button_create(00064, '7', "7", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00063, '8', "8", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00062, '9', "9", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);

   /* Define sixth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = KEY_SMALL;
   h_button[i_count++] = h_button_create(00026, '+', "+", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_SMALL + 3 * KEY_GAP);
   i_width = KEY_NUMERIC;
   h_button[i_count++] = h_button_create(00024, '4', "4", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00023, '5', "5", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00022, '6', "6", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);

   /* Define seventh row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = KEY_SMALL;
   h_button[i_count++] = h_button_create(00036, '*', "\xd7", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_SMALL + 3 * KEY_GAP);
   i_width = KEY_NUMERIC;
   h_button[i_count++] = h_button_create(00034, '1', "1", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00033, '2', "2", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00032, '3', "3", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);

   /* Define bottom row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   i_width = KEY_SMALL;
   h_button[i_count++] = h_button_create(00046, '/', "\xf7", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, MID_BLUE, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_SMALL + 3 * KEY_GAP);
   i_width = KEY_NUMERIC;
   h_button[i_count++] = h_button_create(00044, '0', "0", "", "", "", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00043, '.', ".", "", "", "", h_large_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00042, 000, "\x1c", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, i_width, i_height, False, LIGHT_GRAY, BACKGROUND, BACKGROUND, BACKGROUND);
}

#if defined(RED_DOT)
int i_rom[ROM_SIZE] = {
   00335, 01377, 01044, 00027, 00504, 01104, 00204, 00420,
   00521, 00413, 00137, 00303, 00650, 01547, 01356, 01742,
   00056, 00220, 01752, 01752, 01752, 00153, 01151, 00250,
   01250, 00377, 01752, 01752, 01752, 00060, 00314, 00252,
   00650, 00103, 00723, 01314, 00060, 00000, 00203, 00504,
   00104, 00273, 01104, 00237, 01204, 01413, 00056, 00220,
   01450, 01557, 01752, 01752, 01752, 00113, 01147, 01650,
   01665, 01567, 00733, 01316, 00304, 01603, 00450, 00064,
   01316, 01567, 01250, 01557, 00456, 01372, 00426, 01552,
   01037, 00672, 01067, 01466, 01514, 01067, 01456, 01356,
   00324, 00523, 01576, 00376, 00724, 00537, 00450, 00704,
   00316, 00552, 01676, 01354, 00437, 00435, 00604, 01625,
   01326, 01454, 00573, 01056, 00756, 00114, 00422, 00074,
   00642, 00633, 00556, 01056, 01646, 00354, 01277, 01646,
   01352, 00424, 00577, 00753, 00521, 00220, 00404, 01324,
   00163, 01316, 01625, 00452, 01316, 00472, 01412, 01652,
   00032, 01017, 00252, 00512, 00612, 01701, 01567, 01326,
   00034, 00254, 00437, 01414, 01356, 01366, 01742, 01742,
   00214, 00074, 01542, 01133, 00002, 01107, 01742, 01456,
   00060, 00376, 00650, 00056, 01772, 01772, 00772, 00772,
   00112, 01217, 01656, 01646, 00646, 01237, 01656, 01046,
   00112, 01373, 01216, 01752, 00016, 01373, 01243, 00606,
   00624, 01323, 00034, 00552, 01222, 01414, 00646, 00703,
   00752, 01304, 01142, 00677, 00406, 01353, 00220, 01665,
   00504, 01567, 01316, 00636, 01044, 01477, 00772, 01004,
   00524, 01467, 00752, 01433, 00050, 00024, 01437, 00044,
   00034, 01454, 01503, 01050, 01024, 01463, 00416, 00544,
   00320, 00450, 01656, 01765, 00704, 01625, 00471, 00617,
   00424, 01623, 01632, 01757, 00376, 01244, 01417, 01672,
   00322, 00562, 00332, 01172, 01767, 00316, 00064, 00616,
   01414, 00672, 01737, 00252, 00572, 01637, 00514, 01652,
   00424, 00423, 01452, 00052, 00753, 00146, 01673, 01667,
   01717, 01456, 00241, 00650, 00241, 00650, 01124, 00047,
   01656, 00524, 00113, 00336, 01231, 00450, 01225, 01141,
   00225, 00650, 01231, 01224, 01553, 01356, 01742, 00446,
   01646, 00552, 01222, 00672, 00147, 01322, 00752, 00167,
   01316, 01216, 00450, 01056, 00407, 01056, 00414, 01573,
   00450, 01656, 00642, 00267, 00256, 00616, 00212, 01457,
   00616, 00124, 00227, 01224, 00667, 00524, 00127, 00376,
   01676, 00067, 01222, 01576, 00353, 00776, 01462, 00722,
   01456, 00456, 01522, 00357, 00650, 01316, 01662, 01456,
   00422, 00450, 01776, 01776, 00217, 00316, 00052, 00742,
   01335, 00542, 00650, 01656, 00414, 01221, 00614, 01155,
   01014, 01155, 00214, 01030, 01214, 01155, 01071, 01155,
   01461, 00416, 01155, 00216, 01455, 01461, 01256, 01231,
   01124, 00663, 00376, 01141, 00144, 00316, 00542, 00752,
   00124, 01227, 01231, 01461, 01256, 01225, 01461, 01256,
   01256, 01125, 01256, 01655, 01461, 01214, 01161, 01071,
   01014, 01165, 00214, 01030, 00614, 01161, 00414, 01161,
   01161, 01456, 01116, 01514, 00530, 01757, 00614, 01030,
   00630, 00530, 00230, 00430, 01130, 00124, 01553, 00060,
   01356, 01742, 00020, 00420, 00416, 01226, 01056, 01207,
   00776, 01416, 01203, 01616, 00420, 00420, 00512, 00420,
   00742, 01516, 01243, 01716, 00416, 00034, 01122, 00054,
   01247, 00267, 00742, 01426, 01313, 01626, 00426, 00034,
   00054, 01317, 00267, 00034, 01626, 01557, 00020, 00572,
   00572, 01352, 01536, 01176, 01433, 00420, 01006, 01453,
   00376, 01456, 01416, 00420, 00316, 01314, 00730, 01030,
   00530, 00330, 01130, 01030, 00130, 00630, 00330, 00530,
   01414, 00060, 00020, 01612, 01573, 00542, 00776, 00054,
   01357, 01652, 01352, 00142, 01633, 01316, 01116, 01052,
   00312, 01414, 01273, 00420, 01222, 01222, 00576, 01663,
   00722, 01422, 01062, 00216, 01576, 01673, 01662, 00650,
   00036, 00007, 00416, 01662, 00450, 01222, 00576, 01076,
   00020, 01476, 01776, 01126, 00422, 00113, 00650, 01231,
   00616, 01024, 00417, 01356, 01506, 00003, 01316, 00576,
   00003, 00776, 00456, 01131, 01542, 00107, 01462, 00476,
   01636, 00007, 00714, 00665, 01014, 01165, 01114, 01161,
   01771, 01214, 01161, 00765, 01314, 01161, 01575, 01161,
   01345, 01161, 01731, 01656, 00516, 00032, 00303, 00516,
   01456, 00034, 00416, 00154, 00307, 01656, 00676, 00347,
   00346, 00752, 01314, 01425, 01124, 00033, 00524, 01123,
   01731, 01235, 01123, 01731, 01661, 00066, 01345, 01314,
   01155, 01575, 01214, 01155, 00765, 01114, 01155, 01771,
   01014, 01155, 01155, 01155, 01514, 01056, 01656, 00630,
   01073, 00224, 00573, 01752, 01172, 01413, 01426, 00547,
   01626, 00416, 00552, 00563, 01316, 00322, 01652, 00676,
   00663, 01456, 01416, 00352, 01316, 01056, 00316, 00546,
   00224, 00733, 00430, 00746, 00747, 00630, 00154, 00727,
   01116, 01116, 00224, 01123, 00060, 00714, 00330, 00330,
   00030, 01030, 00530, 00030, 01130, 01653, 01131, 01742,
   00456, 00576, 01033, 01322, 01656, 00426, 01656, 01576,
   01043, 01356, 01742, 01421, 00220, 01322, 01576, 01127,
   01622, 01476, 00060, 00220, 01316, 01056, 01203, 01616,
   00576, 01177, 01656, 00426, 01656, 00667, 00314, 00712,
   00536, 01257, 00276, 01446, 01356, 01454, 01427, 00146,
   01333, 00124, 00003, 00222, 01546, 00772, 01062, 01646,
   00220, 01044, 00630, 01130, 00330, 00130, 00430, 00730,
   00130, 01633, 01746, 00623, 01616, 00542, 01423, 01316,
   00074, 01554, 01427, 00752, 01376, 01414, 00056, 01142,
   01533, 00416, 00552, 01156, 01477, 00316, 00452, 01616,
   01176, 01437, 01646, 00616, 00056, 01414, 00753, 01114,
   00330, 00130, 00030, 00130, 00730, 01130, 01030, 00030,
   00530, 00530, 00330, 01567, 01656, 00456, 00606, 01272,
   00573, 00772, 01316, 00752, 01713, 00637, 00316, 01414,
   00230, 00330, 00030, 00230, 00530, 01007, 00514, 00773
};
#else
int i_rom[ROM_SIZE] = {
   00335, 01377, 01044, 00027, 00504, 01104, 00204, 00420,
   01321, 01773, 00137, 00303, 00650, 01547, 01356, 01742,
   00056, 00220, 01752, 01752, 01752, 00153, 01151, 00250,
   01250, 00377, 01752, 01752, 01752, 00060, 00314, 00252,
   00650, 00103, 00723, 00314, 00060, 00000, 00203, 00504,
   00104, 00273, 01104, 00237, 01204, 01413, 00056, 00220,
   01450, 01557, 01752, 01752, 01752, 00113, 01147, 01650,
   00561, 01567, 01713, 01316, 00304, 00733, 00450, 00064,
   01316, 01565, 00456, 01372, 00426, 01552, 01603, 00672,
   01633, 01466, 01514, 01633, 00034, 00752, 00002, 00463,
   01304, 01326, 01646, 00424, 01037, 00575, 01567, 00322,
   00562, 00332, 01612, 00567, 00316, 00064, 00616, 01414,
   00452, 00612, 00672, 00643, 00252, 00572, 00537, 00514,
   01652, 00424, 00413, 01452, 00052, 01735, 00412, 01316,
   01454, 01047, 01662, 00753, 01321, 00220, 00424, 01733,
   01662, 00372, 00616, 00672, 00777, 00332, 00252, 01514,
   00426, 00552, 01176, 00473, 01166, 01003, 00312, 01735,
   01326, 00636, 01454, 01117, 00216, 00756, 00114, 00422,
   00074, 00642, 01077, 01656, 00354, 01747, 00312, 00604,
   00753, 00376, 00650, 00056, 01772, 01772, 00772, 00772,
   00112, 01217, 01656, 01646, 00646, 01237, 01656, 01046,
   00112, 01373, 01216, 01752, 00016, 01373, 01243, 01366,
   00324, 01323, 01576, 00376, 00724, 01337, 00450, 00704,
   00316, 00556, 00276, 00776, 01056, 00060, 00220, 00561,
   00504, 01567, 01316, 00636, 01044, 01477, 00772, 01004,
   00524, 01467, 00752, 01433, 00050, 00024, 01437, 00044,
   00034, 01454, 01503, 01050, 01024, 01463, 00416, 00544,
   00320, 00450, 01656, 00565, 00704, 01735, 01275, 01053,
   01326, 00034, 00254, 00427, 01414, 01356, 01366, 01742,
   01742, 00214, 00074, 01542, 01677, 00002, 01653, 01742,
   01456, 00060, 00404, 01324, 00163, 00677, 00376, 01244,
   01417, 00624, 01763, 00034, 01222, 00751, 01250, 01557,
   01717, 01456, 00241, 00650, 00241, 00650, 01124, 00047,
   01656, 00524, 00113, 00336, 01231, 00450, 01225, 01141,
   00225, 00650, 01231, 01224, 01553, 01356, 01742, 00446,
   01646, 00552, 01222, 00672, 00147, 01322, 00752, 00167,
   01316, 01216, 00450, 01056, 00407, 01056, 00414, 01573,
   00450, 01656, 00642, 00267, 00256, 00616, 00212, 01457,
   00616, 00124, 00227, 01224, 00667, 00524, 00127, 00376,
   01676, 00067, 01222, 01576, 00353, 00776, 01462, 00722,
   01456, 00456, 01522, 00357, 00650, 01316, 01662, 01456,
   00422, 00450, 01776, 01776, 00217, 00316, 00052, 01326,
   01311, 00542, 00650, 01656, 00414, 01221, 00614, 01155,
   01014, 01155, 00214, 01030, 01214, 01155, 01071, 01155,
   01461, 00416, 01155, 00216, 01455, 01461, 01256, 01231,
   01124, 00663, 00376, 01141, 00144, 00316, 00542, 00752,
   00124, 01227, 01231, 01461, 01256, 01225, 01461, 01256,
   01256, 01125, 01256, 01655, 01461, 01214, 01161, 01071,
   01014, 01165, 00214, 01030, 00614, 01161, 00414, 01161,
   01161, 01456, 01116, 01514, 00530, 01757, 00614, 01030,
   00630, 00530, 00230, 00430, 01130, 00124, 01553, 00060,
   01356, 01742, 00020, 00420, 00416, 01226, 01056, 01207,
   00776, 01416, 01203, 01616, 00420, 00420, 00512, 00420,
   00742, 01516, 01243, 01716, 00416, 00034, 01122, 00054,
   01247, 00267, 00742, 01426, 01313, 01626, 00426, 00034,
   00054, 01317, 00267, 00034, 01626, 01557, 00020, 00572,
   00572, 01352, 01536, 01176, 01433, 00420, 01006, 01453,
   00376, 01456, 01416, 00420, 00316, 01314, 00730, 01030,
   00530, 00330, 01130, 01030, 00130, 00630, 00330, 00530,
   01414, 00060, 00020, 01612, 01573, 00542, 00776, 00054,
   01357, 01652, 01352, 00142, 01633, 01316, 01116, 01052,
   00312, 01414, 01273, 00420, 01222, 01222, 00576, 01663,
   00722, 01422, 01062, 00216, 01576, 01673, 01662, 00650,
   00036, 00007, 00416, 01662, 00450, 01222, 00576, 01076,
   00020, 01476, 01776, 01126, 00422, 00113, 00650, 01231,
   00616, 01024, 00413, 01356, 01506, 00003, 01316, 00576,
   00003, 00776, 00456, 01131, 01542, 00107, 01462, 01636,
   00007, 00714, 00665, 01014, 01165, 01114, 01161, 01771,
   01214, 01161, 00765, 01314, 01161, 01575, 01161, 01345,
   01161, 01731, 01656, 00516, 00032, 00277, 00516, 01456,
   00034, 00416, 00154, 00303, 01656, 00676, 00343, 00346,
   00752, 01314, 01425, 01124, 00033, 00524, 01123, 01731,
   01235, 01123, 01731, 01661, 01345, 01314, 01155, 01575,
   01214, 01155, 00765, 01114, 01155, 01771, 01014, 01155,
   01155, 01155, 00614, 01362, 01514, 01056, 01656, 00630,
   01073, 00224, 00573, 01752, 01172, 01413, 01426, 00547,
   01626, 00416, 00552, 00563, 01316, 00322, 01652, 00676,
   00663, 01456, 01416, 00356, 01316, 01056, 00316, 00546,
   00224, 00733, 00430, 00746, 00747, 00630, 00154, 00727,
   01116, 01116, 00224, 01123, 00060, 00714, 00330, 00330,
   00030, 01030, 00530, 00030, 01130, 01653, 01131, 01742,
   00456, 00576, 01033, 01322, 01656, 00426, 01656, 01576,
   01043, 01456, 01742, 01461, 00220, 01322, 01576, 01127,
   01376, 01616, 00060, 00220, 01316, 01056, 01203, 01616,
   00576, 01177, 01656, 00426, 01656, 00667, 00314, 00712,
   00536, 01257, 00276, 01446, 01356, 01454, 01427, 00146,
   01333, 00124, 00003, 00222, 01546, 00772, 01062, 01646,
   00220, 01044, 00630, 01130, 00330, 00130, 00430, 00730,
   00130, 01633, 01746, 00623, 01616, 00542, 01423, 01316,
   00074, 01554, 01427, 00752, 01376, 01414, 00056, 01142,
   01533, 00416, 00552, 01156, 01477, 00316, 00452, 01616,
   01176, 01437, 01646, 00616, 00056, 01414, 00753, 01114,
   00330, 00130, 00030, 00130, 00730, 01130, 01030, 00030,
   00530, 00530, 00330, 01567, 01656, 00456, 00606, 01272,
   00573, 00772, 01316, 00752, 01713, 00637, 00316, 01414,
   00230, 00330, 00030, 00230, 00530, 01007, 00514, 00773
};
#endif
