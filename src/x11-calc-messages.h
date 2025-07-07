/*
 * x11-calc-text.h - RPN (Reverse Polish) calculator simulator.
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
 * 13 Jun 13         - Initial version - MT
 * 02 Feb 22         - Added formatting strings for relative jumps - MT
 * 31 Mar 22         - Modified to compile on NetBSD - MT
 * 24 Dec 22         - Added and explicit check for '__APPLE__' in order to
 *                     allow Mac OS  to be handled in the same way as other
 *                     unix like systems - MT
 * 01 Nov 23         - Made missing argument text common to all platforms - MT
 * 10 Feb 24         - Added an error message to warn the user that the ROM
 *                     contents are empty - MT
 * 16 Feb 24         - Added 'Can't create' error message - MT
 * 12 Jun 25         - Added  error messages for the number of  breakpoints
 *                     and instruction traps - MT
 *                   - Defined messages as string constants - MT
 *
 */

extern const char *h_msg_loading;
extern const char *h_msg_saving;

extern const char *h_err_register_alloc;
extern const char *h_err_opening_file;

extern const char *h_err_unexpected_opcode;
extern const char *h_err_unexpected_error;
extern const char *h_err_invalid_address;
extern const char *h_err_invalid_register;
extern const char *h_msg_opcode;
extern const char *h_msg_address;

extern const char *h_msg_negative_offset;
extern const char *h_msg_positive_offset;
extern const char *h_msg_rom;

extern const char *h_msg_usage;
extern const char *h_err_invalid_operand;
extern const char *h_err_invalid_option;
extern const char *h_err_duplicate_option;
extern const char *h_err_missing_argument;
#if defined(unix) || defined(__unix__) || defined(__APPLE__)
extern const char *h_err_unrecognised_option;
extern const char *h_err_invalid_number;
extern const char *h_err_numeric_range;
extern const char *h_err_max_breakpoints;
extern const char *h_err_invalid_argument;
#endif

extern const char *h_msg_licence;
extern const char *h_err_display;
extern const char *h_err_display_properties;
extern const char *h_err_display_colour;
extern const char *h_err_font;

extern const char *h_err_creating_file;
extern const char *h_err_memmory_alloc;
extern const char *h_err_ROM;
