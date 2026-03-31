/*
 * x11-calc-58.h - RPN (Reverse Polish) calculator simulator.
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
 * 29 Mar 26         - Initial version - MT
 *
 */


#define HEIGHT             440 * SCALE_HEIGHT
#define WIDTH              230 * SCALE_WIDTH
#define BUTTONS            40
#define SWITCHES           1

#define DIGITS             12

#define DIGIT_COLOUR       RED
#define DIGIT_BACKGROUND   DARK_RED
#define DISPLAY_BACKGROUND RED_BACKGROUND
#define BEZEL_COLOUR       LIGHT_GREY

#define BEZEL_LEFT         0 * SCALE_WIDTH
#define BEZEL_TOP          4 * SCALE_HEIGHT
#define BEZEL_WIDTH        230 * SCALE_WIDTH
#define BEZEL_HEIGHT       61 * SCALE_HEIGHT

#define DISPLAY_LEFT       0 * SCALE_WIDTH
#define DISPLAY_TOP        0 * SCALE_HEIGHT
#define DISPLAY_WIDTH      230 * SCALE_WIDTH
#define DISPLAY_HEIGHT     61 * SCALE_HEIGHT

#define KBD_LEFT           12 * SCALE_WIDTH
#define KBD_TOP            67 * SCALE_HEIGHT
#define KBD_ROW            43 * SCALE_HEIGHT

#define KEY_HEIGHT         25 * SCALE_HEIGHT
#define KEY_WIDTH          39 * SCALE_WIDTH
#define KEY_NUMERIC        48 * SCALE_WIDTH
#define KEY_SMALL          27 * SCALE_WIDTH
#define KEY_GAP            3 * SCALE_WIDTH
#define SWITCH_HEIGHT      10 * SCALE_HEIGHT

#define ROM_SIZE           010000
#define MEMORY_SIZE        1

#define TI57

extern int i_rom [ROM_SIZE];

void v_init_labels(struct olabel *h_label[]);

void v_init_switches(struct oswitch *h_label[]);

void v_init_buttons(struct obutton *h_button[]);
