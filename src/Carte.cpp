#include "Carte.h"
#include <iostream>
#include <cassert>

using namespace std;

Carte::Carte () 
{
    valeur="";
    couleur="";
}

Carte::Carte (const string v, const string c) 
{
    assert(CouleursCartes.find(c)!=CouleursCartes.end());
    assert(ValeursCartes.find(v)!=ValeursCartes.end());
    valeur = v;
    couleur = c;
}

Carte::~Carte () {}

string Carte::getValeur () const 
{
    return valeur;
}

string Carte::getCouleur () const 
{
    return couleur;
}

