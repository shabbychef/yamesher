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
 * $Id: work_queue.h,v 1.22 2005/02/12 01:37:17 spav Exp $
 * $Log: work_queue.h,v $
 * Revision 1.22  2005/02/12 01:37:17  spav
 * fixed xfig read bug.
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
 * Revision 1.17  2004/12/06 03:54:20  spav
 * added more stuff
 *
 * Revision 1.16  2004/12/04 02:50:26  spav
 * straight, arc works. adding bezier.
 *
 * Revision 1.15  2004/11/29 07:05:04  spav
 * post thanksgiving
 *
 * Revision 1.14  2004/11/23 23:43:32  spav
 * appears to work for straight segs. now add arcs, bezier.
 *
 * Revision 1.13  2004/11/23 05:39:02  spav
 * nearly done
 *
 * Revision 1.12  2004/11/19 07:35:00  spav
 * problems with disq_table vs. actual length. i.e. it is splitting ball segments inadvertently.
 *
 * Revision 1.11  2004/11/19 02:10:56  spav
 * something odd: an infinite vertex and repeated ballsplitting
 *
 * Revision 1.10  2004/11/18 02:48:21  spav
 * syntactically ok; a semantic mess past groom..
 *
 * Revision 1.9  2004/11/17 00:04:38  spav
 * it compiles in this crippled form.
 *
 * Revision 1.8  2004/11/10 06:26:29  spav
 * end table is screwy..
 *
 * Revision 1.7  2004/11/09 02:32:55  spav
 * fixed some stuff...
 *
 * Revision 1.6  2004/11/02 23:51:20  spav
 * checking in.
 *
 * Revision 1.5  2004/10/29 05:26:50  spav
 * pong.
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
 *
 */
//END CVSLOG
//END HEADER

#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include "config.h"

//for triple
#include "utility.h"
#include <iostream>
#include <queue>
//for pair?
#include <utility>

namespace lopping {

	//Work_request<Kernel,TDS2,SegObj>//FOLDUP
	//kernel, triangulation and segment lookup key
	template < class Kernel_, class TriangulationDataStructure_2, class SegmentObject >
	class Work_request {
		public:
			typedef Kernel_ Kernel;
			typedef TriangulationDataStructure_2 										TDS_2;
			typedef SegmentObject 																	Segment_Object;
			typedef typename TDS_2::Vertex_handle 									Vertex_handle;
			typedef typename TDS_2::Face_handle 										Face_handle;
			typedef typename Kernel::FT 														FT;
			typedef typename Kernel_::Point_2 											Point;
			typedef typename Kernel_::Triangle_2 										Triangle;
			typedef Triplet< Vertex_handle, Vertex_handle, Segment_Object > 
				Two_vertex_segment;
			typedef Triplet< Vertex_handle, Vertex_handle, Vertex_handle > 
				Three_vertices;
			typedef Two_vertex_segment 															Vertex_vertex_segment;

			enum Work_type {
				CURVATURE_SPLIT		= 50,
				SPLIT_BALL   			= 40,
		    SPLIT_SEGMENT    	= 30,
		    YIELD_TO_SEGMENT 	= 20,
		    SPLIT_FACE    		= 10,
		    DUMMY         		= 0
			};

			Work_type wtype;
			FT radius_squared;

			//an anonymous union.
			//see also
			//http://www.informit.com/guides/content.asp?g=cplusplus&seqNum=178
			union {
				Three_vertices* 								triangle_ptr;
				Vertex_handle* 									vertex_h_ptr;
				Vertex_vertex_segment* 					vvsegment_ptr;
			};

			Point provoker;
			//Vertex_handle provoker;

			//uhhh
			Work_request(void) : wtype(DUMMY), radius_squared(FT(0)), 
			 triangle_ptr( 0 ) { 
				 CARP( GRIPE_CDTORS, "work_req::empty_ctor called\n")
			 };

