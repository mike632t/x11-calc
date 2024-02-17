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
 *
 */

extern char * h_msg_loading;
extern char * h_msg_saving;

extern char * h_err_register_alloc;
extern char * h_err_opening_file;

extern char * h_err_unexpected_opcode;
extern char * h_err_unexpected_error;
extern char * h_err_invalid_address;
extern char * h_err_invalid_register;
extern char * h_msg_opcode;
extern char * h_msg_address;
const char * h_msg_negative_offset;
const char * h_msg_positive_offset;
const char * h_msg_rom;

extern char * c_msg_usage;
extern char * h_err_invalid_operand;
extern char * h_err_invalid_option;
extern char * h_err_missing_argument;
#if defined(unix) || defined(__unix__) || defined(__APPLE__)
extern char * h_err_unrecognised_option;
extern char * h_err_invalid_number;
extern char * h_err_address_range;
extern char * h_err_invalid_argument;
#endif

extern char * h_msg_licence;
extern char * h_err_display;
extern char * h_err_display_properties;
extern char * h_err_display_colour;
extern char * h_err_font;

extern char * h_err_creating_file;
extern const char * h_err_ROM;
