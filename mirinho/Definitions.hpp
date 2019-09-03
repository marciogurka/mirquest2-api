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
/// 	@file Definitons.h

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include<iostream>

using namespace std;

// =============================================================================
// 	Pre-miRNA stem-loop features

static const unsigned int ST_ARM = 25;	// stem arm length
static const unsigned int STL_MIN = 5;	// stem loop minimum length
static const unsigned int STL_MAX = 20;	// stem loop maximun length
static const float LIMIT = -20.6; //0.0;		// free energy threshold	
static const unsigned int W = 2;	// word size 

static const unsigned int LIMIT_int3ini = 30;
static const unsigned int LIMIT_bulge = 30;

// =============================================================================
// 	Dynamic programming parameters

static const int MA = 4;
static const int MI = 5;
static const int GU = 6; 		// gap in seq2
static const int GL = 7; 		// gap in seq1

static const int MAscore = 7;
static const int MIscore = 0;
static const int GAscore = -10;

static const int A = 0;
static const int C = 1;
static const int G = 2;
static const int T = 3; 	

static const unsigned int X = 4; 	// alphabet size

// =============================================================================
//	Metainfo for the positions of the pre-miRNA in the genome (auto set threshold)

struct PosType
{
	string sequenceId;
	int begin; 
	int end; 
	string strand;
};

// =============================================================================
// 	Features of the alignment path 

struct PairType
{
	char nuc1;			// nucleotide from sequence 5->3
	char nuc2;			// nucleotide from sequence 3->5
	int type; 			// MA,MI,GU,GL
};

// =============================================================================
// 	Optimisation parameters

static const unsigned int DW = 5;            	// diagonal width at first line ( max diagonal during alignment = 2*DW-1 )
static const unsigned int tbl = 4;	// t-blocks length                                  //MODIF

// =============================================================================
// 	Features of a t-block

struct tBlock
{
	int Ab;            		// Intersection of first row and column of the block
	int Rb[tbl-1];     		// First row of the block (minus A)
	int Cb[tbl-1]; 	   		// First column of the block (minus A)
	string Db;         		// Substrings of the first nucleotide sequence (vertical)
	string Eb;         		// Substrings of the second nucleotide sequence (horizontal)
	int Ob[2*tbl-1];   		// Last row and column (row numbers followed by column numbers from bottom to top)
};

// =============================================================================
// 	NN energy model constant energies

static const float eINI = 4.09; 	// intermolecular initiation
static const float eSYM = 0.43; 	// symmetry correction (is applied to self-complementary duplexes)
static const float eAU = 0.45;		// per AU end 
static const float eCBU = -0.9;		// special C bulge
static const float eASYM = 0.6; 	//asymetry internal loop

static const float RT = 0.616; 		// gas constant x tempeture (310K)

#endif
