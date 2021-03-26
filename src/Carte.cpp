#include "Carte.h"
#include <iostream>
#include <cassert>

using namespace std;

Carte::Carte()
{
    valeur = 0;
    couleur = 0;
}

Carte::Carte(const unsigned int v, const unsigned int c)
{
    assert(v <= 14);
    assert(c >= 1 && c <= 4);
    valeur = v;
    couleur = c;
}

Carte::~Carte() {}

unsigned int Carte::getValeur() const
{
    return valeur;
}

unsigned int Carte::getCouleur() const
{
    return couleur;
}

friend bool operator < (const Carte& c, const Carte& c2) const
{
    return (c.couleur < c2.couleur) || (c.couleur == c2.couleur && c.valeur < c2.valeur);
}

friend bool operator == (const Carte& c, const Carte& c2) const
{
    return (c.couleur == c2.couleur) && (c.valeur == c2.valeur);
}