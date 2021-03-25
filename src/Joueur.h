#ifndef JOUEUR_H
#define JOUEUR_H


#include <string.h>
#include <Carte.h>
#include <iostream>
class Joueur
{
private:
		char nom;
		Carte * main;

public:
		/**
		* @brief Construit une nouvelle carte.
		* 
		*/
		Joueur();

		
		~Joueur();


		void gagnant();


		void afficherMainJoueur();

		
				
};

#endif
