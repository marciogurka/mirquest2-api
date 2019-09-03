#include "gFold.h"

/*************************************************************************************************************************************************/
Matrice *CreateMatrix(int taille)
{
	Matrice *M = NULL;
	M = new Matrice;
	if(M == NULL)
	{
		fprintf(stderr, "Problem with M matrice allocation memory \n");
		exit(EXIT_FAILURE);
	}
	
	M->taille = taille;
	
	M->sequence = new char[taille+1];
	if(M->sequence == NULL)
	{
		fprintf(stderr, "Problem with M.sequence matrice allocation memory \n");
		exit(EXIT_FAILURE);
	}
	
	M->tab = new int *[taille+1];
	if(M->tab == NULL)
	{
		fprintf(stderr, "Problem with M.tab matrice allocation memory \n");
		exit(EXIT_FAILURE);
	}
	for(int i=0; i<taille; i++)
	{
		M->tab[i] = new int[taille+1];
		if(M->tab[i] == NULL)
		{
			fprintf(stderr, "Problem with M.tab[i] matrice allocation memory \n");
			exit(EXIT_FAILURE);
		}
	}
	
	return M;
}

/*************************************************************************************************************************************************/
void ClearMatrix(Matrice *M)
{
	delete(M->tab);
	delete(M->sequence);
	delete(M);
}

/*************************************************************************************************************************************************/
void InitialiseMatrice(Matrice *M, char *text, int taille, int debut)
{
	for(int i=0; i<=taille; i++)
	{
		M->sequence[i] = text[i+debut];
		
		for(int j=0; j<=taille; j++)
			M->tab[i][j] = 0;
	}
	M->sequence[taille] = '\0';
}

