//-*- c++ -*-
//
// rgb8.h
//
// Copyright (C) 1999-2003 Tom Drummond <twd20@eng.cam.ac.uk>
// Copyright (C)      2003 Paul Smith <pas1001@eng.cam.ac.uk>
//
// This file is part of the CVD image-processing library. See
// http://mi.eng.cam.ac.uk/~twd20/ for updates
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef __RGBA_H
#define __RGBA_H

#include <iostream>

namespace CVD {

//////////////////////////////
// CVD::Rgba
// Template class to represent red, green, blue and alpha components
//

template <typename T>
class Rgba
{
public:
   Rgba() {}
   Rgba(T r, T g, T b, T a) : red(r), green(g), blue(b), alpha(a) {}

   T red;
   T green;
   T blue;
   T alpha;

   Rgba<T>& operator=(const Rgba<T>& c)
      {red = c.red; green = c.green; blue = c.blue; alpha = c.alpha; return *this;}

   template <typename T2>
     Rgba<T>& operator=(const Rgba<T2>& c){
     red = static_cast<T>(c.red);
     green = static_cast<T>(c.green); 
     blue = static_cast<T>(c.blue); 
     alpha = static_cast<T>(c.alpha); 
     return *this;
   }

//   T to_grey() const {return 0.3*red + 0.6*green + 0.1*blue;}
};

template <typename T>
std::ostream& operator <<(std::ostream& os, const Rgba<T>& x)
{
   return os << "(" << x.red << "," << x.green << ","
             << x.blue << "," << x.alpha << ")";
}

inline std::ostream& operator <<(std::ostream& os, const Rgba<unsigned char>& x)
{
   return os << "(" << static_cast<unsigned int>(x.red) << ","
             << static_cast<unsigned int>(x.green) << ","
             << static_cast<unsigned int>(x.blue) << ","
             << static_cast<unsigned int>(x.alpha) << ")";
}

} // end namespace 
#endif
