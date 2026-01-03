/*
 * x11-calc.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2019 - MT
 *
 * Emulation of various models of HP calculator for X11.
 *
 * Deliberately  does not use 'getopt' or 'argparse' to parse command  line
 * in  order  to minimize the number of dependencies and make it easier  to
 * build the code on as many different systems as possible.
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
 *                   - Defined all font names and colours as constants - MT
 * 17 Jun 13         - All fonts now loaded in main routine - MT
 * 18 Jun 13         - Added font attributes to each button - MT
 * 23 Jun 13         - Added  button_create to create a button and return a
 *                     pointer to the new structure - MT
 *                   - Changed button_draw now extracts parameters from the
 *                     button structure - MT
 *                   - Modified  string  constants  in  key  definitions to
 *                     define any hex character codes properly - MT
 * 27 Jun 13         - Any error opening the display are now handled in the
 *                     same way as other errors - MT
 * 30 Jun 13         - Defined text colours as constants - MT
 * 01 Jul 13         - Buttons now move when pressed - MT
 * 02 Jul 13         - Separated  code  and  headers for button and  colour
 *                     manipulation into separate files - MT
 * 07 Jul 13         - Made  all global variables local  variables  (except
 *                     fonts) - MT
 *                   - Use window size hints to set window size - MT
 *                   - Changed  display  of inverse trig functions so  that
 *                     minus sign is shown as a super script - MT
 * 14 Jul 13         - Added display segments - MT
 * 15 Jul 13         - Created a display , to allow the 7 segment LEDs
 *                     to  be  handled as a single display entity  to  make
 *                     updating the display easier - MT
 * 14 Aug 13         - Tidied up comments - MT
 * 17 Aug 13         - Window is now redrawn automatically when application
 *                     loads - MT
 * 09 Mar 14         - Created separate files for code that is dependent on
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
 * 11 Dec 18         - Tidied up comments again - MT
 * 23 Aug 20         - Modified the debug code (again), optional debug code
 *                     executed using the debug() macro, with print() being
 *                     used to print diagnostic messages - MT
 * 30 Aug 20         - Added number formatting routines - MT
 * 31 Aug 20         - Made  version(), about(), and error() model specific
 *                     so that the name reflects the model number - MT
 * 31 Aug 20         - Resolved dependencies between header files by moving
 *                     common function definitions to a separate file - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 09 Aug 21         - The main program loop now checks for pending  events
 *                     before  processing  any event messages, so NextEvent
 *                     no longer blocks program execution - MT
 * 10 Aug 21         - Moved  version(), about(), and error() back to their
 *                     original position - MT
 * 16 Aug 21         - Executes a single instruction in every iteration  of
 *                     the main event loop - MT
 * 19 Aug 21         - Modified processor simulation to make it more object
 *                     orientated - MT
 * 21 Aug 21         - Removed short form of the help option and fixed help
 *                     text - MT
 *                   - Added a flags for the Shift, Ctrl and Alt keys - MT
 *                   - Added single step and trace command line options and
 *                     the ability  to control the processor  using  ctrl-s
 *                     and ctrl-q for single stepping, and ctrl-t to enable
 *                     or disable tracing - MT
 *                   - Added definition for commit id - MT
 * 30 Aug 21         - Separated version and licence notices into their own
 *                     routines - MT
 *                   - Abort if an error occurs - MT
 *  2 Sep 21         - Can  now  enable single stepping and tracing using a
 *                     hard-coded break-point - MT
 *  5 Sep 21         - The display is no longer blanked automatically  when
 *                     a key is pressed - MT
 *                   - Key press events now handled properly using keypress
 *                     and keystate properties - MT
 *                   - Tidied up comments - MT
 *  9 Sep 21         - Don't trace busy loops - MT
 * 12 Sep 21         - Fixed bug with single step and trace  - MT
 *                   - Added option to allow break-point to be set from the
 *                     command line - MT
 * 14 Sep 21         - Eliminated  the delay between ticks when the display
 *                     is blank - MT
 *             0.2   - HP21 simulator works !!
 * 15 Sep 21         - The  escape key now resets the simulator instead  of
 *                     exiting - MT
 * 16 Sep 21         - Improved the display flicker problem by updating the
 *                     display  only once every 100 ticks instead  of every
 *                     tick- MT
 * 18 Sep 21         - Added keycode to the button properties and a  method
 *                     to  determine if this corresponds to a  button which
 *                     allows a button to be operated by a key - MT
 * 19 Sep 21         - Created  a keyboard class to keep track of the input
 *                     state and translate keystrokes into characters - MT
 * 21 Sep 21         - Set the default active button to NULL - MT
 * 28 Sep 21         - The  escape key now mapped to 'Clx' - MT
 *  1 Oct 21         - Converted flags to Boolean variables - MT
 *  2 Oct 21         - Changed property names - MT
 *  4 Oct 21         - Added the ability to display the CPU registers using
 *                     Ctrl-R when in trace mode - MT
 *  7 Oct 21   0.3   - HP25 simulator now working..
 * 10 Oct 21         - Allows  switches  and buttons to be undefined if not
 *                     used - MT
 * 11 Oct 21         - Most text  messages are now defined in  the  header,
 *                     this allows the content to be defined separately for
 *                     each platform which means that about() and licence()
 *                     can  be replaced by simple calls to fprintf().  This
 *                     also  makes it easier to support multiple  languages
 *                     if necessary in the future  - MT
 *                   - Required  colour depth now defined in header (allows
 *                     code to work on VMS using a black and white  display
 *                     without being modified) - MT
 * 12 Oct 21         - Added routine to display warnings - MT
 * 14 Oct 21         - Added support for continuous memory - MT
 * 16 Oct 21   0.4   - HP29 simulator works..!
 *                   - Undefined  switches should be off.  Rather strangely
 *                     the HP27 tests S3 even though it doesn't have a mode
 *                     select switch - go figure - MT
 *             0.5   - HP27 simulator works (requires testing).
 *             0.6   - HP31 and HP32 simulators work (require testing).
 * 26 Oct 21   0.7   - Added the ability to allow the user to specify which
 *                     file should be used when restoring the initial state
 *                     of  the registers on the command line (only  applies
 *                     to models with continuous memory) - MT
 *                   - HP 33C simulator works.
 * 29 Oct 21         - Draw display when window is exposed - MT
 * 01 Nov 21         - Explicitly define the cursor - MT
 *                   - Attempts to center the window on the  display.  Most
 *                     window managers ignore this, but it does work if the
 *                     application is invoked directly by startx - MT
 * 02 Nov 21         - Allows size of the window to be changed by modifying
 *                     the value of SCALE at compile time - MT
 * 15 Nov 21         - Holding down the off switch for two seconds will end
 *                     the simulation - MT
 * 17 Nov 21         - Defined text messages as string constants instead of
 *                     macros and moved them into this file to get the code
 *                     to compile using VAXC.  I would have preferred to be
 *                     able  to define them in a separate language specific
 *                     module but I can't figure out how to make it work on
 *                     the older C compilers - MT
 * 21 Nov 21         - Mapped backspace key to escape - MT
 * 28 Nov 21         - Made the trace flag a processor property - MT
 *             0.8   - HP34 simulator works (require testing).
 * 02 Dec 21         - Removed any references to TRACE and fixed the bug in
 *                     the CPU status that broke radians on the HP29 - MT
 * 07 Dec 21         - Fixed bug in trace - MT
 * 16 Dec 21         - Since later models use more complex tests to monitor
 *                     user input detecting and suppressing the output from
 *                     a 'busy wait' loop isn't useful so I removed it - MT
 * 17 Dec 21         - Counts number of ticks elapsed when holding down the
 *                     off switch instead of counting the number of seconds
 *                     which is more accurate and removes the dependency on
 *                     time.h - MT
 * 20 Dec 21         - Changed all #ifdef to #if defined() - MT
 * 22 Dec 21         - Uses model numbers for conditional compilation - MT
 * 26 Dec 21         - Checks the on/off switch state at startup - MT
 * 03 Jan 22         - Added ability to trap execution of an opcode - MT
 * 03 Jan 22         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 04 Jan 22         - Updated help text - MT
 * 12 Jan 22         - Only sets mode switch state at start up - MT
 *                   - Checks  for breakpoints and instruction traps at the
 *                     same time - MT
 * 21 Jan 22   0.9   - Moved text messages to a separate file  - MT
 * 23 Jan 22         - Removed unwanted debug code - MT
 * 29 Jan 22         - Added an optional bezel to the display, not that you
 *                     will see it yet - MT
 * 31 Jan 22         - Added support for the HP10C, HP11C, HP12C, HP15C and
 *                     HP16C - MT
 * 28 Feb 22         - Read option implemented to allow ROM contents to  be
 *                     loaded from a file - MT
 * 04 Mar 22         - Modified  the delay between each tick for the HP10C,
 *                     HP11C, HP12C, HP15C and HP16C - MT
 * 14 Mar 22         - Modified delay timings to speed up the HP10C, HP11C,
 *                     HP12C, HP15C and HP16C slightly.
 * 31 Mar 22         - Modified way the delay values are calculated to make
 *                     the time needed to hold down the off button the same
 *                     for all simulators - MT
 *                   - Reduced the length of time that the off button  must
 *                     be held down to exit - MT
 * 26 May 22         - Blank line after an error message not needed - MT
 * 11 Dec 22         - Renamed models with continious memory and added HP25
 *                     HP33E and HP38E - MT
 * 24 Dec 22         - Command line parsing routine now uses the same macro
 *                     definitions as the the error message definitions.
 *                   - Execution  speed  now uses VOYAGER and  SPICE  macro
 *                     definitions - MT
 * 28 Dec 22         - Changed the name of printer mode status from mode to
 *                     print - MT
 *                   - Changed the name of prgm/run mode status from select
 *                     to mode - MT
 * 18 Jan 23         - Fixed  warnings when adding an offset to the pointer
 *                     to a string (__DATE__ + 9 and __DATE__ +7) - MT
 * 02 Feb 23         - Changed 'linux' to '__linux__' to fix a problem with
 *                     conditional compilation that stopped keypress events
 *                     from being processed - MT
 * 15 Oct 23         - Checks  the window size and resizes it if  necessary
 *                     if the Windows Manager doesn't act on  WM_SIZE_HINTS
 *                     (WSL2, Raspberry Pi OS) - MT
 * 16 Oct 23         - Decided not to include any code to work around a bug
 *                     in the Wayfire Window Manager that means it does not
 *                     respond properly to WM_SIZE_HINTS, hopefully it will
 *                     be fixed upsteam - MT
 * 21 Oct 23   0.10  - HP10 now uses a three positon switch to select print
 *                     mode - MT
 * 22 Oct 23         - Switch state now updated via a method - MT
 * 26 Oct 23         - Make the HP10 disable the display in PRINT mode - MT
 * 01 Nov 23         - Added /ROM qualifier for non Unix like plaforms - MT
 * 10 Feb 24         - Check that the ROM contents are not empty, and print
 *                     an error if they are.  Hopefully this will serve  as
 *                     a reminder to specify the ROM file when using any of
 *                     the Voyager emulators - MT
 *                   - Use  ROM_SIZE instead of calculating the size of the
 *                     ROM - MT
 *             0.11  - Display the version information just before the  ROM
 *                     size when starting so any error messages are printed
 *                     before the version number is shown - MT
 * 18 Feb 24         - Updated  version number format to include the  build
 *                     number - MT
 * 19 Feb 24         - Check that stat() was successful before checking the
 *                     if the file is a directory or a file! - MT
 *                   - Closes ROM file after reading - MT
 * 23 Feb 24         - Fixed bug in read_rom() - MT
 * 24 Feb 24   0.12  - Defined an array of alternative fonts for each style
 *                     of  text required and select the most suitable fonts
 *                     from these alternatives to reduce the dependancy  on
 *                     a single set of fonts - MT
 * 26 Feb 24         - Errors will print a formatted error message and exit
 *                     returning the errno - MT
 * 27 Feb 24         - Set errno to EBFONT if an error occours loading when
 *                     loading a font - MT
 *                   - If the ROM is empty set errno to ENODATA - MT
 *                   - Fixed busy loop 'bug' on newer compilers that do not
 *                     set 'linux' when compiling on Linux - MT
 * 01 Mar 24         - Return the correct error status if an invalid option
 *                     or parameter is passed on the command line - MT
 * 03 Mar 24         - Updated error handling (now passes the  error number
 *                     to the error handler) - MT
 *                   - Updated font sizes for new fonts - MT
 * 04 Mar 24         - Moved the initial window position to the lower left-
 *                     hand side of the display (if the window manager will
 *                     allow it!) - MT
 *                   - Check  that  all error codes are defined and  define
 *                     any that are not - MT
 *                   - Added desktop file - MT
 * 09 Mar 24         - Updated makefile and screenshots - MT
 * 14 Mar 24         - Allow SCALE_HEIGHT and SCALE_WIDTH to be passed from
 *                     command line at compile time - MT
 * 18 Mar 24   0.14  - Embedded missing firmware - MT
 * 29 Mar 24         - Fixed compiler warnings - MT
 * 30 Mar 24         - Corrected number of switches on models with only one
 *                     switch - MT
 * 09 Apr 24         - Removed some unused parameters from display-update()
 *                     function - MT
 *                   - Changed display data structure to store the size and
 *                     position of the bezel and display using a predefined
 *                     XRectangle structure - MT
 *                   - Display structure stores both the original geometery
 *                     and the current position of the display - MT
 *                   - Finally renamed x11-calc-segment to the more correct
 *                     x11-calc-digit - MT
 * 12 Apr 24         - Added command line option to allow window size to be
 *                     increased '--zoom' - MT
 *                   - Added parameter to '--zoom' to allow the scale to be
 *                     adjusted - MT
 *                   - Updated error messages to make the out of range text
 *                     more generic (only German translation changed) - MT
 * 13 Apr 24         - Uses the key height to draw a button with horizontal
 *                     dividing line in the same place regardless of aspect
 *                     ratio - MT
 * 14 Apr 24         - Fixed zoom validation and checks that zoom value has
 *                     been specified - MT
 *                   - Allows the display on the SPICE series to be changed
 *                     to the use european format,  effectively cutting the
 *                     jumper on the power supply board - MT
 * 18 Apr 24         - Checks for undefined labels when updating indicators
 *                     on the display (fixed segmentation fault) - MT
 * 22 Apr 24         - Define display colour separately - MT
 * 23 Apr 24         - Separated out prototypes for error handlers - MT
 *                   - Tidied up include files and removed some (currently)
 *                     redundant code - MT
 * 02 May 24         - Added shortcut keys for 'A-E' on 15C and 11C - MT
 *                   - Keyboard shortcuts enabled on FreeBSD - MT
 * 03 May 24         - Sets the abort flag and interval counter immediately
 *                     before the main loop - MT
 * 04 May 24         - Do not define unused switches - MT
 * 19 May 24         - Remove unnecessary call to set windows size - MT
 * 15 Jun 24         - Sets the application icon to the X windows logo - MT
 * 24 Jul 24         - Updated release meta data - MT
 * 07 Nov 24         - Tidied up comments - MT
 * 20 May 25   0.15  - Tidied up data structure definitions - MT
 * 25 May 25         - Changed normal exit status to EXIT_SUCCESS - MT
 * 11 Jun 25         - Allows multiple breakpoints to be specified - MT
 * 12 Jun 25         - Simplified linear search function (it didn't need to
 *                     be able to handle any data type) - MT
 *                   - Added  error messages for the number of  breakpoints
 *                     and instruction traps - MT
 * 14 Jun 25         - Increased the maximum number of breakpoints - MT
 * 24 Jun 25         - Fixed storage overflow error display - MT
 * 29 Jun 25         - Changed command line option for the european display
 *                     format to '-c' or '--comma' - MT
 * 07 Jul 25         - Fixed regression bug that affected HP12C - MT
 *             0.16  - Finished adding support for HP10 - MT
 *  3 Aug 25   0.17  - Fix display flicker with double buffering - JC
 *                   - Updates display only when buffer is redrawn - MT
 *  9 Aug 25         - Minor changes to fix compilation issues on VAXC - MT
 *                   - Fixed issues with window sizing on DEC Windows - MT
 *                   - Removed redundant variable - MT
 * 11 Aug 25         - Print the program version details before any warning
 *                     messages - MT
 *                   - Increased  the number of ticks between each  display
 *                     update to make the user interface more responsive as
 *                     well as making the number of ticks a multiple of 2 3
 *                     and 4 - MT
 * 14 Aug 25         - Fixed compiler warnings with clang 17.0.6 - MT
 * 16 Aug 25  (0181) - Added command line option to ignore numlock - MT
 * 20 Aug 25         - Added ability to load a saved state from a file -MT
 * 21 Aug 25         - Loading  or saving the current machine state can  be
 *                     done by right clicking on the application window. In
 *                     run mode the user will be prompted to load the state
 *                     from a previously saved copy, while in prgm mode the
 *                     user will be prompted to save the current state to a
 *                     file - MT
 * 22 Aug 25         - Use the PRGM label to set the processor mode for the
 *                     Voyager models (as they don't have a switch)
 *            (0185) - Clear any pending events after loading or saving the
 *                     machine state, this stops the application closing if
 *                     the user tries to close the window when a dialog box
 *                     is open - MT
 * 23 Aug 25   0.18  - Disable display when printer is in TRACE mode - MT
 * 03 Sep 25         - Added a workaround for issues with Flatpak - MT
 * 05 Sep 25         - Implemented file selection dialog using GTK - MT
 * 06 Sep 25         - Reorganized  continuous memory save/restore routines
 *                     and defined the conditional code in the main program
 *                     removing the need for dummy functions - MT
 * 12 Sep 25   0.19  - Added HP55 - MT
 *                   - Improved accuracy of HP55 timer - MT
 * 15 Sep 25         - Fixed errors when compiling on MacOS - MT
 * 20 Sep 25         - Explicitly include X11 keyboard symbols - MT
 *            (0194) - Enable keyboard shortcuts on any UNIX - MT
 * 17 Oct 25         - Added reset option - MT
 * 18 Oct 25         - Added card reader support - KJC
 *                   - Extended breakpoint address range - KJC
 *                   - Retained the (inaccurate) 'continious' memory on the
 *                     HP67 but right click now prompts the user to  insert
 *                     a card file - MT
 *                   - Ignore right click if GTK isn't available - MT
 * 27 Oct 25         - Added  function key labels for HP67 (only shown when
 *                     function keys are enabled) - MT
 * 30 Oct 25  (0201) - Update function key labels and redraw them when  the
 *                     function state changes - MT
 *                   - Window geometry can be specified on the command line
 *                     (size is ignored) - MT
 *                   - Reworked  the window hints.  By default  the  window
 *                     will centred on the screen, but this will usually be
 *                     ignored by the window manager unless the position is
 *                     specified on the command line - MT
 *                   - HP67 will display default function keys labels if no
 *                     program is loaded - MT
 * 01 Nov 25         - Added program card to HP67 - MT
 *                   - Replace all non alphabetic characters with spaces on
 *                     program cards - MT
 * 04 Nov 25         - Program card defined as part of the processor (makes
 *                     it easier to use in the processor code) - MT
 *                   - Initialize fonts early allowing them to be used when
 *                     creating a program card - MT
 * 08 Nov 25         - Can now read the card colours and text labels from a
 *                     card  file if available (both are optional) - MT
 *                   - Changed  command line parser to use strtol() for any
 *                     numeric arguments and allow breakpoints and traps to
 *                     be set using octal or hexadecimal - MT
 * 11 Nov 25  (0211) - Card text ignores any characters after a '.' - MT
 * 12 Nov 25         - Use a label to display program name - MT
 * 16 Nov 25         - The card reader register addresses are now mapped to
 *                     a buffer register allowing the size of the memory to
 *                     be restored to its original value - MT
 * 19 Nov 25   0.22  - Allocates memory registers dynamically - MT
 *                   - The memory size can be specified on the command line
 *                     (very useful when using modified firmware to provide
 *                     enhanced features) - MT
 *                   - Maintain  compatibility with earlier versions of the
 *                     HP67 emulators and versions without extended  memory
 *                     when loading or saving state - MT
 * 22 Nov 25         - Don't use string concatenation to print the compiler
 *                     version - MT
 * 29 Nov 25         - Don't print compiler version if not defined - MT
 * 06 Dec 25   0.23  - Fixed compilation warnings on VAX/VMS - MT
 * 07 Dec 25         - Use int for boolean types instead of char - MT
 *                   - Fixed regression bug affecting card display - MT
 *                   - Saves program card details when exiting - MT
 * 09 Dec 25         - Use MEMORY_MAX to define the maximum memory size -MT
 * 11 Dec 25         - Implemented /RESET command line option - MT
 * 12 Dec 25         - Attempt to keep the interval between display updates
 *                     constant.  This should ensure that emulation runs at
 *                     the same speed is the same on different systems, but
 *                     this depends on the resolution of the delay routines
 *                     to correctly handle very short delay times - MT
 * 22 Dec 25         - Fixed  bug that caused the geometry to be ignored if
 *                     the value was zero - MT
 * 25 Dec 25         - Tidied up comments and updated build - MT
 * 02 Jan 26  (0228) - Removed redundant calls to XCopyArea() - MT
 *             0.24  - Changed the way the display is drawn. Instead of all
 *                     the display buffer being the same size at the window
 *                     everything is drawn using at it's original size  and
 *                     the  contents of the display buffer are scaled up by
 *                     XScaleArea(). Increased range of values by  '--zoom'
 *                     to take advantage of thsi change- MT
 *
 * To Do             - Parse command line in a separate routine.
 *                   - Must be a better way of handling an arbitrary number
 *                     of switches.
 *                   - Allow VMS users to set breakpoints?
 *                   - Free up allocated memory on exit.
 *                   - Sort out colour mapping.
 *
 */

