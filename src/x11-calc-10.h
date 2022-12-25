/*
 * x11-calc-10.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2018   MEJT
 *
 * Model specific constants and function prototypes.
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
 * 26 Nov 22         - Initial version - MT
 * 24 Dec 22         - Added dummy digit to the HP10 display - MT
 *
 */

#define HEIGHT             450 * SCALE_HEIGHT
#define WIDTH              200 * SCALE_WIDTH
#define BUTTONS            24
#define SWITCHES           2
#define DIGITS             13

#define DIGIT_COLOUR       RED
#define DIGIT_BACKGROUND   DARK_RED
#define DISPLAY_BACKGROUND RED_BACKGROUND
#define BEZEL_COLOUR       WHITE

#define KBD_LEFT           7 * SCALE_WIDTH
#define KBD_TOP            175 * SCALE_HEIGHT /** 67 */
#define KBD_ROW            43 * SCALE_HEIGHT

#define BEZEL_LEFT         0 * SCALE_WIDTH
#define BEZEL_TOP          112 * SCALE_HEIGHT
#define BEZEL_WIDTH        200 * SCALE_WIDTH
#define BEZEL_HEIGHT       61 * SCALE_HEIGHT

#define DISPLAY_LEFT       0 * SCALE_WIDTH
#define DISPLAY_TOP        0 * SCALE_HEIGHT
#define DISPLAY_WIDTH      200 * SCALE_WIDTH
#define DISPLAY_HEIGHT     61 * SCALE_HEIGHT

#define KEY_HEIGHT         30 * SCALE_HEIGHT
#define KEY_WIDTH          35 * SCALE_WIDTH
#define KEY_NUMERIC        40 * SCALE_WIDTH
#define KEY_SMALL          25 * SCALE_WIDTH
#define KEY_GAP            3 * SCALE_WIDTH
#define SWITCH_HEIGHT      10 * SCALE_HEIGHT

#define ROM_SIZE           03400
#define MEMORY_SIZE        3

extern int i_rom [ROM_SIZE];

void v_init_labels(olabel *h_label[]);

void v_init_switches(oswitch *h_label[]);

void v_init_buttons(obutton *h_button[]);
