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
/// 	@file Align.h

#ifndef ALIGN_H_
#define ALIGN_H_

#include "Align.hpp"
#include "Definitions.hpp"
#include "Energy.hpp"

#include<string.h>

#include<iostream>
#include<vector>

using namespace std;

// =============================================================================
/// 	\class Align
/// 	\brief Tackle with the alignment of the stem-arms of 
/// 	a pre-miRNA stemloop sequence. Uses the Needleman–Wunsch
/// 	algorithm.

class Align
{
	public:

		/// Constructor 
		/// Initializes the matrix \c ismatch, the dynamic programming (DP) \c matrix and its base case, 
		/// the \c move(ment) matrix and load the energy parameters. 
		Align( string path2param, int al_type, unsigned int arm_len, unsigned int loop_len );
		
		/// Destructor
		~Align();

		/// \fn int Align::encode( const char &base )
		/// \brief Encodes a letter (nucleotide, base) in an integer. 
		/// \param base A letter to be encoded. 
		/// \return Encoded base.
		int encode( const char base );
   
		void compute_maxj();

		const unsigned int get_maxj();

		const unsigned int get_minj();	

		/// \fn void Align::forward_diagonal( string &arm1, string &arm2, int dw )
		/// \brief Fills the DP matrix for the alignment of two nucleotide sequences with the diagonal method.
		/// \param stemarm1 First nucleotide sequence--stem-arm of the stemloop.
		/// \param stemarm2 Second nucleotide sequence--stem-arm of the stemloop.
		/// \param dw Diagonal width.
		void forward_diagonal( string &arm1, string &arm2, int dw);

		/// \fn void Align::forward( string &arm1, string &arm2 )
		/// \brief Fills the DP matrix for the alignment of two nucleotide sequences.
		/// \param stemarm1 First nucleotide sequence--stem-arm of the stemloop.
		/// \param stemarm2 Second nucleotide sequence--stem-arm of the stemloop.
		void forward( string &arm1, string &arm2 );
		
		/// \fn void Align::backward( int i, int j, string &arm1, string &arm2 )
		/// \brief Backtracks into the matrix to recover the path of the alignment.
		/// \param i Line of the DP matrix.
		/// \param j Column of the DP matrix.
		/// \param arm1 First nucleotide sequence.
		/// \param arm2 Second nucleotide sequence.
		/// \param dw Diagonal width.
		/// \param overflow Counter of gap for see if there is an overflow.
		/// \param verdict Boolean indicate if alignment is ok or overflow.
		void backward( string &arm1, string &arm2, int i, int j );
		
		bool backward_diagonal( string &arm1, string &arm2, int i, int j, int dw, int overflow, bool verdict );
		
		void backward_glocal( string &arm1, string &arm2, int i, int j );

		/// \fn float Align::compute_stack( vector<PairType>& path )
		/// \brief Computes the Gibbs free energy (kcal/mol) of the alignment using 
 		/// the Nearest Neighbor (NN) method (Turner 2004) for a stacking motif.
		/// \param path Path returned by the backtracking of the DP referring to the alignment of the sequences.
		/// \return The free energy of the stacking. 
		float compute_stack( vector<PairType>& path );
		
		/// \fn float Align::compute_dangle( vector<PairType>& path )
		/// \brief Computes the free energy of the alignment using 
 		/// the NN method (Turner 2004) for a dangling ends.
		/// \param path Path returned by the backtracking of the DP referring to the alignment of the sequences.
		/// \return The free energy of the dangling end. 
		float compute_dangle( vector<PairType>& path );
		
		/// \fn float Align::compute_bulge( vector<PairType>& path )
		/// \brief Computes the free energy of the alignment using 
 		/// the NN method (Turner 2004) for a dangling ends.
		/// \param path Path returned by the backtracking of the DP referring to the alignment of the sequences.
		/// \return The free energy of the bulge motif. 
		float compute_bulge( vector<PairType>& path );
		
		/// \fn float Align::compute_internal( vector<PairType>& path )
		/// \brief Computes the free energy of the alignment using 
 		/// the NN method (Turner 2004) for a dangling ends.
		/// \param path Path returned by the backtracking of the DP referring to the alignment of the sequences.
		/// \return The free energy of the internal loop. 
		float compute_internal( vector<PairType>& path );

		/// \fn const Align::vector<PairType>& get_path( void )
		/// \return The path in the DP matrix corresponding to the sequence alignment.
		const vector<PairType>& get_path( void );

		/// \fn int Align::getIsmatchScore( int a, int b )
		/// \return The penality score of the nucleotide pair type.
		int getIsmatchScore( int nuc1, int nuc2 );

	private:

		// \var maxj Minimum value of j in the DP matrix in which the glocal alignemnt ends.	
		unsigned int minj;

		// \var maxj Maximum value of j in the DP matrix in which the glocal alignemnt begins.	
		unsigned int maxj;

		/// \var DP Matrix for the sequence alignment.
		int **matrix; 

		/// \var move Matrix for the movements--diagonal, up, left--performed in the forward step.
		int **move; 

		/// \var ismatch Matrix for the canonical base pairs.
		int ismatch[X][X];

		/// \var path Path in the DP matrix referring to the alignment.
		vector<PairType> path;

		/// \var e Pointer to the methods to compute the free energy.
		Energy *pE;
		
		/// \var al_type Alignment type.
		int al_type;

		/// \var M number of lines of the DP matrix.
		int M;

		/// \var N number of columns of the DP matrix.
		int N;

};

#endif
