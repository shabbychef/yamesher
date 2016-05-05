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
  Author: Steven E. Pav <shabbychef@gmail.com>
  Comments: Steven E. Pav
*/

#ifndef __DEF_ROOTFIND__
#define __DEF_ROOTFIND__

#include <cmath>
#include <CGAL/basic.h>
#include "math.h"
#include "debugging.h"

namespace lopping {

	//solve a0 + a1t + a2t^2 = 0 between lo and hi//FOLDUP
	//return whether a root was found in there
	//(returns false if discriminant is negative).
	//returns root in t if returns true.
	template< class Kernel_ >
		bool     hasroot(const typename Kernel_::FT &a0, 
										 const typename Kernel_::FT &a1,
										 const typename Kernel_::FT &a2, 
										 const typename Kernel_::FT &lo, 
										 const typename Kernel_::FT &hi, 
										 typename Kernel_::FT &t)
		{
			typedef typename Kernel_::FT FT;
			FT root;
	
			if (a2 == FT(0.0)) 
			{     
				if (a1 == FT(0.0))
				{
					if (a0 == FT(0.0))
					{
						t = (lo + hi) / FT(2.0);
						return true;
					} //else
					return false;
				} //else
				root = -a0 / a1;
				if ((root>=lo) && (root<=hi)) { t=root;return true; }
				//else
				return false;
			} //else
	
			FT aa0 =  a0 / a2;
			root = - (a1 / (a2 * FT(2.0)));
			//solve aa0 - 2 root t + t^2 = 0
			//soln: root ± sqrt(root * root - aa0)
	
			FT disc = root * root - aa0;
			if (disc < FT(0.0)) { return false; }
			disc = sqrtFT< Kernel_ >(disc);
			if (root < ((lo+hi)/FT(2.0)))
			{
				root += disc;
			} else
			{ //citardauq:
				root = aa0 / (root + disc);
			}
	
			if ((root>=lo) && (root<=hi)) { t=root;return true; }
			//else
			return false;
		};//UNFOLD

	//solve a0 + a1t + a2t^2 = 0 iteratively//FOLDUP
	//from the initial iterate to the given tolerance
	//spits out the value of the function at the last iterate,
	//and returns last iterate as the t.
	template< class Kernel_ >
		typename Kernel_::FT
			laguerreSolve(const typename Kernel_::FT &a0, 
										const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, 
										typename Kernel_::FT &t, 
										const typename Kernel_::FT &eps, 
										const unsigned int &maxits = 6)
		{
			typedef typename Kernel_::FT FT;

			if (a2 == FT(0.0)) 
			{     
				if (a1 == FT(0.0))
				{
					if (a0 == FT(0.0))
					{
						return FT(0.0);
					} //else
					return FT(0.0);
				} //else
				t = -a0 / a1;
				return FT(0.0);
			} //else

			FT A,disq;
			FT pnext,ppnext,pppnext;
			unsigned int iter = 0;
	
			pnext = a0 + t * (a1 + t * a2);
			pppnext = a2 * FT(2.0);

			while (((pnext > eps) || (pnext < -eps)) && (iter++ < maxits))//FOLDUP
			{
				ppnext  = a1 + t * (a2 * FT(2.0));

				A = -ppnext / pnext;
				disq = A * A - FT(2.0) * pppnext / pnext;
				if (A > FT(0.0))
				{ A = A + sqrtFT< Kernel_ >(disq);
				} else
				{ A = A - sqrtFT< Kernel_ >(disq); }
				t = t + FT(2.0) / A;

				pnext = a0 + t * (a1 + t * a2);
			}//UNFOLD

			if (iter == maxits) { VOICE( CHECK_MATH, "quad solve went to maxit\n" ) }

			return pnext;
		};//UNFOLD

