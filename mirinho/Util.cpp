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
/// @file Util.cpp

#include "Options.hpp"
#include "Util.hpp"
#include "kseq.h"

#include<stack>  
#include<fstream> 

KSEQ_INIT(gzFile, gzread) // fasta parser (by Heng Li)

// =============================================================================

void Util::print_ss( vector<PairType>& align_path, string& sequence, ofstream& ofile, int stArm_size, int loop_size, string& strand )
{
	// === Bracket notation

	stack<char> itemNotationSS;
	// print notation for 5p arm
	for( unsigned int i = 0; i < align_path.size(); ++i ) 
	{
		if( align_path[i].type == MA )
		{
			ofile << "(";
			itemNotationSS.push(')'); 
		}
		else if( align_path[i].type == GL )
		{
			itemNotationSS.push('.'); 
		}
		else if( align_path[i].type == GU )
		{
			ofile << ".";
		}
		else //MI
		{
			ofile << ".";
			itemNotationSS.push('.'); 
		}
	}	
	
	// print notation for loop
	for( unsigned int i=0; i < loop_size; ++i )
	{
		ofile << ".";
	}
	
	// print notation for 3p loop 
	while( !itemNotationSS.empty() )
	{
		ofile << itemNotationSS.top();
		itemNotationSS.pop();
	}

	ofile << endl;

	// === Beautiful structure 

	// =====================================================
	// 	Top strand 

	// --- 1) MI_Stem

	for( int i = 0; i < align_path.size(); ++i )
	{
		if( align_path[i].type == MI ||  align_path[i].type == GL ||  align_path[i].type == GU )
		{
			if( strand == "forward" )
				ofile << align_path[i].nuc1;
			else
				ofile << align_path[i].nuc2;
		}
		else
		{
			ofile << " ";
		}
	}

	// --- 1) MI_Loop

	string loop = sequence.substr( stArm_size, loop_size );
	string loopTop, loopBot; 
	char hangNuc1, hangNuc2, hangNuc3; 
	hangNuc1 = hangNuc2 = hangNuc3 = ' '; 

	if( loop.size() % 2 == 0 )
	{
		loopTop = loop.substr( 0, loop.size()/2 );
		loopBot = loop.substr( loop.size()/2, loop.size()/2 );

		hangNuc1 = loopTop[ loopTop.size()-1 ];
		loopTop.erase( loopTop.size()-1, 1 );
	}
	else
	{
		loopTop = loop.substr( 0, (loop.size()/2)+1 );
		loopBot = loop.substr( (loop.size()/2)+1, loop.size()/2 );
			
		hangNuc1 = loopTop[ loopTop.size()-2 ]; // antepenultimo
		hangNuc2 = loopTop[ loopTop.size()-1 ];

		loopTop.erase( loopTop.size()-1, 1 );
		loopTop.erase( loopTop.size()-1, 1 );
	}
	hangNuc3 = loopBot[0]; 

	ofile << loopTop << endl;				

	// --- 2) MA_Stem

	for( int i = 0; i < align_path.size(); ++i ) 
	{
		if( align_path[i].type == MA )
		{
			if( strand == "forward" )
				ofile << align_path[i].nuc1;
			else
				ofile << align_path[i].nuc2;
		}
		else
		{
			ofile << " ";	
		}
	}
	
	// --- 2) MA_Loop
	
	for( int i = 0; i < loopTop.size(); ++i )
		ofile << " ";
	ofile << hangNuc1 << endl;	

	// =====================================================
	// 	Alignment 

	// --- 3) ALIGN_Stem

	for( int i = 0; i < align_path.size(); ++i ) 
	{
		if( align_path[i].type == MA )
			ofile << "|";
		else
			ofile << " ";
	}

	// --- 3) ALIGN_Loop

	for( int i = 0; i < loopTop.size(); ++i )
		ofile << " ";
	ofile << hangNuc2 << endl;	

	// =====================================================
	// 	Bottom strand 

	// --- 4) MA_Stem

	for( int i = 0; i < align_path.size(); ++i ) 
	{
		if( align_path[i].type == MA )
		{
			if( strand == "forward" )
				ofile << align_path[i].nuc2;
			else
				ofile << align_path[i].nuc1;
		}
		else
		{
			ofile << " ";	
		}
	}

	// --- 4) MA_Loop

	for( int i = 0; i < loopTop.size(); ++i )
		ofile << " ";
	ofile << hangNuc3 << endl;					

	// --- 5) MI_Stem 

	for( int i = 0; i < align_path.size(); ++i ) 
	{
		if( align_path[i].type == MI ||  align_path[i].type == GL ||  align_path[i].type == GU )
		{
			if( strand == "forward" )
				ofile << align_path[i].nuc2;
			else
				ofile << align_path[i].nuc1;
		}
		else
		{
			ofile << " ";
		}
	}				
	// --- 5) MI_Loop
	
	for ( string::reverse_iterator rit = loopBot.rbegin(); rit != loopBot.rend()-1; ++rit )
		 ofile << *rit;				

	ofile << endl;
}

