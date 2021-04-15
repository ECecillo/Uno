#include "AffichageTxt.h"
#include "Fenetre.h"
#include <iostream>

using namespace std;

int main ( int argc, char** argv ) {
    termClear();
	Jeu jeu(2,1);
	//jeu.testRegression();
    //jeu.joueursBot[0].choixJeu(jeu);
    jeu.joueursBot[0].testRegression(jeu);
    //txtBoucle(jeu);
    //termClear();
	return 0;
}
