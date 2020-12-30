/*
 *
 * x11-calc-33.h
 *
 * Copyright(C) 2018   MT
 * 
 * Model specific functions
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
 * 10 Mar 14         - Changed indexes to BCD hex values - MT
 * 10 Dec 18         - Alternate  function key now LIGHT_BLUE, allowing  it
 *                     to be a different color to the alternate text - MT
 * 31 Aug 20         - Made  version(), about(), and error() model specific 
 *                     so that the name reflects the model number - MT
 *                   - Modified version() to display a leading zero in  the
 *                     date instead of a space - MT
 *                      
 * TO DO :           - 
 */
 
#define NAME         "x11-rpncalc33"
#define VERSION      "0.1"
#define BUILD        "0034"
#define AUTHOR       "MT"
 
#define DEBUG 1

#include <X11/Xlib.h>      /* XOpenDisplay(), etc. */
#include <X11/Xutil.h>     /* XSizeHints etc. */
 
#include <stdarg.h>        /* strlen(), etc. */
#include <stdio.h>         /* fprintf(), etc. */
#include <stdlib.h>        /* getenv(), etc. */
 
#include "x11-debug.h"
 
#include "x11-calc-colour.h"
#include "x11-calc-font.h"
#include "x11-calc-button.h"
 
#include "x11-calc-33.h"   /* Model specific functions */
 
