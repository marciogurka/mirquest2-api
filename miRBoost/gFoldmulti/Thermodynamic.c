#include "gFold.h"

/*************************************************************************************************************************************************/
float DeltaG1Base(char haut, char bas)
{
	// valeur perso a la louche
	float score = 0.0;
	
	if(haut == 'A' && bas == 'U')
		score = -0.4;
	else if(haut == 'C' && bas == 'G')
		score = -0.7;
	else if(haut == 'G' && bas == 'C')
		score = -0.6;
	else if(haut == 'U' && bas == 'A')
		score = -0.4;
	
	return score;
}

/*************************************************************************************************************************************************/
float Palindrome1BaseRNA(int debut, int fin, char *text)
{
	// valeur perso a la louche
	float score = 0.0;
	char haut = text[debut-1];
	char bas = text[fin-1];
	
	if(haut == 'A' && bas == 'U')
		score = -0.4;
	else if(haut == 'C' && bas == 'G')
		score = -0.7;
	else if(haut == 'G' && bas == 'C')
		score = -0.6;
	else if(haut == 'G' && bas == 'U')
		score = -0.4;
	else if(haut == 'U' && bas == 'A')
		score = -0.4;
	else if(haut == 'U' && bas == 'G')
		score = -0.4;
	
	return score;
}

/*************************************************************************************************************************************************/
float BoucleTerminal(int distance)
{
	float score = 0.0;
	
	if(distance < 4)
		score = 7.4;
	else if(distance == 4)
		score = 5.6;
	else if(distance == 5)
		score = 4.4;
	else if(distance == 6)
		score = 4.3;
	else if(distance == 7)
		score = 4.1;
	else if(distance == 8)
		score = 4.1;
	else if(distance == 9)
		score = 4.2;
	else if(distance == 10)
		score = 4.3;
	else if(distance == 11)
		score = 4.6;
	else if(distance == 12)
		score = 4.9;
	else if(distance == 13)
		score = 5.2;
	else if(distance == 14)
		score = 5.6;
	else
	{
		int restedistance = distance - 14;
		score = 5.6 + 0.1 * restedistance;
	}
	if(score > 8.9)
		score = 8.9;

	return score;
}

/*************************************************************************************************************************************************/
//To calculate the total energy of a RNA duplex, simply sum the contribution of each pair plus a 
//nucleation term for the first pair, which has been experimentally determined to be 3.4 kcal/mol. 
//It's positive because of entropic loss due to association of two strands.

//					5' basepair
//				GU 	UG 	AU 	UA 	CG 	GC
//			GU 	-0.5 	-0.6 	-0.5 	-0.7 	-1.5 	-1.3
//			UG 	-0.5 	-0.5 	-0.7 	-0.5 	-1.5 	-0.9
// 3' basepair		AU 	-0.5 	-0.7 	-0.9 	-1.1 	-1.8 	-2.3
//			UA 	-0.7 	-0.5 	-0.9 	-0.9 	-1.7 	-2.1
//			CG 	-1.9 	-1.3 	-2.1 	-2.3 	-2.9 	-3.4
//			GC 	-1.5 	-1.5 	-1.7 	-1.8 	-2.0 	-2.9 

//sequence 	5'AA3'	5'AU3'	5'UA3'	5'CA3'	5'CU3'	5'GA3'	5'GU3'	5'CG3'	5'GC3'	5'GG3'
// 		3'UU5'	3'UA5'	3'AU5'	3'GU5'	3'GA5'	3'CU5'	3'CA5'	3'GC5'	3'CG5'	3'CC5'
//ΔG kcal/mol	-0,9 	-0,9 	-1,1 	-1,8 	-1,7 	-2,3 	-2,1 	-2,0 	-3,4 	-2,9


//Loops larger than 12 nucleotides are rarely seen in natural structures. 

