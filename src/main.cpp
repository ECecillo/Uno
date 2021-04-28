#include "AffichageTxt.h"
#include "Fenetre.h"
#include <SalleAttente.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    termClear();

    //Jeu jeu(2,1);
    //jeu.testRegression();
    //jeu.joueursBot[0].choixJeu(jeu);
    //cout << "Le nombre de joueur est : " << jeu.nombreIA + jeu.nombreJoueurs << endl;
    //jeu.joueurActif = 2;
    //cout << "Le joueur actif est : " << jeu.joueurActif << endl;
    //jeu.joueursBot[0].testRegression(jeu);
    //txtBoucle(jeu);
    SalleAttente Attente;
    int choixJeu = txtBoucleDebut(Attente);
    cout << "Fin de txtBoucleDebut" << endl;
    sleep(1);
    switch (choixJeu)
    {
    case 1:
    {
        Jeu jeu(Attente.nombreJoueurs, Attente.nombreIA);
        txtBoucle(jeu);
    }
    case 2:
    {
        VarianteCumul jeu(Attente.nombreJoueurs, Attente.nombreIA);
        txtBoucle(jeu);
        break;
    }
    case 3:
    {
        VarianteDoublon jeu(Attente.nombreJoueurs, Attente.nombreIA);
        txtBoucle(jeu);
        break;
    }
    case 4:
    {
        VarianteEchange jeu(Attente.nombreJoueurs, Attente.nombreIA);
        txtBoucle(jeu);
        break;
    }
    case 5:
    {
        VarianteSuite jeu(Attente.nombreJoueurs, Attente.nombreIA);
        txtBoucle(jeu);
        break;
    }
    case 6:
    {
        VarianteTourne jeu(Attente.nombreJoueurs,Attente.nombreIA);
        txtBoucle(jeu);
        break;
    }
    default:
        break;
    }

    //txtBoucle(jeu);
    termClear();
    return 0;
}
