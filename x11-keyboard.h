/*
 * x11-keyboard.h - An X11 keyboard class.
 *
 * Copyright(C) 2021   MT
 * 
 * A  very simple keyboard 'class' to translate keystrokes into basic ASCII
 * character codes using the key definitions in keysymdef.h.  
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
 * 19 Sep 13         - Initial version - MT
 *
 */

typedef struct { /* Calculator button structure. */
   Display* display;
   char key;
   int keysym;
} okeyboard;

okeyboard *h_key_pressed(okeyboard *h_keyboard, Display *x_display, int i_keycode, int i_keystate);

okeyboard *h_key_released(okeyboard *h_keyboard, Display *x_display, int i_keycode, int i_keystate);

okeyboard *h_keyboard_create(Display* x_display);
