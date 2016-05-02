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
 * $Id: Lop_triangulation_2.cpp,v 1.39 2005/03/16 00:45:01 spav Exp $
 * $Log: Lop_triangulation_2.cpp,v $
 * Revision 1.39  2005/03/16 00:45:01  spav
 * adding quick and dirty Chew mode
 *
 * Revision 1.38  2005/03/11 03:46:57  spav
 * problem was ball splitting? -- changed whether subsegs of ball get encroachment checking.
 *
 * Revision 1.37  2005/02/19 00:02:49  spav
 * looks like cubic bezier is working
 *
 * Revision 1.36  2005/02/12 01:37:16  spav
 * fixed xfig read bug.
 *
 * Revision 1.35  2005/01/28 00:05:23  spav
 * redid the debugging/verbosity thing.
 *
 * Revision 1.34  2005/01/26 23:11:45  spav
 * looks good.
 *
 * Revision 1.33  2005/01/18 22:47:10  spav
 * changing provoker to runtime option.
 *
 * Revision 1.32  2005/01/04 23:54:46  spav
 * some tweaks.
 *
 * Revision 1.31  2005/01/03 03:59:53  spav
 * home from san fran.
 *
 * Revision 1.30  2004/12/21 22:42:57  spav
 * looks like it is working; added command line parser.
 *
 * Revision 1.29  2004/12/10 23:52:29  spav
 * pong back to you.
 *
 * Revision 1.28  2004/12/08 01:36:53  spav
 * looking good. have segment uniqueness problem, tho. have finished curvature split grooming problem?
 *
 * Revision 1.27  2004/12/06 03:54:19  spav
 * added more stuff
 *
 * Revision 1.26  2004/12/04 02:50:26  spav
 * straight, arc works. adding bezier.
 *
 * Revision 1.25  2004/12/02 23:59:35  spav
 * problems with queue ordering...
 *
 * Revision 1.24  2004/12/01 00:02:47  spav
 * seems to work for arcs. start to add bezier.
 *
 * Revision 1.23  2004/11/29 07:05:03  spav
 * post thanksgiving
 *
 * Revision 1.22  2004/11/23 23:43:31  spav
 * appears to work for straight segs. now add arcs, bezier.
 *
 * Revision 1.21  2004/11/23 05:39:02  spav
 * nearly done
 *
 * Revision 1.20  2004/11/23 02:50:36  spav
 * nearly got it...
 *
 * Revision 1.19  2004/11/22 23:31:32  spav
 * seems to work, but problem with degenerate chull edge/faces
 *
 * Revision 1.18  2004/11/19 23:48:24  spav
 * some fixes
 *
 * Revision 1.17  2004/11/19 07:35:00  spav
 * problems with disq_table vs. actual length. i.e. it is splitting ball segments inadvertently.
 *
 * Revision 1.16  2004/11/19 03:01:20  spav
 * still lost
 *
 * Revision 1.15  2004/11/19 02:10:55  spav
 * something odd: an infinite vertex and repeated ballsplitting
 *
 * Revision 1.14  2004/11/18 02:48:21  spav
 * syntactically ok; a semantic mess past groom..
 *
 * Revision 1.13  2004/11/17 00:04:37  spav
 * it compiles in this crippled form.
 *
 * Revision 1.12  2004/11/15 06:20:03  spav
 * converted to merge end_table and the DT
 *
 * Revision 1.11  2004/11/10 06:26:29  spav
 * end table is screwy..
 *
 * Revision 1.10  2004/11/09 23:58:25  spav
 * slow going. have to rewrite how the endtable works?
 *
 * Revision 1.9  2004/11/09 02:32:54  spav
 * fixed some stuff...
 *
 * Revision 1.8  2004/11/08 05:56:38  spav
 * another lost weekend.
 *
 * Revision 1.7  2004/11/06 02:47:57  spav
 * small changes, back to you.
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
 * Revision 1.1  2004/10/20 21:50:26  spav
 * starting code project
 *
 *
 */
//END CVSLOG
//END HEADER

#include "Lop_triangulation_2.h"

#ifndef LOP_TRIANGULATION_2_CPP
#define LOP_TRIANGULATION_2_CPP

namespace lopping { 

////////////////////////////////////////////////////////////////////////

	//read_input//FOLDUP
	//read num points
	//read n points, bless each, make map from string (name) to pair of 
	//	pointhandle, vertexhandle (initialize null)
	//
	//read num segobjs
	//read m segobjs, bless each, make map from string (name) to 
	//	triple of segobjhandle, endpoint names.
	//
	//read num ints, num exts
	//read all int+ext constraints of plane. foreach
	//	if vertex obj and vhandle null in map, insert into DT, update vhandle in map.
	//	if segobj, for each endpoint, attempt to insert into DT as above if vhandle
	//		null, and update.
	//		once vhandle non null, insert double vhandle key into endpoint lookup table.
	//
	//destroy two string keyed maps.
	//
	//return triangulation, endpoint lookup
	template < class Kernel_, class TriangulationDataStructure_2 >
	std::istream&
		Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::read_input(
				std::istream &in_file)
	{

		CARP( GRIPE_GATES, "read .dat input called\n")

		//typedefs;//FOLDUP
		typedef typename 
			Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::Point Point;
		typedef std::string String;

		typedef std::map< std::string, Point> SPMap;
		typedef std::vector< Point > PointVec;

		typedef std::map< std::string, Segment_handle > SHMap;

		//map from point to its vertex handle in the_DT_and_ET
		typedef typename 
			Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::Vertex_handle
			Vertex_handle;
		typedef std::map< std::string, Vertex_handle > PVMap;

		//typename std::string::size_type s_pos;
		String nexline;
		int dim2;
		int num_read = 0;
//UNFOLD
		//get dimension statement?//FOLDUP
		in_file >> nexline;
		in_file >> dim2;

		if (dim2 != 2)
		{ std::cerr << "did not find proper dimension statement in input?\n" << 
				nexline << " " << dim2 << "\n";
			return in_file;
		}//UNFOLD
		//read points//FOLDUP
		int num_points;
		Point *read_point;
		SPMap point_names;
		String *point_name;
		float xc;
		float yc;

		in_file >> nexline;
		in_file >> num_points;

//		GRIPE ("nowgot " << nexline << "this many points: " << num_points);

		while (++num_read <= num_points)
		{
			point_name = new String;
			in_file >> *point_name;
			in_file >> xc;
			in_file >> yc;
			read_point = new Point(xc,yc);
			point_names[*point_name] = *read_point;
			//GRIPE (*point_name << ": " << xc << " " << yc << "\n");
		}
//UNFOLD
		//read segments//FOLDUP
		int num_segs;
		String *seg_name;
		String sub_pnt_name;
		Point* sub_pnt;
		int num_ints,num_exts,num_sub_pnts,sub_pnts_read;
		PointVec defining_points;
		Segment_handle the_seg;
		SHMap seg_names;

		in_file >> nexline;
		in_file >> num_segs;

//		GRIPE("nowgot " << nexline << "this many segments: " << num_segs);

		num_read = 0;
		while (++num_read <= num_segs)
		{
			seg_name = new String;
			in_file >> *seg_name;
			in_file >> num_ints;
			in_file >> num_exts;
			num_sub_pnts = abs(num_ints) + abs(num_exts);

			//FIX: could be inefficient to do this every time?
			defining_points.reserve(num_sub_pnts);  

			sub_pnts_read = 0;
			while (++sub_pnts_read <= num_sub_pnts)
			{
				in_file >> sub_pnt_name;
				sub_pnt = new Point( point_names[sub_pnt_name] );
				defining_points.push_back( *sub_pnt );
//				GRIPE(" " << sub_pnt_name << "coords: ");
//				GRIPE("x: " << sub_pnt->x() << ", y: " << sub_pnt->y() << "\n");
			}
			the_seg = new Segment_wrapper(num_ints,num_exts,defining_points.begin());
			seg_names[*seg_name] = the_seg;
//			GRIPE("nothing done?\n");

			//FIX: does this destroy the points in the vector?
			defining_points.clear(); //for next time
		}
//UNFOLD
		//read planes//FOLDUP
		int num_planes;
		String *plane_name;
		String sub_simp_name;
		int num_int_simps,num_ext_simps,num_sub_simps,sub_simps_read;
		Vertex_handle end1;
		Vertex_handle end2;

		in_file >> nexline;
		in_file >> num_planes;

//		GRIPE("nowgot " << nexline << "this many planes: " << num_planes << "\n");

		num_read = 0;
		while (++num_read <= num_planes)
		{
			plane_name = new String;
			in_file >> *plane_name;
			in_file >> num_int_simps;
			in_file >> num_ext_simps;
			num_sub_simps = abs(num_int_simps) + abs(num_ext_simps);

			sub_simps_read = 0;
			while (++sub_simps_read <= num_sub_simps)
			{
				in_file >> sub_simp_name;
				if ( point_names.count(sub_simp_name) > 0 ) 
				{
					the_DT_and_ET.insert((point_names[sub_simp_name]));
//					GRIPE( sub_simp_name << " is a point\n");
				} else if ( seg_names.count(sub_simp_name) > 0 )
				{
//					GRIPE( sub_simp_name << " is a segment\n");
					end1 = (the_DT_and_ET.insert((seg_names[sub_simp_name])->end_point(1)));
					end2 = (the_DT_and_ET.insert((seg_names[sub_simp_name])->end_point(2)));
//					GRIPE( "e1 " << (end1->point()).x() << " " << (end1->point()).y() << "\n") ;
//					GRIPE( "e2 " << (end2->point()).x() << " " << (end2->point()).y() << "\n") ;
					_insert_seg_check_curv(end1,end2,seg_names[sub_simp_name]);
				} else
				{ 
//					GRIPE(" could not find " << sub_simp_name << "\n"); 
				}
			}

		}
//UNFOLD
#if PARANOID > CHECK_BIZARRE
		the_DT_and_ET.gripe_segs(std::cerr); 
#endif
		CARP( GRIPE_GATES, "now ensure chull\n")
		the_DT_and_ET.ensure_chull_are_segs();
		the_DT_and_ET.add_bounding_tri();
		CARP( GRIPE_GATES, "read input terminates\n")
		return in_file;

	}; //read_input
//UNFOLD

