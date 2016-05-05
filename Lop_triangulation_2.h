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
 * $Id: Lop_triangulation_2.h,v 1.26 2005/03/16 00:45:01 spav Exp $
 * $Log: Lop_triangulation_2.h,v $
 * Revision 1.26  2005/03/16 00:45:01  spav
 * adding quick and dirty Chew mode
 *
 * Revision 1.25  2005/02/12 01:37:16  spav
 * fixed xfig read bug.
 *
 * Revision 1.23  2005/01/03 03:59:53  spav
 * home from san fran.
 *
 * Revision 1.22  2004/12/21 22:42:57  spav
 * looks like it is working; added command line parser.
 *
 * Revision 1.21  2004/12/10 23:52:29  spav
 * pong back to you.
 *
 * Revision 1.20  2004/12/08 01:36:53  spav
 * looking good. have segment uniqueness problem, tho. have finished curvature split grooming problem?
 *
 * Revision 1.19  2004/12/06 03:54:19  spav
 * added more stuff
 *
 * Revision 1.18  2004/12/04 02:50:26  spav
 * straight, arc works. adding bezier.
 *
 * Revision 1.17  2004/12/02 23:59:35  spav
 * problems with queue ordering...
 *
 * Revision 1.16  2004/11/29 07:05:03  spav
 * post thanksgiving
 *
 * Revision 1.15  2004/11/23 23:43:31  spav
 * appears to work for straight segs. now add arcs, bezier.
 *
 * Revision 1.14  2004/11/23 02:50:36  spav
 * nearly got it...
 *
 * Revision 1.13  2004/11/22 23:31:32  spav
 * seems to work, but problem with degenerate chull edge/faces
 *
 * Revision 1.12  2004/11/19 23:48:24  spav
 * some fixes
 *
 * Revision 1.11  2004/11/19 07:35:00  spav
 * problems with disq_table vs. actual length. i.e. it is splitting ball segments inadvertently.
 *
 * Revision 1.10  2004/11/19 02:10:55  spav
 * something odd: an infinite vertex and repeated ballsplitting
 *
 * Revision 1.9  2004/11/18 02:48:21  spav
 * syntactically ok; a semantic mess past groom..
 *
 * Revision 1.8  2004/11/17 00:04:38  spav
 * it compiles in this crippled form.
 *
 * Revision 1.7  2004/11/15 06:20:03  spav
 * converted to merge end_table and the DT
 *
 * Revision 1.6  2004/11/04 23:31:25  spav
 * got a running readinput--still cannot hook it up, though.
 *
 * Revision 1.5  2004/11/04 02:40:38  spav
 * pong
 *
 * Revision 1.4  2004/11/02 23:51:19  spav
 * checking in.
 *
 * Revision 1.3  2004/10/29 05:26:50  spav
 * pong.
 *
 * Revision 1.2  2004/10/28 00:21:17  spav
 *
 * fixing up stuff.
 *
 * Revision 1.1  2004/10/26 23:48:04  spav
 * gaining momentum./
 *
 *
 *
 */
//END CVSLOG
//END HEADER

#ifndef LOP_TRIANGULATION_H
#define LOP_TRIANGULATION_H

#include "utility.h"
#include "work_queue.h"
#include "lookups.h"
#include "segment_base.h"

#include "CGAL/basic.h"
#include <CGAL/enum.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
//#include <stack>
//#include <set>

namespace lopping {

	//Lop_triangulation_2//FOLDUP
	template < class Kernel_, class TriangulationDataStructure_2 >
		class Lop_triangulation_2 
		{
			public:
				//all the types
				typedef Kernel_ Kernel;
				typedef TriangulationDataStructure_2 						TDS_2;
				typedef CGAL::Delaunay_triangulation_2< Kernel, TDS_2 > DT_2;

	
				typedef typename TDS_2::Vertex_handle 					Vertex_handle;
				typedef typename TDS_2::Face_handle 						Face_handle;
				typedef typename TDS_2::Edge                    Edge;

				typedef typename DT_2::Finite_faces_iterator    Finite_faces_iterator;

				typedef typename Kernel::FT 										FT;
				typedef typename Kernel::Point_2 								Point;
				typedef typename Kernel::Triangle_2 						Triangle;
	
				typedef Distance_squared_table< Kernel, TDS_2 > Distance_squared_table;
	
				typedef DT_and_Edge_Table< DT_2 >								DT_and_ET;

				typedef typename DT_and_ET::Segment_wrapper			Segment_wrapper;
				typedef typename DT_and_ET::Segment_handle			Segment_handle;
				typedef typename Segment_wrapper::Split_hints		Split_hints;
	
				typedef Work_queue< Kernel, TDS_2, Segment_handle > 	Work_queue;
				typedef typename Work_queue::Work_request 			      Work_request;
				typedef typename Work_request::Vertex_vertex_segment  Vertex_vertex_segment;
				typedef typename Work_request::Three_vertices         Three_vertices;

				enum Insert_type {
					BALL_SPLITTER,
					CURVATURE_SPLITTER,
					SEGMENT_POINT,
					FACE_CIRCUMCENTER,
				};

				union Insert_hint {
					Edge*     						split_edge;
					Face_handle* 					split_face;
				};

				enum Insert_result {
					POINT_ADDED,
					TOO_CLOSE_TO_INPUT_VERTEX,
					YIELDED_TO_SEGMENT_SPLIT,
					NO_ATTEMPT
				};

