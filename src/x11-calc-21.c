/*
 * x11-calc-21.c - RPN (Reverse Polish) calculator simulator.
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
 * 09 Jun 14   0.1   - Initial version - MT
 * 10 Mar 14         - Changed indexes to BCD hex values - MT
 * 10 Dec 18         - Alternate  function key now LIGHT_BLUE, allowing  it
 *                     to be a different colour to the alternate text - MT
 * 31 Aug 20         - Made  version(), about(), and error() model specific
 *                     so that the name reflects the model number - MT
 *                   - Modified version() to display a leading zero in  the
 *                     date instead of a space - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 10 Aug 21         - Added HP21 ROM contents - MT
 * 30 Sep 21         - Added the additional properties for the label colour
 *                     and alternate function colour - MT
 * 04 Nov 21         - Allows size of the window to be changed by modifying
 *                     the value of SCALE at compile time - MT
 * 23 Nov 21         - Added the alternate function text - MT
 * 06 Dec 21         - Label text colour now explicitly defined - MT
 * 03 Jan 22         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 11 Jan 22         - Removed ROM_BANKS - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0012"
#define DATE           "11 jan 22"
#define AUTHOR         "MT"

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

   /* Define the switches. */
   h_switch[0] = h_switch_create(00000, "OFF", "ON ", h_alternate_font, KEYBOARD_COL_A, KEYBOARD_ROW_0, ENTER_KEY_WIDTH, SWITCH_HEIGHT, True, MID_GREY, DARK_GREY);
   h_switch[1] = h_switch_create(00000, "DEG", "RAD", h_alternate_font, KEYBOARD_COL_D, KEYBOARD_ROW_0, ENTER_KEY_WIDTH, SWITCH_HEIGHT, True, MID_GREY, DARK_GREY);

   /* Define top row of keys. */
   h_button[0] = h_button_create(00264, 000, "1/x", "", "", "yX", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_A, KEYBOARD_ROW_1, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[1] = h_button_create(00263, 000, "SIN", "", "", "SIN-\xb9", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_B, KEYBOARD_ROW_1, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[2] = h_button_create(00262, 000, "COS", "", "", "COS-\xb9", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_C, KEYBOARD_ROW_1, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[3] = h_button_create(00261, 000, "TAN", "", "", "TAN-\xb9", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_D, KEYBOARD_ROW_1, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[4] = h_button_create(00260, 'f', "", "", "", "", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_E, KEYBOARD_ROW_1, SMALL_KEY_WIDTH, KEY_HEIGHT, False, LIGHT_BLUE, BACKGROUND, LIGHT_BLUE, LIGHT_BLUE);

   /* Define second row of keys. */
   h_button[5] = h_button_create(00104, 000, "X-Y", "", "", "-R", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_A, KEYBOARD_ROW_2, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[6] = h_button_create(00103, 000, "R", "", "", "-P", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_B, KEYBOARD_ROW_2, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[7] = h_button_create(00102, 000, "eX", "", "", "LN", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_C, KEYBOARD_ROW_2, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[8] = h_button_create(00101, 000, "STO", "", "", "LOG", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_D, KEYBOARD_ROW_2, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[9] = h_button_create(00100, 000, "RCL", "", "", "10x", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_E, KEYBOARD_ROW_2, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);

   /* Define third row of keys. */
   h_button[10] = h_button_create(00324, 015, "ENTER", "", "", "", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_A, KEYBOARD_ROW_3, ENTER_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[11] = h_button_create(00322, 'c', "CHS", "", "", "/\xaf", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_C, KEYBOARD_ROW_3, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[12] = h_button_create(00321, 'e', "EEX", "", "", "\x1c", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_D, KEYBOARD_ROW_3, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);
   h_button[13] = h_button_create(00320, 033, "CLX", "", "", "CLR", h_normal_font, h_small_font, h_alternate_font, KEYBOARD_COL_E, KEYBOARD_ROW_3, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BLACK, BACKGROUND, MID_BLUE, BLACK);

   /* Define fourth row of keys. */
   h_button[14] = h_button_create(00144, '-', "-", "", "", "M-", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_1, KEYBOARD_ROW_4, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[15] = h_button_create(00143, '7', "7", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_2, KEYBOARD_ROW_4, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[16] = h_button_create(00142, '8', "8", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_3, KEYBOARD_ROW_4, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[17] = h_button_create(00141, '9', "9", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_4, KEYBOARD_ROW_4, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);

   /* Define fifth row of keys. */
   h_button[18] = h_button_create(00244, '+', "+", "", "", "M+", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_1, KEYBOARD_ROW_5, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[19] = h_button_create(00243, '4', "4", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_2, KEYBOARD_ROW_5, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[20] = h_button_create(00242, '5', "5", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_3, KEYBOARD_ROW_5, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[21] = h_button_create(00241, '6', "6", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_4, KEYBOARD_ROW_5, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);

   /* Define sixth row of keys. */
   h_button[22] = h_button_create(00164, '*', "\xd7", "", "", "M\xd7", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_1, KEYBOARD_ROW_6, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[23] = h_button_create(00163, '1', "1", "",  "","", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_2, KEYBOARD_ROW_6, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[24] = h_button_create(00162, '2', "2", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_3, KEYBOARD_ROW_6, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[25] = h_button_create(00161, '3', "3", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_4, KEYBOARD_ROW_6, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);

   /* Define bottom row of keys. */
   h_button[26] = h_button_create(00224, '/', "\xf7", "", "", "M\xf7", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_1, KEYBOARD_ROW_7, SMALL_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[27] = h_button_create(00223, '0', "0", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_2, KEYBOARD_ROW_7, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, YELLOW, MID_BLUE, BLACK);
   h_button[28] = h_button_create(00222, '.', ".", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_3, KEYBOARD_ROW_7, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
   h_button[29] = h_button_create(00221, 000, "DSP", "", "", "", h_large_font, h_small_font, h_alternate_font, KEYBOARD_COL_4, KEYBOARD_ROW_7, NUM_KEY_WIDTH, KEY_HEIGHT, False, BEIGE, BACKGROUND, MID_BLUE, BLACK);
}

int i_rom[ROM_SIZE] = {
   00672, 00672, 01710, 00410, 00432, 00214, 00110, 00310,
   01635, 01566, 00014, 00432, 00072, 00445, 01610, 00134,
   00120, 01015, 01112, 01512, 00264, 00272, 01363, 00006,
   01645, 01715, 01074, 00706, 01174, 00706, 00174, 01646,
   01646, 00232, 00256, 01352, 00047, 00412, 01216, 01371,
   00236, 00174, 01635, 00244, 00006, 00143, 00421, 00232,
   01630, 01230, 01230, 01430, 01230, 00232, 00066, 00110,
   00567, 01124, 00155, 01726, 00667, 01461, 00116, 00557,
   00234, 00103, 01310, 00204, 00432, 00774, 01461, 00022,
   00172, 00032, 00662, 01672, 00662, 00662, 00132, 01020,
   00516, 01015, 01512, 00242, 01461, 00116, 01074, 01502,
   00256, 01116, 01507, 00016, 00116, 00514, 00534, 00736,
   01721, 00401, 01624, 00334, 01771, 00420, 01074, 00642,
   01163, 00706, 00720, 01154, 00147, 00620, 00620, 00232,
   00272, 01146, 00216, 01410, 01526, 00345, 00036, 00236,
   01614, 01715, 00344, 00170, 00613, 00344, 00206, 00620,
   01116, 00767, 01033, 00774, 00656, 01003, 00032, 00246,
   00526, 01103, 00662, 01676, 00656, 00124, 00220, 00620,
   00156, 01020, 01114, 01715, 00344, 00223, 00114, 01124,
   00232, 00104, 01710, 00033, 01102, 00754, 00071, 01104,
   00016, 00673, 01666, 00656, 01213, 00016, 01610, 00774,
   00620, 01116, 01243, 00006, 01526, 00075, 00174, 01031,
   00113, 00110, 01504, 00037, 01461, 00116, 00216, 01352,
   00273, 01216, 01152, 00216, 01645, 00567, 01274, 00620,
   00756, 01302, 00277, 00774, 01156, 01542, 00320, 00620,
   01624, 00304, 00726, 01423, 00006, 00420, 01106, 01410,
   01020, 01726, 00547, 01524, 00674, 01634, 00362, 00174,
   00202, 01242, 00202, 00173, 01566, 00341, 00421, 00742,
   00242, 00174, 01306, 00027, 00743, 01371, 00737, 00140,
   00000, 00174, 00706, 00706, 00046, 00102, 00642, 00642,
   00102, 01020, 01262, 01514, 00262, 00210, 01714, 00274,
   00620, 01354, 00370, 01724, 00366, 01760, 01734, 00376,
   00310, 01474, 00002, 00020, 00040, 00604, 00314, 01515,
   01534, 01622, 00634, 01427, 01262, 00222, 00340, 01524,
   00567, 01110, 01157, 01104, 00574, 01020, 01524, 00627,
   01604, 01020, 01010, 01310, 00232, 01020, 00734, 00633,
   00032, 00642, 01366, 00703, 00610, 01561, 00710, 01555,
   01641, 01427, 00774, 00072, 00652, 00652, 00752, 00752,
   01416, 00463, 00232, 00226, 01366, 00467, 00232, 00366,
   01416, 01670, 01732, 00656, 01332, 01670, 00343, 00040,
   01233, 01743, 01757, 00077, 01534, 00537, 01515, 00704,
   01514, 00151, 01313, 00774, 00552, 00663, 00432, 01020,
   00340, 00432, 00474, 00730, 01030, 00530, 00330, 01130,
   01030, 00130, 00630, 00330, 00530, 00774, 01020, 00151,
   01157, 00642, 00642, 01203, 01701, 01262, 00251, 01534,
   00633, 01641, 00410, 01157, 00406, 01146, 00412, 01020,
   00642, 00642, 00642, 01543, 01701, 01555, 00637, 00704,
   01362, 00573, 00404, 00610, 00710, 01210, 01526, 00602,
   01366, 00633, 00774, 01561, 01641, 00610, 01551, 00151,
   01551, 01010, 00251, 01112, 01112, 01512, 00674, 01641,
   01403, 00040, 00117, 01020, 00774, 01701, 01347, 00375,
   00505, 00572, 00572, 00204, 00064, 00033, 00410, 01253,
   00642, 00642, 00642, 00703, 01701, 00633, 01004, 01524,
   01113, 00375, 00510, 01157, 01534, 00004, 00010, 01157,
   00040, 00033, 01204, 00027, 01524, 00766, 00032, 00774,
   00642, 01004, 01561, 00637, 01515, 00501, 00734, 00633,
   01310, 00610, 00033, 00610, 00232, 01210, 01555, 01641,
   00434, 00713, 01262, 01010, 01310, 00710, 01555, 01157,
   01263, 00133, 00040, 00240, 01524, 00363, 01310, 00023,
   00642, 01020, 00272, 00240, 01056, 00240, 00774, 00342,
   01242, 00342, 00620, 00044, 00140, 01577, 01010, 01020,
   01352, 00757, 01156, 00752, 01152, 00457, 00756, 01020,
   01534, 00746, 00410, 00272, 00510, 01020, 00151, 01204,
   01534, 00636, 00604, 01004, 01534, 01115, 00032, 01626,
   01643, 01672, 01162, 01643, 00774, 00762, 00172, 01755,
   01102, 00027, 00106, 00462, 00757, 00274, 01275, 00374,
   00341, 01574, 00335, 01541, 00674, 00335, 01001, 00474,
   00335, 00621, 00335, 01701, 00335, 00715, 00232, 01072,
   01312, 01043, 01072, 00132, 00620, 00732, 00554, 01044,
   00232, 01362, 01054, 01266, 00756, 00474, 01371, 01224,
   01332, 00634, 00633, 00715, 01565, 01153, 00340, 01672,
   00372, 00357, 00472, 01162, 00353, 00232, 00736, 00232,
   01277, 00232, 00172, 00266, 00552, 01203, 00752, 01672,
   00756, 00437, 01247, 00711, 01561, 00711, 00405, 01701,
   00474, 00331, 00621, 00674, 00331, 01001, 01574, 00331,
   01541, 00374, 00331, 00331, 00331, 01374, 00006, 01274,
   00372, 00232, 00630, 01123, 01574, 00330, 00130, 00030,
   00130, 00730, 01130, 01030, 00030, 00530, 00530, 01344,
   01212, 01533, 00272, 00432, 00774, 00230, 00330, 00030,
   00230, 00530, 01023, 00122, 00662, 01776, 00706, 00033,
   00274, 00330, 00330, 00030, 01030, 00530, 00030, 01144,
   01213, 01130, 00330, 01533, 01755, 00642, 00172, 01162,
   01063, 01646, 00232, 00736, 00232, 01122, 01073, 00132,
   00642, 01425, 00140, 01034, 01240, 00656, 01512, 01370,
   01036, 01157, 00476, 00732, 01156, 01173, 01672, 00406,
   00216, 01362, 01256, 00132, 01032, 01272, 01672, 00372,
   00432, 01166, 01034, 01270, 00430, 00766, 01357, 00630,
   00554, 01267, 01772, 01772, 01020, 00472, 01142, 01367,
   01672, 00720, 01154, 01276, 00756, 00022, 00774, 00072,
   01502, 01317, 00732, 01156, 01532, 01310, 00432, 00156,
   00472, 01522, 01300, 00226, 00272, 00072, 00774, 01020,
   01674, 01007, 01010, 00463, 01174, 00616, 01062, 01607,
   01222, 00072, 00126, 00032, 00254, 01276, 01566, 01355,
   01024, 00056, 00306, 01126, 00752, 00346, 00226, 00340,
   00630, 01130, 00330, 00130, 00430, 00730, 00130, 00657,
   00666, 01233, 01646, 01122, 01753, 00022, 00472, 01020,
   00132, 00271, 00461, 00271, 00461, 01234, 01410, 00232,
   00634, 01424, 01362, 01415, 00404, 00422, 01555, 01661,
   01551, 00235, 00505, 00461, 01555, 01534, 00436, 00032,
   00642, 00166, 00226, 01156, 01706, 01352, 01433, 01646,
   00756, 00177, 01672, 01732, 01661, 00372, 00557, 00032,
   00642, 00140, 00620, 00476, 01563, 00040, 01661, 00232,
   01342, 01463, 01232, 00272, 00316, 01423, 00240, 00732,
   01736, 00372, 00363, 00762, 01032, 00357, 00472, 00240,
   00240, 00742, 01632, 00407, 00532, 00732, 00620, 01746,
   01354, 01502, 00402, 00317, 00232, 00710, 00232, 01020,
   00140, 00372, 00174, 01577, 01706, 01122, 00523, 00762,
   00106, 00606, 00132, 00172, 01606, 00527, 00461, 01672,
   00206, 00132, 00706, 01661, 00662, 00662, 00227, 00432,
   00056, 01676, 01675, 01142, 00461, 00232, 00174, 00375,
   01374, 00331, 00374, 00331, 00574, 01030, 00674, 00331,
   01741, 00331, 00501, 00732, 00331, 00332, 01421, 00501,
   00572, 01224, 01610, 00434, 01615, 00572, 00232, 00262,
   00232, 01262, 00245, 00501, 00572, 00232, 00324, 01630,
   00232, 01555, 00432, 01142, 00756, 01534, 01633, 01551,
   00724, 00537, 00140, 00324, 01641, 01555, 00501, 00572,
   01551, 00501, 00572, 00572, 00572, 00401, 00501, 00674,
   00335, 01741, 00374, 00341, 00574, 01030, 01374, 00335,
   00174, 00335, 00335, 00132, 01772, 01274, 00530, 01523,
   01152, 01152, 00016, 01622, 01522, 01677, 00240, 01466,
   01703, 01262, 00132, 01032, 00240, 01706, 01706, 01162,
   01427, 00606, 01006, 00346, 00332, 01122, 01437, 00206,
   00461, 01322, 01400, 00732, 00206, 01661, 01706, 01162,
   00362, 01463, 00140, 00140, 00456, 01577, 01142, 00762,
   01354, 01452, 00216, 00016, 01542, 01747, 01672, 01772,
   00356, 00416, 00774, 00437, 00610, 00710, 01020, 00742,
   01036, 01677, 00476, 00736, 00620, 01354, 01760, 00317,
   01374, 01030, 00630, 00530, 00230, 00430, 01130, 01020
};
