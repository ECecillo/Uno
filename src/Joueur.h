#ifndef JOUEUR_H
#define JOUEUR_H

#include <string>
#include <vector>
#include "Carte.h"

using namespace std;

#include <string.h>
#include <Carte.h>
#include <iostream>
class Joueur
{
private:

	 
public:
		string nom;
		vector<Carte> main;		
		unsigned int numeroJoueur;

		
		/**
		* @brief 
		* Constructeur par défaut. Initialise numeroJoueur à 1 et nom à Joueur 1.
		* 
		*/
		Joueur();
		
		/**
		* @brief 
		* Constructeur. Affecte num à numeroJoueur et n à nom.
		* 
		* @param
		* [in] entier naturel inférieur ou égal au nombre de joueurs
		* [in] chaîne de caractères
		*/
		Joueur (const unsigned int num, const string n);
		
		/**
		* @brief 
		* Destructeur.
		* 
		*/
		~Joueur();


		/**
		* @brief 
		* Détermine si le joueur est gagnant.
		* 
		* @return
		* Vrai si la main du joueur est vide
		*/
		bool gagnant();



		
				
};

#endif
