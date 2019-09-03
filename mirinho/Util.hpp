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
/// 	@file Util.h

#ifndef UTIL_H_
#define UTIL_H_

#include "Definitions.hpp"

#include<zlib.h>
#include<string.h>
#include<string>
#include<algorithm>
#include<vector>

using namespace std;

// =============================================================================
/// \class Util
/// \brief Implement utility/auxilar methods, such as the ones for encoding 
/// nucleotides into matrix indexes. 

class Util
{
	public:
		
		/// \fn print_ss(...)
		/// \brief Print precursor secondary strcture in dot-bracket and sequence notation
		void print_ss( vector<PairType>& align_path, string& sequence, ofstream& ofile, int stArm_size, int loop_size, string& strand );
		
		/// \fn
		void gen_random( vector<PosType>& pos, string& genome_file, string& genome_random );

		/// \fn
		void parse_gff( string& filename, vector<PosType>& positions );
		
		/// \fn 
		void rev_comp( string& sequence );
		
		/// \fn
		void split(const string& str, const string& delimiters , vector<string>& tokens);
		
		/// \fn Util::isATCG( char nucleotide )
		/// \brief 
		/// \param 
		bool isATCG( char nucleotide );

		/// \fn string Util::clean_sequence( string& sequence ) 
		/// \brief Substitute R Y S W K M B D H V by N.
		///	Substitute masked sequences (lower case letters), 
		///	if they exists, by N.
		/// \param sequence Nucleotide sequence.
		string clean_sequence( string& sequence );

		/// \fn int Util::encode_dimer( char first, char second )
		/// \brief Encode a dimer (two nucleotides) in an integer.
		/// \param first First nucleotide of the dimer.
		/// \param second Second nucleotide of the dimer.
		int encode_dimer( char first, char second );

		/// \fn int Util::encode_closure(char a, char b);
		/// \brief Encode the matching closure of a internal loop 1x1.
		/// \param a First nucleotide of the match.
		/// \param b Second nucleotide of the match.
		int encode_closure(char a, char b);
		
		/// \fn int Util::encode_closure( char a, char b, char c, char d );
		/// \brief Encode the matching closure of a internal loop 2x2.
		/// \param a First nucleotide of the first match.
		/// \param b Second nucleotide of the first match.
		/// \param c First nucleotide of the second match.
		/// \param d Second nucleotide of the second match.
		int encode_closure( char a, char b, char c, char d );

		/// \fn int Util::encode(char a);
		/// \brief Encodes the nucleotide of the single mismatch of internal 1x1.
		/// \param a Nucleotide of the single mismatch.
		int encode(char a);
		
		/// \fn int Util::encode(char a, char b);
		/// \brief Encodes the nucleotides of a mismatch (int22).
		/// \param a First nucleotide of a mismatch.
		/// \param b Second nucleotide of a mismatch.
		int encode(char a, char b);
		
		/// \fn int Util::encode_dangle(char a);
		/// \brief Encodes the dangling nucleotide of a dangle end.
		/// \param a Dangling nucleotide.
		int encode_dangle(char a);
    
		/// \fn string Util::restricted_seq();
		/// \brief Select only the sequences wanted in the genome -> we just read the file and stock sequences ID.
		/// \param r_seq is the restricted file name.
		pair<string*,unsigned int> restricted_seq( string r_seq );
    
		/// \fn bool Util::is_in_restricted_seq( string r_seq, string sequenceID, string* premiRNA, unsigned int taille, string file_name)
		/// \brief Check if the sequenceID is in our restricted files, and if this
		/// file doesn't already exist.
		/// \param r_seq is the restricted file name.
		/// \param sequenceID the sequence ID of the current sequence.
		/// \param premiRNA is the vector with all wanted sequence ID.
		/// \param taille is the length of the premiRNA vector (nb of sequence ID)
		/// \param output_file is the output file name.
		bool is_in_restricted_seq( string r_seq, string sequenceID, string* premiRNA, unsigned int taille, string output_file);
};


#endif