#define  NAME           "x11-calc"
#define  VERSION        "0.24"
#define  BUILD          "0229"
#define  DATE           "25 Dec 25"
#define  AUTHOR         "MT"

#define  TICKS          48 /* Number of ticks to execute before updating the display */

#include <errno.h>      /* errno */

#include <stdarg.h>     /* vargs(), etc */
#include <string.h>     /* strlen(), etc */
#include <stdio.h>      /* fprintf(), etc */
#include <stdlib.h>     /* getenv(), etc */

#include <ctype.h>      /* isalpha(), etc */

#include <X11/Xlib.h>   /* XOpenDisplay(), True/False etc */
#include <X11/Xutil.h>  /* XSizeHints etc */
#include <X11/keysym.h>
#include <X11/cursorfont.h>

#include "x11-calc-messages.h"
#include "x11-calc-errors.h"

#include "x11-calc-colour.h"
#include "x11-calc-font.h"
#include "x11-calc-button.h"
#include "x11-calc-switch.h"
#include "x11-calc-label.h"
#include "x11-calc-card.h"

#include "x11-calc.h"

#include "x11-calc-digit.h"
#include "x11-calc-display.h"
#include "x11-calc-cpu.h"

#include "x11-scale-area.h"
#include "x11-keyboard.h"

