/*
 * gcc-exists.c
 *
 * Copyright(C) 2020 - MT
 *
 * Cross platform functions to check if a file (or directory) exists.
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
 * 06 Jul 20         - Initial version - MT
 *                   - Added exists() - MT
 * 07 Feb 24         - Rewrote to work with ANSI C - MT
 * 17 Feb 24         - Updated comments - MT
 * 19 Feb 24         - Check that stat() was successful before checking the
 *                     if the file is a directory or a file! - MT
 *                   - Clear the file structure before calling stat(), this
 *                     is proabably overkill but guards against  accidental
 *                     reuse - MT
 *
 */

#define NAME           "gcc-exists"
#define BUILD          "0006"
#define DATE           "19 Feb 24"
#define AUTHOR         "MT"

#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(VMS)
#include <types.h>
#include <stat.h>
#else
#include <sys/types.h>
#include <sys/stat.h>   /* stat */
#endif

#include "gcc-debug.h"

int i_isfile(const char *s_name) /* Returns 'True' if the argument is a regular file */
{
   struct stat o_file;
   int i_status;
   memset(&o_file, 0, sizeof o_file);
   if (stat(s_name, &o_file) == -1)
      i_status = 0;
   else
      i_status = S_ISREG(o_file.st_mode);
   return (i_status);
}

int i_isdir(const char *s_name) /* Returns 'True' if the argument is a directory file */
{
   struct stat o_file;
   int i_status;
   memset(&o_file, 0, sizeof o_file);
   if (stat(s_name, &o_file) == -1)
      i_status = 0;
   else
      i_status = S_ISDIR(o_file.st_mode);
   return (i_status);
}

int i_exists(const char *s_name) /* Returns 'True' if the argument exists */
{
   struct stat o_file;
   memset(&o_file, 0, sizeof o_file);
   return (stat(s_name, &o_file) == 0);
}

