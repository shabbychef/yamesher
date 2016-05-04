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
 * $Id: lookups.h,v 1.24 2005/02/12 01:37:16 spav Exp $
 * $Log: lookups.h,v $
 * Revision 1.24  2005/02/12 01:37:16  spav
 * fixed xfig read bug.
 *
 * Revision 1.22  2005/01/03 03:59:54  spav
 * home from san fran.
 *
 * Revision 1.21  2004/12/21 22:42:57  spav
 * looks like it is working; added command line parser.
 *
 * Revision 1.20  2004/12/01 00:02:47  spav
 * seems to work for arcs. start to add bezier.
 *
 * Revision 1.19  2004/11/29 07:05:04  spav
 * post thanksgiving
 *
 * Revision 1.18  2004/11/23 23:43:31  spav
 * appears to work for straight segs. now add arcs, bezier.
 *
 * Revision 1.17  2004/11/23 05:39:02  spav
 * nearly done
 *
 * Revision 1.16  2004/11/23 02:50:36  spav
 * nearly got it...
 *
 * Revision 1.15  2004/11/22 23:31:32  spav
 * seems to work, but problem with degenerate chull edge/faces
 *
 * Revision 1.14  2004/11/19 23:48:24  spav
 * some fixes
 *
 * Revision 1.13  2004/11/19 07:35:00  spav
 * problems with disq_table vs. actual length. i.e. it is splitting ball segments inadvertently.
 *
 * Revision 1.12  2004/11/19 02:10:55  spav
 * something odd: an infinite vertex and repeated ballsplitting
 *
 * Revision 1.11  2004/11/18 02:48:21  spav
 * syntactically ok; a semantic mess past groom..
 *
 * Revision 1.10  2004/11/17 00:04:38  spav
 * it compiles in this crippled form.
 *
 * Revision 1.9  2004/11/15 06:20:03  spav
 * converted to merge end_table and the DT
 *
 * Revision 1.8  2004/11/11 02:41:29  spav
 * fixed map_to_map?
 *
 * Revision 1.7  2004/11/10 06:26:29  spav
 * end table is screwy..
 *
 * Revision 1.6  2004/11/04 23:31:25  spav
 * got a running readinput--still cannot hook it up, though.
 *
 * Revision 1.5  2004/11/02 23:51:20  spav
 * checking in.
 *
 * Revision 1.4  2004/10/29 05:26:50  spav
 * pong.
 *
 * Revision 1.3  2004/10/26 23:48:04  spav
 * gaining momentum./
 *
 * Revision 1.2  2004/10/23 00:38:58  spav
 * checking in.
 *
 * Revision 1.1  2004/10/20 21:50:26  spav
 * starting code project
 *
 *
 */
//END CVSLOG
//END HEADER

#ifndef LOOKUPS_H
#define LOOKUPS_H

#include "config.h"

#define LOOKUPS_BOUNDING_BOX_EXPANSION_FACTOR 1.3

#include "utility.h"
#include "segment_base.h"
#include <map>
#include <vector>
#include <functional>
#include <iostream>
#include <string>

namespace lopping {