//Length 	        1 	 2 	 3 	 4 	 5 	 6 	 7 	 8 	 9 	 10 	 12 	 14 	 16 	 18 	 20 	 25 	 30
//Bulges 	        3.3 	5.2 	6.0 	6.7 	7.4 	8.2 	9.1 	10.0 	10.5 	11.0 	11.8 	12.5 	13.0 	13.6 	14.0 	15.0 	15.8
//Hairpin loops 	∞ 	∞ 	7.4 	5.9 	4.4 	4.3 	4.1 	4.1 	4.2 	4.3 	4.9 	5.6 	6.1 	6.7 	7.1 	8.1 	8.9
//Internal loops 	-- 	0.8 	1.3 	1.7 	2.1 	2.5 	2.6 	2.8 	3.1 	3.6 	4.4 	5.1 	5.6 	6.2 	6.6 	7.6 	8.4 

/*************************************************************************************************************************************************/
float InternalLoop(int distance5, int distance3)
{
	float thermoDistance = 0.0;
	
	if(distance5 == 0 || distance3 == 0)
	{
		int restebulge = abs(distance5 - distance3);

		// Valeur Turner et Freier
		if(restebulge == 1)
			thermoDistance = 3.3;		//3.8
		else if(restebulge == 2)
			thermoDistance = 5.2;		//2.8
		else if(restebulge == 3)
			thermoDistance = 6.0;		//3.2
		else if(restebulge == 4)
			thermoDistance = 6.7;		//3.6
		else if(restebulge == 5)
			thermoDistance = 7.4;		//4.0
		else if(restebulge == 6)
			thermoDistance = 8.2;		//4.4
		else if(restebulge == 7)
			thermoDistance = 9.1;		//4.6
		else if(restebulge == 8)
			thermoDistance = 10.0;		//4.7
		else if(restebulge == 9)
			thermoDistance = 10.5;		//4.8
		else if(restebulge == 10)
			thermoDistance = 11.0;		//4.9
		else if(restebulge == 11)
			thermoDistance = 11.4;		//5.0
		else if(restebulge == 12)
			thermoDistance = 11.8;		//5.1
		else if(restebulge == 13)
			thermoDistance = 12.1;		//5.2
		else if(restebulge == 14)
			thermoDistance = 12.5;		//5.3
		else if(restebulge == 15)
			thermoDistance = 12.8;		//5.4
		else
			thermoDistance = 10000;
	}
	else
	{
		int plusgrand = 0;
		if(distance5 > distance3)
			plusgrand = distance5;
		else
			plusgrand = distance3;


		// valeur Turner et Frieier en symetrie et valeur mfold en asymetrie
		if(plusgrand == 1)
			thermoDistance = thermoDistance + 0.0;
		else if(plusgrand == 2)
		{
			if(distance5 == distance3)
				thermoDistance = thermoDistance + 0.0;
			else
				thermoDistance = thermoDistance + 0.8;
		}
		else if(plusgrand == 3)
		{
			if(distance5 == distance3)
				thermoDistance = thermoDistance + 0.5;
			else
				thermoDistance = thermoDistance + 1.3;
		}
		else if(plusgrand == 4)
		{
			if(distance5 == distance3)
				thermoDistance = thermoDistance + 1.3;
			else
				thermoDistance = thermoDistance + 1.7;
		}
		else if(plusgrand == 5)
		{
			if(distance5 == distance3)
				thermoDistance = thermoDistance + 1.8;
			else
				thermoDistance = thermoDistance + 2.1;
		}
		else if(plusgrand == 6)
		{
			if(distance5 == distance3)
				thermoDistance = thermoDistance + 2.0;
			else
				thermoDistance = thermoDistance + 2.5;
		}
		else if(plusgrand == 7)
		{
			if(distance5 == distance3)
				thermoDistance = thermoDistance + 2.2;
			else
				thermoDistance = thermoDistance + 2.6;
		}
		else if(plusgrand == 8)
		{
			if(distance5 == distance3)
				thermoDistance = thermoDistance + 2.3;
			else
				thermoDistance = thermoDistance + 2.8;
		}
		else if(plusgrand == 9)
			thermoDistance = thermoDistance + 3.1;		//2.4
		else if(plusgrand == 10)
			thermoDistance = thermoDistance + 3.6;		//2.5
		else if(plusgrand == 11)
			thermoDistance = thermoDistance + 4.0;		//2.6
		else if(plusgrand == 12)
			thermoDistance = thermoDistance + 4.4;		//2.7
		else if(plusgrand == 13)
			thermoDistance = thermoDistance + 4.8;		//2.8
		else if(plusgrand == 14)
			thermoDistance = thermoDistance + 5.1;		//2.9
		else if(plusgrand == 15)
			thermoDistance = thermoDistance + 5.3;		//3.0
		else if(plusgrand == 16)
			thermoDistance = thermoDistance + 5.6;		//3.0
		else
			thermoDistance = thermoDistance + 100;				//3.7
	}
	
	return thermoDistance;
}

