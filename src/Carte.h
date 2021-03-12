#ifndef CARTE_H
#define CARTE_H

#include <string>
#include <set>

using namespace std;

const set <string> ValeursCartes = {"0","1","2","3","4","5","6","7","8","9","inverse","passe","+2","joker","super joker"};
const set <string> CouleursCartes = {"rouge","bleu","vert","jaune"};


/**
* @brief
* Données membres: valeur et couleur.
* 
*/
class Carte
{
private:
		string valeur,couleur;

	 

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
		* [in] string valeur
		* [in] string couleur
		*/
		Carte(const string v,const string c);

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
		string getValeur () const;

		/**
		* @brief 
		* Accesseur: récupère la couleur de la carte.
		* 
		* @return
		* Chaîne de caractères
		*/
		string getCouleur () const;


		
				
};

#endif
