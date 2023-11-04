/*
 * x11-calc-10a.c - RPN (Reverse Polish) calculator simulator.
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
 * 26 Nov 22         - Initial version - MT
 * 07 Dec 22         - Fixed  error at 0-141 by replacing the opcode 0x038c
 *                     (which is the opcode used for 14 -> p on the classic
 *                     series) with 0x003c (which is the equlivelent opcode
 *                     for the woodstock processor).  Note that although it
 *                     works here it is not a valid opcode and is not  used
 *                     in any HP model - MT
 *                   - Updated ROM to fix errors (0-617, 0-661, 0-66f, and
 *                     0-6a1 changed to 'data -> c') - MT
 *                   - Updated ROM to fix another error (0-651 changed  to
 *                     'data -> c') - MT
 * 14 Dec 22         - Found another (0-5b5 changed  to 'data -> c') - MT
 * 17 Dec 22         - Found another (0-352 changed  to 'data -> c') - MT
 * 20 Dec 22         - Added switch to control printer mode - MT
 * 24 Dec 22         - Made space for the third switch position - MT
 * 21 Oct 23         - Updated switch parameters to accomodate a 3 position
 *                     switch - MT
 * 23 Oct 23         - Added a 3 position switch to select print mode - MT
 * 26 Oct 23         - Fixed switch labels - MT
 * 04 Nov 23         - Updated switch labels - MT
 *                   - Added keyboard shortcuts for multiply, divide, total
 *                     and sub-total - MT
 *                   - Changed keyboard shortcut for addition/equals to use
 *                     the ENTER key as it just seems to make sense have it
 *                     in the same place on the numeric keypad as it is  on
 *                     the calculator - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0014"
#define DATE           "04 Nov 23"
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
   h_switch[1] = h_switch_create(00000, "DSP", "ALL", "PRT", h_alternate_font, KBD_LEFT + 3 * KEY_WIDTH + 2 * KEY_GAP, KBD_TOP, 2 * KEY_WIDTH + KEY_GAP, SWITCH_HEIGHT * 2, False, MID_GREY, DARK_GREY); /** No option for PRINT only **/
}

void v_init_buttons(obutton *h_button[]) {
   int i_left, i_top, i_count = 0;

   /* Define top row of keys. */
   i_top = KBD_TOP + 3 * (SWITCH_HEIGHT + 1); /* Assumes the function label and switch text use the same font */
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00065, 000, " ^ ", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00046, 000, "M+", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00047, 000, "M-", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00044, 000, "MR", "CM", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, BLACK, YELLOW, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00040, 000, "CE", "C", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, BLACK, YELLOW, BACKGROUND, BACKGROUND);

   /* Define second row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00042, 000, "", "" , "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, YELLOW, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00062, '%', "%", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00050, '/', "\xf7", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00060, '*', "\xd7", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, BLACK, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_WIDTH + KEY_GAP);
   h_button[i_count++] = h_button_create(00063, 's', "S", "#", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_WIDTH, KEY_HEIGHT, False, True, MID_BLUE, YELLOW, BACKGROUND, BACKGROUND);

   /* Define third row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00005, '7', "7", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00004, '8', "8", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00003, '9', "9", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00064, 't', "T", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, MID_BLUE, YELLOW, BACKGROUND, BACKGROUND);

   /* Define fourth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00010, '4', "4", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00007, '5', "5", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00006, '6', "6", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00045, '-', "-", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, MID_BLUE, YELLOW, BACKGROUND, BACKGROUND);

   /* Define fifth row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00021, '1', "1", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00020, '2', "2", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00011, '3', "3", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, LIGHT_GREY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (KEY_NUMERIC + 3 * KEY_GAP);
   h_button[i_count++] = h_button_create(00041, 015, "+  =    ", "", "", "", h_normal_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KBD_ROW + KEY_HEIGHT, False, True, MID_BLUE, YELLOW, BACKGROUND, BACKGROUND);

   /* Define bottom row of keys. */
   i_top += KBD_ROW;
   i_left = KBD_LEFT;
   h_button[i_count++] = h_button_create(00022, '0', "0", "", "", "", h_large_font, h_small_font, h_alternate_font, i_left, i_top, 2 * KEY_NUMERIC + 3 * KEY_GAP, KEY_HEIGHT, False, True, LIGHT_GRAY, BACKGROUND, BACKGROUND, BACKGROUND);
   i_left += (2 * KEY_NUMERIC + 6 * KEY_GAP);
   h_button[i_count++] = h_button_create(00024, '.', ".", "", "", "", h_large_font, h_small_font, h_alternate_font, i_left, i_top, KEY_NUMERIC, KEY_HEIGHT, False, True, LIGHT_GRAY, BACKGROUND, BACKGROUND, BACKGROUND);
}