	//read_xfig_file//FOLDUP
	//
	template < class Kernel_, class TriangulationDataStructure_2 >
	std::ifstream&
		Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::read_xfig_file(
				std::ifstream &in_file) throw(NYI)
	{
		//typedefs;//FOLDUP

		typedef typename 
			Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::Point Point;

		typedef lopping::Map_to_Map<int,Point> 										I_I_P_map;

		typedef std::string String;

		typedef std::map< String, Point> SPMap;
		typedef std::vector< Point > PointVec;

		typedef std::map< String, Segment_handle > SHMap;

		//map from point to its vertex handle in the_DT_and_ET
		typedef typename 
			Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::Vertex_handle
			Vertex_handle;
		typedef std::map< String, Vertex_handle > PVMap;

		//typename std::string::size_type s_pos;
		String nexline;
		int objtype,subtype;
		int i,num_to_read;
		int more_lines_to_read;
		int dummyi;
		float dummyf;

		int xc,yc;
		float xfloat,yfloat;
		Point *a_point;
		Point *prev_point;
		Point *next_point;
		Point *cntr_point;
		Point mid_point;
		Point start_point;

		//rotator stuff
		CGAL::Aff_transformation_2< Kernel_ > 						
			rotator( CGAL::ROTATION, FT( std::sqrt(3.0) ), FT(1.0), FT(2.0) );
		typename Kernel_::Vector_2 rotvec;
		

		I_I_P_map 														coords_to_point;

		Vertex_handle end1;
		Vertex_handle end2;

		Segment_handle the_seg;
		PointVec defining_points;

//UNFOLD

		in_file >> nexline; in_file >> nexline; //#FIG 3.2
		in_file >> nexline;  //orientation
		in_file >> nexline;  //centering
		in_file >> nexline;  //units
		in_file >> nexline;  //paper size
		in_file >> nexline;  //magnification
		in_file >> nexline;  //page type
		in_file >> nexline;  //trans color
		in_file >> nexline; in_file >> nexline;//coordinate nonsense

		//now get all the objects;

		defining_points.clear();
		in_file >> objtype;
		while (! in_file.eof() )
		{
			CARP( GRIPE_LOOPS, "get next objtype\n")
			more_lines_to_read = 0;
			switch (objtype) {
				case 0 : //color object//FOLDUP
						CARP( GRIPE_INFO, "got color object\n")
						CARP( GRIPE_INFO, "i do not understand color objects\n")
						in_file >> dummyi;
						in_file >> nexline;
					break;//UNFOLD
				case 1 : //ellipse//FOLDUP
						CARP( GRIPE_INFO, "got ellipse/circle\n")
						in_file >> subtype;
						for (i=1;i<=7;i++) { in_file >> dummyi; }
						in_file >> dummyf;
						for (i=1;i<=1;i++) { in_file >> dummyi; }
						in_file >> dummyf;

						if ((subtype != 3) && (subtype != 4))
						{
							CARP( GRIPE_INFO, "got ellipse\n")
							CARP( GRIPE_INFO, "i do not understand ellipses\n")
							for (i=1;i<=8;i++) { in_file >> dummyi; }
						} else
						{
							CARP( GRIPE_INFO, "got circle\n")
							in_file >> xc;
							in_file >> yc;
							if (coords_to_point.count(xc,yc))
							{
								cntr_point = &coords_to_point(xc,yc);
							} else
							{
								cntr_point = new Point(FT(xc),FT(-yc));
								coords_to_point.single_insert(xc,yc,*cntr_point);
							}
							//two radii
							for (i=1;i<=2;i++) { in_file >> dummyi; }
							//``first'' point
							for (i=1;i<=2;i++) { in_file >> dummyi; }

							//``second'' point
							in_file >> xc;
							in_file >> yc;
							if (coords_to_point.count(xc,yc))
							{
								prev_point = &coords_to_point(xc,yc);
							} else
							{
								prev_point = new Point(FT(xc),FT(-yc));
								coords_to_point.single_insert(xc,yc,*prev_point);
							}
							start_point = *prev_point;

							//now break the circle into three subarcs...
							rotvec = (*prev_point) - (*cntr_point);

							//first subarc//FOLDUP
							CARP( GRIPE_INFO, "subarc 1\n")

							rotvec = rotator.transform(rotvec);
							mid_point = *cntr_point + rotvec;
							rotvec = rotator.transform(rotvec);
							next_point = new Point(*cntr_point + rotvec);

							end1 = the_DT_and_ET.insert(*prev_point);
							end2 = the_DT_and_ET.insert(*next_point);

							defining_points.reserve(4);
							defining_points.push_back( *prev_point );
							defining_points.push_back(   mid_point );
							defining_points.push_back( *next_point );
							defining_points.push_back( *cntr_point );
	
							the_seg = new Segment_wrapper(-4,0,defining_points.begin());
							_insert_seg_check_curv(end1,end2,the_seg);

							defining_points.clear();//UNFOLD
							//second subarc//FOLDUP
							CARP( GRIPE_INFO, "subarc 2\n")

							prev_point = &(*next_point);
							rotvec = rotator.transform(rotvec);
							mid_point = *cntr_point + rotvec;
							rotvec = rotator.transform(rotvec);
							next_point = new Point(*cntr_point + rotvec);

							end1 = end2;
							end2 = the_DT_and_ET.insert(*next_point);

							defining_points.reserve(4);
							defining_points.push_back( *prev_point );
							defining_points.push_back(   mid_point );
							defining_points.push_back( *next_point );
							defining_points.push_back( *cntr_point );
	
							the_seg = new Segment_wrapper(-4,0,defining_points.begin());
							_insert_seg_check_curv(end1,end2,the_seg);

							defining_points.clear();//UNFOLD
							//third subarc//FOLDUP
							CARP( GRIPE_INFO, "subarc 3\n")

							prev_point = &(*next_point);
							rotvec = rotator.transform(rotvec);
							mid_point = *cntr_point + rotvec;

							end1 = end2;
							end2 = the_DT_and_ET.insert(start_point);

							defining_points.reserve(4);
							defining_points.push_back( *prev_point );
							defining_points.push_back(   mid_point );
							defining_points.push_back( start_point );
							defining_points.push_back( *cntr_point );
	
							the_seg = new Segment_wrapper(-4,0,defining_points.begin());
							_insert_seg_check_curv(end1,end2,the_seg);

							defining_points.clear();//UNFOLD

						};
					break;//UNFOLD
				case 2 : //polyline//FOLDUP
						CARP( GRIPE_INFO, "got polyline\n")
						in_file >> subtype;
						for (i=1;i<=7;i++) { in_file >> dummyi; }
						in_file >> dummyf;
						for (i=1;i<=3;i++) { in_file >> dummyi; }

						//are there arrow lines?
						in_file >> dummyi;
						if (dummyi) { more_lines_to_read += 5; }
						in_file >> dummyi;
						if (dummyi) { more_lines_to_read += 5; }
						in_file >> num_to_read;

						//get arrow lines;
						while (--more_lines_to_read >= 0) { in_file >> nexline; };

						//get first point:
						in_file >> xc;
						in_file >> yc;

						CARP( GRIPE_LOOPS, \
										 "inserting " << 0 << " " << xc << " " << yc << "\n")
						if (coords_to_point.count(xc,yc))
						{
							prev_point = &coords_to_point(xc,yc);
						} else
						{
							prev_point = new Point(FT(xc),FT(-yc));
							coords_to_point.single_insert(xc,yc,*prev_point);
						}
						end1 = the_DT_and_ET.insert(*prev_point);

						for (i=1;i<num_to_read;i++)//FOLDUP
						{
							in_file >> xc;
							in_file >> yc;

							CARP( GRIPE_LOOPS, \
										 "inserting " << i << " " << xc << " " << yc << "\n")
							if (coords_to_point.count(xc,yc))
							{
								next_point = &coords_to_point(xc,yc);
							} else
							{
								next_point = new Point(FT(xc),FT(-yc));
								coords_to_point.single_insert(xc,yc,*next_point);
							}
							end2 = the_DT_and_ET.insert(*next_point);

							defining_points.reserve(2);
							defining_points.push_back( *prev_point );
							defining_points.push_back( *next_point );

							the_seg = new Segment_wrapper(2,0,defining_points.begin());
							_insert_seg_check_curv(end1,end2,the_seg);

							defining_points.clear();
							end1 = end2;
							prev_point = next_point;
						}//UNFOLD
					break;//UNFOLD
				case 3 : //spline//FOLDUP
						CARP( GRIPE_INFO, "got an spline\n")
						in_file >> subtype;
						if (subtype != 0) { throw NYI(); }

						for (i=1;i<=7;i++) { in_file >> dummyi; }
						in_file >> dummyf;
						for (i=1;i<=1;i++) { in_file >> dummyi; }

						//are there arrow lines?
						in_file >> dummyi;
						if (dummyi) { more_lines_to_read += 5; }
						in_file >> dummyi;
						if (dummyi) { more_lines_to_read += 5; }

						in_file >> num_to_read;

						if ((num_to_read != 3) && (num_to_read != 4)) { throw NYI(); }

						//get arrow lines;
						while (--more_lines_to_read >= 0) { in_file >> nexline; };

						defining_points.reserve(num_to_read);
						for (i=1;i<=num_to_read;i++)
						{
							in_file >> xc;
							in_file >> yc;
	
							if (coords_to_point.count(xc,yc))
							{
								a_point = &coords_to_point(xc,yc);
							} else
							{
								a_point = new Point(FT(xc),FT(-yc));
								coords_to_point.single_insert(xc,yc,*a_point);
							}

							defining_points.push_back( *a_point );

							if (i==1) { end1 = the_DT_and_ET.insert(*a_point); }
							if (i==num_to_read) { end2 = the_DT_and_ET.insert(*a_point); }
						}

						the_seg = new Segment_wrapper(num_to_read,0,defining_points.begin());
						_insert_seg_check_curv(end1,end2,the_seg);

						defining_points.clear();

						//get ``control factors''
						for (i=1;i<=num_to_read;i++) { in_file >> dummyf; }

					break;//UNFOLD
				case 4 : //text//FOLDUP
						CARP( GRIPE_INFO, "got text. i will die now\n")
						throw NYI();
					break;//UNFOLD
				case 5 : //arc//FOLDUP
						CARP( GRIPE_INFO, "got an arc\n")
						for (i=1;i<=8;i++) { in_file >> dummyi; }
						in_file >> dummyf;
						for (i=1;i<=2;i++) { in_file >> dummyi; }

						//are there arrow lines?
						in_file >> dummyi;
						if (dummyi) { more_lines_to_read += 5; }
						in_file >> dummyi;
						if (dummyi) { more_lines_to_read += 5; }

						//get center point:
						in_file >> xfloat; in_file >> yfloat;
						cntr_point = new Point(FT(xfloat),FT(-yfloat));

						//FIX: figure out the three points on arc vs. two of
						//my arcs....

						//get first end point:
						in_file >> xc;
						in_file >> yc;

						if (coords_to_point.count(xc,yc))
						{
							prev_point = &coords_to_point(xc,yc);
						} else
						{
							prev_point = new Point(FT(xc),FT(-yc));
							coords_to_point.single_insert(xc,yc,*prev_point);
						}
						end1 = the_DT_and_ET.insert(*prev_point);

						//get second point & ignore:
						in_file >> xc;
						in_file >> yc;

						mid_point = Point(FT(xc),FT(-yc));

						//get first end point:
						in_file >> xc;
						in_file >> yc;

						if (coords_to_point.count(xc,yc))
						{
							next_point = &coords_to_point(xc,yc);
						} else
						{
							next_point = new Point(FT(xc),FT(-yc));
							coords_to_point.single_insert(xc,yc,*next_point);
						}
						end2 = the_DT_and_ET.insert(*next_point);

						defining_points.reserve(4);
						defining_points.push_back( *prev_point );
						defining_points.push_back(   mid_point );
						defining_points.push_back( *next_point );
						defining_points.push_back( *cntr_point );

						the_seg = new Segment_wrapper(-4,0,defining_points.begin());
						_insert_seg_check_curv(end1,end2,the_seg);

						defining_points.clear();
					break;//UNFOLD
				case 6 : //object start//FOLDUP
						CARP( GRIPE_INFO, "got an object\n")
						CARP( GRIPE_INFO, "just ignore the corners?\n")
						//FIX: reset units?
						for (i=1;i<=4;i++) { in_file >> dummyi; }
					break;//UNFOLD
				case -6 : //object end//FOLDUP
					CARP( GRIPE_INFO, "got object end\n")
					break;//UNFOLD
				default :
					break;
			};

			while (--more_lines_to_read >= 0) { in_file >> nexline; };
			in_file >> objtype;
		}

		CARP( GRIPE_GATES, "now ensure chull\n")
		the_DT_and_ET.ensure_chull_are_segs();
		the_DT_and_ET.add_bounding_tri();
		CARP( GRIPE_GATES, "read xfig terminates\n")
		return in_file;
	}; //read_xfig_file
//UNFOLD

