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
 * $Id: segment_base.h,v 1.25 2005/02/19 00:02:49 spav Exp $
 * $Log: segment_base.h,v $
 * Revision 1.25  2005/02/19 00:02:49  spav
 * looks like cubic bezier is working
 *
 * Revision 1.24  2005/02/15 23:51:55  spav
 * still working on bez_3
 *
 * Revision 1.23  2005/01/18 22:47:11  spav
 * changing provoker to runtime option.
 *
 * Revision 1.22  2004/12/21 22:42:57  spav
 * looks like it is working; added command line parser.
 *
 * Revision 1.21  2004/12/10 23:52:29  spav
 * pong back to you.
 *
 * Revision 1.20  2004/12/08 01:36:54  spav
 * looking good. have segment uniqueness problem, tho. have finished curvature split grooming problem?
 *
 * Revision 1.19  2004/12/06 03:54:20  spav
 * added more stuff
 *
 * Revision 1.18  2004/12/04 02:50:26  spav
 * straight, arc works. adding bezier.
 *
 * Revision 1.17  2004/12/02 23:59:35  spav
 * problems with queue ordering...
 *
 * Revision 1.16  2004/12/02 02:48:08  spav
 * bezier 3 coming along.
 *
 * Revision 1.15  2004/12/01 00:02:48  spav
 * seems to work for arcs. start to add bezier.
 *
 * Revision 1.14  2004/11/29 07:05:04  spav
 * post thanksgiving
 *
 * Revision 1.13  2004/11/23 23:43:32  spav
 * appears to work for straight segs. now add arcs, bezier.
 *
 * Revision 1.12  2004/11/22 23:31:32  spav
 * seems to work, but problem with degenerate chull edge/faces
 *
 * Revision 1.11  2004/11/19 07:35:00  spav
 * problems with disq_table vs. actual length. i.e. it is splitting ball segments inadvertently.
 *
 * Revision 1.10  2004/11/19 02:10:56  spav
 * something odd: an infinite vertex and repeated ballsplitting
 *
 * Revision 1.9  2004/11/18 02:48:21  spav
 * syntactically ok; a semantic mess past groom..
 *
 * Revision 1.8  2004/11/17 00:04:38  spav
 * it compiles in this crippled form.
 *
 * Revision 1.7  2004/11/09 23:58:25  spav
 * slow going. have to rewrite how the endtable works?
 *
 * Revision 1.6  2004/11/09 02:32:54  spav
 * fixed some stuff...
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
 * Revision 1.1  2004/10/22 04:52:36  spav
 * pong back to you.
 *
 *
 */
//END CVSLOG
//END HEADER

#ifndef SEGMENT_BASE_H
#define SEGMENT_BASE_H

#include "utility.h"

#include <CGAL/basic.h>
#include <CGAL/enum.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Ray_2_Segment_2_intersection.h>
#include <CGAL/Segment_2_Line_2_intersection.h>
#include <CGAL/Object.h>

#include <iostream>
#include <cmath>
#include <iterator>
#include <vector>

//FIX: it seems like i have things overstored. i have a table
//from vhandle vhandle to segobj. and segobj stores both endpoints.
//so the endpoints are stored twice?
namespace lopping {

	//the base class for a segment//FOLDUP
	template<	class Kernel_ >
	class Segment_base
	{
		public:
			typedef Kernel_ 																		Kernel;
			typedef typename Kernel_::Point_2 									Point;
			typedef typename Kernel_::FT 												FT;
			typedef typename Kernel_::Direction_2 							Direction;
			typedef typename Kernel_::Vector_2 									Vector;

			//an endpoint. possibly mod 2.
			Point end_point(const int &i) const;
			
			//find the natural midpoint
			Point split_point(void) const;
			//find the midpoint given a provoker
			Point split_point(const Point &p) const;
			//find the midpoint at a given distance from one endpoint
			Point split_point(const FT &cut_distance_squared,
												const bool &cut_at_first) const;
			//find esimated total variation midpoint
			Point tv_split_point(void) const;

			//constructor from a parent and new midpoint
			Segment_base() {};
			Segment_base(const Segment_base &parent, const Point &p,
									 const bool &use_first_endpoint);
			Segment_base(const Segment_base &parent, const bool &reverse_segment);

			virtual ~Segment_base() = 0;

			//misc
			virtual FT circumradius_squared(void) const = 0;
			virtual bool encroaches(const Point & test_point) const = 0;