			//constructor
			Work_request(Vertex_handle &vtx) : 
				wtype(SPLIT_BALL), vertex_h_ptr(&vtx) { };
			Work_request(Vertex_vertex_segment &vvs, const FT &rsq) :
				wtype(YIELD_TO_SEGMENT), radius_squared(rsq), vvsegment_ptr(&vvs) { };
			//added for CURVATURE_SPLITS:
			Work_request(const Work_type &wt, Vertex_vertex_segment &vvs, const FT &rsq) :
				wtype(wt), radius_squared(rsq), vvsegment_ptr(&vvs) { };
			Work_request(Vertex_vertex_segment &vvs,Point &provok, 
									 const FT &rsq) : wtype(SPLIT_SEGMENT), radius_squared(rsq), 
									 vvsegment_ptr(&vvs ), provoker(provok) { };
			Work_request(Three_vertices &tvs, const FT &rsq): 
				wtype(SPLIT_FACE), radius_squared(rsq), triangle_ptr(&tvs )
				{ };

			//return 1 if a < b.  recall highest priority
			//are processed first
			bool operator() (Work_request &a,Work_request &b)
			{ if (a.wtype == b.wtype)
				{
					switch (a.wtype) {
						case SPLIT_SEGMENT : 
						case CURVATURE_SPLIT :
						case YIELD_TO_SEGMENT : 
						case SPLIT_FACE : return a.radius_squared < b.radius_squared; break;
						default : return 0; break;
					}
				} else
				{
					return a.wtype < b.wtype;
				}
			}
	};//UNFOLD