#include "x11-logo.xbm"

#include "gcc-debug.h" /* debug() */
#include "gcc-wait.h"  /* i_wait() */

#if defined(HP67)
#define  TIMEOUT        TICKS * 4
#define  INTERVAL       TICKS / 4
#elif defined(VOYAGER) || defined(SPICE) || defined(CLASSIC)
#define  TIMEOUT        TICKS * 3
#define  INTERVAL       TICKS / 3
#else
#define  TIMEOUT        TICKS * 2
#define  INTERVAL       TICKS / 2
#endif

void v_version(void)  /* Display version information */
{
   fprintf(stdout, "%s: Version %s.%s %s", FILENAME, VERSION, BUILD, COMMIT_ID);
#if defined(__compiler__)
   if (strlen(__compiler__)) fprintf(stdout, " %s", __compiler__);  /* Include compiler version if defined - it could be defined as a null string */
#endif
   if (__DATE__[4] == ' ') fprintf(stdout, " 0"); else fprintf(stdout, " %c", __DATE__[4]);
   fprintf(stdout, "%c %c%c%c %s %s\n", __DATE__[5],
      __DATE__[0], __DATE__[1], __DATE__[2], &__DATE__[9], __TIME__ );
}

void v_set_blank_cursor(Display *x_display, Window x_window, Cursor *x_cursor)
{
   Pixmap x_blank;
   XColor x_Color;
   char h_pixmap_data[1] = {0};  /* An empty pixmap */
   x_blank = XCreateBitmapFromData (x_display, x_window, h_pixmap_data, 1, 1);  /* Create an empty bitmap */
   (*x_cursor) = XCreatePixmapCursor(x_display, x_blank, x_blank, &x_Color, &x_Color, 0, 0);  /* Use the empty pixmap to create a blank cursor */
   XFreePixmap (x_display, x_blank);  /* Free up pixmap */
}

