/*
 * x11-calc-font.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * Font definations and data structures.
 *
 * This  program is free software: you can redistribute it and/or modify  it
 * under  the  terms of the GNU General Public License as published  by  the
 * Free  Software  Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is  distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without  even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License  along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * 09 Mar 14         - Initial version - MT
 * 10 Dec 18         - Defined DEC fonts if compiling on VMS - MT
 *
 */

#if defined(vms)

#define NORMAL_TEXT    "*helvetica-medium-r-*-10-*" /* 12 Font to use for function keys. */
#define SMALL_TEXT     "*helvetica-medium-r-*-8-*" /* 10 Font to use for shifted functions. */
#define LARGE_TEXT     "*helvetica-medium-r-*-12-*" /* 13 Font to use for numeric keys. */
#define ALTERNATE_TEXT "*helvetica-medium-r-*-8-*"  /*  8 Font to use for alternate functions. */

#else

#define NORMAL_TEXT    "6x12" /* Font to use for function keys. */
#define SMALL_TEXT     "6x10" /* Font to use for shifted functions. */
#define LARGE_TEXT     "6x13" /* Font to use for numeric keys. */
#define ALTERNATE_TEXT "5x8" /* Font to use for alternate functions. */

#endif

XFontStruct *h_normal_font; /* Font for function keys. */
XFontStruct *h_small_font; /* Font for shifted function labels. */
XFontStruct *h_alternate_font; /* Font for alternate function labels. */
XFontStruct *h_large_font; /* Font for numeric keys. */