	//read_wdb_file//FOLDUP
	//world data bank.
	//consists of records with header
	//segment #  rank #  points #
	//ignore everything but the last one.
	//then read that many points.
	template < class Kernel_, class TriangulationDataStructure_2 >
	std::ifstream&
		Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::read_wdb_file(
				std::ifstream &in_file) 
	{
		//typedefs;//FOLDUP

		typedef typename 
			Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::Point Point;

		typedef lopping::Map_to_Map<float,Point> 										F_F_P_map;

		typedef std::string String;

		typedef std::map< String, Point> SPMap;
		typedef std::vector< Point > PointVec;

		typedef std::map< String, Segment_handle > SHMap;

		//map from point to its vertex handle in the_DT_and_ET
		typedef typename 
			Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::Vertex_handle
			Vertex_handle;
		typedef std::map< String, Vertex_handle > PVMap;

		//typename std::string::size_type s_pos;
		int dummyi,num_pnts,i;
		String nexline;

		float xc,yc;
		Point *prev_point;
		Point *next_point;

		F_F_P_map 														coords_to_point;

		Vertex_handle end1;
		Vertex_handle end2;

		Segment_handle the_seg;
		PointVec defining_points;

//UNFOLD

		defining_points.clear();
		//get first string
		in_file >> nexline; 
		while (! in_file.eof() )//FOLDUP
		{
			CARP( GRIPE_LOOPS, "get next record\n")
			in_file >> dummyi;
			in_file >> nexline;
			in_file >> dummyi;
			in_file >> nexline;

			in_file >> num_pnts;

			//get first point
			in_file >> xc;
			in_file >> yc;

			CARP( GRIPE_LOOPS, "inserting " << 0 << " " << xc << " " << yc << "\n")

			if (coords_to_point.count(xc,yc))
			{
				prev_point = &coords_to_point(xc,yc);
			} else
			{
				prev_point = new Point(FT(xc),FT(-yc));
				coords_to_point.single_insert(xc,yc,*prev_point);
			}
			end1 = the_DT_and_ET.insert(*prev_point);

			for (i=1;i<num_pnts;i++)//read the points//FOLDUP
			{
				in_file >> xc;
				in_file >> yc;

				CARP( GRIPE_LOOPS, "inserting " << i << " " << xc << " " << yc << "\n")
				if (coords_to_point.count(xc,yc))
				{
					next_point = &coords_to_point(xc,yc);
				} else
				{
					next_point = new Point(FT(xc),FT(-yc));
					coords_to_point.single_insert(xc,yc,*next_point);
				}
				end2 = the_DT_and_ET.insert(*next_point);

				defining_points.reserve(2);
				defining_points.push_back( *prev_point );
				defining_points.push_back( *next_point );

				the_seg = new Segment_wrapper(2,0,defining_points.begin());
				_insert_seg_check_curv(end1,end2,the_seg);

				defining_points.clear();
				end1 = end2;
				prev_point = next_point;
			}//UNFOLD

			//get next record 
			in_file >> nexline; 
		}//eof//UNFOLD

		CARP( GRIPE_GATES, "now ensure chull\n")
		the_DT_and_ET.ensure_chull_are_segs();
		the_DT_and_ET.add_bounding_tri();
		CARP( GRIPE_GATES, "read wdb terminates\n")
		return in_file;
	}; //read_wdb_file
//UNFOLD

	//_write_ps//FOLDUP
	//write to a ps file
	template < class Kernel_, class TriangulationDataStructure_2 >
			void 
	Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::_write_ps(const char *fname)
	{
		CARP( GRIPE_GATES, "start _write_ps\n" )
		std::ofstream output_file;output_file.open(fname);
		if (output_file.good()) 
		{ 
			//identifying info first

			output_file << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
			output_file << "%generated by Lop_triangulation_2.cpp\n";
			output_file << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
			output_file << "%groom factor: " << groom_factor << "\n";
			output_file << "%ball red ratio (beta): " << ball_reduction_ratio << "\n";
			output_file << "%edge-radius ratio squared, i.e. 4 sin^2(kappa): " << 
				squared_edge_radius_ratio << "\n";
			output_file << "%curvature limit (tau): " << curvature_limit << "\n";
			output_file << "%maximum vertex degree: " << max_deg << "\n";
			output_file << "%using provoker: " << 
				((Segment_wrapper::Use_Provoker)? " true\n" : "false\n");
			output_file << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";

			//now hand it off.
			the_DT_and_ET.write_ps(output_file); 
		} else
		{ std::cerr << "could not open " << fname << "\n"; }
		output_file.close();

		CARP( GRIPE_GATES, "to exit _write_ps\n" )
	}; //_write_ps
//UNFOLD

//	//_write_msh//FOLDUP
//	//write to a msh file
//	template < class Kernel_, class TriangulationDataStructure_2 >
//			void 
//	Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::_write_ps(const char *fnamestub)
//	{
//		char * fname = strcat(fnamestub,".msh");
//		CARP( GRIPE_GATES, "start _write_msh\n" )
//		std::ofstream output_file;output_file.open(fname);
//		if (output_file.good()) 
//		{ 
//			//identifying info first
//
//			output_file << "#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
//			output_file << "#generated by Lop_triangulation_2.cpp\n";
//			output_file << "#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
//			output_file << "#groom factor: " << groom_factor << "\n";
//			output_file << "#ball red ratio (beta): " << ball_reduction_ratio << "\n";
//			output_file << "#edge-radius ratio squared, i.e. 4 sin^2(kappa): " << 
//				squared_edge_radius_ratio << "\n";
//			output_file << "#curvature limit (tau): " << curvature_limit << "\n";
//			output_file << "#maximum vertex degree: " << max_deg << "\n";
//			output_file << "#using provoker: " << 
//				((Segment_wrapper::Use_Provoker)? " true\n" : "false\n");
//			output_file << "#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
//
//			//now hand it off.
//			the_DT_and_ET.write_msh(output_file); 
//		} else
//		{ std::cerr << "could not open " << fname << "\n"; }
//		output_file.close();
//
//		CARP( GRIPE_GATES, "to exit _write_msh\n" )
//	}; //_write_msh
////UNFOLD

////////////////////////////////////////////////////////////////////////

