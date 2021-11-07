/*
 * x11-calc.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2018   MT
 *
 * Common function definitions.
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
 * 31 Aug 20   0.1   - Initial version - MT
 * 31 Aug 20         - Resolved dependencies between header files by moving
 *                     common function definitions to a separate file - MT
 * 08 Aug 21         - Tidied up spelling errors in the comments - MT
 * 22 Aug 21         - Added definition for commit id - MT
 * 23 Sep 21         - Model number defined in the makefile and selected in
 *                     here  using a preprocessor directive to include  the
 *                     appropriate constants and funtion definitions  - MT
 * 12 Oct 21         - Created  macros for all the text messages making  it
 *                     easier to redefine them based on language at compile
 *                     time if required - MT
 *                   - Defined  title and original filename based on  model
 *                     number - MT
 * 29 Oct 21         - Included files and token defined separately for each
 *                     model - MT
 *
 * TO DO :           -
 */
#define COMMIT_ID "[Commit ID: $Format:%h$]"

/** #define __TIME__     "00:00:00" /* Release only */

#ifdef HP21
#define FILENAME     "x11-calc-21"
#define TITLE        "RPN calc 21"
#include "x11-calc-21.h"

#elif HP22
#define FILENAME     "x11-calc-22"
#define TITLE        "RPN calc 22"
#include "x11-calc-22.h"

#elif HP25
#define FILENAME     "x11-calc-25"
#define TITLE        "RPN calc 25C"
#include "x11-calc-25.h"

#elif HP27
#define FILENAME     "x11-calc-27"
#define TITLE        "RPN calc 27"
#include "x11-calc-27.h"

#elif HP29
#define FILENAME     "x11-calc-29"
#define TITLE        "RPN calc 29C"
#include "x11-calc-29.h"

#elif HP31
#define FILENAME     "x11-calc-31"
#define TITLE        "RPN calc 31"
#include "x11-calc-31.h"

#elif HP32
#define FILENAME     "x11-calc-32"
#define TITLE        "RPN calc 32"
#include "x11-calc-32.h"

#elif HP33
#define FILENAME     "x11-calc-33"
#define TITLE        "RPN calc 33C"
#include "x11-calc-33.h"

#elif HP11
#define FILENAME     "x11-calc-11"
#define TITLE        "RPN calc 11"
#include "x11-calc-11.h"
#endif

#ifdef vms

#define COLOUR_DEPTH 1
#define HELP_TEXT    "Usage: %s [OPTION]... \n"\
                     "An RPN Calculator simulation for X11.\n\n"\
                     "  /cursor                  display cursor (default)\n"\
                     "  /nocursor                hide cursor\n"\
                     "  /step                    trace execution\n"\
                     "  /trace                   trace execution\n"\
                     "  /version                 output version information and exit\n\n"\
                     "  /?, /help                display this help and exit\n"\
#define HELP_COMMAND "Try '%s /help' for more information.\n"

#define INVALID_COMMAND       "invalid parameter(s)\n"
#define INVALID_OPTION        "invalid option %s\n"
#else
#define COLOUR_DEPTH 24
#define HELP_TEXT    "Usage: %s [OPTION]... \n"\
                     "An RPN Calculator simulation for X11.\n\n"\
                     "  -b  ADDR                 set break-point (octal)\n"\
                     "  -s, --step               start in single step\n"\
                     "  -t, --trace              trace execution\n"\
                     "      --cursor             display cursor (default)\n"\
                     "      --no-cursor          hide cursor\n"\
                     "      --help               display this help and exit\n"\
                     "      --version            output version information and exit\n\n"
#define HELP_COMMAND "Try '%s --help' for more information.\n"

#define INVALID_COMMAND       "invalid operand(s)\n"
#define INVALID_OPTION        "invalid option -- '%c'\n"
#define INVALID_ADDRESS       "not an octal address -- '%s' \n"
#define INVALID_RANGE         "out of range -- '%s' \n"
#define MISSING_ARGUMENT      "option requires an argument -- '%s'\n"
#define INVALID_ARGUMENT      "expected argument not -- '%c' \n"

#endif

#define UNRECOGNIZED_OPTION   "unrecognized option '%s'\n"
#define LICENCE_TEXT          "Copyright(C) %s %s\n"\
                              "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n"\
                              "This is free software: you are free to change and redistribute it.\n"\
                              "There is NO WARRANTY, to the extent permitted by law.\n"
#define DISPLAY_ERROR         "Unable to get display properties.\n"
#define COLOUR_ERROR          "Requires a %d-bit colour display.\n"
#define FONT_ERROR            "Cannot load font '%s'.\n"

void v_version(); /* Display version information */

void v_about(); /* Display help text */

void v_error(const char *s_fmt, ...); /* Print formatted error message */

void v_warning(const char *s_fmt, ...); /* Print formatted warning message and return */