			virtual FT estimate_total_curvature(void) const = 0;


			//1 for first endpoint 2 for second endpoint.
			//gives tangent direction for first endpoint, and
			//_reverse_ tangent direction for second endpoint.
			virtual Direction end_tangent(const int & which_end) const = 0;
	};//UNFOLD

	//an actual segment//FOLDUP
	template<	class Kernel_ >
	class Straight_segment: virtual public Segment_base< Kernel_ >
	{
		public:
			typedef Kernel_ 																		Kernel;
			typedef typename Kernel_::Point_2 									Point;
			typedef typename Kernel_::FT 												FT;
			typedef typename Kernel_::Direction_2 							Direction;
			typedef typename Kernel_::Vector_2 									Vector;

			//the two endpoints:
			const Point p1;
			const Point p2;

			static bool Use_Provoker;

			//an endpoint. possibly mod 2.
			Point end_point(const int &i) const
			{ if (i % 2) { return p1; } else { return p2; } }

			//find the natural midpoint
			Point midpoint(void) const { return CGAL::midpoint(p1,p2); }

			//find the natural midpoint
			Point split_point(void) const
				{ return CGAL::midpoint(p1,p2); }
			//find the midpoint given a provoker
			Point split_point(const Point &p) const;
			//find the midpoint at a given distance from one endpoint
			Point split_point(const FT &cut_distance_squared,
												const bool &cut_at_first) const;
			//find esimated total variation midpoint
			Point tv_split_point(void) const { return split_point(); };

			//constructor from a parent and new midpoint
			Straight_segment(const Straight_segment &parent, const Point &p,
									 const bool &use_first_endpoint) :
				p1( (use_first_endpoint)? parent.p1 : p ), 
				p2( (use_first_endpoint)? p : parent.p2 ) {};

			Straight_segment(const Straight_segment &parent, const bool &reverse_segment) :
				p1( (reverse_segment)? parent.p2 : parent.p1 ), 
				p2( (reverse_segment)? parent.p1 : parent.p2 ) {};

			//inherited ctor/dtor
			Straight_segment() {
#if PARANOIA > GRIPE_INFO
						GRIPE("void Straight_segment ctor called\n");
#endif
			};
			virtual ~Straight_segment() {};

			//natural constructor
			Straight_segment(const Point &point1,const Point &point2) : 
				p1(point1),p2(point2) { 
#if PARANOIA > GRIPE_INFO
						GRIPE("Straight_segment point point ctor called\n");
						GRIPE( "x: " << p1.x() << " y: " << p1.y() << "\n" );
						GRIPE( "x: " << p2.x() << " y: " << p2.y() << "\n" );
#endif
				};

			//calculate some stuff
			const FT diameter_squared(void) const
				{ return CGAL::squared_distance(p1,p2); }

			FT circumradius_squared(void) const
				{ return diameter_squared() / FT(4.0); }

			//does the test_point encroach the segment?
			bool encroaches(const Point & test_point) const
			{
				return (CGAL::angle(p1,test_point,p2) != CGAL::ACUTE);
//				Point the_midpoint = this->midpoint();
//				return (CGAL::squared_distance(the_midpoint,test_point) < 
//					 	circumradius_squared());
			}

			FT estimate_total_curvature(void) const 
				{ return FT(0.0); }
			Direction end_tangent(const int & which_end) const 
				{ if (which_end % 2) { return Direction(Vector(p1,p2)); } 
												else { return Direction(Vector(p2,p1)); } };
	};
	
	template < class K > bool Straight_segment<K>::Use_Provoker = false;
	//UNFOLD

	//an arc segment//FOLDUP
	//assume less than pi angle of circle.
	template<	class Kernel_ >
	class Arc_segment: public Straight_segment< Kernel_ >
	{
		public:
			typedef Kernel_ Kernel;
			typedef typename Kernel_::Point_2 Point;
			typedef typename Kernel_::FT FT;
			typedef Kernel_ 																		Kernel;
			typedef typename Kernel_::Point_2 									Point;
			typedef typename Kernel_::FT 												FT;
			typedef typename Kernel_::Direction_2 							Direction;
			typedef typename Kernel_::Vector_2 									Vector;

			//statics aren't inherited?
			static bool Use_Provoker;

			//the center
			const Point center;
			//two endpoints are inherited from straight
			
