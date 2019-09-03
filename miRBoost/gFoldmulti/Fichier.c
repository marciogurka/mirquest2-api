#include "gFold.h"

/*************************************************************************************************************************************************/
void EcrirePalindromeImparfait(Palindrome *listePal, int nbPalindromeT, Parameters *arguments)
{
	char gauche = '\0';
	char droite = '\0';
	
	ofstream fichier(arguments->output, ios_base::app ); 
	if(fichier)
	{
		for(int i=0; i<nbPalindromeT; i++)
		{
			if( (listePal[i].choisi == 1) 
			 && (listePal[i].hairpinFin - listePal[i].hairpinDeb >= arguments->tailleMin)
			 && (listePal[i].hairpinDG <= arguments->deltaGMax) )
			{
				// affichage de la graine DNA palindrome
				if(arguments->optionAffichage == 1)
				{
					fichier << (listePal[i].vraiDeb + arguments->addPos - 1) << " ";
					fichier << (listePal[i].vraiFin + arguments->addPos -1);
					fichier << " +/- [" << listePal[i].score << "] ";
					
					for(int k=0; k<listePal[i].longueur; k++)
						fichier << arguments->text[listePal[i].vraiDeb + k - 1];
					
					fichier << " - ";
					
					for(int k=0; k<listePal[i].longueur; k++)
						fichier << arguments->text[(listePal[i].vraiFin - listePal[i].longueur) + k];
					
					fichier << "\n";
				}
				
				
				fichier << (listePal[i].hairpinDeb + arguments->addPos - 1) << " ";	// -1 = bug C++
				fichier << (listePal[i].hairpinFin + arguments->addPos - 1);
				fichier << " +/- [" << listePal[i].hairpinDG << "]";
				
				for(int k=0; k<listePal[i].hairpinTaille; k++)
				{
//					gauche = arguments->text[listePal[i].hairpinDeb + k - 1];
//					droite = arguments->text[listePal[i].hairpinFin - k - 1];
					if( (gauche == 'A' && droite == 'U') || (gauche == 'U' && droite == 'A')
					 || (gauche == 'C' && droite == 'G') || (gauche == 'G' && droite == 'C') )
					{
						fichier << gauche;
					}
					else
						fichier << '.';
				}
				for(int k=0; k<listePal[i].hairpinDIST; k++)
					fichier << '.';
				for(int k=listePal[i].hairpinTaille-1; k>=0; k--)
				{
//					gauche = arguments->text[listePal[i].hairpinDeb + k - 1];
//					droite = arguments->text[listePal[i].hairpinFin - k - 1];
					if( (gauche == 'A' && droite == 'U') || (gauche == 'U' && droite == 'A')
					 || (gauche == 'C' && droite == 'G') || (gauche == 'G' && droite == 'C') )
					{
						fichier << droite;
					}
					else
						fichier << '.';
				}

				fichier << "\n";
			}
		}
		
		fichier.close();
	}
	else
		cerr << "Impossible d'ouvrir le fichier !" << endl;
	
}

