#include "Jeu.h"


class VarianteDoublon : public Jeu
{
private:
		
		

	 

public:
		/**
		* @brief 
		* Teste si il y a dans la main du joueur la même carte qu'il vient jouer.
		* @return 
		* entier naturel: indice du doublon dans la main, -1 sinon
		*/
		int testDoublon();


		/**
		* @brief 
		* Le joueur met une carte dans le Talon, la carte posée est retirée de la main, l'effet est appliqué.
		* @param indiceCarte : l'indice de la carte que le joueur veut poser.
		* @param messageErreur : Message d'erreur à afficher avec l'interface Graphique.
		*/
		void poserCarte(unsigned int& indiceCarte, string &messageErreur);


		
				
};

#endif
