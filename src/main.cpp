#include "AffichageTxt.h"
#include "Terrain.h"
#include <iostream>

using namespace std;

int main ( int argc, char** argv ) {
    termClear();
	Jeu jeu(2,0);
	txtBoucle(jeu);
    termClear();
	return 0;
}
