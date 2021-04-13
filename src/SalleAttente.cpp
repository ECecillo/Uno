#include <iostream>
#include <cassert>
#include "SalleAttente.h"

using namespace std;

const int largeur = 77;

const int hauteur = 17;

const char salleVierge[hauteur][largeur + 1] = {
    "                    UNO                                                     ",
    "                                                                            ",
    "                Salle d'attente                                             ",
    "                                                                            ",
    "     #####################################                                  ",
    "     #                                   #                                  ",
    "     #       Jeu:                        #                                  ",
    "     #                                   #                                  ",
    "     #       Nombre de joueurs:          #                                  ",
    "     #                                   #                                  ",
    "     #       Nombre d'ordinateurs:       #                                  ",
    "     #                                   #         Choix du jeu             ",
    "     #                                   #         Nombre de joueurs        ",
    "     #                                   #         Nombre d'ordinateurs     ",
    "     #                                   #         Démarrer                 ",
    "     #####################################                                  ",
    "                                                                            ",
   };

   const char salleRegles[hauteur][largeur + 1] = {
    "                    UNO                                                     ",
    "                                                                            ",
    "                Salle d'attente                                             ",
    "                                                                            ",
    "     #####################################                                  ",
    "     #                                   #                                  ",
    "     #       1- Jeu classique            #                                  ",
    "     #       2- Variante Cumul           #                                  ",
    "     #       3- Variante Doublon         #                                  ",
    "     #       4- Variante Echange         #                                  ",
    "     #       5- Variante Suite           #                                  ",
    "     #       6- Variante Tourne          #         Choix du jeu             ",
    "     #                                   #         Nombre de joueurs        ",
    "     #       R pour voir les règles      #         Nombre d'ordinateurs     ",
    "     #                                   #         Démarrer                 ",
    "     #####################################                                  ",
    "                                                                            ",
   };

SalleAttente::SalleAttente()
{
    larg = largeur;
    haut = hauteur;
    nombreJoueurs = 2;
    nombreIA = 0;
    variante = 1;
    //initialisation de la salle à partir de la salle vierge
    for (unsigned int x = 0; x < haut; x++)
        for (unsigned int y = 0; y < larg; y++)
            salle[x][y] = salleVierge[x][y];
}

SalleAttente::~SalleAttente() {}

void SalleAttente::fenetreSalleRegles()
{
    for (unsigned int x = 0; x < haut; x++)
        for (unsigned int y = 0; y < larg; y++)
            salle[x][y] = salleRegles[x][y];
}