	//solve a0 + a1t + a2t^2 + a3t^3 = 0 iteratively//FOLDUP
	//from the initial iterate to the given tolerance
	//spits out the value of the function at the last iterate,
	//and returns last iterate as the t.
	template< class Kernel_ >
		typename Kernel_::FT
			laguerreSolve(const typename Kernel_::FT &a0, 
										const typename Kernel_::FT &a1, 
										const typename Kernel_::FT &a2,
										const typename Kernel_::FT &a3, 
										typename Kernel_::FT &t, 
										const typename Kernel_::FT &eps, 
										const unsigned int &maxits = 6)
		{
			typedef typename Kernel_::FT FT;
			if (a3 == FT(0.0)) { return laguerreSolve< Kernel_ >(a0,a1,a2,t,eps,maxits); }

			FT A,disq;
			FT pnext,ppnext,pppnext;
			unsigned int iter = 0;
	
			pnext = a0 + t * (a1 + t * (a2 + t * a3));
	
			while (((pnext > eps) || (pnext < -eps)) && (iter++ < maxits))//FOLDUP
			{
				ppnext  = a1 + t * (a2 * FT(2.0) + t * a3 * FT(3.0));
				pppnext = a2 * FT(2.0) + t * a3 * FT(6.0);

				A = -ppnext / pnext;
				disq = FT(4.0) * A * A - FT(6.0) * pppnext / pnext;
				if (A > FT(0.0))
				{ A = A + sqrtFT< Kernel_ >(disq);
				} else
				{ A = A - sqrtFT< Kernel_ >(disq); }
				t = t + FT(3.0) / A;

				pnext = a0 + t * (a1 + t * (a2 + t * a3));
			}//UNFOLD

			if (iter == maxits) { VOICE( CHECK_MATH, "cubic solve went to maxit\n" ) }

			return pnext;
		};//UNFOLD

	//solve a0 + a1t + a2t^2 + a3t^3 + a4t^4 = 0 iteratively//FOLDUP
	//from the initial iterate to the given tolerance
	//spits out the value of the function at the last iterate,
	//and returns last iterate as the t.
	template< class Kernel_ >
		typename Kernel_::FT
			laguerreSolve(const typename Kernel_::FT &a0, 
										const typename Kernel_::FT &a1, 
										const typename Kernel_::FT &a2, 
										const typename Kernel_::FT &a3, 
										const typename Kernel_::FT &a4, 
										typename Kernel_::FT &t, 
										const typename Kernel_::FT &eps, 
										const unsigned int &maxits = 6)
		{
			typedef typename Kernel_::FT FT;
			if (a4 == FT(0.0)) { return laguerreSolve< Kernel_ >(a0,a1,a2,a3,t,eps,maxits); }

			FT A,disq;
			FT pnext,ppnext,pppnext;
			unsigned int iter = 0;
	
			pnext = a0 + t * (a1 + t * (a2 + t * (a3 + t * a4)));
	
			while (((pnext > eps) || (pnext < -eps)) && (iter++ < maxits))//FOLDUP
			{
				ppnext  = a1 + t * (a2 * FT(2.0) + t * (a3 * FT(3.0) + t * a4 * FT(4.0)));
				pppnext = a2 * FT(2.0) + t * (a3 * FT(6.0) + t * a4 * FT(12.0));

				A = -ppnext / pnext;
				disq = FT(9.0) * A * A - FT(12.0) * pppnext / pnext;
				if (A > FT(0.0))
				{ A = A + sqrtFT< Kernel_ >(disq);
				} else
				{ A = A - sqrtFT< Kernel_ >(disq); }
				t = t + FT(4.0) / A;

				pnext = a0 + t * (a1 + t * (a2 + t * (a3 + t * a4)));
			}//UNFOLD

			if (iter == maxits) { VOICE( CHECK_MATH, "4th deg solve went to maxit\n" ) }

			return pnext;
		};//UNFOLD

