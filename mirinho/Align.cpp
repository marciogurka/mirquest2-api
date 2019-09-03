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
/// 	@file Align.cpp

#include "Align.hpp"
#include "Definitions.hpp"
#include "Energy.hpp"
#include "Util.hpp"

#include<math.h>

// =============================================================================
// 	Constructor

Align::Align( string path2param, int al_type, unsigned int arm_len, unsigned int loop_len )
{
	// --- Table to check pair type
	for( unsigned int i = 0; i < X; ++i ) 
	{
		for( unsigned int j = 0; j < X; ++j ) 
			ismatch[i][j] = MI;
	}

	ismatch[A][T] = MA;
	ismatch[T][A] = MA;
	ismatch[C][G] = MA;
	ismatch[G][C] = MA;
	ismatch[G][T] = MA;
	ismatch[T][G] = MA;
	
	// --- Alignment type
	this->al_type = al_type; 
	
	if( al_type == 0 ) 	// global
	{
		this->M = arm_len;
		this->N = arm_len;
	}
	else			// glocal
	{
		this->M = arm_len;
		this->N = arm_len + loop_len; 
	}

	// --- DP matrix
	matrix = new int*[ this->M+1 ];
	for( unsigned int i = 0; i < this->M+1; ++i )
		matrix[i] = new int[ this->N+1 ];

	// --- Movement matrix => just a vector is not enough (?)
	move = new int*[ this->M+1 ];
	for( unsigned int i = 0; i < this->M+1; ++i )
		move[i] = new int[ this->N+1 ];
	
	// --- Loading energy data, taken from NNDB (turner2004) 
	pE = new Energy(); 

	pE->parser_stack( path2param + "stack.txt", false );
	pE->parser_stack( path2param + "tstack.txt", true );
	pE->parser_int11( path2param + "int11.txt");
	pE->parser_int22( path2param + "int22.txt");
	pE->parser_int21( path2param + "int21.txt");
	pE->parser_dangle( path2param + "dangle.txt");
}

// =============================================================================
//	Destructor

Align::~Align()
{
	for( unsigned int i = 0; i < this->M+1; ++i )
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	
	// ---

	for( unsigned int i = 0; i < this->M+1; ++i )
		delete[] move[i];	
	delete [] move;

	// ---

	delete pE;
}

// =============================================================================
// 	Get alignemnt path

const vector<PairType>& Align::get_path()
{
	return path;
}

// =============================================================================
//	Encode nucleotide according to its position in the param file 

int Align::encode( const char base )
{	
	if( base == 'A' ) { return A; }
	else if( base == 'T' ) { return T; }
	else if( base == 'C' ) { return C; }
	else if( base == 'G' ) { return G; }
		
	cout << "Unknown nucleotide: [ " << base << " ]" << endl;
	return EXIT_FAILURE; 
}

// =============================================================================
//	Get penality score of the nucleotide pair type

int Align::getIsmatchScore( int nuc1, int nuc2 )
{
    	if( ismatch[nuc1][nuc2] == MA ){
	    return MAscore; }

	return MIscore;
}

// =============================================================================

void Align::compute_maxj()
{
	// use this-> M / N
	int max = -1000000;
	int maxi = M - 1;
	int maxj = N - 1;

	for( unsigned int j = 1; j < M+N+1; ++j )
	{
		if( matrix[maxi][j] > max ) 
		{
			max = matrix[maxi][j];
			maxj = j - 1; 
		}
	}

	this->maxj = maxj;

//	return maxj; 
}

// =============================================================================

const unsigned int Align::get_minj()
{
	return this->minj; 
}

// =============================================================================

const unsigned int Align::get_maxj()
{
	return this->maxj; 
}

// =============================================================================
//	Forward Global (Needleman–Wunsch algorithm) 

