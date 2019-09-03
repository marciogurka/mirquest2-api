#include "gFold.h"

/*************************************************************************************************************************************************/
void initHairpin(Hairpin *resultatH, Parameters *arguments, int i)
{
	resultatH[i].SeedDEB = -1;
	resultatH[i].SeedFIN = -1;
	resultatH[i].SeedLG = -1;
	resultatH[i].SeedDist = -1;
	resultatH[i].Seedscore = 0.0;
	
	resultatH[i].neDEB = -1;
	resultatH[i].neFIN = -1;
	resultatH[i].neTAILLE = -1;
	resultatH[i].neDIST = -1;
	resultatH[i].neMesap = -1;
	resultatH[i].neNBPal = -1;
	resultatH[i].neDG = 0.0;
	resultatH[i].neMAXloop = 0;
	
	resultatH[i].suiteBulge = 0;
	resultatH[i].suiteBcote = 0;
	
	resultatH[i].appariementH = 0.0;
	resultatH[i].GUappariement = 0.0;
	resultatH[i].MFEajusted = 0.0;
	resultatH[i].differenceGaucheDroite = 0;
	resultatH[i].bulgeDiff = 0;
	resultatH[i].bulgeMax = 0;
	resultatH[i].boucle = 0;
	resultatH[i].nbPal = 0;
	resultatH[i].PalMoy = 0.0;
	resultatH[i].positionDEB = -1;
	resultatH[i].positionFIN = -1;
	resultatH[i].deltaG = 0.0;
	resultatH[i].GCappariement = 0.0;
	resultatH[i].appariement = 0;
	resultatH[i].pairGU = 0;
	resultatH[i].pairGC = 0;
	resultatH[i].pairAU = 0;
	resultatH[i].hairpinMAXloop = 0;
	resultatH[i].nbBG = 0;
	resultatH[i].nbBD = 0;
	for(int j=0; j<arguments->tailleMax; j++)
	{
		resultatH[i].structure[j] = '\0';
		resultatH[i].texts[j] = '\0';
	}
	
	for(int j=0; j<50; j++)
	{
		resultatH[i].strGAUCHE[j] = 0;
		resultatH[i].strDROITE[j] = 0;
		resultatH[i].strTAILLE[j] = 0;
	}
	
	resultatH[i].afficher = 0;
	resultatH[i].ratioH = 0.0;
	resultatH[i].ratioP = 0.0;
	resultatH[i].ratioNE = 0.0;
}

