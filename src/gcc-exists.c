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
 * 06 Jul 20   0.1   - Initial version - MT
 *             0.2   - Added exists() - MT
 * 07 Feb 24   0.3   - Rewrote to work with ANSI C - MT
 *
 */

#define VERSION        "0.3"
#define BUILD          "0003"
#define DATE           "07 Feb 24"

#include <stdio.h>
#include <stdlib.h>

#if defined(VMS)
#include <types.h>
#include <stat.h>
#else
#include <sys/types.h>
#include <sys/stat.h>   /* stat */
#endif

int i_isfile(const char *s_name) {
   struct stat o_file;
   stat(s_name, &o_file);
   return (S_ISREG(o_file.st_mode));
}

int i_isdir(const char *s_name) {
   struct stat o_file;
   stat(s_name, &o_file);
   return (S_ISDIR(o_file.st_mode));
}

int i_exists(const char *s_name) {
   struct stat o_file;
   return (stat(s_name, &o_file) == 0);
}