void Align::forward( string &arm1, string &arm2 )
{
	// --- DP base case
	for( unsigned int i = 0; i < arm1.size()+1; ++i )
		matrix[i][0] = 0;
    
	for( unsigned int j = 0; j < arm2.size()+1; ++j )
		matrix[0][j] = 0;
    

	for( unsigned int i = 1; i < arm1.size()+1; ++i )
	{
		int nuc1 = encode( arm1[i-1] );
        
		for( unsigned int j = 1; j < arm2.size()+1; ++j )
		{
			int nuc2 = encode( arm2[j-1] );
            
			matrix[i][j] = matrix[i-1][j-1]+getIsmatchScore( nuc1, nuc2 );
			move[i][j] = ismatch[nuc1][nuc2];

			if( matrix[i][j-1]+GAscore > matrix[i-1][j]+GAscore )
			{
				if( matrix[i][j-1]+GAscore > matrix[i][j] )
				{
				    matrix[i][j] = matrix[i][j-1]+GAscore;
				    move[i][j] = GL;
				}
			}
			else if( matrix[i-1][j]+GAscore > matrix[i][j] )
			{
				matrix[i][j] = matrix[i-1][j]+GAscore;
				move[i][j] = GU;
			}
		}
	}
}

// =============================================================================
// 	Forward Diagonal 

void Align::forward_diagonal( string &arm1, string &arm2, int dw )
{
	int limit_L = 1;
	int limit_U = dw;
    
	// --- DP base case
	for( int i = 0; i < dw; ++i )
	{
		matrix[i][0] = 0;
		matrix[0][i] = 0;
	}

	for( int i = 1; i < (int)arm1.size()+1; ++i )
	{
		int nuc1 = encode(arm1[i-1]);
        
		// --- Update limit Left and limit Up
		if( limit_L != 1 )
		{
			limit_L++;
		}
		else if( (i-dw)+1 > 1)
		{
			limit_L = (i-dw)+1;
		}
        
		// ---
        
		if( i != 1 && limit_U != (int)arm2.size() )
		{
			limit_U++;
		}
        
		// --- Fill the diagonal matrix
		for( int j = limit_L; j < limit_U+1; ++j )
		{
			int nuc2 = encode(arm2[j-1]);
			matrix[i][j] = matrix[i-1][j-1]+getIsmatchScore( nuc1, nuc2 );
			move[i][j] = ismatch[nuc1][nuc2];
		    
			// --- Case where cell on the top diagonal limit : choices are Left or Diag
			if( i == j-(dw-1) )
			{
				if( matrix[i][j-1]+GAscore > matrix[i][j] )
				{
					matrix[i][j] = matrix[i][j-1]+GAscore;
					move[i][j] = GL;
				} 
			}
			// --- Case where cell on the left diagonal limit : choices are Up or Diag
			else if(j == i-(dw-1) )
			{
				if( matrix[i-1][j]+GAscore > matrix[i][j] )
				{
			    		matrix[i][j] = matrix[i-1][j]+GAscore;
			   		move[i][j] = GU;
				} 
			}
			// --- Others.
			else
			{
				if( matrix[i][j-1]+GAscore > matrix[i-1][j]+GAscore )
				{
			    		if( matrix[i][j-1]+GAscore > matrix[i][j] )
			    		{
						matrix[i][j] = matrix[i][j-1]+GAscore;
						move[i][j] = GL;
			  	  	}
				}
				else if( matrix[i-1][j]+GAscore > matrix[i][j] )
				{
			    		matrix[i][j] = matrix[i-1][j]+GAscore;
			    		move[i][j] = GU;
				}
			}
		}
	}
}

// =============================================================================
// 	Backtracking to get the alignment path

