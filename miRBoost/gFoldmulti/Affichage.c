#include "gFold.h"

/*************************************************************************************************************************************************/
void PrintMatrix(Matrice *M)
{
	printf("  ");
	for(int i=0; i<M->taille; i++)
		printf("%c ", M->sequence[i]);
	
	for(int i=0; i<M->taille; i++)
	{
		printf("\n");
		printf("%c ", M->sequence[M->taille-i-1]);

		for(int j=0; j<M->taille-i; j++)
		{
			printf("%d ", M->tab[i][j]);
		}
	}
	printf("\n\n");
}

/*************************************************************************************************************************************************/
void AfficheToutesValeurs(Hairpin *resultatH, Parameters *arguments, int i, char *fichier, int q)
{

		if(arguments->optionAffichage != 2)
		{
			//cout << arguments->nameSeq[q] << "\t";
			cout << arguments->optionAffichage << "\t";
		
		
			float ratioAA = 0.0, ratioAC = 0.0, ratioAG = 0.0, ratioAU = 0.0;
			float ratioCA = 0.0, ratioCC = 0.0, ratioCG = 0.0, ratioCU = 0.0;
			float ratioGA = 0.0, ratioGC = 0.0, ratioGG = 0.0, ratioGU = 0.0;
			float ratioUA = 0.0, ratioUC = 0.0, ratioUG = 0.0, ratioUU = 0.0;
	
			int graineA = 0;
			int graineC = 0;
			int graineG = 0;
			int graineU = 0;
			int MaxA = 0;
			int MaxC = 0;
			int MaxG = 0;
			int MaxU = 0;
			int tempA = 0;
			int tempC = 0;
			int tempG = 0;
			int tempU = 0;
			int GUgraine = 0;
			int GCgraine = 0;
			float ratioA = 0.0;
			float ratioC = 0.0;
			float ratioG = 0.0;
			float ratioU = 0.0;
			float ratioMatchGU = 0.0;
			float ratioMatchGC = 0.0;
			float MFEindex = 0.0;
			
			int tigeA = 0;
			int tigeC = 0;
			int tigeG = 0;
			int tigeU = 0;
		
			int tempSuiteA = 0;
			int tempSuiteC = 0;
			int tempSuiteG = 0;
			int tempSuiteT = 0;
			int MaxSuiteA = 0;
			int MaxSuiteC = 0;
			int MaxSuiteG = 0;
			int MaxSuiteT = 0;
		
			int matchGU = 0;
			int matchGC = 0;
			int tempBigPal = 0;
			int BigPal = 0;
			float ratioTigeA = 0.0;
			float ratioTigeC = 0.0;
			float ratioTigeG = 0.0;
			float ratioTigeU = 0.0;
			
			int nontigeA = 0;
			int nontigeC = 0;
			int nontigeG = 0;
			int nontigeU = 0;
			float NONratioTigeA = 0.0;
			float NONratioTigeC = 0.0;
			float NONratioTigeG = 0.0;
			float NONratioTigeU = 0.0;
			float APPratioA = 0.0;
			float APPratioC = 0.0;
			float APPratioG = 0.0;
			float APPratioU = 0.0;

			int Appariement = 0;
			float appF = 0.0;
			float appAverage = 0.0; 
			float GAsupCU = 0.0;
			int GsupC = 0;
			
			int nuA = 0, nuC = 0, nuG = 0, nuU = 0;
			int maxA = 0, maxC = 0, maxG = 0, maxU = 0;
			float proportionNEPAL = 0.0;
			int tempSUITE = 0, tempSUITE2 = 0, maxSUITE = 0, tailleSYM = 0, tailleBULGE = 0;
			int totalG = 0;
			int totalD = 0;
			float ratioAgauche = 0.0;
			float ratioCgauche = 0.0;
			float ratioGgauche = 0.0;
			float ratioUgauche = 0.0;
			float ratioAdroite = 0.0;
			float ratioCdroite = 0.0;
			float ratioGdroite = 0.0;
			float ratioUdroite = 0.0;
			int SOMMEGaucheDroite = 0;
			float SYMgap = 0.0;
			float BULGEgap = 0.0;
			float ratioLIGNE = 0.0;
			
			int A3parenthese = 0, A2parentheseG = 0, A2parentheseM = 0, A2parentheseD = 0, A1parentheseG = 0, A1parentheseM = 0, A1parentheseD = 0, A0parenthese = 0;
			int C3parenthese = 0, C2parentheseG = 0, C2parentheseM = 0, C2parentheseD = 0, C1parentheseG = 0, C1parentheseM = 0, C1parentheseD = 0, C0parenthese = 0;
			int G3parenthese = 0, G2parentheseG = 0, G2parentheseM = 0, G2parentheseD = 0, G1parentheseG = 0, G1parentheseM = 0, G1parentheseD = 0, G0parenthese = 0;
			int U3parenthese = 0, U2parentheseG = 0, U2parentheseM = 0, U2parentheseD = 0, U1parentheseG = 0, U1parentheseM = 0, U1parentheseD = 0, U0parenthese = 0;
			
			int dernierG = 0, nbGroupG = 0, nbGroupD = 0;
			int ntSymmetricalRegion = 0;
			int tempSR = 0;
			int debSR = 0;
			int finSR = 0;
			int tempD = 0;
			int tempF = 0;
			int tailleASYM = 0;
			int nbSYM = 0;
			int nbASYM = 0;
			int nbBULGE = 0;
			
			
			
			
			
			
			for(int k=0; k<resultatH[i].SeedLG; k++)
			{
				char gauche = arguments->text[q][resultatH[i].SeedDEB + k - 1];
				char droite = arguments->text[q][resultatH[i].SeedFIN - k - 1];
			
				if(gauche == 'A')
				{
					if(k < resultatH[i].SeedLG-1)
					{
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'A')
							ratioAA = ratioAA + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'C')
							ratioAC = ratioAC + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'G')
							ratioAG = ratioAG + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'U')
							ratioAU = ratioAU + 1.0;
					}
				
					graineA++;
				
					tempA++;
					tempC = 0;
					tempG = 0;
					tempU = 0;
					if(MaxA < tempA)
						MaxA = tempA;
				}
				else if(gauche == 'C')
				{
					if(k < resultatH[i].SeedLG-1)
					{
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'A')
							ratioCA = ratioCA + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'C')
							ratioCC = ratioCC + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'G')
							ratioCG = ratioCG + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'U')
							ratioCU = ratioCU + 1.0;
					}
				
					graineC++;
				
					tempC++;
					tempA = 0;
					tempG = 0;
					tempU = 0;
					if(MaxC < tempC)
						MaxC = tempC;
				}
				else if(gauche == 'G')
				{
					if(k < resultatH[i].SeedLG-1)
					{
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'A')
							ratioGA = ratioGA + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'C')
							ratioGC = ratioGC + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'G')
							ratioGG = ratioGG + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'U')
							ratioGU = ratioGU + 1.0;
					}
				
					graineG++;
				
					tempG++;
					tempA = 0;
					tempC = 0;
					tempU = 0;
					if(MaxG < tempG)
						MaxG = tempG;
				}
				else if(gauche == 'U')
				{
					if(k < resultatH[i].SeedLG-1)
					{
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'A')
							ratioUA = ratioUA + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'C')
							ratioUC = ratioUC + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'G')
							ratioUG = ratioUG + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB + k] == 'U')
							ratioUU = ratioUU + 1.0;
					}
				
					graineU++;
				
					tempU++;
					tempA = 0;
					tempG = 0;
					tempC = 0;
					if(MaxG < tempG)
						MaxG = tempG;
				}

				if( (gauche == 'U') && (droite == 'G') )
				{
					GUgraine++;
				}
				if( (gauche == 'G') && (droite == 'U') )
				{
					GUgraine++;
				}
				if( (gauche == 'C') && (droite == 'G') )
				{
					GCgraine++;
				}
				if( (gauche == 'G') && (droite == 'C') )
				{
					GCgraine++;
				}
			}

			tempA = 0;
			tempC = 0;
			tempG = 0;
			tempU = 0;
			for(int k=0; k<resultatH[i].SeedLG; k++)
			{
				char droite = arguments->text[q][resultatH[i].SeedFIN - k - 1];
			
				if(droite == 'A')
				{
					if(k < resultatH[i].SeedLG-1)
					{
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'A')
							ratioAA = ratioAA + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'C')
							ratioAC = ratioAC + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'G')
							ratioAG = ratioAG + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'U')
							ratioAU = ratioAU + 1.0;
					}
				
					graineA++;
				
					tempA++;
					tempC = 0;
					tempG = 0;
					tempU = 0;
					if(MaxA < tempA)
						MaxA = tempA;
				}
				else if(droite == 'C')
				{
					if(k < resultatH[i].SeedLG-1)
					{
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'A')
							ratioCA = ratioCA + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'C')
							ratioCC = ratioCC + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'G')
							ratioCG = ratioCG + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'U')
							ratioCU = ratioCU + 1.0;
					}
				
					graineC++;
				
					tempC++;
					tempA = 0;
					tempG = 0;
					tempU = 0;
					if(MaxC < tempC)
						MaxC = tempC;
				}
				else if(droite == 'G')
				{
					if(k < resultatH[i].SeedLG-1)
					{
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'A')
							ratioGA = ratioGA + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'C')
							ratioGC = ratioGC + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'G')
							ratioGG = ratioGG + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'U')
							ratioGU = ratioGU + 1.0;
					}
				
					graineG++;
				
					tempG++;
					tempA = 0;
					tempC = 0;
					tempU = 0;
					if(MaxG < tempG)
						MaxG = tempG;
				}
				else if(droite == 'U')
				{
					if(k < resultatH[i].SeedLG-1)
					{
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'A')
							ratioUA = ratioUA + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'C')
							ratioUC = ratioUC + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'G')
							ratioUG = ratioUG + 1.0;
						if(arguments->text[q][resultatH[i].SeedDEB - k - 2] == 'U')
							ratioUU = ratioUU + 1.0;
					}
				
					graineU++;
				
					tempU++;
					tempA = 0;
					tempG = 0;
					tempC = 0;
					if(MaxU < tempU)
						MaxU = tempU;
				}
			}

			ratioA = (float)graineA / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioC = (float)graineC / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioG = (float)graineG / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioU = (float)graineU / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioMatchGU = ((float)GUgraine) * 100 / ((float)resultatH[i].SeedLG);
			ratioMatchGC = ((float)GCgraine) * 100 / ((float)resultatH[i].SeedLG);
			MFEindex = resultatH[i].Seedscore / (2 * (float)resultatH[i].SeedLG);
		
			ratioAA = ratioAA / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioAC = ratioAC / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioAG = ratioAG / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioAU = ratioAU / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioCA = ratioCA / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioCC = ratioCC / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioCG = ratioCG / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioCU = ratioCU / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioGA = ratioGA / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioGC = ratioGC / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioGG = ratioGG / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioGU = ratioGU / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioUA = ratioUA / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioUC = ratioUC / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioUG = ratioUG / (2 * (float)resultatH[i].SeedLG) * 100;
			ratioUU = ratioUU / (2 * (float)resultatH[i].SeedLG) * 100;
		
		

		
			cout << "1:" << resultatH[i].SeedDEB << "\t";
			cout << "2:" << resultatH[i].SeedFIN << "\t";
			cout << "3:" << resultatH[i].SeedLG << "\t";
			cout << "4:" << resultatH[i].Seedscore << "\t";
			cout << "5:" << resultatH[i].SeedDist << "\t";
			cout << "6:" << ratioA << "\t";
			cout << "7:" << ratioC << "\t";
			cout << "8:" << ratioG << "\t";
			cout << "9:" << ratioU << "\t";
			cout << "10:" << MaxA << "\t";
			cout << "11:" << MaxC << "\t";
			cout << "12:" << MaxG << "\t";
			cout << "13:" << MaxU << "\t";
			cout << "14:" << ratioMatchGU << "\t";
			cout << "15:" << ratioMatchGC << "\t"; 
			cout << "16:" << ratioAA << "\t";
			cout << "17:" << ratioAC << "\t";
			cout << "18:" << ratioAG << "\t";
			cout << "19:" << ratioAU << "\t";
			cout << "20:" << ratioCA << "\t";
			cout << "21:" << ratioCC << "\t";
			cout << "22:" << ratioCG << "\t";
			cout << "23:" << ratioCU << "\t";
			cout << "24:" << ratioGA << "\t";
			cout << "25:" << ratioGC << "\t";
			cout << "26:" << ratioGG << "\t";
			cout << "27:" << ratioGU << "\t";
			cout << "28:" << ratioUA << "\t";
			cout << "29:" << ratioUC << "\t";
			cout << "30:" << ratioUG << "\t";
			cout << "31:" << ratioUU << "\t";
			cout << "32:" << MFEindex << "\t";
		
	
			
			
			
		
		
			ratioAA = 0.0, ratioAC = 0.0, ratioAG = 0.0, ratioAU = 0.0;
			ratioCA = 0.0, ratioCC = 0.0, ratioCG = 0.0, ratioCU = 0.0;
			ratioGA = 0.0, ratioGC = 0.0, ratioGG = 0.0, ratioGU = 0.0;
			ratioUA = 0.0, ratioUC = 0.0, ratioUG = 0.0, ratioUU = 0.0;
	
			for(int k=0; k<resultatH[i].neTAILLE; k++)
			{
				char gauche = arguments->text[q][resultatH[i].neDEB + k - 1];
				char droite = arguments->text[q][resultatH[i].neFIN - k - 1];
			
				if( (gauche == 'A' && droite == 'U') || (gauche == 'C' && droite == 'G')
				 || (gauche == 'G' && droite == 'C') || (gauche == 'G' && droite == 'U')
				 || (gauche == 'U' && droite == 'A') || (gauche == 'U' && droite == 'G') )
				{
					tempBigPal++;
					if(BigPal < tempBigPal)
						BigPal = tempBigPal;
				}
			
				if(gauche == 'A')
				{
					if(k < resultatH[i].neTAILLE-1)
					{
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'A')
							ratioAA = ratioAA + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'C')
							ratioAC = ratioAC + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'G')
							ratioAG = ratioAG + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'U')
							ratioAU = ratioAU + 1.0;
					}
				
					tigeA++;

					tempSuiteA++;
					tempSuiteC = 0;
					tempSuiteG = 0;
					tempSuiteT = 0;
					if(MaxSuiteA < tempSuiteA)
						MaxSuiteA = tempSuiteA;
				}
				if(gauche == 'C')
				{
					if(k < resultatH[i].neTAILLE-1)
					{
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'A')
							ratioCA = ratioCA + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'C')
							ratioCC = ratioCC + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'G')
							ratioCG = ratioCG + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'U')
							ratioCU = ratioCU + 1.0;
					}
				
					tigeC++;

					tempSuiteC++;
					tempSuiteA = 0;
					tempSuiteG = 0;
					tempSuiteT = 0;
					if(MaxSuiteC < tempSuiteC)
						MaxSuiteC = tempSuiteC;
				}
				if(gauche == 'G')
				{
					if(k < resultatH[i].neTAILLE-1)
					{
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'A')
							ratioGA = ratioGA + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'C')
							ratioGC = ratioGC + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'G')
							ratioGG = ratioGG + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'U')
							ratioGU = ratioGU + 1.0;
					}
				
					tigeG++;

					tempSuiteG++;
					tempSuiteA = 0;
					tempSuiteC = 0;
					tempSuiteT = 0;
					if(MaxSuiteG < tempSuiteG)
						MaxSuiteG = tempSuiteG;
				}
				if(gauche == 'U')
				{
					if(k < resultatH[i].neTAILLE-1)
					{
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'A')
							ratioUA = ratioUA + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'C')
							ratioUC = ratioUC + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'G')
							ratioUG = ratioUG + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE + k] == 'U')
							ratioUU = ratioUU + 1.0;
					}
				
					tigeU++;

					tempSuiteT++;
					tempSuiteA = 0;
					tempSuiteC = 0;
					tempSuiteG = 0;
					if(MaxSuiteT < tempSuiteT)
						MaxSuiteT = tempSuiteT;
				}
			
				if( ((gauche == 'U') && (droite == 'G')) || ((gauche == 'G') && (droite == 'U')))
				{
					matchGU++;
				}
			
				if( ((gauche == 'C') && (droite == 'G')) || ((gauche == 'G') && (droite == 'C')))
				{
					matchGC++;
				}
			}

			tempSuiteA = 0;
			tempSuiteC = 0;
			tempSuiteG = 0;
			tempSuiteT = 0;
			for(int k=0; k<resultatH[i].neTAILLE; k++)
			{
				char droite = arguments->text[q][resultatH[i].neFIN - k - 1];
				if(droite == 'A')
				{
					if(k < resultatH[i].neTAILLE-1)
					{
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'A')
							ratioAA = ratioAA + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'C')
							ratioAC = ratioAC + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'G')
							ratioAG = ratioAG + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'U')
							ratioAU = ratioAU + 1.0;
					}
				
					tigeA++;

					tempSuiteA++;
					tempSuiteC = 0;
					tempSuiteG = 0;
					tempSuiteT = 0;
					if(MaxSuiteA < tempSuiteA)
						MaxSuiteA = tempSuiteA;
				}
				if(droite == 'C')
				{
					if(k < resultatH[i].neTAILLE-1)
					{
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'A')
							ratioCA = ratioCA + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'C')
							ratioCC = ratioCC + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'G')
							ratioCG = ratioCG + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'U')
							ratioCU = ratioCU + 1.0;
					}
				
					tigeC++;

					tempSuiteC++;
					tempSuiteA = 0;
					tempSuiteG = 0;
					tempSuiteT = 0;
					if(MaxSuiteC < tempSuiteC)
						MaxSuiteC = tempSuiteC;
				}
				if(droite == 'G')
				{
					if(k < resultatH[i].neTAILLE-1)
					{
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'A')
							ratioGA = ratioGA + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'C')
							ratioGC = ratioGC + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'G')
							ratioGG = ratioGG + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'U')
							ratioGU = ratioGU + 1.0;
					}
				
					tigeG++;

					tempSuiteG++;
					tempSuiteA = 0;
					tempSuiteC = 0;
					tempSuiteT = 0;
					if(MaxSuiteG < tempSuiteG)
						MaxSuiteG = tempSuiteG;
				}
				if(droite == 'U')
				{
					if(k < resultatH[i].neTAILLE-1)
					{
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'A')
							ratioUA = ratioUA + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'C')
							ratioUC = ratioUC + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'G')
							ratioUG = ratioUG + 1.0;
						if(arguments->text[q][resultatH[i].neTAILLE - k - 2] == 'U')
							ratioUU = ratioUU + 1.0;
					}
				
					tigeU++;

					tempSuiteT++;
					tempSuiteA = 0;
					tempSuiteC = 0;
					tempSuiteG = 0;
					if(MaxSuiteT < tempSuiteT)
						MaxSuiteT = tempSuiteT;
				}
			}

			ratioTigeA = ((float)tigeA) / (2 * ((float)resultatH[i].neTAILLE)) * 100;
			ratioTigeC = ((float)tigeC) / (2 * ((float)resultatH[i].neTAILLE)) * 100;
			ratioTigeG = ((float)tigeG) / (2 * ((float)resultatH[i].neTAILLE)) * 100;
			ratioTigeU = ((float)tigeU) / (2 * ((float)resultatH[i].neTAILLE)) * 100;
			
			
			for(int k=0; k<resultatH[i].neTAILLE; k++)
			{
				char gauche = arguments->text[q][resultatH[i].neDEB + k - 1];
				char droite = arguments->text[q][resultatH[i].neFIN - k - 1];
			
				if( (gauche == 'A' && droite == 'U') || (gauche == 'C' && droite == 'G')
				 || (gauche == 'G' && droite == 'C') || (gauche == 'G' && droite == 'U')
				 || (gauche == 'U' && droite == 'A') || (gauche == 'U' && droite == 'G') )
				{
				}
				else
				{
					tempBigPal = 0;
				
					if(gauche == 'A')
						nontigeA++;
					if(gauche == 'C')
						nontigeC++;
					if(gauche == 'G')
						nontigeG++;
					if(gauche == 'U')
						nontigeU++;
					if(droite == 'A')
						nontigeA++;
					if(droite == 'C')
						nontigeC++;
					if(droite == 'G')
						nontigeG++;
					if(droite == 'U')
						nontigeU++;
				}
			}
		
			NONratioTigeA = ((float)nontigeA) / (2 * ((float)resultatH[i].neMesap)) * 100;
			NONratioTigeC = ((float)nontigeC) / (2 * ((float)resultatH[i].neMesap)) * 100;
			NONratioTigeG = ((float)nontigeG) / (2 * ((float)resultatH[i].neMesap)) * 100;
			NONratioTigeU = ((float)nontigeU) / (2 * ((float)resultatH[i].neMesap)) * 100;
		
			APPratioA = ((float)(tigeA - nontigeA)) / (2 * ((float)(resultatH[i].neTAILLE - resultatH[i].neMesap))) * 100;
			APPratioC = ((float)(tigeC - nontigeC)) / (2 * ((float)(resultatH[i].neTAILLE - resultatH[i].neMesap))) * 100;
			APPratioG = ((float)(tigeG - nontigeG)) / (2 * ((float)(resultatH[i].neTAILLE - resultatH[i].neMesap))) * 100;
			APPratioU = ((float)(tigeU - nontigeU)) / (2 * ((float)(resultatH[i].neTAILLE - resultatH[i].neMesap))) * 100;

			ratioAA = ratioAA / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioAC = ratioAC / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioAG = ratioAG / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioAU = ratioAU / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioCA = ratioCA / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioCC = ratioCC / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioCG = ratioCG / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioCU = ratioCU / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioGA = ratioGA / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioGC = ratioGC / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioGG = ratioGG / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioGU = ratioGU / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioUA = ratioUA / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioUC = ratioUC / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioUG = ratioUG / (2 * (float)resultatH[i].neTAILLE) * 100;
			ratioUU = ratioUU / (2 * (float)resultatH[i].neTAILLE) * 100;


			Appariement = resultatH[i].neTAILLE - resultatH[i].neMesap;
			appF = ((float)Appariement) / ((float)resultatH[i].neTAILLE) * 100;
			appAverage = ((float)Appariement) / ((float)resultatH[i].neNBPal); 
			ratioMatchGU = (float)matchGU / (float)(resultatH[i].neTAILLE - resultatH[i].neMesap) * 100;
			ratioMatchGC = (float)matchGC / (float)(resultatH[i].neTAILLE - resultatH[i].neMesap) * 100;
			if((tigeG + tigeA - tigeC - tigeU) != 0)
				GAsupCU = (float)(tigeG + tigeA - tigeC - tigeU) / (float)(tigeG + tigeA + tigeC + tigeU);
			GsupC = (float)(tigeG - tigeC) / (float)(tigeG + tigeC);
		
		
		
			cout << "33:" << resultatH[i].neDEB << "\t";
			cout << "34:" << resultatH[i].neFIN << "\t";
			cout << "35:" << resultatH[i].neTAILLE << "\t";
			cout << "36:" << resultatH[i].neDIST << "\t";
			cout << "37:" << resultatH[i].neDG << "\t";
			cout << "38:" << appF << "\t";
			cout << "39:" << ratioTigeA << "\t";
			cout << "40:" << ratioTigeC << "\t";
			cout << "41:" << ratioTigeG << "\t";
			cout << "42:" << ratioTigeU << "\t";
			cout << "43:" << MaxSuiteA << "\t";
			cout << "44:" << MaxSuiteC << "\t";
			cout << "45:" << MaxSuiteG << "\t";
			cout << "46:" << MaxSuiteT << "\t";
			cout << "47:" << ratioAA << "\t";
			cout << "48:" << ratioAC << "\t";
			cout << "49:" << ratioAG << "\t";
			cout << "50:" << ratioAU << "\t";
			cout << "51:" << ratioCA << "\t";
			cout << "52:" << ratioCC << "\t";
			cout << "53:" << ratioCG << "\t";
			cout << "54:" << ratioCU << "\t";
			cout << "55:" << ratioGA << "\t";
			cout << "56:" << ratioGC << "\t";
			cout << "57:" << ratioGG << "\t";
			cout << "58:" << ratioGU << "\t";
			cout << "59:" << ratioUA << "\t";
			cout << "60:" << ratioUC << "\t";
			cout << "61:" << ratioUG << "\t";
			cout << "62:" << ratioUU << "\t";
			cout << "63:" << GsupC << "\t";
			cout << "64:" << GAsupCU << "\t";
		
			cout << "65:" << appAverage << "\t";
			cout << "66:" << resultatH[i].neNBPal << "\t";
			cout << "67:" << BigPal << "\t";
			
			if(nontigeA > 0)
				cout << "68:" << NONratioTigeA << "\t";
			else
				cout << "68:" << "0.0" << "\t";
			if(nontigeC > 0)
				cout << "69:" << NONratioTigeC << "\t";
			else
				cout << "69:" << "0.0" << "\t";
			if(nontigeG > 0)
				cout << "70:" << NONratioTigeG << "\t";
			else
				cout << "70:" << "0.0" << "\t";
			if(nontigeU > 0)
				cout << "71:" << NONratioTigeU << "\t";
			else
				cout << "71:" << "0.0" << "\t";
			
			cout << "72:" << APPratioA << "\t";
			cout << "73:" << APPratioC << "\t";
			cout << "74:" << APPratioG << "\t";
			cout << "75:" << APPratioU << "\t";
			cout << "76:" << ratioMatchGU << "\t";
			cout << "77:" << ratioMatchGC << "\t";
		
		
		
		
		
			ratioAA = 0.0, ratioAC = 0.0, ratioAG = 0.0, ratioAU = 0.0;
			ratioCA = 0.0, ratioCC = 0.0, ratioCG = 0.0, ratioCU = 0.0;
			ratioGA = 0.0, ratioGC = 0.0, ratioGG = 0.0, ratioGU = 0.0;
			ratioUA = 0.0, ratioUC = 0.0, ratioUG = 0.0, ratioUU = 0.0;
			tempA = 0, tempC = 0, tempG = 0, tempU = 0;
			tempBigPal = 0, BigPal = 0;
			
			for(int z=0; z<=(resultatH[i].positionFIN-resultatH[i].positionDEB); z++)
			{
				char gauche = arguments->text[q][resultatH[i].positionDEB + z - 1];
			
				if(gauche == 'A')
				{
					if(z < (resultatH[i].positionFIN-resultatH[i].positionDEB))
					{
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'A')
							ratioAA = ratioAA + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'C')
							ratioAC = ratioAC + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'G')
							ratioAG = ratioAG + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'U')
							ratioAU = ratioAU + 1.0;
					}
				
					nuA++;
					tempA++;
					tempC = 0;
					tempG = 0;
					tempU = 0;
					if(maxA < tempA)
						maxA = tempA;
				}
				if(gauche == 'C')
				{
					if(z < (resultatH[i].positionFIN-resultatH[i].positionDEB))
					{
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'A')
							ratioCA = ratioCA + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'C')
							ratioCC = ratioCC + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'G')
							ratioCG = ratioCG + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'U')
							ratioCU = ratioCU + 1.0;
					}
				
					nuC++;
					tempC++;
					tempA = 0;
					tempG = 0;
					tempU = 0;
					if(maxC < tempC)
						maxC = tempC;
				}
				if(gauche == 'G')
				{
					if(z < (resultatH[i].positionFIN-resultatH[i].positionDEB))
					{
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'A')
							ratioGA = ratioGA + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'C')
							ratioGC = ratioGC + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'G')
							ratioGG = ratioGG + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'U')
							ratioGU = ratioGU + 1.0;
					}
				
					nuG++;
					tempG++;
					tempC = 0;
					tempA = 0;
					tempU = 0;
					if(maxG < tempG)
						maxG = tempG;
				}
				if(gauche == 'U')
				{
					if(z < (resultatH[i].positionFIN-resultatH[i].positionDEB))
					{
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'A')
							ratioUA = ratioUA + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'C')
							ratioUC = ratioUC + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'G')
							ratioUG = ratioUG + 1.0;
						if(arguments->text[q][resultatH[i].positionDEB + z] == 'U')
							ratioUU = ratioUU + 1.0;
					}
				
					nuU++;
					tempU++;
					tempC = 0;
					tempG = 0;
					tempA = 0;
					if(maxU < tempU)
						maxU = tempU;
				}
			}
		
			ratioA = ((float)nuA * 100) / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB));
			ratioC = ((float)nuC * 100) / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB));
			ratioG = ((float)nuG * 100) / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB));
			ratioU = ((float)nuU * 100) / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB));
			ratioAA = ratioAA / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioAC = ratioAC / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioAG = ratioAG / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioAU = ratioAU / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioCA = ratioCA / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioCC = ratioCC / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioCG = ratioCG / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioCU = ratioCU / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioGA = ratioGA / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioGC = ratioGC / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioGG = ratioGG / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioGU = ratioGU / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioUA = ratioUA / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioUC = ratioUC / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioUG = ratioUG / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			ratioUU = ratioUU / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB)) * 100;
			if((nuG + nuA - nuC - nuU) == 0)
				GAsupCU = 0;
			else
				GAsupCU = (float)(nuG + nuA - nuC - nuU) / (float)(nuG + nuA + nuC + nuU);
			GsupC = (float)(nuG - nuC) / (float)(nuG + nuC);
			proportionNEPAL = (((float)(resultatH[i].neFIN-resultatH[i].neDEB)) / ((float)(resultatH[i].positionFIN - resultatH[i].positionDEB))) * 100;

			cout << "78:" << (resultatH[i].positionDEB + arguments->addPos - 1) << "\t" ;
			cout << "79:" << (resultatH[i].positionFIN + arguments->addPos - 1) << "\t" ;
			cout << "80:" << (resultatH[i].positionFIN - resultatH[i].positionDEB) << "\t";
			cout << "81:" << resultatH[i].deltaG << "\t";
			cout << "82:" << resultatH[i].boucle << "\t";
			cout << "83:" << resultatH[i].appariementH << "\t";
			cout << "84:" << ratioA << "\t";
			cout << "85:" << ratioC << "\t";
			cout << "86:" << ratioG << "\t";
			cout << "87:" << ratioU << "\t";
			cout << "88:" << ratioAA << "\t";
			cout << "89:" << ratioAC << "\t";
			cout << "90:" << ratioAG << "\t";
			cout << "91:" << ratioAU << "\t";
			cout << "92:" << ratioCA << "\t";
			cout << "93:" << ratioCC << "\t";
			cout << "94:" << ratioCG << "\t";
			cout << "95:" << ratioCU << "\t";
			cout << "96:" << ratioGA << "\t";
			cout << "97:" << ratioGC << "\t";
			cout << "98:" << ratioGG << "\t";
			cout << "99:" << ratioGU << "\t";
			cout << "100:" << ratioUA << "\t";
			cout << "101:" << ratioUC << "\t";
			cout << "102:" << ratioUG << "\t";
			cout << "103:" << ratioUU << "\t";
			cout << "104:" << maxA << "\t";
			cout << "105:" << maxC << "\t";
			cout << "106:" << maxG << "\t";
			cout << "107:" << maxU << "\t";
			cout << "108:" << resultatH[i].nbPal << "\t";
			cout << "109:" << resultatH[i].GUappariement << "\t";
			cout << "110:" << resultatH[i].GCappariement << "\t";
			cout << "111:" << resultatH[i].MFEajusted << "\t";
			cout << "112:" << proportionNEPAL << "\t";
			cout << "113:" << GAsupCU << "\t";
			cout << "114:" << GsupC << "\t";
			cout << "115:" << resultatH[i].PalMoy << "\t";
			
			tigeA = 0;
			tigeC = 0;
			tigeG = 0;
			tigeU = 0;
			for(int k=0; k<resultatH[i].nbPal; k++)
			{
				for(int z=0; z<resultatH[i].strTAILLE[k]; z++)
				{
					char lettre = arguments->text[q][resultatH[i].strGAUCHE[k] + z - 1];
					if(lettre == 'A')
						tigeA++;
					if(lettre == 'C')
						tigeC++;
					if(lettre == 'G')
						tigeG++;
					if(lettre == 'U')
						tigeU++;
				
					lettre = arguments->text[q][resultatH[i].strDROITE[k] - z - 1];
					if(lettre == 'A')
						tigeA++;
					if(lettre == 'C')
						tigeC++;
					if(lettre == 'G')
						tigeG++;
					if(lettre == 'U')
						tigeU++;
				}
			}
			ratioA = ((float)tigeA * 100) / ((float)(resultatH[i].appariement));
			ratioC = ((float)tigeC * 100) / ((float)(resultatH[i].appariement));
			ratioG = ((float)tigeG * 100) / ((float)(resultatH[i].appariement));
			ratioU = ((float)tigeU * 100) / ((float)(resultatH[i].appariement));
		
			cout << "116:" << ratioA << "\t";
			cout << "117:" << ratioC << "\t";
			cout << "118:" << ratioG << "\t";
			cout << "119:" << ratioU << "\t";
			cout << "120:" << resultatH[i].appMFE << "\t"; 
			resultatH[i].appMFE = resultatH[i].appMFE / ((float)resultatH[i].appariement);
			cout << "121:" << resultatH[i].appMFE << "\t";


			
			tigeA = 0;
			tigeC = 0;
			tigeG = 0;
			tigeU = 0;			
			for(int k=0; k<resultatH[i].nbPal-1; k++)
			{
				int finG = (resultatH[i].strGAUCHE[k] + resultatH[i].strTAILLE[k]);
				int tailleG = resultatH[i].strGAUCHE[k + 1] - finG;
				for(int z=0; z<tailleG; z++)
				{
					char lettre = arguments->text[q][finG + z - 1];

					if(lettre == 'A')
						tigeA++;
					if(lettre == 'C')
						tigeC++;
					if(lettre == 'G')
						tigeG++;
					if(lettre == 'U')
						tigeU++;
				}
				totalG = totalG + tailleG;
			}
			if(totalG != 0)
			{
				ratioAgauche = ((float)tigeA * 100) / ((float)totalG);
				ratioCgauche = ((float)tigeC * 100) / ((float)totalG);
				ratioGgauche = ((float)tigeG * 100) / ((float)totalG);
				ratioUgauche = ((float)tigeU * 100) / ((float)totalG);
			}
		
  			tigeA = 0;
			tigeC = 0;
			tigeG = 0;
			tigeU = 0;
			for(int k=0; k<resultatH[i].nbPal-1; k++)
			{
				int finD = (resultatH[i].strDROITE[k] - resultatH[i].strTAILLE[k]);
				int tailleD = finD - resultatH[i].strDROITE[k + 1];
				for(int z=0; z<tailleD; z++)
				{
					char lettre = arguments->text[q][finD - z - 1];
					if(lettre == 'A')
						tigeA++;
					if(lettre == 'C')
						tigeC++;
					if(lettre == 'G')
						tigeG++;
					if(lettre == 'U')
						tigeU++;
				}
				totalD = totalD + tailleD;

				int finG = (resultatH[i].strGAUCHE[k] + resultatH[i].strTAILLE[k]);
				int tailleG = resultatH[i].strGAUCHE[k + 1] - finG;
			
				if(tailleG == tailleD)
				{
					tempSUITE2 = 0;
					tempSUITE++;
					if(maxSUITE < tempSUITE)
						maxSUITE = tempSUITE;
				
					tailleSYM = tailleSYM + tailleG + tailleD;
				}
				else
				{
					tempSUITE2++;
					if(resultatH[i].suiteBulge < tempSUITE2)
						resultatH[i].suiteBulge = tempSUITE2;
					tempSUITE = 0;
					tailleBULGE = tailleBULGE + tailleG + tailleD;
				
					if(tailleG > tailleD)
					{
						resultatH[i].nbBG++;
					}
					else
					{
						resultatH[i].nbBD++;
					}
				}
			}
			if(totalD != 0)
			{
				ratioAdroite = ((float)tigeA * 100) / ((float)totalD);
				ratioCdroite = ((float)tigeC * 100) / ((float)totalD);
				ratioGdroite = ((float)tigeG * 100) / ((float)totalD);
				ratioUdroite = ((float)tigeU * 100) / ((float)totalD);
			}
			maxSUITE++;	// plus 1 car 1 gap symetrique correspond a 2 palindromes !
		
			ratioA = abs(ratioAgauche - ratioAdroite);
			ratioC = abs(ratioCgauche - ratioCdroite);
			ratioG = abs(ratioGgauche - ratioGdroite);
			ratioU = abs(ratioUgauche - ratioUdroite);
			SOMMEGaucheDroite = abs(totalG - totalD);
			SYMgap = ((float)tailleSYM * 100) / (resultatH[i].positionFIN - resultatH[i].positionDEB);
			BULGEgap = ((float)tailleBULGE * 100) / (resultatH[i].positionFIN - resultatH[i].positionDEB);
			ratioLIGNE = ((float)maxSUITE) / ((float)resultatH[i].nbPal);
		
			cout << "122:" << ratioA << "\t";
			cout << "123:" << ratioC << "\t";
			cout << "124:" << ratioG << "\t";
			cout << "125:" << ratioU << "\t";
			cout << "126:" << resultatH[i].differenceGaucheDroite << "\t";
			cout << "127:" << SOMMEGaucheDroite << "\t";
			cout << "128:" << resultatH[i].hairpinMAXloop << "\t";
			cout << "129:" << maxSUITE << "\t";
			cout << "130:" << ratioLIGNE << "\t";
			cout << "131:" << SYMgap << "\t";
			cout << "132:" << BULGEgap << "\t";
			cout << "133:" << resultatH[i].suiteBcote << "\t";
			cout << "134:" << resultatH[i].suiteBulge << "\t";
			cout << "135:" << resultatH[i].bulgeDiff << "\t";
			cout << "136:" << resultatH[i].bulgeMax << "\t";

			tigeA = 0;
			tigeC = 0;
			tigeG = 0;
			tigeU = 0;
			int finGZ = (resultatH[i].strGAUCHE[resultatH[i].nbPal-1] + resultatH[i].strTAILLE[resultatH[i].nbPal-1]);
			for(int m=0; m<resultatH[i].boucle; m++)
			{
				char lettre = arguments->text[q][finGZ + m - 1];
				if(lettre == 'A')
					tigeA++;
				if(lettre == 'C')
					tigeC++;
				if(lettre == 'G')
					tigeG++;
				if(lettre == 'U')
					tigeU++;
			}
		
			ratioA = ((float)tigeA * 100) / ((float)resultatH[i].boucle);
			ratioC = ((float)tigeC * 100) / ((float)resultatH[i].boucle);
			ratioG = ((float)tigeG * 100) / ((float)resultatH[i].boucle);
			ratioU = ((float)tigeU * 100) / ((float)resultatH[i].boucle);
			cout << "137:" << ratioA << "\t";
			cout << "138:" << ratioC << "\t";
			cout << "139:" << ratioG << "\t";
			cout << "140:" << ratioU << "\t";
			
			
			
			
			
			
			
			// nouveau features
			int dernierParentheseG = 0;
			for(int z=0; z<(resultatH[i].positionFIN - resultatH[i].positionDEB); z++)
			{
				if(resultatH[i].structure[z] == '(')
				{
					dernierParentheseG = z;
				}
				if(resultatH[i].structure[z] == ')')
				{
					z = resultatH[i].positionFIN;
				}
			}
			
			for(int z=0; z<dernierParentheseG; z++)
			{
				if(resultatH[i].texts[z] == 'A')
				{
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '(') )
						A3parenthese++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '.') )
						A2parentheseG++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '(') )
						A2parentheseM++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '(') )
						A2parentheseD++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '.') )
						A1parentheseG++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '.') )
						A1parentheseM++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '(') )
						A1parentheseD++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '.') )
						A0parenthese++;
				}
				
				if(resultatH[i].texts[z] == 'C')
				{
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '(') )
						C3parenthese++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '.') )
						C2parentheseG++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '(') )
						C2parentheseM++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '(') )
						C2parentheseD++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '.') )
						C1parentheseG++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '.') )
						C1parentheseM++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '(') )
						C1parentheseD++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '.') )
						C0parenthese++;
				}
				
				if(resultatH[i].texts[z] == 'G')
				{
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '(') )
						G3parenthese++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '.') )
						G2parentheseG++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '(') )
						G2parentheseM++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '(') )
						G2parentheseD++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '.') )
						G1parentheseG++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '.') )
						G1parentheseM++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '(') )
						G1parentheseD++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '.') )
						G0parenthese++;
				}
				
				if(resultatH[i].texts[z] == 'U')
				{
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '(') )
						U3parenthese++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '.') )
						U2parentheseG++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '(') )
						U2parentheseM++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '(') )
						U2parentheseD++;
					if( (resultatH[i].structure[z+1] == '(') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '.') )
						U1parentheseG++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '(') && (resultatH[i].structure[z+3] == '.') )
						U1parentheseM++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '(') )
						U1parentheseD++;
					if( (resultatH[i].structure[z+1] == '.') && (resultatH[i].structure[z+2] == '.') && (resultatH[i].structure[z+3] == '.') )
						U0parenthese++;
				}
			}

			float ratioA3parenthese = (float)A3parenthese / (float)dernierParentheseG * 100;
			float ratioA2parentheseG = (float)A2parentheseG / (float)dernierParentheseG * 100;
			float ratioA2parentheseM = (float)A2parentheseM / (float)dernierParentheseG * 100;
			float ratioA2parentheseD = (float)A2parentheseD / (float)dernierParentheseG * 100;
			float ratioA1parentheseG = (float)A1parentheseG / (float)dernierParentheseG * 100;
			float ratioA1parentheseM = (float)A1parentheseM / (float)dernierParentheseG * 100;
			float ratioA1parentheseD = (float)A1parentheseD / (float)dernierParentheseG * 100;
			float ratioA0parenthese = (float)A0parenthese / (float)dernierParentheseG * 100;
			float ratioC3parenthese = (float)C3parenthese / (float)dernierParentheseG * 100;
			float ratioC2parentheseG = (float)C2parentheseG / (float)dernierParentheseG * 100;
			float ratioC2parentheseM = (float)C2parentheseM / (float)dernierParentheseG * 100;
			float ratioC2parentheseD = (float)C2parentheseD / (float)dernierParentheseG * 100;
			float ratioC1parentheseG = (float)C1parentheseG / (float)dernierParentheseG * 100;
			float ratioC1parentheseM = (float)C1parentheseM / (float)dernierParentheseG * 100;
			float ratioC1parentheseD = (float)C1parentheseD / (float)dernierParentheseG * 100;
			float ratioC0parenthese = (float)C0parenthese / (float)dernierParentheseG * 100;
			float ratioG3parenthese = (float)G3parenthese / (float)dernierParentheseG * 100;
			float ratioG2parentheseG = (float)G2parentheseG / (float)dernierParentheseG * 100;
			float ratioG2parentheseM = (float)G2parentheseM / (float)dernierParentheseG * 100;
			float ratioG2parentheseD = (float)G2parentheseD / (float)dernierParentheseG * 100;
			float ratioG1parentheseG = (float)G1parentheseG / (float)dernierParentheseG * 100;
			float ratioG1parentheseM = (float)G1parentheseM / (float)dernierParentheseG * 100;
			float ratioG1parentheseD = (float)G1parentheseD / (float)dernierParentheseG * 100;
			float ratioG0parenthese = (float)G0parenthese / (float)dernierParentheseG * 100;
			float ratioU3parenthese = (float)U3parenthese / (float)dernierParentheseG * 100;
			float ratioU2parentheseG = (float)U2parentheseG / (float)dernierParentheseG * 100;
			float ratioU2parentheseM = (float)U2parentheseM / (float)dernierParentheseG * 100;
			float ratioU2parentheseD = (float)U2parentheseD / (float)dernierParentheseG * 100;
			float ratioU1parentheseG = (float)U1parentheseG / (float)dernierParentheseG * 100;
			float ratioU1parentheseM = (float)U1parentheseM / (float)dernierParentheseG * 100;
			float ratioU1parentheseD = (float)U1parentheseD / (float)dernierParentheseG * 100;
			float ratioU0parenthese = (float)U0parenthese / (float)dernierParentheseG * 100;
			
			cout << "141:" << ratioA3parenthese << "\t";
			cout << "142:" << ratioA2parentheseG << "\t";
			cout << "143:" << ratioA2parentheseM << "\t";
			cout << "144:" << ratioA2parentheseD << "\t";
			cout << "145:" << ratioA1parentheseG << "\t";
			cout << "146:" << ratioA1parentheseM << "\t";
			cout << "147:" << ratioA1parentheseD << "\t";
			cout << "148:" << ratioA0parenthese << "\t";
			cout << "149:" << ratioC3parenthese << "\t";
			cout << "150:" << ratioC2parentheseG << "\t";
			cout << "151:" << ratioC2parentheseM << "\t";
			cout << "152:" << ratioC2parentheseD << "\t";
			cout << "153:" << ratioC1parentheseG << "\t";
			cout << "154:" << ratioC1parentheseM << "\t";
			cout << "155:" << ratioC1parentheseD << "\t";
			cout << "156:" << ratioC0parenthese << "\t";
			cout << "157:" << ratioG3parenthese << "\t";
			cout << "158:" << ratioG2parentheseG << "\t";
			cout << "159:" << ratioG2parentheseM << "\t";
			cout << "160:" << ratioG2parentheseD << "\t";
			cout << "161:" << ratioG1parentheseG << "\t";
			cout << "162:" << ratioG1parentheseM << "\t";
			cout << "163:" << ratioG1parentheseD << "\t";
			cout << "164:" << ratioG0parenthese << "\t";
			cout << "165:" << ratioU3parenthese << "\t";
			cout << "166:" << ratioU2parentheseG << "\t";
			cout << "167:" << ratioU2parentheseM << "\t";
			cout << "168:" << ratioU2parentheseD << "\t";
			cout << "169:" << ratioU1parentheseG << "\t";
			cout << "170:" << ratioU1parentheseM << "\t";
			cout << "171:" << ratioU1parentheseD << "\t";
			cout << "172:" << ratioU0parenthese << "\t";
			
			
			
			cout << "173:" << resultatH[i].AUappariement << "\t";
			
			
			
			

			// recuperation du palindrome non exact le plus long dans l'hairpinMAXloop
			for(int z=1; z<=(resultatH[i].positionFIN-resultatH[i].positionDEB)-1; z++)
			{
				if(resultatH[i].structure[z] == '(')
					dernierG = z;
				
				if( (resultatH[i].structure[z-1] == '(') && (resultatH[i].structure[z] == '.') )
					nbGroupG++;
			}
			for(int z=(resultatH[i].positionFIN-resultatH[i].positionDEB)-1; z>0; z--)
			{
				if( (resultatH[i].structure[z] == '.') && (resultatH[i].structure[z+1] == ')') )
					nbGroupD++;
			}

			int maxGroup = nbGroupG;
			if(maxGroup < nbGroupD)
				maxGroup = nbGroupD;
				
			int *app = new int [maxGroup+1];
			int *mesG = new int [maxGroup+1];
			int *mesD = new int [maxGroup+1];
			int a = 0, b = (resultatH[i].positionFIN-resultatH[i].positionDEB), groupA = 0;
			app[0] = 0;
			while( a <= dernierG+1 )
			{
				if( (resultatH[i].structure[a] == '(') && (resultatH[i].structure[b] == ')') )
				{
					app[groupA]++;
					a++;
					b--;
				}
				if( (resultatH[i].structure[a] == '.') && (resultatH[i].structure[b] == '.') )
				{
					if( (resultatH[i].structure[a-1] == '(') && (resultatH[i].structure[b+1] == ')') )
					{
						groupA++;
						app[groupA] = 0;
					}
					a++;
					b--;
				}
				if( (resultatH[i].structure[a] == '.') && (resultatH[i].structure[b] == ')') )
				{
					if( (resultatH[i].structure[a-1] == '(') && (resultatH[i].structure[b+1] == ')') )
					{
						groupA++;
						app[groupA] = 0;
					}
					a++;
				}
				if( (resultatH[i].structure[a] == '(') && (resultatH[i].structure[b] == '.') )
				{
					if( (resultatH[i].structure[a-1] == '(') && (resultatH[i].structure[b+1] == ')') )
					{
						groupA++;
						app[groupA] = 0;
					}
					b--;
				}
			}
			groupA++;

			a = 0, b = (resultatH[i].positionFIN-resultatH[i].positionDEB);
			int groupeMG = 0, groupeMD = 0;
			for(int z=0; z<maxGroup; z++)
			{
				for(int y=0; y<app[z]; y++)
				{
					a++;
					b--;
				}
				mesG[groupeMG] = 0;
				mesD[groupeMD] = 0;
				if(resultatH[i].structure[a] == '.')
				{
					while(resultatH[i].structure[a] == '.')
					{
						mesG[groupeMG]++;
						a++;
					}
				}
				if(resultatH[i].structure[b] == '.')
				{
					while(resultatH[i].structure[b] == '.')
					{
						mesD[groupeMD]++;
						b--;
					}
				}
				
				groupeMG++;
				groupeMD++;
			}
			groupeMG--;
			groupeMD--;


	
			tailleSYM = 0;
			tailleBULGE = 0;
			for(int z=0; z<=groupeMG; z++)
			{
				tempSR = tempSR + app[z];
				if(tempSR > ntSymmetricalRegion)
				{
					ntSymmetricalRegion = tempSR;
					debSR = tempD;
					finSR = tempF;
				}
				
				if(mesG[z] == mesD[z])
				{
					if( (z == groupeMG) && (tempSR == ntSymmetricalRegion) )
						finSR = z;
					
					tempSR = tempSR + mesG[z];
					tempF = z;
					tailleSYM = tailleSYM + 2 * mesD[z];
					nbSYM++;
				}
				else
				{
					if(tempSR == ntSymmetricalRegion)
						finSR = tempF+1;
					
					tempSR = 0;
					tempF = z+1; 
					tempD = z+1;
					
					if(mesG[z] > 0 && mesD[z] > 0)
					{
						tailleASYM = tailleASYM + mesG[z] + mesD[z];
						nbASYM++;
					}
					else
					{
						tailleBULGE = tailleBULGE + mesG[z] + mesD[z];
						nbBULGE++;
					}
				}
			}
			
			int boucle = mesG[finSR] + mesD[finSR];
			for(int z=finSR+1; z<groupeMG; z++)
				boucle = boucle + mesG[z] + mesD[z] + app[z]; 
			
			int appariementSR = 0;
			for(int z=debSR; z<=finSR; z++)
				appariementSR = appariementSR + app[z];

			int ntGpasse = 0, ntDpasse = 0, debG = 0, debD = 0, finG = 0, finD = 0, proportionBPSR = 0;
			for(int z=0; z<=groupeMG; z++)
			{
				if(z == debSR)
				{
					debG = ntGpasse;
					debD = ntDpasse;
				}
				ntGpasse = ntGpasse + app[z];
				ntDpasse = ntDpasse + app[z];
				if(z == finSR)
				{
					finG = ntGpasse;
					finD = ntDpasse;
				}
				
				if(z >= debSR && z <= finSR)
					proportionBPSR = proportionBPSR + app[z];
				
				if(mesG[z] == mesD[z])
				{
					ntGpasse = ntGpasse + mesG[z];
					ntDpasse = ntDpasse + mesG[z];
				}
				else
				{
					ntGpasse = ntGpasse + mesG[z];
					ntDpasse = ntDpasse + mesD[z];
				}
			}
			float ratioBPSR = (float)proportionBPSR * 100 / (float)ntSymmetricalRegion;
			
			
			tigeA = 0;
			tigeC = 0;
			tigeG = 0;
			tigeU = 0;
			for(int z=debG; z<finG; z++)
			{
				if(resultatH[i].texts[z] == 'A')
					tigeA++;
				if(resultatH[i].texts[z] == 'C')
					tigeC++;
				if(resultatH[i].texts[z] == 'G')
					tigeG++;
				if(resultatH[i].texts[z] == 'U')
					tigeU++;
			}
			for(int z=(resultatH[i].positionFIN - resultatH[i].positionDEB) - debD; z>=(resultatH[i].positionFIN - resultatH[i].positionDEB) - finD; z--)
			{
				if(resultatH[i].texts[z] == 'A')
					tigeA++;
				if(resultatH[i].texts[z] == 'C')
					tigeC++;
				if(resultatH[i].texts[z] == 'G')
					tigeG++;
				if(resultatH[i].texts[z] == 'U')
					tigeU++;
			}
			
			ratioA = ((float)tigeA * 100) / ((float)(2*(finG - debG)));
			ratioC = ((float)tigeC * 100) / ((float)(2*(finG - debG)));
			ratioG = ((float)tigeG * 100) / ((float)(2*(finG - debG)));
			ratioU = ((float)tigeU * 100) / ((float)(2*(finG - debG)));
			
			
			cout << "174:" << ntSymmetricalRegion << "\t";
			cout << "175:" << boucle << "\t";
			cout << "176:" << appariementSR << "\t";
			cout << "177:" << ratioBPSR << "\t";
			cout << "178:" << ratioA << "\t";
			cout << "179:" << ratioC << "\t";
			cout << "180:" << ratioG << "\t";
			cout << "181:" << ratioU << "\t";  
