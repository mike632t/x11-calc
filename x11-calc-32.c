/*
 *
 * x11-calc-33.h
 *
 * Copyright(C) 2018   MT
 * 
 * Model specific functions
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
 *                      
 * TO DO :           - 
 */
 
#define VERSION        "0.1"
#define BUILD          "0007"
#define DATE           "10 Aug 21"
#define AUTHOR         "MT"

#define DEBUG 1
 
#include <stdarg.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* getenv(), etc. */
 
#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */
 
#include "x11-calc-font.h" 
#include "x11-calc-button.h"
#include "x11-calc-colour.h"
#include "x11-calc-cpu.h"

#include "x11-calc.h"

#include "gcc-debug.h"

oregister o_mem[MEMORY_SIZE];

int i_rom[ROM_SIZE * ROM_BANKS];
 
void v_init_keypad(obutton *h_button[]){

   /* Define top row of keys. */ 
   h_button[0] = h_button_create(00000, 000, "/\xaf", "FIX", "x\xb2", h_normal_font, h_small_font, h_alternate_font, 12, 85, 33, 30, False, BLACK);
   h_button[1] = h_button_create(00000, 000, "1/x", "SCI", "\x1c", h_normal_font, h_small_font, h_alternate_font, 48, 85, 33, 30, False, BLACK);
   h_button[2] = h_button_create(00000, 000, "yX", "ENG", "LSTx", h_normal_font, h_small_font, h_alternate_font, 84, 85, 33, 30, False, BLACK);
   h_button[3] = h_button_create(00000, 'f', "f", "", "", h_normal_font, h_small_font, h_alternate_font, 120, 85, 33, 30, False, YELLOW);
   h_button[4] = h_button_create(00000, 'g', "g", "", "", h_normal_font, h_small_font, h_alternate_font, 156, 85, 33, 30, False, LIGHT_BLUE);
   
   /* Define second row of keys. */ 
   h_button[5] = h_button_create(00000, 000, "X-Y", "Q", "Q-\xb9", h_normal_font, h_small_font, h_alternate_font, 12, 128, 33, 30, False, BLACK);
   h_button[6] = h_button_create(00000, 000, "R", "y", "x", h_normal_font, h_small_font, h_alternate_font, 48, 128, 33, 30, False, BLACK);
   h_button[7] = h_button_create(00000, 000, "STO", "LR", "r", h_normal_font, h_small_font, h_alternate_font, 84, 128, 33, 30, False, BLACK);
   h_button[8] = h_button_create(00000, 000, "RCL", "x", "s", h_normal_font, h_small_font, h_alternate_font, 120, 128, 33, 30, False, BLACK);
   h_button[9] = h_button_create(00000, 000, "E+", "E-", "n!", h_normal_font, h_small_font, h_alternate_font, 156, 128, 33, 30, False, BLACK);

   /* Define third row of keys. */
   h_button[10] = h_button_create(00000, 000, "ENTER", "MANT PREFIX", "", h_normal_font, h_small_font, h_alternate_font, 12, 171, 69, 30, False, BLACK);
   h_button[11] = h_button_create(00000, 'c', "CHS", "ALL", "RAD", h_normal_font, h_small_font, h_alternate_font, 84, 171, 33, 30, False, BLACK);
   h_button[12] = h_button_create(00000, 'e', "EEX", "REG", "GRD", h_normal_font, h_small_font, h_alternate_font, 120, 171, 33, 30, False, BLACK);
   h_button[13] = h_button_create(00000, 033, "CLx", "E", "DEC", h_normal_font, h_small_font, h_alternate_font, 156, 171, 33, 30, False, BLACK);

   /* Define fourth row of keys. */
   h_button[14] = h_button_create(00000, '-', "-", "-in", "-mm", h_large_font, h_small_font, h_alternate_font, 12, 214, 33, 30, False, LIGHT_GRAY);
   h_button[15] = h_button_create(00000, '7', "7", "SIN", "SIN\xaf\xb9", h_large_font, h_small_font, h_alternate_font, 52, 214, 41, 30, False, LIGHT_GRAY);
   h_button[16] = h_button_create(00000, '8', "8", "COS", "COS\xaf\xb9", h_large_font, h_small_font, h_alternate_font, 100, 214, 41, 30, False, LIGHT_GRAY);
   h_button[17] = h_button_create(00000, '9', "9", "TAN", "TAN\xaf\xb9", h_large_font, h_small_font, h_alternate_font, 148, 214, 41, 30, False, LIGHT_GRAY);
   
   /* Alternate key labels
    * h_button[15] = h_button_create(00000, '7', "7", "SIN", "ASIN", h_large_font, h_small_font, h_alternate_font, 52, 214, 41, 30, False, LIGHT_GRAY);
    * h_button[16] = h_button_create(00000, '8', "8", "COS", "ACOS", h_large_font, h_small_font, h_alternate_font, 100, 214, 41, 30, False, LIGHT_GRAY);
    * h_button[17] = h_button_create(00000, '9', "9", "TAN", "ATAN", h_large_font, h_small_font, h_alternate_font, 148, 214, 41, 30, False, LIGHT_GRAY);
   /*

   /* Define fifth row of keys. */
   h_button[18] = h_button_create(00000, '+', "+", "-\x07\x46", "-\x07\x43", h_large_font, h_small_font, h_alternate_font, 12, 257, 33, 30, False, LIGHT_GRAY);
   h_button[19] = h_button_create(00000, '4', "4", "-R", "-P", h_large_font, h_small_font, h_alternate_font, 52, 257, 41, 30, False, LIGHT_GRAY);
   h_button[20] = h_button_create(00000, '5', "5", "-RAD", "-DEG", h_large_font, h_small_font, h_alternate_font, 100, 257, 41, 30, False, LIGHT_GRAY);
   h_button[21] = h_button_create(00000, '6', "6", "-H.MS", "-H", h_large_font, h_small_font, h_alternate_font, 148, 257, 41, 30, False, LIGHT_GRAY);

   /* Define sixth row of keys. */
   h_button[22] = h_button_create(00000, '*', "\xd7", "-lbm", "-kg", h_large_font, h_small_font, h_alternate_font, 12, 300, 33, 30, False, LIGHT_GRAY);
   h_button[23] = h_button_create(00000, '1', "1", "SINH", "SINH\xaf\xb9", h_large_font, h_small_font, h_alternate_font, 52, 300, 41, 30, False, LIGHT_GRAY);
   h_button[24] = h_button_create(00000, '2', "2", "COSH", "COSH\xaf\xb9", h_large_font, h_small_font, h_alternate_font, 100, 300, 41, 30, False, LIGHT_GRAY);
   h_button[25] = h_button_create(00000, '3', "3", "TANH", "TANH\xaf\xb9", h_large_font, h_small_font, h_alternate_font, 148, 300, 41, 30, False, LIGHT_GRAY);
   
   /* Alternate key labels
    * h_button[23] = h_button_create(00000, '1', "1", "SINH", "ASINH", h_large_font, h_small_font, h_alternate_font, 52, 300, 41, 30, False, LIGHT_GRAY);
    * h_button[24] = h_button_create(00000, '2', "2", "COSH", "ACOSH", h_large_font, h_small_font, h_alternate_font, 100, 300, 41, 30, False, LIGHT_GRAY);
    * h_button[25] = h_button_create(00000, '3', "3", "TANH", "ATANH", h_large_font, h_small_font, h_alternate_font, 148, 300, 41, 30, False, LIGHT_GRAY);
    */

   /* Define bottom row of keys. */
   h_button[26] = h_button_create(00000, '/', "\xf7", "-gal", "-ltr", h_large_font, h_small_font, h_alternate_font, 12, 343, 33, 30, False, LIGHT_GRAY);
   h_button[27] = h_button_create(00000, '0', "0", "LN", "eX", h_large_font, h_small_font, h_alternate_font, 52, 343, 41, 30, False, LIGHT_GRAY);
   h_button[28] = h_button_create(00000, 000, "\xb7", "LOG", "10x", h_large_font, h_small_font, h_alternate_font, 100, 343, 41, 30, False, LIGHT_GRAY);
   h_button[29] = h_button_create(00000, '%', "%", "%E", "D%", h_normal_font, h_small_font, h_alternate_font, 148, 343, 41, 30, False, LIGHT_GRAY);
}