/*************************************************************************************************************************************************/
void copytHairpin(Hairpin *resultatH, Parameters *arguments, int i, int j)
{
	resultatH[i].SeedDEB   = resultatH[j].SeedDEB;
	resultatH[i].SeedFIN   = resultatH[j].SeedFIN;
	resultatH[i].SeedLG    = resultatH[j].SeedLG;
	resultatH[i].SeedDist  = resultatH[j].SeedDist;
	resultatH[i].Seedscore = resultatH[j].Seedscore;
	
	resultatH[i].neDEB    = resultatH[j].neDEB;
	resultatH[i].neFIN    = resultatH[j].neFIN;
	resultatH[i].neTAILLE = resultatH[j].neTAILLE;
	resultatH[i].neDIST   = resultatH[j].neDIST;
	resultatH[i].neMesap  = resultatH[j].neMesap;
	resultatH[i].neNBPal  = resultatH[j].neNBPal;
	resultatH[i].neDG     = resultatH[j].neDG;
	resultatH[i].neMAXloop = resultatH[j].neMAXloop;
	
	resultatH[i].afficher = resultatH[j].afficher;
	resultatH[i].ratioH   = resultatH[j].ratioH;
	resultatH[i].ratioP   = resultatH[j].ratioP;
	resultatH[i].ratioNE  = resultatH[j].ratioNE;
	
	resultatH[i].suiteBulge = resultatH[j].suiteBulge;
	resultatH[i].suiteBcote = resultatH[j].suiteBcote;
	
	resultatH[i].appariementH   = resultatH[j].appariementH;
	resultatH[i].GUappariement  = resultatH[j].GUappariement;
	resultatH[i].MFEajusted     = resultatH[j].MFEajusted;
	resultatH[i].differenceGaucheDroite = resultatH[j].differenceGaucheDroite;
	resultatH[i].bulgeDiff      = resultatH[j].bulgeDiff;
	resultatH[i].bulgeMax       = resultatH[j].bulgeMax;
	resultatH[i].boucle         = resultatH[j].boucle;
	resultatH[i].nbPal          = resultatH[j].nbPal;
	resultatH[i].PalMoy         = resultatH[j].PalMoy;
	resultatH[i].positionDEB    = resultatH[j].positionDEB;
	resultatH[i].positionFIN    = resultatH[j].positionFIN;
	resultatH[i].deltaG         = resultatH[j].deltaG;
	resultatH[i].GCappariement  = resultatH[j].GCappariement;
	resultatH[i].appariement    = resultatH[j].appariement;
	resultatH[i].pairGU         = resultatH[j].pairGU;
	resultatH[i].pairGC         = resultatH[j].pairGC;
	resultatH[i].pairAU         = resultatH[j].pairAU;
	resultatH[i].hairpinMAXloop = resultatH[j].hairpinMAXloop;
	resultatH[i].nbBG           = resultatH[j].nbBG;
	resultatH[i].nbBD           = resultatH[j].nbBD;
	
	for(int k=0; k<arguments->tailleMax; k++)
	{
		resultatH[i].structure[k] = resultatH[j].structure[k];
		resultatH[i].texts[k]     = resultatH[j].texts[k];
	}
	
	for(int k=0; k<50; k++)
	{
		resultatH[i].strGAUCHE[k] = resultatH[j].strGAUCHE[k];
		resultatH[i].strDROITE[k] = resultatH[j].strDROITE[k];
		resultatH[i].strTAILLE[k] = resultatH[j].strTAILLE[k];
	}
}