	//Work_queue< class Kernel_, class TriangulationDataStructure_2, class SegObj >//FOLDUP
	template < class Kernel_, class TriangulationDataStructure_2, class SegmentObject >
	class Work_queue : public std::priority_queue< 
			Work_request< Kernel_,TriangulationDataStructure_2, SegmentObject >,
			std::vector< Work_request< Kernel_,TriangulationDataStructure_2, SegmentObject > >,
			Work_request< Kernel_,TriangulationDataStructure_2,SegmentObject > >
	{
		public:

			typedef Kernel_ 																						Kernel;
			typedef TriangulationDataStructure_2 												TDS_2;
			typedef SegmentObject 																			Segment_Object;

			typedef Work_request< Kernel_,TriangulationDataStructure_2, Segment_Object > 
				Work_request;

			typedef typename Work_request::Vertex_vertex_segment 				Vertex_vertex_segment;
			typedef typename Work_request::Three_vertices								Three_vertices;

			typedef typename TDS_2::Vertex_handle 											Vertex_handle;
			typedef typename TDS_2::Face_handle 												Face_handle;
			typedef typename Kernel::FT 																FT;
			typedef typename Kernel_::Point_2 													Point;

			typedef std::priority_queue< Work_request, 
				std::vector< Work_request >, Work_request >
				base_priority_queue;

			//ctor
			Work_queue(void) : base_priority_queue()
				{ 
				 CARP( GRIPE_CDTORS, "work_req::ctor called\n")
				};

			//push a request back on?
			void push_req(const Work_request &topush)
				{ this->push(topush); };

			//hope this works.
			//iffy on the new thing.
			void push_curvature_split(Vertex_handle vtx1,Vertex_handle vtx2,
																 Segment_Object segobj,const FT &rsq) 
				{ 
					Vertex_vertex_segment *new_vvs = 
						new Vertex_vertex_segment(vtx1,vtx2,segobj);
					const Work_request *wreq = 
						new Work_request( Work_request::CURVATURE_SPLIT, *new_vvs, rsq );
				 	CARP( GRIPE_INFO, "pushing curvature_split " << (*wreq).wtype << "\n")
				 	CARP( GRIPE_INFO, "seg is ")
				 	CARPVH( GRIPE_INFO, vtx1)
				 	CARP( GRIPE_INFO, "   <=> ")
				 	CARPVH( GRIPE_INFO, vtx2)

					this->push(*wreq);
				};

			void push_split_ball(Vertex_handle vtx) 
				{
					Vertex_handle *new_v = new Vertex_handle(vtx);
					const Work_request *wreq = new Work_request( *new_v );
				 	CARP( GRIPE_INFO, "pushing split_ball with type " << (*wreq).wtype << "\n")
				 	CARP( GRIPE_INFO, "vtx is ")
				 	CARPVH( GRIPE_INFO, vtx)
					this->push(*wreq); 
				};

			void push_yield_to_segment(Vertex_handle vtx1,Vertex_handle vtx2,
																 Segment_Object segobj,const FT &rsq) 
				{ 
					Vertex_vertex_segment *new_vvs = 
						new Vertex_vertex_segment(vtx1,vtx2,segobj);
					const Work_request *wreq = new Work_request( *new_vvs, rsq );

				 	CARP( GRIPE_INFO, "pushing yield_to_seg with type " << (*wreq).wtype << "\n")
				 	CARP( GRIPE_INFO, "seg is ")
				 	CARPVH( GRIPE_INFO, vtx1)
				 	CARP( GRIPE_INFO, "   <=> ")
				 	CARPVH( GRIPE_INFO, vtx2)

					this->push(*wreq);
				};

			void push_split_segment(Vertex_handle vtx1,Vertex_handle vtx2,
																 Segment_Object segobj,Vertex_handle provo,
																 const FT &rsq) 
				{ 
					Vertex_vertex_segment *new_vvs =
						new Vertex_vertex_segment(vtx1,vtx2,segobj);
					Point *new_p = new Point(provo->point());
					const Work_request *wreq = new Work_request( *new_vvs, *new_p, rsq );

				 	CARP( GRIPE_INFO, "pushing split_seg with type " << (*wreq).wtype << "\n")
				 	CARP( GRIPE_INFO, "seg is ")
				 	CARPVH( GRIPE_INFO, vtx1)
				 	CARP( GRIPE_INFO, "   <=> ")
				 	CARPVH( GRIPE_INFO, vtx2)
				 	CARP( GRIPE_INFO, "seg is " << &(*new_vvs) << "\n")
				 	CARP( GRIPE_INFO, "provoked by ")
				 	CARPVH( GRIPE_INFO, provo)

					this->push(*wreq); 
				};

			void push_split_segment(Vertex_handle vtx1,Vertex_handle vtx2,
																 Segment_Object segobj,Point provo,
																 const FT &rsq) 
				{ 
					Vertex_vertex_segment *new_vvs =
						new Vertex_vertex_segment(vtx1,vtx2,segobj);
					Point *new_p = new Point(provo);
					const Work_request *wreq = new Work_request( *new_vvs, *new_p, rsq );


				 	CARP( GRIPE_INFO, "pushing split_seg with type " << (*wreq).wtype << "\n")
				 	CARP( GRIPE_INFO, "seg is ")
				 	CARPVH( GRIPE_INFO, vtx1)
				 	CARP( GRIPE_INFO, "   <=> ")
				 	CARPVH( GRIPE_INFO, vtx2)
				 	CARP( GRIPE_INFO, "seg is " << &(*new_vvs) << "\n")
				 	CARP( GRIPE_INFO, "provoked by ")
				 	CARPP( GRIPE_INFO, provo)

					this->push(*wreq); 
				};


			void push_split_face(Face_handle face, const FT &rsq)
				{ 
					Three_vertices *tvs = 
						new Three_vertices( face->vertex(0), face->vertex(1), face->vertex(2) );
				 	CARP( GRIPE_INFO, "pushing split face\n")
					const Work_request *wreq = new Work_request( *tvs, rsq );
					this->push(*wreq); 
				};

			//hope this works.
			void pop(void)
			{
				CARP( GRIPE_INFO, "work_queue::pop called\n")
				CARP( GRIPE_INFO, "work_queue has " << this->size() << " elements\n")
				CARP( GRIPE_INFO, "with top val " << (this->top()).wtype << " \n")

//				const Work_request *top_element = &(this->top());
				this->base_priority_queue::pop();

				CARP( GRIPE_INFO, "work_queue now has " << this->size() << " elements\n")
				CARP( GRIPE_INFO, "with top val " << (this->top()).wtype << " \n")
//				delete top_element;
			}

	};//UNFOLD

}

#endif //WORK_QUEUE_H