	//_verify_disq_table//FOLDUP
	template < class Kernel_, class TriangulationDataStructure_2 >
	bool 
		Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::_verify_disq_table(void)
	{
//		typename Distance_squared_table::VFT_iterator  disq_start = //FOLDUP
//			the_disq_table.begin();
//		typename Distance_squared_table::VFT_iterator  disq_end = 
//			the_disq_table.end();
//
//		FT actual_nn;
//		FT dtable_nn;
//		bool is_ok = true;
//
//		while (disq_start != disq_end)
//		{
//			GRIPE( "verify next vtx\n" );
//			actual_nn = the_DT_and_ET.nn_squared((*disq_start).first);
//			dtable_nn = (*disq_start).second;
//			GRIPE( "? is " << dtable_nn.to_double() << " <= " << 
//						 actual_nn.to_double() << "?\n" );
//			is_ok &&= (dtable_nn <= actual_nn);
//			++disq_start;
//		}
//		
//		GRIPE( "exit _verify\n" );
//
//		return is_ok//UNFOLD
		
		typename DT_and_ET::Flat_Iterator edge_walk = 
			the_DT_and_ET.seg_data.flatbegin();
		typename DT_and_ET::Flat_Iterator end_walk =
			the_DT_and_ET.seg_data.flatend();

		FT min_squared_distance;
		FT compare_squared_distance;

		int v_degree;
		int i;

		bool is_ok = true;

		CARP( GRIPE_LOOPS, "_verify to enter edgewalk\n" )
		while (edge_walk != end_walk)
		{
			CARP( GRIPE_LOOPS, "_verify next edgewalk\n" )

			v_degree = the_DT_and_ET.degree( edge_walk.first() );

			if (v_degree > 1)
			{
				min_squared_distance = 
					CGAL::squared_distance( (edge_walk.first())->point(), 
																						(edge_walk.second())->point() );
				for (i=1;i<v_degree;i++)
				{
					++edge_walk;
					compare_squared_distance = 
						CGAL::squared_distance( (edge_walk.first())->point(), 
																							(edge_walk.second())->point() );
					if (compare_squared_distance < min_squared_distance)
						{ min_squared_distance = compare_squared_distance; }
				} //for loop
//				GRIPE( "_verify " << min_squared_distance.to_double() << " vs: " );
//				GRIPE( the_disq_table.get_disq((*edge_walk).first).to_double() << " \n" );

				if (the_disq_table.get_disq(edge_walk.first()) != FT(0))
				{
					is_ok &= 
						(min_squared_distance == the_disq_table.get_disq(edge_walk.first()));
						CGAL_precondition( 
						(min_squared_distance - the_disq_table.get_disq(edge_walk.first()))
						< FT(0.00001));
				}
			} // if deg > 1
			++edge_walk;
		}

		CARP( GRIPE_GATES, "_verify to return " << is_ok << "\n" )
		return is_ok;
	}; //_verify
//UNFOLD

////////////////////////////////////////////////////////////////////////

	//_check_quality//FOLDUP
	//if low quality, adds kill request to work queue;
	//if a quality face, returns true. why not?
	template < class Kernel_, class TriangulationDataStructure_2 >
	bool Lop_triangulation_2<Kernel_,
			 TriangulationDataStructure_2>::_check_quality(const Face_handle &a_face)
	{
		CARP( GRIPE_GATES, "_check_quality start: " << &(*a_face) << "\n" )

		if ((squared_edge_radius_ratio <= FT(0)) && (cradius_squared_limit <= FT(0)))
			{ return true; }
		//FIX: not necessary?
		if (the_DT_and_ET.is_infinite(a_face))
			{ return true; }
		if (the_DT_and_ET.touches_bounding_tri(a_face))
			{ 
				CARP( GRIPE_INFO, "_check_quality touches bt\n")
				return true; 
			} else {
				CARP( GRIPE_INFO, "_check_quality does not touch bt\n")
			}

		//get the ``vertices'' of the triangle, i.e. the corners.
		Point point_0 = (*a_face).vertex(0)->point();
		Point point_1 = (*a_face).vertex(1)->point();
		Point point_2 = (*a_face).vertex(2)->point();

		CARP( GRIPE_INFO, "_check_quality triangle is:\n")
		CARPP( GRIPE_INFO, point_0)
		CARPP( GRIPE_INFO, point_1)
		CARPP( GRIPE_INFO, point_2)

		//FIX: uggh;
		if (lopping::is_collinear<Kernel_>(point_0,point_1,point_2))
			{ return true; 
			} else {
				CARP( GRIPE_FORKS, "the points are not collinear?\n")
			}
			
		CARP( GRIPE_GATES, "_check_quality get CC\n" )
		//get circumcenter, thus circumradius, compare each edge
//		Point circumcenter = CGAL::circumcenter(point_0,point_1,point_2);
		Point circumcenter = the_DT_and_ET.circumcenter(a_face);
		CARP( GRIPE_GATES, "_check_quality got CC\n" )

		if (! the_DT_and_ET.is_inside_bounding_tri(circumcenter))
			{ return true; }

		FT radius_squared = CGAL::squared_distance( point_0, circumcenter );
		FT compare_sqr_edge_len = radius_squared * squared_edge_radius_ratio;

#if PARANOID > CHECK_BIZARRE
		if (radius_squared > FT(100000.0))
		{
			CARP( GRIPE_INFO, "_check_quality found large circumradius?\n")
			CARP( GRIPE_INFO, "_check_quality end triangle:\n")
		}
#endif

		//check edge 01//FOLDUP
		FT sqr_edge_len  = CGAL::squared_distance( point_0, point_1 );
		if (sqr_edge_len < compare_sqr_edge_len)
			//FIX: &a_face could be bad? does a_face get killed?
		{ the_Q.push_split_face(a_face,radius_squared); return false; }
//UNFOLD
		//check edge 02//FOLDUP
		sqr_edge_len  = CGAL::squared_distance( point_0, point_2 );
		if (sqr_edge_len < compare_sqr_edge_len)
		{ the_Q.push_split_face(a_face,radius_squared); return false; }
//UNFOLD
		//check edge 12//FOLDUP
		sqr_edge_len  = CGAL::squared_distance( point_1, point_2 );
		if (sqr_edge_len < compare_sqr_edge_len)
		{ the_Q.push_split_face(a_face,radius_squared); return false; }
//UNFOLD
//
		if ((cradius_squared_limit > FT(0)) && (radius_squared > cradius_squared_limit))
			{ the_Q.push_split_face(a_face,radius_squared); return false; }

		CARP( GRIPE_GATES, "_check_quality exit\n" )
		return true;
	}; //_check_quality
//UNFOLD

