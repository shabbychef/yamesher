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
 * $Id: Lop_triangulation_2_test.h,v 1.7 2005/02/12 01:37:16 spav Exp $
 * $Log: Lop_triangulation_2_test.h,v $
 * Revision 1.7  2005/02/12 01:37:16  spav
 * fixed xfig read bug.
 *
 * Revision 1.5  2004/12/21 22:42:57  spav
 * looks like it is working; added command line parser.
 *
 * Revision 1.4  2004/11/23 02:50:36  spav
 * nearly got it...
 *
 * Revision 1.3  2004/11/04 02:40:39  spav
 * pong
 *
 * Revision 1.2  2004/11/02 23:51:19  spav
 * checking in.
 *
 * Revision 1.1  2004/11/02 22:43:41  spav
 * moving lopper test to LT_2_test
 *
 *
 */
//END CVSLOG
//END HEADER

#ifndef LOP_TRIANGULATION_2_TEST_H
#define LOP_TRIANGULATION_2_TEST_H

// #define USE_HIGH_PRECISION

#include "Lop_triangulation_2.h"
#include <fstream>
#include <string>
#include <iostream>
//CGAL headers
#include <CGAL/basic.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
//TCLAP headers
#include <tclap/CmdLine.h>

#ifdef USE_HIGH_PRECISION
typedef CGAL::Exact_predicates_exact_constructions_kernel					  Geo_kernel;
#else
typedef CGAL::Exact_predicates_inexact_constructions_kernel					Geo_kernel;
#endif //USE_HIGH_PRECISION

typedef CGAL::Triangulation_vertex_base_2<Geo_kernel>               Vertex_base;
typedef CGAL::Triangulation_face_base_2<Geo_kernel>     						Face_base;
typedef CGAL::Triangulation_data_structure_2<Vertex_base,Face_base> TDS_2;
typedef lopping::Lop_triangulation_2<Geo_kernel, TDS_2> 				    Lop_mesh;
typedef Lop_mesh::FT 																								FT;

#endif //LOP_TRIANGULATION_2_TEST_H