/*************************************************************************************************************************************************/
void RecherchePalindrome(Matrice *matrixPalindrome, Palindrome *PalminiList, int debutIntervalle, 
			 int tailleMaxUser, Parameters *arguments, int debfinSeq, int limitHairpin)
{
	int taille = matrixPalindrome->taille;
	int compteur = 0;
	int longueur = 0;

	/* FILLING OF SCORING MATRIX AND SEARCH OF PALINDROMES IN PARALLEL :
	* First step : From bottom to top
	* 
	* 7 * * * * *
	* 5 8 * * * *
	* 3 6 9 * * *
	* 2 4 * * * *
	* 1 * * * * *
	* * * * * * * */

	for(int i=taille-1; i>=0; i--)
	{
		if((taille-1-i)%2 == 0)
			compteur++;
		longueur = 0;
		
			for(int j=0; j<compteur; j++)
			{
				char x = matrixPalindrome->sequence[j];
				char y = matrixPalindrome->sequence[taille - i - j - 1];
				
				if(x == 'G')
				{
					if(y == 'C')
					{
						if(j>0)
							matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					if(y == 'U')
					{
						if(j>0)
						{
							char a = matrixPalindrome->sequence[j-1];
							char b = matrixPalindrome->sequence[taille - i+1 - j - 1];
							if( (a == 'U') && (b == 'G') )
							{
								matrixPalindrome->tab[i+j][j] = 0;
								matrixPalindrome->tab[i+j-1][j-1] = 0;
							}
							else
								matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						}
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					if( (y != 'U') && (y != 'C') ) 
						matrixPalindrome->tab[i+j][j] = 0;
				}
				else if(x == 'U')
				{
					if(y == 'A')
					{
						if(j>0)
							matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					if(y == 'G')
					{
						if(j>0)
						{
							char a = matrixPalindrome->sequence[j-1];
							char b = matrixPalindrome->sequence[taille - i+1 - j - 1];
							if( (a == 'G') && (b == 'U') )
							{
								matrixPalindrome->tab[i+j][j] = 0;
								matrixPalindrome->tab[i+j-1][j-1] = 0;
							}
							else
								matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						}
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					if( (y != 'A') && (y != 'G') ) 
						matrixPalindrome->tab[i+j][j] = 0;
				}
				else if(x == 'A')
				{
					if( (y == 'U') )
					{
						if(j>0)
							matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					else
						matrixPalindrome->tab[i+j][j] = 0;
				}
				else if(x == 'C')
				{
					if(y == 'G')
					{
						if(j>0)
							matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					else
						matrixPalindrome->tab[i+j][j] = 0;
				}
				else
					matrixPalindrome->tab[i+j][j] = 0;

				if(matrixPalindrome->tab[i+j][j] == 0)
					longueur = 0;
			}
	}

	/* FILLING OF SCORING MATRIX AND SEARCH OF PALINDROMES IN PARALLEL :
	* Second step : From left to rigth
	* 
	* * 1 3 5 6 *
	* * * 2 4 * *
	* * * * * * *
	* * * * * * *
	* * * * * * *
	* * * * * * *  */

	for(int i=1; i<taille-1; i++)
	{
		if((taille-1-i)%2 != 0)
		{
			compteur--;
			longueur = 0;
		}
		longueur = 0;
		

			for(int j=0; j<=compteur; j++)
			{
				char x = matrixPalindrome->sequence[i+j];
				char y = matrixPalindrome->sequence[taille-j-1];

				if(x == 'G')
				{
					if(y == 'C')
					{
						if( (j>0 && i>0) || j>0)
							matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					if(y == 'U')
					{
						if( (j>0 && i>0) || j>0)
						{
							char a = matrixPalindrome->sequence[i-1+j];
							char b = matrixPalindrome->sequence[taille-j];
							if( (a == 'U') && (b == 'G') )
							{
								matrixPalindrome->tab[j][j+i] = 0;
								matrixPalindrome->tab[j-1][j+i-1] = 0;
							}
							else
								matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						}
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					if( (y != 'U') && (y != 'C') )
						matrixPalindrome->tab[j][j+i] = 0;
				}
				else if(x == 'U')
				{
					if(y == 'A')
					{
						if( (j>0 && i>0) || (j>0))
							matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					if(y == 'G')
					{
						if( (j>0 && i>0) || (j>0))
						{
							char a = matrixPalindrome->sequence[i-1+j];
							char b = matrixPalindrome->sequence[taille-j];
							if( (a == 'G') && (b == 'U') )
							{
								matrixPalindrome->tab[j][j+i] = 0;
								matrixPalindrome->tab[j-1][j+i-1] = 0;
							}
							else
								matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						}
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					if( (y != 'A') && (y != 'G') )
						matrixPalindrome->tab[j][j+i] = 0;
				}
				else if(x == 'A')
				{
					if( (y == 'U') )
					{
						if((j>0 && i>0) || (j>0))
							matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					else
						matrixPalindrome->tab[j][j+i] = 0;
				}
				else if(x == 'C')
				{
					if(y=='G')
					{
						if((j>0 && i>0) || (j>0))
							matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					else
						matrixPalindrome->tab[j][j+i] = 0;
				}
				else
					matrixPalindrome->tab[j][j+i] = 0;


				if(matrixPalindrome->tab[j][j+i] == 0)
					longueur = 0;
			}
	}



	// je remplis la liste de palindrome
	for(int i=0; i<matrixPalindrome->taille; i++)
	{
		for(int j=0; j<matrixPalindrome->taille-i; j++)
		{
			// je suis a la fin du palindrome je le mets en memoire
			if( (matrixPalindrome->tab[i][j] >= 2) && (matrixPalindrome->tab[i+1][j+1] == 0) )
			{
				int longueurP = matrixPalindrome->tab[i][j];
				int start = j + 1 - (longueurP - 1) + debutIntervalle;
				int end = matrixPalindrome->taille - i + (longueurP - 1) + debutIntervalle;

				float ft = DeltaG_simplePalindromeRNA((start-debutIntervalle), (end-debutIntervalle), longueurP, matrixPalindrome->sequence);
				NouveauPal(PalminiList, 0, start, end, longueurP, ft);
				PalminiList[0].hairpinDG = ft;
				PalminiList[0].hairpinPal = 1;

				if( (PalminiList[0].distance >= 3) )
				{
					int select = SelectionnePalindrome(PalminiList, arguments);
					if(select != -1)
					{
						int newI = i - (longueurP - 1);
						int newJ = j - (longueurP - 1);
						int precedI = i - longueurP;
						int mesapp = 0;
						int premierUN = 0;
						int debHairpin = 0;
						int finHairpin = 0;
						int precedLong = longueurP;

						while( (newI >= 0) && (newJ >= 0) )
						{
							if(matrixPalindrome->tab[newI][newJ] > 0)
							{
								if(matrixPalindrome->tab[newI+1][newJ+1] == 0)
								{
									// ici le palindrome a inserer est de longeur 2 ou plus
									// pour l'accepter sa valeur thermo doit etre inferieur
									if(matrixPalindrome->tab[newI][newJ] > 1)
									{
										int longueurP2 = matrixPalindrome->tab[newI][newJ];
										int start2 = newJ + 1 - (longueurP2 - 1) + debutIntervalle;
										int end2 = matrixPalindrome->taille - newI + (longueurP2 - 1) + debutIntervalle;
										float ft2 = DeltaG_simplePalindromeRNA((start2-debutIntervalle), (end2-debutIntervalle), longueurP2, matrixPalindrome->sequence);
										float DGloop = InternalLoop(mesapp, mesapp);
										
										// la boucle interne coute moins cher (thermo) que le nouveau palindrome
										// je considere que c'est bon est je vais continuer
										if( (precedLong+1 >= mesapp) && (longueurP2+1 >= mesapp) )
										{
											PalminiList[0].hairpinMesap = PalminiList[0].hairpinMesap + mesapp;
											PalminiList[0].hairpinDG = PalminiList[0].hairpinDG + ft2 + DGloop;
											PalminiList[0].hairpinPal++;
											if(PalminiList[0].hairpinMAXloop < mesapp)
												PalminiList[0].hairpinMAXloop = mesapp;
											precedI = newI - longueurP2;
											premierUN =  0;
										}
										else
											newI = newJ = 0;
									}
									// ici le palindrome est de taille est 1
									// donc la loop interne ne doit pas depasser 2
									else
									{
										int start2 = newJ + 1 + debutIntervalle;
										int end2 = matrixPalindrome->taille - newI + debutIntervalle;
										float ft2 = Palindrome1BaseRNA(start2-debutIntervalle, end2-debutIntervalle, matrixPalindrome->sequence);
										
										if(mesapp == 2)
										{
											if(premierUN == 0)
											{
												PalminiList[0].hairpinMesap = PalminiList[0].hairpinMesap + mesapp;
												PalminiList[0].hairpinDG = PalminiList[0].hairpinDG + ft2;
												PalminiList[0].hairpinPal++;
												if(PalminiList[0].hairpinMAXloop < mesapp)
													PalminiList[0].hairpinMAXloop = mesapp;
												precedI = newI - 1;
												premierUN =  1;
											}
											else
												newI = newJ = 0;
										}
										if(mesapp == 1)
										{
											PalminiList[0].hairpinMesap = PalminiList[0].hairpinMesap + mesapp;
											PalminiList[0].hairpinDG = PalminiList[0].hairpinDG + ft2;
											PalminiList[0].hairpinPal++;
											if(PalminiList[0].hairpinMAXloop < mesapp)
												PalminiList[0].hairpinMAXloop = mesapp;
											precedI = newI - 1;
											premierUN = 0;
										}
										if(mesapp > 2)
											newI = newJ = 0;
									}
									mesapp = 0;
									precedLong = matrixPalindrome->tab[newI][newJ];
								}
							}
							else
							{
								mesapp++;
							}
							newI--;
							newJ--;
						}
						debHairpin = precedI;
						
						
						
						newI = i+1;
						newJ = j+1;
						precedI = i;
						mesapp = 0;
						premierUN =  0;
						precedLong = PalminiList[0].longueur;
						while( ((newI+3) + (newJ+3) <= matrixPalindrome->taille) )
						{
							if(matrixPalindrome->tab[newI][newJ] > 0)
							{
								if(matrixPalindrome->tab[newI+1][newJ+1] == 0)
								{
									// ici le palindrome a inserer est de longeur 2 ou plus
									// pour l'accepter sa valeur thermo doit etre inferieur
									if(matrixPalindrome->tab[newI][newJ] > 1)
									{
										int longueurP2 = matrixPalindrome->tab[newI][newJ];
										int start2 = newJ + 1 - (longueurP2 - 1) + debutIntervalle;
										int end2 = matrixPalindrome->taille - newI + (longueurP2 - 1) + debutIntervalle;
										float ft2 = DeltaG_simplePalindromeRNA((start2-debutIntervalle), (end2-debutIntervalle), longueurP2, matrixPalindrome->sequence);
										float DGloop = InternalLoop(mesapp, mesapp);
										
										// la boucle interne coute moins cher (thermo) que le nouveau palindrome
										// je considere que c'est bon est je vais continuer
										if( (precedLong+1 >= mesapp) && (longueurP2+1 >= mesapp) )
										{
											PalminiList[0].hairpinMesap = PalminiList[0].hairpinMesap + mesapp;
											PalminiList[0].hairpinDG = PalminiList[0].hairpinDG + ft2 + DGloop;
											PalminiList[0].hairpinPal++;
											if(PalminiList[0].hairpinMAXloop < mesapp)
												PalminiList[0].hairpinMAXloop = mesapp;
											precedI = newI;
											premierUN =  0;
											precedLong = matrixPalindrome->tab[newI][newJ];
										}
										else
											newI = newJ = matrixPalindrome->taille;
									}
									// ici le palindrome est de taille est 1
									// donc la loop interne ne doit pas depasser 2
									else
									{
										int start2 = newJ + 1 + debutIntervalle;
										int end2 = matrixPalindrome->taille - newI + debutIntervalle;
										float ft2 = Palindrome1BaseRNA(start2-debutIntervalle, end2-debutIntervalle, matrixPalindrome->sequence);
										
										if(mesapp == 2)
										{
											if(premierUN == 0)
											{
												PalminiList[0].hairpinMesap = PalminiList[0].hairpinMesap + mesapp;
												PalminiList[0].hairpinDG = PalminiList[0].hairpinDG + ft2;
												PalminiList[0].hairpinPal++;
												if(PalminiList[0].hairpinMAXloop < mesapp)
													PalminiList[0].hairpinMAXloop = mesapp;
												precedI = newI;
												premierUN =  1;
												precedLong = matrixPalindrome->tab[newI][newJ];
											}
											else
												newI = newJ = matrixPalindrome->taille;
										}
										if(mesapp == 1)
										{
											PalminiList[0].hairpinMesap = PalminiList[0].hairpinMesap + mesapp;
											PalminiList[0].hairpinDG = PalminiList[0].hairpinDG + ft2;
											PalminiList[0].hairpinPal++;
											if(PalminiList[0].hairpinMAXloop < mesapp)
												PalminiList[0].hairpinMAXloop = mesapp;
											precedI = newI;
											premierUN = 0;
											precedLong = matrixPalindrome->tab[newI][newJ];
										}
										if(mesapp > 2)
											newI = newJ = matrixPalindrome->taille;
									}
									mesapp = 0;
								}
							}
							else
							{
								mesapp++;
							}
							newI++;
							newJ++;
						}
						finHairpin = precedI;

						PalminiList[0].hairpinTaille = finHairpin - debHairpin;
						debHairpin = (i - longueurP) - debHairpin;
						PalminiList[0].hairpinDeb = start - debHairpin;
						PalminiList[0].hairpinFin = end + debHairpin;
						PalminiList[0].hairpinDIST = (PalminiList[0].hairpinFin - PalminiList[0].hairpinDeb) - (2 * PalminiList[0].hairpinTaille);  // erreur !! voir JAVA
						PalminiList[0].hairpinDG = PalminiList[0].hairpinDG /*+ BoucleTerminal(PalminiList[0].hairpinDIST)*/;
						
						

						// je garde aussi le meilleur palindrome parfait
						// meme s'il n'est pas le meilleur imparfait (bon resultat pour quelques cas)
						RecopiePal(PalminiList, 0, PalminiList, select);
					}
				}
			}
		}
	}

}

/*************************************************************************************************************************************************/
int RecherchePalindromeARN(Matrice *matrixPalindrome, int debutFenetre, Parameters *arguments, PalindromeSimple *PalHairpin, 
			   int *graine, int DEB, int FIN, int TAILLE, int limitHairpin, int debfinSeq)
{
	int taille = matrixPalindrome->taille;
	int compteur = 0;
	int longueur = 0;

	
	/* FILLING OF SCORING MATRIX AND SEARCH OF PALINDROMES IN PARALLEL :
	* First step : From bottom to top
	* 
	* 7 * * * * *
	* 5 8 * * * *
	* 3 6 9 * * *
	* 2 4 * * * *
	* 1 * * * * *
	* * * * * * * */

	for(int i=taille-1; i>=0; i--)
	{
		if((taille-1-i)%2 == 0)
			compteur++;
		longueur = 0;
		
			
			for(int j=0; j<compteur; j++)
			{
				char x = matrixPalindrome->sequence[j];
				char y = matrixPalindrome->sequence[taille - i - j - 1];
				
				if(x == 'G')
				{
					if(y == 'C')
					{
						if(j>0)
							matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					if(y == 'U')
					{
						if(j>0)
						{
							char a = matrixPalindrome->sequence[j-1];
							char b = matrixPalindrome->sequence[taille - i+1 - j - 1];
							if( (a == 'U') && (b == 'G') )
							{
								matrixPalindrome->tab[i+j][j] = 0;
								matrixPalindrome->tab[i+j-1][j-1] = 0;
							}
							else
								matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						}
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					if( (y != 'U') && (y != 'C') ) 
						matrixPalindrome->tab[i+j][j] = 0;
				}
				else if(x == 'U')
				{
					if(y == 'A')
					{
						if(j>0)
							matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					if(y == 'G')
					{
						if(j>0)
						{
							char a = matrixPalindrome->sequence[j-1];
							char b = matrixPalindrome->sequence[taille - i+1 - j - 1];
							if( (a == 'G') && (b == 'U') )
							{
								matrixPalindrome->tab[i+j][j] = 0;
								matrixPalindrome->tab[i+j-1][j-1] = 0;
							}
							else
								matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						}
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					if( (y != 'A') && (y != 'G') ) 
						matrixPalindrome->tab[i+j][j] = 0;
				}
				else if(x == 'A')
				{
					if( (y == 'U') )
					{
						if(j>0)
							matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					else
						matrixPalindrome->tab[i+j][j] = 0;
				}
				else if(x == 'C')
				{
					if(y == 'G')
					{
						if(j>0)
							matrixPalindrome->tab[i+j][j] = matrixPalindrome->tab[i+j-1][j-1] + 1;
						else
							matrixPalindrome->tab[i+j][j] = 1;
						longueur++;
					}
					else
						matrixPalindrome->tab[i+j][j] = 0;
				}
				else
					matrixPalindrome->tab[i+j][j] = 0;

				if(matrixPalindrome->tab[i+j][j] == 0)
					longueur = 0;
			}
	}

	/* FILLING OF SCORING MATRIX AND SEARCH OF PALINDROMES IN PARALLEL :
	* Second step : From left to rigth
	* 
	* * 1 3 5 6 *
	* * * 2 4 * *
	* * * * * * *
	* * * * * * *
	* * * * * * *
	* * * * * * *  */

	for(int i=1; i<taille-1; i++)
	{
		if((taille-1-i)%2 != 0)
		{
			compteur--;
			longueur = 0;
		}
		longueur = 0;
		
			for(int j=0; j<=compteur; j++)
			{
				char x = matrixPalindrome->sequence[i+j];
				char y = matrixPalindrome->sequence[taille-j-1];

				if(x == 'G')
				{
					if(y == 'C')
					{
						if( (j>0 && i>0) || j>0)
							matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					if(y == 'U')
					{
						if( (j>0 && i>0) || j>0)
						{
							char a = matrixPalindrome->sequence[i-1+j];
							char b = matrixPalindrome->sequence[taille-j];
							if( (a == 'U') && (b == 'G') )
							{
								matrixPalindrome->tab[j][j+i] = 0;
								matrixPalindrome->tab[j-1][j+i-1] = 0;
							}
							else
								matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						}
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					if( (y != 'U') && (y != 'C') )
						matrixPalindrome->tab[j][j+i] = 0;
				}
				else if(x == 'U')
				{
					if(y == 'A')
					{
						if( (j>0 && i>0) || (j>0))
							matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					if(y == 'G')
					{
						if( (j>0 && i>0) || (j>0))
						{
							char a = matrixPalindrome->sequence[i-1+j];
							char b = matrixPalindrome->sequence[taille-j];
							if( (a == 'G') && (b == 'U') )
							{
								matrixPalindrome->tab[j][j+i] = 0;
								matrixPalindrome->tab[j-1][j+i-1] = 0;
							}
							else
								matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						}
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					if( (y != 'A') && (y != 'G') )
						matrixPalindrome->tab[j][j+i] = 0;
				}
				else if(x == 'A')
				{
					if( (y == 'U') )
					{
						if((j>0 && i>0) || (j>0))
							matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					else
						matrixPalindrome->tab[j][j+i] = 0;
				}
				else if(x == 'C')
				{
					if(y=='G')
					{
						if((j>0 && i>0) || (j>0))
							matrixPalindrome->tab[j][j+i] = matrixPalindrome->tab[j-1][j+i-1] + 1;
						else
							matrixPalindrome->tab[j][j+i] = 1;
						longueur++;
					}
					else
						matrixPalindrome->tab[j][j+i] = 0;
				}
				else
					matrixPalindrome->tab[j][j+i] = 0;

				if(matrixPalindrome->tab[j][j+i] == 0)
					longueur = 0;
			}
	}

	// je remplis la liste de palindrome
	int NbPalindrome = 0;
	for(int j=0; j<taille; j++)
	{
		for(int i=taille-1; i>=0; i--)
		{
			// je suis au debut du palindrome
			if(matrixPalindrome->tab[i][j] == 1)
			{
				int x = i, y = j;
				while(matrixPalindrome->tab[x+1][y+1] != 0)
				{
					x++;
					y++;
				}
				
				if(matrixPalindrome->tab[x][y] == 1)
				{
					int start = y + 1 + debutFenetre;
					int end = matrixPalindrome->taille - x + debutFenetre;
					
					if(limitHairpin == 0)
					{
						float ft = Palindrome1BaseRNA(start-debutFenetre, end-debutFenetre, matrixPalindrome->sequence);
						NouveauPalRNA(PalHairpin, NbPalindrome, start, end, 1, ft);
						NbPalindrome++;
					}
				}
				else
				{
					int longueurP = matrixPalindrome->tab[x][y];
					int start = y + 1 - (longueurP - 1) + debutFenetre;
					int end = matrixPalindrome->taille - x + (longueurP - 1) + debutFenetre;

					if( (DEB  == start) && (FIN  == end) && (TAILLE == longueurP) && (*graine == -1) )
					{
						*graine = NbPalindrome;
					}
					
//					for(int a=0; a<longueurP-1; a++)
					for(int a=0; a<2; a++)
					{
						for(int b=0; b<2; b++)
//						for(int b=0; b<longueurP-1; b++)
						{
							if(limitHairpin == 0)
							{
								float ft = DeltaG_simplePalindromeRNA((start-debutFenetre+b), (end-debutFenetre-b), longueurP-a, matrixPalindrome->sequence);
								NouveauPalRNA(PalHairpin, NbPalindrome, start+b, end-b, longueurP-a, ft);
								NbPalindrome++;
							}
						}
					}
				}
			}
		}
	}

	return NbPalindrome;
}
