/*                       
	This file is part of the CVD Library.

	Copyright (C) 2005 The Authors

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 
    51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef CVD_COLOURSPACE_H
#define CVD_COLOURSPACE_H

namespace CVD
{
	/// Internal colourspace conversions
	namespace ColourSpace
	{
		/// Convert YUV 411 pixel data to RGB
		/// @param yuv411 The input data
		/// @param npix The number of pixels
		/// @param out The output data
		void yuv411_to_rgb(const unsigned char* yuv411, int npix, unsigned char* out);
		/// Convert YUV 411 pixel data to Y only
		/// @param yuv411 The input data
		/// @param npix The number of pixels
		/// @param out The output data
		void yuv411_to_y(const unsigned char* yuv411, int npix, unsigned char* out);
		/// Convert YUV 411 pixel data to Y and RGB
		/// @param yuv411 The input data
		/// @param npix The number of pixels
		/// @param outc colour output data
		/// @param outy luma output data
		void yuv411_to_rgb_y(const unsigned char* yuv411, int npix, unsigned char* outc, unsigned char* outy);

		
		/// Convert Bayer pattern of the form ??? to greyscale data
		/// @param bggr The input data
		/// @param grey The output data
		/// @param width The width of the image
		/// @param height The height of the image
		void bayer_to_grey(const unsigned char* bggr, unsigned char* grey, unsigned int width, unsigned int height);

		/// Convert Bayer pattern of the form ??? to rgb444 data
		/// @param bggr The input data
		/// @param grey The output data
		/// @param width The width of the image
		/// @param height The height of the image
		void bayer_to_rgb(const unsigned char* bggr, unsigned char* rgb, unsigned int width, unsigned int height);

		
	}
}

#endif
