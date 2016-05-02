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
 * $Id: cgal_types.h,v 1.2 2004/10/22 04:52:35 spav Exp $
 * $Log: cgal_types.h,v $
 * Revision 1.2  2004/10/22 04:52:35  spav
 * pong back to you.
 *
 * Revision 1.1  2004/10/20 21:50:26  spav
 * starting code project
 *
 *
 */
//END CVSLOG
//END HEADER

#ifndef CGAL_TYPES_H
#define CGAL_TYPES_H

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

#endif //CGAL_TYPES_H

