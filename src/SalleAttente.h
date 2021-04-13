#ifndef ATTENTE_H
#define ATTENTE_H


#include <string.h>
#include <iostream>

using namespace std;

/**
* @brief
* Classe gérant chaque joueur.
* 
*/
class SalleAttente
{
private:
		
			

	 

public:
		/**
		* @brief
		* Largeur de la fenêtre de la salle, pour l'affichage txt.
		* 
		*/
		unsigned int larg;

		/**
		* @brief
		* Hauteur de la fenêtre de la salle, pour l'affichage txt.
		* 
		*/
		unsigned int haut;

		unsigned int nombreJoueurs;
		unsigned int nombreIA;
		unsigned int variante;
		/**
		* @brief
		* Dessin de la table du Joueur pour l'affichage txt.
		* 
		*/
		char salle [20][80];// 

		/**
		* @brief
		* Position du curseur pour l'affichage txt.
		* 
		*/
		unsigned int indiceEtoile; 
				



		SalleAttente(); 


		~SalleAttente();
		
		void fenetreSalleRegles();

		void MaJFenetreSalle();

		void choixNombreJoueurs();

		void choixNombreIA();

		void choixJeu();
		
				
};

#endif
