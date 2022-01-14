/*
 * x11-keyboard.c - An X11 keyboard class.
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
 * 19 Sep 21         - Initial version (Cheese House)- MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0001"
#define DATE           "19 Sep 21"
#define AUTHOR         "MT"

#include <ctype.h>     /* is alpha(), etc. */
#include <string.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* malloc(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */

#include "x11-keyboard.h"

#include "gcc-debug.h"

/* Attempts to translate a key code into a character. */
static void v_key_decode(okeyboard *h_keyboard, Display *x_display, int i_keycode, int i_keystate) {
   h_keyboard->keysym = XKeycodeToKeysym(x_display, i_keycode, 0);
   h_keyboard->key = '\000';
   switch (h_keyboard->keysym) {
   /* Modifier keys */
   case XK_Control_L:
   case XK_Control_R:
   case XK_Shift_L:
   case XK_Shift_R:
   case XK_Alt_L: /* Alt */
   case XK_ISO_Level3_Shift: /* Alt Gr */
   case XK_Super_L: /* Left windows key */
   case XK_Super_R: /* Right windows key */
   case XK_Menu: /* Menu key */
   case XK_Num_Lock:
   case XK_Caps_Lock:
      break;  /* Just ignore all Keyboard Modifier keys */
   /* Control keys */
   case XK_Escape:
   case XK_BackSpace:
   case XK_Tab:
   case XK_Linefeed:
   case XK_Clear:
   case XK_Return:
   case XK_Pause:
   case XK_Scroll_Lock:
   case XK_Sys_Req:
   case XK_Delete:
      h_keyboard->key = h_keyboard->keysym & 0x1f; /* (values chosen to map to ASCII - see keysymdef.h) */
      break;
   /* Numeric keypad */
   case  XK_KP_F1:
   case  XK_KP_F2:
   case  XK_KP_F3:
   case  XK_KP_F4:
   case  XK_KP_Home:
   case  XK_KP_Left:
   case  XK_KP_Up:
   case  XK_KP_Right:
   case  XK_KP_Down:
   case  XK_KP_Page_Up:
   case  XK_KP_Page_Down:
   case  XK_KP_End:
   case  XK_KP_Begin:
   case  XK_KP_Insert:
   case  XK_KP_Delete:
   case  XK_KP_Equal:
      if (!(i_keystate & Mod2Mask) != !(i_keystate & ShiftMask)) { /* If numlock or shift (but not both) is pressed check number*/
         h_keyboard->keysym = XKeycodeToKeysym(x_display, i_keycode, 1);
         switch (h_keyboard->keysym) {
         case  XK_KP_Tab:
         case  XK_KP_Enter:
         case  XK_KP_Multiply:
         case  XK_KP_Add:
         case  XK_KP_Separator:
         case  XK_KP_Subtract:
         case  XK_KP_Decimal:
         case  XK_KP_Divide:
         case  XK_KP_0:
         case  XK_KP_1:
         case  XK_KP_2:
         case  XK_KP_3:
         case  XK_KP_4:
         case  XK_KP_5:
         case  XK_KP_6:
         case  XK_KP_7:
         case  XK_KP_8:
         case  XK_KP_9:
            h_keyboard->key = h_keyboard->keysym & 0x7f; /* (values chosen to map to ASCII - see keysymdef.h) */
            break;
         }
      }
      break;
   case  XK_KP_Tab:
   case  XK_KP_Enter:
   case  XK_KP_Multiply:
   case  XK_KP_Add:
   case  XK_KP_Separator:
   case  XK_KP_Subtract:
   case  XK_KP_Decimal:
   case  XK_KP_Divide:
      h_keyboard->key = h_keyboard->keysym & 0x7f; /* (values chosen to map to ASCII - see keysymdef.h) */
      break;
   /* Everything else */
   default:
      h_keyboard->key = h_keyboard->keysym & 0xff;
      if (isalpha(h_keyboard->keysym)) { /* For alpha keys check both caps lock and shift */
         if (!(i_keystate & ShiftMask) != !(i_keystate & LockMask))
            h_keyboard->key = XKeycodeToKeysym(x_display, i_keycode, 1);
      }
      else {
         if (i_keystate & ShiftMask)
            h_keyboard->key = XKeycodeToKeysym(x_display, i_keycode, 1);
      }

      if (i_keystate & ControlMask) {
         h_keyboard->key = XKeycodeToKeysym(x_display, i_keycode, 1);
         if (((h_keyboard->key >= '@') && (h_keyboard->key <= '_')) || ((h_keyboard->key >= 'a') && (h_keyboard->key <= 'z'))) /* Only modify valid control keys */
            h_keyboard->key &= 0x1f; /* (values chosen to map to ASCII - see keysymdef.h) */
      }
   }
}

/* Update the keyboard state */

okeyboard *h_key_pressed(okeyboard *h_keyboard, Display *x_display, int i_keycode, int i_keystate) {
   v_key_decode(h_keyboard, x_display, i_keycode, i_keystate);
   debug(fprintf(stderr, "Key pressed - '%s'.\n", XKeysymToString(h_keyboard->keysym)));
   return(h_keyboard);
}

/*
 * key_released (keyboard, keycode)
 *
 * Updates the keyboard state when a key is released.
 *
 */
okeyboard *h_key_released(okeyboard *h_keyboard, Display *x_display, int i_keycode, int i_keystate) {
   v_key_decode(h_keyboard, x_display, i_keycode, i_keystate);
   debug(fprintf(stderr, "Key released - '%s'.\n", XKeysymToString(h_keyboard->keysym)));
   return(h_keyboard);
}
/*
 * keyboard_create (index)
 *
 * Allocates storage for a new keyboard object, sets the default key states
 * and  returns a pointer to the keyboard object (or NULL to indecate  that
 * an error occoured when allocating memory).
 *
 */

okeyboard *h_keyboard_create(Display* x_display) {
   okeyboard *h_keyboard; /* Ponter to keyboard structure. */
   if ((h_keyboard = malloc (sizeof(*h_keyboard))) != NULL){
      h_keyboard->display = x_display;
      h_keyboard->key = '\000';
      h_keyboard->keysym = 0x0000;
   }
   else
      h_keyboard = NULL;
   return(h_keyboard);
}
