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
 * $Id: work_queue_test.h,v 1.1 2004/11/04 02:40:39 spav Exp $
 * $Log: work_queue_test.h,v $
 * Revision 1.1  2004/11/04 02:40:39  spav
 * pong
 *
 *
 *
 */
//END CVSLOG
//END HEADER

#ifndef WORK_QUEUE_TEST_H
#define WORK_QUEUE_TEST_H

#include "config.h"

#include "work_queue.h"
#include <fstream>
//CGAL headers
#include <CGAL/basic.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_data_structure_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel					Geo_kernel;
typedef CGAL::Triangulation_vertex_base_2<Geo_kernel>               V_base;
typedef CGAL::Triangulation_face_base_2<Geo_kernel>     						F_base;
typedef CGAL::Triangulation_data_structure_2<V_base,F_base> 				TDS_2;
typedef TDS_2::Edge 																								Edge;
typedef lopping::Work_queue<Geo_kernel, TDS_2, Edge >               work_Q;

#endif //WORK_QUEUE_TEST_H

