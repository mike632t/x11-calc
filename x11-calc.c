/*
 * x11-calc.c - RPN (Reverse Polish) calculator simulator. 
 *
 * Copyright(C) 2019 - MT
 *
 * Emulation of various models of HP calculator for X11.
 * 
 * Deliberately parses the command line without using 'getopt' or 'argparse'
 * to maximise portability.
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
 *                   - Modified  string  constants  in  key  definitions  to
 *                     define any hex character codes properly - MT
 * 27 Jun 13         - Any error opening the display are now handled in the
 *                     same way as other errors - MT
 * 30 Jun 13         - Defined text colours as constants - MT
 * 01 Jul 13         - Buttons now move when pressed - MT
 * 02 Jul 13         - Separated  code  and  headers for button  and  colour
 *                     manipulation into separate files - MT
 * 07 Jul 13         - Made  all  global variables local  variables  (except
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
 * 09 Mar 14         - Created  separate files for code that is dependent on
 *                     calculator the model - MT
 * 10 Mar 14         - Changed key code indexes to BCD hex values - MT
 *                   - Changed  names  of display masks to highlight  their
 *                     association with the display module - MT
 * 07 Dec 18         - Finally remembered how to pass WM_DELETE messages to
 *                     the  message loop so the application exits  properly
 *                     when the user closes the application window - MT
 *                   - When a key is 'pressed' the display goes blank for a
 *                     brief interval (just like the real thing) - MT
 * 10 Dec 18         - Alternate  function key now LIGHT_BLUE, allowing  it
 *                     to be a different colour to the alternate text - MT
 * 11 Dec 28         - Tidied up comments again - MT 
 * 23 Aug 20         - Modified the debug code (again), optional debug code
 *                     executed using the debug() macro, with print() being
 *                     used to print diagnostic messages - MT
 * 30 Aug 20         - Added number formatting routines - MT
 * 31 Aug 20         - Made  version(), about(), and error() model specific 
 *                     so that the name reflects the model number - MT
 * 31 Aug 20         - Resolved dependencies between header files by moving
 *                     common function definitions to a separate file - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 09 Aug 21         - The  main program loop now checks for pending  events
 *                     before  processing  any event messages, so  NextEvent
 *                     no longer blocks program execution - MT
 * 10 Aug 21         - Moved  version(), about(), and error() back to  their
 *                     original position - MT
 * 16 Aug 21         - Now calls tick() to execute  a single instruction for
 *                     each iteration of the main event loop - MT
 * 19 Aug 21         - Modified  processor simulation to make it more object
 *                     orientated - MT
 * 21 Aug 21         - Removed -? help option and fixed help text and (which
 *                     I should have done that a long time ago!) - MT
 *                   - Added a flags for the Shift, Ctrl and Alt keys - MT
 *                   - Added  single step and trace command line options and
 *                     the ability to control the processor using ctrl-s and
 *                     ctrl-q  for single stepping, and ctrl-t to enable  or
 *                     disable tracing - MT
 *                   - Added definition for commit id - MT
 *
 * TO DO :           - Don't blank display when a key is pressed (as it will
 *                     be blanked by the firmware automatically).
 *                   - Free up allocated memory on exit.
 *                   - Sort out colour mapping.
 * 
 */
 
#define NAME           "x11-rpncalc"
#define VERSION        "0.1"
#define BUILD          "0041"
#define DATE           "21 Aug 21"
#define AUTHOR         "MT"
 
#define DEBUG          1

#include <stdarg.h>    /* strlen(), etc. */
#include <string.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* getenv(), etc. */

#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */
 
#include "x11-calc-font.h" 
#include "x11-calc-button.h"
#include "x11-calc-colour.h"

#include "x11-calc.h"

#include "x11-calc-segment.h"
#include "x11-calc-display.h"
#include "x11-calc-cpu.h"

#include "gcc-debug.h" /* print() */
#include "gcc-wait.h"  /* i_wait() */

