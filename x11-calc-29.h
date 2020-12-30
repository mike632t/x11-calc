/*
 *
 * x11-calc-29.h - Model specific functions.
 *
 * Copyright(C) 2018   MEJT
 *
 * This  program is free software: you can redistribute it and/or modify  it
 * under  the  terms of the GNU General Public License as published  by  the
 * Free  Software  Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is  distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without  even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License  along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * 09 Mar 14         - Initial verson - MEJT
 * 10 Mar 14         - Changed indexes to BCD hex values - MEJT
 * 10 Dec 18         - Alternate function key now LIGHT_BLUE, allowing it to
 *                     have a different from the alternate text - MEJT
 *                      
 * TO DO :           - 
 */

#define TITLE          "RPN calc 29"
#define HEIGHT         385
#define WIDTH          201 /* Yes really! */
#define BUTTONS        30

#include "x11-calc-button.h"

/*
 * init_keypad (button[])
 *
 * Initializes the keypad.
 *
 */
 
int v_init_keypad(o_button* h_button[]){

   /* Define top row of keys. */ 
   h_button[0] = h_button_create(0x11, "SST", "FIX", "BST", h_normal_font, h_small_font, h_alternate_font, 12, 85, 33, 30, False, BLACK);
   h_button[1] = h_button_create(0x12, "GSB", "SCI", "RTN", h_normal_font, h_small_font, h_alternate_font, 48, 85, 33, 30, False, BLACK);
   h_button[2] = h_button_create(0x13, "GTO", "ENG", "LBL", h_normal_font, h_small_font, h_alternate_font, 84, 85, 33, 30, False, BLACK);
   h_button[3] = h_button_create(0x14, "f", "", "", h_normal_font, h_small_font, h_alternate_font, 120, 85, 33, 30, False, YELLOW);
   h_button[4] = h_button_create(0x15, "g", "", "", h_normal_font, h_small_font, h_alternate_font, 156, 85, 33, 30, False, LIGHT_BLUE);
   
   /* Define second row of keys. */ 
   h_button[5] = h_button_create(0x21, "X-Y", "x", "%", h_normal_font, h_small_font, h_alternate_font, 12, 128, 33, 30, False, BLACK);
   h_button[6] = h_button_create(0x22, "R", "s", "i", h_normal_font, h_small_font, h_alternate_font, 48, 128, 33, 30, False, BLACK);
   h_button[7] = h_button_create(0x23, "STO", "", "DSZ", h_normal_font, h_small_font, h_alternate_font, 84, 128, 33, 30, False, BLACK);
   h_button[8] = h_button_create(0x24, "RCL", "", "ISZ", h_normal_font, h_small_font, h_alternate_font, 120, 128, 33, 30, False, BLACK);
   h_button[9] = h_button_create(0x25, "E+", "E-", "DEL", h_normal_font, h_small_font, h_alternate_font, 156, 128, 33, 30, False, BLACK); 

   /* Define third row of keys. */
   h_button[10] = h_button_create(0x31, "ENTER", "PREFIX", "", h_normal_font, h_small_font, h_alternate_font, 12, 171, 69, 30, False, BLACK);
   h_button[11] = h_button_create(0x32, "CHS", "PRGM", "DEG", h_normal_font, h_small_font, h_alternate_font, 84, 171, 33, 30, False, BLACK);
   h_button[12] = h_button_create(0x33, "EEX", "REG", "RAD", h_normal_font, h_small_font, h_alternate_font, 120, 171, 33, 30, False, BLACK);
   h_button[13] = h_button_create(0x34, "CLX", "E", "GRD", h_normal_font, h_small_font, h_alternate_font, 156, 171, 33, 30, False, BLACK);

   /* Define fourth row of keys. */
   h_button[14] = h_button_create(0x41, "-", "X\x1a\x59", "X\x1b\x30", h_large_font, h_small_font, h_alternate_font, 12, 214, 33, 30, False, BEIGE);
   h_button[15] = h_button_create(0x7, "7", "LN", "eX" , h_large_font, h_small_font, h_alternate_font, 52, 214, 41, 30, False, BEIGE);
   h_button[16] = h_button_create(0x8, "8", "LOG", "10x", h_large_font, h_small_font, h_alternate_font, 100, 214, 41, 30, False, BEIGE);
   h_button[17] = h_button_create(0x9, "9", "-R", "-P", h_large_font, h_small_font, h_alternate_font, 148, 214, 41, 30, False, BEIGE);

   /* Define fifth row of keys. */
   h_button[18] = h_button_create(0x51, "+", "X>Y", "X>0", h_large_font, h_small_font, h_alternate_font, 12, 257, 33, 30, False, BEIGE);
   h_button[19] = h_button_create(0x4, "4", "SIN", "SIN-\xb9", h_large_font, h_small_font, h_alternate_font, 52, 257, 41, 30, False, BEIGE);
   h_button[20] = h_button_create(0x5, "5", "COS", "COS-\xb9", h_large_font, h_small_font, h_alternate_font, 100, 257, 41, 30, False, BEIGE);
   h_button[21] = h_button_create(0x6, "6", "TAN", "TAN-\xb9", h_large_font, h_small_font, h_alternate_font, 148, 257, 41, 30, False, BEIGE);

   /* Define sixth row of keys. */
   h_button[22] = h_button_create(0x61, "\xd7", "X\x1d\x59", "X\x1d\x30", h_large_font, h_small_font, h_alternate_font, 12, 300, 33, 30, False, BEIGE);
   h_button[23] = h_button_create(0x1, "1", "INT", "FRAC", h_large_font, h_small_font, h_alternate_font, 52, 300, 41, 30, False, BEIGE);
   h_button[24] = h_button_create(0x2, "2", "V\xaf", "x\xb2", h_large_font, h_small_font, h_alternate_font, 100, 300, 41, 30, False, BEIGE);
   h_button[25] = h_button_create(0x3, "3", "yX", "ABS", h_large_font, h_small_font, h_alternate_font, 148, 300, 41, 30, False, BEIGE);

   /* Define bottom row of keys. */
   h_button[26] = h_button_create(0x71, "\xf7", "X=Y", "X=0", h_large_font, h_small_font, h_alternate_font, 12, 343, 33, 30, False, BEIGE);
   h_button[27] = h_button_create(0x0, "0", "-H.MS", "-H", h_large_font, h_small_font, h_alternate_font, 52, 343, 41, 30, False, BEIGE);
   h_button[28] = h_button_create(0x73, ".", "LASTx", "\x1c", h_large_font, h_small_font, h_alternate_font, 100, 343, 41, 30, False, BEIGE);
   h_button[29] = h_button_create(0x74, "R/S", "PAUSE", "1/x", h_normal_font, h_small_font, h_alternate_font, 148, 343, 41, 30, False, BEIGE);

}



