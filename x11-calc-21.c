/*
 * x11-calc-21.c - RPN (Reverse Polish) calculator simulator. 
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
 * 09 Jun 14   0.1   - Initial version - MT
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
 
#define NAME           "x11-rpncalc21"
#define VERSION        "0.1"
#define BUILD          "0034"
#define AUTHOR         "MT"
 
#define DEBUG 1
 
#include <X11/Xlib.h>  /* XOpenDisplay(), etc. */
#include <X11/Xutil.h> /* XSizeHints etc. */
 
#include <stdarg.h>    /* strlen(), etc. */
#include <stdio.h>     /* fprintf(), etc. */
#include <stdlib.h>    /* getenv(), etc. */
 
#include "gcc-debug.h"
 
#include "x11-calc-colour.h"
#include "x11-calc-font.h"
#include "x11-calc-button.h"
  
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
 
int v_init_keypad(o_button* h_button[]){

   /* Define top row of keys. */ 
   h_button[0] = h_button_create(0x11, "1/x", "", "yX", h_normal_font, h_small_font, h_alternate_font, 12, 85, 33, 30, False, BLACK);
   h_button[1] = h_button_create(0x12, "SIN", "", "SIN-\xb9", h_normal_font, h_small_font, h_alternate_font, 48, 85, 33, 30, False, BLACK);
   h_button[2] = h_button_create(0x13, "COS", "", "COS-\xb9", h_normal_font, h_small_font, h_alternate_font, 84, 85, 33, 30, False, BLACK);
   h_button[3] = h_button_create(0x14, "TAN", "", "TAN-\xb9", h_normal_font, h_small_font, h_alternate_font, 120, 85, 33, 30, False, BLACK);
   h_button[4] = h_button_create(0x15, "", "", "", h_normal_font, h_small_font, h_alternate_font, 156, 85, 33, 30, False, LIGHT_BLUE);
   
   /* Define second row of keys. */ 
   h_button[5] = h_button_create(0x21, "X-Y", "", "-R", h_normal_font, h_small_font, h_alternate_font, 12, 128, 33, 30, False, BLACK);
   h_button[6] = h_button_create(0x22, "R", "", "-P", h_normal_font, h_small_font, h_alternate_font, 48, 128, 33, 30, False, BLACK);
   h_button[7] = h_button_create(0x23, "eX", "", "LN", h_normal_font, h_small_font, h_alternate_font, 84, 128, 33, 30, False, BLACK);
   h_button[8] = h_button_create(0x24, "STO", "", "LOG", h_normal_font, h_small_font, h_alternate_font, 120, 128, 33, 30, False, BLACK);
   h_button[9] = h_button_create(0x25, "RCL", "", "10x", h_normal_font, h_small_font, h_alternate_font, 156, 128, 33, 30, False, BLACK); 

   /* Define third row of keys. */
   h_button[10] = h_button_create(0x31, "ENTER", "", "", h_normal_font, h_small_font, h_alternate_font, 12, 171, 69, 30, False, BLACK);
   h_button[11] = h_button_create(0x32, "CHS", "", "/\xaf", h_normal_font, h_small_font, h_alternate_font, 84, 171, 33, 30, False, BLACK);
   h_button[12] = h_button_create(0x33, "EEX", "", "\x1c", h_normal_font, h_small_font, h_alternate_font, 120, 171, 33, 30, False, BLACK);
   h_button[13] = h_button_create(0x34, "CLX", "", "CLR", h_normal_font, h_small_font, h_alternate_font, 156, 171, 33, 30, False, BLACK);

   /* Define fourth row of keys. */
   h_button[14] = h_button_create(0x41, "-", "", "M-", h_large_font, h_small_font, h_alternate_font, 12, 214, 33, 30, False, BEIGE);
   h_button[15] = h_button_create(0x7, "7", "", "", h_large_font, h_small_font, h_alternate_font, 52, 214, 41, 30, False, BEIGE);
   h_button[16] = h_button_create(0x8, "8", "", "", h_large_font, h_small_font, h_alternate_font, 100, 214, 41, 30, False, BEIGE);
   h_button[17] = h_button_create(0x9, "9", "", "", h_large_font, h_small_font, h_alternate_font, 148, 214, 41, 30, False, BEIGE);

   /* Define fifth row of keys. */
   h_button[18] = h_button_create(0x51, "+", "", "M+", h_large_font, h_small_font, h_alternate_font, 12, 257, 33, 30, False, BEIGE);
   h_button[19] = h_button_create(0x4, "4", "", "", h_large_font, h_small_font, h_alternate_font, 52, 257, 41, 30, False, BEIGE);
   h_button[20] = h_button_create(0x5, "5", "", "", h_large_font, h_small_font, h_alternate_font, 100, 257, 41, 30, False, BEIGE);
   h_button[21] = h_button_create(0x6, "6", "", "", h_large_font, h_small_font, h_alternate_font, 148, 257, 41, 30, False, BEIGE);

   /* Define sixth row of keys. */
   h_button[22] = h_button_create(0x61, "\xd7", "", "M\xd7", h_large_font, h_small_font, h_alternate_font, 12, 300, 33, 30, False, BEIGE);
   h_button[23] = h_button_create(0x1, "1", "", "", h_large_font, h_small_font, h_alternate_font, 52, 300, 41, 30, False, BEIGE);
   h_button[24] = h_button_create(0x2, "2", "", "", h_large_font, h_small_font, h_alternate_font, 100, 300, 41, 30, False, BEIGE);
   h_button[25] = h_button_create(0x3, "3", "", "", h_large_font, h_small_font, h_alternate_font, 148, 300, 41, 30, False, BEIGE);

   /* Define bottom row of keys. */
   h_button[26] = h_button_create(0x71, "\xf7", "", "M\xf7", h_large_font, h_small_font, h_alternate_font, 12, 343, 33, 30, False, BEIGE);
   h_button[27] = h_button_create(0x0, "0", "", "", h_large_font, h_small_font, h_alternate_font, 52, 343, 41, 30, False, BEIGE);
   h_button[28] = h_button_create(0x73, ".", "", "", h_large_font, h_small_font, h_alternate_font, 100, 343, 41, 30, False, BEIGE);
   h_button[29] = h_button_create(0x74, "DSP", "", "", h_normal_font, h_small_font, h_alternate_font, 148, 343, 41, 30, False, BEIGE);
}