				//member variables:
				//lop_tri has a DT, a work queue, an endtable
				DT_and_ET                		the_DT_and_ET;
				Work_queue 									the_Q;
				Distance_squared_table			the_disq_table;

				int TERMLIMIT;

				//in paper this is sqrt(2) - 1. restrict to (0,0.5)
				FT													groom_factor;
				//in paper this is beta. restrict to [0.5,1)
				FT								 		 			ball_reduction_ratio;
				//this is 4 sin^2(kappa).  restrict to [0,0.5)
				FT								 		 			squared_edge_radius_ratio;
				//in radians.  in the paper, this is assumed to be less than pi/3
				FT								 		 			curvature_limit;
				//in whatever units.  if zero, is ignored, o/w larger triangles are
				//to be axed.
				FT								 		 			cradius_squared_limit;
				//the maximum degree out of a vertex; above this limit 
				//a protecting ball must be put around the vertex.
				//below this limit we check adjacent segs to see if
				//a protecting ball is necessary. this is useful because we
				//break curvature as a preprocessor, which would introduce
				//spurious input of degree 2, i.e. the curvature break points.
				short unsigned int					max_deg;


				//constructor from void
				Lop_triangulation_2(void) :
					the_DT_and_ET(), the_Q(), the_disq_table(), 
					TERMLIMIT(500),
					groom_factor(FT(0.414213)), ball_reduction_ratio(FT(0.75)),
					squared_edge_radius_ratio(FT(0.5)),
					cradius_squared_limit(FT(0.0)),
					curvature_limit(FT(1.04719755119660)),
					max_deg(3)
				{ };

				//member operations:

				std::istream& read_input(std::istream &in_file);
				std::ifstream& read_xfig_file(std::ifstream &in_file) throw(NYI);
				std::ifstream& read_wdb_file(std::ifstream &in_file);

				void remove_conflicts(void)
				{
					//do this to get rid of CURVATURE split reqs
					//relies on _groom being fixed.
					_write_ps("as_input.ps");
					_process_queue(false);
					_write_ps("curvature_bounded.ps");
					VOICE( GRIPE_PROGRESS, "start groom\n")
					_groom();
					VOICE( GRIPE_PROGRESS, "done with groom\n")
					_write_ps("finished_groom.ps");
					//_verify_disq_table();
					_recover_segments();
					VOICE( GRIPE_PROGRESS, "recovered segments\n")
					_write_ps("input_conforming.ps");
					_insure_quality();
					VOICE( GRIPE_PROGRESS, "quality insured\n")
					_write_ps("quality_insured.ps");
					VOICE( GRIPE_PROGRESS, "mesher terminates\n")
				};

			private:

				//write to a ps file
				void _write_ps(const char *fname); 

				//write to a freefrem .msh file
//				void _write_msh(const char *fnamestub); 
				
				//if low quality, adds kill request to work queue;
				//if a quality face, returns true. why not?
				bool _check_quality(const Face_handle &a_face);

				void _groom(void);
				void _recover_segments(void);
				void _insure_quality(void);
				void _process_queue(const bool & check_skinny);
				void _split_ball(const Vertex_handle & corner_v,
												 const FT & cut_distance_squared,
												 const bool & check_encroached,
												 const bool & check_skinny);

				//check the disq_table
				bool _verify_disq_table(void);

				//return true if sucessful.
				//and the point yields to a midpoint add instead.
				//in that case, splitseg request is added to the queue.
				//FIX: isn't the segment redundant if we do a lookup?
				Insert_result 
					_split_segment(const Vertex_handle &vtx1,const Vertex_handle &vtx2,
													const Segment_handle &s_hand, 
													const Insert_type &itype,
													const Point &split_point,
													const Split_hints &p_loc,
													const bool &check_encroached,
													const bool &check_skinny);

				//do a curvature split
				Insert_result 
					_curvature_split(const Vertex_handle &vtx1,const Vertex_handle &vtx2,
													const Segment_handle &s_hand);

				//insert the segment into the ET and DT table.
				//and check curvature.
				void _insert_seg_check_curv(const Vertex_handle &vtx1,
																		const Vertex_handle &vtx2,
																		const Segment_handle &s_hand);

				//return true if the segment is encroached.
				//may add a kill request to the queue
				//may add edge->segobj to the end_table.
				bool _is_seg_encroached(const Vertex_handle &vtx1,const Vertex_handle &vtx2, 
															 const Segment_handle &s_hand,
															 const bool add_to_end_table,const bool add_to_queue);
																				
				//return the result, and the vertex handle
				Insert_result _insert_point(const Point &p,
																		const Insert_type &itype,
																		const Insert_hint &ihint,
																		const bool & check_encroached,
																		const bool & check_skinny,
																		Vertex_handle &put_vh);

				//return the result, and the vertex handle
				Insert_result _insert_point(const Point &p,
																		const Insert_type &itype,
																		const Insert_hint &ihint,
																		const bool & check_encroached,
																		const bool & check_skinny)
				{
					Vertex_handle put_vh;
					return _insert_point(p,itype,ihint,check_encroached,check_skinny,put_vh);
				}

		};//UNFOLD

}

//yes, this is the dumbest shit ever, no i am not kidding, yes
//you have to include the template definition if you want this
//to work on the 2004 state of the art c++ compilers.

#include "Lop_triangulation_2.cpp"

#endif //LOP_TRIANGULATION_H

