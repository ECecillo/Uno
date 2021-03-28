#include "Joueur.h"
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

const int largeur = 180;

const int hauteur = 38;

const char tableVierge[hauteur][largeur+1] = {
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                     ###########                    ###########                                                                     ",
 "                                                                     #         #                    #         #                                                                     ",
 "                                             ############            #  Pioche #    ############    #  Talon  #            ############                                             ",
 "                                             #   UNO    #            #         #    #  Passer  #    #         #            #Contre UNO#                                             ",
 "                                             # touche U #            #         #    # touche P #    #         #            # touche C #                                             ",
 "                                             ############            #  cartes #    ############    #         #            ############                                             ",
 "                                                                     #         #                    #         #                                                                     ",
 "                                                                     ###########                    ###########                                                                     ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    ",
 "                                                                                                                                                                                    "
};

Joueur::Joueur () 
{
    numeroJoueur = 1;
    nom = "";
    larg = largeur;
    haut = hauteur;
    //initialisation de la table du Joueur à partir de la table vierge
    for(unsigned int x=0;x<haut;x++)
		for(unsigned int y=0;y<larg;y++)
			tableJoueur[x][y] = tableVierge[x][y];

	
} 


Joueur::Joueur (const unsigned int num, const string n) 
{
    numeroJoueur = num;
    nom=n;
    larg = largeur;
    haut = hauteur;
    //initialisation de la table du Joueur à partir de la table vierge
	for(unsigned int x=0;x<haut;x++)
		for(unsigned int y=0;y<larg;y++)
			tableJoueur[x][y] = tableVierge[x][y];
}

Joueur::~Joueur () {}

void Joueur::insererCartePositionIJ(unsigned int indi, unsigned int indj, Carte & c)
{
    for (unsigned i = indi; i < indi+8; i++)
        for (unsigned int j = indj; j < indj+11; j++) tableJoueur[i][j] = c.carte[i-indi][j-indj];
}

void Joueur::insererCarteAdversairePositionJ(unsigned int indj, unsigned int num)
{
    tableJoueur[0][indj] = 'j';
    tableJoueur[0][indj+1] = 'o';
    tableJoueur[0][indj+2] = 'u';
    tableJoueur[0][indj+3] = 'e';
    tableJoueur[0][indj+4] = 'u';
    tableJoueur[0][indj+5] = 'r';
    tableJoueur[0][indj+7] = '0'+num;
    for (unsigned int i=1; i<9; i++)
        for (unsigned int j = indj; j < indj+11; j++) tableJoueur[i][j] = carteVierge[i-1][j-indj];
    tableJoueur[5][indj+2] = 'c';
    tableJoueur[5][indj+3] = 'a';
    tableJoueur[5][indj+4] = 'r';
    tableJoueur[5][indj+5] = 't';
    tableJoueur[5][indj+6] = 'e';
    tableJoueur[5][indj+7] = 's';
}

void Joueur::effacerMain()
{
    for (unsigned int i = 22; i<haut; i++)
        for (unsigned int j = 0; j<larg; j++)
            tableJoueur[i][j] = " ";
}

void Joueur::dessinerMain()
{
    for (unsigned int i = 0; i<main.size(); i++)
        insererCartePositionIJ(22+9*(size/15),12*(i%15),main[i]);
}

bool Joueur::gagnant ()
{
    return main.empty();
}

unsigned int Joueur::getLarg () const { return larg; }

unsigned int Joueur::getHaut () const {	return haut; }

