#ifndef ECHANGE_H
#define ECHANGE_H

#include "Jeu.h"


class VarianteEchange : public Jeu
{
private:
		
		

	 

public:
		/**
		* @brief
		* Constructeur par défaut
		*/
		VarianteEchange();

		/**
		* @brief
		* Constructeur, avec le tableau de joueurs
		* 
		* @param 
		* nombre de joueurs 
		* @param
		* nombre d'IA
		*/
		VarianteEchange(const unsigned int nbjoueurs, const unsigned int nbIA);
		
		/**
		* @brief
		* Constructeur par défaut
		*/
		~VarianteEchange();

		/**
		* @brief 
		* Le joueur met une carte dans le Talon, la carte posée est retirée de la main, l'effet est appliqué.
		* @param indiceCarte : l'indice de la carte que le joueur veut poser.
		* @param messageErreur : Message d'erreur à afficher avec l'interface Graphique.
		*/
		void poserCarte(const unsigned int& indiceCarte, string &messageErreur) override;


		
				
};

#endif
