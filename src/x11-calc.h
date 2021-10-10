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
 *
 * TO DO :           -
 */
#define  COMMIT_ID "[Commit ID: $Format:%h$]"

#ifdef HP21
#include "x11-calc-21.h"
#elif HP22
#include "x11-calc-22.h"
#elif HP25
#include "x11-calc-25.h"
#elif HP27
#include "x11-calc-27.h"
#elif HP29
#include "x11-calc-29.h"
#elif HP31
#include "x11-calc-31.h"
#elif HP32
#include "x11-calc-32.h"
#elif HP33
#include "x11-calc-33.h"
#endif

void v_version(); /* Display version information */
void v_about(); /* Display help text */
void v_error(const char *s_fmt, ...); /* Print formatted error message */
