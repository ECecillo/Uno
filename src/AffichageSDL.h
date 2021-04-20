#ifndef _SDLJEU_H
#define _SDLJEU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "Jeu.h"
#include "VarianteCumul"
#include "VarianteDoublon"
#include "VarianteEchange"
#include "VarianteSuite"
#include "VarianteTourne"

//! \brief Pour gérer une image avec SDL2
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



/**
    La classe gérant le jeu avec un affichage SDL
*/
class sdlJeu {

private :

	Jeu jeu;

    SDL_Window * window;
    SDL_Renderer * renderer;

    TTF_Font * font;
    Image font_im;
    SDL_Color font_color;

    Mix_Chunk * sound;
    bool withSound;

    Image im_pacman;
    Image im_mur;
    Image im_pastille;
    Image im_fantome;

    bool souris;
    bool touche;
public :

    sdlJeu ();
    ~sdlJeu ();
    void sdlBoucleJeu ();
    void sdlAffSalleAttente ();
    void sdlChoixJeu ();

};

#endif
