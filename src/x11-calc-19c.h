/*
 * x11-calc-19c.h - RPN (Reverse Polish) calculator simulator.
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
 * 24 Dec 22   0.1   - Initial version derived from HP29C- MT
 *
 */

#define HEIGHT             450 * SCALE_HEIGHT
#define WIDTH              236 * SCALE_WIDTH
#define BUTTONS            31
#define SWITCHES           2

#define DIGITS             13

#define DIGIT_COLOUR       RED
#define DIGIT_BACKGROUND   DARK_RED
#define DISPLAY_BACKGROUND RED_BACKGROUND
#define BEZEL_COLOUR       LIGHT_GREY

#define KBD_LEFT           12 * SCALE_WIDTH
#define KBD_TOP            165 * SCALE_HEIGHT
#define KBD_ROW            43 * SCALE_HEIGHT

#define BEZEL_LEFT         0 * SCALE_WIDTH
#define BEZEL_TOP          102 * SCALE_HEIGHT
#define BEZEL_WIDTH        236 * SCALE_WIDTH
#define BEZEL_HEIGHT       61 * SCALE_HEIGHT

#define DISPLAY_LEFT       0 * SCALE_WIDTH
#define DISPLAY_TOP        0 * SCALE_HEIGHT
#define DISPLAY_WIDTH      236 * SCALE_WIDTH
#define DISPLAY_HEIGHT     61 * SCALE_HEIGHT

#define KEY_HEIGHT         30 * SCALE_HEIGHT
#define KEY_WIDTH          33 * SCALE_WIDTH
#define KEY_NUMERIC        42 * SCALE_WIDTH

#define KEY_GAP            3 * SCALE_WIDTH
#define SWITCH_HEIGHT      10 * SCALE_HEIGHT

#define MEMORY_SIZE        48
#define ROM_SIZE           012000
/** #define CONTINIOUS /* Enable continious memory  */

extern int i_rom [ROM_SIZE];

void v_init_labels(olabel *h_label[]);

void v_init_switches(oswitch *h_label[]);

void v_init_buttons(obutton *h_button[]);
