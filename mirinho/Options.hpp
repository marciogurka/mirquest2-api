/*
############################################################################
#                                Mirinho				   #
############################################################################
#                                                                          #
# Copyright INRIA 2015                                                     #
#                                                                          #
# Contributors : Susan Higashi						   #
#                Cyril Fournier 					   #
#                Christian Gautier                                         #
#                Christine Gaspin                                          #
#                Marie-France Sagot                                        #
#                                                                          #
# susanhigashi@gmail.com                                                   #
# marie-france.sagot@inria.fr                                              #
#                                                                          #
# This software is a computer program whose purpose is to predict 	   #
# animal and plant precursor miRNAs in an efficient way. It accepts as 	   #
# input genomic and deep sequencing data.				   # 
#                                                                          #
# This software is governed by the CeCILL  license under French law and    #
# abiding by the rules of distribution of free software.  You can  use,    # 
# modify and/ or redistribute the software under the terms of the CeCILL   #
# license as circulated by CEA, CNRS and INRIA at the following URL        #
# "http://www.cecill.info".                                                # 
#                                                                          #
# As a counterpart to the access to the source code and  rights to copy,   #
# modify and redistribute granted by the license, users are provided only  #
# with a limited warranty  and the software's author,  the holder of the   #
# economic rights,  and the successive licensors  have only  limited       #
# liability.                                                               #
#                                                                          #
# In this respect, the user's attention is drawn to the risks associated   #
# with loading,  using,  modifying and/or developing or reproducing the    #
# software by the user in light of its specific status of free software,   #
# that may mean  that it is complicated to manipulate,  and  that  also    #
# therefore means  that it is reserved for developers  and  experienced    #
# professionals having in-depth computer knowledge. Users are therefore    #
# encouraged to load and test the software's suitability as regards their  #
# requirements in conditions enabling the security of their systems and/or #
# data to be ensured and,  more generally, to use and operate it in the    # 
# same conditions as regards security.                                     #
#                                                                          #
# The fact that you are presently reading this means that you have had     #
# knowledge of the CeCILL license and that you accept its terms.           #
############################################################################
*/

// =============================================================================
/// 	@file Options.h

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include"Definitions.hpp"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<iostream>
#include<string>

using namespace std;

// =============================================================================
/// 	OptionsClass
/// 	\brief Parse command-line options. 

class Options 
{
	public:
		
		/// Constructor 
		/// Set default values for the variables.
		Options( void );
		
		/// \fn void Options::usage( void )
		/// \brief Print usage message.
		void usage( void );

		/// \fn void Options::setupOptions( int argc, char** argv )
		/// \brief Setup command line options and its respectives values.
		/// \param arc Number of arguments. 
		/// \param argv Array with the arguments.
		void setupOptions( int argc, char** argv );
	
		/// \fn string Options::get_outname( string ifile )
		/// \brief
		/// \param
		string get_outname( string ifile );

		// ---

		/// \var fname Name of the input file containing the genome sequence(s) in FASTA format;
		/// -i fasta_file
		string ifile;

		/// \var output Name of the output file containing the data about the pre-miRNA filtered candidates;
		/// -o output_file
		string ofile;

		/// \var st_arm Length of the stem arm;
		/// -a st_arm_length---default: 25nt
		unsigned int st_arm;

		/// \var stl_min Minimum length of the stem loop;
		/// -n st_loop_min_length---default: 5nt
		unsigned int stl_min;
		
		/// \var stl_max Maximum length of the stem loop;
		/// -x st_loop_max_length---default: 20 nt
		unsigned int stl_max;

		/// \var limit
		/// -l energy_threshold---default:  kcal/mol
		float limit;
    
		/// \var dw
		/// -w width of the diagonal---default: 3 ( means max width = 2*dw-1 = 5 )
		unsigned int dw;
    	
		/// \var param
		/// -p path_to_parameters
		string path2param;

		/// \var al_type Global or glocal.
		int al_type;

		/// \var 
		string genome_known;

		/// \var
		string mirnas_mature;
		
		/// \var secondary structure
		int ss;
};

// =============================================================================

