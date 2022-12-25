/*
 * x11-calc-67.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2021   MT
 *
 * Model specific constants and function prototypes.
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
 * 12 Feb 22   0.1   - Initial version - MT
 *
 */

#define HEIGHT             433 * SCALE_HEIGHT
#define WIDTH              208 * SCALE_WIDTH
#define BUTTONS            39

#define DIGITS             12

#define KBD_LEFT           16 * SCALE_WIDTH
#define KBD_TOP            59 * SCALE_HEIGHT
#define KBD_ROW            43 * SCALE_HEIGHT /** Was 43 / 48 */

#define KEY_HEIGHT         30 * SCALE_HEIGHT
#define KEY_WIDTH          33 * SCALE_WIDTH /** 33 / 39 */
#define KEY_NUMERIC        43 * SCALE_WIDTH /** 42 / 50 */

#define KEY_GAP            3 * SCALE_WIDTH
#define SWITCH_HEIGHT      16 * SCALE_HEIGHT

#define DIGIT_COLOUR       DIM_GREY
#define DIGIT_BACKGROUND   MID_GREY
#define DISPLAY_BACKGROUND MID_GREY
#define BEZEL_COLOUR       MID_GREY

#define BEZEL_LEFT         4
#define BEZEL_TOP          4 * SCALE_HEIGHT
#define BEZEL_WIDTH        WIDTH * SCALE_HEIGHT - 8
#define BEZEL_HEIGHT       44 * SCALE_HEIGHT

#define DISPLAY_LEFT       0
#define DISPLAY_TOP        4
#define DISPLAY_WIDTH      BEZEL_WIDTH
#define DISPLAY_HEIGHT     BEZEL_HEIGHT


#define ROM_SIZE           030000
#define MEMORY_SIZE        320 /* HP41CV */
//#define CONTINIOUS

extern int i_rom [ROM_SIZE];

void v_init_buttons(obutton *h_button[]);