	//lookup the ``radius'' of an input vertex//FOLDUP
	template < class Kernel_, class TriangulationDataStructure_2 >
	class Distance_squared_table : public 
		std::map< typename TriangulationDataStructure_2::Vertex_handle, typename Kernel_::FT >
	{
		public: 
			typedef TriangulationDataStructure_2 												Tds_2;
			typedef typename Tds_2::Vertex_handle												Vertex_handle;
			typedef Kernel_                                             Kernel;
			typedef typename Kernel::Point_2 														Point;
			typedef typename Kernel::FT    														  FT;
			typedef std::map< Vertex_handle, FT >                       VFT_Map;
			typedef typename VFT_Map::iterator													VFT_iterator;

			FT get_disq(const Vertex_handle &v)
			{ if (count(v) > 0) { return (*(find(v))).second; } 
				else { 
#if PARANOIA > CHECK_BIZARRE
					CARP( GRIPE_INFO, "nofound disq\n")
#endif
					return FT(0); } };

			//return true if |dist_v - test_v|^2 < map(dist_v)
			bool is_close(const Vertex_handle &dist_v, const Point &test_p)
			{
				FT dist_squared = ((test_p - dist_v->point()) * (test_p - dist_v->point()));
				return (dist_squared < get_disq( dist_v ));
			}

			void set_disq(const Vertex_handle &dist_v, const FT &its_disq)
			{ this->VFT_Map::operator[](dist_v) = its_disq; };

			//if v_handle is in the map, set its value; else complain?
			void or_set_disq(const Vertex_handle &dist_v, const FT &its_disq)
			{ 
				//FIX: add an error?
				if (count(v) > 0) { set_disq(v,its_disq); } 
#if PARANOIA > CHECK_BIZARRE
				else { CARP( GRIPE_INFO, "BAD SET\n") } 
#endif
			};

	};//UNFOLD

	//Flat_Iterator//FOLDUP
	//takes two map iterators
	template < class K_, class V_, class C_ = std::less<K_> >
	class Flat_Iterator 
//	this doesn't work:
//	: public std::pair<
//		typename std::map< K_, std::map< K_, V_, C_ >, C_ >::iterator,
//		typename std::map< K_, V_, C_ >::iterator >
	{
		public: 
			typedef K_ K;
			typedef V_ V;
			typedef C_ C;

			typedef std::map< K_, V_, C_ > K_V_Map;
			typedef std::map< K_, K_V_Map, C_ > K_K_V_Map;

			typedef typename K_K_V_Map::iterator    				First_iterator;
			typedef typename K_V_Map::iterator       			 Second_iterator;

		protected:

			First_iterator      _first_it;
			Second_iterator 		_second_it;

		private:
			K_K_V_Map *_the_map;
		public:


			typedef lopping::Triplet<K_, K_, V_>								     K_K_V;

			Flat_Iterator(const First_iterator &it1,const Second_iterator &it2,
										K_K_V_Map *parent) : 
				_first_it(it1), _second_it(it2), _the_map(parent)
				{ };

			K first(void) { return _first_it->first; }
			K second(void) { return _second_it->first; }
			V third(void) { return _second_it->second; }

			//prefix
			Flat_Iterator& operator++(void)
			{
				//the end value is near end first, end second
				if (_second_it == (_first_it->second).end())
					{ return (*this); }

				//o.w. increment.
				++_second_it;
				while ((_second_it) == (_first_it->second).end())
					{ ++_first_it; 
						if (_first_it == _the_map->end())
							{ --_first_it; return (*this); }
						_second_it = (_first_it->second).begin(); 
					}
				return *this;
			};
			//postfix
			Flat_Iterator operator++(const int)
			{
				Flat_Iterator mycopy = *this;
				++(*this); return mycopy;
			};
			bool operator==(const Flat_Iterator &comp)
				{ return ((_first_it == comp._first_it) && 
									(_second_it == comp._second_it)); }
			bool operator!=(const Flat_Iterator &comp)
				{ return ((_first_it != comp._first_it) ||
									(_second_it != comp._second_it)); }

	};//UNFOLD

	//Increasing_Flat_Iterator//FOLDUP
	//takes two map iterators
	template < class K_, class V_, class C_ = std::less<K_> >
	class Increasing_Flat_Iterator : public Flat_Iterator<K_,V_,C_>
	{
		public: 
			typedef K_ K;
			typedef V_ V;
			typedef C_ C;
			typedef Flat_Iterator<K_,V_,C_> Flat_Iterator;

			typedef typename Flat_Iterator::K_V_Map 					K_V_Map;
			typedef typename Flat_Iterator::K_K_V_Map					K_K_V_Map;
			typedef typename Flat_Iterator::First_iterator		First_iterator;
			typedef typename Flat_Iterator::Second_iterator		Second_iterator;