//			cout << "[[" << tigeA << " " << tigeC << " " << tigeG << " " << tigeU << "   " << debG << " " << finG << " ]] ";
			
			float ratioSYM = ((float)tailleSYM * 100) / ((float)nbSYM);
			float ratioASYM = ((float)tailleASYM * 100) / ((float)nbASYM);
			float ratioBULGE = ((float)tailleBULGE * 100) / ((float)nbBULGE);
			
			cout << "182:" << tailleSYM << "\t";
			cout << "183:" << tailleASYM << "\t";
			cout << "184:" << tailleBULGE << "\t";
			if( (nbSYM > 0) && (tailleSYM > 0) )
				cout << "185:" << ratioSYM << "\t";
			else
				cout << "185:" << "0.0" << "\t";
			
			if( (nbASYM > 0) && (tailleASYM > 0) )
				cout << "186:" << ratioASYM << "\t";
			else
				cout << "186:" << "0.0" << "\t";
			if( (nbBULGE > 0) && (tailleBULGE > 0) )
				cout << "187:" << ratioBULGE << "\t";
			else
				cout << "187:" << "0.0" << "\t";
			
			
			
			
			//cout << "\t";
			//cout << resultatH[i].structure;
			//cout << "\t";
			//cout << resultatH[i].texts;
			cout << "\n";
		}
		
		initHairpin(resultatH, arguments, i);

}
