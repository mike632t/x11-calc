/*
 * gcc-wait.h
 *
 * Copyright(C) 2019 - MT
 *
 * Cross platform delay function definition.
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
 * 16 Aug 20         - Initial version - MT
 * 12 Dec 25         - Added now() to return the current time and elapsed()
 *                     to return the number of miliseconds since a previous
 *                     time - MT
 *
 */

int i_wait(long l_delay);
long l_now(void);
long l_since(long l_start);