/*************************************************************************************************************************************************/
void EcrireHairpin(Hairpin *resultatH, Parameters *arguments, int i, int fin4fenetre)
{
	if( (resultatH[i].positionDEB != -1) && (resultatH[i].positionFIN <= fin4fenetre) )
	{
		ofstream fichier(arguments->output, ios_base::app ); 
		if(fichier)
		{
			fichier <<(resultatH[i].positionDEB + arguments->addPos - 1) << "\t" ;
			fichier <<(resultatH[i].positionFIN + arguments->addPos - 1) << "\t" ;
			fichier <<"[" << resultatH[i].deltaG << "]\t";
			fichier <<resultatH[i].structure;
				
			if(arguments->optionEValue == 1)
				fichier <<"\t" << resultatH[i].ratioP << " / " << resultatH[i].ratioNE << " / " << resultatH[i].ratioH;

			fichier <<"\n";
			
			int tailleEspace = 0;
			if( (arguments->optionText == 1) || (arguments->mirbase == 1) )
			{
				arguments->output[0] = '\0';
				sprintf(arguments->output, "%i", resultatH[i].positionDEB);
				tailleEspace = strlen(arguments->output) + 1;
				
				sprintf(arguments->output, "%i", resultatH[i].positionFIN);
				tailleEspace = tailleEspace + strlen(arguments->output) + 1;
				
				if( (abs(fmod(resultatH[i].deltaG, 1)) < 0.0001) || (abs(fmod(resultatH[i].deltaG, 1)) > 0.9999) )
				{
					int temp = 0;
					sprintf(arguments->output, "%i", temp);
					tailleEspace = tailleEspace + strlen(arguments->output) + 1 + 2;
				}
				else
				{
					sprintf(arguments->output, "%.1f", resultatH[i].deltaG);
					tailleEspace = tailleEspace + strlen(arguments->output) + 1;
				}

				tailleEspace = tailleEspace + 2;
			}
			if(arguments->optionText == 1)
			{
				for(int k=0; k<tailleEspace; k++)
					fichier <<" ";
				fichier <<resultatH[i].texts << "\n";
			}
			
			if(arguments->mirbase == 1)
			{				
				// je recupere le milieu !
				int dernierG = 0, premierD = 0, nbGroupG = 0, nbGroupD = 0;
				for(int z=1; z<=(resultatH[i].positionFIN-resultatH[i].positionDEB)-1; z++)
				{
					if(resultatH[i].structure[z] == '(')
						dernierG = z;
					
					if( (resultatH[i].structure[z-1] == '(') && (resultatH[i].structure[z] == '.') )
						nbGroupG++;
				}
				for(int z=(resultatH[i].positionFIN-resultatH[i].positionDEB)-1; z>0; z--)
				{
					if( resultatH[i].structure[z] == ')' )
						premierD = z;
					
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
				int boucle = ((premierD - dernierG) - 1);
				int cote = 0;


					
				// ici affichage du mesappariement gauche de l'hairpin
				for(int k=0; k<tailleEspace; k++)
					fichier <<" ";
				int nt = 0;
				for(int z=0; z<groupeMG; z++)
				{
					for(int a=0; a<app[z]; a++)
					{
						fichier <<" ";
						nt++;
					}
					
					if(mesG[z] < mesD[z])
					{
						int diff = mesD[z] - mesG[z];
						for(int a=0; a<diff; a++)
						{
							fichier <<"-";
						}
					}
		
					for(int a=0; a<mesG[z]; a++)
					{
						fichier <<resultatH[i].texts[nt];
						nt++;
					}
				}
				for(int a=0; a<app[groupeMG]; a++)
				{
					fichier <<" ";
					nt++;
				}
				if( (boucle % 2) == 0)
				{
					cote = (boucle - 2) / 2;
					for(int q=0; q<cote; q++)
					{
						fichier <<resultatH[i].texts[nt];
						nt++;
					}
				}
				else
				{
					if(boucle > 3)
					{
						cote = (boucle - 3) / 2;
						for(int q=0; q<cote; q++)
						{
							fichier <<resultatH[i].texts[nt];
							nt++;
						}
					}
				}
				fichier <<"\n";
					
				// ici affichage de l'appariement gauche de l'hairpin
				for(int k=0; k<tailleEspace; k++)
					fichier <<" ";
				nt = 0;
				for(int z=0; z<=groupeMG; z++)
				{
					for(int a=0; a<app[z]; a++)
					{
						fichier <<resultatH[i].texts[nt];
						nt++;
					}
					
					if(z < groupeMG)
					{
						int diff = 0;
						if(mesG[z] < mesD[z])
							diff = mesD[z] - mesG[z];
						for(int a=0; a<diff; a++)
						{
							fichier <<" ";
						}
						for(int a=0; a<mesG[z]; a++)
						{
							fichier <<" ";
							nt++;
						}
					}
				}
				if( (boucle % 2) == 0)
				{
					cote = (boucle - 2) / 2;
					for(int q=0; q<cote; q++)
					{
						fichier <<" ";
						nt++;
					}
					fichier <<resultatH[i].texts[nt];
				}
				else
				{
					if(boucle == 3)
						fichier <<resultatH[i].texts[nt];
					else
					{
						cote = (boucle - 2) / 2;
						for(int q=0; q<cote; q++)
						{
							fichier <<" ";
							nt++;
						}
						fichier <<resultatH[i].texts[nt];
					}
					nt++;
				}
				fichier <<"\n";
					
				// ici liaison des appariements de l'hairpin
				for(int k=0; k<tailleEspace; k++)
					fichier <<" ";
				nt = 0;
				for(int z=0; z<=groupeMG; z++)
				{
					for(int a=0; a<app[z]; a++)
					{
						fichier <<"|";
						nt++;
					}
					
					if( z < groupeMG )
					{
						int diff = 0;
						if(mesG[z] < mesD[z])
							diff = mesD[z] - mesG[z];
						for(int a=0; a<diff; a++)
						{
							fichier <<" ";
						}
						for(int a=0; a<mesG[z]; a++)
						{
							fichier <<" ";
							nt++;
						}
					}
				}
				if( (boucle % 2) != 0)
				{
					if(boucle == 3)
					{
						fichier <<" " << resultatH[i].texts[nt];
					}
					else
					{
						cote = (boucle - 3) / 2;
						for(int q=0; q<cote; q++)
						{
							fichier <<" ";
							nt++;
						}
						fichier <<" " << resultatH[i].texts[nt+1];
					}
				}
				fichier <<"\n";
				
				// ici appariement droite de l'hairpin
				for(int k=0; k<tailleEspace; k++)
					fichier <<" ";
				nt = resultatH[i].positionFIN - resultatH[i].positionDEB;
				for(int z=0; z<=groupeMG; z++)
				{
					for(int a=0; a<app[z]; a++)
					{
						fichier <<resultatH[i].texts[nt];
						nt--;
					}
					
					if( z < groupeMD )
					{
						int diff = 0;
						if(mesD[z] < mesG[z])
							diff = mesG[z] - mesD[z];
						for(int a=0; a<diff; a++)
						{
							fichier <<" ";
						}
						for(int a=0; a<mesD[z]; a++)
						{
							fichier <<" ";
							nt--;
						}
					}
				}
				if( (boucle % 2) == 0)
				{
					cote = (boucle - 2) / 2;
					for(int q=0; q<cote; q++)
					{
						fichier <<" ";
						nt--;
					}
					fichier <<resultatH[i].texts[nt];
				}
				else
				{
					if(boucle == 3)
						fichier <<resultatH[i].texts[nt];
					else
					{
						cote = (boucle - 2) / 2;
						for(int q=0; q<cote; q++)
						{
							fichier <<" ";
							nt--;
						}
						fichier <<resultatH[i].texts[nt];
					}
					nt--;
				}
				fichier <<"\n";
					
				// ici mesappariement droite de l'hairpin
				for(int k=0; k<tailleEspace; k++)
					fichier <<" ";
				nt = resultatH[i].positionFIN - resultatH[i].positionDEB;
				for(int z=0; z<groupeMD; z++)
				{
					for(int a=0; a<app[z]; a++)
					{
						fichier <<" ";
						nt--;
					}
						
					if(mesD[z] < mesG[z])
					{
						int diff = mesG[z] - mesD[z];
						for(int a=0; a<diff; a++)
						{
							fichier <<"-";
						}
					}
		
					for(int a=0; a<mesD[z]; a++)
					{
						fichier <<resultatH[i].texts[nt];
						nt--;
					}
				}
				for(int a=0; a<app[groupeMG]; a++)
				{
					fichier <<" ";
					nt--;
				}
				if( (boucle % 2) == 0)
				{
					cote = (boucle - 2) / 2;
					for(int q=0; q<cote; q++)
					{
						fichier <<resultatH[i].texts[nt];
						nt--;
					}
				}
				else
				{
					if(boucle > 3)
					{
						cote = (boucle - 3) / 2;
						for(int q=0; q<cote; q++)
						{
							fichier <<resultatH[i].texts[nt];
							nt--;
						}
					}
				}
				fichier <<"\n\n";
			}
			
			
			
			resultatH[i].afficher = 1;
			
			initHairpin(resultatH, arguments, i);
		}
		else
			cerr << "Impossible d'ouvrir le fichier !" << endl;
	}
}