		private:
			static const C *_the_comparator;

			bool _is_increasing(void) 
				{ return ((_second_it == (_first_it->second).end()) ||
						(*_the_comparator)(_first_it->first,_second_it->first)); };

		public:
			Increasing_Flat_Iterator(const First_iterator &it1,const Second_iterator &it2,
										K_K_V_Map *parent) : Flat_Iterator(it1,it2,parent) 
				{ if (!_is_increasing()) { ++(*this); } };

			//prefix
			Increasing_Flat_Iterator& operator++(void)
			{
				do { this->Flat_Iterator::operator++(); } 
					while (!_is_increasing());
				return *this;
			};
			//postfix
			Increasing_Flat_Iterator operator++(const int)
			{
				Increasing_Flat_Iterator mycopy = *this;
				++(*this); return mycopy;
			};


	};
	template < class K_, class V_, class C_ = std::less<K_> >
		const C_* Increasing_Flat_Iterator<K_,V_,C_>::_the_comparator = new C_();
//UNFOLD

	//map to a map//FOLDUP
	template < class K_, class V_, class C_ = std::less<K_> >
	class Map_to_Map : public std::map< K_, std::map< K_, V_, C_ >, C_ >
	{
		public:
			typedef K_ K;
			typedef V_ V;
			typedef C_ C;

			typedef std::map< K_, V_, C_ > K_V_Map;
			typedef std::map< K_, K_V_Map, C_ > K_K_V_Map;

			typedef typename K_K_V_Map::iterator    				First_iterator;
			typedef typename K_V_Map::iterator       			 Second_iterator;

			typedef typename K_V_Map::size_type      			 			 size_type;

			typedef Flat_Iterator< K_, V_, C_ > 						 Flat_Iterator;
			typedef Increasing_Flat_Iterator< K_, V_, C_ >
				Increasing_Flat_Iterator;


			//insert,erase,count//FOLDUP
			void single_insert(const K &k1, const K &k2, const V &va)
				{ ((*this)[k1])[k2] = va; }
					
			void double_insert(const K &k1, const K &k2, const V &va)
				{ single_insert(k1,k2,va);
				  single_insert(k2,k1,va); }

			void single_erase(const K &k1, const K &k2)
				{ ((*this)[k1]).erase(k2);
					if (! ((*this)[k1]).size()) { this->erase(k1); }
				};
			void double_erase(const K &k1, const K &k2)
				{ single_erase(k1,k2);single_erase(k2,k1); };

			size_type degree(const K &k1) 
				{ if (count(k1))
					{ return (*this)[k1].size();
					} else { return count(k1); }
				};

			size_type count(const K &k1) 
				{ return (*this).K_K_V_Map::count(k1); };

			size_type count(const K &k1, const K &k2) const
				{ if ((*this).K_K_V_Map::count(k1))
					{ return (*(this->find(k1))).second.count(k2); } else
					{ return (*this).K_K_V_Map::count(k1); }
				};//UNFOLD

			V & operator()(const K &k1, const K &k2)
				{ return ((*this)[k1])[k2]; }

			K_V_Map* flatten(const K &k1)
				{
					if (count(k1))
					{
						return &((*this)[k1]);
					} else
					{ return new K_V_Map(); }
				};
			
			Flat_Iterator findtwo(const K &k1,const K &k2)
				{
					if (count(k1))
					{ First_iterator fit = find(k1);
						if ((fit->second).count(k2))
							{ return Flat_Iterator(fit,(fit->second).find(k2),this); } 
					}
					return flatend();
				};
	
			//iterate on both
			Flat_Iterator flatbegin(void)
				{ return Flat_Iterator(this->K_K_V_Map::begin(),
															 (this->K_K_V_Map::begin())->second.begin(),
															 this); };

			Flat_Iterator flatend(void)
				{ First_iterator near_end = this->K_K_V_Map::end();
					--near_end;
					return Flat_Iterator(near_end,near_end->second.end(),this); };