/*************************************************************************************************************************************************/
void CreeHairpin(PalindromeSimple *PalHairpin, int NbPalindrome, int graine, int premier, Hairpin *resultatH,
		 Parameters *arguments, int appariNonExact, Palindrome *listePalT, int indicePal, int q)
{
	initHairpin(resultatH, arguments, arguments->nbTige);

	resultatH[arguments->nbTige].SeedDEB   = listePalT[indicePal].vraiDeb;
	resultatH[arguments->nbTige].SeedFIN   = listePalT[indicePal].vraiFin;
	resultatH[arguments->nbTige].SeedLG    = listePalT[indicePal].longueur;
	resultatH[arguments->nbTige].SeedDist  = listePalT[indicePal].distance;
	resultatH[arguments->nbTige].Seedscore = listePalT[indicePal].score;
	
	resultatH[arguments->nbTige].neDEB     = listePalT[indicePal].hairpinDeb;
	resultatH[arguments->nbTige].neFIN     = listePalT[indicePal].hairpinFin;
	resultatH[arguments->nbTige].neTAILLE  = listePalT[indicePal].hairpinTaille;
	resultatH[arguments->nbTige].neDIST    = listePalT[indicePal].hairpinDIST;
	resultatH[arguments->nbTige].neMesap   = listePalT[indicePal].hairpinMesap;
	resultatH[arguments->nbTige].neNBPal   = listePalT[indicePal].hairpinPal;
	resultatH[arguments->nbTige].neDG      = listePalT[indicePal].hairpinDG;
	resultatH[arguments->nbTige].neMAXloop = listePalT[indicePal].hairpinMAXloop;
	
	resultatH[arguments->nbTige].ratioP = listePalT[indicePal].ratioP;
	resultatH[arguments->nbTige].ratioNE = listePalT[indicePal].ratioNE;
	

	resultatH[arguments->nbTige].afficher = 0;
	resultatH[arguments->nbTige].positionDEB = PalHairpin[premier].vraiDeb;
	resultatH[arguments->nbTige].positionFIN = PalHairpin[premier].vraiFin;
	
	for(int i=0; i<(resultatH[arguments->nbTige].positionFIN - resultatH[arguments->nbTige].positionDEB); i++)
		resultatH[arguments->nbTige].structure[i] = '.';
	
	if(arguments->optionText == 1)
	{
		for(int i=0; i<(resultatH[arguments->nbTige].positionFIN - resultatH[arguments->nbTige].positionDEB); i++)
			resultatH[arguments->nbTige].texts[i] = '.';
	}

	int appariment = 0;
	int commencement = PalHairpin[premier].vraiDeb;
	int addPal = 0;
	int z = premier;
	resultatH[arguments->nbTige].appMFE = 0.0;
	int tempSuite = 0;
	int tempCote5 = 0;
	int tempCote3 = 0;
	while(z != -1)
	{
		resultatH[arguments->nbTige].deltaG = resultatH[arguments->nbTige].deltaG + ThermoHairpin(PalHairpin, z, PalHairpin[z].indiceSuivant);
		appariment = appariment + 2 * PalHairpin[z].longueur;
		resultatH[arguments->nbTige].appMFE = resultatH[arguments->nbTige].appMFE +  PalHairpin[z].score;

		for(int i=0; i<PalHairpin[z].longueur; i++)
		{
			resultatH[arguments->nbTige].structure[PalHairpin[z].vraiDeb - commencement + i] = '(';
			resultatH[arguments->nbTige].structure[PalHairpin[z].vraiFin - commencement - i] = ')';

			if( (arguments->text[q][PalHairpin[z].vraiDeb + i - 1] ==  'G') 
			 && (arguments->text[q][PalHairpin[z].vraiFin - i - 1] ==  'U') )
				resultatH[arguments->nbTige].pairGU++;
			if( (arguments->text[q][PalHairpin[z].vraiDeb + i - 1] ==  'U') 
			 && (arguments->text[q][PalHairpin[z].vraiFin - i - 1] ==  'G') )
				resultatH[arguments->nbTige].pairGU++;
				
			if( (arguments->text[q][PalHairpin[z].vraiDeb + i - 1] ==  'G') 
			 && (arguments->text[q][PalHairpin[z].vraiFin - i - 1] ==  'C') )
				resultatH[arguments->nbTige].pairGC++;
			if( (arguments->text[q][PalHairpin[z].vraiDeb + i - 1] ==  'C') 
			 && (arguments->text[q][PalHairpin[z].vraiFin - i - 1] ==  'G') )
				resultatH[arguments->nbTige].pairGC++;
				
			if( (arguments->text[q][PalHairpin[z].vraiDeb + i - 1] ==  'A') 
			 && (arguments->text[q][PalHairpin[z].vraiFin - i - 1] ==  'U') )
				resultatH[arguments->nbTige].pairAU++;
			if( (arguments->text[q][PalHairpin[z].vraiDeb + i - 1] ==  'U') 
			 && (arguments->text[q][PalHairpin[z].vraiFin - i - 1] ==  'A') )
				resultatH[arguments->nbTige].pairAU++;
				
				
		}
		resultatH[arguments->nbTige].structure[(resultatH[arguments->nbTige].positionFIN - resultatH[arguments->nbTige].positionDEB)+1] = '\0';
		addPal = addPal + PalHairpin[z].longueur;
		
		resultatH[arguments->nbTige].strGAUCHE[resultatH[arguments->nbTige].nbPal] = PalHairpin[z].vraiDeb;
		resultatH[arguments->nbTige].strDROITE[resultatH[arguments->nbTige].nbPal] = PalHairpin[z].vraiFin;
		resultatH[arguments->nbTige].strTAILLE[resultatH[arguments->nbTige].nbPal] = PalHairpin[z].longueur;
		
		
		if(PalHairpin[z].indiceSuivant != -1)
		{
			int prime5 = PalHairpin[PalHairpin[z].indiceSuivant].vraiDeb - (PalHairpin[z].vraiDeb + PalHairpin[z].longueur);
			int prime3 = (PalHairpin[z].vraiFin - PalHairpin[z].longueur) - PalHairpin[PalHairpin[z].indiceSuivant].vraiFin;
			if(prime3 != prime5)
			{
				if(prime5 > prime3)
				{
					tempCote5++;
					tempCote3 = 0;
					if(resultatH[arguments->nbTige].suiteBcote < tempCote5)
						resultatH[arguments->nbTige].suiteBcote = tempCote5;
				}
				else
				{
					tempCote3++;
					tempCote5 = 0;
					if(resultatH[arguments->nbTige].suiteBcote < tempCote3)
						resultatH[arguments->nbTige].suiteBcote = tempCote3;
				}
				tempSuite++;
				
				if(resultatH[arguments->nbTige].suiteBulge < tempSuite)
					resultatH[arguments->nbTige].suiteBulge = tempSuite;
				
				if(prime5 > prime3)
					resultatH[arguments->nbTige].nbBG++;
				else
					resultatH[arguments->nbTige].nbBD++;
				
				int diff = abs(prime3 - prime5);
				if(resultatH[arguments->nbTige].bulgeDiff < diff)
					resultatH[arguments->nbTige].bulgeDiff = diff;
				
				int plusgrandbulge = prime3;
				if(prime3 < prime5)
					plusgrandbulge = prime5;
				if(resultatH[arguments->nbTige].bulgeMax < plusgrandbulge)
					resultatH[arguments->nbTige].bulgeMax = plusgrandbulge;
			}
			else
			{
				tempSuite = 0;
				tempCote5 = 0;
				tempCote3 = 0;
				
				if(resultatH[arguments->nbTige].hairpinMAXloop < prime5)
					resultatH[arguments->nbTige].hairpinMAXloop = prime5;
			}
		}
		
		resultatH[arguments->nbTige].boucle = PalHairpin[z].distance;
		resultatH[arguments->nbTige].nbPal++;
		
		z = PalHairpin[z].indiceSuivant;
	}

	for(int i=0; i<=(resultatH[arguments->nbTige].positionFIN - resultatH[arguments->nbTige].positionDEB); i++)
		resultatH[arguments->nbTige].texts[i] = arguments->text[q][resultatH[arguments->nbTige].positionDEB + i - 1];
	resultatH[arguments->nbTige].texts[(resultatH[arguments->nbTige].positionFIN - resultatH[arguments->nbTige].positionDEB)+1] = '\0';

	resultatH[arguments->nbTige].PalMoy = ((float)addPal / (float)resultatH[arguments->nbTige].nbPal);
	resultatH[arguments->nbTige].appariement = appariment;
	resultatH[arguments->nbTige].appariementH = ((float)resultatH[arguments->nbTige].appariement / ((float)(resultatH[arguments->nbTige].positionFIN - resultatH[arguments->nbTige].positionDEB))) * 100.0;
	resultatH[arguments->nbTige].GUappariement = ((float)resultatH[arguments->nbTige].pairGU / (float)resultatH[arguments->nbTige].appariement) * 100.0;
	resultatH[arguments->nbTige].GCappariement = ((float)resultatH[arguments->nbTige].pairGC / (float)resultatH[arguments->nbTige].appariement) * 100.0;
	resultatH[arguments->nbTige].AUappariement = ((float)resultatH[arguments->nbTige].pairAU / (float)resultatH[arguments->nbTige].appariement) * 100.0;
	resultatH[arguments->nbTige].MFEajusted = (resultatH[arguments->nbTige].deltaG / (float)(resultatH[arguments->nbTige].positionFIN - resultatH[arguments->nbTige].positionDEB)) * 100.0;
	resultatH[arguments->nbTige].differenceGaucheDroite = abs(resultatH[arguments->nbTige].nbBD - resultatH[arguments->nbTige].nbBG);

	
	

	// parametres redibitoires
	
	if(resultatH[arguments->nbTige].positionDEB != -1)
		arguments->nbTige++;
}

