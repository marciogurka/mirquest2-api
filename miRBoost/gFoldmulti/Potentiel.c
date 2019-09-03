#include "gFold.h"

/*************************************************************************************************************************************************/
void SelectionnePotentiel(PalindromeSimple *listePal, int indice, Parameters *arguments, float scoreLimit, 
			  int indiceSeed)
{
	int indiceAsso = -1;
	
	// le nouveau palindrome est dans la limite des scores
	if(scoreLimit >= listePal[indice].score)
	{
		float plusMauvais = 1;
		for(int i=0; i<20; i++)
		{
			float scoreAsso = listePal[listePal[indiceSeed].association[i]].score;
			if( (float_compare(plusMauvais, scoreAsso) != 0) && (plusMauvais > scoreAsso) )
			{
				indiceAsso = i;
				plusMauvais = scoreAsso;
			}
		}
		
		if( (float_compare(plusMauvais, listePal[indice].score) != 0) && (plusMauvais > listePal[indice].score) )
		{
			listePal[indiceSeed].association[indiceAsso] = indice;
		}
	}
}

/*************************************************************************************************************************************************/
float ChoisiInterieurDeltaG(PalindromeSimple *PalHairpin, int indiceSeed)
{
	PalHairpin[indiceSeed].indiceSuivant = -1;
	if(PalHairpin[indiceSeed].nbAsso > 0)
	{
		int meilleur = 0;
		float combi = 0.0;
		for(int i=0; i<PalHairpin[indiceSeed].nbAsso; i++)
		{
			float DG = DeltaG_Hairpin_Intern(indiceSeed, PalHairpin[indiceSeed].association[i], PalHairpin);
			DG = DG + ChoisiInterieurDeltaG(PalHairpin, PalHairpin[indiceSeed].association[i]);
			PalHairpin[PalHairpin[indiceSeed].association[i]].choisi = 0;
			
			if( (float_compare(combi, DG) != 0) && (combi > DG) )
			{
				combi = DG;
				meilleur = i;
			}
		}

		PalHairpin[PalHairpin[indiceSeed].association[meilleur]].choisi = 1;
		PalHairpin[indiceSeed].indiceSuivant = PalHairpin[indiceSeed].association[meilleur];
		return combi + PalHairpin[indiceSeed].score;
	}
	else
		return DeltaG_Hairpin_Intern(indiceSeed, -1, PalHairpin);
}

/*************************************************************************************************************************************************/
int AppariementInterne(PalindromeSimple *PalHairpin, int graine)
{
	int appariement = PalHairpin[graine].longueur;
	PalHairpin[graine].hairpinORnot = 1;
	int z = graine;
	while(PalHairpin[z].indiceSuivant != -1)
	{
		PalHairpin[PalHairpin[z].indiceSuivant].hairpinORnot = 1;
		appariement = appariement + PalHairpin[PalHairpin[z].indiceSuivant].longueur;
		z = PalHairpin[z].indiceSuivant;
	}
	
	return appariement;
}

/*************************************************************************************************************************************************/
int MesappariementInterne(PalindromeSimple *PalHairpin, int graine)
{
	int mesappariement = 0;
	int z = graine;
	while(PalHairpin[z].indiceSuivant != -1)
	{
		int ExterieurDeb = PalHairpin[z].vraiDeb  + PalHairpin[z].longueur;
		int ExterieurFin = PalHairpin[z].vraiFin  - PalHairpin[z].longueur;
		int interieurDeb = PalHairpin[PalHairpin[z].indiceSuivant].vraiDeb;
		int interieurFin = PalHairpin[PalHairpin[z].indiceSuivant].vraiFin;
		
		int distance5 = interieurDeb - ExterieurDeb;
		int distance3 = ExterieurFin - interieurFin;
		
		if(distance5 > distance3)
			mesappariement = mesappariement + distance5;
		else
			mesappariement = mesappariement + distance3;
		
		z = PalHairpin[z].indiceSuivant;
	}

	return mesappariement;
}

/*************************************************************************************************************************************************/
float ChoisiExterieurDeltaG(PalindromeSimple *PalHairpin, int indiceSeed)
{
	if(PalHairpin[indiceSeed].nbAsso > 0)
	{
		int meilleur = 0;
		float combi = 0.0;
		for(int i=0; i<PalHairpin[indiceSeed].nbAsso; i++)
		{
			float DG = DeltaG_Hairpin_Extern(PalHairpin[indiceSeed].association[i], indiceSeed, PalHairpin);
			DG = DG + ChoisiExterieurDeltaG(PalHairpin, PalHairpin[indiceSeed].association[i]);
			PalHairpin[PalHairpin[indiceSeed].association[i]].choisi = 0;
			
			if( (float_compare(combi, DG) != 0) && (combi > DG) )
			{
				combi = DG;
				meilleur = i;
			}
		}
		
		PalHairpin[PalHairpin[indiceSeed].association[meilleur]].choisi = 1;
		return combi + PalHairpin[indiceSeed].score;
	}
	else
		return DeltaG_Hairpin_Extern(-1, indiceSeed, PalHairpin);
}

/*************************************************************************************************************************************************/
void TracePremierPal(PalindromeSimple *PalHairpin, int indiceSeed)
{
	PalHairpin[indiceSeed].indicePrecedent = -1;
	
	if(PalHairpin[indiceSeed].nbAsso > 0)
	{
		for(int i=0; i<PalHairpin[indiceSeed].nbAsso; i++)
		{
			if(PalHairpin[PalHairpin[indiceSeed].association[i]].choisi == 1)
			{
				TracePremierPal(PalHairpin, PalHairpin[indiceSeed].association[i]);
				PalHairpin[indiceSeed].indicePrecedent = PalHairpin[indiceSeed].association[i];
			}
		}
	}
	PalHairpin[indiceSeed].hairpinORnot = 1;
}

/*************************************************************************************************************************************************/
int PremierPalindromeRNA(PalindromeSimple *PalHairpin, int graine)
{
	int z = PalHairpin[graine].indicePrecedent;
	int y = graine;
	while(z != -1)
	{
		PalHairpin[z].indiceSuivant = y;
		y = z;
		z = PalHairpin[z].indicePrecedent;
	}

	return y;
}