	//_groom//FOLDUP
	//foreach key in endpoint lookup, if it points to map of size > 1, grab that 
	//	vertex in the dt, walk all its neighbors to find the NN^2, i.e. minimal
	//	squared distance to a neighbor.  put this in dtable.
	//
	//foreach key in endpoint lookup, if it points to map of size > 1, 
	//	call split_ball(vh,q,true,check_skinny)
	//
	//FIX: now that we add curvature assurance at the beginning, we
	//have to check the ange between adjacent segments vs. 
	//the curvature bound to see if it is necessary to add a ball
	//about an `input' point.
	template < class Kernel_, class TriangulationDataStructure_2 >
	void Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::_groom(void)
	{
		CARP( GRIPE_GATES, "starting groom\n" )

		typename DT_and_ET::First_iterator edge_walk = 
			the_DT_and_ET.seg_data.begin();
		typename DT_and_ET::First_iterator end_walk = 
			the_DT_and_ET.seg_data.end();
		typename DT_and_ET::Second_iterator opp_walk;
		typename DT_and_ET::Second_iterator opp_end_walk;

		int v_deg,which_bucket;
		FT angle_to_x;
		FT bucket_converter = FT( float(max_deg) / (lopping::TWOPI) );
		Segment_handle seg1,seg2,segswap;
		FT angle1,angle2,delangle1,delangle2;
		FT prevangle,thisangle,delprev,delthis;

		//make an array?
		Segment_handle* emanators;
		bool full_bucket[max_deg];

		bool requires_ball = false;

		//FIX: STILL not really done...?
		//multideg case?

		while (edge_walk != end_walk)//FOLDUP
		{
			CARP( GRIPE_GATES, "_groom next edgewalk\n" )
			v_deg = (*edge_walk).second.size();
			requires_ball = (v_deg > max_deg);
			if (v_deg == 2) //the most common case is degree 2?//FOLDUP
			{
				CARP( GRIPE_GATES, "_groom vtx is degree 2\n" )
				opp_walk = (*edge_walk).second.begin();
				seg1 = (*opp_walk).second;
				++opp_walk;
				seg2 = (*opp_walk).second;

				angle1 = lopping::angle_to_xaxis<Kernel_>( seg1->end_tangent(1) );
				angle2 = lopping::angle_to_xaxis<Kernel_>( seg2->end_tangent(1) );

				if (angle2 < angle1)
				{
					angle_to_x = angle2;
					angle2 = angle1;
					angle1 = angle_to_x;
					segswap = seg2;
					seg2 = seg1;
					seg1 = segswap;
				}

				if ((lopping::absFT<Kernel_>(angle2 - angle1)) * bucket_converter < FT(1.0))
				{
					requires_ball = true;
				} else 
					if ((lopping::absFT<Kernel_>(angle2 - (angle1 + FT( lopping::TWOPI )) )) * 
								bucket_converter < FT(1.0))
					{
						requires_ball = true;
					} else
					{
						delangle1 = seg1->estimate_total_curvature();
						delangle2 = seg2->estimate_total_curvature();
	
						if ( (angle1 + delangle1 > angle2 - delangle2) ||
								 (angle2 + delangle2 - FT( lopping::TWOPI ) > angle1 - delangle1)
							 )
						{
							requires_ball = true;
						} 
					}
			}//UNFOLD
			if ((!requires_ball) && (v_deg > 2))//FOLDUP
			{
				CARP( GRIPE_INFO, "_groom vtx is degree " << v_deg << "\n" )
				emanators = new Segment_handle [max_deg];
				for (which_bucket = 0;which_bucket < max_deg;++which_bucket)
				{
					full_bucket[which_bucket] = 0;;
				}

				opp_walk = (*edge_walk).second.begin();
				opp_end_walk = (*edge_walk).second.end();
				while ((opp_walk != opp_end_walk) && (!requires_ball))
				{
					angle_to_x = 
						lopping::angle_to_xaxis<Kernel_>( (*opp_walk).second->end_tangent(1) );
					which_bucket = 
						int(floor( lopping::FT2double<Kernel_>((bucket_converter * angle_to_x)) ));

					//two in a bucket is not allowed, by pigeonhole.
					requires_ball = (full_bucket[which_bucket]);
					emanators[which_bucket] = (*opp_walk).second;
					full_bucket[which_bucket] = true;
					++opp_walk;
				}

				if (!requires_ball)//check curvatures, & adjacent buckets.//FOLDUP
				{
					CARP( GRIPE_INFO, "_groom may require a ball.\n" )
					which_bucket = 0;
					while (!full_bucket[which_bucket]) { ++which_bucket; }

					CARP( GRIPE_INFO, "_groom check end_tangent and tc: \n")

					prevangle =
						lopping::angle_to_xaxis<Kernel_>( (emanators[which_bucket])->end_tangent(1) );
					delprev = (emanators[which_bucket])->estimate_total_curvature();

//					for (which_bucket=1;which_bucket < max_deg;++which_bucket)
					while (++which_bucket < max_deg)
					{
						if (full_bucket[which_bucket])
						{
							CARP( GRIPE_LOOPS, "_groom next bucket\n")
							thisangle = lopping::angle_to_xaxis<Kernel_>( 
																		(emanators[which_bucket])->end_tangent(1) );
							delthis = (emanators[which_bucket])->estimate_total_curvature();
							
							if ( ((thisangle - prevangle) * bucket_converter < FT(1.0)) || 
									 (prevangle + delprev > thisangle - delthis) )
							{
								requires_ball = true;
								which_bucket = max_deg;
							} else
							{
								prevangle = thisangle;
								delprev = delthis;
							}
						}
					}

					//finally check the last against the first
					if (!requires_ball)
					{
						thisangle =
							lopping::angle_to_xaxis<Kernel_>( (emanators[0])->end_tangent(1) );
						delthis = (emanators[0])->estimate_total_curvature();
						if ( ((lopping::absFT<Kernel_>(thisangle - prevangle)) * bucket_converter 
								< FT(1.0)) || 
								 (prevangle + delprev > thisangle - delthis) )
						if (prevangle + delprev - FT(lopping::TWOPI) > thisangle - delthis)
						{
							requires_ball = true;
						}
					}
				}//UNFOLD

				//now clean up emanators
				delete emanators;

			}//UNFOLD
			
			//set -1 for now
			if (requires_ball)
				{ the_disq_table.set_disq(((*edge_walk).first),FT(-1.0)); }

			++edge_walk;
		}//end edge walk//UNFOLD

		FT squared_distance;
		FT compare_squared_distance;

		typename DT_2::Finite_edges_iterator check_edge = 
			the_DT_and_ET.finite_edges_begin();
		typename DT_2::Finite_edges_iterator end_edge = 
			the_DT_and_ET.finite_edges_end();

		Vertex_handle v1,v2;

		while (check_edge != end_edge)//FOLDUP
		{
			CARP( GRIPE_LOOPS, "_groom edgewalk enter\n" )
			v1 = (*((*check_edge).first)).vertex(the_DT_and_ET.cw( (*check_edge).second ));
			v2 = (*((*check_edge).first)).vertex(the_DT_and_ET.ccw( (*check_edge).second ));

			squared_distance = 
				CGAL::squared_distance( v1->point(), v2->point() );

			if (the_disq_table.count(v1) > 0)
			{
				compare_squared_distance = the_disq_table.get_disq(v1);
				if ((compare_squared_distance < FT(0)) || 
						(squared_distance < compare_squared_distance))
				{ the_disq_table.set_disq(v1,squared_distance); }
			}
			if (the_disq_table.count(v2) > 0)
			{
				compare_squared_distance = the_disq_table.get_disq(v2);
				if ((compare_squared_distance < FT(0)) || 
						(squared_distance < compare_squared_distance))
				{ the_disq_table.set_disq(v2,squared_distance); }
			}
			++check_edge;
		}//UNFOLD
		CARP( GRIPE_LOOPS, "_groom done with edgewalk\n" )

		//assert: keys of the_disq_table are exactly those vertices with two or
		//more incident segments.  moreover the value is exactly the squared
		//distance to the nearest neighbor in the DT of the input point set.

		typename Distance_squared_table::iterator vertex_dist_handle;
		FT cut_ratio_squared = groom_factor * groom_factor;

		CARP( GRIPE_LOOPS, "_groom start disqtablewalk\n" )
		for (vertex_dist_handle  = the_disq_table.begin(); 
				 vertex_dist_handle != the_disq_table.end(); 
			 ++vertex_dist_handle)
		{
		CARP( GRIPE_LOOPS, "_groom next.v\n" )
		CARP( GRIPE_INFO, "_groom and dist.second is " << (*vertex_dist_handle).second << "\n" )

			_split_ball((*vertex_dist_handle).first,
												(cut_ratio_squared * (*vertex_dist_handle).second),
												false,false); 
//												true,false);//HUH?
		} //end for each vtx/distance in disq_table

		CARP( GRIPE_GATES, "exit _groom\n" )

	}; //_groom
//UNFOLD

	//_recover_segments//FOLDUP
	//for each segobj, check if it is present or encroached
	//process_queue(false)
	template < class Kernel_, class TriangulationDataStructure_2 >
	void Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::_recover_segments(void)
	{
		CARP( GRIPE_GATES, "starting _recover _segs\n" )
		typename DT_and_ET::Increasing_Flat_Iterator edge_walk = 
			the_DT_and_ET.seg_data.increasingflatbegin();
		typename DT_and_ET::Increasing_Flat_Iterator end_walk =
			the_DT_and_ET.seg_data.increasingflatend();

		CARP( GRIPE_LOOPS, "_recover _segs walk each edge\n" )
		while (edge_walk != end_walk)
		{
			_is_seg_encroached( edge_walk.first(), edge_walk.second(), edge_walk.third(),
													false, true );
			++edge_walk;
		}
		CARP( GRIPE_LOOPS, "_recover _segs now process queue\n" )
		_process_queue(false);
		CARP( GRIPE_GATES, "exiting _recover _segs\n" )
	}; //_recover_segments
//UNFOLD

	//_insure_quality//FOLDUP
	//foreach facehandle, if skinny, add kill request to queue.
	//process_queue(true)
	template < class Kernel_, class TriangulationDataStructure_2 >
	void Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::_insure_quality(void)
	{
		CARP( GRIPE_GATES, "start _insure_quality\n" )

		Finite_faces_iterator face_walker = the_DT_and_ET.finite_faces_begin();
		do { _check_quality(face_walker); } 
		while (++face_walker != the_DT_and_ET.finite_faces_end());

		CARP( GRIPE_LOOPS, "_insure_quality now process queue\n" )
		_process_queue(true);
		CARP( GRIPE_GATES, "exit _insure_quality\n" )
	}; //_insure_quality
//UNFOLD