// =============================================================================

void Util::gen_random( vector<PosType>& pos, string& genome_file, string& genome_random )
{
	gzFile file = gzopen( genome_file.c_str(), "r" );
	kseq_t *kseq = kseq_init( file );
	int l;

	// --- Get subsequeces: premirnas regions
	string preMirnas; 

	while( (l = kseq_read(kseq)) >= 0 )
	{
		string seqTmp = kseq->seq.s;
		string sequenceId = kseq->name.s;
		string sequence = clean_sequence( seqTmp );

		for( int i = 0; i < pos.size(); ++i)
		{
			if( sequenceId == pos[i].sequenceId )
			{
				string subseq = sequence.substr( pos[i].begin, pos[i].end - pos[i].begin + 1 );

				if( pos[i].strand == "-" )
					rev_comp( subseq );	

				preMirnas += subseq;
			}
		}
	}

	// --- Compute nucleotide frequency

	int a = count( preMirnas.begin(), preMirnas.end(), 'A' );
	a = a + count( preMirnas.begin(), preMirnas.end(), 'a' );

	int t = count( preMirnas.begin(), preMirnas.end(), 'T' );
	t = t + count( preMirnas.begin(), preMirnas.end(), 't' );

	int c = count( preMirnas.begin(), preMirnas.end(), 'C' );
	c = c + count( preMirnas.begin(), preMirnas.end(), 'c' );

	int g = count( preMirnas.begin(), preMirnas.end(), 'G' );
	g = g + count( preMirnas.begin(), preMirnas.end(), 'g' );

	int n = count( preMirnas.begin(), preMirnas.end(), 'N' );
	n = n + count( preMirnas.begin(), preMirnas.end(), 'N' );

	float freqA = a / (float)preMirnas.size();
	float freqT = t / (float)preMirnas.size();
	float freqC = c / (float)preMirnas.size();
	//float freqG = g / (float)preMirnas.size();
	
	// --- Generate random sequence
	for( unsigned int i = 0; i < preMirnas.size()-n; ++i )
	{
		float r = ((float) rand() / (RAND_MAX));

		if( r < freqA )
			genome_random += "A";
		else if( r < freqA+freqT )
			genome_random += "T";
		else if( r < freqA+freqT+freqC )
			genome_random += "C";
		else
			genome_random += "G";
	}	
}

// =============================================================================

void Util::rev_comp( string& sequence )
{
	replace( sequence.begin(), sequence.end(), 'A', 't' );
	replace( sequence.begin(), sequence.end(), 'T', 'a' );
	replace( sequence.begin(), sequence.end(), 'C', 'g' );
	replace( sequence.begin(), sequence.end(), 'G', 'c' );

	transform( sequence.begin(), sequence.end(), sequence.begin(), ::toupper );

	reverse( sequence.begin(), sequence.end() ); 
}

// =============================================================================

void Util::parse_gff( string& filename, vector<PosType>& positions )
{
	fstream file( filename.c_str() ); 
	string line; 

	while( getline( file, line ) )
	{
		if( line[0] != '#' )
		{
			vector<string> tokens;

			split( line, "\t", tokens );

			if( tokens[2] == "miRNA_primary_transcript" )
			{
				PosType pt;
				pt.sequenceId = tokens[0];
				pt.begin = atoi( tokens[3].c_str() );
				pt.end = atoi( tokens[4].c_str() );
				pt.strand = tokens[6];

				positions.push_back( pt );
			}
		}
	}
}

// =============================================================================

void Util::split(const string& str, const string& delimiters , vector<string>& tokens)
{
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		
		lastPos = str.find_first_not_of(delimiters, pos);
		
		pos = str.find_first_of(delimiters, lastPos);
	}
}

