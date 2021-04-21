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

	Jeu jeu;

    SDL_Window * window;
    SDL_Renderer * renderer;

    TTF_Font * font;
    Image font_im;
    SDL_Color font_color;


    Image im_salleAttente;
    Image im_carte;
/*	Image im_pacman; //? im_carte im_pioche im_talon im_passer im_uno im_contre_uno im_adversaire im_main
    Image im_mur;	//?
    Image im_pastille;	//?
    Image im_fantome;	//?

    bool souris;	//?
    bool touche;	//?*/
public :



    sdlJeu ();
    ~sdlJeu ();
void sdlAffSalleAttente();
void sdlAffJoueurActif();
    void sdlBoucleJeu ();
    /*void sdlAff ();*/

};

#endif