			//is it cw?
			bool is_cw;

			//FIX: the split_points are inherited, make them from arcs.
			//find the natural midpoint
			Point split_point(void) const;
			//find the midpoint given a provoker
			Point split_point(const Point &p) const;
			//find the midpoint at a given distance from one endpoint
			Point split_point(const FT &cut_distance_squared,
												const bool &cut_at_first) const;
			//find esimated total variation midpoint
			Point tv_split_point(void) const { return split_point(); };

			//ctor from a parent and new midpoint
			Arc_segment(const Arc_segment &parent, const Point &p,
									const bool &use_first_endpoint) : 
						Straight_segment< Kernel_ >(parent,p,use_first_endpoint),
						center(parent.center), 
						is_cw( parent.is_cw )
						{ };

			Arc_segment(const Arc_segment &parent, const bool &reverse_segment) :
				Straight_segment< Kernel_ >(parent, reverse_segment), 
			  center(parent.center),
				is_cw( (!reverse_segment && parent.is_cw) || 
							 (reverse_segment && !parent.is_cw) )
				{
//					GRIPE("arc constructor called; reverse = " << reverse_segment << "\n");
				};

			//natural constructor
			//FIX: add radius check?
			Arc_segment(const Point &point1,const Point &point2,
									const Point &cen,const bool clockwise) : 
				Straight_segment< Kernel_ >(point1,point2), center(cen),
				is_cw(clockwise)
				{};

			//four point ctor;
			Arc_segment(const Point &point1,const Point &middling_point,
									const Point &point2,const Point &cen);

			//inherited ctor/dtor
			Arc_segment() {};
			~Arc_segment() {};

			FT estimate_total_curvature(void) const;
			Direction end_tangent(const int & which_end) const;

			//stuff specific to arc?
			
			//return number of intersections of the arc and the line,
			//putting them in point_1 and point_2.
			int intersections(const typename Kernel_::Line_2 &the_line,
												Point & point_1, Point & point_2) const;

			//this is the radius of the arc, not the circumradius of
			//the protecting diametral circle.
			FT radius_sqrd(void) const 
			{ return
				(CGAL::squared_distance(center,p1) + CGAL::squared_distance(center,p2)) / FT(2.0);
			};

			//this is the radius of the arc, not the circumradius of
			//the protecting diametral circle.
			FT radius(void) const 
			{ return lopping::sqrtFT< Kernel_ >(radius_sqrd()); }

			//angle to the xaxis of center to p1;
			FT start_angle(void) const
			{
				typename Kernel_::Vector_2 start_v(center,p1);
				return angle_to_xaxis< Kernel_ >(start_v);
			};

			//angle to the xaxis of center to p2;
			FT end_angle(void) const
			{
				typename Kernel_::Vector_2 end_v(center,p2);
				return angle_to_xaxis< Kernel_ >(end_v);
			};
		private:

			//return true if the ray from center to check_p intersects the arc.
			bool _is_between(const Point &check_p) const;

	};
	template < class K > bool Arc_segment<K>::Use_Provoker = false;
	//UNFOLD
	
	template< class Kernel_ > class Bezier_3;

	//a quadratic bezier curve//FOLDUP
	template<	class Kernel_ >
	class Bezier_2 : public Straight_segment< Kernel_ >
	{
		public:
			typedef Kernel_ 																		Kernel;
			typedef typename Kernel_::Point_2 									Point;
			typedef typename Kernel_::FT 												FT;
			typedef typename Kernel_::Direction_2 							Direction;
			typedef typename Kernel_::Vector_2 									Vector;
			typedef typename Kernel_::Line_2 										Line;

			//statics aren't inherited?
			static bool Use_Provoker;

			//crummy interface here.
			typedef FT Split_hints;

			//control points are p1, p15 p2
			//the middle control point 
			Point p15;

			//find the natural midpoint
			Point split_point(Split_hints &p_loc) const;
			//find the midpoint given a provoker
			Point split_point(const Point &p, Split_hints &p_loc) const;
			//find the midpoint at a given distance from one endpoint
			Point split_point(const FT &cut_distance_squared,
												const bool &cut_at_first,
												Split_hints &p_loc) const;
			//find esimated total variation midpoint
			Point tv_split_point(Split_hints &p_loc) const;

			//ctor from a parent and new midpoint
			Bezier_2(const Bezier_2 &parent, const Point &p,
									const bool &use_first_endpoint,
									const Split_hints &p_loc);

