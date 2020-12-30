/*
 * x11-calc.c - RPN (Reverse Polish) calculator simulator. 
 *
 * Copyright(C) 2019 - MT
 *
 * Simulation of vairous models of HP calculator for X11.
 * 
 * Deliberatly parses the command line without using 'getopt' or 'argparse'
 * to maximize portability.
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
 * 16 Jun 13         - Can use predefined button colours - MT
 *                   - Added text to buttons - MT
 *                   - Defined  all  the font names and colours defined  as
 *                     constants - MT
 * 17 Jun 13         - All fonts now loaded in main routine - MT
 * 18 Jun 13         - Added font attributes to each button - MT
 * 23 Jun 13         - Added  button_create to create a button and return a
 *                     pointer to the new structure - MT
 *                   - Changed button_draw now extracts parameters from the
 *                     button structure - MT
 *                   - Modified  string  constants in  key  definations  to
 *                     define any hex character codes properly - MT
 * 27 Jun 13         - Any error opening the display are now handled in the
 *                     same way as other errors - MT
 * 30 Jun 13         - Defined text colours as constants - MT
 * 01 Jul 13         - Buttons now move when pressed - MT
 * 02 Jul 13         - Seperated  code  and headers for button  and  colour
 *                     manipulation into seperate files - MT
 * 07 Jul 13         - Made  all  global vairables local vairables  (except
 *                     fonts) - MT
 *                   - Use window size hints to set window size - MT
 *                   - Changed  display of inverse trig functions  so  that
 *                     minus sign is shown as a super script - MT
 * 14 Jul 13         - Added display segments - MT
 * 15 Jul 13         - Created a display object to allow the 7 segment LEDs
 *                     to  be  handled as a single display entity  to  make
 *                     updating the display easier - MT
 * 14 Aug 13         - Tidied up comments - MT
 * 17 Aug 13         - Window is now redrawn automatically when application
 *                     loads - MT  
 * 09 Mar 14   0.1   - Created seperate files for code that is dependent on
 *                     calculator the model - MT
 * 10 Mar 14         - Changed key code indexes to BCD hex values - MT
 *                   - Changed  names  of display masks to highlight  their
 *                     association with the display module - MT
 * 07 Dec 18         - Finally remembered how to pass WM_DELETE messages to
 *                     the  message loop so the application exits  properly
 *                     when the user closes the application window - MT
 *                   - When a key is 'pressed' the display goes blank for a
 *                     breif interval (just like the real thing) - MT
 * 10 Dec 18         - Alternate  function key now LIGHT_BLUE, allowing  it
 *                     to be a different color to the alternate text - MT
 * 11 Dec 28         - Tidied up comments again - MT 
 * 23 Aug 20         - Modified the debug code (again), optional debug code
 *                     executed using the debug() macro, with print() being
 *                     used to print diagnostic messages - MT
 * 30 Aug 20         - Added number formatting routines - MT
 * 31 Aug 20         - Made  version(), about(), and error() model specific 
 *                     so that the name reflects the model number - MT
 * 31 Aug 20         - Resolved dependancies between header files by moving
 *                     common function definitions to a sperate file - MT
 *
 * TO DO :           - Free up allocated memory on exit.
 *                   - Implement some functions (starting with  basic  math
 *                     operations).
 *                   - Sort out colour mapping.
 * 
 */
 
#define NAME         "x11-rpncalc"
#define VERSION      "0.1"
#define BUILD        "0030"
#define AUTHOR       "MT"
 
#define DEBUG        1
 
#include <X11/Xlib.h>      /* XOpenDisplay(), etc. */
#include <X11/Xutil.h>     /* XSizeHints etc. */
 
#include <stdarg.h>        /* strlen(), etc. */
#include <string.h>        /* strlen(), etc. */
#include <stdio.h>         /* fprintf(), etc. */
#include <stdlib.h>        /* getenv(), etc. */
 
#include "x11-calc-button.h"

#include "x11-calc.h"
 
#include "gcc-debug.h"     /* print() */
#include "gcc-wait.h"      /* i_wait() */
#include "x11-calc-colour.h"
#include "x11-calc-font.h" 
#include "x11-calc-segment.h"
#include "x11-calc-display.h"
#include "x11-calc-cpu.h"
 
