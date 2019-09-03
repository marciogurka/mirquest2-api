#include "gFold.h"

/*************************************************************************************************************************************************/
long ustrlen(char *text)
{
	long i=0;
	while (text[i] != '\0')
	{
		i++;
	}
	return i;
}


/*************************************************************************************************************************************************/
void GetFileContents(char *filename, Parameters *arguments)
{
	std::ifstream::pos_type posSize;
	std::ifstream file ((char *)filename, std::ios::in|std::ios::binary|std::ios::ate);
	
	if (file.is_open())
	{
		posSize = file.tellg();					// je recupere le nombre de caracteres dans le fichier
		unsigned long size = posSize;
		char *memblock = new char[size + 1];			// tableau de caracteres qui va contenir le texte
		memblock[size] = '\0';
		file.seekg(0, std::ios::beg);				// positionnement au debut du fichier
		file.read(memblock, size);				// mise en memoire du texte
		memblock[size] = '\0';
		file.close();
		
		
		unsigned long i = 0;
		int taillename = 0;
		int tailleseq = 0;
		int nbSeq = 0;
		int nombrepair = 0;
		while(memblock[i] != '\0')				// lecture du chromosome sans la premiere ligne fasta
		{
			if(memblock[i] == '\n')
			{
				if(nombrepair == 0)
				{
					nombrepair = 1;
					tailleseq = 0;
				}
				else
				{
					nombrepair = 0;
					taillename = 0;
					nbSeq++;
				}
			}
			
			if( (nombrepair == 0) && (memblock[i] != '>') && (memblock[i] != '\n') )
			{
				arguments->nameSeq[nbSeq][taillename] = memblock[i];
				taillename++;
			}
			if( (nombrepair == 1) && (memblock[i] != '\n') )
			{
				arguments->text[nbSeq][tailleseq] = memblock[i];
				tailleseq++;
			}
			
			i++;
		}
		delete memblock;
	}
	else
	{
		cout << "Input file not found ! \n";
		exit(0);
	}
}

/*************************************************************************************************************************************************/
int float_compare(float d1, float d2)
{
	if (d1 > d2)
	{
		if ((d1 - d2) < fabsf(d1 * FLT_EPSILON))
			return 0;
		else
			return 1;
	}
	else
	{
		if ((d2 - d1) < fabsf(d2 * FLT_EPSILON))
			return 0;
		else
			return -1;
		return 0;
	}
	
	return 0;
}

/*********************************************************************************************************************************************** */
void usage(char *prog)
{
	cout << "Usage: " << prog << " -s sequence -L size [options] \n\n";
	cout << " -s sequence		: a sequence in fasta format \n";
	cout << " -L size		: the minimal size of the sliding windows on the genome \n";
	cout << " -DATA 1 or 0		: 1 for a positive example, 0 for a negative example \n";
	cout << "\n";
	
	cout << "Options : \n";
	cout << " --help 		: show these lines\n";
	cout << "\n";
	exit(1);
}

/*************************************************************************************************************************************************/
void RecupParametres(int argc, char *argv[], Parameters *arguments)
{
	int compteOptions = 0;

	// recuperation des parametres
	for(int i=1; i<argc; i++)
	{
		if(strcmp(argv[i], "-s") == 0)
		{
			GetFileContents(argv[i + 1], arguments);
			compteOptions++;
			i++;
		}

		else if( (strcmp(argv[i], "-L") == 0) || (strcmp(argv[i], "-l") == 0) )
		{
			arguments->tailleMax = atoi(argv[i + 1]);
			compteOptions++;
			i++;
		}
		
		else if( strcmp(argv[i], "-DATA") == 0 )
		{
			arguments->optionAffichage = atoi(argv[i + 1]);
			compteOptions++;
			i++;
		}
		
		else if( (strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "-H") == 0) || (strcmp(argv[i], "--help") == 0) )
		{
			usage(argv[0]);
		}

		else
		{
			cout << "'" << argv[i] << "' parameter does not exist ! \n\n";
			usage(argv[0]);
		}
	}
	
	arguments->stringence = 0;
	

	

	if(compteOptions < 3)
		usage(argv[0]);

}