inline void Options::setupOptions( int argc, char** argv )
{
	if( argc == 1 || argc > 18 )
	{
		usage();
		exit(1);
	}

	int c; 
	size_t foundSlash;
	string ofilename; 

	while ( (c = getopt( argc, argv, "i:o:a:n:x:l:p:w:s:h" )) != -1 )
	{
		switch (c)
		{
			case 'i':
				ifile = optarg;
				break;
			case 'o':
				ofile = optarg;
				
				foundSlash = ofile.find_last_of( "/" );
				
				if( foundSlash != string::npos ) // if found
				{
					if( ofile.size()-1 == foundSlash ) 		// output_filename: not given
					{
						ofilename = get_outname( ifile );
						ofile += ofilename;
					}
					else if( ofile.size()-1 > foundSlash )		// output_filename: given
					{
						foundSlash = ofile.find( "_out.fa" );//it is not a slash, just reusing memory

						if( foundSlash == string::npos )
						{
							ofilename = ofile + "_out.fa";
							ofile = ofilename;
						}
					}
				}

				break;
			case 'a':
				if( atoi(optarg) < 2 )
				{
					cout << "option -- a: The length of the stem arm should be at least 2nt." << endl;
					exit(1);
				}
				st_arm = atoi(optarg);
				break;
			case 'n': 
				if( atoi(optarg) < 3 )
				{
					cout << "option -- n: The length of the terminal loop should be at least 3nt." << endl;
					exit(1);
				}
				stl_min = atoi(optarg); 
				break;
			case 'x': 
				stl_max = atoi(optarg);
				break;
			case 'l': 
				limit = atof(optarg);
				break;
			case 'p': 
				path2param = optarg;
				break;
			case 'w':
				if(atoi(optarg) < 1 || atoi(optarg) > (int)st_arm)
				{
					cout << "option -- w: Width diagonal must be included in range 1 - " << st_arm << endl;
					exit(1);
				}
				dw = atoi(optarg);
				break;
			case 's':
				ss = atoi(optarg);
				break;
			/*case 't': 
				if( atoi(optarg) != 0 && atoi(optarg) != 1 )
				{
					cout << "option -- t: Unknown value (0 for global or 1 for glocal alignment)." << endl;
				}
				al_type = atoi(optarg);
			case 'g': 
				genome_known = optarg;
				break;
			case 'm':
				mirnas_mature = optarg;
				break;*/
			case 'h':
				usage();
				exit(1);
		}
	}

	if( (mirnas_mature != "" && genome_known == "") || (mirnas_mature == "" && genome_known != "") )
	{
		cout << "option -- m and g: Options -g and -m are complementary to each other: if one option is set the other must be set too." << endl;
		exit(1);
	}
	
	if( ifile == "" )
	{
		cout << "option is mandatory -- i" << endl;
		exit(1);
	}
	
	if( ofile == "" )
	{
		ofile = get_outname( ifile );
	}
}

// =============================================================================

inline string Options::get_outname( string ifile )
{
	size_t foundSlash = ifile.find_last_of( "/" );
	size_t foundDot = ifile.find( ".fa" ); 

	string filename; 

	if( foundDot != string::npos )
	{
		filename = ifile.substr( foundSlash+1, (foundDot-foundSlash-1) );
		filename += "_out.fa";
	}
	else
	{
		filename = ifile + "_out.fa";
	}

	return filename;
}

// =============================================================================

inline void Options::usage( void )
{
	cout << "Usage: ./mirinho [OPTIONS] -i sequence.fasta[.gz]" << endl;
	cout << "\t-a PARAM\tLength of the stem-arm - default: " << ST_ARM << "nt" << endl;
	cout << "\t-n PARAM\tMinimum length of the terminal loop - default: " << STL_MIN << "nt" << endl;
	cout << "\t-x PARAM\tMaximum length of the terminal loop - default: " << STL_MAX << "nt" << endl;
	cout << "\t-l PARAM\tFree energy threshold - default: " << LIMIT << " kcal/mol " << endl;
	cout << "\t-o PARAM\tOutput file name - default: input_out.fa" << endl;
	cout << "\t-p PARAM\tPath to parameters folder - default: ./" << endl;
	cout << "\t-w PARAM\tWidth of the diagonal for alignment - default: " << DW << endl;
	cout << "\t-s PARAM\tPrint or not the secondary structure: 1 for printing, 0 otherwise - default: 1" << endl;
}
 
#endif
