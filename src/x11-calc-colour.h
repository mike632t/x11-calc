/*
 * x11-calc-colour.h - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2013   MT
 *
 * Colour constants and function prototypes.
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
 * 02 Jul 13         - Initial version - MT
 * 24 Jul 13         - Made the shade of red a little darker - MT
 * 10 Dec 18         - Define monochrome colours if compiling on VMS (a bit
 *                     of  a cheat to allow me to test the code using  simh
 *                     as it doesn't support a colour display) - MT
 * 28 Sep 21         - Added mid grey - MT
 * 06 Dec 21         - Changed green to a slightly lighter colour - MT
 */

#if defined(vms)

#define DARK_TEXT      0x000000 /* Dark gray text. */
#define LIGHT_TEXT     0x000000 /* Off white text. */

#define BEIGE          0xffffff /* Beige numeric keys. */
#define BLACK          0xffffff /* Black function keys. */
#define DARK_GREY      0xffffff /* Gray function keys. */
#define DARK_GRAY      0xffffff /* Gray function keys. */
#define MID_GREY       0xffffff /* Mid gray numeric keys. */
#define MID_GRAY       0xffffff /* Mid gray numeric keys. */
#define LIGHT_GREY     0xffffff /* Light gray numeric keys. */
#define LIGHT_GRAY     0xffffff /* Light gray numeric keys. */
#define GREEN          0xffffff /* Green function keys. */
#define YELLOW         0xffffff /* Yellow shift key. */
#define BLUE           0x000000 /* Blue alternate text. */
#define LIGHT_BLUE     0xffffff /* Light Blue shift key. */

#define RED            0x000000 /* LED display colour. */
#define DARK_RED       0xffffff /* LED display shading. */
#define RED_BACKGROUND 0xffffff /* LED display background */
#define BACKGROUND     0x000000 /* Background. */

#else

#define DARK_TEXT      0x101010 /* Dark gray text. */
#define LIGHT_TEXT     0xf0f0f0 /* Off white text. */

#define BEIGE          0xd9c8b6 /* Beige numeric keys. */
#define BLACK          0x2e312e /* Black function keys. */
#define DARK_GREY      0x8f8f8f /* Gray function keys. */
#define DARK_GRAY      0x8f8f8f /* Gray function keys. */
#define MID_GREY       0xa9a9a9 /* Mid gray numeric keys. */
#define MID_GRAY       0xa9a9a9 /* Mid gray numeric keys. */
#define LIGHT_GREY     0xcecfc6 /* Light gray numeric keys. */
#define LIGHT_GRAY     0xcecfc6 /* Light gray numeric keys. */
#define GREEN          0x948639 /* Green function keys. */
#define YELLOW         0xf5a434 /* Yellow shift key. */
#define BLUE           0x4aa0b0 /* Blue alternate text. */
#define LIGHT_BLUE     0x4aa0b0 /* Blue shift key. */

#define RED            0xb00000 /* LED display colour. */
#define DARK_RED       0x400000 /* LED display shading. */
#define RED_BACKGROUND 0x180000 /* LED display background */
#define BACKGROUND     0x433431 /* Background. */

#endif

unsigned int i_shade(unsigned int i_colour);
unsigned int i_tint(unsigned int i_colour);
