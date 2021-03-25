#ifndef CARTE_H
#define CARTE_H



/**
* @brief
* Données membres: valeur et couleur.
* 
*/
class Carte
{
private:
		unsigned int valeur;	// entre 0 et 14 (10 inverse, 11 passe, 12 +2, 13 +4, 14 jocker)
		unsigned int couleur;	// entre 1 et 4 (1 rouge, 2 vert, 3 bleu, 4 jaune)

	 

public:
		/**
		* @brief 
		* Constructeur par défaut.
		* 
		*/
		Carte();


		/**
		* @brief 
		* Constructeur avec valeur et couleur.
		* 
		* @param 
		* [in] valeur entier naturel entre 0 et 14 
		* [in] couleur entier naturel entre 1 et 4
		*/
		Carte(const unsigned int v,const unsigned int c);

		/**
		* @brief 
		* Destructeur.
		* 
		*/
		~Carte();

		/**
		* @brief 
		* Accesseur: récupère la valeur de la carte.
		* 
		* @return
		* Entier naturel (10 pour inverse, 11 pour passe, 12 pour +2, 13 pour +4, 14 pour jocker)
		*/
		unsigned int getValeur () const;

		/**
		* @brief 
		* Accesseur: récupère la couleur de la carte.
		* 
		* @return
		* Entier naturel (1 pour rouge, 2 pour vert, 3 pour bleu, 4 pour jaune)
		*/
		unsigned int getCouleur () const;

		
		/**
		* @brief 
		* Opérateur <.
		* 
		* @return
		* Vrai si la couleur de la première carte est inférieure à celle de la deuxième carte ou si les couleurs sont identiques et la valeur de la première carte est inférieure à celle de la deuxième carte
		*/
		friend bool operator < (const Carte& c, const Carte& c2);


		/**
		* @brief 
		* Opérateur ==.
		* 
		* @return
		* Vrai si les couleurs et les valeurs des 2 cartes sont identiques
		*/
		friend bool operator == (const Carte& c, const Carte& c2);



		
				
};

#endif
