#include "AffichageTxt.h"
#include "Fenetre.h"
#include <iostream>

using namespace std;

int main ( int argc, char** argv ) {
    termClear();
	Jeu jeu(1,1);
	//jeu.testRegression();
    //jeu.joueursBot[0].choixJeu(jeu);
    //cout << "Le nombre de joueur est : " << jeu.nombreIA + jeu.nombreJoueurs << endl;
    //jeu.joueurActif = 2;
    //cout << "Le joueur actif est : " << jeu.joueurActif << endl;
    //jeu.joueursBot[0].testRegression(jeu);
    txtBoucle(jeu);
    //termClear();
	return 0;
}
