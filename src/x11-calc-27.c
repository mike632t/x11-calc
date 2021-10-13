/*
 * x11-calc-29.c - RPN (Reverse Polish) calculator simulator.
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
 * 09 Mar 14   0.1   - Initial version - MT
 * 10 Mar 14         - Changed indexes to BCD hex values - MT
 * 10 Dec 18         - Alternate function key now LIGHT_BLUE, allowing it to
 *                     have a different from the alternate text - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0003"
#define DATE           "21 Sep 21"
#define AUTHOR         "MT"

#define DEBUG 0        /* Enable/disable debug*/

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
   h_switch[0] = h_switch_create(00000, "OFF", "ON ", h_alternate_font, 12, 67, 67, 10, True, MID_GREY, DARK_GREY);
   h_switch[1] = h_switch_create(00000, "PRGM", "RUN", h_alternate_font, 120, 67, 67, 10, True, MID_GREY, DARK_GREY);

   /* Define top row of keys. */ 
   h_button[0] = h_button_create(00000, 000, " y ", "FIX", "LR", h_normal_font, h_small_font, h_alternate_font, 12, 89, 33, 30, False, GREEN, YELLOW, BLACK);
   h_button[1] = h_button_create(00000, 000, " x ", "SCI", " s ", h_normal_font, h_small_font, h_alternate_font, 48, 89, 33, 30, False, GREEN, YELLOW, BLACK);
   h_button[2] = h_button_create(00000, 000, " % ", "ENG", "d%", h_normal_font, h_small_font, h_alternate_font, 84, 89, 33, 30, False, GREEN, YELLOW, BLACK);
   h_button[3] = h_button_create(00000, 'f', "f", "", "", h_normal_font, h_small_font, h_alternate_font, 120, 89, 33, 30, False, YELLOW, BACKGROUND, YELLOW);
   h_button[4] = h_button_create(00000, 'g', "g", "", "", h_normal_font, h_small_font, h_alternate_font, 156, 89, 33, 30, False, BLACK, BACKGROUND, BLACK);
   
   /* Define second row of keys. */ 
   h_button[5] = h_button_create(00000, 000, "X-Y", "n", "r", h_normal_font, h_small_font, h_alternate_font, 12, 132, 33, 30, False, GREEN, YELLOW, BLACK);
   h_button[6] = h_button_create(00000, 000, "R", "i", "VAR", h_normal_font, h_small_font, h_alternate_font, 48, 132, 33, 30, False, GREEN, YELLOW, BLACK);
   h_button[7] = h_button_create(00000, 000, "STO", "PMT", "N.D", h_normal_font, h_small_font, h_alternate_font, 84, 132, 33, 30, False, GREEN, YELLOW, BLACK);
   h_button[8] = h_button_create(00000, 000, "RCL", "PV", "NPV", h_normal_font, h_small_font, h_alternate_font, 120, 132, 33, 30, False, GREEN, YELLOW, BLACK);
   h_button[9] = h_button_create(00000, 000, "yX", "FV", "IRR", h_normal_font, h_small_font, h_alternate_font, 156, 132, 33, 30, False, GREEN, YELLOW, BLACK); 

   /* Define third row of keys. */
   h_button[10] = h_button_create(00000, 015, "ENTER", "PREFIX", "RESET", h_normal_font, h_small_font, h_alternate_font, 12, 175, 69, 30, False, GREEN, YELLOW, BLACK);
   h_button[11] = h_button_create(00000, 'c', "CHS", "E", "DEG", h_normal_font, h_small_font, h_alternate_font, 84, 175, 33, 30, False, GREEN, YELLOW, BLACK);
   h_button[12] = h_button_create(00000, 'e', "EEX", "REG", "RAD", h_normal_font, h_small_font, h_alternate_font, 120, 175, 33, 30, False, GREEN, YELLOW, BLACK);
   h_button[13] = h_button_create(00000, 033, "CLX", "STK", "GRD", h_normal_font, h_small_font, h_alternate_font, 156, 175, 33, 30, False, GREEN, YELLOW, BLACK);

   /* Define fourth row of keys. */
   h_button[14] = h_button_create(00000, '-', "-", "", "", h_large_font, h_small_font, h_alternate_font, 12, 218, 33, 30, False, BEIGE, YELLOW, BLACK);
   h_button[15] = h_button_create(00000, '7', "7", "LN", "eX" , h_large_font, h_small_font, h_alternate_font, 52, 218, 41, 30, False, BEIGE, YELLOW, BLACK);
   h_button[16] = h_button_create(00000, '8', "8", "LOG", "10x", h_large_font, h_small_font, h_alternate_font, 100, 218, 41, 30, False, BEIGE, YELLOW, BLACK);
   h_button[17] = h_button_create(00000, '9', "9", "-R", "-P", h_large_font, h_small_font, h_alternate_font, 148, 218, 41, 30, False, BEIGE, YELLOW, BLACK);

   /* Define fifth row of keys. */
   h_button[18] = h_button_create(00000, '+', "+", "", "", h_large_font, h_small_font, h_alternate_font, 12, 261, 33, 30, False, BEIGE, YELLOW, BLACK);
   h_button[19] = h_button_create(00000, '4', "4", "SIN", "SIN-\xb9", h_large_font, h_small_font, h_alternate_font, 52, 261, 41, 30, False, BEIGE, YELLOW, BLACK);
   h_button[20] = h_button_create(00000, '5', "5", "COS", "COS-\xb9", h_large_font, h_small_font, h_alternate_font, 100, 261, 41, 30, False, BEIGE, YELLOW, BLACK);
   h_button[21] = h_button_create(00000, '6', "6", "TAN", "TAN-\xb9", h_large_font, h_small_font, h_alternate_font, 148, 261, 41, 30, False, BEIGE, YELLOW, BLACK);

   /* Define sixth row of keys. */
   h_button[22] = h_button_create(00000, '*', "\xd7", "", "", h_large_font, h_small_font, h_alternate_font, 12, 304, 33, 30, False, BEIGE, YELLOW, BLACK);
   h_button[23] = h_button_create(00000, '1', "1", "H.MS+", "H.MS-", h_large_font, h_small_font, h_alternate_font, 52, 304, 41, 30, False, BEIGE, YELLOW, BLACK);
   h_button[24] = h_button_create(00000, '2', "2", "V\xaf", "x\xb2", h_large_font, h_small_font, h_alternate_font, 100, 304, 41, 30, False, BEIGE, YELLOW, BLACK);
   h_button[25] = h_button_create(00000, '3', "3", "n!", "1/x", h_large_font, h_small_font, h_alternate_font, 148, 304, 41, 30, False, BEIGE, YELLOW, BLACK);

   /* Define bottom row of keys. */
   h_button[26] = h_button_create(00000, '/', "\xf7", "", "", h_large_font, h_small_font, h_alternate_font, 12, 347, 33, 30, False, BEIGE, YELLOW, BLACK);
   h_button[27] = h_button_create(00000, '0', "0", "-H.MS", "-H", h_large_font, h_small_font, h_alternate_font, 52, 347, 41, 30, False, BEIGE, YELLOW, BLACK);
   h_button[28] = h_button_create(00000, '.', ".", "LASTx", "\x1c", h_large_font, h_small_font, h_alternate_font, 100, 347, 41, 30, False, BEIGE, YELLOW, BLACK);
   h_button[29] = h_button_create(00000, 000, "E+", "E-", "%E", h_normal_font, h_small_font, h_alternate_font, 148, 347, 41, 30, False, BEIGE, YELLOW, BLACK);
}
   
int i_rom[ROM_SIZE * ROM_BANKS];