			//fix the first key!
			Flat_Iterator flatbegin(const K &k1)
				{ First_iterator my_start = this->K_K_V_Map::find(k1);
					return Flat_Iterator(my_start, my_start->second.begin(), this); };

			Flat_Iterator flatend(const K &k1)
				{ First_iterator my_start = this->K_K_V_Map::find(k1);
					++my_start;
					if (my_start == this->K_K_V_Map::end())
					{
						--my_start;
						return Flat_Iterator(my_start,my_start->second.end(),this); 
					} 
					return Flat_Iterator(my_start,my_start->second.begin(),this);
				};

			Increasing_Flat_Iterator increasingflatbegin(void)
				{ return Increasing_Flat_Iterator(this->K_K_V_Map::begin(),
															 (this->K_K_V_Map::begin())->second.begin(),
															 this); };

			Increasing_Flat_Iterator increasingflatend(void)
				{ First_iterator near_end = this->K_K_V_Map::end();
					--near_end;
					return Increasing_Flat_Iterator(near_end,near_end->second.end(),this); };

	};//UNFOLD

	//triangulation with edge, vertex lookups //FOLDUP
	template < class DT_2 >
	class DT_and_Edge_Table : virtual public DT_2
	{
		public: 
			typedef typename DT_2::Geom_traits											Kernel;

			typedef typename Kernel::FT    													FT;
			typedef typename Kernel::Point_2 												Point;

			typedef typename DT_2::Vertex_handle            				Vertex_handle;
			typedef typename DT_2::Face_handle              				Face_handle;
			typedef typename DT_2::Triangulation_data_structure  		Tds;
			typedef typename Tds::Edge                      				Edge;

			typedef Segment_wrapper< Kernel >												Segment_wrapper;
			typedef Segment_wrapper* 																Segment_handle;
			typedef Map_to_Map< Vertex_handle, Segment_handle > 		Segment_table;

			typedef typename Segment_table::size_type      					size_type;
			typedef typename Segment_table::Flat_Iterator      			Flat_Iterator;
			typedef typename Segment_table::Increasing_Flat_Iterator      
																									 Increasing_Flat_Iterator;

			typedef typename Segment_table::First_iterator          First_iterator;
			typedef typename Segment_table::Second_iterator         Second_iterator;

			//member stuff
			Segment_table	seg_data;

		private:
			Vertex_handle _vtop;
			Vertex_handle _vlef;
			Vertex_handle _vrig;
			bool          _bounding_tri_set;
		public:

			//c-tor
			DT_and_Edge_Table(void) : DT_2(), seg_data(), 
				_vtop(),_vlef(),_vrig(),_bounding_tri_set(false) {};

			Segment_handle & operator[](const Edge &ed)
			{
				return seg_data( (ed.first)->vertex(this->cw(ed.second)), 
													(ed.first)->vertex(this->ccw(ed.second)));
			};

			//insert and erase//FOLDUP
			void insert_segment(const Vertex_handle &v1,
													const Vertex_handle &v2,
													const Segment_handle &seg)
				{ 
					CARP( GRIPE_GATES, "lookups::insert_segment inserting \n")

					CARPVH( GRIPE_INFO, v1)
					CARPVH( GRIPE_INFO, v2)
					CARPP( GRIPE_INFO, seg->end_point(1))
					CARPP( GRIPE_INFO, seg->end_point(2))

					seg_data.single_insert(v1,v2,seg);

					CARP( GRIPE_GATES, "lookups::insert_segment reverse inserting \n")
					CARPVH( GRIPE_INFO, v2)
					CARPVH( GRIPE_INFO, v1)
					const Segment_handle shrev = (*seg).reverse();
					seg_data.single_insert(v2,v1,shrev); 
				};

