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
 * $Id: utility.h,v 1.19 2005/03/11 03:46:57 spav Exp $
 * $Log: utility.h,v $
 * Revision 1.19  2005/03/11 03:46:57  spav
 * problem was ball splitting? -- changed whether subsegs of ball get encroachment checking.
 *
 * Revision 1.18  2005/03/11 02:04:53  spav
 * something wrong with cubics?
 *
 * Revision 1.17  2005/02/19 00:02:50  spav
 * looks like cubic bezier is working
 *
 * Revision 1.16  2005/02/15 23:51:55  spav
 * still working on bez_3
 *
 * Revision 1.15  2005/02/12 01:37:17  spav
 * fixed xfig read bug.
 *
 * Revision 1.13  2005/01/26 23:11:46  spav
 * looks good.
 *
 * Revision 1.12  2005/01/18 22:47:11  spav
 * changing provoker to runtime option.
 *
 * Revision 1.11  2005/01/04 23:54:46  spav
 * some tweaks.
 *
 * Revision 1.10  2005/01/03 03:59:54  spav
 * home from san fran.
 *
 * Revision 1.9  2004/12/21 22:42:57  spav
 * looks like it is working; added command line parser.
 *
 * Revision 1.8  2004/12/08 01:36:54  spav
 * looking good. have segment uniqueness problem, tho. have finished curvature split grooming problem?
 *
 * Revision 1.7  2004/12/06 03:54:20  spav
 * added more stuff
 *
 * Revision 1.6  2004/12/04 02:50:26  spav
 * straight, arc works. adding bezier.
 *
 * Revision 1.5  2004/12/01 00:02:48  spav
 * seems to work for arcs. start to add bezier.
 *
 * Revision 1.4  2004/11/29 07:05:04  spav
 * post thanksgiving
 *
 * Revision 1.3  2004/11/23 23:43:32  spav
 * appears to work for straight segs. now add arcs, bezier.
 *
 * Revision 1.2  2004/11/19 02:10:56  spav
 * something odd: an infinite vertex and repeated ballsplitting
 *
 * Revision 1.1  2004/11/10 06:26:29  spav
 * end table is screwy..
 *
 *
 */
//END CVSLOG
//END HEADER

#ifndef UTILITY_H
#define UTILITY_H

#include "config.h"

#include <cmath>
#include <CGAL/basic.h>

//verbosity levels

//whether to gripe info regarding the progress of the algorithm
#define GRIPE_PROGRESS 1
//whether to gripe info regarding the progress of the algorithm
#define GRIPE_ITERATES 2
#define GROSS_ITERATES 10000

//debugging verbosity

//whether to gripe when entering or exiting a function
#define GRIPE_GATES 3
//whether to gripe when entering or exiting a major loop
#define GRIPE_LOOPS 4
//whether to gripe info regarding points, segs, balls being split, etc
#define GRIPE_INFO 3
//whether to gripe info regarding which way control goes
#define GRIPE_FORKS 4
//whether to gripe constructor and destructor calls
#define GRIPE_CDTORS 4

//paranoia levels

//whether to check really bizarre possibilities
#define CHECK_BIZARRE 6
//whether to check for problems with inexact arithmetic.
#define CHECK_MATH 1


//convert objects to string to gripe them

#define PTOSTR(p)  "(x: " << p.x() << " y: " << p.y() << ")\n" 
#define VHTOSTR(vh)  (*vh) << "\n"

#define GRIPEP(p) GRIPE( "(x: " << p.x() << " y: " << p.y() << ")\n" )
#define GRIPEV(v) GRIPEP( v.point() )
#define GRIPEVH(vh) GRIPE( (*vh) << "\n" )

#ifdef PARANOID
#define GRIPE_P(X) std::cerr << X
#define GRIPE_PX(Y,Z,X) if (Y > Z) { GRIPE_P(X); } else { }
#define GRIPE_PXP(Y,Z,p) GRIPE_PX(Y,Z, PTOSTR(p))
#define GRIPE_PXVH(Y,Z,vh) GRIPE_PX(Y,Z, VHTOSTR(vh))
#else
#define GRIPE_P(X) 			/* X */ 
#define GRIPE_PX(Y,Z,X) /* X */
#define GRIPE_PXP(Y,Z,p) /* X */
#define GRIPE_PXVH(Y,Z,vh) /* X */
#endif

