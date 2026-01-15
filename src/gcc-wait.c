/*
 * gcc-wait.c
 *
 * Copyright(C) 2019 - MT
 *
 * Cross platform delay function.
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
 * https://stackoverflow.com/questions/10053788
 *
 * 16 Aug 20         - Initial  verson (derived from original code used  in
 *                     gcc-cat.c) - MT
 * 30 Aug 21         - Changed  format statement to specify a long  integer
 *                     in the debug output (fixed warning) - MT
 * 03 Jan 21         - Changed debug() macro so that debug code is executed
 *                     when DEBUG is defined (doesn't need to be true) - MT
 * 31 Mar 22         - Modified to use usleep() on NetBSD - MT
 * 07 Feb 24         - Removed any windows specific or debug code, so don't
 *                     need to include stdio.h - MT
 * 27 Feb 24         - Fixed busy loop 'bug' on newer compilers that do not
 *                     set 'linux' when compiling on Linux - MT
 * 22 Apr 24         - Moved  compiler feature macro definitions to generic
 *                     busy loop - MT
 * 11 Aug 25         - Warn if busy loop used (requires stdio.h) - MT
 * 17 Aug 25         - Solaris (and FreeBSD) can use usleep() - MT
 * 18 Aug 25         - So can Tru64 UNIX - MT
 * 12 Oct 25         - Added Minix (generic busy loop doesn't work) - MT
 * 05 Dec 25         - Calls to LIB$WAIT on VMS 9.x use milliseconds - MT
 * 12 Dec 25         - Added now() to return the current time and elapsed()
 *                     to return the number of miliseconds since a previous
 *                     time - MT
 * 14 Dec 25         - Negative wait times are ignored - MT
 *                   - Tidied up includes - MT
 *
 * Note              - '__solaris__', '__osf__' etc are defined in makefile
 *                     and are (not standard).
 *
 */

#define NAME           "gcc-wait"
#define BUILD          "0011"
#define DATE           "11 Aug 25"
#define AUTHOR         "MT"

#define  False          0
#define  True           !(False)

#if defined(linux) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__solaris__) || defined(__osf__) || defined(__minix__)
#include <ctype.h>     /* isalpha(), etc */
#include <unistd.h>    /* usleep(), sleep() */
#endif

#if defined(VMS)
#include <timeb.h>
#include <lib$routines.h>
#else
#include <sys/types.h>
#include <sys/timeb.h>
#endif

#include <stdio.h>

/*
 * wait (milliseconds)
 *
 * Waits for the specified number of milliseconds
 *
 * 16 Aug 20         - Initial version taken from gcc-cat.c - MT
 * 04 Sep 21         - Fixed formatting in debug code - MT
 * 07 Feb 24         - Removed any windows specific or debug code - MT
 * 11 Aug 25         - Warn if busy loop used (requires stdio.h) - MT
 * 05 Dec 25         - Updated for VMS 9.x - MT
 *
 */
int i_wait(long l_delay)
{
   if (l_delay > 0)  /* Don't delay if duration is negative */
   {
#  if defined(linux) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__solaris__) || defined(__osf__) || defined(__minix__)
      return (usleep(l_delay * 1000)); /* Use usleep() function */
#  elif defined(VMS)
      float f_seconds;
#     if defined(__x86_64__)
         f_seconds = l_delay;  /* On z86_64 platforms LIB$WAIT waits for the specified number of milliseconds */
#     else
         f_seconds = l_delay / 1000.0;  /* On other platforms LIB$WAIT waits for the specified number of seconds */
#     endif
      return (lib$wait(&f_seconds)); /* Use VMS LIB$WAIT - 20 ms resolution timer on VMS*/
#  else
      struct timeb o_start, o_end;
      static int b_busy = True;
      if (b_busy) b_busy = !(printf("Busy loop in use..!\n"));
      ftime(&o_start);
      ftime(&o_end);
      while ((1000 * (o_end.time - o_start.time) + o_end.millitm - o_start.millitm) < l_delay) /* Use a portable but very inefficient busy loop */
      {
         ftime(&o_end);
      }
#  endif
   }
   return(0);
}

/*
 * elapsed (time)
 *
 * Return current time in milliseconds.
 *
 * 12 Dec 25         - Initial version - MT
 *
 */
long l_now(void)
{
    struct timeb t_now;
    ftime(&t_now);

    return (long)t_now.time * 1000L + (long)t_now.millitm;
}

/*
 * elapsed (time)
 *
 * Determines the number of milliseconds since the specified time.
 *
 * 12 Dec 25         - Initial version - MT
 *
 */
long l_since(long l_start)
{
    return l_now() - l_start;
}