int main (int argc, char *argv[]){

   Display *x_display; /* Pointer to X display structure. */
   Window x_application_window; /* Application window structure. */
   XEvent x_event;
   XSizeHints *h_size_hint;
   Atom wm_delete;
   
   o_button* h_button[BUTTONS]; /* Array to hold pointers to 30 buttons. */
   o_button* h_pressed;
   o_display* h_display; /* Pointer to display strudture. */
   char *s_display_name = ""; /* Just use the default display. */
   char *s_font; /* Font description. */

   char *s_title = TITLE; /* Windows title. */
   unsigned int i_window_width = WIDTH; /* Window width in pixels. */
   unsigned int i_window_height = HEIGHT; /* Window height in pixels. */

   unsigned int i_window_border = 4; /* Window's border width. */
   int i_window_left, i_window_top; /* Window's top-left corner */
   unsigned int i_colour_depth; /* Window's colour depth. */
   unsigned int i_background_colour; /* Window's background colour. */
   int i_screen; /* Default screen number. */  
   
   int i_count, i_index;
   int b_cont = True;
   
#ifdef vms /* Parse DEC style command line options */
   for (i_count = 1; i_count < argc; i_count++) {
      if (argv[i_count][0] == '/') {
         for (i_index = 0; argv[i_count][i_index]; i_index++) /* Convert option to uppercase */
            if (argv[i_count][i_index] >= 'a' && argv[i_count][i_index] <= 'z')
               argv[i_count][i_index] = argv[i_count][i_index] - 32;
         if (!strncmp(argv[i_count], "/VERSION", i_index)) {
            v_version(); /* Display version information */
         } else if (!strncmp(argv[i_count], "/HELP", i_index)) {
            v_about();
         } else if (!strncmp(argv[i_count], "/?", i_index)) {
            v_about();
         } else { /* If we get here then the we have an invalid option */
            v_error("invalid option %s\nTry '%s /help' for more information.\n", argv[i_count] , NAME);
            exit(-1);
         }
         if (argv[i_count][1] != 0) {
            for (i_index = i_count; i_index < argc - 1; i_index++) argv[i_index] = argv[i_index + 1];
            argc--; i_count--;
         }
      }
   }
#else /* Parse UNIX style command line options */
   char b_abort; /* Stop processing command line */
   for (i_count = 1; i_count < argc && (b_abort != True); i_count++) {
      if (argv[i_count][0] == '-') {
         i_index = 1;
         while (argv[i_count][i_index] != 0) {
            switch (argv[i_count][i_index]) {
            case '?': /* Display help */
               v_about();
            case '-': /* '--' terminates command line processing */
               i_index = strlen(argv[i_count]);
               if (i_index == 2)
                 b_abort = True; /* '--' terminates command line processing */
               else
                  if (!strncmp(argv[i_count], "--version", i_index)) {
                     v_version(); /* Display version information */
                  } else if (!strncmp(argv[i_count], "--help", i_index)) {
                     v_about();
                  } else { /* If we get here then the we have an invalid long option */
                     v_error("%s: invalid option %s\nTry '%s --help' for more information.\n", argv[i_count][i_index] , NAME);
                     exit(-1);
                  }
               i_index--; /* Leave index pointing at end of string (so argv[i_count][i_index] = 0) */
               break;
            default: /* If we get here the single letter option is unknown */
               v_error("unknown option -- %c\nTry '%s --help' for more information.\n", argv[i_count][i_index] , NAME);
               exit(-1);
            }
            i_index++; /* Parse next letter in options */
         }
         if (argv[i_count][1] != 0) {
            for (i_index = i_count; i_index < argc - 1; i_index++) argv[i_index] = argv[i_index + 1];
            argc--; i_count--;
         }
      }
   }
#endif 

   i_wait(200);  /* Sleep for 200 milliseconds to 'debounce' keyboard! */
                     
   /* Open the display and create a new window. */
   if (!(x_display = XOpenDisplay(s_display_name))) v_error ("Cannot connect to X server '%s'.\n", s_display_name); 

   /* Get the default screen for our X server. */
   i_screen = DefaultScreen(x_display);
   
   /* Set the background colour. */
   i_background_colour = BACKGROUND;

   /* Create the application window, as a child of the root window. */
   x_application_window = XCreateSimpleWindow(x_display, 
      RootWindow(x_display, i_screen),
      i_window_width, i_window_height,  /* Window position -igore ? */
      i_window_width, /* Window width. */
      i_window_height, /* Window height. */
      i_window_border, /* Border width - ignored ? */
      BlackPixel(x_display, i_screen), /* Preferred method. */
      i_background_colour); 

   /* Set application window size. */
   h_size_hint = XAllocSizeHints();
   h_size_hint->flags = PMinSize | PMaxSize;
   h_size_hint->min_height = i_window_height;
   h_size_hint->min_width = i_window_width;
   h_size_hint->max_height = i_window_height;
   h_size_hint->max_width = i_window_width;
   XSetWMNormalHints(x_display, x_application_window, h_size_hint);
   XStoreName(x_display, x_application_window, s_title); /* Set the window title. */
   
   /* Get window geometry. */
   if (XGetGeometry(x_display, x_application_window, 
         &RootWindow(x_display, i_screen),
         &i_window_left, &i_window_top, 
         &i_window_width,
         &i_window_height,
         &i_window_border,
         &i_colour_depth) == False) v_error("Unable to get display properties.\n");
      
   /* Check colour depth. */
   if (i_colour_depth < 24) v_error("Requires a 24-bit colour display.\n");

   /* Load fonts. */
   s_font = NORMAL_TEXT; /* Normal text font. */
   if (!(h_normal_font = XLoadQueryFont(x_display, s_font))) v_error("Cannot load font '%s'.\n",  s_font);

   s_font = SMALL_TEXT; /* Small text font. */
   if (!(h_small_font = XLoadQueryFont(x_display, s_font))) v_error("Cannot load font '%s'.\n",  s_font);

   s_font = ALTERNATE_TEXT; /* Alternate text font. */
   if (!(h_alternate_font = XLoadQueryFont(x_display, s_font))) v_error("Cannot load font '%s'.\n",  s_font);          

   s_font = LARGE_TEXT; /* Large text font. */

   if (!(h_large_font = XLoadQueryFont(x_display, s_font))) v_error("Cannot load font '%s'.\n",  s_font);          
   
   /* Create buttons. */
   v_init_keypad(h_button);
 
   /* Create display */
   h_display = h_display_create(0, 2, 4, 197, 61, RED, DARK_RED, RED_BACKGROUND);
 
   /* Draw display. */
   i_display_draw(x_display, x_application_window, i_screen, h_display);

   /* Draw buttons. */
   for (i_count = 0; i_count < BUTTONS; i_count++)
      if (!(h_button[i_count] == NULL)) i_button_draw(x_display, x_application_window, i_screen, h_button[i_count]);
 
   /* Flush all pending requests to the X server, and wait until they are processed by the X server. */
   XSync(x_display, False);

   /* Select kind of events we are interested in. */
   XSelectInput(x_display, x_application_window, ExposureMask | 
      KeyPressMask | KeyReleaseMask | ButtonPressMask | 
      ButtonReleaseMask | StructureNotifyMask | SubstructureNotifyMask);
      
   wm_delete = XInternAtom(x_display, "WM_DELETE_WINDOW", False); /* Create a windows delete message 'atom'. */
   XSetWMProtocols(x_display, x_application_window, &wm_delete, 1); /* Tell the display to pass wm_delete messages to the application window. */

   /* Show window on display */
   XMapWindow(x_display, x_application_window); 
   XRaiseWindow(x_display, x_application_window); /* Raise window - ensures expose envent is raised? */
 
   /* Flush all pending requests to the X server, and wait until they are processed by the X server. */
   XSync(x_display, False);
   
   /* -1234567890. */
   i_reg_load(gc_a_reg, 0x9, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0, 0xf, 0xf, 0xf);
   i_reg_load(gc_b_reg, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0);

   /* 1/-1234567890. */
   i_reg_load(gc_a_reg, 0x9, 0x8, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x9, 0x1, 0x0, 0x0, 0x0);
   i_reg_load(gc_b_reg, 0x2, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0);
 
   /* Main program event loop. */
   while (b_cont) {

      XNextEvent(x_display, &x_event);
      
      /*
       * debug(fprintf(stderr, "Debug\t: %s line : %d : Event ID : %i.\n", \
       * __FILE__, __LINE__, x_event.type));      
       */
      
      switch (x_event.type) {
      
         case EnterNotify : 
            debug(fprintf(stderr, "Debug\t: %s line : %d : Notify raised.\n", \
            __FILE__, __LINE__)); 
            break;

         case KeyPress : 
            debug(fprintf(stderr, "Debug\t: %s line : %d : Key pressed - keycode(%d).\n", \
            __FILE__, __LINE__, x_event.xkey.keycode));
            if (x_event.xkey.keycode == XKeysymToKeycode(x_display, XStringToKeysym("Escape"))) { 
               b_cont = False;
            }
            break;

         case KeyRelease :
            debug(fprintf(stderr, "Debug\t: %s line : %d : Key released.\n", \
            __FILE__, __LINE__)); 
            break;

         case ButtonPress : 
            if (x_event.xbutton.button == 1) {
               for (i_count = 0; i_count < BUTTONS; i_count++){
                  h_pressed = h_button_pressed(h_button[i_count], x_event.xbutton.x, x_event.xbutton.y);
                  if (!(h_pressed == NULL)) {
                     h_pressed->state = True; 
                     i_button_draw(x_display, x_application_window, i_screen, h_pressed);
                     
                     /* 
                      * debug(fprintf(stderr, "Debug\t: %s line : %d : Button %.2X is %s.\n", \
                      * __FILE__, __LINE__, h_pressed->index, (h_pressed->state)?"TRUE":"FALSE")); 
                      */
                      
                     debug(fprintf(stderr, "Debug\t: %s line : %d : Button pressed - keycode(%.2X).\n", \
                     __FILE__, __LINE__, h_pressed->index)); 

                     /* Clear display. */ 
                     for (i_count = 0; i_count < DIGITS ; i_count++)
                        h_display->segment[i_count]->mask = DISPLAY_SPACE;
                     
                     /* Redraw display. */
                     i_display_draw(x_display, x_application_window, i_screen, h_display);
                     XFlush(x_display);

                     i_wait(100);  /* Show blank display */

                     /* Update display. */ 
                     h_display->segment[1]->mask = DISPLAY_ZERO | DISPLAY_DECIMAL;
                     h_display->segment[2]->mask = DISPLAY_ZERO;
                     h_display->segment[3]->mask = DISPLAY_ZERO;
                     
                     i_display_update(x_display, x_application_window, i_screen, h_display);

                     /* Redraw display. */
                     i_display_draw(x_display, x_application_window, i_screen, h_display);
                     break;
                  }                 
               }
            }
            break;

         case ButtonRelease : 
            if (x_event.xbutton.button == 1) {
               if (!(h_pressed == NULL)) {
                  h_pressed->state = False;
                  i_button_draw(x_display, x_application_window, i_screen, h_pressed);
                  
                  /*
                   * debug(fprintf(stderr, "Debug\t: %s line : %d : Button %.2X is %s.\n", \
                   * __FILE__, __LINE__, h_pressed->index, (h_pressed->state)?"TRUE":"FALSE")); 
                   */

                  debug(fprintf(stderr, "Debug\t: %s line : %d : Button released.\n", \
                  __FILE__, __LINE__)); 

                  /* Redraw display. */
                  i_display_draw(x_display, x_application_window, i_screen, h_display);
               }
            }
            break;

         case Expose : /* Draw or redraw the window. */

            /* Redraw display. */
            i_display_draw(x_display, x_application_window, i_screen, h_display);

            /* Draw buttons. */
            for (i_count = 0; i_count < 30; i_count++)
               if (!(h_button[i_count] == NULL)) i_button_draw(x_display, x_application_window, i_screen, h_button[i_count]);
            break;
         case ClientMessage : /* Message from window manager */
            if (x_event.xclient.data.l[0] == wm_delete) b_cont = False;
            break;

      }
   }

   /* Close connection to server. */
   XDestroyWindow(x_display, x_application_window);
   XCloseDisplay(x_display);
   exit(0);
}
