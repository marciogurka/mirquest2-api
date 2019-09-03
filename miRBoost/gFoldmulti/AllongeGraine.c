#include "gFold.h"

/*************************************************************************************************************************************************/
int MeilleurPalindromeInterieur(PalindromeSimple *listePal, int nouvelleIndice, int NbPalindrome, float *score, int debutPalindrome)
{
	int indice = -1;
	// je recupere les limites interieurs du palindrome choisi
	int interieurDeb = listePal[nouvelleIndice].vraiDeb + listePal[nouvelleIndice].longueur;
	int interieurFin = listePal[nouvelleIndice].vraiFin - listePal[nouvelleIndice].longueur;
	float scoreMin = 3.4 * (interieurFin - interieurDeb);	// recherche le palindrome le meilleur DG
	
	for(int i=debutPalindrome; i<NbPalindrome; i++)
	{
		// ici je suis a l'interieur du palindrome precedemment trouve
		if( (listePal[i].vraiDeb >= interieurDeb) && (listePal[i].vraiFin <= interieurFin) )
		{
			int distance5 = listePal[i].vraiDeb - interieurDeb;
			int distance3 = interieurFin - listePal[i].vraiFin;
			float scoreLoop = InternalLoop(distance5, distance3);	// score entre le palindrome choisi et le nouveau
			
			if( (distance3 + distance5 > 0) 
			 && (listePal[i].longueur > 1) 
			 && (listePal[i].distance >= 3) )
			{
				if( (float_compare(scoreMin, (scoreLoop + listePal[i].score)) != 0) 
				 && ((scoreLoop + listePal[i].score) < scoreMin) 
				 && ((scoreLoop + listePal[i].score) <= 0.0) )
				{
					indice = i;
					scoreMin = scoreLoop + listePal[i].score;
				}
			}
		}
	}
	
	*score = scoreMin;

	return indice;
}

/*************************************************************************************************************************************************/
void AlongeHairpinInterieur(PalindromeSimple *listePal, int NbPalindrome, Parameters *arguments, 
			   int debutPalindrome, int indiceSeed)
{
	listePal[indiceSeed].nbAsso = 0;
	float bestScore = 0.0;
	int indice = MeilleurPalindromeInterieur(listePal, indiceSeed, NbPalindrome, &bestScore, debutPalindrome);

	if(indice != -1)
	{
		float scoreLimit = bestScore - (bestScore * arguments->incertitude);
		int interieurDeb = listePal[indiceSeed].vraiDeb + listePal[indiceSeed].longueur;
		int interieurFin = listePal[indiceSeed].vraiFin - listePal[indiceSeed].longueur;

		for(int i=debutPalindrome; i<NbPalindrome; i++)
		{
			// ici je suis a l'interieur du palindrome precedemment trouve
			if( (listePal[i].vraiDeb >= interieurDeb) 
			 && (listePal[i].vraiFin <= interieurFin) 
			 && (listePal[i].longueur > 1) )
			{
				int distance5 = listePal[i].vraiDeb - interieurDeb;
				int distance3 = interieurFin - listePal[i].vraiFin;
				float scoreLoop = InternalLoop(distance5, distance3);

				// je prends tous les palindromes exceptes ceux de taille 1
				// que je garde pour boucher les espaces entre palindromes
				// je ne prends pas les palindromes qui forment de trop grand boucle asymetrique
				if( (distance3 + distance5 > 0)  
				&& (listePal[i].longueur > 1)
				&& (listePal[i].distance >= 3) )
				{
					if( (float_compare(scoreLimit, (scoreLoop + listePal[i].score)) != 0) 
					&& ((scoreLoop + listePal[i].score) < scoreLimit) 
					&& ((scoreLoop + listePal[i].score) <= 0.0) )
					{
						if(listePal[indiceSeed].nbAsso <= 20)
						{
							listePal[indiceSeed].association[listePal[indiceSeed].nbAsso] = i;
							listePal[indiceSeed].nbAsso++;
						}
						else
							SelectionnePotentiel(listePal, i, arguments, scoreLimit, indiceSeed);
					}
				}
			}
				
			if( (listePal[i].vraiDeb + listePal[i].longueur < listePal[indiceSeed].vraiDeb) 
			 && (listePal[i].vraiDeb + listePal[i].longueur > debutPalindrome) )
				debutPalindrome = i;
		}
	}

	for(int i=0; i<listePal[indiceSeed].nbAsso; i++)
		AlongeHairpinInterieur(listePal, NbPalindrome, arguments, debutPalindrome, listePal[indiceSeed].association[i]);
}