int i_rom[ROM_SIZE] = {
   0x01b4, 0x0001, 0x00db, 0x01ba, 0x01ba, 0x01ba, 0x01ba, 0x01ba, /* 0-000   */
   0x01ba, 0x01ba, 0x01ba, 0x01ba, 0x0000, 0x03b7, 0x0074, 0x02f3, /* 0-008   */
   0x01ba, 0x01ba, 0x0000, 0x03b7, 0x011c, 0x0092, 0x01b3, 0x00f4, /* 0-010   */
   0x026b, 0x01b4, 0x00d9, 0x005a, 0x00eb, 0x0000, 0x00eb, 0x0000, /* 0-018   */
   0x033f, 0x00ab, 0x0284, 0x023f, 0x0363, 0x003b, 0x03e3, 0x03db, /* 0-020   */
   0x00f4, 0x004f, 0x0074, 0x02fb, 0x0000, 0x0174, 0x016b, 0x0000, /* 0-028   */
   0x00f4, 0x0003, 0x00b7, 0x0203, 0x005f, 0x0067, 0x0074, 0x0001, /* 0-030   */
   0x01b4, 0x0269, 0x0000, 0x00c8, 0x0134, 0x005b, 0x007a, 0x0088, /* 0-038   */
   0x0194, 0x0222, 0x00cc, 0x02d0, 0x00dc, 0x0040, 0x00cc, 0x00c8, /* 0-040   */
   0x018c, 0x00b4, 0x00fd, 0x008e, 0x00ae, 0x0174, 0x02dd, 0x03c4, /* 0-048   */
   0x01b4, 0x01e7, 0x0090, 0x00f4, 0x01d1, 0x021c, 0x005e, 0x017d, /* 0-050   */
   0x021c, 0x005e, 0x0174, 0x026d, 0x03b0, 0x017d, 0x0210, 0x0110, /* 0-058   */
   0x02bc, 0x0190, 0x0272, 0x018b, 0x026c, 0x0061, 0x0250, 0x00dc, /* 0-060   */
   0x005f, 0x00cc, 0x0308, 0x0210, 0x033c, 0x0002, 0x01a2, 0x01a2, /* 0-068   */
   0x00c4, 0x01cf, 0x00cc, 0x01b4, 0x0135, 0x02bc, 0x01b4, 0x018d, /* 0-070   */
   0x03c8, 0x0174, 0x0001, 0x009a, 0x01b4, 0x00d9, 0x009a, 0x00db, /* 0-078   */
   0x0114, 0x00fb, 0x01b4, 0x016d, 0x01b4, 0x01a5, 0x01b4, 0x00dd, /* 0-080   */
   0x0148, 0x0244, 0x0074, 0x0001, 0x0108, 0x0148, 0x00db, 0x0104, /* 0-088   */
   0x005a, 0x00ef, 0x01d4, 0x00c0, 0x01c4, 0x0054, 0x00c0, 0x001a, /* 0-090   */
   0x026f, 0x0054, 0x00a7, 0x011a, 0x0110, 0x027a, 0x033c, 0x0308, /* 0-098   */
   0x009a, 0x0082, 0x01da, 0x0309, 0x0012, 0x0044, 0x007b, 0x0052, /* 0-0a0   */
   0x0072, 0x01b2, 0x02b3, 0x0303, 0x033c, 0x0082, 0x005a, 0x01dc, /* 0-0a8   */
   0x00b6, 0x01fc, 0x01c6, 0x033c, 0x0082, 0x02fb, 0x0082, 0x01fc, /* 0-0b0   */
   0x01c6, 0x0309, 0x021c, 0x00be, 0x020c, 0x007b, 0x01b2, 0x007b, /* 0-0b8   */
   0x005a, 0x007b, 0x0321, 0x0308, 0x0210, 0x0321, 0x0331, 0x030f, /* 0-0c0   */
   0x033c, 0x0002, 0x0242, 0x0110, 0x01a2, 0x01a2, 0x0210, 0x0114, /* 0-0c8   */
   0x00d4, 0x01b4, 0x00e9, 0x00db, 0x01b4, 0x0045, 0x0104, 0x00db, /* 0-0d0   */
   0x0114, 0x00e4, 0x0174, 0x02a9, 0x009a, 0x0204, 0x01b4, 0x00e9, /* 0-0d8   */
   0x01b4, 0x0165, 0x009a, 0x00db, 0x011a, 0x0174, 0x030d, 0x038c, /* 0-0e0   */
   0x01b4, 0x00e9, 0x0174, 0x0001, 0x00db, 0x01b4, 0x016d, 0x023c, /* 0-0e8   */
   0x0002, 0x01fc, 0x000a, 0x0114, 0x0072, 0x0267, 0x0384, 0x03e7, /* 0-0f0   */
   0x038c, 0x00b4, 0x01a7, 0x00b4, 0x035b, 0x0000, 0x0000, 0x0000, /* 0-0f8   */

   0x00b4, 0x0003, 0x00a9, 0x033c, 0x00fa, 0x01e2, 0x0027, 0x01d0, /* 0-100   */
   0x0017, 0x0262, 0x0190, 0x0110, 0x0266, 0x00a6, 0x0308, 0x01d0, /* 0-108   */
   0x0262, 0x0053, 0x01d0, 0x0043, 0x0190, 0x03a6, 0x03e6, 0x0002, /* 0-110   */
   0x0242, 0x0110, 0x01a2, 0x01a2, 0x0012, 0x0252, 0x02d2, 0x0122, /* 0-118   */
   0x0252, 0x0252, 0x0308, 0x00b4, 0x0125, 0x025c, 0x0129, 0x0174, /* 0-120   */
   0x0005, 0x0210, 0x01ce, 0x01ce, 0x00e3, 0x03be, 0x01ae, 0x00b7, /* 0-128   */
   0x02bc, 0x004a, 0x006a, 0x0190, 0x00ec, 0x014c, 0x001a, 0x024e, /* 0-130   */
   0x003a, 0x0204, 0x023c, 0x006a, 0x00b6, 0x01de, 0x0342, 0x0130, /* 0-138   */
   0x020c, 0x003c, 0x00ae, 0x036a, 0x012d, 0x0190, 0x00ec, 0x0149, /* 0-140   0x020c, 0x038c, 0x00ae, 0x036a, 0x012d, 0x0190, 0x00ec, 0x0149, */
   0x00db, 0x024e, 0x0117, 0x00c7, 0x024a, 0x00cf, 0x0062, 0x0190, /* 0-148   */
   0x0006, 0x00ae, 0x013e, 0x0167, 0x03be, 0x01b2, 0x01ae, 0x021c, /* 0-150   */
   0x015a, 0x01d0, 0x03be, 0x003e, 0x024a, 0x034a, 0x0160, 0x00db, /* 0-158   */
   0x01aa, 0x005a, 0x01a2, 0x01a2, 0x01de, 0x024a, 0x0193, 0x0006, /* 0-160   */
   0x0246, 0x03be, 0x005a, 0x021c, 0x0174, 0x008e, 0x002e, 0x02ea, /* 0-168   */
   0x0173, 0x028e, 0x026a, 0x008e, 0x0210, 0x02f6, 0x01a3, 0x003a, /* 0-170   */
   0x00f6, 0x0232, 0x01f3, 0x0292, 0x022e, 0x000e, 0x0012, 0x0116, /* 0-178   */
   0x01fc, 0x020f, 0x01e2, 0x021a, 0x020b, 0x013a, 0x0190, 0x00ec, /* 0-180   */
   0x0190, 0x01be, 0x00ee, 0x010e, 0x023b, 0x025e, 0x033a, 0x018d, /* 0-188   */
   0x01da, 0x026c, 0x0183, 0x001a, 0x009a, 0x0092, 0x00ee, 0x0297, /* 0-190   */
   0x001a, 0x01b2, 0x028e, 0x03ba, 0x01ff, 0x0208, 0x00b4, 0x0261, /* 0-198   */
   0x0174, 0x0115, 0x0210, 0x0184, 0x0210, 0x00b4, 0x02eb, 0x003a, /* 0-1a0   */
   0x0056, 0x018e, 0x027c, 0x0232, 0x02bb, 0x0292, 0x001a, 0x02cf, /* 0-1a8   */
   0x033c, 0x02bb, 0x013a, 0x0262, 0x02cb, 0x00a4, 0x01ba, 0x01d0, /* 0-1b0   */
   0x03ba, 0x02cf, 0x00b4, 0x0323, 0x0384, 0x02ff, 0x038c, 0x00fa, /* 0-1b8   */
   0x02c8, 0x009a, 0x0174, 0x0005, 0x00f4, 0x0119, 0x039c, 0x01c9, /* 0-1c0   */
   0x02b2, 0x0314, 0x01e3, 0x0354, 0x01e3, 0x015c, 0x03ac, 0x009a, /* 0-1c8   */
   0x0174, 0x02b5, 0x00b4, 0x0261, 0x0194, 0x0222, 0x0174, 0x0115, /* 0-1d0   */
   0x00b4, 0x024d, 0x0244, 0x0074, 0x0001, 0x009a, 0x01b4, 0x00d9, /* 0-1d8   */
   0x009a, 0x0034, 0x00db, 0x0174, 0x031d, 0x014c, 0x0114, 0x01ef, /* 0-1e0   */
   0x01b4, 0x019d, 0x00f4, 0x0175, 0x0194, 0x0222, 0x0248, 0x009a, /* 0-1e8   */
   0x007a, 0x03cc, 0x0134, 0x0069, 0x01b4, 0x00dd, 0x03c4, 0x0084, /* 0-1f0   */
   0x01c8, 0x0034, 0x00db, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-1f8   */

   0x001a, 0x033c, 0x021c, 0x0207, 0x0242, 0x020c, 0x0023, 0x0388, /* 0-200   */
   0x0062, 0x02ea, 0x020e, 0x0042, 0x0074, 0x000b, 0x0242, 0x0047, /* 0-208   */
   0x002f, 0x01a2, 0x0059, 0x021c, 0x020b, 0x004b, 0x020c, 0x0042, /* 0-210   */
   0x0062, 0x0142, 0x0087, 0x0042, 0x0242, 0x0062, 0x02c2, 0x0221, /* 0-218   */
   0x0204, 0x0210, 0x00c8, 0x0034, 0x014d, 0x011a, 0x0110, 0x027a, /* 0-220   */
   0x007c, 0x0398, 0x0298, 0x0298, 0x0318, 0x0298, 0x0308, 0x009a, /* 0-228   */
   0x0088, 0x000c, 0x00cc, 0x02d0, 0x00dc, 0x0232, 0x00cc, 0x00fd, /* 0-230   */
   0x0048, 0x011a, 0x00c8, 0x01b4, 0x007d, 0x0034, 0x00db, 0x005a, /* 0-238   */
   0x02c8, 0x010e, 0x0110, 0x026e, 0x0270, 0x03f8, 0x00ae, 0x0308, /* 0-240   */
   0x0210, 0x033c, 0x0110, 0x01a2, 0x0147, 0x03ba, 0x0252, 0x0000, /* 0-248   */
   0x012f, 0x0242, 0x0308, 0x00fa, 0x0314, 0x025d, 0x0354, 0x025d, /* 0-250   */
   0x005a, 0x0388, 0x01a2, 0x019f, 0x005a, 0x0110, 0x0242, 0x0193, /* 0-258   */
   0x03ba, 0x0252, 0x0000, 0x017b, 0x01a2, 0x0308, 0x0210, 0x005a, /* 0-260   */
   0x0210, 0x0174, 0x0001, 0x0244, 0x0074, 0x0001, 0x039c, 0x0271, /* 0-268   */
   0x02b2, 0x02c8, 0x0174, 0x02a9, 0x0244, 0x0074, 0x0001, 0x009a, /* 0-270   */
   0x0248, 0x02c8, 0x00b4, 0x0261, 0x0194, 0x0222, 0x0174, 0x0115, /* 0-278   */
   0x00b4, 0x024d, 0x0244, 0x0074, 0x0001, 0x0174, 0x030d, 0x01b4, /* 0-280   */
   0x00b5, 0x01b4, 0x0165, 0x0248, 0x039c, 0x0290, 0x02b2, 0x038c, /* 0-288   */
   0x005a, 0x0034, 0x00db, 0x01fc, 0x0366, 0x0297, 0x0112, 0x0210, /* 0-290   */
   0x01fc, 0x003a, 0x01aa, 0x01aa, 0x01ea, 0x01ea, 0x030e, 0x02a1, /* 0-298   */
   0x009a, 0x0096, 0x02f6, 0x02a5, 0x009a, 0x00f6, 0x030e, 0x02ad, /* 0-2a0   */
   0x03da, 0x01ae, 0x02da, 0x02ad, 0x029b, 0x026a, 0x026a, 0x000e, /* 0-2a8   */
   0x0392, 0x0352, 0x02b5, 0x013a, 0x0323, 0x0336, 0x02b9, 0x02b2, /* 0-2b0   */
   0x005a, 0x021a, 0x01fc, 0x0346, 0x02bf, 0x010e, 0x0337, 0x0342, /* 0-2b8   */
   0x02cd, 0x01c6, 0x0110, 0x01b2, 0x0308, 0x026e, 0x0000, 0x02ff, /* 0-2c0   */
   0x0352, 0x02cb, 0x0337, 0x01ee, 0x03ba, 0x01fc, 0x003a, 0x006e, /* 0-2c8   */
   0x0126, 0x0353, 0x01ee, 0x01a2, 0x0096, 0x0210, 0x005a, 0x03cc, /* 0-2d0   */
   0x0134, 0x0069, 0x010c, 0x004c, 0x0034, 0x00fb, 0x0000, 0x0000, /* 0-2d8   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-2e0   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-2e8   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-2f0   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-2f8   */

   0x01b4, 0x01a5, 0x00da, 0x02c8, 0x009a, 0x0174, 0x0001, 0x0119, /* 0-300   */
   0x0204, 0x014c, 0x011c, 0x0326, 0x009a, 0x0174, 0x02b5, 0x01ad, /* 0-308   */
   0x020c, 0x02c4, 0x009f, 0x01b4, 0x01a5, 0x0174, 0x0001, 0x0119, /* 0-310   */
   0x009a, 0x02c8, 0x009a, 0x0204, 0x0144, 0x011c, 0x0326, 0x009a, /* 0-318   */
   0x0174, 0x02b5, 0x0199, 0x020c, 0x02c4, 0x009f, 0x0175, 0x0194, /* 0-320   */
   0x0222, 0x02c8, 0x01b4, 0x00dd, 0x0248, 0x021c, 0x0335, 0x015c, /* 0-328   */
   0x0334, 0x0344, 0x014c, 0x00d7, 0x0304, 0x00fa, 0x0248, 0x009a, /* 0-330   */
   0x00da, 0x007a, 0x02c8, 0x03cc, 0x0134, 0x0069, 0x00cc, 0x02dc, /* 0-338   */
   0x0343, 0x03c4, 0x0084, 0x0248, 0x0034, 0x00db, 0x01d4, 0x035c, /* 0-340   */
   0x02fa, 0x035c, 0x005c, 0x035c, 0x02c8, 0x00cc, 0x011a, 0x033c, /* 0-348   */
   0x0098, 0x0270, 0x0038, 0x02f2, 0x0356, 0x00c4, 0x0248, 0x00dc, /* 0-350   * 0x0098, 0x0270, 0x02f0, 0x02f2, 0x0356, 0x00c4, 0x0248, 0x00dc, */
   0x035c, 0x026e, 0x026e, 0x00cc, 0x0210, 0x01b4, 0x016d, 0x0314, /* 0-358   */
   0x0369, 0x035c, 0x0371, 0x009a, 0x01c8, 0x009a, 0x0074, 0x01d5, /* 0-360   */
   0x01b7, 0x009a, 0x01c8, 0x0074, 0x029d, 0x00b4, 0x024d, 0x0174, /* 0-368   */
   0x0115, 0x0188, 0x01c8, 0x0210, 0x020c, 0x0050, 0x01ce, 0x024a, /* 0-370   */
   0x024a, 0x01f7, 0x0204, 0x009a, 0x0210, 0x024a, 0x01ef, 0x01eb, /* 0-378   */
   0x0322, 0x0384, 0x0082, 0x024b, 0x0042, 0x01a2, 0x01a2, 0x0322, /* 0-380   */
   0x038f, 0x01a2, 0x01a2, 0x0322, 0x038e, 0x0263, 0x0242, 0x0242, /* 0-388   */
   0x0242, 0x0082, 0x01d0, 0x0203, 0x00ba, 0x0248, 0x00fa, 0x009a, /* 0-390   */
   0x0308, 0x0210, 0x0114, 0x03a7, 0x0108, 0x00ba, 0x01b4, 0x007d, /* 0-398   */
   0x009a, 0x0174, 0x031d, 0x01b4, 0x016d, 0x01b4, 0x02e3, 0x005a, /* 0-3a0   */
   0x01b4, 0x016d, 0x0034, 0x00eb, 0x01b4, 0x0135, 0x013c, 0x01b4, /* 0-3a8   */
   0x0151, 0x00dc, 0x03b9, 0x03c1, 0x0194, 0x0222, 0x01b4, 0x016d, /* 0-3b0   */
   0x03ab, 0x01b4, 0x0135, 0x01fc, 0x01b4, 0x0151, 0x00dc, 0x03d4, /* 0-3b8   */
   0x02c8, 0x03c1, 0x0194, 0x0222, 0x0188, 0x01c8, 0x01b4, 0x01a5, /* 0-3c0   */
   0x0248, 0x0244, 0x0074, 0x0001, 0x03cc, 0x0134, 0x0069, 0x01b4, /* 0-3c8   */
   0x00d9, 0x0148, 0x0034, 0x00db, 0x005c, 0x03dc, 0x02c8, 0x0112, /* 0-3d0   */
   0x0174, 0x031d, 0x0248, 0x039b, 0x0174, 0x02b5, 0x01b4, 0x032d, /* 0-3d8   */
   0x0074, 0x00a9, 0x009a, 0x039c, 0x03e6, 0x02b2, 0x03c1, 0x0194, /* 0-3e0   */
   0x0222, 0x00c4, 0x009a, 0x01b4, 0x00d9, 0x009a, 0x0034, 0x00db, /* 0-3e8   */
   0x009a, 0x0148, 0x00b4, 0x0261, 0x0194, 0x03fc, 0x0174, 0x0115, /* 0-3f0   */
   0x00b4, 0x024d, 0x0108, 0x0148, 0x0210, 0x0000, 0x0000, 0x0000, /* 0-3f8   */

   0x013f, 0x0283, 0x00a3, 0x0000, 0x019b, 0x02a7, 0x017b, 0x018b, /* 0-400   */
   0x023f, 0x00a3, 0x00a3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-408   */
   0x021f, 0x00a3, 0x025b, 0x01c3, 0x01e3, 0x0123, 0x00d4, 0x0042, /* 0-410   */
   0x03dc, 0x002f, 0x005a, 0x00f4, 0x01d1, 0x021c, 0x0028, 0x020c, /* 0-418   */
   0x0094, 0x00b4, 0x0174, 0x02c5, 0x023c, 0x0262, 0x00d7, 0x00a3, /* 0-420   */
   0x03dc, 0x0033, 0x03cc, 0x008c, 0x005a, 0x0294, 0x0030, 0x010c, /* 0-428   */
   0x028c, 0x0034, 0x00fb, 0x005a, 0x0210, 0x0262, 0x00e3, 0x00a3, /* 0-430   */
   0x000e, 0x0262, 0x0113, 0x033c, 0x00a2, 0x01ce, 0x0034, 0x017d, /* 0-438   */
   0x0349, 0x0090, 0x01b4, 0x00fb, 0x033c, 0x01a2, 0x01ce, 0x00f3, /* 0-440   */
   0x02bc, 0x03d8, 0x026c, 0x0049, 0x03b0, 0x005a, 0x00c3, 0x011c, /* 0-448   */
   0x0028, 0x0318, 0x00d8, 0x02d8, 0x03b0, 0x037d, 0x0248, 0x0399, /* 0-450   */
   0x00f4, 0x0201, 0x03d0, 0x0248, 0x00fa, 0x00a3, 0x0058, 0x01d8, /* 0-458   */
   0x00d8, 0x01f7, 0x0058, 0x0198, 0x03d8, 0x01f7, 0x0114, 0x006c, /* 0-460   */
   0x0058, 0x0158, 0x0118, 0x01f7, 0x0318, 0x0018, 0x0158, 0x01f7, /* 0-468   */
   0x0114, 0x0075, 0x0198, 0x00d8, 0x01f3, 0x0298, 0x01d8, 0x01f3, /* 0-470   */
   0x0114, 0x007f, 0x01d8, 0x00d8, 0x02d8, 0x03b0, 0x015f, 0x02bc, /* 0-478   */
   0x03d8, 0x03d8, 0x0358, 0x026c, 0x0082, 0x03d0, 0x00a3, 0x0118, /* 0-480   */
   0x0114, 0x008c, 0x02d8, 0x01f3, 0x0298, 0x0298, 0x01f3, 0x0318, /* 0-488   */
   0x0114, 0x0093, 0x022b, 0x0298, 0x0298, 0x01f3, 0x0354, 0x009c, /* 0-490   */
   0x0218, 0x0018, 0x0098, 0x01f7, 0x0018, 0x0298, 0x0018, 0x01f7, /* 0-498   */
   0x0154, 0x00c9, 0x02e5, 0x0214, 0x0054, 0x0094, 0x00b4, 0x0318, /* 0-4a0   */
   0x02c7, 0x0154, 0x00cb, 0x02e5, 0x0214, 0x0054, 0x0094, 0x00b4, /* 0-4a8   */
   0x02d8, 0x03d8, 0x02d8, 0x0153, 0x0034, 0x017d, 0x0349, 0x008c, /* 0-4b0   */
   0x01f7, 0x020c, 0x0314, 0x00be, 0x035c, 0x00c4, 0x0394, 0x00c5, /* 0-4b8   */
   0x0018, 0x02d8, 0x02d8, 0x0204, 0x0210, 0x02d8, 0x0318, 0x0098, /* 0-4c0   */
   0x030f, 0x0318, 0x0333, 0x02d8, 0x0398, 0x0018, 0x014c, 0x038c, /* 0-4c8   */
   0x0153, 0x02c8, 0x011a, 0x0110, 0x027a, 0x0308, 0x007c, 0x03d8, /* 0-4d0   */
   0x02d8, 0x009c, 0x00de, 0x0398, 0x03d8, 0x02d8, 0x0210, 0x02bc, /* 0-4d8   */
   0x0190, 0x0272, 0x0387, 0x026c, 0x00e0, 0x0210, 0x00da, 0x02c8, /* 0-4e0   */
   0x033c, 0x001a, 0x025a, 0x0110, 0x01a2, 0x01d0, 0x02ec, 0x00ec, /* 0-4e8   */
   0x007a, 0x00ba, 0x011a, 0x027a, 0x0210, 0x0000, 0x0000, 0x0000, /* 0-4f0   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-4f8   */

   0x005a, 0x007a, 0x0308, 0x011a, 0x02bc, 0x020c, 0x0110, 0x0272, /* 0-500   */
   0x0272, 0x01da, 0x0190, 0x0312, 0x0109, 0x0272, 0x0312, 0x0112, /* 0-508   */
   0x01f2, 0x005f, 0x0204, 0x01f2, 0x0027, 0x01da, 0x0190, 0x0352, /* 0-510   */
   0x011a, 0x0057, 0x0272, 0x0272, 0x0272, 0x0312, 0x0130, 0x02bc, /* 0-518   */
   0x0112, 0x0272, 0x0272, 0x0272, 0x0272, 0x0272, 0x0302, 0x013c, /* 0-520   */
   0x0264, 0x012d, 0x01ee, 0x00e4, 0x013c, 0x0190, 0x03fe, 0x009b, /* 0-528   */
   0x0308, 0x026e, 0x01da, 0x0190, 0x0352, 0x013d, 0x02e4, 0x0139, /* 0-530   */
   0x00c7, 0x010e, 0x020c, 0x00f7, 0x01c6, 0x03be, 0x0308, 0x021c, /* 0-538   */
   0x0142, 0x0252, 0x009e, 0x005a, 0x0210, 0x023c, 0x02ea, 0x0153, /* 0-540   */
   0x01e2, 0x0157, 0x0262, 0x033c, 0x0262, 0x0262, 0x0143, 0x0157, /* 0-548   */
   0x01e2, 0x01e2, 0x0210, 0x02e2, 0x0157, 0x0184, 0x011a, 0x0210, /* 0-550   */
   0x0262, 0x0210, 0x00fa, 0x02c8, 0x00fa, 0x0001, 0x02fa, 0x0162, /* 0-558   */
   0x026e, 0x026e, 0x02c8, 0x0314, 0x016e, 0x0354, 0x016e, 0x005c, /* 0-560   */
   0x0175, 0x0174, 0x02b5, 0x0188, 0x0304, 0x01d7, 0x031c, 0x0175, /* 0-568   */
   0x01c8, 0x031d, 0x01b4, 0x019d, 0x0144, 0x0248, 0x009a, 0x01c8, /* 0-570   */
   0x0314, 0x017e, 0x009a, 0x0074, 0x01d5, 0x0203, 0x0074, 0x029d, /* 0-578   */
   0x00b4, 0x024d, 0x0194, 0x0193, 0x0188, 0x0248, 0x009a, 0x007a, /* 0-580   */
   0x03cc, 0x0134, 0x0069, 0x03c4, 0x0084, 0x01b4, 0x00d9, 0x01c8, /* 0-588   */
   0x00cc, 0x0034, 0x00db, 0x00b4, 0x008b, 0x03c4, 0x0084, 0x01b4, /* 0-590   */
   0x00d9, 0x0034, 0x00db, 0x03d8, 0x03d8, 0x0398, 0x03d8, 0x02d8, /* 0-598   */
   0x0398, 0x03d8, 0x0218, 0x0158, 0x0058, 0x0118, 0x00d8, 0x0058, /* 0-5a0   */
   0x0118, 0x0210, 0x011a, 0x033c, 0x02d3, 0x011a, 0x033c, 0x0058, /* 0-5a8   */
   0x02d3, 0x011a, 0x033c, 0x0098, 0x0270, 0x0038, 0x0210, 0x03ee, /* 0-5b0   * 0x02d3, 0x011a, 0x033c, 0x0098, 0x0270, 0x02f0, 0x0210, 0x03ee, */
   0x02c8, 0x009a, 0x02c8, 0x01b4, 0x0139, 0x008e, 0x02f0, 0x0248, /* 0-5b8   */
   0x009a, 0x0248, 0x0210, 0x02c8, 0x011a, 0x033c, 0x032f, 0x02c8, /* 0-5c0   */
   0x011a, 0x033c, 0x0058, 0x0270, 0x0248, 0x02f0, 0x0210, 0x0000, /* 0-5c8   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-5d0   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-5d8   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-5e0   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-5e8   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-5f0   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-5f8   */

   0x00c8, 0x0048, 0x0008, 0x011a, 0x0270, 0x02f0, 0x033c, 0x0058, /* 0-600   */
   0x0270, 0x011a, 0x02f0, 0x033c, 0x0098, 0x0270, 0x008e, 0x02f0, /* 0-608   */
   0x03c8, 0x033c, 0x011a, 0x0270, 0x02f0, 0x0098, 0x0270, 0x0038, /* 0-610   0x03c8, 0x033c, 0x011a, 0x0270, 0x02f0, 0x0098, 0x0270, 0x02f0, */
   0x00ae, 0x02bc, 0x00a2, 0x011a, 0x008e, 0x0082, 0x02f0, 0x011a, /* 0-618   */
   0x0108, 0x011a, 0x0188, 0x033c, 0x011a, 0x01e2, 0x0270, 0x011a, /* 0-620   */
   0x02f0, 0x01b4, 0x016d, 0x030c, 0x034c, 0x010c, 0x01b4, 0x01a5, /* 0-628   */
   0x02cc, 0x014c, 0x004c, 0x01cc, 0x018c, 0x00ef, 0x010c, 0x030c, /* 0-630   */
   0x034c, 0x00cb, 0x00cb, 0x011a, 0x02bc, 0x0210, 0x009a, 0x02c8, /* 0-638   */
   0x02c8, 0x0174, 0x02b5, 0x0074, 0x0001, 0x00cc, 0x03cc, 0x0134, /* 0-640   */
   0x0069, 0x0248, 0x00ba, 0x0034, 0x00fb, 0x02c8, 0x011a, 0x033c, /* 0-648   */
   0x0098, 0x0270, 0x0038, 0x0210, 0x00cc, 0x02e2, 0x0277, 0x00c4, /* 0-650   * 0x0098, 0x0270, 0x02f0, 0x0210, 0x00cc, 0x02e2, 0x0277, 0x00c4,*/
   0x01df, 0x00c4, 0x0173, 0x00cc, 0x02c8, 0x011a, 0x033c, 0x0098, /* 0-658   */
   0x0270, 0x0038, 0x013c, 0x0102, 0x00dc, 0x0276, 0x01d3, 0x00c4, /* 0-660   0x0270, 0x02f0, 0x013c, 0x0102, 0x00dc, 0x0276, 0x01d3, 0x00c4, */
   0x01ab, 0x00cc, 0x02c8, 0x011a, 0x033c, 0x0098, 0x0270, 0x0038, /* 0-668   0x01ab, 0x00cc, 0x02c8, 0x011a, 0x033c, 0x0098, 0x0270, 0x02f0, */
   0x01fc, 0x0102, 0x00dc, 0x0276, 0x01e2, 0x00cc, 0x02f0, 0x0248, /* 0-670   */
   0x0210, 0x02c8, 0x017c, 0x0098, 0x0058, 0x0018, 0x022e, 0x02ee, /* 0-678   */
   0x0288, 0x017c, 0x0098, 0x0158, 0x0018, 0x022e, 0x036e, 0x0295, /* 0-680   */
   0x02dc, 0x0297, 0x02cc, 0x01c8, 0x0074, 0x0001, 0x0174, 0x0005, /* 0-688   */
   0x0174, 0x031d, 0x00d9, 0x0034, 0x00eb, 0x02cc, 0x014c, 0x0248, /* 0-690   */
   0x0034, 0x014b, 0x0114, 0x02b7, 0x007a, 0x011a, 0x033c, 0x0098, /* 0-698   */
   0x0270, 0x0038, 0x00ba, 0x017c, 0x0018, 0x0098, 0x0118, 0x022e, /* 0-6a0   0x0270, 0x02f0, 0x00ba, 0x017c, 0x0018, 0x0098, 0x0118, 0x022e, */
   0x036e, 0x02b6, 0x00da, 0x033c, 0x0362, 0x02b4, 0x03fa, 0x02bc, /* 0-6a8   */
   0x0110, 0x0262, 0x0308, 0x02af, 0x00ba, 0x0210, 0x005a, 0x0210, /* 0-6b0   */
   0x0074, 0x0001, 0x03cc, 0x0134, 0x0069, 0x00cc, 0x03c4, 0x005a, /* 0-6b8   */
   0x011a, 0x033c, 0x0098, 0x0270, 0x02f0, 0x023c, 0x00d8, 0x0158, /* 0-6c0   */
   0x02f0, 0x0034, 0x0067, 0x001a, 0x0388, 0x033c, 0x0062, 0x02ea, /* 0-6c8   */
   0x02d3, 0x0042, 0x0210, 0x0242, 0x035b, 0x0347, 0x01a2, 0x00b4, /* 0-6d0   */
   0x0059, 0x021c, 0x02d1, 0x035f, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-6d8   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-6e0   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-6e8   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 0-6f0   */
   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000  /* 0-6f8   */
};
