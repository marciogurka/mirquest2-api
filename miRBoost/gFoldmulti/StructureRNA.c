#include "gFold.h"


/*************************************************************************************************************************************************/
int ConstruitPalindrome(Palindrome *listePalT, int nbPalindromeT, Parameters *arguments, 
			Matrice *matrixPalindromeT, PalindromeSimple *PalHairpin, int *NbPalindrome, int k, int limitHairpin, int i)
{
	int tailleSequence = strlen(arguments->text[i]);
	// je recupere les palindromes de cette intervalle pour faire l'hairpin
	int graine = - 1;
	InitialiseMatrice(matrixPalindromeT, arguments->text[i], tailleSequence+1, 0);
	int nbPal = RecherchePalindromeARN(matrixPalindromeT, 0, arguments, PalHairpin, &graine, 
					   listePalT[k].vraiDeb, listePalT[k].vraiFin, listePalT[k].longueur, limitHairpin, 0);	
	
	*NbPalindrome = nbPal;
	return graine;
}

/*************************************************************************************************************************************************/
int AssemblePalindrome(PalindromeSimple *PalHairpin, int NbPalindrome, int graine, Parameters *arguments, Hairpin *resultatH)
{
	// je vais d'abord chercher les palindromes a l'interieur du candidat
	// le but est de diminuer la taille de la boucle !
	AlongeHairpinInterieur(PalHairpin, NbPalindrome, arguments, 0, graine);

	ChoisiInterieurDeltaG(PalHairpin, graine);
	PalHairpin[graine].choisi = 1;
	PalHairpin[graine].appariement = AppariementInterne(PalHairpin, graine);
	PalHairpin[graine].mesappariement = MesappariementInterne(PalHairpin, graine);

	// ici je remplis les vides laisses entre les grands palindromes internes
	RemplitVideInterne(PalHairpin, graine, NbPalindrome, arguments);
	miniPalindromeBoucle(PalHairpin, graine, NbPalindrome);





	// je recherche maintenant les palindromes a l'exterieur du candidat le but est d'allonger l'hairpin
	// ca ne sert a rien d'alonger plus dque necessaire !
	AlongeHairpinExterieur(PalHairpin, NbPalindrome, arguments, graine);
	
	ChoisiExterieurDeltaG(PalHairpin, graine);
	TracePremierPal(PalHairpin, graine);
	int pere = PremierPalindromeRNA(PalHairpin, graine);

	// ici je remplis les vides laisses entre les grands palindromes externes
	RemplitVideExterne(PalHairpin, pere, graine, NbPalindrome);

	return pere;
}