void Align::backward( string &arm1, string &arm2, int i, int j )
{
	PairType p; 

	if( i == 0 && j == 0 )
	{
		path.clear();
		return;
	}
	else if( j == 0 )
	{
		backward( arm1, arm2, i-1, j );
		p.type = GU;
		p.nuc1 = arm1[i-1]; 
		p.nuc2 = '-';
	}
	else if( i == 0 )
	{
		backward( arm1, arm2, i, j-1 );
		p.type = GL;
		p.nuc1 = '-';
		p.nuc2 = arm2[j-1]; 
	}
	else // j > 0 && i > 0
	{
		switch ( move[i][j] )
		{
		    case MA:    // match -> helix
			backward( arm1, arm2, i-1, j-1 );
			p.type = MA;
			p.nuc1 = arm1[i-1]; 
			p.nuc2 = arm2[j-1]; 
			break;
			
		    case MI:	// mis -> internal loop
			backward( arm1, arm2, i-1, j-1 );
			p.type = MI;
			p.nuc1 = arm1[i-1]; 
			p.nuc2 = arm2[j-1]; 
			break;
			
		    case GU:    // gap_up -> bulge_up
			backward( arm1, arm2, i-1, j );
			p.type = GU;
			p.nuc1 = arm1[i-1];
			p.nuc2 = '-'; 
			break;
			
		    case GL:    // gap_left -> bulge_left
			backward( arm1, arm2, i, j-1 );
			p.type = GL;
			p.nuc1 = '-'; 
			p.nuc2 = arm2[j-1]; 
			break;
		}
	}
	
	path.push_back(p);
}

// =============================================================================
// 	Backtracking to get the alignment path (Diagonal)

bool Align::backward_diagonal( string &arm1, string &arm2, int i, int j, int dw, int overflow, bool verdict )
{
	// If path hit the diagonal, we don't need to go deeply so we stop the recursion here.
	if( fabs(overflow) >= dw-1 || not verdict )
	{
		path.clear();
		return(false);
	}

	// Else, run normaly the backward alignment.
	PairType p; 
    
	if( i == 0 && j == 0 )
	{
		path.clear();
		return(true);
	}
	else if( j == 0 )
	{
		overflow--;
		verdict = backward_diagonal( arm1, arm2, i-1, j, dw, overflow, verdict );
		p.type = GU;
		p.nuc1 = arm1[i-1]; 
		p.nuc2 = '-';
	}
	else if( i == 0 )
	{
		overflow++;
		verdict = backward_diagonal( arm1, arm2, i, j-1, dw, overflow, verdict );
		p.type = GL;
		p.nuc1 = '-';
		p.nuc2 = arm2[j-1];
	}
	else // j > 0 && i > 0
	{
		switch ( move[i][j] )
		{
		    case MA:    // match -> helix
			verdict = backward_diagonal( arm1, arm2, i-1, j-1, dw, overflow, verdict );
			p.type = MA;
			p.nuc1 = arm1[i-1]; 
			p.nuc2 = arm2[j-1]; 
			break;
			
		    case MI:	// mis -> internal loop
			verdict = backward_diagonal( arm1, arm2, i-1, j-1, dw, overflow, verdict );
			p.type = MI;
			p.nuc1 = arm1[i-1]; 
			p.nuc2 = arm2[j-1]; 
			break;
			
		    case GU:    // gap_up -> bulge_up
			overflow--;
			verdict = backward_diagonal( arm1, arm2, i-1, j, dw, overflow, verdict );
			p.type = GU;
			p.nuc1 = arm1[i-1];
			p.nuc2 = '-';
			break;
			
		    case GL:    // gap_left -> bulge_left
			overflow++;
			verdict = backward_diagonal( arm1, arm2, i, j-1, dw, overflow, verdict );
			p.type = GL;
			p.nuc1 = '-';
			p.nuc2 = arm2[j-1]; 
			break;
		}
	}
	
	path.push_back(p);
    
	return(verdict);
}

// =============================================================================
// 	Backtracking to get the alignment path (Glocal)