/*************************************************************************************************************************************************/
int MeilleurPalindromeExterieur(PalindromeSimple *listePal, int nouvelleIndice, int NbPalindrome, float *score)
{
	int indice = -1;
	int ExterieurDeb = listePal[nouvelleIndice].vraiDeb;
	int ExterieurFin = listePal[nouvelleIndice].vraiFin;
	float scoreMin = 3.4 * (ExterieurFin - ExterieurDeb) + 1;	// recherche le palindrome le meilleur DG
	int gdDistance = 0;

	for(int i=0; i<NbPalindrome; i++)
	{
		// ici je suis a l'exterieur du palindrome precedemment trouve
		if( (listePal[i].vraiDeb + listePal[i].longueur <= ExterieurDeb) 
		 && (listePal[i].vraiFin - listePal[i].longueur >= ExterieurFin) )
		{
			int distance5 = ExterieurDeb - (listePal[i].vraiDeb + listePal[i].longueur);
			int distance3 = (listePal[i].vraiFin - listePal[i].longueur) - ExterieurFin;
			float scoreLoop = InternalLoop(distance5, distance3);
			if(distance5 > distance3)
				gdDistance = distance5;
			else
				gdDistance = distance3;
				
			// je prends tous les palindromes exceptes ceux de taille 1
			// que je garde pour boucher les espaces entre palindromes
			if( (distance3 + distance5 > 0) 
			 && (listePal[i].longueur > 1) )
			{
				if( (float_compare(scoreMin, (scoreLoop + listePal[i].score)) != 0) 
				&& ((scoreLoop + listePal[i].score) < scoreMin) 
				&& ((scoreLoop + listePal[i].score) <= 0.0) )
				{
					// la distance mesapparie entre le nouveau palindrome et l'hairpin 
					// est assez petite pour associer ce palindrome a l'hairpin
					if(gdDistance + listePal[nouvelleIndice].mesappariement <= listePal[nouvelleIndice].appariement + listePal[i].longueur)
					{
						indice = i;
						scoreMin = scoreLoop + listePal[i].score;
					}
				}
			}
		}
		if(listePal[i].vraiDeb > ExterieurDeb)
			i = NbPalindrome;
	}
	
	*score = scoreMin;

	return indice;
}

/*************************************************************************************************************************************************/
void AlongeHairpinExterieur(PalindromeSimple *listePal, int NbPalindrome, Parameters *arguments, int indiceSeed)
{
	listePal[indiceSeed].nbAsso = 0;
	float bestScore = 0.0;
	int indice = MeilleurPalindromeExterieur(listePal, indiceSeed, NbPalindrome, &bestScore);

	if(indice != -1)
	{
		float scoreLimit = bestScore - (bestScore * arguments->incertitude);
		int ExterieurDeb = listePal[indiceSeed].vraiDeb;
		int ExterieurFin = listePal[indiceSeed].vraiFin;
		int gdDistance = 0;

		for(int i=0; i<NbPalindrome; i++)
		{
			// ici je suis a l'exterieur du palindrome precedemment trouve
			if( (listePal[i].vraiDeb + listePal[i].longueur <= ExterieurDeb) 
			 && (listePal[i].vraiFin - listePal[i].longueur >= ExterieurFin) 
			 && (listePal[i].longueur > 1) )
			{
				int distance5 = ExterieurDeb - (listePal[i].vraiDeb + listePal[i].longueur);
				int distance3 = (listePal[i].vraiFin - listePal[i].longueur) - ExterieurFin;
				float scoreLoop = InternalLoop(distance5, distance3);
				if(distance5 > distance3)
					gdDistance = distance5;
				else
					gdDistance = distance3;
	
				// je prends tous les palindromes exceptes ceux de taille 1
				// que je garde pour boucher les espaces entre palindromes
				// je prends pas les palindromes qui forment de trop grande loop asymetrique
				if( (distance3 + distance5 > 0) 
				 && (listePal[i].longueur > 1) )
				{
					if( (float_compare(scoreLimit, (scoreLoop + listePal[i].score)) != 0) 
					&& ((scoreLoop + listePal[i].score) < scoreLimit) 
					&& ((scoreLoop + listePal[i].score) <= 0.0) )
					{
						// la distance mesapparie entre le nouveau palindrome et l'hairpin 
						// est assez petite pour associer ce palindrome a l'hairpin
						if(gdDistance + listePal[indiceSeed].mesappariement <= listePal[indiceSeed].appariement + listePal[i].longueur)
						{
							if(listePal[indiceSeed].nbAsso <= 20)
							{
								listePal[indiceSeed].association[listePal[indiceSeed].nbAsso] = i;
								listePal[indiceSeed].nbAsso++;
								listePal[i].appariement = listePal[indiceSeed].appariement + listePal[i].longueur;
								listePal[i].mesappariement = listePal[indiceSeed].mesappariement + gdDistance;
							}
							else
								SelectionnePotentiel(listePal, i, arguments, scoreLimit, indiceSeed);
						}
					}
				}
			}

			if(listePal[i].vraiDeb > ExterieurDeb)
				i = NbPalindrome;
		}
	}

	for(int i=0; i<listePal[indiceSeed].nbAsso; i++)
		AlongeHairpinExterieur(listePal, NbPalindrome, arguments, listePal[indiceSeed].association[i]);
}

/*************************************************************************************************************************************************/
int EspaceEntrePalindrome(PalindromeSimple *listePal, int NbPalindrome, int Indice5, int Indice3)
{
	int indice = -1;
	int debut5 = listePal[Indice5].vraiDeb + listePal[Indice5].longueur;
	int fin5 = listePal[Indice5].vraiFin - listePal[Indice5].longueur;
	int debut3 = listePal[Indice3].vraiDeb;
	int fin3 = listePal[Indice3].vraiFin;
	float scoreMin = 3.4 * (fin5 - debut5 + 1);		// recherche le palindrome le meilleur DG
	float scoreBoucleInterne = InternalLoop((debut3-debut5), (fin5-fin3)); // thermodynamic de la boucle interne actuelle
	
	for(int i=0; i<NbPalindrome; i++)
	{
		if( (listePal[i].vraiDeb >= debut5) && (listePal[i].vraiDeb+listePal[i].longueur <= debut3) 
		 && (listePal[i].vraiFin <= fin5)   && (listePal[i].vraiFin-listePal[i].longueur >= fin3) )
		{
			int distance51 = listePal[i].vraiDeb - debut5;
			int distance31 = fin5 - listePal[i].vraiFin;
			float scoreLoopAvant = InternalLoop(distance51, distance31);
			
			int distance52 = debut3 - (listePal[i].vraiDeb + listePal[i].longueur);
			int distance32 = (listePal[i].vraiFin - listePal[i].longueur) - fin3;
			float scoreLoopApres = InternalLoop(distance52, distance32);
			
			if( (distance31 + distance51 > 0) && (distance52 + distance32 > 0) )
			{
				if( (float_compare((scoreLoopAvant + scoreLoopApres + listePal[i].score), scoreMin) != 0) 
				 && (float_compare((scoreLoopAvant + scoreLoopApres + listePal[i].score), scoreBoucleInterne) != 0)
					       && ((scoreLoopAvant + scoreLoopApres + listePal[i].score) < scoreMin ) 
					       && ((scoreLoopAvant + scoreLoopApres + listePal[i].score) < scoreBoucleInterne) )
				{
					if(listePal[i].longueur > 1)
					{
						indice = i;
						scoreMin = scoreLoopAvant + scoreLoopApres + listePal[i].score;
					}
					else if( (distance51 == distance31) || (distance52 == distance32) )
					{
						indice = i;
						scoreMin = scoreLoopAvant + scoreLoopApres + listePal[i].score;
					}
				}
			}
		}
	}
	
	return indice;
}

