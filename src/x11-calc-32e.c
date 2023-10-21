/*
 * x11-calc-32.c - RPN (Reverse Polish) calculator simulator.
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
 * 17 Oct 21         - Defined ROM contents and key codes - MT
 * 04 Nov 21         - Allows size of the window to be changed by modifying
 *                     the value of SCALE at compile time - MT
 * 23 Nov 21         - Added the alternate function text - MT
 * 06 Dec 21         - Label text colour now explicitly defined - MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 11 Jan 22         - Removed ROM_BANKS - MT
 * 12 Feb 22         - Updated layout and separated the initialisation code
 *                     for buttons and switches into two functions - MT
 * 21 Oct 23         - Updated switch parameters to accomodate a 3 position
 *                     switch - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0014"
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
   h_switch[0] = h_switch_create(00000, "OFF", "", "ON ", h_alternate_font, KBD_LEFT, KBD_TOP, 2 * KEY_WIDTH + KEY_GAP, SWITCH_HEIGHT, True, MID_GREY, DARK_GREY);
   h_switch[1] = NULL;
}

void v_init_buttons(obutton *h_button[]) {
   int i_left, i_top;

   /* Define top row of keys. */
   i_top = KBD_TOP + 2* (SWITCH_HEIGHT + 1); /* Assumes the function label and switch text use the same font */
   i_left = KBD_LEFT;
   h_button[0] = h_button_create(00064, 000, "/\xaf", "FIX", "", "x\xb2", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[1] = h_button_create(00063, 000, "1/x", "SCI", "", "\x1c", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[2] = h_button_create(00062, 000, "yX", "ENG", "", "LSTx", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[3] = h_button_create(00061, 'f', "f", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, YELLOW, YELLOW, YELLOW, YELLOW);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[4] = h_button_create(00060, 'g', "g", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, LIGHT_BLUE, LIGHT_BLUE, LIGHT_BLUE, LIGHT_BLUE);

   /* Define second row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[5] = h_button_create(00224, 000, "X-Y", "Q", "", "Q-\xb9", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[6] = h_button_create(00223, 000, "R", "y", "", "x", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[7] = h_button_create(00222, 000, "STO", "LR", "", "r", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[8] = h_button_create(00221, 000, "RCL", "x", "", "s", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[9] = h_button_create(00220, 000, "E+", "E-", "", "n!", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);

   /* Define third row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[10] = h_button_create(00163, 015, "ENTER", "PREFIX", "", "MANT", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, 2 * KEY_WIDTH + KEY_GAP, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += 2 * (KEY_WIDTH + KEY_GAP);
   h_button[11] = h_button_create(00162, 'c', "CHS", "ALL", "", "RAD", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[12] = h_button_create(00161, 'e', "EEX", "REG", "", "GRD", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[13] = h_button_create(00160, 033, "CLx", "E", "", "DEC", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, BLACK, YELLOW, MID_BLUE, BLACK);

   /* Define fourth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;

   h_button[14] = h_button_create(00243, '-', "-", "-in", "", "-mm", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + 2 * KEY_GAP );
   h_button[15] = h_button_create(00242, '7', "7", "SIN", "", "SIN\xaf\xb9", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP );
   h_button[16] = h_button_create(00241, '8', "8", "COS", "", "COS\xaf\xb9", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP );
   h_button[17] = h_button_create(00240, '9', "9", "TAN", "", "TAN\xaf\xb9", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);

   /* Alternate key labels
    * h_button[15] = h_button_create(00000, '7', "7", "SIN", "", "ASIN", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
    * i_left += (KEY_WIDTH + 2 * KEY_GAP );
    * h_button[16] = h_button_create(00000, '8', "8", "COS", "", "ACOS", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
    * i_left += (KEY_WIDTH + 2 * KEY_GAP );
    * h_button[17] = h_button_create(00000, '9', "9", "TAN", "", "ATAN", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);

   /* Define fifth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[18] = h_button_create(00143, '+', "+", "-\x07\x46", "", "-\x07\x43", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + 2 * KEY_GAP );
   h_button[19] = h_button_create(00142, '4', "4", "-R", "", "-P", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP);
   h_button[20] = h_button_create(00141, '5', "5", "-RAD", "", "-DEG", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP);
   h_button[21] = h_button_create(00140, '6', "6", "-H.MS", "", "-H", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);

   /* Define sixth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[22] = h_button_create(00323, '*', "\xd7", "-lbm", "", "-kg", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + 2 * KEY_GAP);
   h_button[23] = h_button_create(00322, '1', "1", "SINH", "", "SINH\xaf\xb9", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2* KEY_GAP);
   h_button[24] = h_button_create(00321, '2', "2", "COSH", "", "COSH\xaf\xb9", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP);
   h_button[25] = h_button_create(00320, '3', "3", "TANH", "", "TANH\xaf\xb9", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);

   /* Alternate key labels
    * h_button[23] = h_button_create(00000, '1', "1", "SINH", "", "ASINH", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
    * i_left += (KEY_NUMERIC + 2 * KEY_GAP);
    * h_button[24] = h_button_create(00000, '2', "2", "COSH", "", "ACOSH", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
    * i_left += (KEY_NUMERIC + 2 * KEY_GAP);
    * h_button[25] = h_button_create(00000, '3', "3", "TANH", "", "ATANH", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
    */

   /* Define bottom row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[26] = h_button_create(00103, '/', "\xf7", "-gal", "", "-ltr", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_WIDTH + 2 * KEY_GAP);
   h_button[27] = h_button_create(00102, '0', "0", "LN", "", "eX", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP);
   h_button[28] = h_button_create(00101, '.', "\xb7", "LOG", "", "10x", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
   i_left += (KEY_NUMERIC + 2 * KEY_GAP);
   h_button[29] = h_button_create(00100, '%', "%", "%E", "", "D%", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, False, LIGHT_GREY, YELLOW, MID_BLUE, BLACK);
}

int i_rom[ROM_SIZE] = {
   00440, 00063, 00073, 00107, 00747, 00757, 00773, 01113,
   01123, 01137, 01713, 01367, 00072, 00126, 00410, 00510,
   00416, 00062, 00422, 00410, 01074, 00620, 00652, 00752,
   00254, 00025, 00372, 01372, 00075, 00410, 00132, 01372,
   00075, 01456, 00150, 01022, 01522, 00047, 00317, 01232,
   00262, 01456, 00063, 00410, 00232, 00732, 00232, 00410,
   00132, 01116, 00132, 01456, 00075, 00656, 01772, 00222,
   00262, 00620, 01154, 00063, 00432, 00232, 00510, 00372,
   00472, 01152, 01152, 01152, 00232, 00410, 00510, 01362,
   00113, 00656, 01772, 00232, 00774, 01506, 00143, 00372,
   00232, 00774, 00272, 00556, 00607, 00766, 00607, 00316,
   00756, 00742, 00322, 00132, 01566, 00140, 00432, 00022,
   01020, 00316, 00553, 01502, 00117, 01156, 00706, 00617,
   00410, 00132, 01456, 00155, 00207, 00232, 00410, 01432,
   00164, 00410, 00232, 00217, 00410, 00232, 00410, 00132,
   00217, 00072, 00126, 00410, 00510, 00416, 00062, 00422,
   00410, 00616, 01062, 01027, 01222, 00032, 00410, 01274,
   00720, 01672, 01063, 00472, 01142, 01057, 00254, 00210,
   00510, 00423, 00072, 00126, 00410, 00510, 00416, 00062,
   00422, 01366, 00267, 00410, 01056, 01062, 01203, 01222,
   00410, 00232, 00132, 01472, 00251, 00410, 00732, 01156,
   00410, 00774, 00432, 01267, 00742, 01032, 01263, 00472,
   00732, 00620, 01154, 00255, 00232, 00510, 00477, 01474,
   00464, 00023, 00072, 00366, 00232, 01522, 00267, 00062,
   01332, 00347, 00332, 00132, 00572, 00572, 00632, 00372,
   00436, 00272, 00572, 01226, 00556, 00616, 01474, 01542,
   00322, 01732, 01772, 00216, 00432, 00132, 01274, 00530,
   01772, 01613, 00742, 01632, 01553, 00532, 00732, 01344,
   00345, 00620, 01746, 00402, 01557, 00232, 00503, 00032,
   00432, 00503, 00072, 00366, 00232, 00432, 00410, 00432,
   00774, 00130, 00372, 00232, 00410, 00232, 01137, 00432,
   01262, 00774, 00130, 00073, 00432, 01747, 00630, 01130,
   00330, 00130, 00430, 00730, 00130, 01030, 00030, 00530,
   00530, 01146, 00774, 01020, 00343, 00567, 00467, 00553,
   00670, 00222, 00774, 00346, 00650, 01170, 01674, 00706,
   00706, 00706, 00206, 00216, 01150, 00447, 00372, 00410,
   01170, 00266, 00166, 01726, 01726, 01726, 00374, 00346,
   00356, 01150, 00670, 00372, 00770, 00650, 01070, 00750,
   00226, 01772, 01772, 01772, 00362, 00410, 00372, 01020,
   00770, 01050, 00670, 00750, 00332, 00222, 00262, 00650,
   01170, 00226, 00474, 00706, 00706, 00706, 00226, 01150,
   00206, 01646, 01646, 01646, 01020, 00770, 00374, 00372,
   00410, 01170, 00620, 01772, 00354, 00522, 00362, 00232,
   00410, 01020, 01070, 00474, 00477, 00670, 01674, 00477,
   00072, 00366, 00232, 00614, 01247, 00614, 01014, 00062,
   00232, 00272, 00556, 01073, 01522, 00621, 00232, 00725,
   00774, 00232, 00436, 00272, 01053, 00132, 00172, 00410,
   00510, 01672, 01532, 00577, 00510, 00232, 01020, 00756,
   00747, 00774, 00642, 00132, 00064, 01217, 00644, 01752,
   00620, 00762, 00756, 01033, 00132, 01373, 00064, 01761,
   01247, 01004, 00703, 00774, 00332, 01142, 00232, 01532,
   00633, 00264, 00033, 01502, 00640, 01156, 00732, 01157,
   00410, 01021, 00703, 00072, 00366, 00232, 00604, 01326,
   01756, 01014, 00062, 01522, 00267, 01326, 00267, 00216,
   00256, 01356, 00623, 00516, 01337, 01256, 01004, 00036,
   00236, 00032, 00774, 01006, 01142, 00742, 00172, 00410,
   00510, 01417, 01672, 01162, 01413, 00510, 00472, 01122,
   01367, 00762, 00132, 00732, 00620, 01254, 00676, 00232,
   00172, 00706, 00706, 00706, 00232, 01474, 00730, 01216,
   01316, 01024, 01374, 01672, 00372, 00264, 00661, 00372,
   00264, 00155, 01366, 00777, 01154, 00733, 00072, 01474,
   00142, 00472, 01672, 00372, 00364, 01245, 01024, 00765,
   00132, 01032, 00132, 00472, 00132, 01174, 00264, 00155,
   01366, 00777, 01672, 01733, 01672, 00264, 00013, 01522,
   00774, 01156, 00436, 01034, 01006, 01262, 00064, 00461,
   00634, 01040, 00164, 00171, 00064, 00771, 00510, 01362,
   01044, 01116, 00372, 00077, 01672, 01321, 00132, 01374,
   00164, 01567, 00472, 01142, 00153, 00402, 00756, 00720,
   01020, 00072, 00366, 00232, 01004, 01522, 01050, 01014,
   00036, 00132, 00332, 00556, 00347, 00332, 01522, 01075,
   01274, 00620, 01244, 01213, 00756, 00307, 00661, 00372,
   01007, 00364, 01245, 01374, 00413, 00132, 00656, 01732,
   00243, 00766, 01032, 00407, 00472, 00732, 01156, 00507,
   01674, 01342, 01117, 01142, 01542, 01126, 00742, 00774,
   01047, 00232, 00726, 00232, 01342, 01102, 00432, 00774,
   01146, 00272, 00574, 00130, 01034, 01137, 01256, 00132,
   00272, 00434, 01151, 00164, 00341, 00064, 01735, 00164,
   00101, 00132, 00064, 00503, 00432, 00244, 00376, 01166,
   00430, 00766, 01544, 01671, 00444, 01706, 00144, 01722,
   01644, 01734, 00644, 01744, 01474, 00330, 01374, 01020,
   00742, 01032, 01003, 00472, 00644, 01214, 00732, 01156,
   00620, 00372, 00333, 00372, 00434, 01221, 01321, 00232,
   00132, 01274, 00630, 01674, 01366, 01312, 00720, 01342,
   01236, 01142, 00172, 00410, 00510, 01273, 00756, 01672,
   01162, 01123, 01772, 01772, 01772, 00642, 00132, 00272,
   01034, 01141, 00064, 01665, 00607, 01732, 01162, 01267,
   00472, 00662, 00510, 01137, 00410, 00510, 00172, 00372,
   00572, 00572, 00632, 00132, 01772, 01572, 01302, 00510,
   00232, 01020, 00656, 01363, 01232, 00422, 00410, 00756,
   00064, 01203, 00132, 00436, 00272, 01034, 01322, 01262,
   00164, 00725, 00434, 01326, 00164, 00341, 00064, 01761,
   00647, 00072, 00366, 00232, 00164, 00341, 01210, 00710,
   01522, 01771, 01717, 01356, 00267, 01156, 00736, 01526,
   01343, 01556, 01363, 00222, 00262, 00562, 00562, 00622,
   01362, 01363, 00704, 01210, 00022, 00232, 00414, 00164,
   01215, 01010, 00734, 01375, 01262, 00464, 00013, 01460,
   00040, 00040, 00040, 00040, 00040, 00040, 00040, 00774,
   01562, 00267, 01552, 00267, 00272, 00172, 00736, 01506,
   01426, 00656, 01416, 01432, 00752, 01020, 01116, 00067,
   00064, 01337, 00432, 00742, 01772, 00762, 00372, 01302,
   01443, 01706, 00756, 00032, 01602, 00247, 00732, 00472,
   00237, 01622, 00303, 01646, 00672, 00756, 00472, 00273,
   00106, 01142, 00177, 01162, 00177, 00732, 00156, 00436,
   00446, 00532, 00236, 01020, 01514, 00377, 01504, 01211,
   00432, 00774, 00130, 00731, 00050, 00170, 00272, 00710,
   00731, 00150, 00710, 00272, 00015, 00270, 01105, 00250,
   01210, 00370, 00232, 00731, 00350, 01210, 00232, 00272,
   00015, 00470, 01105, 00450, 00710, 01210, 00015, 00570,
   01105, 00550, 01201, 00464, 00027, 01211, 00370, 00232,
   01566, 01555, 01174, 00464, 00023, 00031, 00751, 01141,
   01211, 00170, 00232, 00031, 00464, 00003, 01534, 01571,
   01262, 00001, 01566, 01575, 00432, 01410, 00774, 01352,
   01606, 01156, 00752, 01152, 01037, 00756, 01020, 00552,
   01057, 00432, 01077, 00406, 01146, 00412, 00620, 00620,
   01020, 01534, 01626, 00232, 01262, 00232, 00005, 00753,
   00272, 01124, 01635, 00710, 01310, 00232, 01310, 01020,
   00710, 01250, 01225, 00272, 01020, 00432, 01160, 00070,
   01020, 00774, 00230, 00330, 00030, 00230, 00530, 01030,
   00530, 00030, 01130, 00230, 01130, 01130, 00430, 00372,
   01020, 00330, 00130, 00030, 00130, 00730, 01130, 01030,
   00030, 00430, 00330, 00230, 00474, 01020, 00374, 00330,
   00330, 00030, 01030, 00530, 00330, 00130, 00630, 01030,
   00674, 01020, 01374, 00330, 00330, 00330, 00030, 01030,
   00330, 00530, 01574, 01020, 00174, 00330, 00330, 00330,
   00330, 00130, 00374, 01020, 00574, 00330, 00330, 00330,
   00274, 01020, 00132, 00372, 00264, 00127, 00164, 00171,
   01366, 00267, 00510, 01562, 00267, 00032, 00072, 00264,
   00607, 00226, 01552, 00267, 00264, 01633, 00000, 00533,
   00117, 00043, 00333, 01474, 01653, 01104, 00610, 00137,
   00310, 00110, 00010, 00432, 00564, 01345, 01260, 00430,
   00232, 01710, 01110, 00364, 00751, 00610, 01114, 00435,
   01735, 00451, 00664, 01541, 00710, 00016, 01444, 01237,
   01544, 00477, 00020, 00114, 01157, 00674, 00147, 00064,
   00745, 01567, 00364, 00363, 00764, 00757, 00764, 01163,
   00357, 00227, 00507, 00367, 00064, 01351, 00232, 00710,
   01250, 00232, 00003, 00474, 00147, 00064, 01651, 00013,
   00517, 01057, 01303, 01254, 01117, 01074, 00147, 01314,
   00214, 00314, 00414, 00614, 00714, 01014, 01214, 01514,
   01020, 00264, 01547, 01210, 01156, 01156, 00064, 00745,
   00013, 00432, 00027, 00574, 00023, 00656, 00604, 01207,
   00743, 01473, 01477, 01254, 00150, 01174, 00147, 01262,
   01010, 00064, 00061, 00013, 01374, 00147, 01674, 00147,
   00547, 00263, 00273, 01244, 00175, 01310, 00027, 00656,
   00656, 01224, 00133, 01004, 00607, 01234, 01723, 00727,
   00244, 00052, 01224, 00052, 00654, 00052, 00370, 00364,
   01141, 00170, 00003, 00244, 01630, 00444, 01630, 01237,
   01003, 00663, 00673, 00113, 01010, 01310, 00232, 00003,
   01074, 00023, 00104, 00004, 01710, 00137, 01010, 00533,
   00567, 00656, 00737, 01254, 00147, 00174, 00147, 00144,
   01630, 01644, 01630, 01224, 01630, 01204, 00153, 00656,
   00244, 01432, 00444, 00364, 00144, 00043, 01644, 00232,
   00624, 00133, 01224, 00302, 00656, 00420, 00656, 01410,
   00652, 01367, 00232, 01160, 00070, 00232, 00644, 01304,
   01244, 00341, 00044, 00346, 01537, 00000, 00656, 00656,
   00656, 00656, 01277, 01254, 00236, 00574, 00147, 00744,
   00347, 00344, 00047, 00064, 01111, 00364, 00751, 01544,
   00230, 01360, 00432, 01160, 00710, 00003, 01262, 00064,
   00061, 01567, 00432, 01160, 00420, 01172, 00012, 01344,
   01266, 00652, 00620, 01677, 00014, 00114, 01163, 00710,
   00372, 01324, 01373, 01410, 00332, 00414, 00272, 00024,
   00427, 01552, 00421, 01074, 00130, 00030, 01416, 00427,
   00016, 01610, 00456, 01416, 00416, 00153, 01156, 00256,
   00153, 00016, 01610, 00656, 00516, 00137, 00153, 00404,
   00016, 01610, 00332, 00422, 00774, 00620, 01116, 00167,
   00424, 00445, 01352, 00445, 00720, 00032, 00246, 00536,
   00006, 00236, 01577, 00364, 00373, 01004, 01264, 00507,
   00337, 00347, 00620, 00620, 00620, 00464, 00147, 00474,
   00327, 00674, 00327, 00332, 00272, 00022, 01723, 00020,
   00543, 01273, 00567, 00404, 00661, 00330, 00730, 01030,
   00530, 00430, 00130, 00130, 00730, 01030, 00430, 00424,
   00525, 00064, 00745, 00464, 00013, 00064, 01111, 00517,
   00232, 00656, 00656, 00170, 00527, 00164, 00601, 00517,
   00267, 01773, 01407, 00661, 00130, 01030, 00064, 00745,
   01131, 00064, 00071, 00517, 00272, 00432, 00774, 01020,
   01017, 00763, 00727, 00774, 00327, 00010, 01345, 01260,
   00610, 00710, 00464, 00137, 01345, 01350, 01450, 01550,
   01650, 01750, 00753, 00432, 00050, 00150, 00250, 00350,
   00450, 00550, 00753, 00756, 00620, 01653, 00364, 00627,
   00257, 01073, 01340, 01340, 01264, 01363, 00432, 00756,
   00774, 00330, 00230, 01020, 00704, 00404, 01364, 00453,
   01263, 01257, 01253, 00404, 00661, 00756, 00230, 00530,
   00430, 00477, 01204, 00604, 01164, 00363, 00164, 00601,
   00432, 00410, 00510, 00364, 01245, 00372, 00064, 01135,
   00517, 00432, 01074, 00130, 01160, 01260, 00432, 01160,
   01020, 01164, 00343, 01102, 01116, 01437, 01723, 00662,
   01410, 01156, 00620, 00420, 00642, 01417, 01423, 00000,
   01163, 01173, 01167, 00404, 00661, 01156, 00430, 00530,
   00330, 00530, 01130, 00230, 00330, 00730, 00477, 01362,
   00753, 00434, 00613, 00756, 01352, 00752, 01152, 01352,
   00473, 00752, 01776, 00276, 00420, 01106, 00256, 01410,
   00434, 01361, 00124, 01201, 01352, 00771, 01410, 01216,
   01152, 00216, 00764, 01275, 00664, 01757, 00664, 00047,
   00040, 00040, 00040, 00040, 00040, 00040, 00040, 00040,
   00040, 00032, 00111, 00372, 00710, 00021, 00761, 00035,
   00464, 00013, 01064, 01153, 00272, 00761, 00015, 00111,
   00041, 00103, 00226, 00116, 01020, 00032, 00364, 01245,
   00710, 00021, 00264, 00221, 00103, 00364, 00035, 00103,
   00032, 00774, 00642, 01020, 00604, 01504, 01064, 00327,
   00337, 00373, 01647, 01303, 00272, 00015, 00103, 00474,
   00464, 00147, 01124, 01075, 01310, 01020, 00674, 00343,
   01223, 00167, 00463, 00540, 01414, 01614, 00753, 01414,
   00453, 01404, 01604, 00753, 00264, 00205, 00103, 01366,
   00003, 01010, 00031, 00103, 00710, 00272, 00072, 00366,
   00164, 00341, 00710, 00021, 01364, 00711, 00164, 00343,
   01473, 00123, 00673, 00272, 00564, 01131, 01262, 00001,
   00432, 00774, 00130, 01030, 00035, 00103, 01064, 00003,
   00423, 00437, 00447, 00232, 00072, 00210, 01714, 01724,
   01166, 00310, 00464, 00137, 00432, 00774, 01130, 00756,
   01020, 00356, 00652, 01116, 01074, 00416, 00330, 01616,
   01037, 00516, 01756, 01616, 01057, 01153, 01364, 00123,
   00227, 01143, 01133, 01147, 01564, 00517, 01364, 01143,
   01340, 01340, 00516, 00316, 00774, 00564, 01423, 00020,
   00267, 01204, 00263, 00540, 01210, 00222, 01262, 00222,
   00001, 00232, 01210, 00232, 01156, 01156, 00035, 00103,
   00351, 00111, 00572, 00766, 00416, 01677, 00732, 00732,
   00174, 00202, 00474, 01630, 01230, 01230, 01430, 01230,
   00232, 00072, 01541, 00753, 01124, 01307, 01310, 00432,
   01160, 00232, 01677, 00262, 00372, 01020, 01264, 00507,
   01533, 01073, 01523, 00540, 01364, 00347, 01364, 00207,
   00310, 01410, 00210, 01714, 01724, 01333, 01734, 01336,
   00310, 01020, 01376, 00427, 01672, 00756, 01356, 01371,
   01623, 00351, 01270, 01677, 01366, 00030, 01262, 00464,
   00003, 00022, 01512, 01342, 01737, 00662, 01116, 01516,
   01365, 00420, 01116, 00432, 00222, 00262, 00420, 00652,
   00137, 01112, 01322, 01406, 00774, 00430, 01274, 00620,
   01122, 00037, 00742, 00244, 01313, 00720, 00244, 01313,
   00720, 00244, 01313, 00720, 00742, 00742, 00053, 01674,
   00630, 00007, 01334, 01444, 00372, 00332, 00416, 00420,
   00224, 01514, 00226, 00233, 00611, 01166, 00716, 00716,
   01174, 00706, 00742, 00317, 00726, 00720, 00254, 01452,
   00742, 00317, 00373, 01142, 00754, 01471, 00304, 00416,
   00377, 00324, 01474, 00662, 00620, 01666, 00606, 00266,
   00256, 00620, 01106, 01366, 01506, 00655, 00234, 01511,
   01275, 00610, 00464, 00143, 00216, 01674, 01646, 01646,
   01474, 00202, 00716, 00174, 01646, 00216, 00256, 01410,
   01352, 01534, 00412, 01216, 00655, 00364, 00751, 00254,
   00000, 00433, 01124, 01545, 01310, 01114, 01304, 00432,
   00276, 00420, 01020, 01410, 00172, 00016, 00520, 00136,
   00226, 00616, 00774, 01542, 01032, 01156, 00620, 00224,
   01563, 00726, 00717, 01324, 01606, 00611, 00774, 00130,
   00272, 01106, 00016, 00204, 00304, 00433, 00224, 01623,
   01366, 01626, 01674, 00226, 00266, 00406, 01366, 01623,
   00710, 01306, 01602, 00774, 00464, 00147, 00625, 00773,
   00304, 01324, 01623, 00223, 01334, 01354, 00224, 01647,
   01366, 01643, 01262, 00420, 01116, 00000, 00447, 01674,
   01646, 01646, 01646, 00212, 01252, 00252, 00527, 01674,
   00706, 00706, 00706, 01020, 01474, 00272, 01160, 00070,
   01632, 01532, 01711, 01142, 01327, 01260, 00416, 00774,
   00742, 00610, 00710, 01020, 00534, 01676, 01447, 00432,
   00610, 01574, 00464, 00023, 01474, 01160, 01360, 01142,
   01467, 01020, 01460, 00010, 00420, 00772, 00610, 00432,
   00610, 00410, 00410, 01110, 01110, 01110, 01010, 01710,
   01710, 00132, 00372, 01172, 01572, 01707, 00774, 00742,
   00610, 00710, 01474, 01730, 01461, 01321, 01074, 00130,
   00330, 01461, 00330, 01321, 00514, 00264, 01775, 01421,
   01511, 01421, 01264, 01311, 01421, 01264, 01677, 01142,
   00710, 01010, 01366, 00050, 01362, 00011, 00704, 01204,
   00422, 00064, 01111, 01566, 00016, 00032, 00164, 00341,
   00164, 00565, 00064, 00771, 00064, 01761, 00064, 01365,
   00710, 00422, 00064, 00755, 01310, 00164, 00565, 00232,
   00272, 00343, 01224, 00046, 01204, 01020, 01214, 01020,
   00232, 00704, 01362, 00055, 00404, 00422, 01310, 01366,
   00373, 00032, 01513, 00211, 00307, 00072, 00272, 00126,
   01362, 00102, 00404, 01534, 00102, 01234, 00102, 01214,
   00704, 00414, 00774, 00216, 00256, 00556, 00443, 01307,
   01516, 00130, 00332, 01332, 00061, 00774, 01142, 01572,
   00130, 00624, 00063, 01201, 00032, 01116, 00232, 01513,
   00624, 00375, 00064, 01665, 00211, 00132, 00332, 00774,
   00436, 00756, 01356, 00152, 00762, 00620, 00654, 00141,
   00372, 01513, 00410, 00432, 00762, 01772, 00757, 00232,
   00410, 00742, 00262, 00410, 01732, 01732, 01122, 00723,
   00022, 00472, 00232, 00172, 01632, 00677, 00410, 00762,
   00410, 00132, 00732, 00620, 00654, 00173, 00372, 00064,
   01245, 00132, 00410, 00416, 00274, 00372, 01775, 00372,
   01113, 00472, 01142, 01107, 01672, 00402, 01366, 00317,
   00720, 01067, 00432, 00410, 01201, 00572, 01772, 01020,
   00774, 00432, 00730, 01030, 00530, 00330, 01130, 01030,
   00130, 00630, 00330, 00330, 01130, 00730, 00530, 00774,
   01020, 00634, 00135, 00164, 00341, 00164, 00341, 00064,
   01761, 00164, 00101, 00064, 01735, 00164, 00171, 00064,
   00771, 00022, 00064, 01365, 00164, 00171, 00064, 01711,
   00022, 00216, 00256, 00556, 00553, 00567, 00720, 01156,
   00254, 00316, 00436, 00064, 00461, 01234, 00335, 01151,
   01262, 00222, 00422, 00064, 00105, 00734, 00342, 01151,
   00064, 00105, 01424, 00361, 01151, 00656, 00656, 00064,
   01135, 01624, 00361, 00432, 00774, 01130, 01156, 00064,
   00755, 00434, 00364, 01262, 01524, 00373, 00364, 00751,
   01010, 01310, 00232, 00464, 00013, 00464, 00017, 00432,
   01172, 00422, 00244, 00420, 01444, 00442, 01544, 00452,
   00444, 00460, 00144, 00464, 01474, 00730, 00274, 01020,
   00674, 00630, 00630, 01030, 00630, 00530, 00230, 00430,
   01130, 00130, 00130, 00630, 00774, 01020, 00630, 01344,
   00415, 00173, 00374, 00171, 01474, 00530, 00174, 01030,
   00474, 01020, 01374, 00171, 01474, 01130, 00674, 01020,
   00174, 00171, 01574, 01020, 00574, 00171, 00374, 01020,
   01504, 00604, 01010, 00232, 00032, 00072, 00226, 01362,
   00510, 00704, 00634, 00506, 01234, 00507, 00404, 00422,
   00372, 01424, 00624, 01634, 00521, 00232, 00272, 01772,
   01632, 00432, 00774, 00430, 00530, 00372, 01156, 01552,
   00535, 01156, 00567, 00756, 01672, 00372, 00410, 00510,
   00572, 00572, 00572, 01772, 00372, 01552, 00571, 01032,
   00637, 00472, 00732, 01156, 00637, 00432, 00372, 00510,
   00572, 01434, 00565, 01672, 01772, 00372, 01032, 01013,
   00472, 00372, 00510, 00372, 01434, 00635, 01556, 00634,
   00732, 01167, 01224, 00613, 01204, 00424, 00611, 00404,
   00733, 00414, 00733, 01214, 00634, 00605, 00734, 00622,
   00714, 00733, 00704, 00733, 01064, 01201, 00573, 00132,
   01032, 01064, 00211, 01213, 00756, 01552, 00642, 01032,
   01137, 00472, 01156, 00064, 00461, 01424, 00660, 00510,
   00572, 01156, 00064, 01121, 01064, 01151, 00064, 00771,
   00410, 00232, 00272, 00756, 01357, 00132, 00732, 01373,
   00620, 00644, 00760, 00756, 01343, 00132, 00432, 00372,
   01064, 01775, 00372, 01427, 00762, 01032, 01423, 00472,
   00620, 01776, 00732, 00654, 00677, 00410, 01672, 01672,
   00432, 00774, 00130, 00410, 01474, 00630, 00630, 01613,
   01646, 01646, 01122, 01543, 00022, 00410, 00232, 01072,
   00472, 00410, 00172, 00262, 01142, 01553, 00232, 00726,
   00232, 01366, 01043, 01162, 01156, 00022, 01672, 01613,
   00510, 01524, 01063, 01234, 01063, 00064, 01665, 01264,
   00317, 00710, 00410, 00510, 00416, 01264, 00113, 00000,
   00040, 00040, 00040, 00040, 00040, 00040, 00040, 00040,
   00040, 00040, 00040, 00140, 00140, 00140, 00140, 00140,
   00364, 00753, 00045, 01310, 00061, 00025, 01234, 01033,
   01010, 01310, 00232, 00101, 00734, 01037, 01262, 01010,
   01310, 00232, 00413, 00422, 00410, 00232, 00510, 01132,
   01524, 01054, 01224, 01056, 01216, 00132, 01326, 01107,
   00410, 00064, 01215, 00634, 01102, 00055, 00232, 00272,
   00410, 00332, 00025, 00064, 01761, 00051, 01524, 00771,
   00064, 01665, 00434, 01105, 01262, 00464, 00013, 00432,
   00774, 01146, 00574, 00130, 00272, 00172, 00634, 01105,
   00317, 00372, 00545, 00427, 00506, 01746, 01546, 01124,
   01020, 01326, 01144, 00774, 00316, 00756, 00756, 01552,
   01205, 00620, 01354, 01146, 00332, 01020, 01156, 00607,
   00432, 00326, 01034, 01171, 00720, 01154, 01161, 01051,
   00723, 00720, 01051, 00620, 00620, 01051, 00272, 00332,
   01017, 00032, 01041, 00720, 01144, 01177, 00720, 01041,
   00732, 00532, 00372, 00064, 00423, 01312, 01144, 00643,
   01746, 00606, 00246, 01746, 00546, 00546, 01046, 01034,
   01124, 00032, 00256, 00632, 00416, 01020, 00061, 01522,
   01606, 00065, 01522, 01606, 00364, 01211, 00072, 00126,
   00064, 01735, 00055, 00071, 00061, 00045, 00364, 01225,
   00035, 00051, 00424, 01730, 00101, 00364, 01141, 00404,
   00065, 01223, 01460, 00071, 00041, 00101, 00364, 01141,
   00032, 00072, 01364, 01653, 01514, 01250, 00272, 00022,
   01362, 01303, 01504, 00432, 01274, 01130, 00130, 00230,
   01030, 00001, 01522, 01345, 00710, 00422, 00610, 00664,
   00521, 01464, 00761, 00075, 01564, 00125, 00264, 00221,
   00061, 00045, 01564, 00405, 00025, 01524, 01343, 01564,
   00125, 00064, 01761, 00464, 00003, 00664, 00521, 01464,
   00101, 00564, 00665, 01156, 00530, 00005, 01617, 01564,
   01765, 00764, 01421, 00432, 00774, 00230, 00354, 01365,
   00372, 00332, 00566, 00566, 00232, 00664, 01541, 00040,
   00040, 00040, 00040, 00040, 00040, 00040, 00040, 00040,
   00040, 00040, 00040, 00140, 00140, 00140, 00140, 00140,
   00164, 00627, 00264, 00223, 01105, 01045, 00055, 00710,
   01105, 00001, 00061, 00025, 00051, 01761, 00101, 00464,
   00013, 00735, 00624, 01437, 00272, 00774, 01102, 01506,
   01455, 00432, 01146, 00412, 00607, 00272, 00001, 00022,
   00055, 00710, 00422, 01105, 01051, 00061, 00041, 00101,
   00603, 00735, 00624, 01437, 00272, 01075, 00064, 01761,
   00051, 00064, 01761, 00061, 00041, 00710, 00422, 00005,
   00101, 00607, 00735, 00424, 01522, 00634, 01524, 01105,
   00232, 00177, 00624, 01437, 00422, 00264, 00205, 00055,
   00434, 01550, 00065, 00041, 00065, 00011, 00724, 01546,
   00711, 00272, 00710, 00262, 00232, 00177, 00075, 00055,
   00064, 01665, 01761, 00724, 01557, 00711, 00177, 00065,
   00041, 00607, 00432, 00774, 01156, 00530, 00023, 01372,
   01601, 01352, 01602, 00216, 01474, 00530, 01216, 01416,
   01602, 00604, 00710, 01020, 00364, 01227, 01174, 00464,
   00023, 00232, 01262, 00000, 00003, 00272, 01262, 00015,
   00057, 00664, 00243, 00604, 01004, 01153, 00364, 01213,
   00404, 01204, 01021, 01105, 01045, 01562, 01606, 01366,
   01606, 01514, 00714, 00370, 01262, 00272, 00170, 01075,
   00570, 00272, 01021, 00015, 01761, 00075, 01524, 01666,
   01504, 00370, 01065, 00470, 00272, 01253, 00724, 01676,
   00704, 00170, 01065, 00270, 00272, 01253, 00424, 01732,
   01224, 01226, 00624, 01750, 01024, 01750, 00071, 01721,
   00370, 01262, 00005, 00061, 00025, 00075, 00071, 00170,
   00021, 01761, 00071, 00045, 01021, 00035, 00634, 01437,
   00464, 00003, 00061, 00065, 00025, 01522, 01606, 01366,
   01606, 00710, 00664, 00351, 00051, 00071, 00041, 01543,
   00061, 01721, 00170, 01262, 00005, 00071, 00025, 00055,
   00055, 00071, 00370, 01503, 01326, 01606, 00624, 01263,
   01021, 00272, 00710, 00017, 00061, 00013, 00000, 01672,
   00064, 00773, 00040, 00432, 00410, 00564, 00667, 00164,
   00567, 00040, 00164, 00467, 00432, 01262, 00756, 00023,
   00035, 00015, 00530, 01130, 00230, 01030, 01030, 00530,
   00730, 00230, 00430, 00430, 00330, 01030, 00011, 00025,
   00756, 00021, 00430, 01030, 00630, 01130, 00530, 01130,
   01130, 00330, 00030, 00630, 01130, 00230, 00045, 00035,
   00011, 00015, 00230, 00630, 00230, 00430, 00330, 00330,
   00130, 00230, 00130, 00630, 00730, 01130, 00011, 00061,
   00230, 01130, 01030, 00230, 00130, 00330, 00530, 00530,
   00730, 01030, 00030, 01030, 00045, 00035, 00011, 00015,
   00530, 00730, 00530, 01030, 01030, 00530, 00430, 01030,
   00030, 00430, 00530, 01030, 00011, 00164, 00341, 00051,
   01564, 00361, 00021, 00330, 01130, 01130, 01130, 00030,
   00330, 00430, 00330, 01030, 00530, 00030, 00430, 00001,
   00035, 00045, 01564, 00405, 01174, 00430, 00430, 00430,
   00011, 00164, 00551, 00003, 00051, 00015, 00330, 01130,
   01130, 00030, 00130, 01130, 00430, 00130, 00730, 00030,
   00130, 00130, 00011, 00432, 00756, 00021, 00330, 00030,
   00730, 01030, 01130, 01130, 00330, 00330, 00030, 00330,
   00430, 00045, 00051, 00011, 00432, 01156, 00021, 00730,
   00430, 00230, 00330, 01030, 00030, 01130, 00230, 00430,
   00030, 00230, 00730, 00011, 00061, 00130, 00530, 00130,
   00530, 00030, 01030, 01130, 00730, 00230, 00430, 00530,
   00130, 00045, 00015, 00430, 01030, 00330, 01030, 00530,
   01130, 00130, 00230, 01030, 00030, 01030, 00011, 00051,
   00011, 00015, 00530, 00230, 01130, 00330, 00330, 00030,
   00330, 00230, 00430, 01130, 00230, 00630, 00045, 01564,
   00361, 00021, 00130, 00530, 00130, 00630, 00730, 01130,
   00130, 00130, 00630, 00630, 00330, 00530, 00011, 00051,
   00011, 00015, 00130, 01130, 01030, 00630, 00130, 00530,
   00330, 01030, 00130, 00330, 00630, 00430, 00045, 00051,
   00011, 00432, 01474, 00430, 01216, 00021, 01564, 00147,
   00040, 00040, 00040, 00040, 00040, 00040, 00040, 00040,
   00040, 00040, 00040, 00140, 00140, 00140, 00140, 00140,
   00264, 00223, 00432, 01464, 00023, 00222, 01262, 00222,
   01020, 00330, 01130, 01030, 00030, 00630, 00430, 00730,
   01130, 00430, 00011, 00111, 00130, 01374, 00630, 00130,
   00530, 00330, 00030, 00230, 00045, 00065, 00011, 00432,
   01262, 01474, 01030, 01216, 00115, 00330, 01030, 00030,
   00530, 00230, 00013, 00272, 00432, 00774, 01262, 01156,
   01020, 00432, 01156, 00410, 00432, 00774, 00330, 01130,
   01030, 01130, 00430, 00230, 00230, 01030, 00030, 00330,
   01030, 00530, 01020, 01362, 00527, 00464, 00017, 01552,
   00550, 01372, 00541, 00664, 00241, 01072, 01572, 00525,
   01004, 01172, 00412, 01034, 00546, 00422, 00464, 00013,
   01250, 01514, 00355, 01130, 00001, 01362, 00625, 00710,
   00355, 00130, 00001, 01522, 00634, 00710, 00355, 00530,
   00001, 01262, 00610, 00405, 00041, 00055, 00061, 00025,
   00664, 00561, 01464, 00101, 00710, 00005, 00075, 00065,
   00101, 00061, 00025, 00405, 00041, 00125, 00075, 00071,
   00061, 00011, 00055, 01635, 00773, 00664, 00241, 00710,
   01262, 00001, 00610, 01167, 01504, 00710, 00164, 00601,
   00111, 01262, 00230, 00021, 00051, 00055, 00115, 01030,
   00005, 01464, 00061, 01130, 00430, 00045, 00061, 00011,
   00115, 00130, 00330, 00756, 00005, 00432, 01262, 00115,
   00630, 00130, 00045, 00061, 00011, 00055, 00061, 00025,
   00664, 00561, 00075, 00055, 00065, 00101, 00210, 00055,
   01464, 00761, 00064, 01665, 00125, 00055, 00065, 00405,
   00041, 00710, 00021, 00061, 00011, 00071, 00025, 00055,
   00064, 01761, 00071, 00041, 00055, 00164, 00615, 00115,
   00230, 00021, 00022, 00051, 00075, 01635, 01443, 00055,
   00071, 01326, 00763, 01512, 00756, 01707, 01074, 00642,
   01717, 00210, 00067, 00310, 00065, 00132, 00064, 00501,
   01534, 00773, 01262, 00464, 00003, 01460, 00000, 01475
};