			Bezier_2(const Bezier_2 &parent, const bool &reverse_segment) :
				Straight_segment< Kernel_ >(parent, reverse_segment),
				p15(parent.p15)
				{ };

			//natural constructor
			Bezier_2(const Point &point1,
							 const Point &point15,
							 const Point &point2):
				Straight_segment< Kernel_ >(point1,point2), p15(point15)
				{};

			//inherited ctor/dtor
			Bezier_2() {};
			~Bezier_2() {};

			FT estimate_total_curvature(void) const;

			Direction end_tangent(const int & which_end) const 
				{ if (which_end % 2) { return Direction(Vector(p1,p15)); } 
												else { return Direction(Vector(p2,p15)); } };

			//stuff specific to bezier_2?
			
			//return the point on the bezier curve for the given t.
			//also set the linear combinations of p1,p15 and p15,p2
			Point blossom(const FT &t,Point &p115,Point &p152) const;

			//return the point on the bezier curve for the given t.
			Point blossom(const FT &t) const
			{
				Point dummy1,dummy2;
				return blossom(t,dummy1,dummy2);
			};

		protected:
			//give the lambda of the point on the bisector of p1,p2
			FT _bisectors_lambda(void) const;
			//FIX: make this give # of intersections?
			//give the lambda of the point on the given line
			FT _line_intersection_lambda(const Line &the_line) const;

		public:
			friend class lopping::Bezier_3< Kernel_ >;
	};
	template < class K > bool Bezier_2<K>::Use_Provoker = false;
	//UNFOLD
	
	//a cubic bezier curve//FOLDUP
	template<	class Kernel_ >
	class Bezier_3 : public Straight_segment< Kernel_ >
	{
		public:
			typedef Kernel_ 																		Kernel;
			typedef typename Kernel_::Point_2 									Point;
			typedef typename Kernel_::FT 												FT;
			typedef typename Kernel_::Direction_2 							Direction;
			typedef typename Kernel_::Vector_2 									Vector;
			typedef typename Kernel_::Line_2 										Line;

			//statics aren't inherited?
			static bool Use_Provoker;

			//crummy interface here.
			typedef FT Split_hints;

			//control points are p1, p133, p166, p2
			//the middle control point 
			Point p133;
			Point p166;

			//find the natural midpoint
			Point split_point(Split_hints &p_loc) const;
			//find the midpoint given a provoker
			Point split_point(const Point &p, Split_hints &p_loc) const;
			//find the midpoint at a given distance from one endpoint
			Point split_point(const FT &cut_distance_squared,
												const bool &cut_at_first,
												Split_hints &p_loc) const;
			//find esimated total variation midpoint
			Point tv_split_point(Split_hints &p_loc) const;

			//ctor from a parent and new midpoint
			Bezier_3(const Bezier_3 &parent, const Point &p,
									const bool &use_first_endpoint,
									const Split_hints &p_loc);

			Bezier_3(const Bezier_3 &parent, const bool &reverse_segment) :
				Straight_segment< Kernel_ >(parent, reverse_segment),
				p133(((reverse_segment)? parent.p166:parent.p133)),
				p166(((reverse_segment)? parent.p133:parent.p166))
				{ };

			//natural constructor
			Bezier_3(const Point &point1,
							 const Point &point133,
							 const Point &point166,
							 const Point &point2):
				Straight_segment< Kernel_ >(point1,point2), p133(point133), p166(point166)
				{};

			//inherited ctor/dtor
			Bezier_3() {};
			~Bezier_3() {};

			//return true if it has an inflection point. if
			//so gives it in p_loc
			bool has_inflection_point(Split_hints &p_loc) const;

			FT estimate_total_curvature(void) const;

			Direction end_tangent(const int & which_end) const 
				{ if (which_end % 2) { return Direction(Vector(p1,p133)); } 
												else { return Direction(Vector(p2,p166)); } };

			//stuff specific to bezier_3?
			
			//return the point on the bezier curve for the given t.
			//also set all the linear combinations of p1,p133; p133,p166; and
			//p166,p2; p1,p133 & p133,p166 and p133,p166 & p166,p2;
			Point blossom(const FT &t,Point &p1133,Point &p133166,Point &p1662,
										Point &p1133166,Point &p1331662) const;

