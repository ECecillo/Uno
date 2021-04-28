#ifndef _SDLJEU_H
#define _SDLJEU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Jeu.h"

using namespace std;

class Image {

private:

    SDL_Surface * surface;
    SDL_Texture * texture;
    bool has_changed;

public:
	Image () ;
	void loadFromFile (const char* filename, SDL_Renderer * renderer);
	void loadFromCurrentSurface (SDL_Renderer * renderer);
	void draw (SDL_Renderer * renderer, int x, int y, int w=-1, int h=-1);
	SDL_Texture * getTexture() const;
	void setSurface(SDL_Surface * surf);


		
		


		


		
				
};



class sdlJeu {

private :



    SDL_Window * window;
    SDL_Renderer * renderer;

	
    TTF_Font * font;
    Image font_im;
    SDL_Color font_color;


    Image im_salleAttente;
    Image im_carte;

public :



	sdlJeu ();
	~sdlJeu ();
	void sdlAffSalleAttente(SDL_Window * param, SDL_Renderer * rendererParam, unsigned int variante, unsigned int nombreJoueurs, unsigned int nombreIA);
	void sdlUno();
	unsigned int sdlAffChoixJoueurs();
	unsigned int sdlAffChoixOrdinateurs();
	void sdlAffCarte(const Carte & c, int positionX, int positionY);
	void sdlAffJoueur(Jeu & jeu, unsigned int indiceJoueur);
	unsigned int choixCouleur();
void sdlAffCouleurChoisie(unsigned int couleur);
	void sdlBoucleJeu (Jeu & jeu);
	unsigned int sdlAffChoixJeu ();

};

#endif
