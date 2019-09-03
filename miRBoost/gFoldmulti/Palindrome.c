#include "gFold.h"

/*************************************************************************************************************************************************/
void InitialisePalindrome(Palindrome *listePal, int tailleTab)
{
	for(int i=0; i<tailleTab; i++)
		Init1Palindrome(listePal, i);
}

/*************************************************************************************************************************************************/
void Init1Palindrome(Palindrome *listePal, int i)
{
	listePal[i].vraiDeb = -1;
	listePal[i].vraiFin = -1;
	listePal[i].longueur = 0;
	listePal[i].distance = 0;
	listePal[i].score = 0.0;
	listePal[i].choisi = 0;
	listePal[i].posInter = 0;
	listePal[i].ratioA = 0.0;
	listePal[i].ratioC = 0.0;
	listePal[i].ratioG = 0.0;
	listePal[i].ratioU = 0.0;
	
	listePal[i].hairpinDeb = -1;
	listePal[i].hairpinFin = -1;
	listePal[i].hairpinTaille = 0;
	listePal[i].hairpinDIST = 0;
	listePal[i].hairpinDG = 0.0;
	listePal[i].hairpinMesap = 0;
	listePal[i].hairpinMAXloop = 0;
	listePal[i].hairpinPal = 1;
	listePal[i].MoyennePal = 0.0;
	listePal[i].ratioMatchGU = 0.0;
	listePal[i].basePairing = 0.0;
}

/*************************************************************************************************************************************************/
void NouveauPal(Palindrome *listePal, int i, int debut, int fin, int longueur, float score)
{
	listePal[i].vraiDeb = debut;
	listePal[i].vraiFin = fin;
	listePal[i].longueur = longueur;
	listePal[i].distance = (fin - debut) - (2 * longueur) + 1;
	listePal[i].score = score;
	listePal[i].choisi = 0;
	listePal[i].posInter = ((fin + debut) / 2);
	listePal[i].ratioA = 0.0;
	listePal[i].ratioC = 0.0;
	listePal[i].ratioG = 0.0;
	listePal[i].ratioU = 0.0;
	
	listePal[i].hairpinDeb = 0;
	listePal[i].hairpinFin = 0;
	listePal[i].hairpinTaille = 0;
	listePal[i].hairpinDIST = 0;
	listePal[i].hairpinDG = 0.0;
	listePal[i].hairpinMesap = 0;
	listePal[i].hairpinMAXloop = 0;
	listePal[i].hairpinPal = 1;
	listePal[i].MoyennePal = 0.0;
	listePal[i].ratioMatchGU = 0.0;
	listePal[i].basePairing = 0.0;
}

/*************************************************************************************************************************************************/
void RecopiePal(Palindrome *Pal, int i, Palindrome *Pal2, int j)
{
	Pal2[j].vraiDeb = Pal[i].vraiDeb;
	Pal2[j].vraiFin = Pal[i].vraiFin;
	Pal2[j].longueur = Pal[i].longueur;
	Pal2[j].distance = Pal[i].distance;
	Pal2[j].score = Pal[i].score;
	Pal2[j].choisi = Pal[i].choisi;
	Pal2[j].posInter = Pal[i].posInter;
	Pal2[j].ratioA = Pal[i].ratioA;
	Pal2[j].ratioC = Pal[i].ratioC;
	Pal2[j].ratioG = Pal[i].ratioG;
	Pal2[j].ratioU = Pal[i].ratioU;
	
	Pal2[j].hairpinDeb = Pal[i].hairpinDeb;
	Pal2[j].hairpinFin = Pal[i].hairpinFin;
	Pal2[j].hairpinTaille= Pal[i].hairpinTaille;
	Pal2[j].hairpinDIST = Pal[i].hairpinDIST;
	Pal2[j].hairpinDG = Pal[i].hairpinDG;
	Pal2[j].hairpinMesap = Pal[i].hairpinMesap;
	Pal2[j].hairpinMAXloop = Pal[i].hairpinMAXloop;
	Pal2[j].hairpinPal = Pal[i].hairpinPal;
	Pal2[j].MoyennePal = Pal[i].MoyennePal;
	Pal2[j].ratioMatchGU = Pal[i].ratioMatchGU;
	Pal2[j].basePairing = Pal[i].basePairing;
}

/*************************************************************************************************************************************************/
int PalIndentique(Palindrome *Pal, int i, Palindrome *Pal2, int j)
{
	int erreur = 0;

	if(Pal2[j].hairpinDeb != Pal[i].hairpinDeb)
		erreur++;
	if(Pal2[j].hairpinFin != Pal[i].hairpinFin)
		erreur++;
	if(Pal2[j].hairpinTaille != Pal[i].hairpinTaille)
		erreur++;
	if(Pal2[j].hairpinDIST != Pal[i].hairpinDIST)
		erreur++;
	if(Pal2[j].hairpinDG != Pal[i].hairpinDG)
		erreur++;
	
	return erreur;
}

/*************************************************************************************************************************************************/
void NouveauPalRNA(PalindromeSimple *listePal, int indice, int debut, int fin, int longueur, float score)
{
	listePal[indice].vraiDeb = debut;
	listePal[indice].vraiFin = fin;
	listePal[indice].longueur = longueur;
	listePal[indice].distance = (fin - debut) - (2 * longueur) + 1;
	listePal[indice].score = score;
	listePal[indice].indiceSuivant = -1;
	listePal[indice].indicePrecedent = -1;
	
	listePal[indice].choisi = 0;
	listePal[indice].mesappariement = 0;
	listePal[indice].appariement = 0;
	listePal[indice].nbAsso = 0;
	for(int i=0; i<20; i++)
		listePal[indice].association[i] = -1;
}

