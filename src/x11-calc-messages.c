/*
 * x11-calc-text.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2019 - MT
 *
 * Text messages.
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
 * 21 Jan 22   0.1   - Initial version - MT
 *
 */

#define OCTAL

const char * h_msg_loading = "Loading '%s'.\n";
const char * h_msg_saving = "Saving '%s'.\n";

const char * h_err_register_alloc = "Run-time error\t: %s line : %d : Memory allocation failed!\n";
const char * h_err_opening_file = "Unable to open '%s'.\n";

#if defined(HEXADECIMAL)
const char * h_err_unexpected_opcode = "Unexpected opcode (%03x) at %1o-%04x in %s line : %d\n";
const char * h_err_unexpected_error = "Unexpected error at %1o-%04x in %s line : %d\n";
const char * h_err_invalid_address = "Address (%02o) out of range at %1o-%04x in %s line : %d\n";
const char * h_err_invalid_register = "Invalid register (REG[%d]) at %1o-%04x in %s line : %d\n";
const char * h_msg_opcode = "%1o-%04x %04x  ";
const char * h_msg_address = "%04x";
# else
const char * h_err_unexpected_opcode = "Unexpected opcode (%04o) at %1o-%04o in %s line : %d\n";
const char * h_err_unexpected_error = "Unexpected error at %1o-%04o in %s line : %d\n";
const char * h_err_invalid_address = "Address (%02o) out of range at %1o-%04o in %s line : %d\n";
const char * h_err_invalid_register = "Invalid register (REG[%d]) at %1o-%04o in %s line : %d\n";
const char * h_msg_opcode = "%1o-%04o %04o  ";
const char * h_msg_address = "%04o";
#endif

#if defined(unix)

const char * c_msg_usage = "Usage: %s [OPTION]... [FILE]\n\
An RPN Calculator simulation for X11.\n\n\
  -b  ADDR                 set break-point (octal)\n\
  -i, OPCODE               break when specified instruction is executed\n\
  -s,                      start in single step\n\
  -t,                      trace execution\n\
      --cursor             display cursor (default)\n\
      --no-cursor          hide cursor\n\
      --help               display this help and exit\n\
      --version            output version information and exit\n\n";
const char * h_err_invalid_operand = "invalid operand(s)\n";
const char * h_err_invalid_option = "invalid option -- '%c'\n";
const char * h_err_unrecognised_option = "unrecognised option '%s'\n";
const char * h_err_invalid_number = "not an octal number -- '%s' \n";
const char * h_err_address_range = "out of range -- '%s' \n";
const char * h_err_missing_argument = "option requires an argument -- '%s'\n";
const char * h_err_invalid_argument = "expected argument not -- '%c' \n";

#else

const char * c_msg_usage = "Usage: %s [OPTION...] [FILE]\n\
An RPN Calculator simulation for X11.\n\n\
  /cursor                  display cursor (default)\n\
  /nocursor                hide cursor\n\
  /step                    trace execution\n\
  /trace                   trace execution\n\
  /version                 output version information and exit\n\n\
  /?, /help                display this help and exit\n";

const char * h_err_invalid_operand = "invalid parameter(s)\n";
const char * h_err_invalid_option = "invalid option %s\n";

#endif

const char * h_msg_licence = "Copyright(C) %s %s\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n";

const char * h_err_display = "Cannot connect to X server '%s'.\n";
const char * h_err_display_properties = "Unable to get display properties.\n";
const char * h_err_display_colour = "Requires a %d-bit colour display.\n";
const char * h_err_font = "Cannot load font '%s'.\n";
