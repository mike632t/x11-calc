/*
 * gcc-wait.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2019 - MT
 *
 * Cross platform delay.
 *
 * Waits for the specified number of milliseconds.
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
 * 16 Aug 20         - Initial  verson (derived from original code used  in
 *                     gcc-cat.c) - MT
 * 30 Aug 21         - Changed  format statement to specify a long  integer
 *                     in the debug output (fixed warning) - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0001"
#define DATE           "16 Aug 20"

#define DEBUG 0        /* Enable/disable debug */

#include <stdio.h>
#if defined(linux)
#include <unistd.h>
#include <sys/types.h>
#elif defined(WIN32)
#include <windows.h>
#elif defined(VMS)
#include <timeb.h>
#include <lib$routines.h>
#else
#include <sys/types.h>
#include <sys/timeb.h>
#endif

#include "gcc-debug.h"

/*
 * wait (milliseconds)
 *
 * Waits for the specified number of milliseconds
 *
 * 16 Aug 20         - Initial version taken from gcc-cat.c - MT
 *  4 Sep 21         - Fixed formatting in debug code - MT
 *
 */
int i_wait(long l_delay) { /* wait for milliseconds */
#if defined(linux) /* Use usleep() function */
debug(fprintf(stderr, "Pausing using usleep() for  %ld ms.\n", l_delay));
return (usleep(l_delay * 1000));
#elif defined(WIN32) /* Use usleep() function */
debug(fprintf(stderr, "Pausing using sleep() for %d ms.\n", l_delay));
Sleep(l_delay);
return (0);
#elif defined(VMS) /* Use VMS LIB$WAIT */
float f_seconds;
debug(fprintf(stderr, "Pausing using LIB$WAIT for %ld ms.\n", l_delay));
f_seconds = l_delay / 1000.0;
return (lib$wait(&f_seconds));
#else /* Use a portable but very inefficent busy loop */
struct timeb o_start, o_end;
debug(fprintf(stderr, "Pausing for %ld ms.\n", l_delay));
ftime(&o_start);
ftime(&o_end);
while ((1000 * (o_end.time - o_start.time) + o_end.millitm - o_start.millitm) < l_delay) {
   ftime(&o_end);
}
return(0);
#endif
}
