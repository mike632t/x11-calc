/*
 * x11-scale-area.c
 *
 * Copyright(C) 2025   MT
 *
 * Resizes  a pixmap or image using bi-linear scaling to find the  weighted 
 * average  of  the colours in each  pixel to provide an  approximation  of
 * intensity at any point.
 * 
 * For example if the we have four pixels with the colour values
 *
 *   (top-left)    C10 = 100 / C11 = 200    (top-right)
 *   (bottom-left) C00 =  50 / C01 = 150  (bottom-right)
 *
 * Then to find the intensity at the middle of all four pixels we would use 
 * 
 *   dx = 0.5,  (halfway between left and right)
 *   dy = 0.5   (halfway between top and bottom)
 *
 * First step is to find the horizontal intensity values
 * 
 *   C_top   = (1 - dx) * C10 + dx * C11
 *         = (0.5 * 100) + (0.5 * 200)
 *         = 50 + 100 = 150
 *
 *   C_bottom = (1 - dx) * C00 + dx * C01
 *         = (0.5 * 50) + (0.5 * 150)
 *         = 25 + 75 = 100
 *
 * Then we need to average the top and bottom intensity values 
 * 
 *   C = (1 - dy) * C_top + dy * C_bottom
 *    = (0.5 * 150) + (0.5 * 100)
 *    = 75 + 50 = 125
 *
 * The result is an average of all four pixels.
 *
 * If the fractional offsets are different 
 * 
 *   dx = 0.0   (exactly aligned with left column)
 *   dy = 0.5   (halfway between top and bottom)
 *
 * Find the horizontal intensity values using the left-hand values only (as 
 * dx = 0.0).
 * 
 *    C_top    = (1 - 0.0) * C10 + 0.0 * C11 = 100
 *    C_bottom = (1 - 0.0) * C00 + 0.0 * C01 =  50
 *
 * Then average the top and bottom intensity values 
 * 
 *   C = (1 - dy) * C_top + dy * C_bottom
 *    = (0.5 * 100) + (0.5 * 50)
 *    = 50 + 25 = 75
 *
 * The result is an average between the top-left and bottom-left pixels.
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
#include <stdio.h>
#include <stdlib.h>

#include "gcc-debug.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

/*
 * Resizes a source pixmap into a destination pixmap using bilinear scaling 
 * optimized for 24/32-bit.
 * 
 * Assumes that each pixel is stored as (B, G, R, X) (little endian)  where 
 * X is the alpha channel.
 * 
 * Approx 0.0112s 
 * 
 */

