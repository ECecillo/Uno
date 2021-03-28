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

// insère une carte dans tableJoueur pour version txt
void Joueur::insererCartePositionIJ(unsigned int indi, unsigned int indj, Carte & c)
{
    for (unsigned i = indi; i < indi+8; i++)
        for (unsigned int j = indj; j < indj+11; j++) tableJoueur[i][j] = c.carte[i-indi][j-indj];
}

// insère une carte adversaire, sans le nombre de cartes, dans tableJoueur pour version txt
void Joueur::insererCarteAdversairePositionJ(unsigned int indj, unsigned int num)
{
    tableJoueur[0][indj] = 'j';
    tableJoueur[0][indj+1] = 'o';
    tableJoueur[0][indj+2] = 'u';
    tableJoueur[0][indj+3] = 'e';
    tableJoueur[0][indj+4] = 'u';
    tableJoueur[0][indj+5] = 'r';
    tableJoueur[0][indj+7] = '0'+num;
    //contour de la carte
    for (unsigned int i=1; i<9; i++)
        for (unsigned int j = indj; j < indj+11; j++) tableJoueur[i][j] = carteVierge[i-1][j-indj];
    tableJoueur[5][indj+2] = 'c';
    tableJoueur[5][indj+3] = 'a';
    tableJoueur[5][indj+4] = 'r';
    tableJoueur[5][indj+5] = 't';
    tableJoueur[5][indj+6] = 'e';
    tableJoueur[5][indj+7] = 's';
}

// met des " " dans tableJoueur dans toute la zone de la main en bas pour la version txt
void Joueur::effacerMainTxt()
{
    for (unsigned int i = 22; i<haut; i++)
        for (unsigned int j = 0; j<larg; j++)
            tableJoueur[i][j] = " ";
}

// dessine toutes les cartes de la main dans tableJoueur pour la version txt
void Joueur::dessinerMainTxt()
{
    for (unsigned int i = 0; i<main.size(); i++)
        insererCartePositionIJ(22+9*(size/15),12*(i%15),main[i]);
}

// pour la version txt
void Joueur::modifMainTxt()
{
    effacerMainTxt();
    dessinerMainTxt();
}

// gagnant si la main est vide
bool Joueur::gagnant ()
{
    return main.empty();
}

// récupère la largeur pour version txt
unsigned int Joueur::getLarg () const { return larg; }

// récupère la hauteur pour la version txt
unsigned int Joueur::getHaut () const {	return haut; }