void Align::backward_glocal( string &arm1, string &arm2, int i, int j )
{
	PairType p; 
	
	if( i == 0 || matrix[i][j] == 0 )
	{
		this->minj = j;

		path.clear();
		return;
	}
	else if( j == 0 )
	{
		backward( arm1, arm2, i-1, j );
		p.type = GU;
		p.nuc1 = arm1[i-1]; 
		p.nuc2 = '-';
	}
	else // j > 0 && i > 0
	{
		switch ( move[i][j] )
		{
		    case MA:    // match -> helix
			backward_glocal( arm1, arm2, i-1, j-1 );
			p.type = MA;
			p.nuc1 = arm1[i-1]; 
			p.nuc2 = arm2[j-1]; 
			break;
			
		    case MI:	// mis -> internal loop
			backward_glocal( arm1, arm2, i-1, j-1 );
			p.type = MI;
			p.nuc1 = arm1[i-1]; 
			p.nuc2 = arm2[j-1]; 
			break;
			
		    case GU:    // gap_up -> bulge_up
			backward_glocal( arm1, arm2, i-1, j );
			p.type = GU;
			p.nuc1 = arm1[i-1];
			p.nuc2 = '-';
			break;
			
		    case GL:    // gap_left -> bulge_left
			backward_glocal( arm1, arm2, i, j-1 );
			p.type = GL;
			p.nuc1 = '-';
			p.nuc2 = arm2[j-1]; 
			break;
		}
	}
	

	path.push_back(p);
}

// =============================================================================
//	Stacking

float Align::compute_stack(vector<PairType>& path)
{
	Util *u = new Util();

	float acc_stack = 0.0;

	// stacking
	for( unsigned int i = 0; i < path.size()-1; ++i )	
	{
		if( path[i].type == MA )
		{
			int a = u->encode_dimer( path[i].nuc1,  path[i+1].nuc1 ); // 5- AT -3
			int b = u->encode_dimer( path[i].nuc2,  path[i+1].nuc2 ); // 3- TA -5

			if( path[i+1].type == MA )
			{
				acc_stack += pE->get_stack(a, b);
			}
			else 
			{
				acc_stack += pE->get_tstack(a, b);
			}
		}

	}

	delete u;

	return acc_stack;
}

// =============================================================================
// 	Dangling ends

float Align::compute_dangle( vector<PairType>& path )
{	
	float acc_dangle = 0.0;
	char nuc1mat, nuc2mat, nuc_dangle;
	bool dangle_seq1 = false;

	if( path[1].type == MA )
	{
		if( path[0].nuc1 == '-' && path[0].nuc2 != '-' )
		{
			nuc_dangle = path[0].nuc2;
			dangle_seq1 = false;		

			nuc1mat = path[1].nuc1; 
			nuc2mat = path[1].nuc2;

			acc_dangle = pE->get_dangle( nuc_dangle, dangle_seq1, nuc1mat, nuc2mat );
		}
		else if( path[0].nuc1 != '-' && path[0].nuc2 == '-' )
		{
			nuc_dangle = path[0].nuc1;
			dangle_seq1 = true;
			
			nuc1mat = path[1].nuc1; 
			nuc2mat = path[1].nuc2;

			acc_dangle = pE->get_dangle( nuc_dangle, dangle_seq1, nuc1mat, nuc2mat );
		}
	}
	
	return acc_dangle;
}

// =============================================================================
//	Bulges

