#include "AffichageTxt.h"
#include "Fenetre.h"
#include <SalleAttente.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    termClear();

    Jeu jeu(2,1);
    jeu.testRegression();
    jeu.joueursBot[0].testRegression(jeu);
    
    termClear();
    return 0;
}