// =============================================================================

string Util::clean_sequence( string& sequence )
{
	// --- Sequence in upper case

	transform( sequence.begin(), sequence.end(), sequence.begin(), ::toupper );
	
	// --- Substitute the unknown nucleotides by N

	for( unsigned int i = 0; i < sequence.size(); ++i )
	{
		if( !isATCG( sequence[i] ) )
		{
			sequence[i] = 'N';
		}
	}

	return sequence;
}

// =============================================================================

bool Util::isATCG( char nucleotide )
{
	if( nucleotide == 'A' || nucleotide == 'T' || nucleotide == 'C' || nucleotide == 'G' )
	{
		return true;
	}
	
	return false;
}

// =============================================================================
// 	Encoders 

int Util::encode_dimer( char first, char second )
{
	/*if( first != 'A' || != 'T' )
		return 0;*/

	/*static char bin[CHAR_BIT + 1] = {0};
    	int i;
 
	for ( i = CHAR_BIT - 1; i >= 0; i-- )
	{
		bin[i] = (c % 2) + '0';
		c /= 2;
	}

	return bin;*/

	if( first == 'A' && second == 'A' ){ return 0; }
	else if( first == 'A' && second == 'T' ){ return 1; }
	else if( first == 'A' && second == 'G' ){ return 2; }
	else if( first == 'A' && second == 'C' ){ return 3; }

	else if( first == 'T' && second == 'T' ){ return 4; }
	else if( first == 'T' && second == 'G' ){ return 5; }
	else if( first == 'T' && second == 'C' ){ return 6; }
	else if( first == 'T' && second == 'A' ){ return 7; }

	else if( first == 'C' && second == 'C' ){ return 8; }
	else if( first == 'C' && second == 'G' ){ return 9; }
	else if( first == 'C' && second == 'T' ){ return 10; }
	else if( first == 'C' && second == 'A' ){ return 11; }

	else if( first == 'G' && second == 'G' ){ return 12; }
	else if( first == 'G' && second == 'T' ){ return 13; }
	else if( first == 'G' && second == 'C' ){ return 14; }
	else if( first == 'G' && second == 'A' ){ return 15; }

	return 0;
}

// =============================================================================

int Util::encode_closure(char a, char b)
{
	/*corresponds to the single mismatch from the file int11.txt 
	corresponds to a block of four lines/columns from the file int11.txt
	a -> nuc53 -> seq1 -> seq_up
	b -> nuc35 -> seq2 -> seq_down*/
	
	if( a == 'A' && b == 'T' ) { return 1; }
	else if( a == 'C' && b == 'G' ) { return 2; }
	else if( a == 'G' && b == 'C' ) { return 3; }
	else if( a == 'T' && b == 'A' ) { return 4; }
	else if( a == 'G' && b == 'T' ) { return 5; }
	else if( a == 'T' && b == 'G' ) { return 6; }
	
	return 0;
}

// =============================================================================