float Align::compute_bulge( vector<PairType>& path )
{
	// --- Bulge loop down (Gap Right) 
	vector<PairType>::iterator it = path.begin()+1;	
	unsigned int cgu; // counter
	float acc_bulge_up = 0.0;
	Util *u = new Util();

	while( it != path.end()-1 )
	{	
		cgu = 0;

		while( (*it).type == GU && it != path.end()-1 )
		{
			// first AU end
			if( cgu == 0 )
			{
				if( ( (*(it-1)).nuc1 == 'A' && (*(it-1)).nuc2 == 'T' ) || ( (*(it-1)).nuc1 == 'T' && (*(it-1)).nuc2 == 'A' ) )
					acc_bulge_up += eAU;
			}

			++cgu;
			++it;
		}
		
		if( cgu == 0 )
		{
			++it;
		}
		else if( cgu == 1 )
		{
			char nt = (*(it-1)).nuc1;
			vector<PairType>::iterator aux = it;
			int states = 1;

			while( (*(it++)).nuc1 == nt && it < path.end() )
			{
				++states;
			}
			it = aux;
			while( (*(it--)).nuc1 == nt && it > path.begin() )
			{
				++states;
			}
			it = aux;

			int a = u->encode_dimer( (*(it-2)).nuc1, (*it).nuc1 ); // 5- A^{C}^T -3
			int b = u->encode_dimer( (*(it-2)).nuc2, (*it).nuc2 ); // 3- T_{_}_A -5

			float stack = pE->get_stack(a, b);

			acc_bulge_up += pE->get_bulge(cgu) + stack - ( RT * log(states) ); 

			if( (*(it-1)).nuc1 == 'C' ) 
			{
				acc_bulge_up += eCBU; 
			}
		}
		else if( cgu < LIMIT_bulge ) 
		{
			if( (*(it-(cgu+1))).type == MA && (*it).type == MA )
			{
				acc_bulge_up += pE->get_bulge(cgu);
				
				// second AU end
				if( ( (*it).nuc1 == 'A' && (*it).nuc2 == 'T' ) || ( (*it).nuc1 == 'T' && (*it).nuc2 == 'A' ) )
				{
					acc_bulge_up += eAU;
				}
			}
		}
	}	

	// --- Bulge loop left (Gap Left)
	it = path.begin()+1;	
	unsigned int cgl; 		
	float acc_bulge_le = 0.0;

	while( it != path.end()-1 )
	{	
		cgl = 0;

		while( (*it).type == GL && it != path.end()-1 )
		{
			// first AU end
			if( cgl == 0 )
			{
				if( ( (*(it-1)).nuc1 == 'A' && (*(it-1)).nuc2 == 'T' ) || ( (*(it-1)).nuc1 == 'T' && (*(it-1)).nuc2 == 'A' ) )
					acc_bulge_le += eAU;
			}

			++cgl;
			++it;
		}

		if( cgl == 0 )
		{
			++it;
		}
		else if( cgl == 1 )
		{
			char nt = (*(it-1)).nuc1;
			vector<PairType>::iterator aux = it;
			int states = 1;

			while( (*(it++)).nuc1 == nt && it < path.end() )
			{
				++states;
			}
			it = aux;
			while( (*(it--)).nuc1 == nt && it > path.begin() )
			{
				++states;
			}
			it = aux;

			int a = u->encode_dimer( (*(it-2)).nuc1, (*it).nuc1 ); // 5- A^{C}^T -3
			int b = u->encode_dimer( (*(it-2)).nuc2, (*it).nuc2 ); // 3- T_{_}_A -5

			float stack = pE->get_stack(a, b);
			
			acc_bulge_up += pE->get_bulge(cgl) + stack - ( RT * log(states) ); 

			if( (*(it-1)).nuc1 == 'C' ) 
			{
				acc_bulge_up += eCBU; 
			}
		}
		else if( cgl < LIMIT_bulge )
		{
			if( (*(it-(cgl+1))).type == MA && (*it).type == MA )
			{
				acc_bulge_le += pE->get_bulge(cgl);

				// second AU end
				if( ( (*it).nuc1 == 'A' && (*it).nuc2 == 'T' ) || ( (*it).nuc1 == 'T' && (*it).nuc2 == 'A' ) )
				{
					acc_bulge_le += eAU;
				}
			}
		}
	}

	delete u;

	return (acc_bulge_up + acc_bulge_le); 
}

// =============================================================================
//	Internal loop

