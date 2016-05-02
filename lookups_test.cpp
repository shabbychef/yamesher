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
 * $Id: lookups_test.cpp,v 1.5 2004/11/17 00:04:38 spav Exp $
 * $Log: lookups_test.cpp,v $
 * Revision 1.5  2004/11/17 00:04:38  spav
 * it compiles in this crippled form.
 *
 * Revision 1.4  2004/11/15 06:20:03  spav
 * converted to merge end_table and the DT
 *
 * Revision 1.3  2004/11/11 02:41:29  spav
 * fixed map_to_map?
 *
 * Revision 1.2  2004/11/04 02:40:39  spav
 * pong
 *
 * Revision 1.1  2004/10/29 05:26:50  spav
 * pong.
 *
 * Revision 1.1  2004/10/20 21:50:26  spav
 * starting code project
 *
 *
 */
//END CVSLOG
//END HEADER

#include "lookups_test.h"
#include <iostream>

int main()
{
	typedef lopping::Map_to_Map<int, float> IIFmap;
	typedef IIFmap::Flat_Iterator flat_it;
	typedef IIFmap::Increasing_Flat_Iterator inc_flat_it;
	IIFmap iif;

	iif.double_insert(1,3,4.051);
	std::cerr << "gots: 1,3," << iif(1,3) << "\n";
	std::cerr << "gots: 3,1," << iif(1,3) << "\n";
	iif.single_insert(4,22,3.1415);
	std::cerr << "gots: 4,22," << iif(4,22) << "\n";
	std::cerr << "gots: 22,4," << iif(22,4) << "\n";
	iif.double_erase(1,3);
	std::cerr << "gots: 1,3," << iif(1,3) << "\n";
	std::cerr << "gots: 3,1," << iif(1,3) << "\n";

	std::cerr << "number for 1,6: " << iif.count(1,6) << "\n";
	std::cerr << "number for 1,6: " << iif.count(1,6) << "\n";
	std::cerr << "number for 2,6: " << iif.count(1,6) << "\n";
	std::cerr << "number for 4,22: " << iif.count(4,22) << "\n";
	std::cerr << "number for 22,4: " << iif.count(22,4) << "\n";
	std::cerr << "number for 1,3: " << iif.count(1,3) << "\n";

	iif.single_insert(0,9,81.81);
	iif.single_erase(0,9);
	std::cerr << "number for 0,9: " << iif.count(0,9) << "\n";


	IIFmap jif;
	jif.double_insert(1,2,3.4);
	jif.double_insert(10,11,400.10);
	jif.double_insert(20,11,34.56);
	jif.double_insert(80,11,80118.1);

	int j = 0;


	std::cerr << "start flatbegin\n";
	flat_it fit = jif.flatbegin();
	while ((fit != jif.flatend()) && (++j!=17))
	{
		std::cerr << "call: (" << (*fit).first << ", " << (*fit).second << ", " <<
			(*fit).third << ")\n";
		++fit;
	}

		std::cerr << "degree of 1 is " << jif.degree(1) << "\n";
		std::cerr << "degree of 20 is " << jif.degree(20) << "\n";
		std::cerr << "degree of 11 is " << jif.degree(11) << "\n";

	std::cerr << "start bounded flatbegin\n";
	fit = jif.flatbegin(20);
	while (fit != jif.flatend(20))
	{
		std::cerr << "only 20's: got: (" << (*fit).first << ", " << (*fit).second << ", " <<
			(*fit).third << ")\n";
		++fit;
	}

	std::cerr << "start bounded flatbegin\n";
	fit = jif.flatbegin(11);
	while (fit != jif.flatend(11))
	{
		std::cerr << "only 11's: got: (" << (*fit).first << ", " << (*fit).second << ", " <<
			(*fit).third << ")\n";
		++fit;
	}



	std::cerr << "start incflatbegin\n";
	inc_flat_it ifit = jif.increasingflatbegin();
	while ((ifit != jif.increasingflatend()))
	{
		std::cerr << "call: (" << (*ifit).first << ", " << (*ifit).second << ", " <<
			(*ifit).third << ")\n";
		++ifit;
	}


	fit = jif.findtwo(10,11);

		std::cerr << "found: (" << (*fit).first << ", " << (*fit).second << ", " <<
			(*fit).third << ")\n";

	fit = jif.findtwo(66,66);

		std::cerr << "puke found: (" << (*fit).first << ", " << (*fit).second << ", " <<
			(*fit).third << ")\n";



	return 0;
};

//lost and found
////FOLDUP
//	Two_Key mykey_one(1,2);
//	Two_Key mykey_two(1,3);
//	Two_Key mykey_three(2,4);
//	
//	Two_Key_Compare new_comparator;
//
//	if (new_comparator(mykey_one,mykey_three))
//	{
//		std::cerr << "1,2  < 2,4\n";
//	}
//
//	if (new_comparator(mykey_three,mykey_two))
//	{
//		std::cerr << "2,4  < 1,3\n";
//	}

//	int_three     myints;
//	myints.insert(1,2,3);
//	myints.insert(2,4,3);
//	myints.insert(3,1,3);
//	myints.insert(2,9,3);
//	myints.insert(4,1,3);
//
//	int_three::Flat_iterator flat_walk = myints.flat_begin();
//	do
//	{
//		std::cerr << "walking in lookups.test.cpp called: "
//		<< flat_walk->first.first << flat_walk-> first.second << flat_walk->second
//		<< "\n";
//
//	} while (++flat_walk != myints.flat_end());
//	
//	std::cerr << "now start increasing walk: \n";
//
//	int_three::Increasing_flat_iterator inc_flat_walk = myints.increasing_flat_begin();
//	do
//	{
//		std::cerr << "walking in lookups.test.cpp called: "
//		<< inc_flat_walk->first.first << inc_flat_walk-> first.second 
//		<< inc_flat_walk->second << "\n";
//	} while (++inc_flat_walk != myints.increasing_flat_end());//UNFOLD