int Util::encode_closure( char a, char b, char c, char d )
{
	if( a == 'A' && b == 'T' && c == 'A' && d == 'T' ) { return 1; }
	else if( a == 'A' && b == 'T' && c == 'C' && d == 'G' ) { return 2; }
	else if( a == 'A' && b == 'T' && c == 'G' && d == 'C' ) { return 3; }
	else if( a == 'A' && b == 'T' && c == 'G' && d == 'T' ) { return 4; }
	else if( a == 'A' && b == 'T' && c == 'T' && d == 'A' ) { return 5; }
	else if( a == 'A' && b == 'T' && c == 'T' && d == 'G' ) { return 6; }

	else if( a == 'C' && b == 'G' && c == 'A' && d == 'T' ) { return 7; }
	else if( a == 'C' && b == 'G' && c == 'C' && d == 'G' ) { return 8; }
	else if( a == 'C' && b == 'G' && c == 'G' && d == 'C' ) { return 9; }
	else if( a == 'C' && b == 'G' && c == 'G' && d == 'T' ) { return 10; }
	else if( a == 'C' && b == 'G' && c == 'T' && d == 'A' ) { return 11; }
	else if( a == 'C' && b == 'G' && c == 'T' && d == 'G' ) { return 12; }

	else if( a == 'G' && b == 'C' && c == 'A' && d == 'T' ) { return 13; }
	else if( a == 'G' && b == 'C' && c == 'C' && d == 'G' ) { return 14; }
	else if( a == 'G' && b == 'C' && c == 'G' && d == 'C' ) { return 15; }
	else if( a == 'G' && b == 'C' && c == 'G' && d == 'T' ) { return 16; }
	else if( a == 'G' && b == 'C' && c == 'T' && d == 'A' ) { return 17; } 
	else if( a == 'G' && b == 'C' && c == 'T' && d == 'G' ) { return 18; }

	else if( a == 'T' && b == 'A' && c == 'A' && d == 'T' ) { return 19; }
	else if( a == 'T' && b == 'A' && c == 'C' && d == 'G' ) { return 20; }
	else if( a == 'T' && b == 'A' && c == 'G' && d == 'C' ) { return 21; }
	else if( a == 'T' && b == 'A' && c == 'G' && d == 'T' ) { return 22; }
	else if( a == 'T' && b == 'A' && c == 'T' && d == 'A' ) { return 23; }
	else if( a == 'T' && b == 'A' && c == 'T' && d == 'G' ) { return 24; }

	else if( a == 'G' && b == 'T' && c == 'A' && d == 'T' ) { return 25; }
	else if( a == 'G' && b == 'T' && c == 'C' && d == 'G' ) { return 26; }
	else if( a == 'G' && b == 'T' && c == 'G' && d == 'C' ) { return 27; }
	else if( a == 'G' && b == 'T' && c == 'G' && d == 'T' ) { return 28; }
	else if( a == 'G' && b == 'T' && c == 'T' && d == 'A' ) { return 29; }
	else if( a == 'G' && b == 'T' && c == 'T' && d == 'G' ) { return 30; }

	else if( a == 'T' && b == 'G' && c == 'A' && d == 'T' ) { return 31; }
	else if( a == 'T' && b == 'G' && c == 'C' && d == 'G' ) { return 32; }
	else if( a == 'T' && b == 'G' && c == 'G' && d == 'C' ) { return 33; }
	else if( a == 'T' && b == 'G' && c == 'G' && d == 'T' ) { return 34; }
	else if( a == 'T' && b == 'G' && c == 'T' && d == 'A' ) { return 35; }
	else if( a == 'T' && b == 'G' && c == 'T' && d == 'G' ) { return 36; }

	return 0;
}

// =============================================================================

int Util::encode( char a )
{
	/*corresponds to the single mismatch from the file int11.txt*/
	if(a == 'A') { return 4; }
	else if(a == 'C') { return 3; }
	else if(a == 'G') { return 2; }
	else if(a == 'T') { return 1; }

	return 0; 
}

// =============================================================================

int Util::encode(char a, char b)
{
	if( a == 'A' && b == 'A' ) { return 1; }
	else if( a == 'A' && b == 'C' ) { return 2; }
	else if( a == 'A' && b == 'G' ) { return 3; }
	else if( a == 'A' && b == 'T' ) { return 4; }

	else if( a == 'C' && b == 'A' ) { return 5; }
	else if( a == 'C' && b == 'C' ) { return 6; }
	else if( a == 'C' && b == 'G' ) { return 7; }
	else if( a == 'C' && b == 'T' ) { return 8; }

	else if( a == 'G' && b == 'A' ) { return 9; }
	else if( a == 'G' && b == 'C' ) { return 10; }
	else if( a == 'G' && b == 'G' ) { return 11; }
	else if( a == 'G' && b == 'T' ) { return 12; }

	else if( a == 'T' && b == 'A' ) { return 13; }
	else if( a == 'T' && b == 'C' ) { return 14; }
	else if( a == 'T' && b == 'G' ) { return 15; }
	else if( a == 'T' && b == 'T' ) { return 16; }

	return 0;
}

// =============================================================================

int Util::encode_dangle(char a)
{
	if(a == 'A') { return 1; }
	else if(a == 'C') { return 2; }
	else if(a == 'G') { return 3; }
	else if(a == 'T') { return 4; }

	return 0;
}

// =============================================================================

pair<string*,unsigned int> Util::restricted_seq( string r_seq )
{
	unsigned int taille = 1;
	unsigned int r_count = 0;
	pair<string*,unsigned int> restr_seq;
	restr_seq.second = 1;

	if( r_seq != "" ) 
	{
		ifstream r_premiRNA( r_seq.c_str() , ios::in );

		string line;

		getline( r_premiRNA, line);

		taille = atoi(line.c_str());

		string *premiRNA = new string[ taille ];

		while( getline( r_premiRNA, line ) )
		{
			premiRNA[r_count] = line;
			r_count++;
		}

		restr_seq.first = premiRNA;
		restr_seq.second = taille;
	}

	return( restr_seq );
}

