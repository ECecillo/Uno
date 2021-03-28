#ifndef JOUEUR_H
#define JOUEUR_H

#include <string>
#include <vector>
#include "Carte.h"

using namespace std;

class Joueur
{
private:
		unsigned int larg;
		unsigned int haut;	 

public:
		string nom;
		vector<Carte> main;		
		unsigned int numeroJoueur;
		char tableJoueur [50][200];// dessin de la table du Joueur en version txt

		
		/**
		* @brief 
		* Constructeur par défaut. Initialise numeroJoueur à 1 et nom à Joueur 1.
		* 
		*/
		Joueur();
		
		/**
		* @brief 
		* Constructeur. Affecte num à numeroJoueur et n à nom.
		* 
		* @param
		* [in] entier naturel inférieur ou égal au nombre de joueurs
		* [in] chaîne de caractères
		*/
		Joueur (const unsigned int num, const string n);
		
		/**
		* @brief 
		* Destructeur.
		* 
		*/
		~Joueur();
		
		/**
		* @brief 
		* Insère une carte dans la tableJoueur.
		* 
		*/
		void insererCartePositionIJ(unsigned int indi, unsigned int indj, Carte & c);

		/**
		* @brief 
		* Insère une carte adversaire avec le nom de l'adversaire dans tableJoueur.
		* 
		*/
		void insererCarteAdversairePositionJ(unsigned int indj, unsigned int num);

		/**
		* @brief 
		* Efface les cartes en mettant un espace dans la tableJoueur .
		* 
		*/
		void effacerMainTxt();

		/**
		* @brief 
		* Dessine toutes les cartes de la main dans tableJoueur.
		* 
		*/
		void dessinerMainTxt()

		/**
		* @brief 
		* Actualise les cartes de la main dans tableJoueur.
		* 
		*/
		void modifMainTxt();
		
		/**
		* @brief 
		* Détermine si le joueur est gagnant.
		* 
		* @return
		* Vrai si la main du joueur est vide
		*/
		bool gagnant();

		/**
		* @brief 
		* Accesseur: récupère la largeur de la table du joueur.
		* 
		* @return
		* Entier
		*/
		unsigned int getLarg () const;

		/**
		* @brief 
		* Accesseur: récupère la hauteur de la table du joueur.
		* 
		* @return
		* Entier
		*/
		unsigned int getHaut () const;


		
				
};

#endif
