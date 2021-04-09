#include "Jeu.h"


class VarianteTourne : public Jeu
{
private:
		
		

	 

public:
		
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