			//return the point on the bezier curve for the given t.
			Point blossom(const FT &t) const
			{
				Point dummy1,dummy2,dummy3,dummy4,dummy5;
				return blossom(t,dummy1,dummy2,dummy3,dummy4,dummy5);
			};

		private: 
			//give the lambda of the point on the bisector of p1,p2
			FT _bisectors_lambda(void) const;
			//FIX: make this give # of intersections?
			//give the lambda of the point on the given line
			FT _line_intersection_lambda(const Line &the_line) const;

	};
	template < class K > bool Bezier_3<K>::Use_Provoker = false;
	//UNFOLD
	
	//the wrapper class//FOLDUP
	template<	class Kernel_ >
	class Segment_wrapper //: virtual public Segment_base< Kernel_ >
	{
		public:
			typedef Kernel_ 																		Kernel;
			typedef typename Kernel_::Point_2 									Point;
			typedef typename Kernel_::FT 												FT;
			typedef typename Kernel_::Direction_2 							Direction;
			typedef typename Kernel_::Vector_2 									Vector;

			typedef Straight_segment< Kernel_ > 		Straight_segment;
			typedef Arc_segment< Kernel_ > 					Arc_segment;
			typedef Bezier_2< Kernel_ > 						Bezier_2;
			typedef Bezier_3< Kernel_ > 						Bezier_3;

			typedef typename Bezier_2::Split_hints	Split_hints;

			typedef std::vector<Point> Point_Vector;
			typedef typename Point_Vector::iterator Point_Iterator;

			//which kind
			enum segment_type {
				STRAIGHT,
				ARC,
				BEZIER_2,
				BEZIER_3
			};

			segment_type styp;

			//an anonymous union.
			//see also
			//http://www.informit.com/guides/content.asp?g=cplusplus&seqNum=178
			union {
				Straight_segment*			straight;
				Arc_segment*   				arc;
				Bezier_2*   				  bezier_2;
				Bezier_3*   				  bezier_3;
			};

			static bool Use_Provoker;

			static void set_provoker(const bool &set_to);

			//an endpoint. possibly mod 2.
			Point end_point(const int &i) const;

			//find the natural midpoint
			Point split_point(Split_hints &p_loc) const;
			//find the midpoint given a provoker
			Point split_point(const Point &p, Split_hints &p_loc) const;
			//find the midpoint at a given distance from one endpoint
			Point split_point(const FT &cut_distance_squared,
												const bool &cut_at_first,
												Split_hints &p_loc) const;
			//find esimated total variation midpoint
			Point tv_split_point(Split_hints &p_loc) const;

			//input constructor 
			//num ints is 2:  give the two endpoints of the seg
			//num ints is 3:  give the control points of the bez2.
			//num ints is 4:  give the control points of the bez3.
			//num ints is -4: give the endpoint, point on arc, endpoint, then center.
			Segment_wrapper(const int &num_ints, const int &num_exts, Point_Iterator pnt_it);

			//constructor from a parent and new midpoint
			Segment_wrapper(const Segment_wrapper &parent, const Point &p,
									 const bool &use_first_endpoint,
									 const Split_hints &p_loc);

			//constructor for a segment from the two endpoints
			Segment_wrapper(const Point &end1, const Point &end2) :
				styp(STRAIGHT), straight( new Straight_segment(end1,end2) ) { };

			//constructor from a parent and bool 
			Segment_wrapper(const Segment_wrapper &parent, const bool &reverse_segment);

			//inherited ctor/dtor
			Segment_wrapper() : styp(STRAIGHT), straight(0) {};
			//Dtor
			~Segment_wrapper(void);

			FT circumradius_squared(void) const;
			bool encroaches(const Point & test_point) const;
			FT estimate_total_curvature(void) const;
			Direction end_tangent(const int & which_end) const;
			//if it has ip; if so, return it in P, p_loc;
			bool has_inflection_point(Point &p, Split_hints &p_loc) const;

			Segment_wrapper* reverse(void) const
				{ 
					return new Segment_wrapper(*this,true); 
				};

			//for debugging:
			std::ostream& operator << (std::ostream& os);

	};
	template < class K > bool Segment_wrapper<K>::Use_Provoker = false;
	//UNFOLD
	
}

//yes, this is the dumbest shit ever, no i am not kidding, yes
//you have to include the template definition if you want this
//to work on the 2004 state of the art c++ compilers.

#include "segment_base.cpp"

#endif //SEGMENT_BASE_H