/*************************************************************************************************************************************************/
float DeltaG_simplePalindromeRNA(int debut, int fin, int longueur, char *window)
{
	float score = 0.0;
	char Cinq1, Cinq2, Trois1, Trois2;
	
	// je scanne tout le palindrome deux a deux nucleotides
	for(int k=0; k<longueur-1; k++)
	{
		Cinq1 = window[debut + k - 1];
		Cinq2 = window[debut + k - 1 + 1];
		Trois1 = window[fin - k - 1];
		Trois2 = window[fin - k - 2];
		
		if(Cinq1 == 'A')
		{
			if(Cinq2 == 'A')
			{
				if( (Trois1 == 'U') && (Trois2 == 'U') )
					score = score - 0.9;
			}
			
			else if(Cinq2 == 'C')
			{
				if( (Trois1 == 'U') && (Trois2 == 'G') )
					score = score - 2.2;
			}
			
			else if(Cinq2 == 'G')
			{
				if(Trois1 == 'U')
				{
					if(Trois2 == 'C')
						score = score - 2.1;
					
					if(Trois2 == 'U')
						score = score - 0.6;
				}
			}
			
			else if(Cinq2 == 'U')
			{
				if(Trois1 == 'U')
				{
					if(Trois2 == 'A')
						score = score - 1.1;
					
					if(Trois2 == 'G')
						score = score - 1.4;
				}
			}
		}
		
		else if(Cinq1 == 'C')
		{
			if(Cinq2 == 'A')
			{
				if( (Trois1 == 'G') && (Trois2 == 'U') )
					score = score - 2.1;
			}
			
			else if(Cinq2 == 'C')
			{
				if( (Trois1 == 'G') && (Trois2 == 'G') )
					score = score - 3.3;
			}
			
			else if(Cinq2 == 'G')
			{
				if(Trois1 == 'G')
				{
					if(Trois2 == 'C')
						score = score - 2.4;
					
					if(Trois2 == 'U')
						score = score - 1.4;
				}
			}
			
			else if(Cinq2 == 'U')
			{
				if(Trois1 == 'G')
				{
					if(Trois2 == 'A')
						score = score - 2.1;
					
					if(Trois2 == 'G')
						score = score - 2.1;
				}
			}
		}
		
		else if(Cinq1 == 'G')
		{
			if(Cinq2 == 'A')
			{
				if( (Trois1 == 'C') && (Trois2 == 'U') )
					score = score - 2.4;
				
				else if( (Trois1 == 'U') && (Trois2 == 'U') )
					score = score - 1.3;
			}
			
			else if(Cinq2 == 'C')
			{
				if( (Trois1 == 'C') && (Trois2 == 'G') )
					score = score - 3.4;
				
				else if( (Trois1 == 'U') && (Trois2 == 'G') )
					score = score - 2.5;
			}
			
			else if(Cinq2 == 'G')
			{
				if(Trois1 == 'C')
				{
					if(Trois2 == 'C')
						score = score - 3.3;
					
					if(Trois2 == 'U')
						score = score - 1.5;
				}
				
				else if(Trois1 == 'U')
				{
					if(Trois2 == 'C')
						score = score - 2.1;
					
					if(Trois2 == 'U')
						score = score - 0.5;
				}
			}
			
			else if(Cinq2 == 'U')
			{
				if(Trois1 == 'C')
				{
					if(Trois2 == 'A')
						score = score - 2.2; // valeur bizarre !
						
					else if(Trois2 == 'G')
						score = score - 2.5;
				}
				
				else if(Trois1 == 'U')
				{
					if(Trois2 == 'A')
						score = score - 1.4;
					
					else if(Trois2 == 'G')
						score = score + 1.3;
					
				}
			}
		}
		
		else if(Cinq1 == 'U')
		{
			if(Cinq2 == 'A')
			{
				if( (Trois1 == 'A') && (Trois2 == 'U') )
					score = score - 1.3;
				
				else if( (Trois1 == 'G') && (Trois2 == 'U') )
					score = score - 1.0;
			}
			
			else if(Cinq2 == 'C')
			{
				if( (Trois1 == 'A') && (Trois2 == 'G') )
					score = score - 2.4;
				
				else if( (Trois1 == 'G') && (Trois2 == 'G') )
					score = score - 1.5;
			}
			
			else if(Cinq2 == 'G')
			{
				if(Trois1 == 'A')
				{
					if(Trois2 == 'C')
						score = score - 2.1;
					
					if(Trois2 == 'U')
						score = score - 1.0; 
				}
				
				else if(Trois1 == 'G')
				{
					if(Trois2 == 'C')
						score = score - 1.4;
					
					if(Trois2 == 'U')
						score = score + 0.3; 
				}
			}
			
			else if(Cinq2 == 'U')
			{
				if(Trois1 == 'A')
				{
					if(Trois2 == 'A')
						score = score - 0.9;
					
					if(Trois2 == 'G')
						score = score - 1.3;
				}
				
				else if(Trois1 == 'G')
				{
					if(Trois2 == 'A')
						score = score - 0.6;
					
					if(Trois2 == 'G')
						score = score - 0.5;
				}
			}
		}
		
		if( score <= 0.0001 && score >= -0.0001)
			score = 0.0;
	}
	
	return score;
}

