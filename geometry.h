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

#ifndef __DEF_GEOMETRY__
#define __DEF_GEOMETRY__

#include <cmath>
#include <CGAL/basic.h>
#include "math.h"
#include "debugging.h"

namespace lopping {

	// check for collinear points on a horizontal line, i.e. same y coordinate//FOLDUP
	template< class Kernel_ >
		bool is_horizontal_collinear(const typename Kernel_::Point_2 &a,
																 const typename Kernel_::Point_2 &b,
																 const typename Kernel_::Point_2 &c)
		{ 
			return ((a.y() == b.y()) && (b.y() == c.y())); 
		};//UNFOLD

	//check for collinear points on a vertical line, i.e. same x coordinate//FOLDUP
	template< class Kernel_ >
		bool   is_vertical_collinear(const typename Kernel_::Point_2 &a,
																 const typename Kernel_::Point_2 &b,
																 const typename Kernel_::Point_2 &c)
		{ 
			return ((a.x() == b.x()) && (b.x() == c.x())); 
		};//UNFOLD

	// check for collinear point, i.e. same x coordinate, or same y coordinate, or//FOLDUP
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

	//compute and return the min and max angle of the triangle;//FOLDUP
	//return true value if the triangle is degenerate. 
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

	//make sure the points a, b, and c are collinear. //FOLDUP
	//if they are, return true. 
	//otherwise, shift point c by a small amount to make orthogonal and
	//collinear, then return false.
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

#endif /* __DEF_GEOMETRY__ */

//for vim modeline: (do not edit)
// vim:ts=2:sw=2:tw=79:fdm=marker:fmr=FOLDUP,UNFOLD:cms=//%s:tags=.c_tags;:syn=cpp:ft=cpp:mps+=<\:>:ai:si:cin:nu:fo=croql:cino=p0t0c5(0:
