#ifndef AFFICHAGE_TXT
#define AFFICHAGE_TXT

#include <vector>
#include <string>
#include <Jeu.h>
#include "Fenetre.h"
#include "SalleAttente.h"

using namespace std;

		/**
		* @brief 
		* Affiche la table de jeu.
		*
		*/
		void txtAff(Fenetre &win, const Jeu &jeu);

		/**
		* @brief 
		* Affiche la salle d'attente.
		*
		*/
		void txtAffSalleAttente(Fenetre & win, const SalleAttente & s);

		/**
		* @brief 
		* Boucle à partir de la salle d'attente.
		*
		*/
		void txtBoucleDebut(SalleAttente & s);


		/**
		* @brief 
		* Boucle de jeu.
		*
		*/
		void txtBoucle(Jeu & jeu);

#endif