void v_version(int b_verbose) { /* Display version information */

   fprintf(stderr, "%s: Version %s %s", NAME, VERSION, COMMIT_ID);
   if (__DATE__[4] == ' ') fprintf(stderr, " 0"); else fprintf(stderr, " %c", __DATE__[4]);
   fprintf(stderr, "%c %c%c%c %s %s", __DATE__[5],
      __DATE__[0], __DATE__[1], __DATE__[2], __DATE__ +9, __TIME__ );
   fprintf(stderr, " (Build: %s)\n", BUILD );
   if (b_verbose) {
      fprintf(stderr, "Copyright(C) %s %s\n", __DATE__ +7, AUTHOR);
      fprintf(stderr, "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
      fprintf(stderr, "This is free software: you are free to change and redistribute it.\n");
      fprintf(stderr, "There is NO WARRANTY, to the extent permitted by law.\n");
   }
}

void v_about() { /* Display help text */
   fprintf(stdout, "Usage: %s [OPTION]... \n", NAME);
   fprintf(stdout, "An RPN Calculator simulation for X11.\n\n");
#ifdef vms /* Use DEC command line options */
   fprintf(stdout, "  /step                    trace execution\n");
   fprintf(stdout, "  /trace                   trace execution\n");
   fprintf(stdout, "  /version                 output version information and exit\n\n");
   fprintf(stdout, "  /?, /help                display this help and exit\n");
#else
   fprintf(stdout, "  -s, --step               start in single step\n");
   fprintf(stdout, "  -t, --trace              trace execution\n");
   fprintf(stdout, "      --help               display this help and exit\n");
   fprintf(stdout, "      --version            output version information and exit\n\n");
#endif
}

void v_error(const char *s_fmt, ...) { /* Print formatted error message */
   va_list t_args;
   va_start(t_args, s_fmt);
   fprintf(stderr, "%s : ", NAME);
   vfprintf(stderr, s_fmt, t_args);
   va_end(t_args);
}

 
int main (int argc, char *argv[]){

   Display *x_display; /* Pointer to X display structure. */
   Window x_application_window; /* Application window structure. */
   XEvent x_event;
   XSizeHints *h_size_hint;
   Atom wm_delete;
   
   obutton *h_button[BUTTONS]; /* Array to hold pointers to 30 buttons. */
   obutton *h_pressed;
   odisplay *h_display; /* Pointer to display strudture. */
   oprocessor *h_processor; 
   
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
   char b_trace = False; /* Trace flag */
   char b_step = False; /* Single step flag flag */
   char b_run = True; /* Run flag controls CPU instruction execution in main loop */
   char b_abort = False; /*Abort flag controls execution of main loop */
   char b_ctrl = 0, b_alt = 0, b_shift= 0; /* State of the ctrl, alt and shift keys */

#ifdef vms /* Parse DEC style command line options */
   for (i_count = 1; i_count < argc; i_count++) {
      if (argv[i_count][0] == '/') {
         for (i_index = 0; argv[i_count][i_index]; i_index++) /* Convert option to uppercase */
            if (argv[i_count][i_index] >= 'a' && argv[i_count][i_index] <= 'z')
               argv[i_count][i_index] = argv[i_count][i_index] - 32;
         if (!strncmp(argv[i_count], "/STEP", i_index)) {
            b_trace = True); /* Start in single step mode */
         if (!strncmp(argv[i_count], "/TRACE", i_index)) {
            b_trace = True); /* Enable tracing */
         if (!strncmp(argv[i_count], "/VERSION", i_index)) {
            v_version(True); /* Display version information */
            exit(0);
         } else if ((!strncmp(argv[i_count], "/HELP", i_index)) | (!strncmp(argv[i_count], "/?", i_index))) {
            v_about();
            exit(0);
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
   b_abort = False; /* Stop processing command line */
   for (i_count = 1; i_count < argc && (b_abort != True); i_count++) {
      if (argv[i_count][0] == '-') {
         i_index = 1;
         while (argv[i_count][i_index] != 0) {
            switch (argv[i_count][i_index]) {
            case 's': /* Start in single step mode */
               b_run = !(b_step = True);
               break;
            case 't': /* Enable tracing */
               b_trace = True;
               break;
            case '-': /* '--' terminates command line processing */
               i_index = strlen(argv[i_count]);
               if (i_index == 2)
                 b_abort = True; /* '--' terminates command line processing */
               else
                  if (!strncmp(argv[i_count], "--step", i_index)) {
                     b_step = True; /* Start in single step mode */
                  } else if (!strncmp(argv[i_count], "--trace", i_index)) {
                     b_trace = True; /* Enable tracing */
                  } else if (!strncmp(argv[i_count], "--version", i_index)) {
                     v_version(True); /* Display version information */
                     exit(0);
                  } else if (!strncmp(argv[i_count], "--help", i_index)) {
                     v_about();
                     exit(0);
                  } else { /* If we get here then the we have an invalid long option */
                     v_error("unrecognized option '%s'\nTry '%s --help' for more information.\n", argv[i_count], NAME);
                     exit(-1);
                  }
               i_index--; /* Leave index pointing at end of string (so argv[i_count][i_index] = 0) */
               break;
            default: /* If we get here the single letter option is unknown */
               v_error("invalid option -- '%c'\nTry '%s --help' for more information.\n", argv[i_count][i_index], NAME);
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

   if (argc > 1 ) {  /* Check command lime parameters - there shouldn't be any! */
#ifdef vms
      v_error("invalid parameter(s)\nTry '%s /help' for more information.\n", NAME);
#else
      v_error("invalid operand(s)\nTry '%s --help' for more information.\n", NAME);
#endif 
      exit(-1);
   }

   i_wait(200);  /* Sleep for 200 milliseconds to 'debounce' keyboard! */
   
   debug(v_version(False));
                     
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
   XSelectInput(x_display, x_application_window, FocusChangeMask | ExposureMask | 
      KeyPressMask | KeyReleaseMask | ButtonPressMask | 
      ButtonReleaseMask | StructureNotifyMask | SubstructureNotifyMask);
      
   wm_delete = XInternAtom(x_display, "WM_DELETE_WINDOW", False); /* Create a windows delete message 'atom'. */
   XSetWMProtocols(x_display, x_application_window, &wm_delete, 1); /* Tell the display to pass wm_delete messages to the application window. */

   /* Show window on display */
   XMapWindow(x_display, x_application_window); 
   XRaiseWindow(x_display, x_application_window); /* Raise window - ensures expose envent is raised? */
 
   /* Flush all pending requests to the X server, and wait until they are processed by the X server. */
   XSync(x_display, False);
   
   debug(fprintf(stderr, "ROM Size : %4u words \n", (unsigned)(sizeof(i_rom) / sizeof i_rom[0])));   
   h_processor = h_processor_create(i_rom); 
   
   h_processor->flags[TRACE] = b_trace;
   
   /* -1234567890e99e-99. */
   /* v_reg_load(h_processor->reg[A_REG], 0x9, 0x1, 0x2, 0x3, 0xf, 0xf, 0xf, 0xf, 0xf, 0x9, 0x9, 0x9, 0x0, 0x0); */
   /* v_reg_load(h_processor->reg[B_REG], 0x2, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0); */

   /* v_reg_load(h_processor->reg[C_REG], 0x9, 0x1, 0x2, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0); */

   /* Main program event loop. */
   b_abort = False;
   while (!b_abort) {
      i_wait(3);  /* Sleep for 3 milliseconds */

      /* Update and redraw the display. */ 
      i_display_update(x_display, x_application_window, i_screen, h_display, h_processor);
      i_display_draw(x_display, x_application_window, i_screen, h_display);
      XFlush(x_display);
      
      if (b_run) i_processor_tick(h_processor);
      if (b_step) b_run = False;

      while (XPending(x_display)) {

         XNextEvent(x_display, &x_event);
         
         /* debug(fprintf(stderr, "Event ID : %i.\n", x_event.type)); */
         
         switch (x_event.type) {
      
         case EnterNotify : 
            debug(fprintf(stderr, "Enter Notify Event\n")); 
            break;

         case FocusOut : 
            b_ctrl = b_alt = b_shift = 0; /* Clear keyboard modifiers */
            break;
         
         case KeyPress : 
            debug(fprintf(stderr, "Key pressed - keycode(%d).\n", x_event.xkey.keycode));
            
            if ((x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Control_L)) || \
               (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Control_R))) { 
               b_ctrl++;
            }
            else if (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Alt_L)) { 
               b_alt++;
            }
            else if ((x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Shift_L)) || \
               (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Shift_R))) { 
               b_shift++;
            }
            else if (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Q))  { /* Check for Ctrl-Q */
               if ((b_ctrl == 1) && (b_shift == 0)) b_step = !(b_run  = True);
            }
            else if (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_S))  { /* Check for Ctrl-S */
               if ((b_ctrl == 1) && (b_shift == 0)) b_step = b_run =  True; 
            }
            else if (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_T))  { /* Check for Ctrl-T */
               if ((b_ctrl == 1) && (b_shift == 0)) {
                  h_processor->flags[TRACE]  = !(h_processor->flags[TRACE]); /* Toggle CPU tracing */
               }
            }
            else if (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Escape)) { /* Check for Escape */
               b_abort = True;
            }
            else if (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_space))  { /* Check for space */
               if ((b_ctrl == 0) && (b_shift == 0)) b_step = b_run =  True; 
            }
            break;

         case KeyRelease :
            debug(fprintf(stderr, "Key released - keycode(%d).\n", x_event.xkey.keycode));
            if ((x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Control_L)) || \
               (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Control_R))) { 
               b_ctrl--;
            }
            else if (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Alt_L)) { 
               b_alt--;
            }
            else if ((x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Shift_L)) || \
               (x_event.xkey.keycode == XKeysymToKeycode(x_display, XK_Shift_R))) { 
               b_shift--;
            }
            break;

         case ButtonPress : 
            if (x_event.xbutton.button == 1) {
               for (i_count = 0; i_count < BUTTONS; i_count++){
                  h_pressed = h_button_pressed(h_button[i_count], x_event.xbutton.x, x_event.xbutton.y);
                  if (!(h_pressed == NULL)) {
                     h_pressed->state = True; 
                     i_button_draw(x_display, x_application_window, i_screen, h_pressed);
 
                     debug(fprintf(stderr, "Button pressed - keycode(%.2X).\n", h_pressed->index)); 

                     /* Clear the display segments and redraw it. */ 
                     for (i_count = 0; i_count < DIGITS ; i_count++)
                        h_display->segment[i_count]->mask = DISPLAY_SPACE;
                     i_display_draw(x_display, x_application_window, i_screen, h_display);
                     XFlush(x_display);

                     i_wait(100);  /* Show blank display */
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
 
                  debug(fprintf(stderr, "Button released - keycode(%.2X).\n", h_pressed->index)); 
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
            if (x_event.xclient.data.l[0] == wm_delete) b_abort = True;
            break;

         }
      }
      
   }

   /* Close connection to server. */
   XDestroyWindow(x_display, x_application_window);
   XCloseDisplay(x_display);
   exit(0);
}