/*************************************************************************************************************************************************/
void RemoveTigeDouble(Hairpin *resultatH, Parameters *arguments, int debut4fenetre)
{
	// elimine les hairpins trop chevauchantes ou incluses si elles ont un taux inferieure bon
	for(int k=0; k<arguments->nbTige; k++)
	{
		if(resultatH[k].positionDEB != -1)
		{
			for(int i=0; i<arguments->nbTige; i++)
			{
				if( (i != k) && (resultatH[i].positionDEB != -1) )
				{
					// ici i est egale a k et i un plus mauvais ratio
					if( (resultatH[k].positionDEB == resultatH[i].positionDEB) && (resultatH[k].positionFIN == resultatH[i].positionFIN)
					 && (float_compare(resultatH[k].deltaG, resultatH[i].deltaG) != 0) && (resultatH[k].deltaG < resultatH[i].deltaG) )
					{
						initHairpin(resultatH, arguments, i);
					}

					// si une hairpin est inclus ou englobe on garde le meilleur MFE ajuste
					if( (float_compare(resultatH[k].MFEajusted, resultatH[i].MFEajusted) != 0) && (resultatH[k].MFEajusted < resultatH[i].MFEajusted) )
					{
						if((resultatH[k].positionDEB <= resultatH[i].positionDEB) && (resultatH[k].positionFIN >= resultatH[i].positionFIN))
						{
							initHairpin(resultatH, arguments, i);
						}
						
						if((resultatH[k].positionDEB > resultatH[i].positionDEB) && (resultatH[k].positionFIN < resultatH[i].positionFIN))
						{
							initHairpin(resultatH, arguments, i);
						}
					}
				}
			}
		}
	}

	for(int k=0; k<arguments->nbTige; k++)
	{
		if(resultatH[k].positionDEB != -1)
		{
			//float limiteMFE = resultatH[k].MFEajusted * 0.9;
			float limiteMFE = resultatH[k].MFEajusted * 0.9 * (resultatH[k].ratioH + resultatH[k].ratioNE + resultatH[k].ratioP);
			
			for(int i=0; i<arguments->nbTige; i++)
			{
				if( (i != k) && (resultatH[i].positionDEB != -1) )
				{
					// je vais eliminer en fonction d'une limite
					//if( (float_compare(resultatH[i].MFEajusted, limiteMFE) != 0) && (resultatH[i].MFEajusted > limiteMFE) )
					if( (float_compare((resultatH[i].MFEajusted * (resultatH[i].ratioH + resultatH[i].ratioNE + resultatH[i].ratioP)), limiteMFE) != 0) && ((resultatH[i].MFEajusted * (resultatH[i].ratioH + resultatH[i].ratioNE + resultatH[i].ratioP)) > limiteMFE) )
					{
						// debut chevauchant
						// le chevauchement est superieur a la moitie de la taille des deux hairpins
						if( (resultatH[k].positionDEB < resultatH[i].positionDEB) && (resultatH[i].positionDEB < resultatH[k].positionFIN) 
						&& ((resultatH[k].positionFIN-resultatH[i].positionDEB) >= (resultatH[k].positionFIN-resultatH[k].positionDEB)/2) 
						&& ((resultatH[k].positionFIN-resultatH[i].positionDEB) >= (resultatH[i].positionFIN-resultatH[i].positionDEB)/2) )
						{
							initHairpin(resultatH, arguments, i);
						}
					
						if( (resultatH[k].positionDEB < resultatH[i].positionFIN) && (resultatH[i].positionFIN < resultatH[k].positionFIN) 
						&& ((resultatH[i].positionFIN-resultatH[k].positionDEB) >= (resultatH[k].positionFIN-resultatH[k].positionDEB)/2) 
						&& ((resultatH[i].positionFIN-resultatH[k].positionDEB) >= (resultatH[i].positionFIN-resultatH[i].positionDEB)/2) )
						{
							if(resultatH[k].deltaG < resultatH[i].deltaG)
								initHairpin(resultatH, arguments, i);
						}

					}
				}
			}
		}
	}

	// elimine les hairpins inutiles
	for(int k=0; k<arguments->nbTige; k++)
	{
		if(resultatH[k].positionDEB == -1)
		{
			for(int z=k; z<arguments->nbTige; z++)
			{
				copytHairpin(resultatH, arguments, z, z+1);
			}
			arguments->nbTige--;
			k--;
		}
	}
}