#define GRIPE_IX(Y,Z,X) if (Y > Z) { std::cerr << X; }

#define LUVERBOSITY lopping::VERBOSITY

#define VOICE(Z,X) GRIPE_IX(LUVERBOSITY,Z,X)
#define CARP(Z,X)   GRIPE_PX(LUVERBOSITY,Z,X)
#define CARPP(Z,p)  GRIPE_PXP(LUVERBOSITY,Z,p)
#define CARPVH(Z,vh) GRIPE_PXVH(LUVERBOSITY,Z,vh)


namespace lopping {

	static int VERBOSITY = 0;

	const double PI = 3.14159265358979233438;
	const double TWOPI = 2.0 * PI;

	//Triplet<A_,B_,C_>//FOLDUP
	template < class A_, class B_, class C_ >
	class Triplet
	{
		public:
			typedef A_ first_type;
			typedef B_ second_type;
			typedef C_ third_type;

			first_type first;
			second_type second;
			third_type third;

			//ctors
			Triplet(const Triplet &t) : 
				first(t.first), second(t.second), third(t.third) 
				{ CARP( GRIPE_CDTORS, "triplet ctor copy called\n") };
			Triplet(const first_type &a, const second_type &b, const third_type &c) : 
				first(a), second(b), third(c) 
				{ CARP( GRIPE_CDTORS, "triplet ctor three kind called\n") };

			//dtor
			~Triplet(void) 
				{ CARP( GRIPE_CDTORS, "triplet dtor called\n") };

			//ops
			Triplet& operator=(const Triplet &t)
				{ first = t.first; second = t.second; third = t.third; };
			bool operator==(const Triplet &y)
				{ return (first == y.first) && (second == y.second) && (third == y.third); };
	};//UNFOLD

	class badCode       { public: badCode() {} };
	class NYI           { public: NYI() {} };

	//stuff with the field type
	
	//uggh. sometimes == gives false negatives for FT?
	template< class Kernel_ >
		inline bool
			FTequal(const typename Kernel_::FT &a,const typename Kernel_::FT &b);
	
	//really convert a FT to a double;
	template< class Kernel_ >
		inline double 
			FT2double(const typename Kernel_::FT &inp);

	//get abs of FT;
	template< class Kernel_ >
		typename Kernel_::FT
			absFT(const typename Kernel_::FT &inp);

	//get sqrt of FT;
	template< class Kernel_ >
		typename Kernel_::FT
			sqrtFT(const typename Kernel_::FT &inp);

	//polynomial solving//FOLDUP
	
	//solve a0 + a1t + a2t^2 = 0 between lo and hi
	//return whether a root was found in there
	//(returns false if discriminant is negative).
	//returns root in t if returns true.
	template< class Kernel_ >
		bool     hasroot(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, 
										const typename Kernel_::FT &lo, const typename Kernel_::FT &hi,
										typename Kernel_::FT &t);

	//solve a0 + a1t + a2t^2 = 0 iteratively
	//from the initial iterate to the given tolerance
	//spits out the value of the function at the last iterate,
	//and returns last iterate as the t.
	//yes, not the best idea, quad eqn would be better, but....
	template< class Kernel_ >
		typename Kernel_::FT
			laguerreSolve(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, 
										typename Kernel_::FT &t, const typename Kernel_::FT &eps, 
										const unsigned int &maxits);
	
	//solve a0 + a1t + a2t^2 + a3t^3 = 0 iteratively
	//from the initial iterate to the given tolerance
	//spits out the value of the function at the last iterate,
	//and returns last iterate as the t.
	template< class Kernel_ >
		typename Kernel_::FT
			laguerreSolve(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, const typename Kernel_::FT &a3,
										typename Kernel_::FT &t, const typename Kernel_::FT &eps, 
										const unsigned int &maxits);