void XScaleArea(Display *x_display, Drawable x_src_pixmap, Drawable x_dest_pixmap, GC x_context, 
            int i_src_x, int i_src_y, unsigned int i_src_width, unsigned int i_src_height,
            int i_dest_x, int i_dest_y, unsigned int i_dest_width, unsigned int i_dest_height)
{
   XImage *x_src_image, *x_dest_image;
   /** Pixmap x_image; /* Buffer */
   double *d_column_offsets, *d_row_offsets;
   unsigned char *c_src_data, *c_dest_data;
   char *p_output_buffer;  

   double d_src_x;  /* Fractional source x coordinate /*/  
   double d_src_y;  /* Fractional source y coordinate /*/
   double d_frac_x;  /* Fractional offset in x axis */
   double d_frac_y;  /* Fractional offset in y axis */
   double d_red, d_green, d_blue;
   unsigned int i_src_x0;  /* Integer source x (floor) */
   unsigned int i_src_x1;  /* Integer source x (ceil/next) */
   unsigned int i_src_bottom_row;  /* Integer source y (floor) */
   unsigned int i_src_top_row ;  /* Integer source y (ceil/next) */ 
   unsigned char *p_row_dst;  /* Pointer to start of current destination row */
   unsigned char *p_src_bottom_left, *p_src_bottom_right,  *p_src_top_left, *p_src_top_right; 
   int i_red_bottom_left, i_red_bottom_right, i_red_top_left, i_red_top_right;
   int i_blue_bottom_left, i_blue_bottom_right, i_blue_top_left, i_blue_top_right;
   int i_green_bottom_left, i_green_bottom_right, i_green_top_left, i_green_top_right;
   int i_bytes_per_pixel_src = 0, i_bytes_per_pixel_dest = 0;
   double d_inv_dest_w = 0.0, d_inv_dest_h = 0.0;
   
   if (i_src_width >= 2 && i_src_height >= 2 && i_dest_width >= i_src_width && i_dest_height >= i_src_height)  /* Check minimum dimensions (destination could be smaller that source) */ 
   {
      
      x_src_image = XGetImage(x_display, x_src_pixmap, i_src_x, i_src_y, i_src_width, i_src_height, AllPlanes, ZPixmap);  /* Get source image */
      if (x_src_image == None) 
      {
         fprintf(stderr, "XScaleArea: XGetImage failed\n");
      }
      else
      {
         /* Allocate buffer for destination image */
         size_t buf_size = (size_t)i_dest_width * i_dest_height * x_src_image->bits_per_pixel / 8;
         p_output_buffer = (char *)malloc(buf_size);
         if (p_output_buffer == None) 
         {
            fprintf(stderr, "XScaleArea: malloc for destination buffer failed\n");
         }
         else
         {
            /* Create destination image */
            x_dest_image = XCreateImage(x_display, DefaultVisual(x_display, DefaultScreen(x_display)), DefaultDepth(x_display, DefaultScreen(x_display)), ZPixmap, 0, p_output_buffer, i_dest_width, i_dest_height, 32, 0); /* Assume 32-bit colour depth */
            /** x_image = XCreatePixmap(x_display, XDefaultRootWindow(x_display), i_dest_width, i_dest_height, 32); /* Create a buffer */

            if (x_dest_image == None) 
            {
               if (p_output_buffer) free(p_output_buffer);  /* Free output buffer */
               fprintf(stderr, "XScaleArea: XCreateImage failed\n");
            }
            else 
            {
               /* Setup pointers and constants */
               c_src_data = (unsigned char *)x_src_image->data;
               c_dest_data = (unsigned char *)x_dest_image->data;
               i_bytes_per_pixel_src = x_src_image->bits_per_pixel / 8;
               i_bytes_per_pixel_dest = x_dest_image->bits_per_pixel / 8;

               d_inv_dest_w = 1.0 / (double)i_dest_width;  /* Precompute 1/width */
               d_inv_dest_h = 1.0 / (double)i_dest_height;  /* Precompute 1/height */

               /* Allocate coordinate maps */
               d_column_offsets = (double *)malloc(i_dest_width * sizeof(double));
               d_row_offsets = (double *)malloc(i_dest_height * sizeof(double));
               if (d_column_offsets && d_row_offsets)  /* Check arrays for offsets allocated */
               {
                  /* Precompute row and column offsets */
                  for (unsigned int i_x = 0; i_x < i_dest_width; ++i_x)
                     d_column_offsets[i_x] = (double)i_x * (double)(i_src_width  - 1) * d_inv_dest_w;
                  for (unsigned int i_y = 0; i_y < i_dest_height; ++i_y)
                     d_row_offsets[i_y] = (double)i_y * (double)(i_src_height  - 1) * d_inv_dest_h;

                  /* Iterate over destination pixmap working out the colour of each pixel from original image */
                  for (unsigned int i_y = 0; i_y < i_dest_height; ++i_y) {
                     d_src_y = d_row_offsets[i_y];
                     i_src_bottom_row = (unsigned int)d_src_y;
                     d_frac_y = d_src_y - i_src_bottom_row;
                     i_src_top_row = (i_src_bottom_row + 1 < x_src_image->height) ? i_src_bottom_row + 1 : i_src_bottom_row;

                     p_row_dst = c_dest_data + i_y * x_dest_image->bytes_per_line;

                     for (unsigned int i_x = 0; i_x < i_dest_width; ++i_x) {
                        d_src_x = d_column_offsets[i_x];
                        i_src_x0 = (unsigned int)d_src_x;
                        d_frac_x = d_src_x - i_src_x0;
                        i_src_x1 = (i_src_x0 + 1 < x_src_image->width) ? i_src_x0 + 1 : i_src_x0;

                        /* Source pixel pointers */
                        p_src_bottom_left = c_src_data + i_src_bottom_row * x_src_image->bytes_per_line + i_src_x0 * i_bytes_per_pixel_src;
                        p_src_top_left = c_src_data + i_src_bottom_row * x_src_image->bytes_per_line + i_src_x1 * i_bytes_per_pixel_src;
                        p_src_bottom_right = c_src_data + i_src_top_row * x_src_image->bytes_per_line + i_src_x0 * i_bytes_per_pixel_src;
                        p_src_top_right = c_src_data + i_src_top_row * x_src_image->bytes_per_line + i_src_x1 * i_bytes_per_pixel_src;

#if defined(BIGENDIAN)
                        int i_red_bottom_left   = p_src_px_bottom_left[1], i_green_bottom_left = p_src_px_bottom_left[2], i_blue_bottom_left = p_src_px_bottom_left[3];
                        int i_red_bottom_right   = p_src_px_bottom_right[1], i_green_bottom_right = p_src_px_bottom_right[2], i_blue_bottom_right = p_src_px_bottom_right[3];
                        int i_red_top_left   = p_src_px_top_left[1], i_green_top_left = p_src_px_top_left[2], i_blue_top_left = p_src_px_top_left[3];
                        int i_red_top_right   = p_src_px_top_right[1], i_green_top_right = p_src_px_top_right[2], i_blue_top_right = p_src_px_top_right[3];
#else
                        i_blue_bottom_left  = p_src_bottom_left[0], i_green_bottom_left = p_src_bottom_left[1], i_red_bottom_left = p_src_bottom_left[2];
                        i_blue_bottom_right  = p_src_bottom_right[0], i_green_bottom_right = p_src_bottom_right[1], i_red_bottom_right = p_src_bottom_right[2];
                        i_blue_top_left  = p_src_top_left[0], i_green_top_left = p_src_top_left[1], i_red_top_left = p_src_top_left[2];
                        i_blue_top_right  = p_src_top_right[0], i_green_top_right = p_src_top_right[1], i_red_top_right = p_src_top_right[2];
#endif
                        /* Bilinear interpolation */
                        d_red = i_red_bottom_left * (1.0 - d_frac_x) * (1.0 - d_frac_y) + i_red_top_left  *d_frac_x * (1.0 - d_frac_y) + i_red_bottom_right * (1.0 - d_frac_x) * d_frac_y + i_red_top_right * d_frac_x*d_frac_y;
                        d_green = i_green_bottom_left * (1.0 - d_frac_x) * (1.0 - d_frac_y) + i_green_top_left*d_frac_x * (1.0 - d_frac_y) + i_green_bottom_right * (1.0 - d_frac_x) * d_frac_y + i_green_top_right * d_frac_x*d_frac_y;
                        d_blue =  i_blue_bottom_left * (1.0 - d_frac_x) * (1.0 - d_frac_y) + i_blue_top_left *d_frac_x * (1.0 - d_frac_y) + i_blue_bottom_right * (1.0 - d_frac_x) * d_frac_y + i_blue_top_right * d_frac_x*d_frac_y;

                        /* Write pixel */
#if defined(BIGENDIAN)
                        p_row_dst[1] = (unsigned char)d_red;
                        p_row_dst[2] = (unsigned char)d_green;
                        p_row_dst[3] = (unsigned char)d_blue;
                        if (i_bytes_per_pixel_dest == 4) p_row_dst[0] = 0xFF;
#else
                        p_row_dst[0] = (unsigned char)d_blue;
                        p_row_dst[1] = (unsigned char)d_green;
                        p_row_dst[2] = (unsigned char)d_red;
                        if (i_bytes_per_pixel_dest == 4) p_row_dst[3] = 0xFF;
#endif
                        p_row_dst += i_bytes_per_pixel_dest; /* Advance pointer */
                     }
                  }

                  /* Copy scaled image into destination drawable */
                  XPutImage(x_display, x_dest_pixmap, x_context, x_dest_image, 0, 0, i_dest_x, i_dest_y, i_dest_width, i_dest_height);
                  /** XCopyArea(x_display, x_image, x_dest_pixmap, x_context, 0, 0, i_dest_width, i_dest_height, 0, 0); /* Copy buffer to destination */
                  /** XFreePixmap(x_display, x_image); /* Free buffer */
                  free(d_column_offsets);
                  free(d_row_offsets);
               } 
               else 
               {
                  fprintf(stderr, "XScaleArea: failed to allocate coordinate arrays\n");
               }
               XDestroyImage(x_dest_image); /* Destroy destination image */
            } 
         } 
         /* Destroy source image (only if it was created) */
         if (x_src_image) XDestroyImage(x_src_image);
      } 
   } 
   else 
   {
      fprintf(stderr, "XScaleArea: invalid dimensions (%u x %u -> %u x %u)\n",
              i_src_width, i_src_height, i_dest_width, i_dest_height);
   }
}
