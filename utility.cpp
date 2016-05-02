//START HEADER
//START COPYRIGHT
/*
 *  Corner Lopping Mesher Software
 *  Copyright (C) 2004 Steven E. Pav
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 spav@ucsd.edu 
 */
//END COPYRIGHT
//START CVSLOG
/* CVS info:
 * $Id: utility.cpp,v 1.14 2005/03/11 03:46:57 spav Exp $
 * $Log: utility.cpp,v $
 * Revision 1.14  2005/03/11 03:46:57  spav
 * problem was ball splitting? -- changed whether subsegs of ball get encroachment checking.
 *
 * Revision 1.13  2005/03/11 02:04:53  spav
 * something wrong with cubics?
 *
 * Revision 1.12  2005/02/19 00:02:49  spav
 * looks like cubic bezier is working
 *
 * Revision 1.11  2005/02/15 23:51:55  spav
 * still working on bez_3
 *
 * Revision 1.10  2005/02/12 01:37:17  spav
 * fixed xfig read bug.
 *
 * Revision 1.8  2005/01/04 23:54:46  spav
 * some tweaks.
 *
 * Revision 1.7  2005/01/03 03:59:54  spav
 * home from san fran.
 *
 * Revision 1.6  2004/12/21 22:42:57  spav
 * looks like it is working; added command line parser.
 *
 * Revision 1.5  2004/12/08 01:36:54  spav
 * looking good. have segment uniqueness problem, tho. have finished curvature split grooming problem?
 *
 * Revision 1.4  2004/12/06 03:54:20  spav
 * added more stuff
 *
 * Revision 1.3  2004/12/01 00:02:48  spav
 * seems to work for arcs. start to add bezier.
 *
 * Revision 1.2  2004/11/29 07:05:04  spav
 * post thanksgiving
 *
 * Revision 1.1  2004/11/10 06:26:29  spav
 * end table is screwy..
 *
 * Revision 1.1  2004/10/20 21:50:26  spav
 * starting code project
 *
 *
 */
//END CVSLOG
//END HEADER

#include "utility.h"

//because template export is not supported, utility.h 
//may include this file, so we add this gate:

#ifndef UTILITY_CPP
#define UTILITY_CPP

namespace lopping {

	//cgal is a piece of shit
	
	//really convert a FT to a double;//FOLDUP
	template< class Kernel_ >
		inline double 
			FT2double(const typename Kernel_::FT &inp)
	{
		//return to_double(inp);
		return CGAL::to_double(inp);
	};//UNFOLD

	//uggh. sometimes == gives false negatives for FT?//FOLDUP
	template< class Kernel_ >
		inline bool
			FTequal(const typename Kernel_::FT &a,const typename Kernel_::FT &b)
	{
		//try this
		typedef typename Kernel_::FT FT;
		CARP( GRIPE_INFO, "FTeq checking: " << FT2double< Kernel_ >(a - b) << "\n")
		return (0.0 == FT2double< Kernel_ >(a - b));
	};//UNFOLD
	
	//get abs of FT;//FOLDUP
	template< class Kernel_ >
		typename Kernel_::FT
			absFT(const typename Kernel_::FT &inp)
	{
		typedef typename Kernel_::FT FT;
		if (inp < FT(0)) { return -inp; }
		return inp;
	};//UNFOLD

	//get sqrt of FT;//FOLDUP
	template< class Kernel_ >
		typename Kernel_::FT
			sqrtFT(const typename Kernel_::FT &inp)
	{
		typedef typename Kernel_::FT FT;
		return FT( (std::sqrt( FT2double< Kernel_ >(inp) )) );
	};//UNFOLD