	//solve a0 + a1t + a2t^2 + a3t^3 + a4t^4 + a5t^5 = 0 iteratively//FOLDUP
	//from the initial iterate to the given tolerance
	//spits out the value of the function at the last iterate,
	//and returns last iterate as the t.
	template< class Kernel_ >
		typename Kernel_::FT
			laguerreSolve(const typename Kernel_::FT &a0, 
										const typename Kernel_::FT &a1, 
										const typename Kernel_::FT &a2, 
										const typename Kernel_::FT &a3, 
										const typename Kernel_::FT &a4, 
										const typename Kernel_::FT &a5, 
										typename Kernel_::FT &t, 
										const typename Kernel_::FT &eps, 
										const unsigned int &maxits = 7)
		{
			typedef typename Kernel_::FT FT;
			if (a5 == FT(0.0)) { return laguerreSolve< Kernel_ >(a0,a1,a2,a3,a4,t,eps,maxits); }

			FT A,disq;
			FT pnext,ppnext,pppnext;
			unsigned int iter = 0;
	
			pnext = a0 + t * (a1 + t * (a2 + t * (a3 + t * (a4 + t * a5))));
	
			while (((pnext > eps) || (pnext < -eps)) && (iter++ < maxits))//FOLDUP
			{
				ppnext  = a1 + t * (a2 * FT(2.0) + t * 
														(a3 * FT(3.0) + t * 
														 (a4 * FT(4.0) + t *
															(a5 * FT(5.0)))));
				ppnext  = a2 * FT(2.0) + t * (a3 * FT(6.0) + t * 
																			(a4 * FT(12.0) + t * 
																			 (a5 * FT(20.0))));

				A = -ppnext / pnext;
				disq = FT(16.0) * A * A - FT(20.0) * pppnext / pnext;
				if (A > FT(0.0))
				{ A = A + sqrtFT< Kernel_ >(disq);
				} else
				{ A = A - sqrtFT< Kernel_ >(disq); }
				t = t + FT(5.0) / A;

				pnext = a0 + t * (a1 + t * (a2 + t * (a3 + t * (a4 + t * a5))));
			}//UNFOLD

			if (iter == maxits) 
			{ 
				VOICE( CHECK_MATH, "5th deg solve went to maxit\n" ) 
				if ((pnext > (FT(5.0) * eps)) || (pnext < (FT(5.0) * -eps)))
				{
					VOICE( CHECK_MATH, "pnext is " << pnext << "\n" )
				}
			}
			

			return pnext;
		};//UNFOLD

	//solve a0 + a1t + a2t^2 + a3t^3 + a4t^4 + a5t^5 + a6t^6 = 0 iteratively//FOLDUP
	//from the initial iterate to the given tolerance
	//spits out the value of the function at the last iterate,
	//and returns last iterate as the t.
	template< class Kernel_ >
		typename Kernel_::FT
			laguerreSolve(const typename Kernel_::FT &a0, 
										const typename Kernel_::FT &a1, 
										const typename Kernel_::FT &a2, 
										const typename Kernel_::FT &a3, 
										const typename Kernel_::FT &a4, 
										const typename Kernel_::FT &a5, 
										const typename Kernel_::FT &a6, 
										typename Kernel_::FT &t, 
										const typename Kernel_::FT &eps, 
										const unsigned int &maxits = 7)
		{
			typedef typename Kernel_::FT FT;
			if (a6 == FT(0.0)) { return laguerreSolve< Kernel_ >(a0,a1,a2,a3,a4,a5,t,eps,maxits); }

			FT A,disq;
			FT pnext,ppnext,pppnext;
			unsigned int iter = 0;
	
			pnext = a0 + t * (a1 + t * (a2 + t * (a3 + t * (a4 + t * (a5 + t * a6)))));
	
			while (((pnext > eps) || (pnext < -eps)) && (iter++ < maxits))//FOLDUP
			{
				ppnext  = a1 + t * (a2 * FT(2.0) + t * 
														(a3 * FT(3.0) + t * 
														 (a4 * FT(4.0) + t *
															(a5 * FT(5.0) + t * a6 * FT(6.0)))));
				ppnext  = a2 * FT(2.0) + t * (a3 * FT(6.0) + t * 
																			(a4 * FT(12.0) + t * 
																			 (a5 * FT(20.0) + t * a6 * FT(30.0))));

				A = -ppnext / pnext;
				disq = FT(25.0) * A * A - FT(30.0) * pppnext / pnext;
				if (A > FT(0.0))
				{ A = A + sqrtFT< Kernel_ >(disq);
				} else
				{ A = A - sqrtFT< Kernel_ >(disq); }
				t = t + FT(6.0) / A;

				pnext = a0 + t * (a1 + t * (a2 + t * (a3 + t * (a4 + t * (a5 + t * a6)))));
			}//UNFOLD

			if (iter == maxits) 
			{ 
				VOICE( CHECK_MATH, "6th deg solve went to maxit\n" ) 
				if ((pnext > (FT(5.0) * eps)) || (pnext < (FT(5.0) * -eps)))
				{
					VOICE( CHECK_MATH, "pnext is " << pnext << "\n" )
				}
			}
			

			return pnext;
		};//UNFOLD

}

#endif /* __DEF_ROOTFIND__ */

//for vim modeline: (do not edit)
// vim:ts=2:sw=2:tw=79:fdm=marker:fmr=FOLDUP,UNFOLD:cms=//%s:tags=.c_tags;:syn=cpp:ft=cpp:mps+=<\:>:ai:si:cin:nu:fo=croql:cino=p0t0c5(0:
