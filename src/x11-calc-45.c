/*
 * x11-calc-45.c - RPN (Reverse Polish) calculator simulator.
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
 * 24 Dec 21         - Initial version with new button style - MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 11 Jan 22         - Removed ROM_BANKS - MT
 * 12 Feb 22         - Updated layout and separated the initialisation code
 *                     for buttons and switches into two functions - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0004"
#define DATE           "12 Feb 22"
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

void v_init_switches(oswitch *h_switch[]) /* Define the switches. */
{
   h_switch[0] = h_switch_create(00000, "OFF", "ON ", h_alternate_font, KBD_LEFT, KBD_TOP, 2 * KEY_WIDTH + KEY_GAP, SWITCH_HEIGHT, True, MID_GREY, DARK_GREY);
   h_switch[1] = NULL;
}

void v_init_buttons(obutton *h_button[]) {
   int i_left, i_top, i_count = 0;

   /* Define top row of keys. */
   i_top = KBD_TOP + 2* (SWITCH_HEIGHT + 1); /* Assumes the function label and switch text use the same font */
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00006, 000, "1/x", "yX", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, DIM_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00004, 000, "ln", "log", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, DIM_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00003, 000, "eX", "10x", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, DIM_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00002, 000, "FIX", "SCI", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, DIM_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00000, 'f', "", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, YELLOW, BACKGROUND, BACKGROUND, BACKGROUND);

   /* Define second row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00056, 000, "x\xb2", "/\xaf", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, DIM_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00054, 000, "-P", "-R", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00053, 000, "SIN", "SIN-\xb9", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00052, 000, "COS", "COS-\xb9", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00050, 000, "TAN", "TAN-\xb9", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, BACKGROUND, BACKGROUND);

   /* Define third row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00016, 000, "X-Y", "n!", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00014, 000, "Rv", "x,s", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00013, 000, "STO", "-D.MS", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00012, 000, "RCL", "D.MS-", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00010, 000, "%", "d%", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, DIM_GREY, YELLOW, BACKGROUND, BACKGROUND);

   /* Define fourth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00074, 015, "ENTER", "      ", "DEG", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, 2* KEY_WIDTH + KEY_GAP, KEY_HEIGHT, False, False, MID_GREY, BACKGROUND, YELLOW, BACKGROUND);
   i_left += (2 * (KEY_WIDTH + KEY_GAP));
   h_button[i_count++] = h_button_create(00073, 'c', "CHS", "RAD", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00072, 'e', "EEX", "GRD", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00070, 033, "CLX", "CLEAR", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);

   /* Define fifth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00066, '-', "-", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_SMALL, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_SMALL + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00064, '7', "7", "cm/in", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00063, '8', "8", "kg/lb", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00062, '9', "9", "ltr/gal", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, BACKGROUND, BACKGROUND);

   /* Define sixth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00026, '+', "+", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_SMALL, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_SMALL + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00024, '4', "4", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00023, '5', "5", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00022, '6', "6", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, BACKGROUND, BACKGROUND);

   /* Define seventh row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00036, '*', "\xd7", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_SMALL, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_SMALL + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00034, '1', "1", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00033, '2', "2", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00032, '3', "3", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, BACKGROUND, BACKGROUND);

   /* Define bottom row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00046, '/', "\xf7", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_SMALL, KEY_HEIGHT, False, False, MID_GREY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_SMALL + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00044, '0', "0", "LAST x", "", "", h_large_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GRAY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00043, '.', ".", "\x1c", "", "", h_large_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GRAY, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00042, 000, "E+", "E-", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GRAY, YELLOW, BACKGROUND, BACKGROUND);
}
int i_rom[ROM_SIZE] = {
   00255, 01420, 00451, 01456, 01746, 00472, 01572, 01616,
   01352, 01611, 01611, 01352, 01445, 00623, 01024, 00507,
   01035, 01656, 00616, 00013, 01220, 01035, 01656, 01020,
   00220, 00324, 00143, 00450, 00316, 01160, 00000, 01370,
   01656, 01171, 01431, 01211, 01431, 01376, 01370, 00676,
   00267, 01576, 00265, 00123, 00220, 01656, 01024, 01713,
   00376, 01711, 00124, 00263, 01224, 00647, 00353, 00220,
   00616, 00220, 01656, 00616, 00772, 00433, 00652, 00433,
   00316, 00014, 00530, 01414, 00712, 01657, 01224, 00727,
   01656, 00343, 00312, 00014, 00530, 01512, 01172, 01463,
   01777, 00451, 01615, 00056, 01462, 01456, 00416, 01625,
   00704, 01445, 00316, 00330, 00630, 01414, 01171, 01031,
   00767, 00752, 00742, 01017, 00316, 00214, 00430, 01656,
   00007, 00220, 00704, 00424, 00717, 01256, 00736, 01146,
   00713, 00376, 01231, 00104, 00127, 00220, 01024, 00653,
   01035, 00104, 01224, 00353, 01717, 01656, 00620, 00552,
   00552, 00704, 00542, 01224, 01173, 01213, 01244, 00704,
   00444, 00604, 01250, 00676, 01073, 00404, 00776, 01107,
   00644, 00316, 00752, 00124, 01403, 00424, 01607, 00744,
   00624, 01013, 00607, 01253, 01224, 01213, 01304, 00144,
   00056, 01227, 01304, 00144, 00220, 00220, 01304, 00376,
   00144, 00244, 00056, 01772, 01772, 00772, 00772, 00112,
   01313, 01656, 01646, 00646, 01333, 01656, 01046, 00112,
   01373, 01216, 01752, 00016, 01373, 01337, 00220, 01777,
   00624, 01607, 00744, 00424, 01013, 00777, 00650, 00450,
   00060, 01304, 00220, 00220, 01046, 01216, 01752, 01467,
   01356, 00614, 00060, 01324, 01543, 01256, 00704, 01163,
   00220, 01414, 01517, 01324, 01747, 01304, 00724, 01457,
   01344, 00060, 00056, 01462, 01216, 01214, 01616, 00034,
   00454, 01633, 00060, 00316, 00742, 01224, 00337, 00647,
   00124, 00773, 01656, 00316, 00542, 01160, 00316, 01360,
   00767, 00324, 01703, 01224, 01773, 01020, 01220, 00420,
   01076, 01717, 01456, 00301, 01370, 01656, 00301, 01370,
   01656, 01124, 00063, 01656, 00524, 00153, 00176, 00107,
   01044, 00336, 01231, 01360, 01225, 01141, 00265, 01370,
   00020, 01656, 01231, 01224, 01553, 01356, 01742, 00446,
   01646, 00552, 01222, 00672, 00207, 01322, 00752, 00227,
   01316, 01216, 01360, 01056, 00433, 01056, 00414, 01573,
   01360, 01656, 00642, 00327, 00256, 00616, 00212, 01457,
   00047, 00020, 00524, 00167, 00376, 01676, 00117, 01222,
   01576, 00377, 00776, 01462, 00722, 01456, 00456, 01522,
   00403, 01656, 01370, 01116, 01676, 01456, 00422, 01360,
   01776, 01776, 00257, 00316, 00052, 01326, 01311, 01542,
   01370, 00414, 01221, 00614, 01155, 01014, 01155, 00214,
   01030, 01214, 01155, 01071, 01155, 01461, 00416, 01155,
   00216, 01455, 01461, 01256, 01224, 00727, 01124, 00727,
   01656, 00376, 01151, 01461, 01256, 00020, 01461, 01256,
   01256, 01125, 01256, 01655, 01461, 01214, 01161, 01071,
   01014, 01165, 00214, 01030, 00614, 01161, 00414, 01161,
   01161, 01456, 01116, 01514, 00530, 01763, 00614, 01030,
   00630, 00530, 00230, 00430, 01130, 00124, 01553, 00060,
   01356, 01742, 00020, 00420, 00416, 01226, 01056, 01207,
   00776, 01416, 01203, 01616, 00420, 00420, 00512, 00420,
   00742, 01516, 01243, 01716, 00416, 00034, 01122, 00054,
   01247, 00327, 00742, 01426, 01313, 01626, 00426, 00034,
   00054, 01317, 00327, 00034, 01626, 01557, 00020, 00572,
   00572, 01352, 01536, 01176, 01433, 00420, 01006, 01453,
   00376, 01456, 01416, 00420, 00316, 01314, 00730, 01030,
   00530, 00330, 01130, 01030, 00130, 00630, 00330, 00530,
   00020, 00060, 00020, 01612, 01573, 00542, 00776, 00054,
   01357, 01652, 01352, 00142, 01633, 01316, 01116, 01052,
   00312, 01414, 01273, 00420, 01222, 01222, 00576, 01663,
   00722, 01422, 01062, 00216, 01576, 01673, 01676, 01370,
   01656, 00036, 00013, 00416, 01662, 01360, 01222, 00576,
   01420, 01476, 01776, 01126, 00422, 00113, 00650, 01231,
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
   01333, 00124, 00003, 01220, 01557, 00000, 01062, 01646,
   00220, 01044, 00630, 01130, 00330, 00130, 00430, 00730,
   00130, 01633, 01746, 00623, 01616, 00542, 01423, 01316,
   00074, 01554, 01427, 00752, 01376, 01414, 00056, 01142,
   01533, 00416, 00552, 01156, 01477, 00316, 00452, 01616,
   01176, 01437, 01646, 00616, 00056, 01414, 00753, 01114,
   00330, 00130, 00030, 00130, 00730, 01130, 01030, 00030,
   00530, 00530, 00330, 01567, 01656, 00456, 00606, 01272,
   00573, 00772, 01316, 00752, 01713, 00637, 00316, 01414,
   00230, 00330, 00030, 00230, 00530, 01007, 00514, 00773,
   01020, 00000, 00453, 00547, 00553, 00000, 00423, 00420,
   01020, 01420, 00633, 00643, 01450, 00777, 00650, 00450,
   00773, 00377, 01756, 01756, 01756, 00153, 01020, 01035,
   00416, 00513, 01756, 01756, 01756, 00060, 01020, 00320,
   01044, 01020, 00203, 00314, 00060, 01020, 00227, 00504,
   00621, 00413, 00463, 00237, 01020, 00000, 00605, 00443,
   00000, 00020, 01756, 01756, 01756, 00113, 01020, 00000,
   01231, 01007, 01357, 01407, 00056, 01620, 00450, 01235,
   01007, 00605, 01144, 00307, 00605, 01356, 01742, 00447,
   01020, 01020, 01144, 00137, 00621, 01104, 00504, 00307,
   00416, 01756, 00416, 00250, 01652, 00250, 00713, 00000,
   00000, 01044, 01104, 00605, 00204, 00037, 01035, 00503,
   00304, 01244, 01420, 01420, 00104, 00607, 01104, 00647,
   01144, 00204, 01035, 00725, 00615, 01124, 00707, 01731,
   00773, 01360, 00724, 00377, 00777, 00014, 01142, 01777,
   00713, 01035, 00725, 01020, 00000, 00000, 01656, 01235,
   00704, 01025, 01725, 01533, 00376, 01244, 01043, 01316,
   00636, 01044, 01123, 00772, 01004, 00524, 01113, 00752,
   01057, 00050, 00024, 01063, 00044, 00034, 01454, 01127,
   01050, 01024, 01107, 00416, 00544, 01224, 00177, 01020,
   00000, 00322, 00562, 00332, 01612, 01237, 00316, 00064,
   00616, 01414, 00452, 00612, 00672, 01307, 00252, 00572,
   01207, 01652, 00424, 00047, 01452, 00052, 01025, 01454,
   01037, 00412, 01373, 00404, 01324, 00163, 01316, 01662,
   01433, 01316, 00424, 01023, 01662, 00372, 00616, 00672,
   01457, 00332, 00252, 01514, 00426, 00552, 01176, 01663,
   01166, 01463, 00312, 01025, 01326, 00636, 01454, 01577,
   00216, 00756, 00114, 00422, 00074, 00642, 01557, 01656,
   00354, 01627, 00312, 00604, 01433, 00624, 01643, 00034,
   01222, 01431, 00034, 00752, 00002, 01653, 01304, 01326,
   01646, 00424, 01517, 01245, 01007, 01366, 00724, 01747,
   00450, 00704, 00316, 00556, 00276, 00776, 01056, 00060,
   00000, 01023, 00563, 01443, 00543, 01420, 00605, 00553,
   01143, 00413, 01044, 00103, 00203, 00020, 00605, 01777,
   00605, 00067, 01013, 01013, 01013, 01220, 00000, 00643,
   01327, 00000, 01013, 01013, 01013, 00000, 00000, 00673,
   00601, 01220, 01503, 00713, 00707, 00000, 00717, 00504,
   00571, 00407, 00457, 00237, 01117, 01243, 00605, 00407,
   00000, 00000, 01756, 01756, 00703, 01220, 00000, 00657,
   00605, 01727, 01756, 00377, 00000, 00000, 01556, 00014,
   00473, 00620, 00601, 01044, 01450, 00450, 00552, 00552,
   01024, 01217, 01207, 00571, 00620, 00620, 00416, 00074,
   01554, 00473, 00250, 01642, 00250, 00620, 00000, 00000,
   00504, 00620, 01035, 00620, 01244, 00620, 00104, 00607,
   00304, 01204, 01420, 01420, 01420, 01204, 00304, 00617,
   00604, 00404, 00727, 00644, 00404, 00727, 00604, 00723,
   01756, 01420, 01420, 00644, 00444, 00224, 00777, 01244,
   00620, 01244, 01220, 00000, 01057, 00000, 00620, 00605,
   00650, 00747, 01244, 01027, 01204, 01144, 00620, 00650,
   00450, 01656, 00060, 01124, 01103, 00601, 01656, 00747,
   00625, 01360, 01073, 00571, 00650, 00304, 01244, 00467,
   00601, 01450, 00450, 01231, 00423, 00000, 00000, 01220,
   00320, 00344, 01227, 00344, 00220, 00220, 00376, 00020,
   00571, 00304, 00444, 00676, 01273, 00404, 01450, 01041,
   01146, 00327, 00322, 00742, 00325, 00144, 01035, 00616,
   01221, 01360, 01455, 00621, 01221, 01370, 01235, 01572,
   01572, 01572, 01427, 00572, 01705, 00616, 00405, 01707,
   00605, 00204, 00027, 00316, 01160, 00000, 01370, 00060,
   01124, 01517, 01177, 00601, 00127, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00020, 00000, 00000, 00000, 01220, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00060, 01420,
   01420, 00742, 00742, 00742, 00742, 00742, 00516, 01160,
   00000, 01370, 00424, 01777, 01656, 01024, 01237, 01233,
   00620, 00000, 00000, 00000, 00000, 01623, 00404, 01244,
   01221, 00011, 01355, 00265, 00616, 00015, 00217, 00000,
   00000, 00000, 00413, 01355, 00311, 00021, 01355, 00316,
   00742, 00616, 00005, 01355, 00103, 00316, 01160, 00000,
   01370, 00060, 01450, 00450, 00616, 00060, 01044, 01144,
   01225, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00620, 01244, 00444, 00015, 00616, 01221, 00005,
   00176, 00003, 01225, 00011, 01656, 01231, 00450, 00005,
   01656, 00316, 00742, 01231, 00650, 01225, 00405, 00450,
   00015, 00616, 00005, 01205, 00444, 00724, 00603, 00450,
   00616, 00021, 00450, 00616, 00015, 00773, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 01355, 01656, 00620,
   00000, 00000, 00000, 01561, 01124, 00727, 00620, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00563, 00344, 01227, 00344, 00220, 00220, 00376, 00020,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00322, 00562, 00332, 00420, 01303, 00322, 00562, 00332,
   01612, 01357, 00316, 00616, 01414, 00452, 00612, 00672,
   01423, 00252, 00572, 01327, 01666, 01370, 01656, 01360,
   00060, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 00000, 00424, 01607, 00624, 01233,
   01237, 00624, 01227, 01223, 00316, 00576, 00214, 00230,
   00250, 00316, 01356, 01414, 00542, 01160, 01656, 00450,
   01360, 01656, 00742, 00742, 01663, 00773, 00316, 00630,
   01653, 00000, 00000, 00000, 00000, 00000, 00000, 00060,
   01363, 00037, 00064, 00777, 00772, 00771, 01721, 00316,
   00064, 00504, 00616, 01250, 01656, 00147, 01326, 01752,
   00073, 01514, 01472, 00472, 00034, 00254, 00267, 01356,
   01552, 00056, 01004, 00114, 00472, 00606, 00426, 01142,
   00107, 01044, 01614, 00612, 00172, 00073, 00034, 00254,
   00253, 00137, 01552, 00233, 00113, 01572, 00123, 00442,
   00034, 01362, 00612, 01626, 00353, 01326, 01776, 01752,
   01024, 00357, 00074, 01326, 00066, 01572, 01172, 00407,
   00137, 01772, 01456, 01742, 01742, 00426, 01572, 00427,
   01362, 01562, 01326, 01456, 01024, 00527, 01652, 00052,
   00672, 00523, 00252, 00572, 01652, 00524, 01773, 01003,
   00000, 00000, 00000, 00000, 00000, 00000, 00000, 00000,
   00000, 00000, 00000, 01323, 01223, 00616, 00316, 00542,
   01160, 01656, 00616, 01757, 00000, 00000, 00000, 00000,
   00000, 00000, 01007, 00731, 00064, 00220, 00724, 00747,
   00450, 00316, 00060, 00000, 01244, 00621, 00620, 00145,
   00620, 00416, 00416, 00731, 01572, 01037, 00763, 01572,
   01067, 00230, 00530, 00430, 00773, 01572, 01147, 00430,
   00530, 00330, 00530, 01130, 00230, 00330, 00730, 00552,
   00771, 00330, 00730, 01030, 00530, 00430, 00130, 00130,
   00730, 01030, 00430, 00773, 00014, 00416, 00074, 01454,
   01227, 01376, 01656, 01160, 00244, 01370, 01656, 01344,
   00056, 00124, 01757, 00627, 00456, 01656, 00316, 01160,
   00216, 01656, 01360, 00623, 00176, 00037, 00172, 00037,
   00152, 01423, 00034, 01457, 00034, 00354, 01443, 00023,
   00552, 01401, 00420, 00162, 00037, 01652, 01314, 00652,
   01527, 00552, 00152, 00023, 00416, 01656, 01356, 01742,
   00256, 01453, 01656, 01116, 00776, 01414, 00466, 01716,
   01577, 01516, 00416, 01716, 01617, 01776, 01456, 01731,
   01314, 01731, 00216, 00062, 01216, 01456, 01626, 01567,
   01656, 01052, 00752, 01451, 01044, 00420, 00002, 01753,
   01222, 01752, 00060, 01224, 01773, 01020, 00620, 00000,
   00027, 01213, 00027, 00027, 00027, 00067, 00027, 00107,
   00027, 01237, 00027, 00027, 00027, 01223, 00027, 01130,
   01117, 01227, 00303, 00177, 00430, 00767, 00027, 01030,
   01117, 01243, 00753, 00763, 00130, 00767, 00027, 00530,
   01117, 00237, 01103, 01507, 00030, 00767, 00027, 00047,
   00027, 00267, 00027, 00027, 00027, 00277, 00027, 00337,
   00630, 01117, 00077, 00137, 00730, 00767, 00027, 01767,
   01356, 00065, 00723, 01073, 00713, 00620, 01124, 00367,
   00064, 00176, 01773, 00552, 00552, 00672, 01773, 00447,
   01106, 00752, 00443, 01656, 01250, 00130, 00330, 00106,
   01773, 00630, 01214, 00102, 01773, 01014, 01322, 00714,
   00630, 00714, 00102, 01773, 00250, 00514, 01322, 01322,
   01322, 01322, 01456, 01562, 01352, 01014, 01542, 01314,
   01742, 01742, 01576, 01456, 00044, 00245, 00245, 00024,
   01063, 01233, 01204, 01223, 00012, 01043, 00052, 00037,
   00000, 00000, 00330, 01117, 00230, 01117, 00620, 00024,
   00007, 01024, 00663, 01050, 01044, 01414, 00316, 00320,
   01452, 01552, 01452, 01227, 01004, 00207, 01224, 00713,
   01244, 01160, 01133, 01160, 01224, 01157, 01656, 00616,
   01106, 01360, 01233, 01370, 01414, 00642, 01203, 00316,
   01656, 00426, 00265, 00275, 00275, 00275, 00275, 00335,
   01250, 01050, 00050, 01224, 00777, 00114, 01762, 00777,
   00614, 01742, 01213, 00714, 01742, 00102, 01343, 01217,
   01362, 01114, 01742, 00067, 01214, 01742, 00102, 01407,
   01227, 00322, 01362, 01314, 01742, 01453, 01356, 01414,
   01742, 01237, 00106, 01467, 00047, 01356, 01314, 01742,
   00147, 01250, 00322, 00214, 00012, 01543, 00430, 01547,
   00630, 00250, 00316, 01414, 00056, 01160, 01056, 01370,
   00142, 01623, 01655, 01360, 01056, 00742, 01567, 01306,
   01656, 01655, 00773, 00662, 01767, 01656, 00416, 01014,
   00422, 00614, 00422, 00422, 00422, 01414, 01752, 01142,
   01763, 01552, 00406, 01737, 01656, 00060, 00316, 00455
};
