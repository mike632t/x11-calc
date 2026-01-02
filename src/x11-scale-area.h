/*
 * x11-scale-area.h
 *
 * Copyright(C) 2025   MT
 *
 * Display a PNG image.
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
 * 23 Sep 25  0.1.0001  - Initial version - MT
 * 
 */
 
void XScaleArea(Display *p_x_display,
                Drawable d_x_source,
                Drawable d_x_dest,
                GC d_x_context,
                int i_src_x, int i_src_y,
                unsigned int i_src_width, unsigned int i_src_height,
                int i_dest_x, int i_dest_y,
                unsigned int i_dest_width, unsigned int i_dest_height);