/*************************************************************************************************************************************************/
void initParameters(Parameters *arguments, int tailleSLIDE, int tailleFASTA, int tailleDATA, int nbSequence)
{
	arguments->nbSequence = nbSequence;
	arguments->mirbase = 0;
	arguments->optionTOUT = 0;
	arguments->optionEValue = 0;
	arguments->nbTige = 0;
	arguments->stringence = 50;
	arguments->slidingStep = 10;
	arguments->addPos = 0;
	arguments->tailleSeed = 0;
	arguments->tailleMin = 0;
	arguments->tailleMax = 0;
	arguments->optionAffichage = 0;
	arguments->optionText = 0;
	arguments->noHairpin = 0;
	arguments->incertitude = 0.2;
	arguments->deltaGMax = 0.0;
	arguments->tailleReelle = 0;
	
	for(int i=0; i<250; i++)
		arguments->output[i] = '\0';

	arguments->fichierDATA = new char[tailleDATA+1];
	if(arguments->fichierDATA == NULL)
	{
		cout << "Allocation Memory arguments->fichierDATA Impossible ! \n";
		exit(1);
	}
	for(int i=0; i<tailleDATA; i++)
		arguments->fichierDATA[i] = '\0';
	
	arguments->tailleSeq = new int [nbSequence];
	arguments->text = new char* [nbSequence];
	if(arguments->text == NULL)
	{
		cout << "Allocation Memory arguments->text Impossible ! \n";
		exit(1);
	}
	else
	{
		for(int i=0; i<nbSequence; i++)
			arguments->text[i] = new char [tailleSLIDE+1];
	}
	
	arguments->nameSeq = new char* [nbSequence];
	if(arguments->text == NULL)
	{
		cout << "Allocation Memory arguments->text Impossible ! \n";
		exit(1);
	}
	else
	{
		for(int i=0; i<nbSequence; i++)
			arguments->nameSeq[i] = new char [256];
	}

	for(int i=0; i<nbSequence; i++)
	{
		arguments->tailleSeq = 0;
		for(int j=0; j<tailleSLIDE+1; j++)
			arguments->text[i][j] = '\0';
		for(int j=0; j<256; j++)
			arguments->nameSeq[i][j] = '\0';
	}
}

/*************************************************************************************************************************************************/
int recupVITALmemoire(int argc, char *argv[], long *tailleFASTA, int *tailleDATA, int *tailleSLIDE, int *nbSequence)
{
	int fichierFASTA = 0;
	int fichierDATA = 0;
	int nbSeq = 0;
	// recuperation des parametres
	for(int i=1; i<argc; i++)
	{
		if( (strcmp(argv[i], "-L") == 0) || (strcmp(argv[i], "-l") == 0) )
		{
			*tailleSLIDE = atoi(argv[i + 1]);
		}
		
		if(strcmp(argv[i], "-s") == 0)
		{
			fichierFASTA = i + 1;
		}
		
		if( (strcmp(argv[i], "-parameter") == 0) || (strcmp(argv[i], "-par") == 0) || (strcmp(argv[i], "-PARAMETER") == 0) )
		{
			fichierDATA = i + 1;
		}
	}
	
	if(fichierFASTA != 0)
	{
		std::ifstream::int_type posSize;
		std::ifstream file (argv[fichierFASTA], std::ios::in|std::ios::binary|std::ios::ate);
		posSize = file.tellg();		// je recupere le nombre de caracteres dans le fichier de sequence
		*tailleFASTA = posSize;
		
		// recupere le nombre de sequence
		char *memblock = new char[posSize+1];
		memblock[posSize] = '\0';
		//cout<<posSize<<endl;
		file.seekg(0, std::ios::beg);				// positionnement au debut du fichier
		file.read(memblock, posSize);				// mise en memoire du texte
		memblock[posSize] = '\0';
		//cout<<posSize<<endl;	
		file.close();
		int i = 0;
		while(memblock[i] != '\0')				// lecture du chromosome sans la premiere ligne fasta
		{
			if( (memblock[i] == '>') )
			{
				nbSeq++;
			}
			i++;
		}
		delete memblock;
		*nbSequence = nbSeq;
	}
	
	if(fichierDATA != 0)
	{
		std::ifstream::pos_type posSize;
		std::ifstream file (argv[fichierDATA], std::ios::in|std::ios::binary|std::ios::ate);
		posSize = file.tellg();		// je recupere le nombre de caracteres dans le fichier de sequence
		*tailleDATA = posSize;
	}
	else
	{
		std::ifstream::pos_type posSize;
		std::ifstream file ("DATA/DEFAULT.DAT", std::ios::in|std::ios::binary|std::ios::ate);
		posSize = file.tellg();		// je recupere le nombre de caracteres dans le fichier de sequence
		*tailleDATA = posSize;
	}
	
	return fichierFASTA;
}