			//FIX: actually call delete on the seg?
			void erase_segment(const Vertex_handle &v1,
											const Vertex_handle &v2)
				{ return seg_data.double_erase(v1,v2); }
			void erase_segment(const Edge &ed)
			{
				return seg_data.double_erase(
					(ed.first)->vertex(this->cw(ed.second)), 
					(ed.first)->vertex(this->ccw(ed.second)));
			};
			//UNFOLD
			
			//check if an edge has data, return if it does
			bool has_segment(const Vertex_handle &v1,const Vertex_handle &v2)
			{
				Flat_Iterator do_find = seg_data.findtwo(v1,v2);
				return (do_find != seg_data.flatend());
			};
			//check if an edge has data, return if it does
			bool has_segment(const Vertex_handle &v1,const Vertex_handle &v2,
											 Segment_handle *put_seg)
			{
				Flat_Iterator do_find = seg_data.findtwo(v1,v2);
				if (do_find != seg_data.flatend())
				{
					*put_seg = (do_find.third());
					return true;
				} else { return false; }
			};
			//check if an edge has data, return if it does
			bool has_segment(const Edge &ed,Segment_handle *put_seg)
			{
				return has_segment( (ed.first)->vertex(this->cw(ed.second)), 
														(ed.first)->vertex(this->ccw(ed.second)), put_seg);
			};
			//check if an edge has data, return if it does
			bool has_segment(const Edge &ed)
			{
				return has_segment( (ed.first)->vertex(this->cw(ed.second)), 
														(ed.first)->vertex(this->ccw(ed.second)) );
			};

			size_type degree(const Vertex_handle &v) { return seg_data.degree(v); }

			void report_stuff(void)
			{
				if (LUVERBOSITY > GRIPE_ITERATES) 
				{
					VOICE( GRIPE_ITERATES, "******************************\n")
					VOICE( GRIPE_ITERATES, "num of vertices: " << (this->tds().number_of_vertices()) << " \n")
					VOICE( GRIPE_ITERATES, "   num of faces: " << (this->tds().number_of_faces()) << " \n")
					VOICE( GRIPE_ITERATES, "   num of edges: " << (this->tds().number_of_edges()) << " \n")
				} else if (!(this->tds().number_of_vertices() % GROSS_ITERATES))
				{
					VOICE( GRIPE_PROGRESS, "******************************\n")
					VOICE( GRIPE_PROGRESS, "num of vertices: " << (this->tds().number_of_vertices()) << " \n")
					VOICE( GRIPE_PROGRESS, "   num of faces: " << (this->tds().number_of_faces()) << " \n")
					VOICE( GRIPE_PROGRESS, "   num of edges: " << (this->tds().number_of_edges()) << " \n")
				}
			}
			
			std::ostream & write_ps(std::ostream& os);
//			std::ostream & write_msh(std::ostream& os);
			std::ostream & gripe_segs(std::ostream& os);
			//write a pcmp file
			std::ostream & write_pcmp(std::ostream& os);
			void write_pcmp(const char *fname)//FOLDUP
			{
				std::ofstream output_file;output_file.open(fname);
				if (output_file.good()) { write_pcmp(output_file); } else
					{ std::cerr << "could not open " << fname << "\n"; }
				output_file.close();
			};//UNFOLD

			//give squared distance to nearest vtx in the DT
			FT nn_squared(const Vertex_handle &vtx);

			void add_bounding_tri(void);

			bool touches_bounding_tri(const Vertex_handle &ck_v)
				{ return ( (ck_v == _vtop) || (ck_v == _vlef) || (ck_v == _vrig) ); };
			bool touches_bounding_tri(const Face_handle &a_face);

			bool is_inside_bounding_tri(const Point &check_point);

			//ensure the convex hull are segments or add a bounding box.
			//return true if the chull are segs.
			bool ensure_chull_are_segs(void);

			//find the maximum and minimum angles in the mesh.
			void find_min_max_angles(double &min_angle,double &max_angle);

