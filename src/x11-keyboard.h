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
 * 19 Sep 13    0.1  - Initial version - MT
 * 22 Apr 24         - Tidied up data types - MT
 * 23 Apr 24         - More changes to data types - MT
 * 16 Aug 25         - Added flag to optionally ignore numlock - MT
 * 15 Sep 25         - Changed bool to char - MT
 * 07 Dec 25         - Use int for boolean types instead of char - MT
 * 14 Feb 26    0.2  - Change function prototypes to pass XKeyEvent - MT
 *                   - Removed  the display property and added a pointer to
 *                     the key name - MT
 *                   - Added NumLock mask property - MT
 *                   - Define Keyboard mask - MT
 * 15 Feb 26         - Enable  keyboard on all operating systems, not  just
 *                     UNIX - MT
 * 24 Feb 26         - Corrected function names - MT
 *
 */

#define KeyboardMask (ShiftMask | LockMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask)  /* All keyboard event masks */

typedef struct { /* Calculator button structure. */
   KeySym keysym;
   const char* name;
   char key;
   unsigned int NumLockMask;
} okeyboard;

void v_key_pressed(okeyboard *h_keyboard, Display *x_display, XKeyEvent *x_event, int b_numlock);

void v_key_released(okeyboard *h_keyboard, Display *x_display, XKeyEvent *x_event, int b_numlock);

okeyboard *h_keyboard_create(Display *x_display);

