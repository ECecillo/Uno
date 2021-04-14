#ifndef TOURNE_H
#define TOURNE_H

#include "Jeu.h"


class VarianteTourne : public Jeu
{
private:
		
		

	 

public:
		/**
		* @brief
		* Constructeur par défaut
		*/
		VarianteTourne();

		/**
		* @brief
		* Constructeur, avec le tableau de joueurs
		* 
		* @param 
		* nombre de joueurs 
		* @param
		* nombre d'IA
		*/
		VarianteTourne(const unsigned int nbjoueurs, const unsigned int nbIA);
		
		/**
		* @brief
		* Constructeur par défaut
		*/
		~VarianteTourne();

		/**
		* @brief 
		* Le joueur met une carte dans le Talon, la carte posée est retirée de la main, l'effet est appliqué.
		* @param indiceCarte : l'indice de la carte que le joueur veut poser.
		* @param messageErreur : Message d'erreur à afficher avec l'interface Graphique.
		*/
		void poserCarte(unsigned int& indiceCarte, string &messageErreur);

		/**
		* @brief 
		* Fait tourner les mains d'un joueur à l'autre dans le sens du jeu.
		*/
		void tournerMains();


		
				
};

#endif
