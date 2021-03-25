#include "Carte.h"
#include <iostream>
#include <cassert>

using namespace std;

Carte::Carte () 
{
    valeur=0;
    couleur=0;
}

Carte::Carte (const unsigned int v, const unsigned int c) 
{
    assert(v<=14);
    assert(c>=1 && c<=4);
    valeur = v;
    couleur = c;
}

Carte::~Carte () {}

unsigned int Carte::getValeur () const 
{
    return valeur;
}

unsigned int Carte::getCouleur () const 
{
    return couleur;
}

/* bool Carte::operator < (const Carte c) const
{
    return couleur <    c.couleur| (couleur == c.couleur && valeur<c.valeur);
} */

