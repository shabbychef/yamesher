/*
 
  This file is part of yamesher.
  
  yamesher is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  yamesher is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public License
  along with yamesher.  If not, see <http://www.gnu.org/licenses/>.

  Created: 2016.05.02
  Copyright: Steven E. Pav, 2016
  Author: Steven E. Pav <steven@corecast.io>
  Comments: Steven E. Pav
*/

#ifndef __DEF_MATH__
#define __DEF_MATH__

#include "config.h"

#include <cmath>
#include <CGAL/basic.h>
#include "debugging.h"
#include "types.h"

namespace lopping {

	//really convert a FT to a double;//FOLDUP
	template< class Kernel_ >
		inline double FT2double(const typename Kernel_::FT &inp)
	{
		//return to_double(inp);
		return CGAL::to_double(inp);
	};//UNFOLD

	//uggh. sometimes == gives false negatives for FT?//FOLDUP
	template< class Kernel_ >
		inline bool FTequal(const typename Kernel_::FT &a,const typename Kernel_::FT &b)
	{
		//try this
		typedef typename Kernel_::FT FT;
		CARP( GRIPE_INFO, "FTeq checking: " << FT2double< Kernel_ >(a - b) << "\n")
		return (0.0 == FT2double< Kernel_ >(a - b));
	};//UNFOLD
	
	//get abs of FT;//FOLDUP
	template< class Kernel_ >
		typename Kernel_::FT absFT(const typename Kernel_::FT &inp)
	{
		typedef typename Kernel_::FT FT;
		if (inp < FT(0)) { return -inp; }
		return inp;
	};//UNFOLD

	//get sqrt of FT;//FOLDUP
	template< class Kernel_ >
		typename Kernel_::FT sqrtFT(const typename Kernel_::FT &inp)
	{
		typedef typename Kernel_::FT FT;
		return FT( (std::sqrt( FT2double< Kernel_ >(inp) )) );
	};//UNFOLD

	//mod between 0 and 2pi//FOLDUP
	template< class Kernel_ >
		typename Kernel_::FT mod_twopi(const typename Kernel_::FT &inp)
	{
		typedef typename Kernel_::FT FT;
		FT retval = FT(std::fmod( FT2double< Kernel_ >(inp), TWOPI ));
		if (retval < FT(0.0)) {
			retval = retval + FT(TWOPI);
		}
		return retval;
	};//UNFOLD

	//scalar cross product: a x b . k//FOLDUP
	template< class Kernel_ >
		typename Kernel_::FT
			scalar_cross(const typename Kernel_::Vector_2 &a,
									 const typename Kernel_::Vector_2 &b)
			{ return ((a.x() * b.y()) - (a.y() * b.x())); };
//UNFOLD

}

#endif /* __DEF_MATH__ */

//for vim modeline: (do not edit)
// vim:ts=2:sw=2:tw=79:fdm=marker:fmr=FOLDUP,UNFOLD:cms=//%s:tags=.c_tags;:syn=cpp:ft=cpp:mps+=<\:>:ai:si:cin:nu:fo=croql:cino=p0t0c5(0:
