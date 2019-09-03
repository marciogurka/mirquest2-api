#include "gFold.h"

/*************************************************************************************************************************************************/
int SelectionnePalindrome(Palindrome *PalminiList, Parameters *arguments)
{
	int select = -1;

	// ici j'elimine les palindromes plus petits sur la meme diagonale !
	// selection des plus grands palindromes qui forme la meme palindrome ne
	int inutile = 0;
	for(int i=1; i<11; i++)
	{
		if( PalminiList[0].posInter == PalminiList[i].posInter )
		{
			if(PalminiList[0].longueur > PalminiList[i].longueur)
			{
				select = i;
				i = 11;
				
			}
			if(PalminiList[0].longueur == PalminiList[i].longueur)
			{
				if(PalminiList[0].vraiDeb < PalminiList[i].vraiDeb)
				{
					select = i;
					i = 11;
				}
				else
					inutile = 1;
			}
			if(PalminiList[0].longueur < PalminiList[i].longueur)
			{
				inutile = 1;
				i = 11;
			}
		}
	}

	if( (select == -1) && (inutile == 0) )
	{
		int plusMauvais = 100;
		int indice = -1;
		for(int i=1; i<11; i++)
		{
			if( plusMauvais > PalminiList[i].longueur )
			{
				plusMauvais = PalminiList[i].longueur;
				indice = i;
			}
		}
			
		if( plusMauvais < PalminiList[0].longueur)
		{
			select = indice;
			Init1Palindrome(PalminiList, indice);
		}
	}

	return select;
}

/*************************************************************************************************************************************************/
// ici elimine les palindromes non exact qui sont en doubles
void RemoveGraineAndPalNE(Palindrome *listePalT, int nbPalindromeT, Palindrome *PalminiList, int k)
{
	int remplace = -1;
	for(int j=0; j<nbPalindromeT; j++)
	{
		if( (PalminiList[k].vraiDeb == listePalT[j].vraiDeb) && (PalminiList[k].vraiFin == listePalT[j].vraiFin) )
		{
			if( (PalminiList[k].hairpinDeb >= listePalT[j].hairpinDeb) && (PalminiList[k].hairpinFin <= listePalT[j].hairpinFin) )
			{
				PalminiList[k].vraiDeb = 0;
				j = nbPalindromeT;
			}
			
			if( (PalminiList[k].hairpinDeb < listePalT[j].hairpinDeb) && (PalminiList[k].hairpinFin > listePalT[j].hairpinFin) )
			{
				remplace = j;
				j = nbPalindromeT;
			}
		}
		
		if( (PalminiList[k].hairpinDeb >= listePalT[j].hairpinDeb) && (PalminiList[k].hairpinFin <= listePalT[j].hairpinFin) )
		{
			if( (PalminiList[k].hairpinDeb - listePalT[j].hairpinDeb) == (listePalT[j].hairpinFin - PalminiList[k].hairpinFin) )
			{
				PalminiList[k].vraiDeb = 0;
				j = nbPalindromeT;
			}
		}
		
		if( (PalminiList[k].hairpinDeb < listePalT[j].hairpinDeb) && (PalminiList[k].hairpinFin > listePalT[j].hairpinFin) )
		{
			if( (listePalT[j].hairpinDeb - PalminiList[k].hairpinDeb) == (PalminiList[k].hairpinFin - listePalT[j].hairpinFin) )
			{
				remplace = j;
				j = nbPalindromeT;
			}
		}
		
	}
	
	if( (remplace != -1) && (PalminiList[k].vraiDeb != 0) )
	{
		RecopiePal(PalminiList, k, listePalT, remplace);
	}
}

/*************************************************************************************************************************************************/
void ElimineGraine(Palindrome *listePalT, int nbPalindromeT, Parameters *arguments)
{
	for(int i=0; i<nbPalindromeT; i++)
	{
		if( (listePalT[i].vraiDeb != 0) && (listePalT[i].choisi == 1) )
		{
			for(int j=0; j<nbPalindromeT; j++)
			{
				if( (listePalT[j].vraiDeb != 0) && (i != j) && (listePalT[i].choisi == 1) )
				{
					// enleve les graines de meme positions
					if( (listePalT[i].vraiDeb == listePalT[j].vraiDeb) && (listePalT[i].vraiFin == listePalT[j].vraiFin) )
					{
						if( listePalT[i].longueur >= listePalT[j].longueur )
						{
							Init1Palindrome(listePalT, j);
						}
					}

					if(listePalT[i].longueur > listePalT[j].longueur)
					{
						if( (listePalT[i].vraiDeb <= listePalT[j].vraiDeb) 
						 && (listePalT[j].vraiDeb <= listePalT[i].vraiDeb+listePalT[i].longueur) )
						{
							if( (listePalT[i].vraiFin-listePalT[i].longueur <= listePalT[j].vraiFin) 
							 && (listePalT[j].vraiFin                       <= listePalT[i].vraiFin) )
							{
								Init1Palindrome(listePalT, j);
							}
							
							if( (listePalT[i].vraiFin-listePalT[i].longueur <= listePalT[j].vraiFin-listePalT[j].longueur) 
							 && (listePalT[j].vraiFin-listePalT[j].longueur <= listePalT[i].vraiFin) )
							{
								Init1Palindrome(listePalT, j);
							}
						}
						
						if( (listePalT[i].vraiDeb                       <= listePalT[j].vraiDeb+listePalT[j].longueur) 
						 && (listePalT[j].vraiDeb+listePalT[j].longueur <= listePalT[i].vraiDeb+listePalT[i].longueur) )
						{
							if( (listePalT[i].vraiFin-listePalT[i].longueur <= listePalT[j].vraiFin) 
							 && (listePalT[j].vraiFin                       <= listePalT[i].vraiFin) )
							{
								Init1Palindrome(listePalT, j);
							}
							
							if( (listePalT[i].vraiFin-listePalT[i].longueur <= listePalT[j].vraiFin-listePalT[j].longueur) 
							 && (listePalT[j].vraiFin-listePalT[j].longueur <= listePalT[i].vraiFin) )
							{
								Init1Palindrome(listePalT, j);
							}
						}
					}

				}
			}
		}
	}
	


	for(int i=0; i<nbPalindromeT; i++)
	{
		if( (listePalT[i].vraiDeb != 0) && (listePalT[i].choisi == 1) )
		{
			for(int j=0; j<nbPalindromeT; j++)
			{
				if( (listePalT[j].vraiDeb != 0) && (i != j) && (listePalT[j].choisi == 1) )
				{
					// ici palindrome i est inclus avec palindrome j
					if( (listePalT[j].vraiDeb + listePalT[j].longueur < listePalT[i].vraiDeb ) 
					 && (listePalT[j].vraiFin + listePalT[j].longueur > listePalT[i].vraiFin) )
					{
						int ecart = abs( abs(listePalT[i].vraiDeb - listePalT[j].vraiDeb) - abs(listePalT[i].vraiFin - listePalT[j].vraiFin) );
						if( (ecart < listePalT[i].longueur) && (ecart < listePalT[j].longueur) )
						{
							if(listePalT[i].longueur > listePalT[j].longueur)
							{
								Init1Palindrome(listePalT, j);
							}
							if( (listePalT[i].longueur == listePalT[j].longueur) 
							&& ((float_compare(listePalT[i].score, listePalT[j].score) != 0) && (listePalT[i].score < listePalT[j].score)) )
							{
								Init1Palindrome(listePalT, j);
							}
						}
					}

					// ici palindrome i englobe palindrome j
					if( (listePalT[i].vraiDeb + listePalT[i].longueur < listePalT[j].vraiDeb) 
					 && (listePalT[i].vraiFin - listePalT[i].longueur > listePalT[j].vraiFin) )
					{
						int ecart = abs( abs(listePalT[i].vraiDeb - listePalT[j].vraiDeb) - abs(listePalT[i].vraiFin - listePalT[j].vraiFin) );
						if( (ecart < listePalT[i].longueur) && (ecart < listePalT[j].longueur) )
						{
							if(listePalT[i].longueur > listePalT[j].longueur)
							{
								Init1Palindrome(listePalT, j);
							}
							if( (listePalT[i].longueur == listePalT[j].longueur) 
							&& ((float_compare(listePalT[i].score, listePalT[j].score) != 0) && (listePalT[i].score < listePalT[j].score)) )
							{
								Init1Palindrome(listePalT, j);
							}
						}
					}
				}
			}
		}
	}

}

/*************************************************************************************************************************************************/
void ElimineNEPal(Palindrome *listePalT, int nbPalindromeT, Parameters *arguments)
{

 	for(int i=0; i<nbPalindromeT; i++)
	{
		if( (listePalT[i].hairpinDeb != 0) && (listePalT[i].choisi == 1) )
		{
			int finDebutI = listePalT[i].hairpinDeb + listePalT[i].hairpinTaille;
			int debutFinI = listePalT[i].hairpinFin - listePalT[i].hairpinTaille;
			
			for(int j=0; j<nbPalindromeT; j++)
			{
				if( (listePalT[j].hairpinDeb != 0) && (i != j) && (listePalT[i].choisi == 1) )
				{
					int finDebutJ = listePalT[j].hairpinDeb + listePalT[j].hairpinTaille;
					int debutFinJ = listePalT[j].hairpinFin - listePalT[j].hairpinTaille;
					
					int MAXtailleNEPal = listePalT[i].hairpinTaille;
					if(MAXtailleNEPal > listePalT[j].hairpinTaille)
						MAXtailleNEPal = listePalT[j].hairpinTaille;
				
					
					
					
					// i englobe j
					if( (finDebutI <= listePalT[j].hairpinDeb) && (debutFinI >= listePalT[j].hairpinFin) )
					{
						if( (float_compare(listePalT[i].hairpinDG, listePalT[j].hairpinDG) != 0) 
						               && (listePalT[i].hairpinDG < listePalT[j].hairpinDG) )
						{
							Init1Palindrome(listePalT, j);
						}
					}




					// 5' [di [dj fi] -> i chevauche i et i est devant !
					if( (listePalT[i].hairpinDeb <= listePalT[j].hairpinDeb) && (listePalT[j].hairpinDeb < finDebutI)
					 && (listePalT[j].hairpinDeb != 0) )
					{
						// 5' [di fi] fj]
						if(finDebutI < finDebutJ)
						{
							// 3' [di [dj fi]
							if( (debutFinI <= debutFinJ) && (debutFinJ < listePalT[i].hairpinFin) )
							{
								// 3' [dj fi] fj]
								if(listePalT[i].hairpinFin < listePalT[j].hairpinFin) 
								{
									if( ((finDebutI - listePalT[j].hairpinDeb) >= MAXtailleNEPal)
									 && ((listePalT[i].hairpinFin - debutFinJ) >= MAXtailleNEPal) )
									{
										if( (float_compare(listePalT[i].hairpinDG, listePalT[j].hairpinDG) != 0) 
											       && (listePalT[i].hairpinDG < listePalT[j].hairpinDG) )
										{
											Init1Palindrome(listePalT, j);
										}
									}
								}
								else // donc le 3' j est inclus dans 3' i
								{
									if( (finDebutI - listePalT[j].hairpinDeb) >= MAXtailleNEPal)
									{
										if( (float_compare(listePalT[i].hairpinDG, listePalT[j].hairpinDG) != 0) 
											       && (listePalT[i].hairpinDG < listePalT[j].hairpinDG) )
										{
											Init1Palindrome(listePalT, j);
										}
									}
								}
							}
							
							// 3' [dj [di fi]
							if(debutFinJ < debutFinI)
							{
								// 3' [di fj] fi]
								if( (debutFinI < listePalT[j].hairpinFin) && (listePalT[j].hairpinFin <= listePalT[i].hairpinFin) )
								{
									if( ((finDebutI - listePalT[j].hairpinDeb) >= MAXtailleNEPal)
									 && ((listePalT[j].hairpinFin - debutFinI) >= MAXtailleNEPal) )
									{
										if( (float_compare(listePalT[i].hairpinDG, listePalT[j].hairpinDG) != 0) 
											&& (listePalT[i].hairpinDG < listePalT[j].hairpinDG) )
										{
											Init1Palindrome(listePalT, j);
										}
									}
								}
							}
						}
						else // donc le 5' j est inclus dans le 5' i
						{
							// 3' [di [dj fi]
							if( (debutFinI <= debutFinJ) && (debutFinJ < listePalT[i].hairpinFin) )
							{
								// 3' [dj fi] fj]
								if(listePalT[i].hairpinFin < listePalT[j].hairpinFin) 
								{
									if( (listePalT[i].hairpinFin - debutFinJ) > MAXtailleNEPal )
									{
										if( (float_compare(listePalT[i].hairpinDG, listePalT[j].hairpinDG) != 0) 
											       && (listePalT[i].hairpinDG < listePalT[j].hairpinDG) )
										{
											Init1Palindrome(listePalT, j);
										}
									}
								}
								else  // 5' et 3' j sont inclus dans i 
								{
									// l'ecart entre i et j doit etre faible
									if( (listePalT[j].hairpinDeb - listePalT[i].hairpinDeb < MAXtailleNEPal) 
									 && (listePalT[i].hairpinFin - listePalT[j].hairpinFin < MAXtailleNEPal) 
									 && (finDebutI - finDebutJ < MAXtailleNEPal) 
									 && (debutFinI - debutFinI < MAXtailleNEPal) )
									{
										if( (float_compare(listePalT[i].hairpinDG, listePalT[j].hairpinDG) != 0) 
											       && (listePalT[i].hairpinDG < listePalT[j].hairpinDG) )
										{
											Init1Palindrome(listePalT, j);
										}
									}
								}
							}
						}
					}

					// 5' [dj [di fj] -> i chevauche i et i est derriere !
					if( (listePalT[j].hairpinDeb < listePalT[i].hairpinDeb) && (listePalT[i].hairpinDeb < finDebutJ)
					 && (listePalT[j].hairpinDeb != 0) )
					{
						// 5' [di fj] fi]
						if(finDebutJ < finDebutI)
						{
							// 3' [di [dj fi]
							if( (debutFinI <= debutFinJ) && (debutFinJ < listePalT[i].hairpinFin) )
							{
								// 3' [dj fi] fj]
								if(listePalT[i].hairpinFin < listePalT[j].hairpinFin) 
								{
									if( ((finDebutJ - listePalT[i].hairpinDeb) >= MAXtailleNEPal)
									 && ((listePalT[i].hairpinFin - debutFinJ) >= MAXtailleNEPal) )
									{
										if( (float_compare(listePalT[i].hairpinDG, listePalT[j].hairpinDG) != 0) 
											       && (listePalT[i].hairpinDG < listePalT[j].hairpinDG) )
										{
											Init1Palindrome(listePalT, j);
										}
									}
								}
							}
							
							// 3' [dj [di fi]
							if(debutFinJ < debutFinI)
							{
								// 3' [di fj] fi]
								if( (debutFinI < listePalT[j].hairpinFin) && (listePalT[j].hairpinFin <= listePalT[i].hairpinFin) )
								{
									if( ((finDebutJ - listePalT[i].hairpinDeb) >= MAXtailleNEPal)
									 && ((listePalT[j].hairpinFin - debutFinI) >= MAXtailleNEPal) )
									{
										if( (float_compare(listePalT[i].hairpinDG, listePalT[j].hairpinDG) != 0) 
											&& (listePalT[i].hairpinDG < listePalT[j].hairpinDG) )
										{
											Init1Palindrome(listePalT, j);
										}
									}
								}
							}
						}
					}

				}
			}
		}
	
	}

}
