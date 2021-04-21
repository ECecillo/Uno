#include "AffichageTxt.h"
#include "Fenetre.h"
#include <SalleAttente.h>
#include <iostream>

using namespace std;

int main ( int argc, char** argv ) {
    termClear();

	Jeu jeu(2,0);
	//jeu.testRegression();
    txtBoucle(jeu);
    termClear();
    //SalleAttente Attente;
    //txtBoucleDebut(Attente);
	return 0;

}