/*************************************************************************************************************************************************/
float DeltaG_Hairpin_Intern(int indice5, int indice3, PalindromeSimple *listePal)
{
	float DELTAG = 0.0;	// pas mis de -3.4 car on enleve la valeur du dernier DG dans le palindrome
	
	if(indice3 != -1)
	{
		// je calcule le cout entre ce palindrome et le suivant
		int distance5 = listePal[indice3].vraiDeb - (listePal[indice5].vraiDeb + listePal[indice5].longueur);
		int distance3 = (listePal[indice5].vraiFin - listePal[indice5].longueur) - listePal[indice3].vraiFin; 
			
		DELTAG = InternalLoop(distance5, distance3) + listePal[indice3].score;
	}
	else	// c'est le palindrome le plus interne donc je calcule la tige boucle
	{
		DELTAG = BoucleTerminal(listePal[indice5].distance);
	}
	
	return DELTAG;
}

/*************************************************************************************************************************************************/
float DeltaG_Hairpin_Extern(int indice5, int indice3, PalindromeSimple *listePal)
{
	float DELTAG = 0.0;	// pas mis de -3.4 car on enleve la valeur du dernier DG dans le palindrome
	
	if(indice5 != -1)
	{
		// je calcule le cout entre ce palindrome et le suivant
		int distance5 = listePal[indice3].vraiDeb - (listePal[indice5].vraiDeb + listePal[indice5].longueur);
		int distance3 = (listePal[indice5].vraiFin - listePal[indice5].longueur) - listePal[indice3].vraiFin; 
		
		DELTAG = listePal[indice5].score + InternalLoop(distance5, distance3);
	}
		
	return DELTAG;
}

/*************************************************************************************************************************************************/
float ThermoHairpin(PalindromeSimple *listePal, int indice5, int indice3)
{
	float DELTAG = 0.0;	// pas mis de -3.4 car on enleve la valeur du dernier DG dans le palindrome
	
	if(indice3 != -1)
	{
		// je calcule le cout entre ce palindrome et le suivant
		int distance5 = listePal[indice3].vraiDeb - (listePal[indice5].vraiDeb + listePal[indice5].longueur);
		int distance3 = (listePal[indice5].vraiFin - listePal[indice5].longueur) - listePal[indice3].vraiFin; 
			
		float espace = InternalLoop(distance5, distance3);
		DELTAG = listePal[indice5].score + espace; 
	}
	else // c'est le palindrome le plus interne donc je calcule la tige boucle
	{
		DELTAG =  listePal[indice5].score;
		DELTAG =  DELTAG + BoucleTerminal(listePal[indice5].distance);
	}

	return DELTAG;
}
