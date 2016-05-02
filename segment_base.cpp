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
 * $Id: segment_base.cpp,v 1.26 2005/03/11 02:04:53 spav Exp $
 * $Log: segment_base.cpp,v $
 * Revision 1.26  2005/03/11 02:04:53  spav
 * something wrong with cubics?
 *
 * Revision 1.25  2005/02/19 00:02:49  spav
 * looks like cubic bezier is working
 *
 * Revision 1.24  2005/02/15 23:51:55  spav
 * still working on bez_3
 *
 * Revision 1.23  2005/02/12 01:37:16  spav
 * fixed xfig read bug.
 *
 * Revision 1.21  2005/01/18 22:47:11  spav
 * changing provoker to runtime option.
 *
 * Revision 1.20  2005/01/04 23:54:46  spav
 * some tweaks.
 *
 * Revision 1.19  2005/01/03 03:59:54  spav
 * home from san fran.
 *
 * Revision 1.18  2004/12/21 22:42:57  spav
 * looks like it is working; added command line parser.
 *
 * Revision 1.17  2004/12/08 01:36:53  spav
 * looking good. have segment uniqueness problem, tho. have finished curvature split grooming problem?
 *
 * Revision 1.16  2004/12/06 03:54:19  spav
 * added more stuff
 *
 * Revision 1.15  2004/12/04 02:50:26  spav
 * straight, arc works. adding bezier.
 *
 * Revision 1.14  2004/12/02 23:59:35  spav
 * problems with queue ordering...
 *
 * Revision 1.13  2004/12/02 02:48:08  spav
 * bezier 3 coming along.
 *
 * Revision 1.12  2004/12/01 00:02:47  spav
 * seems to work for arcs. start to add bezier.
 *
 * Revision 1.11  2004/11/29 07:05:04  spav
 * post thanksgiving
 *
 * Revision 1.10  2004/11/23 23:43:31  spav
 * appears to work for straight segs. now add arcs, bezier.
 *
 * Revision 1.9  2004/11/22 23:31:32  spav
 * seems to work, but problem with degenerate chull edge/faces
 *
 * Revision 1.8  2004/11/18 02:48:21  spav
 * syntactically ok; a semantic mess past groom..
 *
 * Revision 1.7  2004/11/17 00:04:38  spav
 * it compiles in this crippled form.
 *
 * Revision 1.6  2004/11/09 23:58:25  spav
 * slow going. have to rewrite how the endtable works?
 *
 * Revision 1.5  2004/11/02 23:51:20  spav
 * checking in.
 *
 * Revision 1.4  2004/10/28 00:21:17  spav
 *
 * fixing up stuff.
 *
 * Revision 1.3  2004/10/26 23:48:05  spav
 * gaining momentum./
 *
 * Revision 1.2  2004/10/23 00:38:58  spav
 * checking in.
 *
 * Revision 1.1  2004/10/22 04:52:35  spav
 * pong back to you.
 *
 * Revision 1.1  2004/10/20 21:50:26  spav
 * starting code project
 *
 *
 */
//END CVSLOG
//END HEADER

#include "segment_base.h"

#ifndef SEGMENT_BASE_CPP
#define SEGMENT_BASE_CPP

#define SLEPS 0.0000000000000001
#define SMALLEPS 0.00000000000000003
#define MAXITS_QUAD 4
#define MAXITS_CUB 15 

