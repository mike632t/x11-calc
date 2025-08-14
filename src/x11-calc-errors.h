/*
 * x11-error.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2018   MT
 *
 * Prototypes for error handlers.
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
 * 23 Apr 24         - Initial version - MT
 * 14 Aug 25  (0820) - Fixed compiler warnings with clang 17.0.6 - MT
 *
 * TO DO :           -
 */

/** #define __TIME__     "00:00:00" /* Release only */

/* Check that all error codes are defined and define any that are not */

#if !defined(SUCCESS)
#define SUCCESS      0
#endif

#if !defined(ENOENT)
#define ENOENT       2
#endif

#if !defined(EINVAL)
#define EINVAL       22
#endif

#if !defined(ENODATA)
#define ENODATA      61
#endif

#if !defined(ENOACC)
#define ENOACC      126
#endif

#if !defined(ENOCMD)
#define ENOCMD      127
#endif

#if !defined(ENOFNT)
#define ENOFNT      192
#endif

void v_version(void);  /* Display version information */

void v_error(int i_errno, const char *s_fmt, ...);  /* Print formatted error message */

void v_warning(const char *s_fmt, ...);  /* Print formatted warning message */
