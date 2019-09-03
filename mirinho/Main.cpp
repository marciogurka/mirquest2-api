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
///	@file Main.cpp
///	@brief Mirinho is a miRNA predictor which uses a quadratic complexity 
///		algorithm to approximate the secondary structure of a pre-miRNA. 
///		It is much faster than the other methods availble in the literature 
///		that use cubic complexity algorithms. 

#include "Align.hpp"
#include "Definitions.hpp"
#include "Energy.hpp"
#include "kseq.h"
#include "Options.hpp"
#include "Sequence.hpp"

#include<dirent.h>
#include<time.h>
#include<zlib.h>

#include<algorithm>
#include<iostream>
#include<fstream>

using namespace std;

// =============================================================================

KSEQ_INIT(gzFile, gzread) // fasta parser (by Heng Li)

void AssessCandidates( Sequence* pSq, Align& al, Options& opt, ofstream& ofile );

// =============================================================================

int main(int argc, char **argv)
{
	Options opt; 
	opt.setupOptions(argc, argv);

	// --- Directory for the NN free energy parameters
	DIR *pDir = opendir( opt.path2param.c_str() );
	if( pDir == NULL )
	{
		cout << "Directory for energy parameters was not found. Please specify its path with -p option." << endl;
		return EXIT_FAILURE;
	}
    	closedir(pDir);
   
   	// --- Threshold
	 
    	if( opt.genome_known != "" )
	{
		Util *pU = new Util();

		vector<PosType> pos;
		string filename( opt.mirnas_mature );
		pU->parse_gff( filename, pos );
		
		string genome_file( opt.genome_known );
		string genome_random;

		pU->gen_random( pos, genome_file, genome_random );

		//TODO: implement in Util a method compute the threshold

		cout << genome_random << endl;
	
		delete pU; 
	}

	// --- Input file
	gzFile ifile = gzopen(opt.ifile.c_str(), "r");
	if( ifile == NULL )
	{
		cout << "Could not open input file: " << opt.ifile << endl;
		return EXIT_FAILURE;
	}
	kseq_t *kseq = kseq_init(ifile);
	int l;

    	// --- Processing sequences  
	Align al = Align( opt.path2param, opt.al_type, opt.st_arm, opt.stl_max );
	ofstream ofile( opt.ofile.c_str() );

	while( (l = kseq_read(kseq)) >= 0 )
	{
		string seqTmp = kseq->seq.s;
		string sequenceId = kseq->name.s;
        
		// --- Convert all non-determined letters in N
		Util *pU = new Util();

		string sequence = pU->clean_sequence( seqTmp );
		
		delete pU;

		// ---

		if( sequence.size() < (2 * opt.st_arm) + opt.stl_min )
		{
			cout << "Sequence " << sequenceId << " should be longer than " \
			     << (2 * opt.st_arm) + opt.stl_min << " nt; skipping sequence" \
			     << endl;

			continue;
		}
        
		// ---

                cout << "Sequence " << sequenceId << " on fire (" << sequence.size() << "nt)" << endl;

		// --- Extract and assess miRNAs candidadtes on the forward sequence 5'-> 3'
		string strand( "forward" );

		Sequence *sqFor = new Sequence( sequence, sequenceId, strand ); 
        
		AssessCandidates( sqFor, al, opt, ofile );

		delete sqFor;
		
		// --- Extract and assess miRNAs candidadtes on the reverse sequence 3'-> 5'
		replace( sequence.begin(), sequence.end(), 'A', 't' );
		replace( sequence.begin(), sequence.end(), 'T', 'a' );
		replace( sequence.begin(), sequence.end(), 'C', 'g' );
		replace( sequence.begin(), sequence.end(), 'G', 'c' );

		transform( sequence.begin(), sequence.end(), sequence.begin(), ::toupper );

		strand = "reverse";

		Sequence *sqRev = new Sequence( sequence, sequenceId, strand );
        
		AssessCandidates( sqRev, al, opt, ofile );
		
		delete sqRev;
		
	}
	
	kseq_destroy(kseq);
	
	gzclose( ifile );
	
	cout << "Output was written in: " << opt.ofile << endl;
	ofile.close();
	
	return EXIT_SUCCESS;
}

// =============================================================================
/// 	\fn void AssessCandidates( Sequence *sq, Align& al, Options& opt )
/// 	\brief Look for miRNAs candidates in a sequence.
/// 	\param sq 		pointer to Sequence 
/// 	\param al		object for the alignemnt 
/// 	\param opt		object with the options set by the user

void AssessCandidates( Sequence* pSq, Align& al, Options& opt, ofstream& ofile )
{
	for( unsigned int i = 0; i < pSq->seq.size() - (opt.st_arm + opt.stl_max + opt.st_arm); ++i )
	{
		if( pSq->seq.size() < i+(2*opt.st_arm)+opt.stl_min)
		{
			break;
		}
				
		string preMirna = pSq->seq.substr( i, (2 * opt.st_arm) + opt.stl_max );
		size_t found = preMirna.find_last_of( 'N', preMirna.size()-1 );
		
		if( found == string::npos )
		{
			string stArm1 = pSq->seq.substr( i, opt.st_arm );

			int loop_size = -opt.stl_min;
			float energy_min = opt.limit;
			vector<PairType> align_path;
			
			string stArm2; 
			for( unsigned int loopSize = opt.stl_min; loopSize < opt.stl_max+1; ++loopSize )
			{


				stArm2 = pSq->seq.substr( i+opt.st_arm+loopSize, opt.st_arm );
		
				reverse( stArm2.begin(), stArm2.end() );
	    
				al.forward_diagonal( stArm1, stArm2, opt.dw );

				if( not al.backward_diagonal( stArm1, stArm2, stArm1.size(), stArm2.size(), opt.dw, 0, true ) )
				{
					continue;
				}

				// --- Compute the free energy
				vector<PairType> path = al.get_path();
	    
				float energy = eSYM; 			// Symmetry penalty
				
				energy += al.compute_stack( path );	// Stacking energy

				energy += al.compute_dangle( path );	// Dangling nucleotide

				energy += al.compute_internal( path );	// Internal loops (=mismatches)

				if( energy > opt.limit ){ continue; }	

				energy += al.compute_bulge( path );	// Bulges (=gaps)
				
				// --- Just mirna candidates under the threshold
				if( energy < energy_min )
				{
					loop_size = loopSize;
					energy_min = energy; 
					align_path = path;
				}
			}
		
			// --- Write the miRNA candidates features  
			if( align_path.size() > 0 )
			{
				stringstream sst; 
				sst <<  pSq->id 			<< ":" <<\
					i+1 				<< ":" <<\
					i+1+(2*opt.st_arm)+loop_size-1 	<< ":" <<\
					i+1+opt.st_arm 			<< ":" <<\
					i+1+opt.st_arm+loop_size-1 	<< ":" <<\
					energy_min 			<< ":" <<\
					pSq->strand;
				
				// header 	
				string header = sst.str();

				// sequence
				string sequence = pSq->seq.substr( i, 2*opt.st_arm+loop_size );
				if( pSq->strand == "reverse" )
				{
					reverse( sequence.begin(), sequence.end() );
				}

				ofile << ">" << header << endl;
				ofile << sequence << endl;

				// print SS
				if( opt.ss == 1 )
				{
					Util *pU = new Util();

					pU->print_ss( align_path, sequence, ofile, opt.st_arm, loop_size, pSq->strand );
					ofile << energy_min << endl;

					delete pU;
				}
			}
		}
		else
		{
			i += found;
		}
	}
}