/*************************************************************************************************************************************************/
void RemplitVideInterne(PalindromeSimple *PalHairpin, int graine, int NbPalindrome, Parameters *arguments)
{
	int z = graine;
	while(PalHairpin[z].indiceSuivant != -1)
	{
		int newIndice = 1;
		int Indice5 = z;
		int Indice3 = PalHairpin[z].indiceSuivant;
		while(newIndice != -1)
		{
			newIndice = EspaceEntrePalindrome(PalHairpin, NbPalindrome, Indice5, Indice3);
			if(newIndice != -1)
			{
				PalHairpin[Indice5].indiceSuivant = newIndice;
				PalHairpin[newIndice].hairpinORnot = 1;
				PalHairpin[newIndice].choisi = 1;
				PalHairpin[newIndice].indiceSuivant = Indice3;
				Indice5 = newIndice;
			}
		}
		
		z = Indice3;
	}
}

/*************************************************************************************************************************************************/
void RemplitVideExterne(PalindromeSimple *PalHairpin, int premier, int graine, int NbPalindrome)
{
	int z = premier;
	while(z != graine)
	{
		int newIndice = 1;
		int Indice5 = z;
		int Indice3 = PalHairpin[z].indiceSuivant;
		while(newIndice != -1)
		{
			newIndice = EspaceEntrePalindrome(PalHairpin, NbPalindrome, Indice5, Indice3);
			if(newIndice != -1)
			{
				PalHairpin[Indice5].indiceSuivant = newIndice;
				PalHairpin[newIndice].hairpinORnot = 1;
				PalHairpin[newIndice].choisi = 1;
				PalHairpin[newIndice].indiceSuivant = Indice3;
				Indice3 = newIndice;
			}
		}
		
		z = Indice3;
	}
}

/*************************************************************************************************************************************************/
void miniPalindromeBoucle(PalindromeSimple *listePal, int graine, int NbPalindrome)
{
	// je recupere le dernier palindrome 
	int nouvelleIndice = graine;
	while(listePal[nouvelleIndice].indiceSuivant != -1)
		nouvelleIndice = listePal[nouvelleIndice].indiceSuivant;

	int indice = 1;
	while(indice != -1)
	{
		indice = -1;
		int debut = listePal[nouvelleIndice].vraiDeb + listePal[nouvelleIndice].longueur;
		int fin   = listePal[nouvelleIndice].vraiFin - listePal[nouvelleIndice].longueur;
		float scoreMin = 3.4 * (fin - debut) + 1;	// recherche le palindrome le meilleur DG
		
		for(int i=0; i<NbPalindrome; i++)
		{
			if( (listePal[i].vraiDeb >= debut) 
			 && (listePal[i].vraiFin <= fin) 
			 && (listePal[i].distance >= 3) )
			{
				int distance5 = listePal[i].vraiDeb - debut;
				int distance3 = fin - listePal[i].vraiFin;
				float scoreLoop = InternalLoop(distance5, distance3);
				
				if( (distance5 + distance3 > 0)
				 && (distance3 <= listePal[i].longueur+1) 
				 && (distance5 <= listePal[i].longueur+1) )
				{
					if( (float_compare((scoreLoop + listePal[i].score), scoreMin) != 0) 
						&& ((scoreLoop + listePal[i].score) < scoreMin) 
						&& ((scoreLoop + listePal[i].score) <= 0.0) )
					{
						if(listePal[i].longueur > 2)
						{
							indice = i;
							scoreMin = scoreLoop + listePal[i].score;
						}
						else if(distance5 == distance3)
						{
							indice = i;
							scoreMin = scoreLoop + listePal[i].score;
						}
					}
				}
			}
		}

		if(indice != -1)
		{
			listePal[nouvelleIndice].indiceSuivant = indice;
			listePal[indice].hairpinORnot = 1;
			listePal[indice].indiceSuivant = -1;
			nouvelleIndice = indice;
		}

	}
}
