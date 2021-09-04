/*
 * gcc-debug.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * Debug macro.
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
 * 17 Aug 13         - Initial version - MT
 * 07 Dec 18         - Updated  allow debugging to be turned on and off  by
 *                     setting  DEBUG true or false - MT
 * 22 Aug 21         - Prints Current filename and line number - MT
 *                   - Added verbose macro - MT
 *
 */

/* Execute code if DEBUG is True */
#ifndef debug /* Don't redefine macro if already defined. */
#define debug(code) do {if(DEBUG){fprintf(stderr, "Debug\t: %s line : %d : ", \
            __FILE__, __LINE__); code;}} while(0)
#endif

#ifndef verbose /* Don't redefine macro if already defined. */
#define verbose(code) do {if(VERBOSE){fprintf(stderr, "Verbose\t: %s line : %d : ", \
            __FILE__, __LINE__); code;}} while(0)
#endif
