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
/// 	@file Energy.cpp

#include "Energy.hpp"
#include "Definitions.hpp"
#include "Options.hpp"
#include "Util.hpp"

// =============================================================================
// 	Constructor

Energy::Energy()
{
	// stacking and tstacking energy matrix
	
	unsigned int size = pow(4, W);

	for(unsigned int i = 0; i < size; ++i)
	{
		for(unsigned int j = 0; j < size; ++j)
		{
			stack[i][j] = 0.0;
			tstack[i][j] = 0.0;
		}
	}

	// dangle ends -- dangle.txt

	for(unsigned int i = 0; i < 12+2; ++i)
		dangle[i][0] = 0.0;

	for(unsigned int j = 0; j < 4+2; ++j)
		dangle[0][j] = 0.0;

	// bulge loop initiation

	bulge[0] = 0.0;
	bulge[1] = 3.8;		bulge[2] = 2.8;		bulge[3] = 3.2;
	bulge[4] = 3.6;		bulge[5] = 4.0;		bulge[6] = 4.4;
	bulge[7] = 4.6;		bulge[8] = 4.7;		bulge[9] = 4.8;
	bulge[10] = 4.9;	bulge[11] = 5.0;	bulge[12] = 5.1;
	bulge[13] = 5.2;	bulge[14] = 5.3;	bulge[15] = 5.4;
	bulge[16] = 5.4;	bulge[17] = 5.5;	bulge[18] = 5.5;
	bulge[19] = 5.6;	bulge[20] = 5.7;	bulge[21] = 5.7;
	bulge[22] = 5.8;	bulge[23] = 5.8;	bulge[24] = 5.8;
	bulge[25] = 5.9;	bulge[26] = 5.9;	bulge[27] = 6.0;
	bulge[28] = 6.0;	bulge[29] = 6.0;	bulge[30] = 6.1;

	// internal loop 1x1 -- int11.txt

	for(unsigned int i = 0; i < 24+2; ++i)
	{
		int11[i][0] = 0.0;
        	int11[0][i] = 0.0;
	}

	// internal loop 2x2 -- int22.txt

	for(unsigned int i = 0; i < 576+2; ++i)
		int22[i][0] = 0.0;

	for(unsigned int j = 0; j < 16+2; ++j)
		int22[0][j] = 0.0;

	// internal loop 2x1 -- int21.txt

	for(unsigned int i = 0; i < 96+2; ++i)
		int21[i][0] = 0.0;
	
	for(unsigned int j = 0; j < 24+2; ++j)
		int21[0][j] = 0.0;
	
	// internal loop init > 3

	int3ini[0] = 0.0;	int3ini[1] = 0.0;	int3ini[2] = 0.0;
	int3ini[3] = 1.1;	int3ini[4] = 2.1;	int3ini[5] = 1.9;
	int3ini[6] = 2.0;	int3ini[7] = 2.2;	int3ini[8] = 2.3;
	int3ini[9] = 2.4;	int3ini[10] = 2.5;	int3ini[11] = 2.6;
	int3ini[12] = 2.7;	int3ini[13] = 2.8;	int3ini[14] = 2.8;
	int3ini[15] = 2.9;	int3ini[16] = 3.0;	int3ini[17] = 3.0;
	int3ini[18] = 3.1;	int3ini[19] = 3.2;	int3ini[20] = 3.2;
	int3ini[21] = 3.3;	int3ini[22] = 3.3;	int3ini[23] = 3.4;
	int3ini[24] = 3.4;	int3ini[25] = 3.4;	int3ini[26] = 3.5;
	int3ini[27] = 3.5;	int3ini[28] = 3.6;	int3ini[29] = 3.6;
}

// =============================================================================
// 	Getters 

const float Energy::get_stack( int i, int j ) const
{
	return stack[i][j];
}

// --- 

const float Energy::get_tstack( int i, int j ) const
{
	return tstack[i][j];
}

// --- 

const float Energy::get_dangle( int nuc_dangle, bool dangle_seq1, char nuc53mat, char nuc35mat ) const
{
	Util *u = new Util();

	int i = u->encode_closure(nuc35mat, nuc53mat);

	if( dangle_seq1 )
		i = i+6; // adjusting it according to dangle.txt

	int j = u->encode_dangle(nuc_dangle); 

	delete u;

	return dangle[i][j];
}

// =============================================================================
//	Bulge loop

const float Energy::get_bulge( unsigned int cbulge ) const
{
	if( cbulge > LIMIT_bulge ) 
    	{
        	cout << "Bulge: " << cbulge << " must be in the range of 0 and " << LIMIT_bulge << endl;
        	return EXIT_FAILURE;
    	}
    
	return bulge[ cbulge ];
}

// =============================================================================
//	Internal loop

const float Energy::get_int11(int closure1, int closure2, int nuc53mis, int nuc35mis) const
{
	if( closure1 == 0 || closure2 == 0 )
		return 0.0;

	int i = ((closure1 * 4)+1) - nuc53mis;
	int j = ((closure2 * 4)+1) - nuc35mis;

	return int11[i][j];
}

// --- 

const float Energy::get_int21(int closure1, int closure2, int nuc53mis, int nuc35mis) const
{
	if( closure1 == 0 || closure2 == 0 )
		return 0.0;

	int i = (closure1 * 16) - nuc53mis;
	int j = (closure2 * 4) - nuc35mis;

	return int21[i][j];
}

