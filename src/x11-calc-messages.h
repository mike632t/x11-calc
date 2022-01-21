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
 * 13 Jun 13   0.1   - Initial version - MT
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

extern char * c_msg_usage;
extern char * h_err_invalid_operand;
extern char * h_err_invalid_option;
#if defined(unix)
extern char * h_err_unrecognised_option;
extern char * h_err_invalid_number;
extern char * h_err_address_range;
extern char * h_err_missing_argument;
extern char * h_err_invalid_argument;
#endif

extern char * h_msg_licence;
extern char * h_err_display;
extern char * h_err_display_properties;
extern char * h_err_display_colour;
extern char * h_err_font;
