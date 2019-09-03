#ifndef _gFold_H_
#define _gFold_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <float.h>
#include <cmath>

using namespace std;

typedef struct
{
	int nbSequence;
	int mirbase;			// affichage version MiRBase
	long tailleReelle;		// taille reelle du fichier FASTA
	int optionTOUT;			// affiche toutes les valeurs de l'hairpin
	int optionEValue;		// affiche le pourcentage de bon parametres de chaque etape
	int nbTige;
	int slidingStep;
	int addPos;			// ajoute addPos aux resultats
	int tailleSeed;			// taille minimale de la graine de l'hairpin
	int tailleMin;			// taille minimale de l'hairpin
	int tailleMax;			// taille maximale de l'hairpin (la taille de la fenetre)
	int optionAffichage;		// affichage ou non des DNA palindromes et non-exact DNA palindrome
	int optionText;			// affiche ou non le texte de l'hairpin en plus de la version brackets
	int noHairpin;			// ne donne que les palindromes imparfaits
	int stringence;			// percentage de parametres bon pour aller a l'etape suivante
	float incertitude;		// % entre le meilleur palindrome et le plus mauvais choisi
	float deltaGMax;		// score du deltaG max pour une hairpin
	int *tailleSeq;
	char output[250];		// fichier de resultats
	char **nameSeq;
	char **text;			// texte du fichier d'entree
	char *fichierDATA;		// fichier qui contient les parametres
} Parameters;



typedef struct
{
	int taille;
	char *sequence;
	int **tab;
} Matrice;



typedef struct 
{
	int vraiDeb;			// position absolue (dans le texte) du debut du palindrome
	int vraiFin;			// position absolue de la findu palindrome
	int longueur;			// longueur du palindrome
	int distance;			// distance entre les deux parties du palindromes
	int choisi;			// si le palindrome est selectionne pour une hairpin
	int posInter;			// position du centre du palindrome
	
	int hairpinDeb;
	int hairpinFin;
	int hairpinTaille;
	int hairpinDIST;
	int hairpinMesap;
	int hairpinPal;
	int hairpinMAXloop;
	float hairpinDG;
	float MoyennePal;
	float ratioMatchGU;
	float basePairing;
	
	float ratioP;
	float ratioNE;
	
	float ratioA;			// pourcentage de A
	float ratioC;			// pourcentage de C
	float ratioG;			// pourcentage de G
	float ratioU;			// pourcentage de U
	float score;			// score theromodynamique du palindrome
} Palindrome;



typedef struct 
{
	int vraiDeb;
	int vraiFin;
	int longueur;
	int distance;
	int hairpinORnot;
	int indiceSuivant;
	int indicePrecedent;
	float score;

	int choisi;
	int mesappariement;
	int appariement;
	int nbAsso;
	int association[20];
} PalindromeSimple;





typedef struct
{
	int SeedDEB;
	int SeedFIN;
	int SeedLG;
	int SeedDist;
	float Seedscore;
	int neDEB;
	int neFIN;
	int neTAILLE;
	int neDIST;
	int neMesap;
	int neNBPal;
	float neDG;
	int neMAXloop;
	
	
	int afficher;
	float ratioH;
	float ratioP;
	float ratioNE;
	

	
	int positionDEB;
	int positionFIN;
	int appariement;
	float appariementH;
	float deltaG;
	float MFEajusted;
	int boucle;
	int nbPal;
	int hairpinMAXloop;
	float PalMoy;
	int bulgeDiff;
	int bulgeMax;
	int nbBG;
	int nbBD;
	int differenceGaucheDroite;
	int suiteBulge;
	int suiteBcote;
	int pairGU;
	float GUappariement;
	int pairGC;
	float GCappariement;
	int pairAU;
	float AUappariement;
	float appMFE;
	int strGAUCHE[50];
	int strDROITE[50];
	int strTAILLE[50];
	char *structure;
	char *texts;
} Hairpin;


/*************************************************************************************************************************************************/

void PrintInterval(char *, int, int);
void PrintMatrix(Matrice *);
void PrintPalindrome(Palindrome *, int , Parameters *);
void PrinListePalindrome(Palindrome *, int, Parameters *);
void PrintPalImparfait(Palindrome *, int, Parameters *);
void AffichePalindromeImparfait(Palindrome *, int, Parameters *);
void AfficheToutesValeurs(Hairpin *, Parameters *, int, char *, int);

void PrinListePalindromeARN(PalindromeSimple *, int, Parameters *);
void PrintPalindromeARN(PalindromeSimple *, int, char *);
void printListePotentiel(PalindromeSimple *, int, char *);
void AfficheHairpin(Hairpin *, Parameters *, int, int);

