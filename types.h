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

  Created: 2016.05.03
  Copyright: Steven E. Pav, 2016
  Author: Steven E. Pav <steven@corecast.io>
  Comments: Steven E. Pav
*/

#ifndef __DEF_TYPES__
#define __DEF_TYPES__

#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/point_generators_2.h>

typedef double Coord_type;
typedef CGAL::Cartesian<Coord_type>	    Rep;

typedef CGAL::Point_2<Rep>                  Point;
typedef CGAL::Segment_2<Rep>                Segment;
typedef CGAL::Line_2<Rep>                   Line;
typedef CGAL::Triangle_2<Rep>               Triangle;
typedef CGAL::Circle_2<Rep>                 Circle;

typedef CGAL::Delaunay_triangulation_2<Rep> Delaunay;

typedef Delaunay::Vertex_iterator           Vertex_iterator;
typedef Delaunay::Face_handle               Face_handle;
typedef Delaunay::Vertex_handle             Vertex_handle;
typedef Delaunay::Edge                      Edge;
typedef Delaunay::Line_face_circulator      Line_face_circulator;

#endif /* __DEF_TYPES__ */

//for vim modeline: (do not edit)
// vim:ts=2:sw=2:tw=79:fdm=marker:fmr=FOLDUP,UNFOLD:cms=//%s:tags=.c_tags;:syn=cpp:ft=cpp:mps+=<\:>:ai:si:cin:nu:fo=croql:cino=p0t0c5(0:
