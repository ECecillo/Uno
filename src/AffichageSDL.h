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
	void sdlMenu();
	void sdlReglage(Menu &m);
	void sdlAffSalleAttente(SDL_Window *param, SDL_Renderer *rendererParam, unsigned int variante, unsigned int nombreJoueurs, unsigned int nombreIA);
	void sdlUno();
	unsigned int sdlAffChoixJoueurs();
	unsigned int sdlAffChoixOrdinateurs();
	void sdlAffCarte(const Carte &c, int positionX, int positionY);
	void sdlAffJoueur(Jeu &jeu, unsigned int indiceJoueur);
	unsigned int choixCouleur();
	void sdlAffCouleurChoisie(unsigned int couleur);
	void situationUno(Jeu &jeu);
	void sdlBoucleJeu(Jeu &jeu);
	unsigned int sdlAffChoixJeu();
};

#endif