	//_process_queue//FOLDUP
	//while queue not empty
	//	pop next queue request
	//	switch request:
	//	  CURVATURE_SPLIT:
	//	    call split_seg, but with special request?
	//		SPLIT_BALL:
	//			call split_ball(vh,q,true,check_skinny)
	//		SPLIT_SEGMENT:
	//			if segment is in lookup table (necessary?)
	//			remove segment from endpoint lookup table (both orientations)
	//			call split_seg with provoker, getting new midpoint.
	//			insert_point(midpoint,q,false,true,check_skinny),
	//				getting new vertex handle.
	//			add lookups for new subsegments.
	//		YIELD:
	//			if segment is in lookup table (necessary?)
	//			remove segment from endpoint lookup table (both orientations)
	//			call split_seg with getting new midpoint.
	//			insert_point(midpoint,q,false,true,check_skinny),
	//				getting new vertex handle.
	//			add lookups for new subsegments.
	//		SPLIT_FACE:
	//			if face is in triangulation, let p be its cc,
	//				insert_point(p,q,true,true,true)
	//				(may not insert cc)
	//				if unsuccessful, return SPLIT_FACE(facehandle) to queue.
	//
	template < class Kernel_, class TriangulationDataStructure_2 >
	void Lop_triangulation_2<Kernel_,
			 TriangulationDataStructure_2>::_process_queue(const bool &check_skinny)
	{
		CARP( GRIPE_GATES, "start _process_queue\n" )
		Vertex_handle a_vertex;
		FT            a_squared_distance;
		Face_handle   a_face;
		Point 				split_point;
		Split_hints   p_loc;
		typename Lop_triangulation_2::Insert_result in_result;
		Work_request  top_req;

		while (!the_Q.empty())
		{
			top_req = Work_request(the_Q.top());
			the_Q.pop();
			CARP( GRIPE_LOOPS, "_process_queue get top\n" )
			CARP( GRIPE_LOOPS, "queue has top value " << ((the_Q.top()).wtype) << "\n" )

#if PARANOID > CHECK_BIZARRE
			_verify_disq_table(); 
#endif

			
			the_DT_and_ET.report_stuff(); 

			if ( int(the_DT_and_ET.tds().number_of_vertices()) == TERMLIMIT)
			{
				_write_ps("catchend.ps");
//				the_DT_and_ET.gripe_segs(std::cerr); 
				CGAL_precondition( int(the_DT_and_ET.tds().number_of_vertices()) != TERMLIMIT );
			}

			switch (top_req.wtype) {
				//FIX: there is some error with stuff being added to the
				//queue, then pop taking the top element off it.
				//best strategy would be to pop off the top, do work, and
				//then put back on if need be...
				case Work_request::SPLIT_BALL ://FOLDUP
					CARP( GRIPE_FORKS, "_process_queue is ::SPLIT_BALL\n" )
					//find distance to cut the ball down to
					a_vertex = *(top_req.vertex_h_ptr);
					a_squared_distance = (the_disq_table.get_disq(a_vertex));
					//FIX: getting a negative here?
					a_squared_distance *= ball_reduction_ratio * ball_reduction_ratio;
					//FIX: not done here.
					//?is this done?

					_split_ball(a_vertex,a_squared_distance,true, check_skinny);

					break;//UNFOLD
				case Work_request::CURVATURE_SPLIT ://FOLDUP
					CARP( GRIPE_FORKS, "_process_queue is ::CURVATURE_SPLIT\n" )
					CARP( GRIPE_INFO, "_proc_q kill " << &(*(top_req.vvsegment_ptr)) << "\n" )
					in_result = _curvature_split((*(top_req.vvsegment_ptr)).first,
												 (*(top_req.vvsegment_ptr)).second,
												 (*(top_req.vvsegment_ptr)).third);
						switch (in_result)//FOLDUP
						{
							case Lop_triangulation_2::TOO_CLOSE_TO_INPUT_VERTEX:  //fall through
							case Lop_triangulation_2::YIELDED_TO_SEGMENT_SPLIT : //fall through
							case Lop_triangulation_2::NO_ATTEMPT :               //fall through
							case Lop_triangulation_2::POINT_ADDED : break;			 //no action
						};//UNFOLD
					break;//UNFOLD
				case Work_request::SPLIT_SEGMENT ://FOLDUP
					CARP( GRIPE_FORKS, "_process_queue is ::SPLIT_SEGMENT\n" )
					CARP( GRIPE_INFO, "_proc_q kill " << &(*(top_req.vvsegment_ptr)) << "\n" )
					split_point = 
						(*(top_req.vvsegment_ptr)).third->split_point( (top_req.provoker), p_loc );
					in_result = _split_segment((*(top_req.vvsegment_ptr)).first,
												 (*(top_req.vvsegment_ptr)).second,
												 (*(top_req.vvsegment_ptr)).third,
												 Lop_triangulation_2::SEGMENT_POINT,
												 split_point,p_loc,true,check_skinny);
						switch (in_result)//FOLDUP
						{
							case Lop_triangulation_2::TOO_CLOSE_TO_INPUT_VERTEX:  //push back on
								the_Q.push_req(top_req); break;
							case Lop_triangulation_2::YIELDED_TO_SEGMENT_SPLIT : //fall through
							case Lop_triangulation_2::NO_ATTEMPT :               //fall through
							case Lop_triangulation_2::POINT_ADDED : break;			 //no action
						};//UNFOLD
					break;//UNFOLD
				case Work_request::YIELD_TO_SEGMENT ://FOLDUP
					CARP( GRIPE_FORKS, "_process_queue is ::YIELD_TO_SEGMENT\n")
					split_point = (*(top_req.vvsegment_ptr)).third->split_point(p_loc);
					in_result = _split_segment((*(top_req.vvsegment_ptr)).first,
												 (*(top_req.vvsegment_ptr)).second,
												 (*(top_req.vvsegment_ptr)).third,
												 Lop_triangulation_2::SEGMENT_POINT,
												 split_point,p_loc,true,check_skinny);
						switch (in_result)//FOLDUP
						{
							case Lop_triangulation_2::TOO_CLOSE_TO_INPUT_VERTEX:  //push back on
								the_Q.push_req(top_req); break;
							case Lop_triangulation_2::YIELDED_TO_SEGMENT_SPLIT : //fall through
							case Lop_triangulation_2::NO_ATTEMPT :               //fall through
							case Lop_triangulation_2::POINT_ADDED : break;			 //no action
						};//UNFOLD
					break;//UNFOLD
				case Work_request::SPLIT_FACE ://FOLDUP
					CARP( GRIPE_FORKS, "_process_queue is ::SPLIT_FACE\n" )
					if (the_DT_and_ET.tds().is_face(
									(*(top_req.triangle_ptr)).first, 
									(*(top_req.triangle_ptr)).second,
									(*(top_req.triangle_ptr)).third, a_face))
					{
//#if PARANOID > CHECK_BIZARRE
//						if (the_DT_and_ET.touches_bounding_tri(a_face))
//						{ CARP( GRIPE_INFO, "_proc_q kills touchface: " << &(*a_face) << "\n")
//						} else { CARP( GRIPE_INFO, "_proc_q kills okface\n") }
//#endif

						Insert_hint ihint;
						ihint.split_face = &a_face;
						in_result = _insert_point(the_DT_and_ET.circumcenter(a_face),
														Lop_triangulation_2::FACE_CIRCUMCENTER,ihint,
														true, check_skinny);
						switch (in_result)//FOLDUP
						{
							case Lop_triangulation_2::YIELDED_TO_SEGMENT_SPLIT : 		//push back on
								the_Q.push_req(top_req); break;
							case Lop_triangulation_2::NO_ATTEMPT : 									//fall through
							case Lop_triangulation_2::POINT_ADDED : 								//fall through
							case Lop_triangulation_2::TOO_CLOSE_TO_INPUT_VERTEX: 
																												break;				//no action
						};//UNFOLD
					} //else not a tri, don't worry;
					break;//UNFOLD
				default: 
					CARP( GRIPE_INFO, "_process_queue is ::OTHER type?\n" )
					CARP( GRIPE_INFO, "_process_queue value: " << (top_req.wtype) << "\n" )
					break;
			}

		}
		CARP( GRIPE_GATES, "exit _process_queue\n" )
	}; //_process_queue
//UNFOLD

	//_split_ball//FOLDUP
	//
	//find degree of vh in the endpoint table.
	//create new vector of same size as old vector.
	//foreach segobj pointed to:
	//{
	//	get proper splitting point.
	//	insert_point(new centerpoint,no_yield,true,check_skinny). 
	//	get vhandle.
	//	for both subsegs
	//		check_encroached(vhandl1,vhandl2,segobj,true,true)
	//		check_encroached(vhandl1,vhandl2,segobj,true,false?)
	//	add segobj to new vector
	//}
	//
	//find smallest edge emanating from vertex..
	//}
	template < class Kernel_, class TriangulationDataStructure_2 >
		void Lop_triangulation_2<Kernel_, TriangulationDataStructure_2>::_split_ball(
			const Vertex_handle & corner_v, const FT & cut_distance_squared,
			const bool & check_encroached, const bool & check_skinny)
		{
			//first temporarily set own distsq to zero to prevent checking problems; 
			//fix it later
			CARP( GRIPE_GATES, "_split_ball called\n" )
			CARP( GRIPE_INFO,  "_split_ball : cut_dist_sq: " << \
							 lopping::FT2double< Kernel_ >(cut_distance_squared) << "\n")
			the_disq_table.set_disq(corner_v,FT(0));

			typedef typename DT_and_ET::Segment_table::K_V_Map K_V_Map;

			K_V_Map *opp_the_corner = new K_V_Map( *(the_DT_and_ET.seg_data.flatten(corner_v)) );
			typename DT_and_ET::Segment_table::Second_iterator
				opp_walk = (*opp_the_corner).begin();
			typename DT_and_ET::Segment_table::Second_iterator
				opp_end = (*opp_the_corner).end();

			Point split_point;
			Split_hints   p_loc;

			int vdeg = the_DT_and_ET.degree(corner_v);
			CARP( GRIPE_INFO, "_split_ball corner_v has deg " << vdeg << "\n" )
			CARP( GRIPE_INFO, "_split_ball corner_v is " )
			CARPVH( GRIPE_INFO, corner_v)

#if PARANOID > CHECK_BIZARRE
			if (vdeg < 2) { 
				_write_ps("catchend.ps");
				//the_DT_and_ET.gripe_segs(std::cerr); 
			}
			CGAL_precondition( vdeg > 1 );
#else
			//just to shut up warnings
			vdeg = 0;
#endif

			int i = 0;

			while (opp_walk != opp_end)
			{
				++i;
				CARP( GRIPE_LOOPS, "_split_ball walk " << i << " of " << vdeg << "\n" )

				CARPVH( GRIPE_INFO, (*opp_walk).first )
				CARP( GRIPE_INFO, "_split_ball oppwalk find split_point\n" )
				split_point = (*opp_walk).second->split_point(cut_distance_squared,true,p_loc);

				CARP( GRIPE_LOOPS, "_split_ball call splitseg\n" )
				_split_segment(corner_v,(*opp_walk).first,(*opp_walk).second,
											 Lop_triangulation_2::BALL_SPLITTER,
											 split_point,p_loc,check_encroached,check_skinny);
				//FIX: now kill the edge? or does split_segment do that?
				CARP( GRIPE_LOOPS, "_split_ball oppwalk exit loop\n" )
				++opp_walk;
			}

			//fix it later
			the_disq_table.set_disq(corner_v,cut_distance_squared);

			CARP( GRIPE_GATES, "_split_ball exit\n" )
		}; //_split_ball//UNFOLD