/*************************************************************************************************************************************************/
int main(int argc, char *argv[])
{
	// je recupere d'abord les variables vitales pour la memoire du programme
	long tailleFASTA = 0;
	int tailleDATA = 0;
	int tailleSLIDE = 0;
	int nbSequence = 0;
	int posARGV = recupVITALmemoire(argc, argv, &tailleFASTA, &tailleDATA, &tailleSLIDE, &nbSequence);

	
	
	// mise en memoire de toutes les structures de donnees !
	// creation de la matrice de la fenetre 
	Matrice *matrixPalindromeT = NULL;
	matrixPalindromeT = CreateMatrix(tailleSLIDE);
		
	// structure qui va contenir les graines des hairpins
	Palindrome *PalminiList = NULL;
	PalminiList = new Palindrome[11];
	
	// structure qui memorise les palindromes non exactes 
	int nbPalindromeT = 0;
	Palindrome *listePalT = NULL;
	listePalT = new Palindrome [(tailleSLIDE) * (tailleSLIDE) + 1];

	// structure qui va contenir les palindromes necessaire pour la construction de l'hairpin finale
	PalindromeSimple *PalHairpin = NULL;
	PalHairpin = new PalindromeSimple[(tailleSLIDE) * (tailleSLIDE) + 1];
	
	// structure des l'hairpin finales
	Hairpin *resultatH = NULL;
	resultatH = new Hairpin[10 * tailleSLIDE];
	if(resultatH == NULL)
	{
		cout << "Allocation Memory resultatH Impossible ! \n";
		exit(1);
	}
	for(int i=0; i<(10 * tailleSLIDE); i++)
	{
		resultatH[i].structure = new char[tailleSLIDE+1];
		if(resultatH[i].structure == NULL)
		{
			cout << "Allocation Memory resultatH[i].structure Impossible ! \n";
			exit(1);
		}
		resultatH[i].texts = new char[tailleSLIDE+1];
		if(resultatH[i].texts == NULL)
		{
			cout << "Allocation Memory resultatH[i].texts Impossible ! \n";
			exit(1);
		}
	}
	
	// structure qui memorise toutes les variables necessaires pour le programme
	Parameters *arguments = NULL;
	arguments = new Parameters[1];
	initParameters(arguments, tailleSLIDE, tailleFASTA, tailleDATA, nbSequence);
	
	
	
	// mise en memoire des parametres du programme
	RecupParametres(argc, argv, arguments);
	int limitHairpin = 0;
	if(arguments->stringence != 0)
		limitHairpin = 1;
	arguments->tailleMax = tailleSLIDE;
	

	
	for(int i=0; i<arguments->nbSequence; i++)
	{
		int tailleSequence = strlen(arguments->text[i]);
		
		InitialisePalindrome(PalminiList, 11);
		InitialiseMatrice(matrixPalindromeT, arguments->text[i], tailleSequence, 0);

			
		
		// je calcule maintenant tous les palindromes present dans la fenetre et de taille min Lmin
		RecherchePalindrome(matrixPalindromeT, PalminiList, 0, tailleSLIDE, arguments, 0, limitHairpin);
		
		

		for(int k=1; k<11; k++)
		{
			RemoveGraineAndPalNE(listePalT, nbPalindromeT, PalminiList, k);
	
			if( PalminiList[k].vraiDeb > 0 )
			{
				RecopiePal(PalminiList, k, listePalT, nbPalindromeT);
				nbPalindromeT++;
			}
		}

		if(nbPalindromeT > 0)
		{
			for(int k=0; k<nbPalindromeT; k++)
			{
				listePalT[k].choisi = 1;
				if(listePalT[k].choisi == 1)
				{
					int NbPal = 0;
					int graine = ConstruitPalindrome(listePalT, nbPalindromeT, arguments, matrixPalindromeT, PalHairpin, &NbPal, k, limitHairpin, i);
					if(graine != -1)
					{
						int appariNonExact = listePalT[k].hairpinTaille - listePalT[k].hairpinMesap;
						int premierPal = AssemblePalindrome(PalHairpin, NbPal, graine, arguments, resultatH);
						CreeHairpin(PalHairpin, NbPal, graine, premierPal, resultatH, arguments, appariNonExact, listePalT, k, i);
					}
				}
			}
		}

		for(int y=0; y<nbPalindromeT; y++)
			Init1Palindrome(listePalT, y);

		nbPalindromeT = 0;
		
		
		
		int indiceHairpin = 0;
		float meilleurDG = resultatH[0].deltaG;
		for(int k=1; k<arguments->nbTige; k++)
		{
			if( (float_compare(resultatH[k].deltaG, meilleurDG) != 0) && (resultatH[k].deltaG > meilleurDG) )
				resultatH[k].positionDEB = -1;
			else
			{
				meilleurDG = resultatH[k].deltaG;
				resultatH[indiceHairpin].positionDEB = -1;
				indiceHairpin = k;
			}
		}
		for(int k=0; k<arguments->nbTige; k++)
		{
			if(resultatH[k].positionDEB != -1) 
				AfficheToutesValeurs(resultatH, arguments, k, argv[posARGV], i);
		}
		arguments->nbTige = 0;
	}



	delete resultatH;
	delete PalHairpin;
	delete listePalT;
	delete PalminiList;
	ClearMatrix(matrixPalindromeT);
	
	delete arguments->text;
	delete arguments;

}

