#ifndef _SDLJEU_H
#define _SDLJEU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <Menu.h>
#include <fstream>
#include <sstream>

//#include <cstring>

#include "Jeu.h"

using namespace std;

class Image
{

private:
	SDL_Surface *surface;
	SDL_Texture *texture;
	bool has_changed;

public:
	Image();
	void loadFromFile(const char *filename, SDL_Renderer *renderer);
	void loadFromCurrentSurface(SDL_Renderer *renderer);
	void draw(SDL_Renderer *renderer, int x, int y, int w = -1, int h = -1);
	SDL_Texture *getTexture() const;
	void setSurface(SDL_Surface *surf);
};

class sdlJeu
{

private:
	SDL_Window *window;
	SDL_Renderer *renderer;

	TTF_Font *font;
	TTF_Font *fontTexte;

	Image font_im;
	SDL_Color font_color;

	Image fondMenu;
	Image im_carte;

	

	Mix_Chunk *sons[3]; // Tableau dans lequel on charge nos fichiers audio.

	int choixVolume;	  // Volume qui sert de référence pour la définition du volume.
	int volume;		  // Volume Sons.
	Mix_Music *selection; // son pour la sélection d'un paramètre.

	int HauteurEcran; // Résolution de la fenêtre.
	int LargeurEcran; // Résolution de la fenêtre.

	//template <typename T> T HauteurEcran;
	//template <typename T> T LargeurEcran;

public:
	sdlJeu();
	~sdlJeu();
	/**
	 * @brief Lis le fichier temporaire contenant la résolution de la fenêtre.
	 * 
	 */
	void lireFichierRes();
	/**
	 * @brief Ecris la nouvelle résolution en remplaçant l'ancienne.
	 * 
	 */
	void modifFichierRes(int largeur, int hauteur);
	
	/**
	 * @brief 
	 * Affiche le Menu avec tous les fonts à partir d'un objet Menu
	 * qui contient des vecteurs string dans lesquels on a mis tous les titres, résolutions ..
	 */
	void sdlMenu();

	/**
	 * @brief 
	 * Affiche la partie Regalge (Resolution, Son ....)
	 * @param m : Objet Menu qui contient les string à afficher pour la partie Reglage. 
	 */
	void sdlReglage(Menu &m);
	
	/**
	 * @brief 
	 * Affiche la salle d'attente avec les textes (Nombre Joueurs, Nombre Bots ...)
	 * @param variante : Variante du Jeu sur lequel on va jouer.
	 * @param nombreJoueurs : Nombre de Joueur dans la partie.
	 * @param nombreIA : Nombre d'ordinateurs dans le Jeu.
	 */
	void sdlAffSalleAttente(unsigned int variante, unsigned int nombreJoueurs, unsigned int nombreIA);
	
	/**
	 * @brief 
	 * Appelle l'affichage de la salle d'attente et s'occupe des interactions avec la souris, 
	 * c'est aussi dans cette procédure qu'on va lancer la boucle du Jeu suivant les paramètres de la salle d'attente. 
	 */
	void sdlUno();

	/**
	 * @brief 
	 * Créer une nouvelle fenêtre et affiche [1..9] pour le nombre de joueurs.
	 * @return unsigned int correspondant au nombre de Joueur. 
	 */
	unsigned int sdlAffChoixJoueurs();

	/**
	 * @brief 
	 * Créer une nouvelle fenêtre et affiche de [0..2] pour le nombre de Bots.
	 * @return unsigned int correspondant au nombre de Bot. 
	 */
	unsigned int sdlAffChoixOrdinateurs();
	
	/**
	 * @brief 
	 * Créer une nouvelle fenêtre et affiche toutes les variantes sur lequel on peut Jouer.
	 * @return unsigned int correspondant à la variante du Jeu. 
	 */
	unsigned int sdlAffChoixJeu();

	/**
	 * @brief 
	 * La boucle qui fera tourner la partie de UNO.
	 * @param jeu 
	 */
	void sdlBoucleJeu(Jeu &jeu);

	/**
	 * @brief 
	 * Procédure qui s'occupe d'afficher les cartes du Joueur Actif.
	 * @param c : La carte.
	 * @param positionX  : La position dans la fenêtre.
	 * @param positionY : //
	 */
	void sdlAffCarte(const Carte &c, int positionX, int positionY);

	/**
	 * @brief 
	 * Affiche une carte le nombre de carte restant pour ce joueur et son Nom.
	 * @param jeu : Variable permettant de récupérer les infos du Joueur.
	 * @param indiceJoueur : Indice du Joueur que l'on affiche. 
	 */
	void sdlAffJoueur(Jeu &jeu, unsigned int indiceJoueur);

	/**
	 * @brief 
	 * Fonction permettant de choisir la couleur sur laquelle on jouera lorsque l'on pose un Joker 
	 * ou un +4.
	 * @return unsigned int entre [] chaque entier correspond à une Couleur.
	 */
	unsigned int choixCouleur();

	/**
	 * @brief 
	 * Affiche à côté du talon la couleur que l'on a choisie dans choixCouleur.
	 * @param couleur : Entier qui correspond à une couleur.
	 */
	void sdlAffCouleurChoisie(unsigned int couleur);

	/**
	 * @brief 
	 * Procédure qui gère le moment où on est dans un Uno.
	 * @param jeu
	 */
	void situationUno(Jeu &jeu);
};

#endif