	//_split_segment//FOLDUP
	//	create a new segment object by splitting the old, possibly with
	//		the given point, if pointhandle non null.
	//	insert_point(new centerpoint,no_yield,true,check_skinny). 
	//	for both subsegs
	//	check_encroached(vhandl1,vhandl2,segobj,true,true)
	template < class Kernel_, class TriangulationDataStructure_2 >
		typename Lop_triangulation_2<Kernel_, TriangulationDataStructure_2>::Insert_result
		Lop_triangulation_2<Kernel_, TriangulationDataStructure_2>::_split_segment(
				 const Vertex_handle &vtx1,const Vertex_handle &vtx2,
				 const Segment_handle &s_hand, 
				 const Insert_type &itype,
				 const Point &split_point,
				 const Split_hints &p_loc,
				 const bool &check_encroached,
				 const bool &check_skinny)
		{
			CARP( GRIPE_GATES, "_split_segment called\n" )
			CARP( GRIPE_INFO, "_split_segment killing segment: " )
			CARPVH( GRIPE_INFO, vtx1)
			CARP( GRIPE_INFO, "              <==============>: " )
			CARPVH( GRIPE_INFO, vtx2)
			CARP( GRIPE_INFO, "_split_segment by inserting ")
			CARPP( GRIPE_INFO, split_point)

			bool add_to_queue = 
				((itype == Lop_triangulation_2::SEGMENT_POINT) ||
				 (itype == Lop_triangulation_2::FACE_CIRCUMCENTER) ||
				 (itype == Lop_triangulation_2::BALL_SPLITTER) );
			//FIX: added BALL_SPLITTER here. is it wrong?


			//FIX: if the segment is part of an infinite face,
			//then remove that face and replace it with two
			//subfaces????

			//only split if the segment exists in lookup table?
			//FIX: the s-hand is not necessary if we look it up. eliminate.
			//uhoh, now it is necessary. so keep it?
			if (the_DT_and_ET.has_segment( vtx1, vtx2 ))
			{
				CARP( GRIPE_FORKS, "_split_segment now erase seg\n" )
				the_DT_and_ET.erase_segment(vtx1,vtx2);

				//check if on the chull
				typename TDS_2::Face_handle    a_face;
				int                            v_index;
	
				bool forms_edge = the_DT_and_ET.tds().is_edge(vtx1,vtx2,(a_face),(v_index));
				bool on_chull = (forms_edge) && 
						( (the_DT_and_ET.is_infinite( (a_face)->vertex(v_index) )) ||
						  (the_DT_and_ET.is_infinite( (a_face)->mirror_vertex(v_index) )) );

				Insert_hint ihint;
				if (on_chull)
				{ ihint.split_edge = new Edge(a_face,v_index); }
				else { ihint.split_edge = 0; }
	
				//give a hint? near vtx1?
				Vertex_handle new_v;

				Insert_result the_ret = 
					_insert_point(split_point,itype,ihint,check_encroached,check_skinny,new_v);

				if (the_ret == Lop_triangulation_2::POINT_ADDED)
				{
					CARP( GRIPE_LOOPS, "_split_segment now check encroached\n" )
					CARP( GRIPE_INFO, "vtx1,vtx2,and the split vertex:\n")
					CARPVH( GRIPE_INFO, vtx1)
					CARPVH( GRIPE_INFO, vtx2)
					CARPVH( GRIPE_INFO, new_v)
					CARP( GRIPE_INFO, "endpoints:\n")
					CARPP( GRIPE_INFO, (s_hand->end_point(1)))
					CARPP( GRIPE_INFO, (s_hand->end_point(2)))

					Segment_wrapper* seg1 = new Segment_wrapper(*s_hand,split_point,true,p_loc);
					Segment_wrapper* seg2 = new Segment_wrapper(*s_hand,split_point,false,p_loc);
		
					CARP( GRIPE_GATES, "checking subsegment 1\n")
					_is_seg_encroached(vtx1,new_v,seg1,true,add_to_queue);
					CARP( GRIPE_GATES, "checking subsegment 2\n")
					_is_seg_encroached(new_v,vtx2,seg2,true,add_to_queue);
				} else //back up on that one.
				{ the_DT_and_ET.insert_segment(vtx1,vtx2,s_hand); }

				if (forms_edge) { delete ihint.split_edge; }

				CARP( GRIPE_GATES, "_split_segment returns after split\n" )
				return the_ret;
			} 

			CARP( GRIPE_GATES, "_split_segment: terminates NO_ATTEMPT\n" )
			return Lop_triangulation_2::NO_ATTEMPT;
		}; //_split_segment//UNFOLD

	//_curvature_split//FOLDUP
	//do a curvature split
	template < class Kernel_, class TriangulationDataStructure_2 >
		typename Lop_triangulation_2<Kernel_, TriangulationDataStructure_2>::Insert_result
		Lop_triangulation_2<Kernel_, TriangulationDataStructure_2>::_curvature_split(
				 const Vertex_handle &vtx1,const Vertex_handle &vtx2,
				 const Segment_handle &s_hand)
		{
			CARP( GRIPE_GATES, "_curvature_split called\n" )

			Split_hints p_loc;
			Point split_point;

			//FIX: we shouldn't have to check this more than once
			//for a cubic bezier, and not at all for the others.
			//why have it in for generality?
			if (!(s_hand->has_inflection_point(split_point,p_loc)))
			{
				split_point = s_hand->tv_split_point(p_loc);
				CARP( GRIPE_INFO, "_curvature_split inserting tv split point\n" )
			} 
#if PARANOID
			else {
				CARP( GRIPE_INFO, "_curvature_split inserting inflection point\n" )
			}
			CARP( GRIPE_INFO, "_curvature_split p_loc is " << p_loc << "\n" )
			CARPP( GRIPE_INFO, split_point )
#endif


			Insert_type itype = Lop_triangulation_2::CURVATURE_SPLITTER;

			//only split if the segment exists in lookup table?
			//FIX: the s-hand is not necessary if we look it up. eliminate.
			//uhoh, now it is necessary. so keep it?
			if (the_DT_and_ET.has_segment( vtx1, vtx2 ))
			{
				CARP( GRIPE_LOOPS, "_curvature_split now erase seg\n" )
				the_DT_and_ET.erase_segment(vtx1,vtx2);

				//check if on the chull
				typename TDS_2::Face_handle    a_face;
				int                            v_index;
	
				bool forms_edge = the_DT_and_ET.tds().is_edge(vtx1,vtx2,(a_face),(v_index));
				bool on_chull = (forms_edge) && 
						( (the_DT_and_ET.is_infinite( (a_face)->vertex(v_index) )) ||
						  (the_DT_and_ET.is_infinite( (a_face)->mirror_vertex(v_index) )) );

				Insert_hint ihint;
				if (on_chull)
				{ ihint.split_edge = new Edge(a_face,v_index); }
				else { ihint.split_edge = 0; }
	
				//give a hint? near vtx1?
				Vertex_handle new_v;

				Insert_result the_ret = 
					_insert_point(split_point,itype,ihint,false,false,new_v);

				if (the_ret == Lop_triangulation_2::POINT_ADDED)
				{
					CARP( GRIPE_INFO, "_curvature_split now check curvature\n")
					CARPVH( GRIPE_INFO, vtx1)
					CARPVH( GRIPE_INFO, vtx2)
					CARPVH( GRIPE_INFO, new_v)

					Segment_wrapper* seg1 = new Segment_wrapper(*s_hand,split_point,true,p_loc);
					Segment_wrapper* seg2 = new Segment_wrapper(*s_hand,split_point,false,p_loc);

					_insert_seg_check_curv(vtx1,new_v,seg1);
					_insert_seg_check_curv(new_v,vtx2,seg2);

				} else //back up on that one.
				{ the_DT_and_ET.insert_segment(vtx1,vtx2,s_hand); }

				if (forms_edge) { delete ihint.split_edge; }

				CARP( GRIPE_GATES, "_curvature_split returns after split\n" )
				return the_ret;
			} 

			CARP( GRIPE_GATES, "_curvature_split returns NO_ATTEMPT\n" )
			return Lop_triangulation_2::NO_ATTEMPT;
		}; //_curvature_split//UNFOLD

	//_insert_seg_check_curv//FOLDUP
	//insert the segment into the ET and DT table.
	//and check curvature.
	template < class Kernel_, class TriangulationDataStructure_2 >
		void
		Lop_triangulation_2<Kernel_, TriangulationDataStructure_2>::_insert_seg_check_curv(
				const Vertex_handle &vtx1, const Vertex_handle &vtx2, 
				const Segment_handle &s_hand)
		{
			the_DT_and_ET.insert_segment(vtx1,vtx2,s_hand);
			if (s_hand->estimate_total_curvature() > curvature_limit)
			{
				the_Q.push_curvature_split( vtx1, vtx2, s_hand, 
																			s_hand->circumradius_squared() );
			}
		}; //_insert_seg_check_curv//UNFOLD

	//_is_seg_encroached//FOLDUP
	// see if is edge from vhandle1,vhandle2 in tds. 
	//	if so, add edge->segobj in segtable
	template < class Kernel_, class TriangulationDataStructure_2 >
		bool Lop_triangulation_2<Kernel_, TriangulationDataStructure_2>::_is_seg_encroached(
				const Vertex_handle &vtx1,const Vertex_handle &vtx2,
				const Segment_handle &s_hand,const bool add_to_end_table,
				const bool add_to_queue)
		{
			CARP( GRIPE_GATES, "_is_seg_encroached called\n" )
#if PARANOID > CHECK_BIZARRE
			//check to be sure
			CGAL_precondition( the_DT_and_ET.tds().is_vertex(vtx1) );
			CGAL_precondition( the_DT_and_ET.tds().is_vertex(vtx2) );
#endif

			//add both orders. 
			if ( add_to_end_table ) { 
				CARPVH( GRIPE_INFO, vtx1)
				CARPVH( GRIPE_INFO, vtx2)
				the_DT_and_ET.insert_segment(vtx1,vtx2,s_hand); 
			}

			//edge interpretation:
			typename TDS_2::Face_handle    a_face;
			int                            v_index;

			bool forms_edge = the_DT_and_ET.tds().is_edge(vtx1,vtx2,(a_face),(v_index));
			bool is_encroached_this_order = false;
			bool is_encroached_mirror_order = false;

			if (forms_edge) {
				CARP( GRIPE_FORKS, "_is_seg_encroached isedge; check encd\n")
				is_encroached_this_order = 
					s_hand->encroaches((a_face)->vertex(v_index)->point());
				is_encroached_mirror_order = 
					s_hand->encroaches((a_face)->mirror_vertex(v_index)->point());
			} else
			{
				CARP( GRIPE_FORKS, "_is_seg_encroached is not edge\n")
			}


			bool is_encroached = 
				(!forms_edge) || is_encroached_this_order || is_encroached_mirror_order;
			
			if (add_to_queue && is_encroached)
			{
				CARP( GRIPE_FORKS, "_is_seg_encroached seg IS enc'd;\n" )
				CARP( GRIPE_FORKS, "_is_seg_encroached add to queue.\n" )
				if (!forms_edge) 
				{//edge is buried, so is encroached
					typename TDS_2::Vertex_circulator check_vtx = 
						the_DT_and_ET.incident_vertices(vtx1);
					typename TDS_2::Vertex_circulator end_vtx = check_vtx;
					Vertex_handle provoker;
					bool found_enc = false;
					do { 
						if (found_enc = (s_hand->encroaches( (*check_vtx).point() )))
							{ provoker = check_vtx; }
					} while ((!found_enc) && (++check_vtx != end_vtx));

					if (found_enc)
					{
						the_Q.push_split_segment( vtx1, vtx2, s_hand, provoker,
																					s_hand->circumradius_squared() );
					} else
					{
						CARP( GRIPE_FORKS, "_is_seg_encroached does not find encroacher of buried? panic!\n" )
						the_Q.push_yield_to_segment( vtx1, vtx2, s_hand, 
																					s_hand->circumradius_squared() );
					}
				} else if (is_encroached_this_order)
				{
					the_Q.push_split_segment( vtx1, vtx2, s_hand, (a_face)->vertex(v_index),
																		s_hand->circumradius_squared() );
				} else if (is_encroached_mirror_order)
				{
					the_Q.push_split_segment( vtx1, vtx2, s_hand, (a_face)->mirror_vertex(v_index),
																					s_hand->circumradius_squared() );
				}
			}

			CARP( GRIPE_GATES, "_is_seg_encroached terminates\n" )
			return is_encroached;
		}; //_is_seg_encroached//UNFOLD

