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
<<<<<<< HEAD
=======
		int dimx;
		int dimy;
		
	 
>>>>>>> 2382532ccd651d598fbf02e47544d997bee83e56

public:

		char carte [8][11];// dessin de la Carte en version txt
		

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
		* Chaîne de caractères
		*/
		unsigned int getValeur () const;

		/**
		* @brief 
		* Accesseur: récupère la couleur de la carte.
		* 
		* @return
		* Chaîne de caractères
		*/
		unsigned int getCouleur () const;

		/**
		* @brief 
		* Opérateur <.
		* 
		* @return
		* Vrai si la couleur et la valeur sont inférieures à celles de la carte passée en paramètre
		*/
		bool operator < (const Carte& c);

		/**
		* @brief 
		* Opérateur ==.
		* 
		* @return
		* Vrai si les couleurs et les valeurs des 2 cartes sont identiques
		*/
		bool operator == (const Carte& c);



		
				
};

#endif
