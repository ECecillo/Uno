#ifndef CARTE_H
#define CARTE_H


const char carteVierge[8][12] = {
 "###########",
 "#         #",
 "#         #",
 "#         #",
 "#         #",
 "#         #",
 "#         #",
 "###########"
};

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
		int dimx;
		int dimy;
		
	 

public:
		char carte [8][11];// dessin de la Carte en version txt

		/**
		 * @brief 
		 * Position du curseur pour indiquer dans quelle carte on est.
		 */
		unsigned int positionEtoile;		

		/**
		* @brief 
		* Constructeur par défaut.
		* 
		*/
		Carte();


		/**
		 * @brief Constructeur d'une carte à partir du patern carteVierge.
		 * 
		 * @param v : valeur de la carte.
		 * @param c : couleur de la carte.
		 * @param indiceEtoile : indice de l'étoile qui sert de curseur.
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
		* Mutateur: modifie la couleur de la carte.
		* 
		* @param
		* [in] entier naturel entre 1 et 4, pour la couleur
		*/
		void setCouleur(const unsigned int c);

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

		void insereEtoile();

		
				
};

#endif