	//_insert_point//FOLDUP
	//FIX: add hints
	//
	//get cavity iterator for point in triangulation.
	//	foreach edge, lookitup in endpoint lookup. if a segment,
	//	and killed, or on boundary and encroached by new point then, 
	//		if do_yield, add first one with yield to q, quit false.
	//		if not, add to q, without? provoking new point.
	//add point to triangulation, getting vertex handle.
	//if check_skinny, check all new faces for poor quality, adding kill reqs
	//	to queue.
	template < class Kernel_, class TriangulationDataStructure_2 >
		typename Lop_triangulation_2<Kernel_, TriangulationDataStructure_2>::Insert_result
			Lop_triangulation_2<Kernel_, TriangulationDataStructure_2>::_insert_point(
				const Point &p,
				const Insert_type &itype,
				const Insert_hint &ihint,
				const bool & check_encroached,
				const bool & check_skinny,
				Vertex_handle &put_vh)
		{
			CARP( GRIPE_GATES, "_insert_point called\n" )
			CARP( GRIPE_INFO, "_insert_point inserting: ")
			CARPP( GRIPE_INFO, p )

			bool really_check_disq = 
				((itype != Lop_triangulation_2::BALL_SPLITTER) &&
				 (itype != Lop_triangulation_2::CURVATURE_SPLITTER));

#if PARANOID
			if (check_skinny) 
					{ CARP( GRIPE_INFO, "flag set check_skinny\n" ) }
			if (really_check_disq) 
					{ CARP( GRIPE_INFO, "flag set really_check_disq\n" ) }
			if (check_encroached) 
					{ CARP( GRIPE_INFO, "flag set check_encroached\n" ) }
#endif

			typedef typename 
				Lop_triangulation_2<Kernel_,TriangulationDataStructure_2>::Segment_handle 
				Segment_handle;
			Segment_handle killed_seg;

			CARP( GRIPE_LOOPS, "_insert_point check badness\n" )
			if (check_encroached || really_check_disq) //check killed & boundary edges.//FOLDUP
			{
				std::list<Face_handle> conflict_faces;
    		std::list<Edge>  boundary_edges;
				typename std::list<Face_handle>::iterator fit;
				typename std::list<Edge>::iterator bit;

//				std::vector<Face_handle> conflict_faces;
//				std::vector<Edge> boundary_edges;
//				typename std::vector<Face_handle>::iterator fit;
//				typename std::vector<Edge>::iterator bit;
	
				CARP( GRIPE_LOOPS, "_insert_point find confs & bdry?\n" )
				the_DT_and_ET.get_conflicts_and_boundary(p,
					std::back_inserter(conflict_faces),
					std::back_inserter(boundary_edges));

				Face_handle    			 					 a_face;
				int                            v_index;

				CARP( GRIPE_LOOPS, "_insert_point check bdry for close inputvs\n" )
				//check boundary edges for close input vertices//FOLDUP
	    	if (really_check_disq)
				{
					bit = boundary_edges.begin();
					a_face = (*bit).first;
					v_index = (*bit).second;

					if (the_disq_table.is_close(a_face->vertex(the_DT_and_ET.cw(v_index)), p))
					{

						CARP( GRIPE_INFO, "_insert_point found close inputv: " )
						CARPVH( GRIPE_INFO, \
											 a_face->vertex(the_DT_and_ET.cw(v_index)) )
						CARP( GRIPE_INFO, "close to " )
						CARPP( GRIPE_INFO, p );

						//FIX: does this need fixed?
						if (itype != Lop_triangulation_2::FACE_CIRCUMCENTER)
						{ the_Q.push_split_ball( (a_face->vertex(the_DT_and_ET.cw(v_index))) ); }

						return Lop_triangulation_2::TOO_CLOSE_TO_INPUT_VERTEX;
					}
					
					while (bit != boundary_edges.end())
					{
						a_face = (*bit).first;
						v_index = (*bit).second;
	
						if (the_disq_table.is_close(a_face->vertex(the_DT_and_ET.ccw(v_index)), p))
						{

							CARP( GRIPE_INFO, "_insert_point found close inputv: " )
							CARPVH( GRIPE_INFO, a_face->vertex(the_DT_and_ET.ccw(v_index)) )
							CARP( GRIPE_INFO, "close to " )
							CARPP( GRIPE_INFO, p );

//							GRIPE( "dist is " << the_disq_table.get_disq( (a_face->vertex(the_DT_and_ET.ccw(v_index))) ).to_double() << " \n" );

							if (itype != Lop_triangulation_2::FACE_CIRCUMCENTER)
							{ the_Q.push_split_ball( (a_face->vertex(the_DT_and_ET.ccw(v_index))) ); }

							return Lop_triangulation_2::TOO_CLOSE_TO_INPUT_VERTEX;
						}
						++bit;
					}
	
				} //walk boundary edges//UNFOLD

//not necessary since we check every edge in the cavity//FOLDUP
//			GRIPE( "_insert_point check boundary for enc'd edges\n" );
//				//check boundary edges for encroached edges//FOLDUP
//				if (check_encroached)
//				{
//		    	for(bit = boundary_edges.begin(); bit != boundary_edges.end(); bit++)  
//					{
//						a_face = (*bit).first;
//						v_index = (*bit).second;
//	
//						if (the_DT_and_ET.has_segment(*bit,&killed_seg))//b_edge is seg//FOLDUP
//						{ //the check if encroached.
//							if ((*killed_seg).encroaches(p))
//							{
//								the_Q.push_yield_to_segment( 
//											(a_face->vertex(the_DT_and_ET.ccw(v_index))),
//											(a_face->vertex(the_DT_and_ET.cw(v_index))),
//											killed_seg, (*killed_seg).circumradius_squared() );
//								if (itype == Lop_triangulation_2::FACE_CIRCUMCENTER)
//								{ return Lop_triangulation_2::YIELDED_TO_SEGMENT_SPLIT; }
//							} //check encroached
//						} //check if a segment edge//UNFOLD
//					}
//				} //walk boundary edges//UNFOLD
//	//UNFOLD
				CARP( GRIPE_LOOPS, \
								 "_insert_point check faces for killed/encroached edges\n" )
				//check encroached faces for killed edges//FOLDUP
	    	for(fit = conflict_faces.begin(); fit != conflict_faces.end(); fit++)  
				{
					//FIX: you have to check each of these for encroachment. pooh.
					//check each edge
	    		for(v_index=0;v_index < 3;++v_index)
					{
						if (the_DT_and_ET.has_segment( (*fit)->vertex( the_DT_and_ET.ccw(v_index) ),
																				(*fit)->vertex( the_DT_and_ET.cw(v_index) ),
																			 &killed_seg))
						{ //check for encroachment.
							if ((*killed_seg).encroaches(p))
							{
								//FIX: when should a provoker be used?
								//here i have it such that a circumcenter yield is
								//not used as a provoker, but a midpoint is.
								if (itype == Lop_triangulation_2::FACE_CIRCUMCENTER)
								{ 
									the_Q.push_yield_to_segment( 
												((*fit)->vertex(the_DT_and_ET.ccw(v_index))),
												((*fit)->vertex(the_DT_and_ET.cw(v_index))),
												killed_seg, (*killed_seg).circumradius_squared() );
									return Lop_triangulation_2::YIELDED_TO_SEGMENT_SPLIT; 
								} else
								{
									the_Q.push_split_segment( 
												((*fit)->vertex(the_DT_and_ET.ccw(v_index))),
												((*fit)->vertex(the_DT_and_ET.cw(v_index))),
												killed_seg, p,
												(*killed_seg).circumradius_squared() );
								}
							} //check encroached
						}
						//FIX: add data to faces. if it will get killed,
						//remove it from poor quality queue? or will that
						//not save any time?
					}
				} //walk each encroached face//UNFOLD

			}//end check_enc or check_disq //UNFOLD

			//FIX: should you use ihint in other cases?
			CARP( GRIPE_LOOPS, "_insert_point now insert using hint\n" )
			if (itype == Lop_triangulation_2::FACE_CIRCUMCENTER)
			{
				put_vh = the_DT_and_ET.insert(p,*(ihint.split_face));
//			} else if (ihint.split_edge != 0)
//			{ put_vh = the_DT_and_ET.insert_in_edge(p,(*(ihint.split_edge)).first,
//																							(*(ihint.split_edge)).second); 
			} else
			{ put_vh = the_DT_and_ET.insert(p); }

			CARP( GRIPE_LOOPS, "_insert_point insert done\n" )

			if (check_skinny) //check new faces for skinnyness.//FOLDUP
			{
				typename TDS_2::Face_circulator start_face = 
					the_DT_and_ET.incident_faces(put_vh);
				typename TDS_2::Face_circulator adjacent_face = start_face;
				do { _check_quality(adjacent_face); } while (++adjacent_face != start_face);
			}//end check skinny//UNFOLD

			CARP( GRIPE_GATES, "_insert_point to exit\n" )
			return Lop_triangulation_2::POINT_ADDED;
		}; //_insert_point//UNFOLD

} 

#endif //LOP_TRIANGULATION_2_CPP

