#include "Joueur.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <iterator>

using namespace std;

Joueur::Joueur () 
{
    numeroJoueur = 1;
    nom = "";
} 


Joueur::Joueur (const unsigned int num, const string n) 
{
    numeroJoueur = num;
    nom=n;
}

Joueur::~Joueur () {}

bool Joueur::gagnant ()
{
    return main.empty();
}