	//solve a0 + a1t + a2t^2 + a3t^3 + a4t^4 = 0 iteratively
	//from the initial iterate to the given tolerance
	//spits out the value of the function at the last iterate,
	//and returns last iterate as the t.
	template< class Kernel_ >
		typename Kernel_::FT
			laguerreSolve(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, const typename Kernel_::FT &a3,
									  const typename Kernel_::FT &a4, 
										typename Kernel_::FT &t, const typename Kernel_::FT &eps, 
										const unsigned int &maxits);

	//solve a0 + a1t + a2t^2 + a3t^3 + a4t^4 + a5t^5 = 0 iteratively
	//from the initial iterate to the given tolerance
	//spits out the value of the function at the last iterate,
	//and returns last iterate as the t.
	template< class Kernel_ >
		typename Kernel_::FT
			laguerreSolve(const typename Kernel_::FT &a0, const typename Kernel_::FT &a1,
									  const typename Kernel_::FT &a2, const typename Kernel_::FT &a3,
									  const typename Kernel_::FT &a4, const typename Kernel_::FT &a5,
										typename Kernel_::FT &t, const typename Kernel_::FT &eps, 
										const unsigned int &maxits);

	//solve a0 + a1t + a2t^2 + a3t^3 + a4t^4 + a5t^5 + a6t^6 = 0 iteratively
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
										const unsigned int &maxits);