// --- 

const float Energy::get_int22(int closure, int mis1, int mis2) const
{
	if( closure == 0 )
		return 0.0;

	int i = ((closure * 16) - 16) + mis2;
	int j = mis1;
	
	return int22[i][j];
}

// --- 

const float Energy::get_int3( unsigned int cmis ) const
{
    if( cmis > LIMIT_int3ini ) 
    {
        cout << "Internal loop of size 3: " << cmis << " must be in the range of 0 and " << LIMIT_int3ini << endl;
        return EXIT_FAILURE;
    }
    
    return int3ini[ cmis ];
}

// =============================================================================
// 	Parsers 

void Energy::parser_dangle( string filename )
{
	ifstream file( filename.c_str() );

	int i = 1;
	int j = 1; 

	while( !file.eof() )
	{
		file >> dangle[i][j];
		j++;

		if(j == 4+1)
		{
			j = 1;
			i++;
		}
	}

	file.close();
}

// --- 

void Energy::parser_int11( string filename )
{
        ifstream file( filename.c_str() );
        
        int i = 1;
        int j = 1;

        while(!file.eof())
        {
                file >> int11[i][j];
                j++;

                if( j == 24+1 )
                {
                        j = 1;
                        i++;
                }
        }

        file.close();
}

// --- 

void Energy::parser_int22( string filename )
{
        ifstream file( filename.c_str() );
        
        int i = 1;
        int j = 1;

        while(!file.eof())
        {
                file >> int22[i][j];
                j++;

                if( j == 16+1 )
                {
                        j = 1;
                        i++;
                }
        }

        file.close();
}

// --- 

void Energy::parser_int21( string filename )
{
    	ifstream file( filename.c_str() );
    
	int i = 1;
    	int j = 1;

  	while(!file.eof())
    	{
        	file >> int21[i][j];
        	j++;

        	if( j == 24+1 )
        	{
            		j = 1;
            		i++;
        	}
	}
        
	file.close();
}

// --- 

void Energy::parser_stack(string filename, bool isTerminal)
{
	Util *u = new Util();

	ifstream file( filename.c_str() );

	string line, token;

	int line_num = 0; 	// file line number
	int lin = 0; 		// 'matrix' line number

	char dimerf[W+1]; 
	dimerf[0] = ' '; 
	dimerf[1] = ' '; 
	dimerf[W] = '\0';

	char dimerr[W+1];
	dimerr[0] = ' '; 
	dimerr[1] = ' '; 
	dimerr[W] = '\0';

      	while( getline(file, line) )
      	{
		++line_num;

		stringstream ss(line);

		vector<string> tokens;	
		
		while(ss >> token)
			tokens.push_back(token);

		size_t found;

		if( (found = line.find("AX", 0)) != string::npos)
		{
			dimerf[0] = 'A';
			dimerr[0] = 'T';
			lin = 0;
		}
		else if( (found = line.find("CX", 0)) != string::npos)
		{
			dimerf[0] = 'C';
			dimerr[0] = 'G';
			lin = 0;
		}
		else if( (found = line.find("GX", 0)) != string::npos)
		{
			dimerf[0] = 'G';
			dimerr[0] = 'C';
			lin = 0;
		}
		else if( (found = line.find("UX", 0)) != string::npos)
		{
			dimerf[0] = 'T';
			dimerr[0] = 'A'; 
			lin = 0;
		}
		
		// --- line is energy

		if( tokens.size() == 16 && tokens[0] != "A" )
		{
			++lin;

			for( unsigned int col = 0; col < tokens.size(); ++col )
			{
				if( tokens[col] != ".")
				{
					// --- it is not *GC*, it is *GU*
					if( line_num > 50 && line_num < 56 )
					{
                        			if( col > 11 && col < 16 )
						{
							dimerr[0] = 'T';
						}
						else if ( col > 3 && col < 8 )
						{
							dimerr[0] = 'C';
						}
					}
                    
					// --- it is not *UA*, it is *UG*
                    
					if( line_num > 62 && line_num < 68 )
					{
						if( col > 7 && col < 12 )
						{
							dimerr[0] = 'G';
						}
						else if( col < 4 )
						{
							dimerr[0] = 'A';
						}
					}
                    
					// --- nucletide line 
                    
					switch( lin )
					{
						case 1:
							dimerf[1] = 'A';
							break;
						case 2:
							dimerf[1] = 'C';
							break;
						case 3:
							dimerf[1] = 'G';
							break;
						case 4:
							dimerf[1] = 'T';
							break;
					}
                    
					// --- nucletide column
					
			    		switch( col % 4 )
			   		{
						case 0:// col = 0,4,8,12
				    			dimerr[1] = 'A';
				    			break;
						case 1:// col = 1,5,9,13
						    dimerr[1] = 'C';
						    break;
						case 2:// col = 2,6,10,14
						    dimerr[1] = 'G';
						    break;
						case 3:// col = 3,7,11,15
						    dimerr[1] = 'T';
						    break;
			    		}
                    
					int i = u->encode_dimer( dimerf[0], dimerf[1] );
					int j = u->encode_dimer( dimerr[0], dimerr[1] );
					
					float energy = atof(tokens[col].c_str());
					
					if(isTerminal)
					{
						tstack[i][j] = energy; 
					}
					else
					{
						stack[i][j] = energy; 
					}
				}
			}
		}
	} 
	
	delete u;
        file.close();
}
