#include "AffichageTxt.h"
#include "AffichageSDL.h"
#include "Fenetre.h"
#include "SalleAttente.h"
#include "Carte.h"
#include "VarianteDoublon.h"
#include "VarianteEchange.h"
#include "VarianteTourne.h"
#include "VarianteSuite.h"
#include "VarianteCumul.h"
#include <iostream>

using namespace std;

int main ( int argc, char** argv ) {
    
    
    sdlJeu partie;
    /*partie.sdlBoucleJeu(1,4,0);*/
    partie.sdlAffSalleAttente();

    
	return 0;
}