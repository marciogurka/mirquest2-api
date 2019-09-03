
To compile it simply type make (developed and tested on Mac OS X 10.6.8). 

To run it check the usage before by typing ./mirinho 

Usage: ./mirinho [OPTIONS] -i sequence.fasta[.gz]
	-a PARAM	Length of the stem-arm - default: 25nt
	-n PARAM	Minimum length of the terminal loop - default: 5nt
	-x PARAM	Maximum length of the terminal loop - default: 20nt
	-l PARAM	Free energy threshold - default: -20.6 kcal/mol 
	-o PARAM	Output file name - default: input_out.fa
	-p PARAM	Path to parameters folder - default: ./
	-w PARAM	Width of the diagonal for alignment - default: 5
	-s PARAM	Print or not the secondary structure: 1 for printing, 0 otherwise - default: 1

Contact: Susan Higashi ( susanhigashi@gmail.com )

Website: mirinho.gforge.inria.fr

-----------
 Input Description 
-----------
 
Input sequence in fasta format. 

If the sequence is masked, one should use 'N' to mask the appropriate regions. 
Mirinho will not recognise 'x' nor lower case representation as a masked region.
Then, if the user uses RepeatMasker, options -xsmall and -small should not be used.

-----------
 Input Example:
-----------

>hsa-let-7d
CCTAGGAAGAGGTAGTAGGTTGCATAGTTTTAGGGCAGGGATTTTGCCCACAAGGAGGTAACTATACGACCTGCTGCCTTTCTTAGG

-----------
 Output Description: 
-----------

Output with the putative miRNA sequences. The header of each entry in the output is 
presented like this: 
 
>Sequence_Id:Start_preMirna:End_preMirna:Start_Loop:End_Loop:Energy:Strand

- Sequence_Id: name of the sequence which is the same as the header of the input 
file. If there are spaces in the name, everything after the first space will be 
ignored.

- Start_preMirna: position in the sequence "Sequence_Id" of the first nucleotide of the pre-miRNA.

- End_preMirna: position in the sequence "Sequence_Id" of the last nucleotide of the pre-miRNA.

- Start_Loop: position in the sequence "Sequence_Id" of the first nucleotide of the terminal loop. 

- End_Loop: position in the sequence "Sequence_Id" of the last nucleotide of the terminal loop. 
 
- Energy: free energy of the alignment of both stem-arms. 

- Strand: pre-miRNA originating from the forward or the reverse (ie, reverse complement of the input sequence) strand. 

-----------
 Output Example: 
-----------

>hsa-let-7d:11:78:36:53:-27.77:forward
GGTAGTAGGTTGCATAGTTTTAGGGCAGGGATTTTGCCCACAAGGAGGTAACTATACGACCTGCTGCC
((((((((((((.((((((.(..........................).)))))).))))))))))))
            C      T AGGGCAGGGATT
GGTAGTAGGTTG ATAGTT T            T
|||||||||||| |||||| |             
CCGTCGTCCAGC TATCAA G            T
            A      T GAGGAACACCCG

-----------
 Cite
-----------

Higashi, S., Fournier, C., Gautier, C., Gaspin, C., & Sagot, M. F. (2015). 
Mirinho: An efficient and general plant and animal pre-miRNA predictor for genomic and deep sequencing data. 
BMC Bioinformatics, 16(1), 179.