/*************************************************************************************************************************************************/
void EffacePalindromeRNA(PalindromeSimple *listePal, int indice)
{
	listePal[indice].vraiDeb = 0;
	listePal[indice].vraiFin = 0;
	listePal[indice].longueur = 0;
	listePal[indice].distance = 0;
	listePal[indice].score = 0.0;
	listePal[indice].indiceSuivant = -1;
	listePal[indice].indicePrecedent = -1;
	listePal[indice].hairpinORnot = 0;
}

/*************************************************************************************************************************************************/
int ElimineInutile(Palindrome *listePalT, int nbPalindromeT, Palindrome *PalTemp)
{
	// elimine tous les palindromes inutiles
	for(int i=0; i<nbPalindromeT; i++)
	{
		if(listePalT[i].choisi != 1)
		{
			for(int j=i+1; j<nbPalindromeT; j++)
			{
				if(listePalT[j].choisi == 1)
				{
					RecopiePal(listePalT, i, PalTemp, 0);
					RecopiePal(listePalT, j, listePalT, i);
					RecopiePal(PalTemp, 0, listePalT, j);
					j = nbPalindromeT;
				}
			}
		}
	}
	
	int compt = 0;
	for(int i=0; i<nbPalindromeT; i++)
	{
		if(listePalT[i].choisi == 1)
			compt++;
	}
	
	return compt;
}

/*************************************************************************************************************************************************/
void OrdonneListeHairpin(Palindrome *listePal, int nbPalindrome, Palindrome *Pal)
{
	for(int i=0; i<nbPalindrome; i++)
	{
		int meilleurPos = listePal[i].hairpinDeb;
		int meilleurIndice = i;
		for(int j=i; j<nbPalindrome; j++)
		{
			if(meilleurPos > listePal[j].hairpinDeb)
			{
				meilleurPos = listePal[j].hairpinDeb;
				meilleurIndice = j;
			}
			else if( (meilleurPos == listePal[j].hairpinDeb) 
				&& (listePal[meilleurIndice].hairpinFin > listePal[j].hairpinFin) )
			{
				meilleurPos = listePal[j].hairpinDeb;
				meilleurIndice = j;
			}
		}
		
		// le meilleur palindrome n'est pas i mais meilleurIndice
		if(meilleurIndice != i)
		{
			// je copie le palindrome i en Pal[0] et le palindrome meilleurIndice en i
			// je recopie le Pal[0] en meilleurIndice 
			RecopiePal(listePal, i, Pal, 0);
			RecopiePal(listePal, meilleurIndice, listePal, i);
			RecopiePal(Pal, 0, listePal, meilleurIndice);
		}
	}
}

/*************************************************************************************************************************************************/
int RemoveDoublons(Palindrome *listePal, int nbPalindrome, Palindrome *Pal)
{
	int compt = 0;
//	compt = nbPalindrome;
	
	// j'elimine les hairpins qui sont en doubles
	for(int i=0; i<nbPalindrome; i++)
	{
		if(listePal[i].choisi != -1)
		{
			for(int j=i+1; j<nbPalindrome; j++)
			{
				if( (listePal[i].vraiDeb  == listePal[j].vraiDeb) 
				 && (listePal[i].vraiFin  == listePal[j].vraiFin)
				 && (listePal[i].longueur == listePal[j].longueur) )
				{
					listePal[j].choisi = -1;
				}
			}
		}
	}

	// j'elimine les hairpins qui sont en doubles
	for(int i=0; i<nbPalindrome; i++)
	{
		if(listePal[i].choisi != -1)
		{
			// elimine les tiges identiques
			for(int j=i+1; j<nbPalindrome; j++)
			{
				if( (listePal[j].choisi == 1) 
					&& (listePal[i].hairpinDeb == listePal[j].hairpinDeb) 
					&& (listePal[i].hairpinFin == listePal[j].hairpinFin) )
				{
					if(listePal[i].hairpinTaille == listePal[j].hairpinTaille)
					{
						if( (float_compare(listePal[i].score, listePal[j].score) != 0) 
							&& (listePal[i].score > listePal[j].score) )
						{
							listePal[i].choisi = -1;
							j = nbPalindrome;
						}
						else
						{
							listePal[j].choisi = -1;
						}
					}
					if(listePal[i].hairpinTaille > listePal[j].hairpinTaille)
						listePal[j].choisi = -1;
				}
				
				if( (listePal[j].choisi == 1) && (listePal[i].choisi == 1)
					&& (listePal[i].hairpinDeb < listePal[j].hairpinDeb) && (listePal[i].hairpinFin > listePal[j].hairpinFin) 
					&& ((listePal[j].hairpinDeb - listePal[i].hairpinDeb) == (listePal[i].hairpinFin - listePal[j].hairpinFin)) )
				{
					if(listePal[i].hairpinTaille >= listePal[j].hairpinTaille)
					{
						listePal[j].choisi = -1;
					}
					else
					{
						listePal[i].choisi = -1;
						j = nbPalindrome;
					}
				}
			}
		}
	}


	// je reclasse les hairpins
	for(int i=0; i<nbPalindrome; i++)
	{
		if(listePal[i].choisi == -1)
		{
			for(int j=i; j<nbPalindrome; j++)
			{
				RecopiePal(listePal, j+1, listePal, j);
			}
			i--;
		}
	}
	for(int i=0; i<nbPalindrome; i++)
	{
		if(listePal[i].choisi != -1)
			compt++;
		if(listePal[i].vraiDeb == 0)
		{
			compt--;
			i = nbPalindrome;
		}
	}

	return compt;
}