void v_version() { /* Display version information */
   fprintf(stderr, "%s: Version %s", NAME, VERSION);
   if (__DATE__[4] == ' ') fprintf(stderr, " (0"); else fprintf(stderr, " (%c", __DATE__[4]);
   fprintf(stderr, "%c %c%c%c %s %s)", __DATE__[5],
      __DATE__[0], __DATE__[1], __DATE__[2], __DATE__ +9, __TIME__ );
   fprintf(stderr, " (Build %s)", BUILD );
   fprintf(stderr,"\n");
   fprintf(stderr, "Copyright(C) %s %s\n", __DATE__ +7, AUTHOR);
   fprintf(stderr, "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
   fprintf(stderr, "This is free software: you are free to change and redistribute it.\n");
   fprintf(stderr, "There is NO WARRANTY, to the extent permitted by law.\n");
   exit(0);
} 
 
#ifdef vms /* Use DEC command line options */
void v_about() { /* Display help text */
   fprintf(stdout, "Usage: %s [OPTION]... [FILE]...\n", NAME);
   fprintf(stdout, "Concatenate FILE(s)to standard output.\n\n");
   fprintf(stdout, "  /delay                   delay 8ms between each byte\n");
   fprintf(stdout, "  /header                  display filenames\n");
   fprintf(stdout, "  /number                  number all output lines \n");
   fprintf(stdout, "  /restart                 line numbers start at zero, implies -n\n");
   fprintf(stdout, "  /skip                    skip over repeated blank lines\n");
   fprintf(stdout, "  /version                 output version information and exit\n\n");
   fprintf(stdout, "  /?, /help                display this help and exit\n");
   exit(0);
}
#else
void v_about() { /* Display help text */
   fprintf(stdout, "Usage: %s [OPTION]... [FILE]...\n", NAME);
   fprintf(stdout, "Concatenate FILE(s)to standard output.\n\n");
   fprintf(stdout, "  -d, --delay              delay 8ms between each byte\n");
   fprintf(stdout, "  -f, --filenames          display filenames\n");
   fprintf(stdout, "  -n, --number             number all output lines \n");
   fprintf(stdout, "  -r, --restart            line numbers start at zero, implies -n\n");
   fprintf(stdout, "  -s, --squeeze-blank      suppress repeated blank lines\n");
   fprintf(stdout, "  -?, --help               display this help and exit\n");
   fprintf(stdout, "      --version            output version information and exit\n\n");
   fprintf(stdout, "With no FILE, or when FILE is -, read standard input.\n");
   exit(0);
}
#endif
 
void v_error(const char *s_fmt, ...) { /* Print formatted error message */
   va_list t_args;
   va_start(t_args, s_fmt);
   fprintf(stderr, "%s : ", NAME);
   vfprintf(stderr, s_fmt, t_args);
   va_end(t_args);
}
 
void v_init_keypad(o_button* h_button[]){ /* Initialize the keypad buttons. */

   /* Define top row of keys. */ 
   h_button[0] = h_button_create(0x11, "SST", "FIX", "BST", h_normal_font, h_small_font, h_alternate_font, 12, 85, 33, 30, False, BLACK);
   h_button[1] = h_button_create(0x12, "GSB", "SCI", "RTN", h_normal_font, h_small_font, h_alternate_font, 48, 85, 33, 30, False, BLACK);
   h_button[2] = h_button_create(0x13, "GTO", "ENG", "NOP", h_normal_font, h_small_font, h_alternate_font, 84, 85, 33, 30, False, BLACK);
   h_button[3] = h_button_create(0x14, "f", "", "", h_normal_font, h_small_font, h_alternate_font, 120, 85, 33, 30, False, YELLOW);
   h_button[4] = h_button_create(0x15, "g", "", "", h_normal_font, h_small_font, h_alternate_font, 156, 85, 33, 30, False, LIGHT_BLUE);
   
   /* Define second row of keys. */ 
   h_button[5] = h_button_create(0x21, "X-Y", "x", "DEG", h_normal_font, h_small_font, h_alternate_font, 12, 128, 33, 30, False, BLACK);
   h_button[6] = h_button_create(0x22, "R", "y", "RAD", h_normal_font, h_small_font, h_alternate_font, 48, 128, 33, 30, False, BLACK);
   h_button[7] = h_button_create(0x23, "STO", "r", "GRD", h_normal_font, h_small_font, h_alternate_font, 84, 128, 33, 30, False, BLACK);
   h_button[8] = h_button_create(0x24, "RCL", "L.R", "x", h_normal_font, h_small_font, h_alternate_font, 120, 128, 33, 30, False, BLACK);
   h_button[9] = h_button_create(0x25, "E+", "E-", "s", h_normal_font, h_small_font, h_alternate_font, 156, 128, 33, 30, False, BLACK); 

   /* Define third row of keys. */
   h_button[10] = h_button_create(0x31, "ENTER", "PREFIX", "MANT", h_normal_font, h_small_font, h_alternate_font, 12, 171, 69, 30, False, BLACK);
   h_button[11] = h_button_create(0x32, "CHS", "PRGM", "INT", h_normal_font, h_small_font, h_alternate_font, 84, 171, 33, 30, False, BLACK);
   h_button[12] = h_button_create(0x33, "EEX", "REG", "FRAC", h_normal_font, h_small_font, h_alternate_font, 120, 171, 33, 30, False, BLACK);
   h_button[13] = h_button_create(0x34, "CLX", "STK", "ABS", h_normal_font, h_small_font, h_alternate_font, 156, 171, 33, 30, False, BLACK);

   /* Define fourth row of keys. */
   h_button[14] = h_button_create(0x41, "-", "X\x1a\x59", "X<0", h_large_font, h_small_font, h_alternate_font, 12, 214, 33, 30, False, LIGHT_GRAY);
   h_button[15] = h_button_create(0x7, "7", "SIN", "SIN-\xb9", h_large_font, h_small_font, h_alternate_font, 52, 214, 41, 30, False, LIGHT_GRAY);
   h_button[16] = h_button_create(0x8, "8", "COS", "COS-\xb9", h_large_font, h_small_font, h_alternate_font, 100, 214, 41, 30, False, LIGHT_GRAY);
   h_button[17] = h_button_create(0x9, "9", "TAN", "TAN-\xb9", h_large_font, h_small_font, h_alternate_font, 148, 214, 41, 30, False, LIGHT_GRAY);

   /* Define fifth row of keys. */
   h_button[18] = h_button_create(0x51, "+", "X>Y", "X>0", h_large_font, h_small_font, h_alternate_font, 12, 257, 33, 30, False, LIGHT_GRAY);
   h_button[19] = h_button_create(0x4, "4", "-R", "-P", h_large_font, h_small_font, h_alternate_font, 52, 257, 41, 30, False, LIGHT_GRAY);
   h_button[20] = h_button_create(0x5, "5", "-RAD", "-DEG", h_large_font, h_small_font, h_alternate_font, 100, 257, 41, 30, False, LIGHT_GRAY);
   h_button[21] = h_button_create(0x6, "6", "-H.MS", "-H", h_large_font, h_small_font, h_alternate_font, 148, 257, 41, 30, False, LIGHT_GRAY);

   /* Define sixth row of keys. */
   h_button[22] = h_button_create(0x61, "\xd7", "X\x1d\x59", "X\x1d\x30", h_large_font, h_small_font, h_alternate_font, 12, 300, 33, 30, False, LIGHT_GRAY);
   h_button[23] = h_button_create(0x1, "1", "LN", "eX", h_large_font, h_small_font, h_alternate_font, 52, 300, 41, 30, False, LIGHT_GRAY);
   h_button[24] = h_button_create(0x2, "2", "LOG", "10x", h_large_font, h_small_font, h_alternate_font, 100, 300, 41, 30, False, LIGHT_GRAY);
   h_button[25] = h_button_create(0x3, "3", "yX", "1/x", h_large_font, h_small_font, h_alternate_font, 148, 300, 41, 30, False, LIGHT_GRAY);

   /* Define bottom row of keys. */
   h_button[26] = h_button_create(0x71, "\xf7", "X=Y", "X=0", h_large_font, h_small_font, h_alternate_font, 12, 343, 33, 30, False, LIGHT_GRAY);
   h_button[27] = h_button_create(0x0, "0", "√\xaf", "x\xb2", h_large_font, h_small_font, h_alternate_font, 52, 343, 41, 30, False, LIGHT_GRAY);
   h_button[28] = h_button_create(0x73, ".", "LASTx", "\x1c", h_large_font, h_small_font, h_alternate_font, 100, 343, 41, 30, False, LIGHT_GRAY);
   h_button[29] = h_button_create(0x74, "R/S", "PAUSE", "%", h_normal_font, h_small_font, h_alternate_font, 148, 343, 41, 30, False, LIGHT_GRAY);

}