// =============================================================================

bool Util::is_in_restricted_seq( string r_seq, string sequenceID, string* premiRNA, unsigned int taille, string output_file)
{
	if( r_seq != "" )
	{        
		for( unsigned int ab = 0; ab < taille; ab++ ) 
		{
			size_t found = sequenceID.find(premiRNA[ab]);
    
			if (found!=string::npos)
			{
				// Test if the file already exist and is readable.
				ifstream my_file( output_file.c_str() );

				if (my_file.good())
				{
					cout << "File for sequence '" << premiRNA[ab] << "' already exist, so it will not be analysed.\t" << endl;
					return( false );
				}
				else
				{
					return( true );
				}
			}
		}
		// This sequenceID is not in the restricted sequences.
		return( false );
	}	
	// Don't use the option 'restricted sequence'.
	return( true );
}



// =============================================================================
/// 	\fn void Glocal( Sequence *sq, Align& al, Options& opt )
/// 	\brief Look for miRNAs candidates in a sequence.
/// 	\param sq 		pointer to Sequence 
/// 	\param al		object for the alignemnt 
/// 	\param opt		object with the options set by the user

/*void Glocal( Sequence *pSq, Align& al, Options& opt )
{
	for( unsigned int i = 0; i < pSq->seq.size() - (opt.st_arm + opt.stl_max + opt.st_arm); ++i )
	{
		string premirna = pSq->seq.substr( i, opt.st_arm + opt.st_arm + opt.stl_max );

		size_t found = premirna.find( 'N', 0 );

		if( found == string::npos )
		{
			string arm1 = pSq->seq.substr( i, opt.st_arm );
			string arm2 = pSq->seq.substr( i+opt.st_arm+1, opt.st_arm + opt.stl_max ); 
	
			reverse( arm2.begin(), arm2.end() );
   
   			// --- Alignment

			al.forward( arm1, arm2 );
			
			al.compute_maxj();
			unsigned int maxj = al.get_maxj();

			al.backward_glocal( arm1, arm2, arm1.size(), maxj );

			// --- Compute the free energy

			vector<PairType> path = al.get_path();
    
			float energy = eSYM; 			// Symmetry penalty
			
			energy += al.compute_stack( path );	// Stacking energy

			energy += al.compute_dangle( path );	// Dangling nucleotide

			energy += al.compute_internal( path );	// Internal loops (=mismatches)

			if( energy > opt.limit ){ continue; }	
			
			energy += al.compute_bulge( path );	// Bulges (=gaps)
			
			// --- Just mirna candidates under the threshold

			if( energy < opt.limit )
			{
				unsigned int arm2_beg = al.get_minj(); // coordinate in arm scale
				unsigned int arm2_end = al.get_maxj(); // idem

				unsigned int arm2_end_prime = (i+(2*opt.st_arm)+opt.stl_max) - arm2_end; // coordinate in genome scale
				unsigned int arm2_beg_prime = arm2_end_prime - (arm2_end - arm2_beg);    // idem 

				unsigned int loop_size = arm2_beg_prime - i+opt.st_arm + 1; 
				unsigned int loop_beg = i+opt.st_arm + 1; 
				unsigned int loop_end = loop_beg + loop_size;
				
				stringstream sst; 

				sst <<  pSq->id 	 	 << ":" <<\
					i+1 		 << ":" <<\
					arm2_end_prime+1 << ":" <<\
					loop_beg+1 	 << ":" <<\
					loop_end+1	 << ":" <<\
					energy 		 << ":" <<\
					pSq->strand;  
				
				// header
				string header = sst.str();
				
				//sequence
				if( pSq->strand == "reverse" )   
				{
					// to be condescendent with the mirbase gff
					reverse( pSq->seq.begin(), pSq->seq.end() );
				}
				string sequence = pSq->seq.substr( i, arm2_end_prime );

				//pSq->ofile << ">" << header << endl;
				//pSq->ofile << sequence << endl;

				//ofile << ">" << header << endl;
				//ofile << sequence << endl;
			}
		}
		else
		{
			i += found; 
		}
	}
}*/