//UNFOLD

	//geometry stuff
	
	//scalar cross product: a x b . k
	template< class Kernel_ >
		typename Kernel_::FT
			scalar_cross(const typename Kernel_::Vector_2 &a,
									 const typename Kernel_::Vector_2 &b);

	//collinearity testing is a problem with CGAL?
	
	//points on a horizontal line, i.e. same y coordinate
	template< class Kernel_ >
		bool is_horizontal_collinear(const typename Kernel_::Point_2 &a,
																 const typename Kernel_::Point_2 &b,
																 const typename Kernel_::Point_2 &c);
	//points on a vertical line, i.e. same x coordinate
	template< class Kernel_ >
		bool   is_vertical_collinear(const typename Kernel_::Point_2 &a,
																 const typename Kernel_::Point_2 &b,
																 const typename Kernel_::Point_2 &c);
	//points are collinear i.e. same x coordinate, or same y coordinate, or
	//cgal::collineaer
	template< class Kernel_ >
		bool   is_collinear(const typename Kernel_::Point_2 &a,
												const typename Kernel_::Point_2 &b,
												const typename Kernel_::Point_2 &c);

	//get max and min angle of triangle; bool value returns
	//whether the triangle is degenerate. 
	template< class Kernel_ >
		bool min_max_angle(double &min_deg,double &max_deg,
											 const typename Kernel_::Point_2 &a,
											 const typename Kernel_::Point_2 &b,
											 const typename Kernel_::Point_2 &c);

	//the points a,b,c are supposed to be collinear.  so 
	//shift point c by a small amount to make orthogonal and
	//collinear if necessary.
	template< class Kernel_ >
		bool shift_orthogonal(const typename Kernel_::Point_2 &a,
												const typename Kernel_::Point_2 &b,
												typename Kernel_::Point_2 &c);

	//scale a point about another by ratio;
	template< class Kernel_ >
		typename Kernel_::Point_2 
			scale_by_ratio(const typename Kernel_::Point_2 &anchor, 
							  const typename Kernel_::Point_2 &preimage,
							  const typename Kernel_::FT ratio);

	//scale a point about another by sqrt(ratio);
	template< class Kernel_ >
		typename Kernel_::Point_2 
			scale_by_ratio_sqrt(const typename Kernel_::Point_2 &anchor, 
							  const typename Kernel_::Point_2 &preimage,
							  const typename Kernel_::FT ratio);

	//scale a point to sqrt distance 
	//that is make a point along line from anchor to preimage
	//with distance sqrt(dist_sqrd) from anchor
	template< class Kernel_ >
		typename Kernel_::Point_2 
			scale_to_distance(const typename Kernel_::Point_2 &anchor, 
							  const typename Kernel_::Point_2 &preimage,
							  const typename Kernel_::FT dist_sqrd);

	//mod between 0 and 2pi
	template< class Kernel_ >
		typename Kernel_::FT
			mod_twopi(const typename Kernel_::FT &inp);

	//get angle to x-axis;
	//between 0 and 2pi
	template< class Kernel_ >
		typename Kernel_::FT
			angle_to_xaxis(const typename Kernel_::Vector_2 &thev);

	//get angle to x-axis;
	//between 0 and 2pi
	template< class Kernel_ >
		typename Kernel_::FT
			angle_to_xaxis(const typename Kernel_::Direction_2 &thed);

	//return counterclockwise angle abc
	//between 0 and 2pi
	template< class Kernel_ >
		typename Kernel_::FT
			ccw_angle(const typename Kernel_::Point_2 &a,
								const typename Kernel_::Point_2 &b,
								const typename Kernel_::Point_2 &c);

	//return angle abc
	//between 0 and pi
	template< class Kernel_ >
		typename Kernel_::FT
			abs_angle(const typename Kernel_::Point_2 &a,
								const typename Kernel_::Point_2 &b,
								const typename Kernel_::Point_2 &c);

	//return true if points are in different half spaces
	//defined by the line
	template< class Kernel_ >
		bool in_different_half_spaces(const typename Kernel_::Line_2 &cl,
																	const typename Kernel_::Point_2 &a,
																	const typename Kernel_::Point_2 &b);

	//get bisector line of angle abc;
	//if abc are collinear, return the bisector of a & c
	template< class Kernel_ >
		typename Kernel_::Line_2 
			bisector(const typename Kernel_::Point_2 &a, 
							 const typename Kernel_::Point_2 &b,
							 const typename Kernel_::Point_2 &c);

	//return point on segment between two center points and
	//on the radical axis;
	template< class Kernel_ >
		typename Kernel_::Point_2
			radax_vertex(const typename Kernel_::Point_2 &center_1,
								   const typename Kernel_::Point_2 &center_2,
									 const typename Kernel_::FT radius_sqrd_1,
									 const typename Kernel_::FT radius_sqrd_2);

	//return radical axis of the two circles
	template< class Kernel_ >
		typename Kernel_::Line_2
			radax(const typename Kernel_::Point_2 &center_1,
					  const typename Kernel_::Point_2 &center_2,
						const typename Kernel_::FT radius_sqrd_1,
						const typename Kernel_::FT radius_sqrd_2);

	//return radical axis of the two points, i.e. circles of zero radius
	template< class Kernel_ >
		typename Kernel_::Line_2
			radax(const typename Kernel_::Point_2 &center_1,
					  const typename Kernel_::Point_2 &center_2);

	//return number of intersections of circle and 
	//line, with the intersections put into the two points.
	template< class Kernel_ >
		int intersections(const typename Kernel_::Point_2 &center,
											const typename Kernel_::FT radius_sqrd,
											const typename Kernel_::Line_2 &the_line,
											typename Kernel_::Point_2 & point_1,
											typename Kernel_::Point_2 & point_2);

	//return number of intersections of two circles and 
	//with the intersections put into the two points.
	template< class Kernel_ >
		int intersections(const typename Kernel_::Point_2 &center_1,
											const typename Kernel_::Point_2 &center_2,
											const typename Kernel_::FT radius_sqrd_1,
											const typename Kernel_::FT radius_sqrd_2,
											typename Kernel_::Point_2 & point_1,
											typename Kernel_::Point_2 & point_2);

	//return linear combination a(1-lambda) + b(lambda);
	//i.e. a + vector(a,b) * lambda 
	//FIX: dumb to have two such functions...
	template< class Kernel_ >
		typename Kernel_::Point_2 
		linear_combination(const typename Kernel_::Point_2 &a,
											 const typename Kernel_::Point_2 &b,
											 const typename Kernel_::FT lambda)
	{ return scale_by_ratio< Kernel_ >(a,b,lambda); };

	//return intersection of segment ac with bisector of abc
	template< class Kernel_ >
		typename Kernel_::Point_2 
		bisector_cevian(const typename Kernel_::Point_2 &a,
										const typename Kernel_::Point_2 &b,
										const typename Kernel_::Point_2 &c);



}

//yes, this is the dumbest shit ever, no i am not kidding, yes
//you have to include the template definition if you want this
//to work on the 2004 state of the art c++ compilers.

#include "utility.cpp"

#endif //UTILITY_H