float Align::compute_internal( vector<PairType>& path )
{
	Util *u = new Util();
	unsigned int i = 1;
	float acc_int3 = 0.0; 
	float acc_int11 = 0.0;
	float acc_int22 = 0.0;
	float acc_int21 = 0.0;
		
	char a=' ', b=' ', c=' ', d=' '; 

	while( i != path.size()-1 ) 	
	{
		unsigned int cmis = 0;
		int closure1 = 0; 	// int11
		int closure2 = 0; 	// int11
		int closure = 0; 	// int22
		vector<char> mis;

		int n1 = 0; 	// for assymentry (n1 - n2)
		int n2 = 0; 

		// counting the mismatches
		while( (path[i].type == MI) && (i < path.size()-1) )
		{
			if( path[i].nuc1 == '-' )
				++n1;

			if( path[i].nuc2 == '-' )
				++n2;

			cmis++;
			i++;
		}

		// motif processing according to the number of mismatches
		if( cmis == 0 )
		{
			i++;
		}
		else if( cmis == 1 )
		{
			// first closure (=match)
			closure1 = u->encode_closure( path[i-cmis-1].nuc1, path[i-cmis-1].nuc2 ); 

			// single mismatch int11
			int nuc1mis = u->encode( path[i-1].nuc1 ); 
			int nuc2mis = u->encode( path[i-1].nuc2 );

			// int21
			if( path[i].type == GU && i < path.size()-1 )
			{
				closure2 = u->encode_closure( path[i+1].nuc1, path[i+1].nuc2 );

				acc_int21 += pE->get_int21( closure1, closure2, nuc1mis, nuc2mis );
			}
			// int11
			else	
			{
				closure2 = u->encode_closure( path[i].nuc1, path[i].nuc2 );

				acc_int11 += pE->get_int11( closure1, closure2, nuc1mis, nuc2mis ); 
			}
		}
		else if( cmis == 2 ) 
		{
			// first closure 
			a = path[i-cmis-1].nuc1;
			b = path[i-cmis-1].nuc2;

			// second closure
			c = path[i].nuc1;
			d = path[i].nuc2;
			
			// double mismatch int22
			int mis1 = u->encode( path[i-1].nuc1, path[i-1].nuc2 );
			int mis2 = u->encode( path[i-2].nuc1, path[i-2].nuc2 );

			closure = u->encode_closure( a, b, c, d );

			acc_int22 += pE->get_int22( closure, mis1, mis2 );
		}
		else if( cmis == 3 )
		{
			int a = u->encode_dimer( path[i-1].nuc1, path[i-1].nuc2 );
			int b = u->encode_dimer( path[i-cmis].nuc1, path[i-cmis].nuc2 );

			acc_int3 = pE->get_tstack( a, b );

			acc_int3 += pE->get_int3( cmis );
		}
		else 
		{
			//ΔG° internal = 
			//ΔG° initiation(n) + 
			//ΔG° asymmetry×| n1 – n2| + 
			//ΔG° mismatch(mismatch 1) + 
			//ΔG° mismatch(mismatch 2) + 
			//ΔG° AU/GU closure(per AU or GU closure) *** missing GU closure ***

			int a = u->encode_dimer( path[i-cmis-1].nuc1, path[i-cmis].nuc1 );
			int b = u->encode_dimer( path[i-cmis-1].nuc2, path[i-cmis].nuc2 );

			float mis1 = pE->get_stack(a, b);

			int c = u->encode_dimer( path[i-1].nuc1, path[i].nuc1 );
			int d = u->encode_dimer( path[i-1].nuc2, path[i].nuc2 );
		
			float mis2 = pE->get_stack(c, d);
			
			acc_int3 = pE->get_int3( cmis ) + ( eASYM * abs( n1 - n2 ) ) + mis1 + mis2;
		}			

		if( cmis > 0 )
		{
			// first AU end
			if( (path[i-cmis-1].nuc1 == 'A' && path[i-cmis-1].nuc2 == 'T') || (path[i-cmis-1].nuc1 == 'T' && path[i-cmis-1].nuc2 == 'A') )
			{
				acc_int11 += eAU;
			}

			// check second AU end
			if( (path[i].nuc1 == 'A' && path[i].nuc2 == 'T') || (path[i].nuc1 == 'T' && path[i].nuc2 == 'A') )
			{
				acc_int11 += eAU;
			}
		}
	}

	delete u;

	return (acc_int11 + acc_int22 + acc_int21 + acc_int3); 
}