	//solve a0 + a1t + a2t^2 = 0 between lo and hi//FOLDUP
	//return whether a root was found in there
	//(returns false if discriminant is negative).
	//returns root in t if returns true.
	template< class Kernel_ >
		bool     hasroot(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, 
										const typename Kernel_::FT &lo, const typename Kernel_::FT &hi,
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
			laguerreSolve(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, 
										typename Kernel_::FT &t, const typename Kernel_::FT &eps, 
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
			laguerreSolve(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, const typename Kernel_::FT &a3,
										typename Kernel_::FT &t, const typename Kernel_::FT &eps, 
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
			laguerreSolve(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, const typename Kernel_::FT &a3,
									  const typename Kernel_::FT &a4, 
										typename Kernel_::FT &t, const typename Kernel_::FT &eps, 
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
			laguerreSolve(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, const typename Kernel_::FT &a3,
									  const typename Kernel_::FT &a4, const typename Kernel_::FT &a5, 
										typename Kernel_::FT &t, const typename Kernel_::FT &eps, 
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
			laguerreSolve(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, const typename Kernel_::FT &a3,
									  const typename Kernel_::FT &a4, const typename Kernel_::FT &a5, 
									  const typename Kernel_::FT &a6, 
										typename Kernel_::FT &t, const typename Kernel_::FT &eps, 
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

	//scalar cross product: a x b . k//FOLDUP
	template< class Kernel_ >
		typename Kernel_::FT
			scalar_cross(const typename Kernel_::Vector_2 &a,
									 const typename Kernel_::Vector_2 &b)
			{ return ((a.x() * b.y()) - (a.y() * b.x())); };
//UNFOLD

	//collinearity testing is a problem with CGAL?
	
	//points on a horizontal line, i.e. same y coordinate//FOLDUP
	template< class Kernel_ >
		bool is_horizontal_collinear(const typename Kernel_::Point_2 &a,
																 const typename Kernel_::Point_2 &b,
																 const typename Kernel_::Point_2 &c)
//		{ 
//#if PARANOIA > CHECK_BIZARRE
//			GRIPE("horcheck:\n");
//			GRIPE("a: " << a.y() << " b: " << b.y() << " c: " << c.y() << "\n");
//			GRIPE("a: " << a.hy() << "/" << a.hw() << "\n");
//			GRIPE("b: " << b.hy() << "/" << b.hw() << "\n");
//			GRIPE("c: " << c.hy() << "/" << c.hw() << "\n");
//			if ((a.y()) == (b.y()))
//			{
//				GRIPE("a: " << a.y() << "==  b: " << b.y() << "\n");
//			} else
//			{
//				GRIPE("a != b\n");
//				GRIPE("lessee: ");
//				GRIPE( FT2double< Kernel_ >(a.hy() * b.hw())  << " != " << FT2double< Kernel_ >(b.hy() * a.hw()) << "?\n" );
//			}
//
//			if ((b.y()) == (c.y()))
//			{
//				GRIPE("b: " << b.y() << "==  c: " << c.y() << "\n");
//			} else
//			{
//				GRIPE("b != c" << "\n");
//			}
//
//			GRIPE("endhorcheck\n");
//#endif
//
//			return ( ( (a.hy() * b.hw()) == (b.hy() * a.hw()) ) && 
//			         ( (b.hy() * c.hw()) == (c.hy() * b.hw()) ) ); 
//			return ( (FTequal< Kernel_ >(a.y(),b.y())) &&
//			         (FTequal< Kernel_ >(b.y(),c.y())) );
		{ return ((a.y() == b.y()) && (b.y() == c.y())); 
		};//UNFOLD

	//points on a vertical line, i.e. same x coordinate//FOLDUP
	template< class Kernel_ >
		bool   is_vertical_collinear(const typename Kernel_::Point_2 &a,
																 const typename Kernel_::Point_2 &b,
																 const typename Kernel_::Point_2 &c)
		{ return ((a.x() == b.x()) && (b.x() == c.x())); };//UNFOLD

	//points are collinear i.e. same x coordinate, or same y coordinate, or//FOLDUP
	//cgal::collinear
	template< class Kernel_ >
		bool   is_collinear(const typename Kernel_::Point_2 &a,
												const typename Kernel_::Point_2 &b,
												const typename Kernel_::Point_2 &c)
	{
		return ( (is_horizontal_collinear< Kernel_ >(a,b,c)) || 
		         (  is_vertical_collinear< Kernel_ >(a,b,c)) || 
						 CGAL::collinear(a,b,c) );
	};//UNFOLD

	//get max and min angle of triangle; bool value returns//FOLDUP
	//whether the triangle is degenerate. 
	template< class Kernel_ >
		bool min_max_angle(double &min_deg,double &max_deg,
											 const typename Kernel_::Point_2 &a,
											 const typename Kernel_::Point_2 &b,
											 const typename Kernel_::Point_2 &c)
	{
		if (is_collinear<Kernel_>(a,b,c))
		{ return true; }
		//now find angles
		
		double smallest,largest;
		typedef typename Kernel_::FT FT;

		typename Kernel_::Vector_2 vC(a,b);
		typename Kernel_::Vector_2 vB(a,c);
		typename Kernel_::Vector_2 vA(b,c);

		double AA = FT2double< Kernel_ >(sqrtFT< Kernel_ >(vA * vA));
		double BB = FT2double< Kernel_ >(sqrtFT< Kernel_ >(vB * vB));
		double CC = FT2double< Kernel_ >(sqrtFT< Kernel_ >(vC * vC));

		double AB = FT2double< Kernel_ >(vA * vB);
		double AC = FT2double< Kernel_ >(vA * vC);
		double BC = FT2double< Kernel_ >(vB * vC);

		double angA = std::acos( BC / (BB * CC) );
		double angB = std::acos( -AC / (AA * CC) );
		double angC = std::acos( AB / (AA * BB) );

		if (angA > angB)
		{
			if (angA > angC)
			{
				largest = angA;
				smallest = ((angC > angB)? angB : angC);
			} else
			{
				largest = angC;
				smallest = ((angA > angB)? angB : angA);
			}
		} else // angA <= angB
		{
			if (angB > angC)
			{
				largest = angB;
				smallest = ((angC > angA)? angA : angC);
			} else
			{
				largest = angC;
				smallest = ((angA > angB)? angB : angA);
			}
		}

		if (smallest == 0)
		{
#if PARANOIA > CHECK_BIZARRE
			CARP( GRIPE_INFO, "caught collinear, should not have computed?\n")
			CARPP( GRIPE_INFO, a)
			CARPP( GRIPE_INFO, b)
			CARPP( GRIPE_INFO, c)
#endif
			return true;
		}

		min_deg = smallest;
		max_deg = largest;

		return false;
	};//UNFOLD

	//the points a,b,c are supposed to be collinear.  so //FOLDUP
	//shift point c by a small amount to make orthogonal and
	//collinear if necessary.
	template< class Kernel_ >
		bool shift_orthogonal(const typename Kernel_::Point_2 &a,
												const typename Kernel_::Point_2 &b,
												typename Kernel_::Point_2 &c)
	{
		if (is_collinear< Kernel_ >(a,b,c))
			{ return true; }
			
		typedef typename Kernel_::FT FT;
		if ((a.y() == b.y()) && (c.y() != b.y()))
		{
			c = typename Kernel_::Point_2(c.x(),a.y());
		} 
		if ((a.x() == b.x()) && (c.x() != b.x()))
		{
			c = typename Kernel_::Point_2(a.x(),c.y());
		} 
		return false;
	};//UNFOLD

	//scale a point about another by ratio;//FOLDUP
	template< class Kernel_ >
		typename Kernel_::Point_2 
			scale_by_ratio(const typename Kernel_::Point_2 &anchor, 
							  const typename Kernel_::Point_2 &preimage,
							  const typename Kernel_::FT ratio)
	{
		typename Kernel_::Vector_2 offset_v(anchor,preimage);
		return anchor + (offset_v * ratio);
	};//UNFOLD

	//scale a point about another by sqrt(ratio);//FOLDUP
	template< class Kernel_ >
		typename Kernel_::Point_2 
			scale_by_ratio_sqrt(const typename Kernel_::Point_2 &anchor, 
							  const typename Kernel_::Point_2 &preimage,
							  const typename Kernel_::FT ratio)
	{
		typedef typename Kernel_::FT FT;
		//FIX seems like a bad idea:
		if (ratio < FT(0))
		{
			return scale_by_ratio< Kernel_ >(anchor,preimage, 
																		 - (sqrtFT< Kernel_ >(-ratio)) );
		} else {
			return scale_by_ratio< Kernel_ >(anchor,preimage, 
																		 sqrtFT< Kernel_ >(ratio));
		}
	};//UNFOLD

	//mod between 0 and 2pi//FOLDUP
	template< class Kernel_ >
		typename Kernel_::FT
			mod_twopi(const typename Kernel_::FT &inp)
	{
		typedef typename Kernel_::FT FT;
		FT retval = FT(std::fmod( FT2double< Kernel_ >(inp), TWOPI ));
		if (retval < FT(0.0))
		{
			retval = retval + FT(TWOPI);
		}
		return retval;
	};//UNFOLD

	//get angle to x-axis;//FOLDUP
	//between 0 and 2pi;
	template< class Kernel_ >
		typename Kernel_::FT
			angle_to_xaxis(const typename Kernel_::Vector_2 &thev)
	{
		typedef typename Kernel_::FT FT;
		FT vang = FT( std::atan2( FT2double< Kernel_ >(thev.y()), 
															FT2double< Kernel_ >(thev.x()) ) );
		return mod_twopi<Kernel_>(vang);
	};//UNFOLD

	//get angle to x-axis;//FOLDUP
	//between 0 and 2pi;
	template< class Kernel_ >
		typename Kernel_::FT
			angle_to_xaxis(const typename Kernel_::Direction_2 &thed)
	{
		typedef typename Kernel_::FT FT;
		FT dang = FT( std::atan2( FT2double< Kernel_ >(thed.dy()), 
															FT2double< Kernel_ >(thed.dx()) ) );
		return mod_twopi<Kernel_>(dang);
	};//UNFOLD

	//return counterclockwise angle abc//FOLDUP
	//between 0 and 2pi;
	template< class Kernel_ >
		typename Kernel_::FT
			ccw_angle(const typename Kernel_::Point_2 &a,
								const typename Kernel_::Point_2 &b,
								const typename Kernel_::Point_2 &c)
	{
		typedef typename Kernel_::FT FT;
		typename Kernel_::Vector_2 btoa(b,a);
		typename Kernel_::Vector_2 btoc(b,c);
		FT ba_tox = angle_to_xaxis<Kernel_>( btoa );
		FT bc_tox = angle_to_xaxis<Kernel_>( btoc );
		FT abc_ang = bc_tox - ba_tox;
		return mod_twopi<Kernel_>(abc_ang);
	};//UNFOLD

	//return angle abc//FOLDUP
	//between 0 and pi;
	template< class Kernel_ >
		typename Kernel_::FT
			abs_angle(const typename Kernel_::Point_2 &a,
								const typename Kernel_::Point_2 &b,
								const typename Kernel_::Point_2 &c)
	{
		typedef typename Kernel_::FT FT;
		typename Kernel_::Vector_2 btoa(b,a);
		typename Kernel_::Vector_2 btoc(b,c);

		FT CC = btoa * btoa;
		FT AA = btoc * btoc;
		FT cos_thet = (btoa * btoc) / sqrtFT< Kernel_ >(AA * CC);
		return FT(std::acos( FT2double< Kernel_ >(cos_thet)));
	};//UNFOLD


	//scale a point to sqrt distance //FOLDUP
	//that is make a point along line from anchor to preimage
	//with distance sqrt(dist_sqrd) from anchor
	//if dist_sqrd is negative, flip it.
	template< class Kernel_ >
		typename Kernel_::Point_2 
			scale_to_distance(const typename Kernel_::Point_2 &anchor, 
							  const typename Kernel_::Point_2 &preimage,
							  const typename Kernel_::FT dist_sqrd)
	{
		return scale_by_ratio_sqrt<Kernel_>(anchor,preimage,
				(dist_sqrd/CGAL::squared_distance(anchor,preimage)));
	};//UNFOLD

	//return true if both points are on same side//FOLDUP
	//of the line. n.b. there are three sides to a line...
	template< class Kernel_ >
		bool in_different_half_spaces(const typename Kernel_::Line_2 &cl,
																	const typename Kernel_::Point_2 &a,
																	const typename Kernel_::Point_2 &b)
	{
		
#if PARANOID
		switch (cl.oriented_side(a)) {
			case CGAL::ON_ORIENTED_BOUNDARY : CARP( GRIPE_INFO, "a is on boundary\n" );break;
			case CGAL::ON_POSITIVE_SIDE : CARP( GRIPE_INFO, "a is on + side\n" );break;
			case CGAL::ON_NEGATIVE_SIDE : CARP( GRIPE_INFO, "a is on - side\n" );break;
		};
		switch (cl.oriented_side(b)) {
			case CGAL::ON_ORIENTED_BOUNDARY : CARP( GRIPE_INFO, "b is on boundary\n" );break;
			case CGAL::ON_POSITIVE_SIDE : CARP( GRIPE_INFO, "b is on + side\n" );break;
			case CGAL::ON_NEGATIVE_SIDE : CARP( GRIPE_INFO, "b is on - side\n" );break;
		};

		typename Kernel_::FT ca = cl.a() * a.x() + cl.b() * a.y() + cl.c();
		typename Kernel_::FT cb = cl.a() * b.x() + cl.b() * b.y() + cl.c();
		CARP( GRIPE_INFO, "a in line eq: " << ca << "\n")
		CARP( GRIPE_INFO, "b in line eq: " << cb << "\n")

#endif
		return ((cl.oriented_side(a) != cl.oriented_side(b)) &&
						(cl.oriented_side(a) != CGAL::ON_ORIENTED_BOUNDARY) &&
						(cl.oriented_side(b) != CGAL::ON_ORIENTED_BOUNDARY));
	};//UNFOLD

	//get bisector line of angle abc;//FOLDUP
	//if abc are collinear, return the bisector of a & c
	template< class Kernel_ >
		typename Kernel_::Line_2 
			bisector(const typename Kernel_::Point_2 &a, 
							 const typename Kernel_::Point_2 &b,
							 const typename Kernel_::Point_2 &c)
	{
		if (is_collinear< Kernel_ >(a,b,c)) { return radax< Kernel_ >(a,c); } else
		{
			//first get the bisector to p1 p p2
			typename Kernel_::FT d1 = CGAL::squared_distance(b,a);
			typename Kernel_::FT d2 = CGAL::squared_distance(b,c);
	
			typename Kernel_::Point_2 pnt_left(a);
			typename Kernel_::Point_2 pnt_rght(c);
	
			//typename Kernel_::FT scale_amount;
	
			if (d1 > d2)
			{ pnt_rght = scale_by_ratio_sqrt<Kernel_>(b,c,(d1/d2)); } else
			{ pnt_left = scale_by_ratio_sqrt<Kernel_>(b,a,(d2/d1)); };
	
			//bisecting line:
			typename Kernel_::Point_2 vtip = CGAL::midpoint(pnt_left,pnt_rght);
	
			if (vtip == b) { return radax< Kernel_ >(a,c); };
			//else
			return typename Kernel_::Line_2(b, vtip);
		}
	};//UNFOLD

	//return point on segment between two center points and//FOLDUP
	//on the radical axis;
	template< class Kernel_ >
		typename Kernel_::Point_2
			radax_vertex(const typename Kernel_::Point_2 &center_1,
								   const typename Kernel_::Point_2 &center_2,
									 const typename Kernel_::FT radius_sqrd_1,
									 const typename Kernel_::FT radius_sqrd_2)
	{
		typedef typename Kernel_::FT FT;
		FT cen_dist_sqrd = CGAL::squared_distance(center_1,center_2);

		FT scale_ratio = 
			(FT(1.0) + (radius_sqrd_1 - radius_sqrd_2)/cen_dist_sqrd) / FT(2.0);

		return scale_by_ratio< Kernel_ >(center_1,center_2,scale_ratio);
	};//UNFOLD

	//return radical axis of the two circles//FOLDUP
	template< class Kernel_ >
		typename Kernel_::Line_2
			radax(const typename Kernel_::Point_2 &center_1,
					  const typename Kernel_::Point_2 &center_2,
						const typename Kernel_::FT radius_sqrd_1,
						const typename Kernel_::FT radius_sqrd_2)
	{
		typename Kernel_::Point_2 r_vert = 
			radax_vertex< Kernel_ >(center_1,center_2,radius_sqrd_1,radius_sqrd_2);

		typename Kernel_::Line_2 center_line(center_1,center_2);
		return center_line.perpendicular(r_vert);
	};//UNFOLD

	//return radical axis of the two points, i.e. circles of zero radius//FOLDUP
	template< class Kernel_ >
		typename Kernel_::Line_2
			radax(const typename Kernel_::Point_2 &center_1,
					  const typename Kernel_::Point_2 &center_2)
	{
		typename Kernel_::Point_2 r_vert = CGAL::midpoint(center_1,center_2);
		typename Kernel_::Line_2 center_line(center_1,center_2);
		return center_line.perpendicular(r_vert);
	};//UNFOLD

	//return number of intersections of circle and //FOLDUP
	//line, with the intersections put into the two points.
	template< class Kernel_ >
		int intersections(const typename Kernel_::Point_2 &center,
											const typename Kernel_::FT radius_sqrd,
											const typename Kernel_::Line_2 &the_line,
											typename Kernel_::Point_2 & point_1,
											typename Kernel_::Point_2 & point_2)
	{
		typedef typename Kernel_::FT FT;

		typename Kernel_::Point_2 c_proj = the_line.projection(center);
		FT rcos_sqrd = CGAL::squared_distance(center,c_proj);

		FT rsin_sqrd = radius_sqrd - rcos_sqrd;
		if (rsin_sqrd < FT(0))
		{ return 0; }
		if (rsin_sqrd == FT(0))
		{ point_1 = c_proj;point_2 = c_proj; return 1; }

		typename Kernel_::Vector_2 line_as_vec = the_line.to_vector();
		typename Kernel_::Point_2 v_tip = c_proj + line_as_vec;

		//FIX: a bit inefficient to calculate distance twice?
		point_1 = scale_to_distance< Kernel_ >(c_proj,v_tip,rsin_sqrd);
		point_2 = scale_to_distance< Kernel_ >(c_proj,v_tip,-rsin_sqrd);
		return 2;
	};//UNFOLD

	//return number of intersections of two circles and //FOLDUP
	//with the intersections put into the two points.
	template< class Kernel_ >
		int intersections(const typename Kernel_::Point_2 &center_1,
											const typename Kernel_::Point_2 &center_2,
											const typename Kernel_::FT radius_sqrd_1,
											const typename Kernel_::FT radius_sqrd_2,
											typename Kernel_::Point_2 & point_1,
											typename Kernel_::Point_2 & point_2)
	{
		typename Kernel_::Line_2 radical_axis = 
			radax< Kernel_ >(center_1,center_2,radius_sqrd_1,radius_sqrd_2);
		return intersections(center_1,radius_sqrd_1,radical_axis,point_1,point_2);
	};//UNFOLD

	//return intersection of segment ac with bisector of abc//FOLDUP
	template< class Kernel_ >
		typename Kernel_::Point_2 
		bisector_cevian(const typename Kernel_::Point_2 &a,
										const typename Kernel_::Point_2 &b,
										const typename Kernel_::Point_2 &c)
	{
		typename Kernel_::Line_2 biline = bisector<Kernel_>(a,b,c);
		typename Kernel_::Segment_2 pseg(a,c);
		typename CGAL::Object iobj = CGAL::intersection(pseg,biline);
		typename Kernel_::Point_2 ipoint;

		if (CGAL::assign(ipoint,iobj)) { return ipoint; } 
		//else//FIX?
		return b;

	};//UNFOLD

} 

#endif //UTILITY_CPP
