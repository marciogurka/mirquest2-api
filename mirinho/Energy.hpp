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
/// 	@file Energy.h

#ifndef ENERGY_H_
#define ENERGY_H_

#include<string.h>
#include<fstream>
#include<vector>
#include<sstream>
#include<math.h>

#include "Util.hpp"

using namespace std;

// =============================================================================
/// 	\class Energy
/// 	\brief Set the Gibbs free energies parameters for nucleotide dimers, 
/// 	according to the Nearest Neighbor method for themodynamics.

class Energy
{
	public:

		/// Constructor 
		/// Allocates memory for all free energy matrices \c stack, \c tstack, \c dangle
		/// \c bulge, \c int11, \c int22 and \c int21.

		Energy();
		
		// --- Getters

		/// \fn const float Energy::get_stack( int i, int j )
		/// \brief Get the stacking free energy referring to two dimer. 
		/// \param i Code of the dimer1.
		/// \param j Code of the dimer2.
		/// \return The free energy for stacking two consecutive base pairs.

		const float get_stack( int i, int j ) const;

		/// \fn const float Energy::get_tstack( int i, int j )
		/// \brief Get the terminal stacking free energy referring to two dimer. 
		/// \param i Code of the dimer1 5->3.
		/// \param j Code of the dimer2 3->5.
		/// \return The free energy for stacking a terminal two consecutive base pairs.

		const float get_tstack( int i, int j ) const;
		
		/// \fn const float Energy::get_dangle( int nuc_dangle, bool dangle_seq1, char nuc1mat, char nuc2mat )
		/// \brief Get the dagle ends free energy according to the dangling nucleotide 
		/// and consecutive match.
		/// \param nuc_dangle Dangling nucleotide.
		/// \param dangle_seq1 Indicate if the dangle is in 5->3 (sequence 1) or 3->5 (sequence 2). 
		/// \param nuc1mat First nucleotide of the consecutive match.
		/// \param nuc2mat Second nucleotide of the consecutive match.
		/// \return The free ernergy of a dangle end.  

		const float get_dangle( int nuc_dangle, bool dangle_seq1, char nuc1mat, char nuc2mat ) const;

		/// \fn const float Energy::get_bulge( int i );
		/// \brief Get the bulge free energy according to the size of the bulge.
		/// \param i Size of the bulge, that is the number of nucletides in the bulge. 
		/// \return The free ernergy of a bulge (ie, consecutive gap(s)).

		const float get_bulge( unsigned int cbulge ) const;

		/// \fn const float Energy::get_int11(int closure1, int closure2, int nuc1mis, int nuc2mis);
		/// \brief Get the internal loop (with one mismatch) free energy according to the 
		/// flanking matches and its mismatch.
		/// \param closure1 Left side match.
		/// \param closure2 Right side match.
		/// \param nuc1mis First nucleotide of the mismatch.
		/// \param nuc2mis Second nucleotide of the mismatch.
		/// \return The free ernergy of an internal loop of size 1x1 (ie, one mismatch).

		const float get_int11(int closure1, int closure2, int nuc1mis, int nuc2mis) const;

		/// \fn const float Energy::get_int22(int closure, int mis1, int mis2);
		/// \brief Get the internall loop (with two mismatches) free energy according to 
		/// the flaking matches and its two mismatches.
		/// \param closure1 Left side match.
		/// \param closure2 Right side match.
		/// \param nuc1mis Code of the first dimer mismatch.	
		/// \param nuc2mis Code of the second dimer mismatch.	
		/// \return The free ernergy of an internal loop of size 2x2 (ie, two consecutive mismatches).

		const float get_int22(int closure, int mis1, int mis2) const;

		/// \fn const float Energy::get_int21(int closure1, int closure2, int nuc1mis, int nuc2mis);
		/// \brief Get the internall loop (with one mismatch and one gap) free energy 
		/// according to the flanking matches and its mismatche and gap.
		/// \param closure1 Left side match.
		/// \param closure2 Right side match.
		/// \param nuc1mis Code of the first dimer mismatch.	
		/// \param nuc2mis Code of the second dimer mismatch.	
		/// \return The free ernergy of an internal loop of size 2x1 (ie, one mismatch followed by one gap).

		const float get_int21(int closure1, int closure2, int nuc1mis, int nuc2mis) const;
		
		/// \fn const float get_int3(int cmis);

		const float get_int3( unsigned int cmis ) const;

		// --- Parsers

		/// \fn void Energy::parser_stack(string filename, bool isTerminal ) 
		/// \brief Parser for loading (terminal) stacking free energy.

		void parser_stack( string filename, bool isTerminal );

		/// \fn void Energy::parser_dangle( string filename );
		/// \brief Parser for loading dangle ends free energy.

		void parser_dangle( string filename );

		/// \fn void Energy::parser_int11( string filename )
		/// \brief Parser for loading internal loop 1x1 free energy
		/// \param filename Internal loop 1x1 energies file name.

		void parser_int11( string filename );
		
		/// \fn void Energy::parser_int22( string filename )
		/// \brief Parser for loading internal loop 2x2 free energy.
		/// \param filename Internal loop 2x2 energies file name.

		void parser_int22( string filename );
		
		/// \fn void Energy::parser_int21( string filename )
		/// \brief Parser for loading internal loop 2x1 free energy.
		/// \param filename Internal loop 2x1 energies file name.

		void parser_int21( string filename );

	private:
		
		/// \var stack Stores stacking free energy.

		float stack[16][16]; //4^W
		
		/// \var tstack Stores terminal stacking free energy.

		float tstack[16][16]; 

		/// \var dangle Stores gangle ends free energy.

		float dangle[14][6];

		/// \var bulge Stores bulge initiation free energy.

		float bulge[31];

		/// \var Stores internal loop 1x1 free energy.

		float int11[26][26];

		/// \var Stores internal loop 2x2 free energy.

		float int22[578][18];

		/// \var Stores internal loop 2x1 free energy.

		float int21[98][26];

		/// \var Stores internal loop initialization free energy (loops > 3).

		float int3ini[30];
};

#endif