		private:
			//get the bounding box of the triangulation;
			//assumes there is no bounding triangle;
			void _get_bounding_box(FT &xmin,FT &xmax,FT &ymin,FT &ymax);

			//get the center and radius of circle which encloses the
			//bounding box.
			void _get_bbox_center(FT &xc,FT &yc,FT &rad);

			void gripe_edge(const Edge &ed) const
			{
				CARP( GRIPE_INFO, "the edge is: \n")
				CARPVH( GRIPE_INFO, (ed.first)->vertex(this->cw(ed.second)) )
				CARPVH( GRIPE_INFO, (ed.first)->vertex(this->ccw(ed.second)) )
				CARP( GRIPE_INFO, "|||\n")
			};

	};//UNFOLD

}

//yes, this is the dumbest shit ever, no i am not kidding, yes
//you have to include the template definition if you want this
//to work on the 2004 state of the art c++ compilers.

#include "lookups.cpp"

#endif //LOOKUPS_H

//lost and found
//	//Two_Key_Map//FOLDUP
//	template < class Key_, class Value_, class KeyCompare = std::less<Key_> >
//	class Two_Key_Map
//	{
//		public:
//			typedef Key_ Key;
//			typedef Value_ Value;
//			typedef KeyCompare Key_Compare;
//
//		private:
//			static Key_Compare _the_comparator;
//		public:
//
//			//Two_Key//FOLDUP
//			class Two_Key : public std::pair<Key,Key>
//			{
//				public:
//					Two_Key (void) : std::pair<Key,Key>() { };
//					Two_Key (const Key &k1,const Key &k2) : std::pair<Key,Key>(k1,k2) {};
//					bool is_increasing(void) const 
//						{ return _the_comparator(this->first,this->second); };
//			};//UNFOLD
//			//Lecicographic_less//FOLDUP
//			template <class Pair_, class Comparator_>
//			class Lexicographic_less 
//			{
//				public:
//					typedef Pair_ Pair;
//					typedef Comparator_ Comparator;
//				private:
//					Comparator _the_element_comparator;
//				public:
//
//					Lexicographic_less (void) : _the_element_comparator() {};
//					bool operator()(const Pair& x,const Pair& y) const
//						{ return ( _the_element_comparator(x.first,y.first) ||
//											 ( (! _the_element_comparator(y.first,x.first)) && 
//												 _the_element_comparator(x.second,y.second)) );
//						};
//			};//UNFOLD
//
//			typedef Lexicographic_less<Two_Key, KeyCompare >					Two_Key_Compare;
//
//			typedef std::map< Two_Key, Value, Two_Key_Compare >				Key_Key_Value_Map;
//			typedef typename Key_Key_Value_Map::value_type						value_type;
//			typedef typename Key_Key_Value_Map::iterator      				Key_Key_iterator;
//
//			//Flat_iterator//FOLDUP
//			class Flat_iterator : public Key_Key_iterator
//			{
//				public: 
//					Two_Key first(void) const
//						{ return Key_Key_iterator(*this).first; }
//					Value second(void) const
//						{ return Key_Key_iterator(*this).second; }
//
//					Flat_iterator(const Key_Key_iterator &convfrom) : Key_Key_iterator(convfrom)
//						{  };
//					//prefix
//					Flat_iterator& operator++(void)
//					{
//						this->Key_Key_iterator::operator++();
//						return *this;
//					};
//					//postfix
//					Flat_iterator operator++(const int dummy)
//					{
//						Flat_iterator mycopy = *this;
//						this->Key_Key_iterator::operator++(dummy);
//						return mycopy;
//					};
//			};//UNFOLD
//			//Increasing_flat_iterator//FOLDUP
//			class Increasing_flat_iterator : public Key_Key_iterator
//			{
//				public: 
////					Increasing_flat_iterator(const Flat_iterator &convfrom) : 
////						Flat_iterator(convfrom) {  };
//					Increasing_flat_iterator(const Key_Key_iterator &convfrom) : 
//						Key_Key_iterator(convfrom) {  };
//					
//					Two_Key first(void) const
//						{ return Key_Key_iterator(*this).first; }
//					Value second(void) const
//						{ return Key_Key_iterator(*this).second; }
//
//					//only want increasing iterator to be for an 
//					//increasing key. so advance to a valid one.
//					void advance_to_valid(void);
////						while ( (! ( ((*this).first()).is_increasing() )) && 
////										(*this) )
////							{ this->Key_Key_iterator::operator++(); }
//
//					//prefix
//					Increasing_flat_iterator& operator++(void)
//					{
//						this->Key_Key_iterator::operator++();
//						this->advance_to_valid();
//						return *this;
//					};
//					//postfix
//					Increasing_flat_iterator operator++(const int dummy)
//					{
//						Increasing_flat_iterator mycopy = *this;
//						this->Key_Key_iterator::operator++(dummy);
//						this->advance_to_valid();
//						return mycopy;
//					};
//			};//UNFOLD
//
//		private:
//			Key_Key_Value_Map _the_map;
//		public:
//
//			Flat_iterator flat_begin(void) { return Flat_iterator(_the_map.begin()); }
//			Flat_iterator flat_end(void) { return Flat_iterator(_the_map.end()); }
//
//			Increasing_flat_iterator increasing_flat_begin(void) 
//			{ 
//				Increasing_flat_iterator* the_it = 
//					new Increasing_flat_iterator(_the_map.begin());
//				the_it->advance_to_valid();
//				return *the_it;
//			}
//
//			Increasing_flat_iterator increasing_flat_end(void) 
//				{ return _the_map.end(); }
//
//			std::pair<Key_Key_iterator,bool> insert 
//				(const Key_ &key_1,const Key_ &key_2,const Value_ &val)
//				{ return _the_map.insert( value_type(Two_Key(key_1,key_2),val) ); };
//
//			Flat_iterator find (const Key_ &key_1,const Key_ &key_2,Value *found_val)
//				{
//					
//
//				};
//
//			//returns true if it found the double key val, returning it.
//			bool double_erase(const Key_ &key_1,const Key_ &key_2,Value *found_val);
//	};//UNFOLD
//	//segment table //FOLDUP
//	template < class Kernel_ >
//	class Segment_info
//	{
//		public: 
//			typedef Kernel_																					Kernel;
//			typedef typename Kernel_::Point_2 											Point;
//			typedef typename Kernel_::FT 														FT;
//
//			//which kind
//			enum segment_type {
//				STRAIGHT,
//				ARC
//			};
//			
//			//class data
//			segment_type seg_type;
//			Point center_point;
//	};//UNFOLD
//	//segment table //FOLDUP
//	template < class Kernel_, class TriangulationDataStructure_2 >
//	class Segment_table: public
//		Map_to_Map< typename TriangulationDataStructure_2::Vertex_handle, 
//								Segment_info< Kernel_ > >
//	{
//		public: 
//			typedef Kernel_																	Kernel;
//			typedef typename Kernel_::Point_2 							Point;
//			typedef typename Kernel_::FT 										FT;
//
//			typedef TriangulationDataStructure_2				    Tds;
//			typedef typename Tds::Vertex_handle            	Vertex_handle;
//			typedef typename Tds::Face_handle               Face_handle;
//			typedef typename Tds::Edge                      Edge;
//
//			typedef Segment_info< Kernel_ >									Segment_info;
//			typedef Map_to_Map< Vertex_handle, Segment_info > VVS_map;
//
//
//			typedef typename VVS_map::size_type      				size_type;
//			typedef typename VVS_map::Flat_Iterator      		Seg_Iterator;
//			typedef typename VVS_map::Increasing_Flat_Iterator      
//																											Increasing_Seg_Iterator;
//
//			static bool encroaches(const Seg_Iterator &seg, const Point &test_point) 
//			{
//
//
//			}
//
//
//
//	};//UNFOLD
