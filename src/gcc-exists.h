/*
 * gcc-exists.h
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
 *
 */

int i_isfile(char *_name);

int i_isdir(char *_name);

int i_exists(char *_name);
