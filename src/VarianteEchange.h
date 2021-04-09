#include "Jeu.h"


class VarianteEchange : public Jeu
{
private:
		
		

	 

public:
		
		/**
		* @brief 
		* Le joueur met une carte dans le Talon, la carte posée est retirée de la main, l'effet est appliqué.
		* 
		* @param indiceCarte : l'indice de la carte que le joueur veut poser.
		* @param messageErreur : Message d'erreur à afficher avec l'interface Graphique.
		*/
		void poserCarte(unsigned int& indiceCarte, string &messageErreur);

		/**
		* @brief 
		* Echange la main du joueur avec celle du joueur dont le numéro est i.
		*
		* @param entier naturel: numéro du joueur avec qui échanger la main.
		*/
		void echangeMain(unsigned int i);


		
				
};

#endif
