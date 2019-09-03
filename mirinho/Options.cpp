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
/// 	@file Options.cpp

#include "Options.hpp"

// =============================================================================

Options::Options( void ):

ifile( "" ),
ofile( "" ),
st_arm( ST_ARM ),
stl_min( STL_MIN ),
stl_max( STL_MAX ),
limit( LIMIT ),
dw( DW ),
path2param( "./parameters/" ), 
al_type( 0 ),
genome_known( "" ),
mirnas_mature( "" ),
ss( 1 )

{}