int b_search(int *a, int m, int n) /* Linear search. */
{
   int i_count;
   for (i_count = 0; i_count < n; i_count++)
      if (a[i_count] == m)
         return True;
   return False;
}

int main(int argc, char *argv[])
{
   Display *x_display;                 /* Pointer to display structure */
   Window x_window;                    /* Application window structure */
   Pixmap x_buffer;                    /* Display buffer */
   Cursor x_cursor;                    /* Application cursor */
   Pixmap x_logo;                      /* Application icon */
   XEvent x_event;
   XSizeHints *h_size_hint;
   Atom wm_delete;
   XRectangle o_window_position;
   XRectangle o_window_geometry;

   struct obutton *h_button[BUTTONS];  /* Array to hold pointers to buttons */
   struct obutton *h_pressed = NULL;
   struct odisplay *h_display;         /* Pointer to display structure */
   oprocessor *h_processor;

   char *s_display_name = "";          /* Just use the default display */
   char *s_title = TITLE;              /* Windows title */
   char *s_text;                       /* Temporary pointer */

   float f_scale = 1.0;

   unsigned int i_screen_width = 0;    /* Screen width */
   unsigned int i_screen_height = 0;   /* Screen height */

   int i_window_top = 0;               /* Window top */
   int i_window_left = 0;              /* Window left */
   unsigned int i_window_width = 0;    /* Window width */
   unsigned int i_window_height = 0;   /* Window height */

   unsigned int i_window_border = 4;   /* Window's border width */
   unsigned int i_colour_depth;        /* Window's colour depth */
   int i_screen;                       /* Default screen number */

   long l_start;                       /* Start time */
   long l_elapsed;                     /* Elapsed time */

   int b_trace = False;                /* Trace flag */
   int b_step = False;                 /* Single step flag flag */
   int b_cursor = True;                /* Draw a cursor */
   int b_run = True;                   /* Run flag controls CPU instruction execution in main loop */
   int b_abort = False;                /* Abort flag controls execution of main loop */
   int b_geometry = False;             /* User specified window position */
#if defined (__unix__)
   int b_numlock = False;              /* Use number pad - even if numlock is off */
#endif
#if defined(HP31e) || defined(HP32e) || defined(HP33e) || defined(HP33c) || defined(HP34c) || defined(HP37e) || defined(HP38e) || defined(HP38c)
   int b_euro = False;
#endif
   unsigned int i_memory_size = MEMORY_SIZE;
   int i_breakpoints[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};  /* Array to hold breakpoints */
   int i_offset, i_count, i_index, i_value, i_size;
   int i_delay = -1;                   /* Delay timer - used to work out when switch has been held off for 2 seconds */
   int i_ticks = 0;
   int i_trap = -1;                    /* Trap instruction */

#if defined(CONTINIOUS)
   int b_reset = False;                /* Do not restore state (reset) */
   char *s_pathname = NULL;
#endif

#if defined(SWITCHES)
   struct oswitch *h_switch[SWITCHES];
#endif

#if defined(LABELS)
   struct olabel *h_label[LABELS];
#endif

#if defined (__unix__)
   okeyboard *h_keyboard;
#endif

   if (!(x_display = XOpenDisplay(s_display_name))) v_error (errno, h_err_display, s_display_name);  /* Open the default display */

   if (!(h_normal_font = h_get_font(x_display, s_normal_fonts))) v_error(errno, h_err_font, s_normal_fonts[0]);
   if (!(h_small_font = h_get_font(x_display, s_small_fonts))) v_error(errno, h_err_font, s_small_fonts[0]);
   if (!(h_alternate_font = h_get_font(x_display, s_alternate_fonts))) v_error(errno, h_err_font, s_alternate_fonts[0]);
   if (!(h_large_font = h_get_font(x_display, s_large_fonts))) v_error(errno, h_err_font, s_large_fonts[0]);

#if defined(unix) || defined(__unix__) || defined(__APPLE__)  /* Parse UNIX style command line options */
   b_abort = False;  /* Stop processing command line */
   for (i_count = 1; i_count < argc && (b_abort != True); i_count++)
   {
      if (argv[i_count][0] == '-')
      {
         i_index = 1;
         while (argv[i_count][i_index] != 0)
         {
            switch (argv[i_count][i_index])
            {
            case 'b':  /* Breakpoint */
               if (argv[i_count][i_index + 1] != 0)
                  v_error(EINVAL, h_err_invalid_argument, argv[i_count][i_index + 1]);
               else
                  if (i_count + 1 < argc)
                  {
                     if (strncmp(argv[i_count + 1], "0x", 2) == 0 || strncmp(argv[i_count + 1], "0X", 2) == 0 )  /* Check for a hexadecimal value */
                        i_value = strtol(argv[i_count + 1], &s_text, 16);
                     else
                        i_value = strtol(argv[i_count + 1], &s_text, 8);
                     if (*s_text != '\0')
                        v_error(EINVAL, h_err_invalid_number, argv[i_count + 1]);
                     if ((i_value < 0) || (i_value > ROM_SIZE) || (i_value > 017777) || (errno == ERANGE))  /* Check address range, allow bank - KJC */
                        v_error(EINVAL, h_err_numeric_range, argv[i_count + 1]);
                     else
                     {
                        i_size = sizeof(i_breakpoints) / sizeof(i_breakpoints[0]); /* Find position in the array to store the breakpoint */
                        for (i_offset = 0; i_offset < i_size && i_breakpoints[i_offset] != i_value && i_breakpoints[i_offset] != -1; i_offset++) {} /* Find the  position in the array */
                        if (i_offset < i_size) /* Save it - if there is space! */
                           i_breakpoints[i_offset] = i_value;
                        else
                           v_error(EINVAL, h_err_max_breakpoints);
                        if (i_count + 2 < argc)  /* Remove the parameter from the arguments */
                           for (i_offset = i_count + 1; i_offset < argc - 1; i_offset++)
                              argv[i_offset] = argv[i_offset + 1];
                        argc--;
                     }
                  }
                  else
                     v_error(EINVAL, h_err_missing_argument, argv[i_count]);
               i_index = strlen(argv[i_count]) - 1;
               break;
            case 'i':  /* Trap Instruction */
               if (argv[i_count][i_index + 1] != 0)
                  v_error(EINVAL, h_err_invalid_argument, argv[i_count][i_index + 1]);
               else
                  if (i_count + 1 < argc)
                  {
                     if (strncmp(argv[i_count + 1], "0x", 2) == 0 || strncmp(argv[i_count + 1], "0X", 2) == 0 )  /* Check for a hexadecimal value */
                        i_value = strtol(argv[i_count + 1], &s_text, 16);
                     else
                        i_value = strtol(argv[i_count + 1], &s_text, 8);
                     if (*s_text != '\0')
                        v_error(EINVAL, h_err_invalid_number, argv[i_count + 1]);
                     if ((i_value < 0) || (i_value > 01777) || (errno == ERANGE))  /* Check range */
                        v_error(EINVAL, h_err_numeric_range, argv[i_count + 1]);
                     else
                     {
                        if (i_trap == -1) /* Save it - if not already defined! */
                           i_trap = i_value;
                        else
                           v_error(EINVAL, h_err_duplicate_option, argv[i_count][i_index]);
                        if (i_count + 2 < argc)  /* Remove the parameter from the arguments */
                           for (i_offset = i_count + 1; i_offset < argc - 1; i_offset++)
                              argv[i_offset] = argv[i_offset + 1];
                        argc--;
                     }
                  }
                  else
                     v_error(EINVAL, h_err_missing_argument, argv[i_count]);
               i_index = strlen(argv[i_count]) - 1;
               break;
            case 'm':  /* Memory registers */
               if (argv[i_count][i_index + 1] != 0)
                  v_error(EINVAL, h_err_invalid_argument, argv[i_count][i_index + 1]);
               else
                  if (i_count + 1 < argc)
                  {
                     i_value = strtol(argv[i_count + 1], &s_text, 0);  /* Auto detect base allow octal, decimal or hexadecimal */
                     if (*s_text != '\0')
                        v_error(EINVAL, h_err_invalid_number, argv[i_count + 1]);
                     if ((i_value < 0) || (i_value < MEMORY_SIZE) || (i_value > MEMORY_MAX) || (errno == ERANGE))  /* Check range */
                        v_error(EINVAL, h_err_numeric_range, argv[i_count + 1]);
                     else
                     {
                        i_memory_size = (unsigned int)i_value;
                        if (i_count + 2 < argc)  /* Remove the parameter from the arguments */
                           for (i_offset = i_count + 1; i_offset < argc - 1; i_offset++)
                              argv[i_offset] = argv[i_offset + 1];
                        argc--;
                     }
                  }
                  else
                     v_error(EINVAL, h_err_missing_argument, argv[i_count]);
               i_index = strlen(argv[i_count]) - 1;
               break;
            case 'r':  /* Read ROM contents  */
               if (argv[i_count][i_index + 1] != 0)
                  v_error(EINVAL, h_err_invalid_argument, argv[i_count][i_index + 1]);
               else
                  if (i_count + 1 < argc)
                  {
                     v_read_rom(i_rom, argv[i_count + 1]);  /* Load user specified ROM */
                     if (i_count + 2 < argc)  /* Remove the parameter from the arguments */
                        for (i_offset = i_count + 1; i_offset < argc - 1; i_offset++)
                           argv[i_offset] = argv[i_offset + 1];
                     argc--;
                  }
                  else
                     v_error(EINVAL, h_err_missing_argument, argv[i_count]);
               i_index = strlen(argv[i_count]) - 1;
               break;

#if defined(HP31e) || defined(HP32e) || defined(HP33e) || defined(HP33c) || defined(HP34c) || defined(HP37e) || defined(HP38e) || defined(HP38c)
            case 'c':  /* Use european display format */
               b_euro = True;
               break;
#endif
            case 's':  /* Start in single step mode */
               b_trace = b_step = True;
               break;
            case 't':  /* Enable tracing */
               b_trace = True;
               break;
            case '-':  /* '--' terminates command line processing */
               i_index = strlen(argv[i_count]);
               if (i_index == 2)
                 b_abort = True;  /* '--' terminates command line processing */
               else
                  if (!strncmp(argv[i_count], "--cursor", i_index))
                     b_cursor = True;  /* Draw cursor */
                  else if (!strncmp(argv[i_count], "--no-cursor", i_index))
                     b_cursor = False;  /* Don't draw a cursor - unless drawn by the window manager */
#if defined(CONTINIOUS)
                  else if (!strncmp(argv[i_count], "--reset", i_index))
                     b_reset = True;  /* Reset state */
#endif
#if defined(SPICE)
                  else if (!strncmp(argv[i_count], "--comma", i_index))
                     b_euro = True; /* Use european display format */
                  else if (!strncmp(argv[i_count], "--no-comma", i_index))
                     b_euro = False;  /* Don't use european display format */
#endif
#if defined (__unix__)
                  else if (!strncmp(argv[i_count], "--numlock", i_index))
                     b_numlock = True;  /* Use number pad - even if numlock is off */
#endif
                  else if (!strncmp(argv[i_count], "--zoom", i_index))
                  {
                     if (i_count + 1 < argc)
                     {
                        i_value = strtol(argv[i_count + 1], &s_text, 10);
                        if (*s_text != '\0')
                           v_error(EINVAL, h_err_invalid_number, argv[i_count + 1]);
                        if ((i_value < 0) || (i_value > 16) || (errno == ERANGE))  /* Check range */
                           v_error(EINVAL, h_err_numeric_range, argv[i_count + 1]);
                        else
                        {
                           if (i_count + 2 < argc)  /* Remove the parameter from the arguments */
                              for (i_offset = i_count + 1; i_offset < argc - 1; i_offset++)
                                 argv[i_offset] = argv[i_offset + 1];
                           argc--;
                        }
                        f_scale = 1 + (0.125 * i_value);
                     }
                     else
                        v_error(EINVAL, h_err_missing_argument, argv[i_count]);
                  }
                  else if (!strncmp(argv[i_count], "--geometry=", 11)) /* Just check the first 11 characters match */
                  {
                     char *c_geometry = argv[i_count] + 11;
                     char *c_char;

                     for (c_char = c_geometry; *c_char; c_char++) /* Convert to lowercase before parsing */
                        *c_char = (char)tolower((unsigned char)*c_char);
                     if (XParseGeometry(c_geometry,  &i_window_left, &i_window_top, &i_window_width, &i_window_height))
                     {
                        if (i_count + 1 < argc)  /* Remove the parameter from the arguments */
                           for (int i_offset = i_count + 1; i_offset < argc - 1; i_offset++)
                              argv[i_offset] = argv[i_offset + 1];
                        argc--;
                        b_geometry = True;
                     }
                     else
                        v_error(EINVAL, h_err_geometry, c_geometry);
                  }
                  else if (!strncmp(argv[i_count], "--geometry", i_index))
                  {
                     if (i_count + 1 < argc)
                     {
                        char *c_geometry = argv[i_count + 1];
                        char *c_char;

                        for (c_char = c_geometry; *c_char; c_char++) /* Convert to lowercase before parsing */
                           *c_char = (char)tolower((unsigned char)*c_char);
                        if (XParseGeometry(c_geometry,  &i_window_left, &i_window_top, &i_window_width, &i_window_height))
                        {
                           if (i_count + 2 < argc)  /* Remove the parameter from the arguments */
                              for (int i_offset = i_count + 1; i_offset < argc - 1; i_offset++)
                                 argv[i_offset] = argv[i_offset + 1];
                           argc--;
                           b_geometry = True;
                        }
                        else
                           v_error(EINVAL, h_err_geometry, c_geometry);
                     }
                     else
                        v_error(EINVAL, h_err_missing_argument, argv[i_count]);
                  }
                  else if (!strncmp(argv[i_count], "--version", i_index))
                  {
                     v_version();  /* Display version information */
                     v_usage(stdout, h_msg_licence, &__DATE__[7], AUTHOR);
                     exit(EXIT_SUCCESS);
                  }
                  else if (!strncmp(argv[i_count], "--help", i_index))
                  {
                     v_usage(stdout, h_msg_usage, FILENAME);
                     exit(EXIT_SUCCESS);
                  }
                  else  /* If we get here then the we have an invalid long option */
                     v_error(EINVAL, h_err_unrecognised_option, argv[i_count]);
               i_index--;  /* Leave index pointing at end of string (so argv[i_count][i_index] = 0) */
               break;
            default:  /* If we get here the single letter option is unknown */
               v_error(EINVAL, h_err_invalid_option, argv[i_count][i_index]);
            }
            i_index++;  /* Parse next letter in option */
         }
         if (argv[i_count][1] != 0)
         {
            for (i_offset = i_count; i_offset < argc - 1; i_offset++)
               argv[i_offset] = argv[i_offset + 1];
            argc--; i_count--;
         }
      }
   }
#else  /* Parse DEC style command line options */
   for (i_count = 1; i_count < argc; i_count++)
   {
      if (argv[i_count][0] == '/')
      {
         for (i_index = 0; argv[i_count][i_index]; i_index++)  /* Convert option to uppercase */
            if (argv[i_count][i_index] >= 'a' && argv[i_count][i_index] <= 'z')
               argv[i_count][i_index] = argv[i_count][i_index] - 32;  /* TO DO - Assumes 8-bit ASCII encoding */
         if (!strncmp(argv[i_count], "/STEP", i_index))
            b_trace = True;  /* Start in single step mode */
         else if (!strncmp(argv[i_count], "/CURSOR", i_index))
            b_cursor = True;  /* Enable tracing */
         else if (!strncmp(argv[i_count], "/NOCURSOR", i_index))
            b_trace = False;  /* Enable tracing */
         else if (!strncmp(argv[i_count], "/TRACE", i_index))
            b_trace = True;  /* Enable tracing */
#if defined(CONTINIOUS)
         else if (!strncmp(argv[i_count], "/RESET", i_index))
            b_reset = True;  /* Enable tracing */
#endif
         else if (!strncmp(argv[i_count], "/ROM", i_index))
         {
            if (i_count + 1 < argc)
            {
               v_read_rom(i_rom, argv[i_count + 1]);  /* Load user specified ROM */
               if (i_count + 2 < argc)  /* Remove the parameter from the arguments */
                  for (i_offset = i_count + 1; i_offset < argc - 1; i_offset++)
                     argv[i_offset] = argv[i_offset + 1];
               argc--;
            }
            else
               v_error(EINVAL, h_err_missing_argument, argv[i_count]);
         }
         else if (!strncmp(argv[i_count], "/VERSION", i_index))
         {
            v_version();  /* Display version information */
            v_usage(stdout, h_msg_licence, &__DATE__[7], AUTHOR);
            exit(EXIT_SUCCESS);
         }
         else if ((!strncmp(argv[i_count], "/HELP", i_index)) | (!strncmp(argv[i_count], "/?", i_index)))
         {
            v_usage(stdout, h_msg_usage, FILENAME);
            exit(EXIT_SUCCESS);
         }
         else /* If we get here then the we have an invalid option */
            v_error(EINVAL, h_err_invalid_option, argv[i_count]);
         if (argv[i_count][1] != 0)
         {
            for (i_index = i_count; i_index < argc - 1; i_index++)
               argv[i_index] = argv[i_index + 1];
            argc--; i_count--;
         }
      }
   }
#endif

#if defined(CONTINIOUS)
   if (argc > 2) v_error(EINVAL, h_err_invalid_operand);  /* There should never be more than one command lime parameter */
   if (argc > 1) s_pathname = argv[1];  /* Set path name if a parameter was passed and continuous memory is enabled */
#else
   if (argc > 1) v_error(EINVAL, h_err_invalid_operand);  /* There shouldn't any command line parameters */
#endif

   v_version();
   fprintf(stdout, "ROM Size: %4u words \n", ROM_SIZE);
   i_wait(200);  /* Sleep for 200 milliseconds to 'debounce' keyboard! */

   h_processor = h_processor_create(i_rom, i_memory_size);

   i_count = ROM_SIZE;
   while ((i_count > 0) && (i_rom[--i_count] == 0));  /* Check that the ROM isn't empty */
   if (i_count == 0) v_error (ENODATA, h_err_ROM);

   i_screen = DefaultScreen(x_display);  /* Get the default screen for our X server */
   i_screen_width = DisplayWidth(x_display, i_screen);
   i_screen_height = DisplayHeight(x_display, i_screen);

   o_window_position.width = (int)(WIDTH * f_scale);  /* Window width in pixels */
   o_window_position.height = (int)(HEIGHT * f_scale);  /* Window height in pixels */
   /* If the window position wasn't specified just centre it on the screen - ignored by most window managers but useful in kiosk mode */
   if (i_window_left || b_geometry )  /* Fixed bug that caused position to be ignored if value was zero */
      o_window_position.x = i_window_left;  /* User specified position */
   else
      o_window_position.x = (i_screen_width - o_window_position.width) / 2;  /* Centre window on screen  */
   if (i_window_top || b_geometry)  /* Fixed bug that caused position to be ignored if value was zero */
      o_window_position.y = i_window_top;  /* User specified position */
   else
      o_window_position.y = (i_screen_height - o_window_position.height) / 2;  /* Centre window on screen  */
   o_window_geometry = o_window_position;  /* Save window position */

   x_window = XCreateSimpleWindow(x_display, /* Create the application window, as a child of the root window */
      XDefaultRootWindow(x_display),
      o_window_position.x, o_window_position.y, /* Windows position */
      o_window_position.width, /* Window width */
      o_window_position.height, /* Window height */
      i_window_border, /* Border width - ignored ? */
      BlackPixel(x_display, i_screen), /* Border colour - ignored ? */
      WhitePixel(x_display, i_screen)); /* Background colour */


   if (XGetGeometry(x_display, x_window,    /* Get window geometry */
         &RootWindow(x_display, i_screen),
         &i_window_left, &i_window_top,
         &i_window_width,
         &i_window_height,
         &i_window_border,
         &i_colour_depth) == False)
      v_error(errno, h_err_display_properties);

   if (i_colour_depth != COLOUR_DEPTH) v_error(errno, h_err_display_colour, COLOUR_DEPTH);  /* Check colour depth */

   if  (!(x_logo = XCreateBitmapFromData(x_display, x_window, (char*) logo_bits, logo_width, logo_height))) v_error(errno, h_err_pixmap);  /* Check colour depth */

   if (b_cursor)
      x_cursor = XCreateFontCursor(x_display, XC_arrow);  /* Create a 'default' cursor */
   else
      v_set_blank_cursor(x_display, x_window, &x_cursor);  /* Get a blank cursor */
   XDefineCursor(x_display, x_window, x_cursor);  /* Define the desired X cursor */

   x_buffer = XCreatePixmap(x_display, x_window, o_window_position.width, o_window_position.height, COLOUR_DEPTH);
   XSetForeground(x_display, DefaultGC(x_display, i_screen), BACKGROUND); /* Set background colour */
   XFillRectangle(x_display, x_buffer, DefaultGC(x_display, i_screen), 0, 0, o_window_position.width, o_window_position.height);

   h_display = h_display_create(0, BEZEL_LEFT, BEZEL_TOP, BEZEL_WIDTH, BEZEL_HEIGHT,
      DISPLAY_LEFT, DISPLAY_TOP, DISPLAY_WIDTH, DISPLAY_HEIGHT, DIGIT_COLOUR, DIGIT_BACKGROUND,
      DISPLAY_BACKGROUND, BEZEL_COLOUR);  /* Create display */

#if defined(HP31e) || defined(HP32e) || defined(HP33e) || defined(HP33c) || defined(HP34c) || defined(HP37e) || defined(HP38e) || defined(HP38c)
   h_display->euro = b_euro;
#endif
   v_init_buttons(h_button); /* Create buttons */

#if defined(SWITCHES)
   v_init_switches(h_switch);
#endif

#if defined(LABELS)
   v_init_labels(h_label);  /* Passes the address of the array */
#endif

   /* Resize application window */
   o_window_position.x = o_window_geometry.x;
   o_window_position.y = o_window_geometry.y;
   o_window_position.width = o_window_geometry.width;
   o_window_position.height = o_window_geometry.height;

   h_size_hint = XAllocSizeHints();  /* Set application window size */
   h_size_hint->flags = PMinSize | PMaxSize;
   if (b_geometry)  /* Update flags if geometry specified on command line otherwise let window manager position window */
   {
      h_size_hint->flags = h_size_hint->flags | USPosition;  /* Update flags */
      h_size_hint->x = o_window_position.x;
      h_size_hint->y = o_window_position.y;
   }
   h_size_hint->width = o_window_position.width;
   h_size_hint->height = o_window_position.height;
   h_size_hint->min_width = o_window_position.width;
   h_size_hint->min_height = o_window_position.height;
   h_size_hint->max_width = o_window_position.width;
   h_size_hint->max_height = o_window_position.height;

   XSetStandardProperties(x_display, x_window, s_title, s_title, x_logo, argv, argc, h_size_hint);  /* Set the window title and icon */

#if defined (__unix__)
   h_keyboard = h_keyboard_create(x_display);  /* Only works with Linux */
#endif

   XSelectInput(x_display, x_window, FocusChangeMask | ExposureMask | /* Select kind of events we are interested in */
      KeyPressMask | KeyReleaseMask | ButtonPressMask |
      ButtonReleaseMask | StructureNotifyMask | SubstructureNotifyMask);

   wm_delete = XInternAtom(x_display, "WM_DELETE_WINDOW", False);  /* Create a windows delete message 'atom'. */
   XSetWMProtocols(x_display, x_window, &wm_delete, 1);  /* Tell the display to pass wm_delete messages to the application window */

   XMapWindow(x_display, x_window);  /* Show window on display */
   XRaiseWindow(x_display, x_window);  /* Raise window - ensures expose event is raised? */

   v_processor_reset(h_processor);
   h_processor->trace = b_trace;
   h_processor->step = b_step;

#if defined(CONTINIOUS)
   if (!b_reset)  /* Don't restore state if user selects reset */
   {
      if (s_pathname == NULL)
         v_restore_state(h_processor);
      else
         v_read_state(h_processor, s_pathname);  /* Load user specified settings */
   }
#endif

#if defined(SWITCHES)
   if (h_switch[0] != NULL) h_processor->enabled = h_switch[0]->state; /* Allow switches to be undefined if not used */
   if (SWITCHES == 2) /** To Do - Must be a better way of handling an arbitrary number of switches */
   {
#if defined(HP10)
      switch (h_switch[1]->state)
      {
         case 0:
            h_processor->print = MANUAL;
            break;
         case 3:
         case 1:
            h_processor->print = NORMAL;
            break;
         case 2:
            h_processor->print = TRACE;
            break;
      }
#elif defined(HP55)
      switch (h_switch[1]->state)
      {
         case 0:
            h_processor->timer = True;
            h_processor->mode = False;
            break;
         case 3:
         case 1:
            h_processor->timer = False;
            h_processor->mode = True;
            break;
         case 2:
            h_processor->timer = False;
            h_processor->mode = False;
            break;
      }
#else
      if (h_switch[1] != NULL)
         h_processor->mode = h_switch[1]->state;
#endif
   }
#endif
   b_abort = False;
   l_start = l_now();  /* Get current time */
   i_ticks = 0;
   while (!b_abort)  /* Main program event loop */
   {
      i_ticks--;
      if (i_ticks < 0)
      {
         i_display_update(h_display, h_processor);
#if defined(HP67)
         if (h_processor->crc[FUNCTION])
         {
            int i_count = 0;
            h_processor->card->state = False;  /* Hide the program card */
            for (i_count = 0; i_count < LABELS; i_count++)  /* Update label state */
               h_label[i_count]->state = h_processor->crc[FUNCTION];
            for (i_count = 0; i_count < LABELS; i_count++)  /* Draw labels */
               i_label_draw(x_display, x_buffer, i_screen, h_label[i_count]);
         }
         else
         {
            h_processor->card->state = True;  /* Display the program card */
            i_card_draw(x_display, x_buffer, i_screen, h_processor->card);  /* Update display */
         }
#endif
         i_display_draw(x_display, x_buffer, i_screen, h_display);  /* Redraw display */
         if (f_scale > 1.0)
            XScaleArea(x_display, x_buffer, x_window, DefaultGC(x_display, i_screen), 0, 0, WIDTH, HEIGHT, 0, 0, o_window_position.width, o_window_position.height);
         else
            XCopyArea(x_display, x_buffer, x_window, DefaultGC(x_display, i_screen), 0, 0, o_window_position.width, o_window_position.height, 0, 0);
         l_elapsed = (l_now() - l_start + 1);
         i_wait((INTERVAL - l_elapsed));  /* Sleep */
         l_start = l_now();  /* Get current time */
         i_ticks = TICKS;  /* Reset timer */
         if (--i_delay == 0) b_abort = True;  /* Delay timer reached zero so exit */
      }
      if ( (b_search(i_breakpoints, (h_processor->pc & 0x1fff), sizeof(i_breakpoints) / sizeof(i_breakpoints[0]))) || (h_processor->rom[h_processor->pc] == i_trap))  /* Check for Breakpoint or Instruction Trap */
      {
         if (!h_processor->trace || !h_processor->step) fprintf(stderr, "** break **\n");
         h_processor->trace = h_processor->step = True;
      }
      if (b_run) v_processor_tick(h_processor);
      if (h_processor->step) b_run = False;

      while (XPending(x_display))
      {
         XNextEvent(x_display, &x_event);
         switch (x_event.type)
         {
         case FocusOut:
            if (!(h_pressed == NULL))
            {
               h_pressed->state = False;
               i_button_draw(x_display, x_buffer, i_screen, h_pressed);
               h_processor->keypressed = False;  /* Don't clear the status bit here!! */
            }
            break;
#if defined (__unix__)
         case KeyPress :
            h_key_pressed(h_keyboard, x_display, x_event.xkey.keycode, x_event.xkey.state, b_numlock);  /* Attempts to translate a key code into a character */
            if (h_keyboard->key == (XK_BackSpace & 0x1f)) h_keyboard->key = XK_Escape & 0x1f;  /* Map backspace to escape */
            if (h_keyboard->key == (XK_Z & 0x1f))  /* Ctrl-Z to exit */
               b_abort = True;
            else if (h_keyboard->key == (XK_T & 0x1f))  /* Ctrl-T to toggle tracing */
               h_processor->trace = !h_processor->trace;
            else if (h_keyboard->key == (XK_S & 0x1f))  /* Ctrl-S or space to single step */
               h_processor->trace = h_processor->step = b_run = True;
            else if (h_keyboard->key == (XK_R & 0x1f))  /* Ctrl-R to display internal CPU registers */
               v_fprint_registers(stdout, h_processor);
            else if (h_keyboard->key == (XK_Q & 0x1f))  /* Ctrl-Q to resume */
               h_processor->step = !(b_run  = True);
            else if (h_keyboard->key == (XK_C & 0x1f))  /* Ctrl-C to reset */
            {
               v_processor_reset(h_processor);
#if defined(CONTINIOUS)
               if (s_pathname == NULL)
                  v_restore_state(h_processor);  /* Load current saved settings */
               else
                  v_read_state(h_processor, s_pathname);  /* Load user specified settings */
#endif
               b_run = True;
            }
            else  /* Check for matching button */
            {
               int i_count;
               for (i_count = 0; i_count < BUTTONS; i_count++)
               {
                  h_pressed = h_button_key_pressed(h_button[i_count], h_keyboard->key);
                  if (h_pressed != NULL)
                  {
                     h_pressed->state = True;
                     i_button_draw(x_display, x_buffer, i_screen, h_pressed);
                     h_processor->code = h_pressed->index;
                     h_processor->keypressed = True;
#if !defined(SWITCHES)
                     h_processor->enabled = True;  /* Any key press will wake up the processor */
                     h_processor->sleep = False;
#endif
                     break;
                  }
               }
            }
            break;
         case KeyRelease :
            h_key_released(h_keyboard, x_display, x_event.xkey.keycode, x_event.xkey.state, b_numlock);
            if (h_keyboard->key == (XK_BackSpace & 0x1f)) h_keyboard->key = XK_Escape & 0x1f;  /* Map backspace to escape */
            if (h_pressed != NULL)
            {
               if (h_keyboard->key == h_pressed->key)
               {
                  h_pressed->state = False;
                  i_button_draw(x_display, x_buffer, i_screen, h_pressed);
                  h_processor->keypressed = False;  /* Don't clear the status bit here!! */
               }
            }
            break;
#endif
         case ButtonPress :
            /** debug(printf("Mouse button [%d] pressed.\n", x_event.xbutton.button)); */

            x_event.xbutton.x = (int)(x_event.xbutton.x / f_scale + 0.5f);  /* Scale the coordinate's */
            x_event.xbutton.y = (int)(x_event.xbutton.y / f_scale + 0.5f);

            if (x_event.xbutton.button == 1)
            {
               int i_count;
               for (i_count = 0; i_count < sizeof(h_button) / sizeof(*h_button); i_count++)
               {
                  h_pressed = h_button_pressed(h_button[i_count], x_event.xbutton.x, x_event.xbutton.y);
                  if (!(h_pressed == NULL))
                  {
                     h_pressed->state = True;
                     i_button_draw(x_display, x_buffer, i_screen, h_pressed);
                     h_processor->code = h_pressed->index;
                     h_processor->keypressed = True;
#if !defined(SWITCHES)
                     h_processor->enabled = True;  /* Any key press will wake up the processor */
                     h_processor->sleep = False;
#endif
                     break;
                  }
               }
#if defined(SWITCHES)
               if (h_pressed == NULL)  /* It wasn't a button that was pressed check the switches */
               {
                  if (!(h_switch_pressed(h_switch[0], x_event.xbutton.x, x_event.xbutton.y) == NULL))
                  {
                     h_switch[0]->state = !(h_switch[0]->state);  /* Toggle switch */
                     i_switch_draw(x_display, x_buffer, i_screen, h_switch[0]);
                     if (h_switch[0]->state)
                     {
                        v_processor_reset(h_processor);  /* Reset the processor */
#if defined(CONTINIOUS)
                        v_restore_state(h_processor);  /* Restore saved settings */
#endif
                     }
                     else
                     {
                        h_processor->enabled = False;  /* Disable the processor */
#if defined(CONTINIOUS)
                        v_backup_state(h_processor);  /* Save current settings */
#endif
                        i_delay = TIMEOUT;  /* Set count down */
                     }
                  }
                  if (SWITCHES == 2) /** To Do - Must be a better way of handling an arbitrary number of switches */
                     if (h_switch_pressed(h_switch[1], x_event.xbutton.x, x_event.xbutton.y) != NULL)
                     {
#if defined(HP10)
                        switch(i_switch_click(h_switch[1]))
                        {
                        case 0:
                           h_processor->print = MANUAL;
                           break;
                        case 3:
                        case 1:
                           h_processor->print = NORMAL;
                           break;
                        case 2:
                           h_processor->print = TRACE;
                           break;
                        }
#elif defined(HP55)
                        switch(i_switch_click(h_switch[1]))
                        {
                           case 0:
                              h_processor->timer = True;
                              h_processor->mode = False;
                              break;
                           case 3:
                           case 1:
                              h_processor->timer = False;
                              h_processor->mode = True;
                              break;
                           case 2:
                              h_processor->timer = False;f_scale
                              h_processor->mode = False;
                              break;
                        }
#else
                        h_processor->mode = i_switch_click(h_switch[1]);  /* Update prgm/run switch */
#endif
                        i_switch_draw(x_display, x_buffer, i_screen, h_switch[1]);
                     }
               }
#endif
            }
            break;
         case ButtonRelease :

            x_event.xbutton.x = (int)(x_event.xbutton.x / f_scale + 0.5f);  /* Scale the coordinate's */
            x_event.xbutton.y = (int)(x_event.xbutton.y / f_scale + 0.5f);

            if (x_event.xbutton.button == 1)
            {
               if (!(h_pressed == NULL))
               {
                  h_pressed->state = False;
                  i_button_draw(x_display, x_buffer, i_screen, h_pressed);
                  h_processor->keypressed = False;  /* Don't clear the status bit here!! */
               }
#if defined(SWITCHES)
               if (h_pressed == NULL)  /* It wasn't a button that was released so check the switches */
                  if (!(h_switch_pressed(h_switch[0], x_event.xbutton.x, x_event.xbutton.y) == NULL))
                     i_delay = -1;  /* Decrement delay timer if switch is held down */
#endif
            }
#if defined(__GTK__)  /* Don't attempt to load/save if GTK isn't available */
#if defined(CONTINIOUS) || defined(HP67)
            if (x_event.xbutton.button == 3)  /* Right mouse button */
#if defined(HP67)
            {
               h_processor->crc[CARD] = True;               /* Clear at motor start & stop - KJC */
               h_processor->flags[DISPLAY_ENABLE] = False;  /* Disable display while reading card - KJC */
            }
#else
            {
               if (h_processor->mode)
                  v_load_state(h_processor);  /* Load saved state (resets calculator unless cancelled) */
               else
                  v_save_state(h_processor);  /* Save current state */
               b_run = True;
               while (XPending(x_display))
                  XNextEvent(x_display, &x_event);  /* Clear the event queue */
            }
#endif
#endif
#endif
            break;
         case Expose :  /* Draw or redraw the window */
            {
               int i_count;
               i_display_draw(x_display, x_buffer, i_screen, h_display);/* Draw display */
#if defined(LABELS)
               for (i_count = 0; i_count < LABELS; i_count++)  /* Draw labels */
                  i_label_draw(x_display, x_buffer, i_screen, h_label[i_count]);
#endif
#if defined(SWITCHES)
               for (i_count = 0; i_count < SWITCHES; i_count++)  /* Draw switches */
                  i_switch_draw(x_display, x_buffer, i_screen, h_switch[i_count]);
#endif
               for (i_count = 0; i_count < BUTTONS; i_count++)  /* Draw buttons */
                  i_button_draw(x_display, x_buffer, i_screen, h_button[i_count]);
            }
            break;
         case ClientMessage :  /* Message from window manager */
            if (x_event.xclient.data.l[0] == wm_delete) b_abort = True;f_scale
            break;
         }
      }
   }

#if defined(CONTINIOUS)
   v_backup_state(h_processor);  /* Save state */
#endif

   XDestroyWindow(x_display, x_window);  /* Close connection to server */
   XCloseDisplay(x_display);

   exit(EXIT_SUCCESS);
}
