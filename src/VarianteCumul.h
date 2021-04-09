#include "Jeu.h"

class VarianteCumul : public Jeu
{
private:
		
		unsigned int cumulCarteAPiocher;

	 

public:
		/**
		* @brief Vérifie si la carte qui va être posé est valide ou non.
		* 
		* @return true 
		* @return false 
		*/
		bool carteValide(const Carte) const;

		/**
		* @brief 
		* Le joueur met une carte dans le Talon, la carte posée est retirée de la main, l'effet est appliqué.
		* 
		* @param indiceCarte : l'indice de la carte que le joueur veut poser.
		* @param messageErreur : Message d'erreur à afficher avec l'interface Graphique.
		*/
		void poserCarte(unsigned int& indiceCarte, string &messageErreur);

		


		
				
};

#endif
