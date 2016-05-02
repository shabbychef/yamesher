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
 * $Id: Lop_triangulation_2_test.cpp,v 1.14 2005/03/16 00:45:01 spav Exp $
 * $Log: Lop_triangulation_2_test.cpp,v $
 * Revision 1.14  2005/03/16 00:45:01  spav
 * adding quick and dirty Chew mode
 *
 * Revision 1.13  2005/02/12 01:37:16  spav
 * fixed xfig read bug.
 *
 * Revision 1.11  2005/01/18 22:47:10  spav
 * changing provoker to runtime option.
 *
 * Revision 1.10  2005/01/03 03:59:54  spav
 * home from san fran.
 *
 * Revision 1.9  2004/12/21 22:42:57  spav
 * looks like it is working; added command line parser.
 *
 * Revision 1.8  2004/12/10 23:52:29  spav
 * pong back to you.
 *
 * Revision 1.7  2004/11/29 07:05:03  spav
 * post thanksgiving
 *
 * Revision 1.6  2004/11/23 23:43:31  spav
 * appears to work for straight segs. now add arcs, bezier.
 *
 * Revision 1.5  2004/11/17 00:04:38  spav
 * it compiles in this crippled form.
 *
 * Revision 1.4  2004/11/04 23:31:25  spav
 * got a running readinput--still cannot hook it up, though.
 *
 * Revision 1.3  2004/11/04 02:40:38  spav
 * pong
 *
 * Revision 1.2  2004/11/02 23:51:19  spav
 * checking in.
 *
 * Revision 1.1  2004/11/02 22:43:40  spav
 * moving lopper test to LT_2_test
 *
 */
//END CVSLOG
//END HEADER

#include "Lop_triangulation_2_test.h"

int  main(int argc, char * argv[])
{
#ifdef USE_HIGH_PRECISION
	VOICE( GRIPE_INFO, "using high precision\n")
#else
	VOICE( GRIPE_INFO, "using low  precision\n")
#endif //USE_HIGH_PRECISION

	Lop_mesh the_mesh;
	std::string infile;
	std::string outfile;
	std::string filext;

	try {   //parse command line//FOLDUP
    // Define the command line object.
   	TCLAP::CmdLine cmd("Corner Lopping Planar Mesh Generator", ' ', "0.00");

    // the groom factor 
    TCLAP::ValueArg<double> gammaArg("g","gamma",
			"Initial groom reduction factor (0.414213562373095)",false,
			0.414213562373095,"double");
    cmd.add( gammaArg );

    // the ball reduction ratio
    TCLAP::ValueArg<double> betaArg("b","beta","Ball reduction ratio (0.75)",false,
				0.75,"double");
    cmd.add( betaArg );

    // the squared edge radius ratio
    TCLAP::ValueArg<double> qualArg("q","quality",
			"Squared edge-radius ratio, i.e., 4 sin^2(kappa), (0.5)",false,0.5,"double");
    cmd.add( qualArg );

    // the curvature limit
    TCLAP::ValueArg<double> tauArg("t","tau",
			"Curvature limit (1.04719755119660)",false,1.04719755119660,"double");
    cmd.add( tauArg );

    // the upper bound on circumradius squared
    TCLAP::ValueArg<double> chewArg("C","cirumradius",
			"Uniform Circumradius Squared Upper bound (0)",false,0.0,"double");
    cmd.add( chewArg );

    // the maximum degree of a vertex
    TCLAP::ValueArg<int> degArg("d","maxdeg","Maximum Vertex Degree w/out ball (3)",
			false,3,"int");
    cmd.add( degArg );

		// whether to use provokers
		TCLAP::SwitchArg useProvoSwitch("u","use_provoker",
																		"Use provokers to split segments (false)", false);
		cmd.add( useProvoSwitch );

#ifdef PARANOID
    // the paranoia level 
    TCLAP::ValueArg<int> paranoiaArg("V","verbosity","Paranoia verbosity level (2)",
																		 false,2,"int");
#else
    TCLAP::ValueArg<int> paranoiaArg("V","verbosity","Verbosity level (2)",
																		 false,2,"int");
#endif //PARANOID

    cmd.add( paranoiaArg );


    // the number of iterations to run
    TCLAP::ValueArg<int> itlArg("L","itlimit","Maximum iterations (500)",
			false,500,"int");
    cmd.add( itlArg );

		// the input file name
		TCLAP::UnlabeledValueArg<std::string>  
			infileArg( "filename", "name of input file","input.fig","string");
    cmd.add( infileArg );

		TCLAP::UnlabeledValueArg<std::string>  
			outfileArg( "outfile", "name of output file","outfile.pcmp","string");
    cmd.add( outfileArg );

    // Parse the args.
    cmd.parse( argc, argv );

		the_mesh.groom_factor = FT(gammaArg.getValue());
		the_mesh.ball_reduction_ratio = FT(betaArg.getValue());
		the_mesh.squared_edge_radius_ratio = FT(qualArg.getValue());
		the_mesh.curvature_limit = FT(tauArg.getValue());
		the_mesh.cradius_squared_limit = FT(chewArg.getValue());
		the_mesh.max_deg = degArg.getValue();
		the_mesh.TERMLIMIT = itlArg.getValue();

		Lop_mesh::Segment_wrapper::set_provoker( useProvoSwitch.getValue() );
		LUVERBOSITY = paranoiaArg.getValue();

		infile  = infileArg.getValue();
		outfile = outfileArg.getValue();

//    SwitchArg reverseSwitch("r","reverse","Print name backwards", false);
//    cmd.add( reverseSwitch );
//    bool reverseName = reverseSwitch.getValue();

	} catch (TCLAP::ArgException &e)  // catch any exceptions
	{ std::cerr << "error: " << e.error() << " for arg " << e.argId() << "\n"; }
//UNFOLD

	const char* inputfname = infile.c_str();
	const char* pcmpname = outfile.c_str();

	int in_len = infile.length();
	filext = infile.substr(in_len - 4,4);

	//get input//FOLDUP
	std::ifstream input_file;
	input_file.open(inputfname);
	if (input_file.good())
	{
		std::cerr << "now going to read file " << inputfname;
		if (filext.compare(".fig") == 0) {
			std::cerr << " as .fig file\n";
			the_mesh.read_xfig_file(input_file);
		} else if (filext.compare(".dat") == 0)
		{
			std::cerr << " as .dat file\n";
			the_mesh.read_input(input_file);
		} else if (filext.compare(".wdb") == 0)
		{
			std::cerr << " as .wdb file\n";
			the_mesh.read_wdb_file(input_file);
		} else
		{
			std::cerr << " unknown file extension: " << filext << "\n";
		}
		std::cerr << "read finished \n";
	} else
	{
		std::cerr << "could not open " << inputfname << "\n";
	}
	input_file.close();
	//UNFOLD

	//fix the mesh
	the_mesh.remove_conflicts();

	//write the output file//FOLDUP
//	std::ofstream output_file;
//	output_file.open(oname);
//	if (input_file.good())
//	{
//		std::cerr << "now going to write file " << oname << "\n";
//		output_file << the_mesh.the_DT_and_ET;
//		std::cerr << "write finished \n";
//	} else
//	{
//		std::cerr << "could not open " << oname << "\n";
//	}
//	output_file.close();//UNFOLD

	std::cerr << "now write the pcmp file " << pcmpname << "\n";
	the_mesh.the_DT_and_ET.write_pcmp(pcmpname);

	return 0;
};