/*************************************************************************************************************************************************/

Matrice *CreateMatrix(int);
void ClearMatrix(Matrice *);
void InitialiseMatrice(Matrice *, char *, int, int);
void RecherchePalindrome(Matrice *, Palindrome *, int, int, Parameters *, int, int);
int RecherchePalindromeARN(Matrice *, int, Parameters *, PalindromeSimple *, int *, int, int, int, int, int);

void PrintMatrix(Matrice *);

/*************************************************************************************************************************************************/

float DeltaG1Base(char, char);
float BoucleTerminal(int);
float InternalLoop(int, int);

float Palindrome1BaseRNA(int, int, char *);
float DeltaG_simplePalindromeRNA(int, int, int, char *);
float DeltaG_Hairpin_Intern(int, int, PalindromeSimple *);
float DeltaG_Hairpin_Extern(int, int, PalindromeSimple *);
float ThermoHairpin(PalindromeSimple *, int, int);

/*************************************************************************************************************************************************/

void InitialisePalindrome(Palindrome *, int);
void Init1Palindrome(Palindrome *, int);
void NouveauPal(Palindrome *, int, int, int, int, float);
void RecopiePal(Palindrome *, int, Palindrome *, int);
int PalIndentique(Palindrome *, int, Palindrome *, int);

void NouveauPalRNA(PalindromeSimple *, int, int, int, int, float);
void EffacePalindromeRNA(PalindromeSimple *, int);
int ElimineInutile(Palindrome *, int, Palindrome *);
void OrdonneListeHairpin(Palindrome *, int, Palindrome *);
int RemoveDoublons(Palindrome *, int, Palindrome *);

/*************************************************************************************************************************************************/

int SelectionnePalindrome(Palindrome *, Parameters *);
void RemoveGraineAndPalNE(Palindrome *, int, Palindrome *, int);
void ElimineGraine(Palindrome *, int, Parameters *);
void ElimineNEPal(Palindrome *, int, Parameters *);

/*************************************************************************************************************************************************/

void GetParameters(Parameters *, char *);

/*************************************************************************************************************************************************/

void SelectionnePotentiel(PalindromeSimple *, int, Parameters *, float, int);
float ChoisiInterieurDeltaG(PalindromeSimple *, int);
int AppariementInterne(PalindromeSimple *, int);
int MesappariementInterne(PalindromeSimple *, int);
float ChoisiExterieurDeltaG(PalindromeSimple *, int);
void TracePremierPal(PalindromeSimple *, int);
int PremierPalindromeRNA(PalindromeSimple *, int);

/*************************************************************************************************************************************************/

int MeilleurPalindromeInterieur(PalindromeSimple *, int, int, float *, int);
void AlongeHairpinInterieur(PalindromeSimple *, int, Parameters *, int, int);
void RemplitVideInterne(PalindromeSimple *, int, int, Parameters *);
void miniPalindromeBoucle(PalindromeSimple *, int, int);

int MeilleurPalindromeExterieur(PalindromeSimple *, int, int, float *);
void AlongeHairpinExterieur(PalindromeSimple *, int, Parameters *, int);
void RemplitVideExterne(PalindromeSimple *, int, int, int);

int EspaceEntrePalindrome(PalindromeSimple *, int, int, int);

/*************************************************************************************************************************************************/

int ConstruitPalindrome(Palindrome *, int, Parameters *, Matrice *, PalindromeSimple *, int *, int, int, int);
int AssemblePalindrome(PalindromeSimple *, int, int, Parameters *, Hairpin *);

/*************************************************************************************************************************************************/
void initHairpin(Hairpin *, Parameters *, int);
void copytHairpin(Hairpin *, Parameters *, int, int);
void CreeHairpin(PalindromeSimple *, int, int, int, Hairpin *, Parameters *, int, Palindrome *, int, int);
void RemoveTigeDouble(Hairpin *, Parameters *, int);

/*************************************************************************************************************************************************/

void EcrirePalindromeImparfait(Palindrome *, int, Parameters *);
void EcrireHairpin(Hairpin *, Parameters *, int, int);

/*************************************************************************************************************************************************/

int float_compare(float, float);
long ustrlen(char *);
void GetFileContents(char *, Parameters *);
void usage(char *);
void RecupParametres(int, char *, Parameters *);
void initParameters(Parameters *, int, int, int);
int recupVITALmemoire(int, char *, long *, int *, int *, int *);

/*************************************************************************************************************************************************/

#endif
