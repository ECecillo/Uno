#ifndef AFFICHAGE_TXT
#define AFFICHAGE_TXT

#include <vector>
#include <string>
#include <Jeu.h>
#include "SalleAttente.h"
#include "Fenetre.h"
#include "SalleAttente.h"
#include "VarianteDoublon.h"
#include "VarianteEchange.h"
#include "VarianteTourne.h"
#include "VarianteSuite.h"
#include "VarianteCumul.h"

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
		int txtBoucleDebut(SalleAttente & s);


		/**
		* @brief 
		* Boucle de jeu.
		*
		*/
		void txtBoucle(Jeu & jeu);

#endif