namespace lopping { 

////////////////////////////////////////////////////////////////////////
//straight segs//FOLDUP
	//split straight at distance//FOLDUP
	template< class Kernel_ >
		typename Straight_segment<Kernel_>::Point 
			Straight_segment<Kernel_>::split_point(const FT &cut_distance_squared,
																						 const bool &cut_at_first) const
	{
		FT min_dist(0);
		FT point_dist = diameter_squared();

		CARP( GRIPE_INFO, \
						 "? " << lopping::FT2double< Kernel_ >(min_dist) << " < " << \
						 lopping::FT2double< Kernel_ >(cut_distance_squared) << " < " << \
						 lopping::FT2double< Kernel_ >(point_dist)  << "?\n")

		CGAL_precondition( min_dist < cut_distance_squared );
		CGAL_precondition( cut_distance_squared < point_dist );

		FT cut_ratio = 
			lopping::sqrtFT< Kernel_ > ( (cut_distance_squared / point_dist) );

		Point midpnt = (cut_at_first)? 
			( p1 + (p2 - p1) * cut_ratio ):( p2 + (p1 - p2) * cut_ratio );

		//FIX: danger!
		lopping::shift_orthogonal< Kernel_ >(p1,p2,midpnt);
		
#if PARANOID 
		if (! lopping::is_collinear< Kernel_ >(p1,p2,midpnt))
		{
			CARP( CHECK_MATH, \
							 "noncollinearity in distance cut segment split?\n")
		}
#endif

		return midpnt;
	};//UNFOLD
	//split straight with provoker//FOLDUP
	template< class Kernel_ >
		typename Straight_segment<Kernel_>::Point 
			Straight_segment<Kernel_>::split_point(const Point &p) const
	{
		if (Use_Provoker)
		{

#if PARANOID 
			if (lopping::is_collinear< Kernel_ >(p1,p2,p))
			{
				CARP( CHECK_MATH, "provoker point is collinear?\n")
			}
#endif
	
			typename Kernel_::Segment_2 pseg(p1,p2);
			typename Kernel_::Line_2 biline = lopping::bisector<Kernel_>(p1,p,p2);
	
			//typename CGAL::Object iobj = CGAL::intersection(biray,pseg);
			typename CGAL::Object iobj = CGAL::intersection(pseg,biline);
			Point midpnt;
	
			if (CGAL::assign(midpnt,iobj))
			{
				CARP( GRIPE_INFO, "provoker does give intersection!\n")
	
			//FIX: danger!
			lopping::shift_orthogonal< Kernel_ >(p1,p2,midpnt);
			
#if PARANOID 
			if (! lopping::is_collinear< Kernel_ >(p1,p2,midpnt))
			{
				CARP( CHECK_MATH, \
								 "noncollinearity in provoked segment split?\n")
			}
			if (! pseg.has_on(midpnt))
			{
				CARP( CHECK_MATH, \
								 "midpnt is not considered on the segment?\n")
			}
			if (! biline.has_on(midpnt))
			{
				CARP( CHECK_MATH, \
								 "midpnt is not considered on the line either?\n")
			}
#endif
				return midpnt;
			} else if (CGAL::assign(pseg,iobj))
			{
				CARP( GRIPE_INFO, "provoker point is collinear?\n")
				return midpoint();
			} else
			{
				CARP( GRIPE_INFO, "provoker gives no intersection?\n")
				CARPP( GRIPE_INFO, p1)
				CARPP( GRIPE_INFO, p2)
				CARPP( GRIPE_INFO, p)
				return midpoint();
			} 
			return midpnt;
		} 

		//else
		//do not use provoker
		return midpoint();
	};//UNFOLD
	//Dtor//FOLDUP
	template< class Kernel_ >
		Segment_base<Kernel_>::~Segment_base() {};
//UNFOLD
//UNFOLD
////////////////////////////////////////////////////////////////////////
//arc segs//FOLDUP
	//four point ctor;//FOLDUP
	template< class Kernel_ >
		Arc_segment<Kernel_>::Arc_segment(const Point &point1,const Point &middling_point,
																			const Point &point2,const Point &cen) :
			Straight_segment< Kernel_ >(point1,point2), center(cen)
	{
		//figure out if is_cw;
		typename Kernel_::Line_2 endline(point1,point2);
		switch (endline.oriented_side(middling_point)) {
			case CGAL::ON_NEGATIVE_SIDE: is_cw = false; break;
			case CGAL::ON_POSITIVE_SIDE: is_cw = true; break;
			default : //ERROR?
				is_cw = false;
		}
	};//UNFOLD
	//find intersections with a line//FOLDUP
	template< class Kernel_ >
		int Arc_segment<Kernel_>::intersections(const typename Kernel_::Line_2 &the_line,
												Point & point_1, Point & point_2) const
	{
		FT avg_rad = radius_sqrd();
		//only bonk point_1,point_2 if they intersect the _arc_, not just 
		//the circle
		Point lp_1,lp_2;
		int num_ints = 
			lopping::intersections<Kernel_>(center,avg_rad,the_line,lp_1,lp_2);

		if (!num_ints) { return 0; }
		if (num_ints == 1)
		{
			if (_is_between(lp_1)) { point_1 = lp_1; return 1; }
			else { return 0; }
		};

		num_ints = 0;
		if (_is_between(lp_1)) { point_1 = lp_1; ++num_ints; } 
		if (_is_between(lp_2)) { 
			if (num_ints) { point_2 = lp_2; }
			else { point_1 = lp_2; }
			++num_ints; }
		return num_ints;
	};//UNFOLD
	//split arc with void//FOLDUP
	template< class Kernel_ >
		typename Arc_segment<Kernel_>::Point 
			Arc_segment<Kernel_>::split_point(void) const
	{
		typename Kernel_::Line_2 biline = lopping::bisector<Kernel_>(p1,center,p2);
		Point lp_1,lp_2;
		int num_ints = intersections(biline,lp_1,lp_2);
		if (num_ints) { return lp_1; }

		CARP( GRIPE_INFO, \
						 "void arc split: bisector should intersect the arc!\n")
		return center;
	};//UNFOLD
	//split arc with a provoker//FOLDUP
	template< class Kernel_ >
		typename Arc_segment<Kernel_>::Point 
			Arc_segment<Kernel_>::split_point(const Point &p) const
	{ 
		if (Use_Provoker)
		{
			//first get the bisector to p1 p p2
			typename Kernel_::Line_2 biline = lopping::bisector<Kernel_>(p1,p,p2);
			Point lp_1,lp_2;
			int num_ints = intersections(biline,lp_1,lp_2);
			if (num_ints) { return lp_1; }
	
			CARP( GRIPE_INFO, \
							 "provoked arc split: bisector should intersect the arc!\n")
			return split_point();
		}
		//else
		//do not use provoker
		return split_point();
	};//UNFOLD
	//split arc at distance//FOLDUP
	template< class Kernel_ >
		typename Arc_segment<Kernel_>::Point 
			Arc_segment<Kernel_>::split_point(const FT &cut_distance_squared,
																						 const bool &cut_at_first) const
	{
		FT min_dist(0);
		FT point_dist = diameter_squared();

		CARP( GRIPE_INFO, \
						 "? " << lopping::FT2double< Kernel_ >(min_dist) << " < " << \
						 lopping::FT2double< Kernel_ >(cut_distance_squared) << " < " << \
						 lopping::FT2double< Kernel_ >(point_dist)  << "?\n")

		CGAL_precondition( min_dist < cut_distance_squared );
		CGAL_precondition( cut_distance_squared < point_dist );

		//find the radical axis of two circles.
		//one at center through p1&p2;
		//the other through cen_2 of radius cut_dist_sqrd
		Point cen_2 = (cut_at_first)? p1:p2;
		FT avg_rad = radius_sqrd();
		typename Kernel_::Line_2 radline = 
			lopping::radax< Kernel_ >(center,cen_2,avg_rad,cut_distance_squared);

		Point lp_1,lp_2;
		int num_ints = intersections(radline,lp_1,lp_2);
		if (num_ints) { return lp_1; }

		CARP( GRIPE_INFO, "radax should intersect the arc!\n")
		return split_point();
	};//UNFOLD
	//estimate curvature//FOLDUP
	template< class Kernel_ >
		typename Arc_segment<Kernel_>::FT
			Arc_segment<Kernel_>::estimate_total_curvature(void) const
	{
		FT retval;
		if (is_cw)
		{
			retval = lopping::ccw_angle<Kernel_>(p2,center,p1);

			CARP( GRIPE_INFO, \
							 "etcurv gives: " << lopping::FT2double< Kernel_ >(retval) << "\n")
			return lopping::ccw_angle<Kernel_>(p2,center,p1);
		}

		retval = lopping::ccw_angle<Kernel_>(p1,center,p2);
		CARP( GRIPE_INFO, \
						 "etcurv gives: " << lopping::FT2double< Kernel_ >(retval) << "\n")
		return lopping::ccw_angle<Kernel_>(p1,center,p2);
	};//UNFOLD
	//direction at end//FOLDUP
	template< class Kernel_ >
		typename Arc_segment<Kernel_>::Direction
			Arc_segment<Kernel_>::end_tangent(const int & which_end) const
	{
		Vector v_out;
		if (which_end % 2) 
		{ 
			v_out = Vector(center,p1);
			v_out = v_out.perpendicular( ((is_cw)? CGAL::RIGHT_TURN : CGAL::LEFT_TURN ) );
		} else 
		{
			v_out = Vector(center,p2);
			v_out = v_out.perpendicular( ((is_cw)? CGAL::LEFT_TURN : CGAL::RIGHT_TURN ) );
		}
		return Direction(v_out);
	};//UNFOLD
	//return true if the ray from center to check_p intersects the arc.//FOLDUP
	template< class Kernel_ >
		bool
			Arc_segment<Kernel_>::_is_between(const Point &check_p) const
	{
		FT p1_tox = lopping::angle_to_xaxis<Kernel_>(Vector(center,p1));
		FT p2_tox = lopping::angle_to_xaxis<Kernel_>(Vector(center,p2));
		FT p_tox = lopping::angle_to_xaxis<Kernel_>(Vector(center,check_p));

		if (is_cw)
		{
			if (p2_tox > p1_tox)
				{ return ((p1_tox > p_tox) || (p2_tox < p_tox)); } 
			else
				{ return ((p1_tox > p_tox) && (p2_tox < p_tox)); }
		}
		//else
		
		if (p1_tox > p2_tox)
			{ return ((p2_tox > p_tox) || (p1_tox < p_tox)); } 
		else
			{ return ((p2_tox > p_tox) && (p1_tox < p_tox)); }
	};//UNFOLD

//UNFOLD
////////////////////////////////////////////////////////////////////////
//bezier_2//FOLDUP
	//split bezier_2 with void//FOLDUP
	template< class Kernel_ >
		typename Bezier_2<Kernel_>::Point 
			Bezier_2<Kernel_>::split_point(Split_hints &p_loc) const
	{ 
		p_loc = _bisectors_lambda();
		return blossom(p_loc);
	};//UNFOLD
	//split bezier_2 with provoker//FOLDUP
	template< class Kernel_ >
		typename Bezier_2<Kernel_>::Point 
			Bezier_2<Kernel_>::split_point(const Point &p, Split_hints &p_loc) const
	{ 
		if (Use_Provoker)
		{
			typename Kernel_::Line_2 biline = lopping::bisector<Kernel_>(p1,p,p2);
			p_loc = _line_intersection_lambda(biline);
			//FIX: i changed this. does it screw things up?
			//return split_point(p_loc);
			return blossom(p_loc);
		}
		//else
		//do not use provoker;
		return split_point(p_loc);
	};//UNFOLD
	//find the midpoint at a given distance from one endpoint//FOLDUP
	template< class Kernel_ >
		typename Bezier_2<Kernel_>::Point 
			Bezier_2<Kernel_>::split_point(const FT &cut_distance_squared,
																		 const bool &cut_at_first,
																		 Split_hints &p_loc) const
	{ 
		//set up the problem
		typename Kernel_::Vector_2 v1(p1,p15);
		typename Kernel_::Vector_2 v2(p15,p2);
		typename Kernel_::Vector_2 vdif = v2-v1;

		FT alpha,beta,gamma;

		alpha = vdif * vdif;

		if (cut_at_first)
		{
			beta  = FT(4.0) * ( v1 * vdif );
			gamma = FT(4.0) * ( v1 * v1 );
		} else
		{
			beta  = FT(4.0) * ( v2 * vdif );
			gamma = FT(4.0) * ( v2 * v2 );
		}

		//now use newton's method (or laguerre's algorithm) 
		//to find a root to
		//f(t) = t^2 [ alpha t^2 + beta t + gamma ] - cut_dist_squared

		//FIX: use something `better'?
		//initial guess;
		FT t = FT(0.5);
		const unsigned int maxits = MAXITS_QUAD;
		FT epsi(SMALLEPS);

		lopping::laguerreSolve<Kernel_>(- cut_distance_squared, FT(0.0), gamma, beta, alpha,
													 t, epsi, maxits);

		if (cut_at_first)
		{ p_loc = t; } else
		{ p_loc = FT(1.0) - t; }
		
		return blossom(p_loc);
	};//UNFOLD
	//find something like total variation halfway point//FOLDUP
	template< class Kernel_ >
		typename Bezier_2<Kernel_>::Point 
			Bezier_2<Kernel_>::tv_split_point(Split_hints &p_loc) const
	{ 
		typename Kernel_::Vector_2 v2(p15,p2);
		Point ghost_2 = p1 + v2;
		Point l_point = lopping::bisector_cevian<Kernel_>(ghost_2,p1,p15);
		FT rat_sqrd = 
			(CGAL::squared_distance(p15,l_point)) / 
			(CGAL::squared_distance(p15,ghost_2));

		p_loc = lopping::sqrtFT< Kernel_ >(rat_sqrd);
		return blossom(p_loc);
	};//UNFOLD
	//ctor from a parent and new midpoint//FOLDUP
	template< class Kernel_ >
		Bezier_2<Kernel_>::Bezier_2(const Bezier_2 &parent, const Point &p,
																const bool &use_first_endpoint,
																const Split_hints &p_loc) :
				Straight_segment< Kernel_ >(parent,p,use_first_endpoint)
		{
			Point dummy1,dummy2;
			parent.blossom(p_loc,dummy1,dummy2);
			p15 = ((use_first_endpoint)? dummy1:dummy2);
		};//UNFOLD
	//estimate curvature//FOLDUP
	template< class Kernel_ >
		typename Bezier_2<Kernel_>::FT
			Bezier_2<Kernel_>::estimate_total_curvature(void) const
	{ return FT(lopping::PI) - lopping::abs_angle< Kernel_ >(p1,p15,p2);
	};//UNFOLD
	//return the point on the bezier curve for the given t.//FOLDUP
	//also set the linear combinations of p1,p15 and p15,p2
	template< class Kernel_ >
		typename Bezier_2<Kernel_>::Point 
			Bezier_2<Kernel_>::blossom(const FT &t,Point &p115,Point &p152) const
	{
		p115 = lopping::linear_combination< Kernel_ >(p1,p15,t);
		p152 = lopping::linear_combination< Kernel_ >(p15,p2,t);
		return lopping::linear_combination< Kernel_ >(p115,p152,t);
	};//UNFOLD
	//give the lambda of the point on the bisector of p1,p2//FOLDUP
	template< class Kernel_ >
		typename Bezier_2<Kernel_>::FT
			Bezier_2<Kernel_>::_bisectors_lambda(void) const
	{ 
		typename Kernel_::Vector_2 v1(p1,p15);
		typename Kernel_::Vector_2 v2(p15,p2);
		typename Kernel_::Vector_2 vsum = v1+v2;
		typename Kernel_::Vector_2 vdif = v2-v1;

		FT alpha = FT(2.0) * vdif * vsum;
		FT beta  = FT(4.0) * v1 * vsum;
		FT gamma = - vsum * vsum;

		//now solve t^2 + beta t + gamma = 0
		FT epsi(SMALLEPS);
		FT t;
		if (lopping::hasroot< Kernel_ >(gamma,beta,alpha,epsi,FT(1.0)-epsi,t))
		{
			return t;
		} else
		{ return FT(0.0); }
	};//UNFOLD
	//give the lambda of the point on the given line//FOLDUP
	template< class Kernel_ >
		typename Bezier_2<Kernel_>::FT
			Bezier_2<Kernel_>::_line_intersection_lambda(const Line &the_line) const
	{ 
		typename Kernel_::Vector_2 v1(p1,p15);
		typename Kernel_::Vector_2 v2(p15,p2);
		typename Kernel_::Vector_2 vdif = v2-v1;

		typename Kernel_::Vector_2 ab(the_line.a(),the_line.b());

		FT alpha = ab * vdif;
		FT beta  = FT(2.0) * ab * v1;
		FT gamma = the_line.a() * p1.x() + the_line.b() * p1.y() + the_line.c();

		//now solve alpha t^2 + beta t + gamma = 0
		FT epsi(SMALLEPS);
		FT t;
		if (lopping::hasroot< Kernel_ >(gamma,beta,alpha,epsi,FT(1.0)-epsi,t))
		{
			return t;
		} else
		{ return FT(0.0); }
	};//UNFOLD
//UNFOLD
////////////////////////////////////////////////////////////////////////
//bezier_3//FOLDUP
	//split bezier_3 with void//FOLDUP
	template< class Kernel_ >
		typename Bezier_3<Kernel_>::Point 
			Bezier_3<Kernel_>::split_point(Split_hints &p_loc) const
	{ 
		p_loc = _bisectors_lambda();
		return blossom(p_loc);
	};//UNFOLD
	//split bezier_3 with provoker//FOLDUP
	template< class Kernel_ >
		typename Bezier_3<Kernel_>::Point 
			Bezier_3<Kernel_>::split_point(const Point &p, Split_hints &p_loc) const
	{ 
		if (Use_Provoker)
		{
			//FIX?
			typename Kernel_::Line_2 biline = lopping::bisector<Kernel_>(p1,p,p2);
			p_loc = _line_intersection_lambda(biline);
			return blossom(p_loc);
		}
		//else
		//do not use provoker;
		return split_point(p_loc);
	};//UNFOLD
	//find the midpoint at a given distance from one endpoint//FOLDUP
	template< class Kernel_ >
		typename Bezier_3<Kernel_>::Point 
			Bezier_3<Kernel_>::split_point(const FT &cut_distance_squared,
																		 const bool &cut_at_first,
																		 Split_hints &p_loc) const
	{ 
		//assume that total curvature is small enough,
		//and cut distance is small enough that there
		//is a unique solution.
		//FIX: is it done?
		//set up the problem
		typename Kernel_::Vector_2 v1,v2,v3;
		if (cut_at_first)
		{
			v1 = p133 - p1;
			v2 = p166 - p133;
			v3 = p2 - p166;
		} else
		{
			v1 = p166 - p2;
			v2 = p133 - p166;
			v3 = p1 - p133;
		}
		
		typename Kernel_::Vector_2 w12 = v2 - v1;
		typename Kernel_::Vector_2 w13 = v3 - v1;
		typename Kernel_::Vector_2 w23 = v3 - v2;
		typename Kernel_::Vector_2 wmix = w23 - w12;

		FT alpha,beta,gamma,delta,eta;

		//error?
//		alpha = FT(2.0) * (w12 * w12) + FT(2.0) * (w23 * w23) - (w13*w13);
//		beta  = FT(-6.0) * ( (w12*w12) + (w12*w23) );
//		gamma = FT(9.0) * (w12*w12) + FT(6.0) * ( (v1*w23) - (v1*w12) );
//		delta = FT(18.0) * (v1 * w12);
//		eta   = FT(9.0) * (v1 * v1);

		//new stuff, also with error?
//		alpha = (w12 * w12) + (w23 * w23) - (v2*v2) + FT(2.0) * (v1*v3);
//		beta  = FT(-6.0) * ( w12 * (w12 + v3) );
//		gamma = FT(9.0) * (w12*w12) + FT(6.0) * ( v1 * (v3 - w12) );
//		delta = FT(18.0) * (v1 * w12);
//		eta   = FT(9.0) * (v1 * v1);

		//using maple:
//		alpha = FT(2.0) * ( (w12 * w12) + (w23 * w23) ) - (w13*w13);
//		beta  = FT(6.0) * ( w12 * ( w23 - w12 ) );
//		gamma = FT(9.0) * (w12*w12) + FT(6.0) * ( v1 * (w23 - w12) );
//		delta = FT(18.0) * (v1 * w12);
//		eta   = FT(9.0) * (v1 * v1);

		//using maple again:
		alpha = wmix * wmix;
		beta  = FT(6.0) * ( w12 * wmix );
		gamma = FT(9.0) * (w12*w12) + FT(6.0) * ( v1 * wmix );
		delta = FT(18.0) * (v1 * w12);
		eta   = FT(9.0) * (v1 * v1);

		//now use newton's method (or laguerre's algorithm) 
		//to find a root to
		//f(t) = t^2 [ alpha t^4 + beta t^3 + gamma t^2 + delta t + eta] - cut_dist_squared
		
		//FIX: use something `better'?
		//initial guess;
		//FT t = FT(0.5);
		//make guess based on distance from p1 to p2.
		//this assumes tau is relatively small (i.e. the curve is relatively straight)
		FT t =  lopping::sqrtFT<Kernel_>( cut_distance_squared / ((p1 - p2) * (p1 - p2)) );
		VOICE( GRIPE_INFO, "t is " << t << "\n" )

		const unsigned int maxits = MAXITS_CUB;
		FT epsi(SMALLEPS);

		lopping::laguerreSolve<Kernel_>( (-cut_distance_squared), FT(0.0), 
													 eta, delta, gamma, beta, alpha,
													 t, epsi, maxits);

		VOICE( GRIPE_INFO, "t is " << t << "\n" )
		CGAL_precondition( FT(0.0) <= t );
		CGAL_precondition( FT(1.0) >= t );

		if (cut_at_first)
		{ p_loc = t; } else
		{ p_loc = FT(1.0) - t; }
		
		return blossom(p_loc);
	};//UNFOLD
	//return true if it has an inflection point.//FOLDUP
	//and if so, give it in p_loc
	template< class Kernel_ >
		bool Bezier_3<Kernel_>::has_inflection_point(Split_hints &p_loc) const
	{ 
		typename Kernel_::Line_2 midline(p133,p166);

		CARP( GRIPE_INFO, "has_inflection_point: \n" )
		CARPP( GRIPE_INFO, p1)
		CARPP( GRIPE_INFO, p133)
		CARPP( GRIPE_INFO, p166)
		CARPP( GRIPE_INFO, p2)
		if (lopping::in_different_half_spaces< Kernel_ >(midline,p1,p2))
		{
			typename Kernel_::Vector_2 v01(p1,p133);
			typename Kernel_::Vector_2 v12(p133,p166);
			typename Kernel_::Vector_2 v23(p166,p2);

			typename Kernel_::Vector_2 v012 = v12 - v01;
			typename Kernel_::Vector_2 v123 = v23 - v12;

			typename Kernel_::Vector_2 v0123 = v123 - v012;


			FT a0 = lopping::scalar_cross< Kernel_ >(v01,v12);
			FT a1 = lopping::scalar_cross< Kernel_ >(v012,v12) +
							lopping::scalar_cross< Kernel_ >(v01,v123);
			FT a2 = lopping::scalar_cross< Kernel_ >(v012,v123);

			//now solve a0 + a1 t + a2 t^2 = 0
			FT epsi(SMALLEPS);
			return (lopping::hasroot< Kernel_ >(a0,a1,a2,epsi,FT(1.0)-epsi,p_loc));
		} //else
		return false;
	};//UNFOLD
	//find something like total variation halfway point//FOLDUP
	//assumes the bez_3 has no inflection point.
	template< class Kernel_ >
		typename Bezier_3<Kernel_>::Point 
			Bezier_3<Kernel_>::tv_split_point(Split_hints &p_loc) const
	{ 
		typename Kernel_::Vector_2 v1(p133,p166);
		typename Kernel_::Vector_2 v2(p166,p2);

		Bezier_2<Kernel_> tang_bez(p133,p1 + v1,p1 + v2);
		typename Kernel_::Line_2 biline = 
			lopping::bisector<Kernel_>(tang_bez.p1,p1,tang_bez.p2);
		p_loc = tang_bez._line_intersection_lambda(biline);
		return blossom(p_loc);
	};//UNFOLD
	//ctor from a parent and new midpoint//FOLDUP
	template< class Kernel_ >
		Bezier_3<Kernel_>::Bezier_3(const Bezier_3 &parent, const Point &p,
																const bool &use_first_endpoint,
																const Split_hints &p_loc) :
				Straight_segment< Kernel_ >(parent,p,use_first_endpoint)
		{
			Point p1133,p133166,p1662,p1133166,p1331662;
			parent.blossom(p_loc,p1133,p133166,p1662,p1133166,p1331662);
			if (use_first_endpoint)
			{ p133 = p1133;p166 = p1133166;
			} else
			{ p133 = p1331662;p166 = p1662;
			}
		};//UNFOLD
	//estimate curvature//FOLDUP
	//assumes the bez_3 has no inflection point.
	template< class Kernel_ >
		typename Bezier_3<Kernel_>::FT
			Bezier_3<Kernel_>::estimate_total_curvature(void) const
	{
		FT turn1 = FT(lopping::PI) - lopping::abs_angle< Kernel_ >(p1,p133,p166);
		FT turn2 = FT(lopping::PI) - lopping::abs_angle< Kernel_ >(p133,p166,p2);
		return turn1+turn2;
	};//UNFOLD
	//return the point on the bezier curve for the given t.//FOLDUP
	//also set the linear combinations of p1,p15 and p15,p2
	template< class Kernel_ >
		typename Bezier_3<Kernel_>::Point 
			Bezier_3<Kernel_>::blossom(const FT &t,Point &p1133,Point &p133166,
																 Point &p1662,Point &p1133166,Point &p1331662) const
	{
		p1133    = lopping::linear_combination< Kernel_ >(p1,p133,t);
		p133166  = lopping::linear_combination< Kernel_ >(p133,p166,t);
		p1662    = lopping::linear_combination< Kernel_ >(p166,p2,t);

		p1133166 = lopping::linear_combination< Kernel_ >(p1133,p133166,t);
		p1331662 = lopping::linear_combination< Kernel_ >(p133166,p1662,t);

		return lopping::linear_combination< Kernel_ >(p1133166,p1331662,t);
	};//UNFOLD
	//give the lambda of the point on the bisector of p1,p2//FOLDUP
	template< class Kernel_ >
		typename Bezier_3<Kernel_>::FT
			Bezier_3<Kernel_>::_bisectors_lambda(void) const
	{ 
		typename Kernel_::Line_2 bisector = lopping::radax< Kernel_ >(p1,p2);
		return _line_intersection_lambda(bisector);

//
//
//		typename Kernel_::Vector_2 v1(p1,p133);
//		typename Kernel_::Vector_2 v2(p133,p166);
//		typename Kernel_::Vector_2 v3(p166,p2);
//
//		typename Kernel_::Vector_2 vsum = v1+v2+v3;
//
//		//by hand. has errors?
////		FT alpha = (v0 + v1) * (v0 + v1) - (v0 * v0) * FT(2.0)
////						 + (v1 + v2) * (v1 + v2) - (v2 * v2) * FT(2.0)
////						 - (v0 + v2) * (v0 + v2) - (v1 * v1) * FT(2.0);
////		FT beta  = FT(6.0) * ( v0 * (v0 + v2) - v1 * (v1 + v2) );
////		FT gamma = FT(-3.0) * ( v0 * (v0 + (v1 * FT(2.0))) + v0 * (v0 + (v2 * FT(2.0))) );
////		FT delta = v0 * (v0 + (v1 * FT(2.0))) + 
////							 v1 * (v1 + (v2 * FT(2.0))) + 
////							 v2 * (v2 + (v0 * FT(2.0)));
//
//		FT alpha = FT(2.0) * (vsum * (v1 + v3 - v2 - v2));
//		FT beta  = FT(6.0) * (vsum * (v2 - v1));
//		FT gamma = FT(6.0) * (vsum * v1);
//		FT delta = - vsum * vsum;
//
//		//now find the solution to
//		//alpha t^3 + beta t + gamma t + delta = 0...
//		//using Laguerre's Algorithm
//
//		//FIX: use something `better'?
//		//initial guess;
//		FT t = FT(0.5);
//		const unsigned int maxits = MAXITS_CUB;
//		FT epsi(SMALLEPS);
//
//		lopping::laguerreSolve<Kernel_>(delta, gamma, beta, alpha, t, epsi, maxits);
//
//		return t;
	};//UNFOLD
	//give the lambda of the point on the given line//FOLDUP
	template< class Kernel_ >
		typename Bezier_3<Kernel_>::FT
			Bezier_3<Kernel_>::_line_intersection_lambda(const Line &the_line) const
	{ 
		typename Kernel_::Vector_2 v1(p1,p133);
		typename Kernel_::Vector_2 v2(p133,p166);
		typename Kernel_::Vector_2 v3(p166,p2);

		typename Kernel_::Vector_2 ab(the_line.a(),the_line.b());

		FT alpha = ab * (v1 - v2 + v3 - v2);
		FT beta  = (ab * (v2 - v1)) * FT(3.0);
		FT gamma = (ab * v1) * FT(3.0);
		FT delta = the_line.a() * p1.x() + the_line.b() * p1.y() + the_line.c();

		//now solve 
		//alpha t^3 + beta t^2 + gamma t + delta = 0;

		//FIX: use something `better'?
		//initial guess;
		FT t = FT(0.5);
		const unsigned int maxits = MAXITS_CUB;
		FT epsi(SMALLEPS);

		lopping::laguerreSolve<Kernel_>(delta, gamma, beta, alpha, t, epsi, maxits);

		return t;
	};//UNFOLD
//UNFOLD
////////////////////////////////////////////////////////////////////////
//wrapper segs //FOLDUP
	//set_provoker//FOLDUP
	template< class Kernel_ >
		void Segment_wrapper<Kernel_>::set_provoker(const bool &set_to) 
	{ 
		Segment_wrapper<Kernel_>::Use_Provoker = set_to;
		Straight_segment::Use_Provoker = set_to;
		Arc_segment::Use_Provoker = set_to;
		Bezier_2::Use_Provoker = set_to;
		Bezier_3::Use_Provoker = set_to;
	};//UNFOLD
	//end_point//FOLDUP
	template< class Kernel_ >
		typename Segment_wrapper<Kernel_>::Point
			Segment_wrapper<Kernel_>::end_point(const int &i) const
	{ switch (styp) { 
			case STRAIGHT :  return straight->end_point(i);break;
			case ARC : 			 return arc->end_point(i);break;
			case BEZIER_2 :  return bezier_2->end_point(i);break;
			case BEZIER_3 :  return bezier_3->end_point(i);break;
		default : break; } 
			return arc->end_point(i);
	};//UNFOLD
	//split_point(void)//FOLDUP
	template< class Kernel_ >
		typename Segment_wrapper<Kernel_>::Point
			Segment_wrapper<Kernel_>::split_point(Split_hints &p_loc) const
	{ switch (styp) { 
			case STRAIGHT :  return straight->split_point();break;
			case ARC : 			 return arc->split_point();break;
			case BEZIER_2 :  return bezier_2->split_point(p_loc);break;
			case BEZIER_3 :  return bezier_3->split_point(p_loc);break;
		default : break; } 
			return arc->split_point();
	};//UNFOLD
	//split_point(Point)//FOLDUP
	template< class Kernel_ >
		typename Segment_wrapper<Kernel_>::Point
			Segment_wrapper<Kernel_>::split_point(const Point &p,Split_hints &p_loc) const
	{ switch (styp) { 
			case STRAIGHT :  return straight->split_point(p);break;
			case ARC : 			 return arc->split_point(p);break;
			case BEZIER_2 :	 return bezier_2->split_point(p,p_loc);break;
			case BEZIER_3 :	 return bezier_3->split_point(p,p_loc);break;
		default : break; } 
			return arc->split_point(p);
	};//UNFOLD
	//split_point(FT,bool)//FOLDUP
	template< class Kernel_ >
		typename Segment_wrapper<Kernel_>::Point
			Segment_wrapper<Kernel_>::split_point(const FT &cut_distance_squared,
												const bool &cut_at_first,
												Split_hints &p_loc) const
	{ switch (styp) { 
			case STRAIGHT :  return 
				straight->split_point(cut_distance_squared,cut_at_first);break;
			case ARC : 			 return 
				arc->split_point(cut_distance_squared,cut_at_first);break;
			case BEZIER_2 :	 return 
				bezier_2->split_point(cut_distance_squared,cut_at_first,p_loc);break;
			case BEZIER_3 :	 return 
				bezier_3->split_point(cut_distance_squared,cut_at_first,p_loc);break;
		default : break; } 
		return arc->split_point(cut_distance_squared,cut_at_first);
	};//UNFOLD
	//tv_split_point(void)//FOLDUP
	//find esimated total variation midpoint
	template< class Kernel_ >
		typename Segment_wrapper<Kernel_>::Point
			Segment_wrapper<Kernel_>::tv_split_point(Split_hints &p_loc) const
	{ switch (styp) { 
			case STRAIGHT :  return 
				straight->tv_split_point();break;
			case ARC : 			 return 
				arc->tv_split_point();break;
			case BEZIER_2 :	 return 
				bezier_2->tv_split_point(p_loc);break;
			case BEZIER_3 :	 return 
				bezier_3->tv_split_point(p_loc);break;
		default : break; } 
		return arc->tv_split_point();
	};//UNFOLD
	//input constructor //FOLDUP
	template< class Kernel_ >
			Segment_wrapper<Kernel_>::Segment_wrapper(const int &num_ints, 
									const int &num_exts, Point_Iterator pnt_it) : 
									styp(STRAIGHT),straight(0) 
	{
		switch (num_ints) { 
			case 2 :
				styp = STRAIGHT;
				straight = new Straight_segment(*pnt_it,*(pnt_it + 1));
				CARP( GRIPE_INFO, "caught a straight seg (2)\n")
				break;
			case 3 :
				styp = BEZIER_2;
				bezier_2 = new Bezier_2(*pnt_it,*(pnt_it + 1),*(pnt_it + 2));
				CARP( GRIPE_INFO, "caught a quadratic bezier (3)\n")
				break;
			case 4 :
				styp = BEZIER_3;
				bezier_3 = new Bezier_3(*pnt_it,*(pnt_it + 1),*(pnt_it + 2),*(pnt_it + 3));
				CARP( GRIPE_INFO, "caught a cubic bezier (4)\n")
				break;
			case -4 :
				styp = ARC;
				arc = new Arc_segment(*pnt_it,*(pnt_it + 1),*(pnt_it + 2),*(pnt_it+3));
				CARP( GRIPE_INFO, "caught an arc (-4)\n")
				break;
		default : break;
		}
	};//UNFOLD
	//constructor from a parent and new midpoint//FOLDUP
	template< class Kernel_ >
			Segment_wrapper<Kernel_>::Segment_wrapper(const Segment_wrapper &parent, 
											const Point &p, const bool &use_first_endpoint,
											const Split_hints &p_loc) :
											styp(parent.styp)
	{
//#if PARANOID > CHECK_BIZARRE
//		GRIPE("_segment_wrapper ctor: check split point\n");
//		GRIPEP(p);
//		GRIPE("parent endpoints are:\n");
//		GRIPEP(parent.end_point(1));
//		GRIPEP(parent.end_point(2));
//		if ((parent.end_point(1) == p) || (parent.end_point(2) == p))
//		{
//			GRIPE("_segment_wrapper ctor: split point is an end point of parent!\n");
//		} else
//		{
//			GRIPE("_segment_wrapper ctor: split point seems ok\n");
//		};
//#endif
		switch (styp) { 
			case STRAIGHT :  
				straight = 
						new Straight_segment(*(parent.straight),p,use_first_endpoint);
				break;
			case ARC : 			 
				arc = new Arc_segment(*(parent.arc), p,use_first_endpoint);
				break;
			case BEZIER_2 :  
				bezier_2 = 
						new Bezier_2(*(parent.bezier_2),p,use_first_endpoint,p_loc);
				break;
			case BEZIER_3 :  
				bezier_3 = 
						new Bezier_3(*(parent.bezier_3),p,use_first_endpoint,p_loc);
				break;
		default : break;
		}
	};//UNFOLD
	//constructor from a parent and bool //FOLDUP
	template< class Kernel_ >
			Segment_wrapper<Kernel_>::Segment_wrapper(const Segment_wrapper &parent, 
																								const bool &reverse_segment) :
																								styp(parent.styp)
			{
				switch (styp) { 
					case STRAIGHT :  
						straight = 
								new Straight_segment(*(parent.straight),reverse_segment);
						break;
					case ARC : 			 
						arc = new Arc_segment(*(parent.arc),reverse_segment);
						break;
					case BEZIER_2 :
						bezier_2 = new Bezier_2(*(parent.bezier_2),reverse_segment);
						break;
					case BEZIER_3 :
						bezier_3 = new Bezier_3(*(parent.bezier_3),reverse_segment);
						break;
				default : break;
				}
			};//UNFOLD
	//Dtor//FOLDUP
	template< class Kernel_ >
			Segment_wrapper<Kernel_>::~Segment_wrapper(void)
			{ switch (styp) { 
					case STRAIGHT :  delete straight;break;
					case ARC : 			 delete arc;break;
					case BEZIER_2 :	 delete bezier_2;break;
					case BEZIER_3 :	 delete bezier_3;break;
				default : break; } 
			};//UNFOLD
	//circumradius_squared(void)//FOLDUP
	template< class Kernel_ >
		typename Segment_wrapper<Kernel_>::FT
			Segment_wrapper<Kernel_>::circumradius_squared(void) const
	{
		switch (styp) { 
			case STRAIGHT :  return straight->circumradius_squared();break;
			case ARC : 			 return arc->circumradius_squared();break;
			case BEZIER_2 :  return bezier_2->circumradius_squared();break;
			case BEZIER_3 :  return bezier_3->circumradius_squared();break;
		default : return FT(0);break;
		}
	};//UNFOLD
	//encroaches(Point)//FOLDUP
	template< class Kernel_ >
		bool
			Segment_wrapper<Kernel_>::encroaches(const Point & test_point) const
	{
		switch (styp) { 
			case STRAIGHT : return straight->encroaches(test_point);break;
			case ARC : 			return arc->encroaches(test_point);break;
			case BEZIER_2 : return bezier_2->encroaches(test_point);break;
			case BEZIER_3 : return bezier_3->encroaches(test_point);break;
		default : return false;break;
		}
	};//UNFOLD
	//estimate_total_curvature(void)//FOLDUP
	template< class Kernel_ >
		typename Segment_wrapper<Kernel_>::FT
			Segment_wrapper<Kernel_>::estimate_total_curvature(void) const
			{
				switch (styp) { 
					case STRAIGHT : return straight->estimate_total_curvature();break;
					case ARC : 			return arc->estimate_total_curvature();break;
					case BEZIER_2 : return bezier_2->estimate_total_curvature();break;
					case BEZIER_3 : return bezier_3->estimate_total_curvature();break;
				default : return straight->estimate_total_curvature();break;
				}
			};//UNFOLD
	//get direction at endpoint//FOLDUP
	template< class Kernel_ >
		typename Segment_wrapper<Kernel_>::Direction
			Segment_wrapper<Kernel_>::end_tangent(const int & which_end) const
			{
				switch (styp) { 
					case STRAIGHT : return straight->end_tangent(which_end);break;
					case ARC : 			return arc->end_tangent(which_end);break;
					case BEZIER_2 : return bezier_2->end_tangent(which_end);break;
					case BEZIER_3 : return bezier_3->end_tangent(which_end);break;
				default : return straight->end_tangent(which_end);break;
				}
			};//UNFOLD
	//has_inflection_point(Point,Split_hints)//FOLDUP
	template< class Kernel_ >
		bool 
			Segment_wrapper<Kernel_>::has_inflection_point(Point &p, Split_hints &p_loc) const
			{
				switch (styp) { 
					case STRAIGHT : return false;break;
					case ARC : 			return false;break;
					case BEZIER_2 : return false;break;
					case BEZIER_3 : if (bezier_3->has_inflection_point(p_loc))
													{ p = bezier_3->blossom(p_loc);
														return true;
													} else
													{ return false; }
													break;
				default : break;
				};
				return false;
			};//UNFOLD
	//for debugging://FOLDUP
	template< class Kernel_ >
		std::ostream& 
			Segment_wrapper<Kernel_>::operator << (std::ostream& os)
	{
		switch (seg_wrap.styp) { 
			case STRAIGHT :  os << "STRAIGHT: "; break;
			case ARC : 			 os << "ARC: "; break;
			case BEZIER_2 :  os << "BEZIER_2: "; break;
			case BEZIER_3 :  os << "BEZIER_3: "; break;
		default : os << "UNKNOWN\n"; break;
		};
		return os;
	};//UNFOLD
//UNFOLD
} 

#endif //SEGMENT_BASE_CPP
