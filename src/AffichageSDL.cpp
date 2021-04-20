// g++ simple.cpp -lSDL2 -lSDL2_ttf -lSDL2_image

#include <cassert>
#include <time.h>
#include <stdlib.h>

#include "AffichageSDL.h"


#include <iostream>




const int TAILLE_SPRITE = 32;

const SDL_Color jaune={255,255,0};


float temps () {
    return float(SDL_GetTicks()) / CLOCKS_PER_SEC;  // conversion des ms en secondes en divisant par 1000
}


// ============= CLASS IMAGE =============== //
//! \brief Pour gérer une image avec SDL2



Image::Image () {
    surface = NULL;
    texture = NULL;
    has_changed = false;
}


void Image::loadFromFile (const char* filename, SDL_Renderer * renderer) {
    surface = IMG_Load(filename);
    if (surface == NULL) {
        string nfn = string("../") + filename;
        cout << "Error: cannot load "<< filename <<". Trying "<<nfn<<endl;
        surface = IMG_Load(nfn.c_str());
        if (surface == NULL) {
            nfn = string("../") + nfn;
            surface = IMG_Load(nfn.c_str());
        }
    }
    if (surface == NULL) {
        cout<<"Error: cannot load "<< filename <<endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Surface * surfaceCorrectPixelFormat = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_ARGB8888,0);
    SDL_FreeSurface(surface);
    surface = surfaceCorrectPixelFormat;

    texture = SDL_CreateTextureFromSurface(renderer,surfaceCorrectPixelFormat);
    if (texture == NULL) {
        cout << "Error: problem to create the texture of "<< filename<< endl;
        SDL_Quit();
        exit(1);
    }
}

void Image::loadFromCurrentSurface (SDL_Renderer * renderer) {
    texture = SDL_CreateTextureFromSurface(renderer,surface);
    if (texture == NULL) {
        cout << "Error: problem to create the texture from surface " << endl;
        SDL_Quit();
        exit(1);
    }
}

void Image::draw (SDL_Renderer * renderer, int x, int y, int w, int h) {
    int ok;
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = (w<0)?surface->w:w;
    r.h = (h<0)?surface->h:h;

    if (has_changed) {
        ok = SDL_UpdateTexture(texture,NULL,surface->pixels,surface->pitch);
        assert(ok == 0);
        has_changed = false;
    }

    ok = SDL_RenderCopy(renderer,texture,NULL,&r);
    assert(ok == 0);
}

SDL_Texture * Image::getTexture() const {return texture;}

void Image::setSurface(SDL_Surface * surf) {surface = surf;}







// ============= CLASS SDLJEU =============== //



sdlJeu::sdlJeu() : window(nullptr), renderer(nullptr), font(nullptr)
{
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << endl;SDL_Quit();exit(1);
    }

    if (TTF_Init() != 0) {
        cout << "Erreur lors de l'initialisation de la SDL_ttf : " << TTF_GetError() << endl;SDL_Quit();exit(1);
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if( !(IMG_Init(imgFlags) & imgFlags)) {
        cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;SDL_Quit();exit(1);
    }

    // Creation de la fenetre
    window = SDL_CreateWindow("UNO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2000/*dimx*/, 1000/*dimy*/, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl; 
        SDL_Quit(); 
        exit(1);
    }

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    // beaucoup de cartes, on ne peut pas une im_carte pour chaque carte
    // IMAGES
    im_salleAttente.loadFromFile("data/uno4.png",renderer);

    // FONTS
    font = TTF_OpenFont("data/DejaVuSansCondensed.ttf",1000);
    if (font == NULL)
        font = TTF_OpenFont("../data/DejaVuSansCondensed.ttf",1000);
    if (font == NULL) {
            cout << "Failed to load DejaVuSansCondensed.ttf! SDL_TTF Error: " << TTF_GetError() << endl; 
            SDL_Quit(); 
            exit(1);
	}
	

}
    
sdlJeu::~sdlJeu () {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void sdlJeu::sdlAffSalleAttente () 
{
    SDL_Event events;
	bool quit = false;

    Uint32 t = SDL_GetTicks(), nt;

	// tant que ce n'est pas la fin ...
	while (!quit) {

        //Remplir l'écran de noir
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        im_salleAttente.draw(renderer,0,0,2000,1000);
        // Rectangle noir par dessus
        SDL_Rect param;
        param.x = 500;param.y = 200;param.w = 1000;param.h = 500;
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderFillRect(renderer,&param);
        
        // Texte "Paramètres du jeu"
        SDL_Rect titre;
        titre.x = 700;titre.y = 200;titre.w = 600;titre.h = 50;
        font_color.r = 0;font_color.g = 0;font_color.b = 255;
        font_im.setSurface(TTF_RenderText_Solid(font,"Parametres du jeu",jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&titre);

        // Texte "Jeu"
        SDL_Rect paramJeu;
        paramJeu.x = 600;paramJeu.y = 300;paramJeu.w = 100;paramJeu.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,"Jeu : ",jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&paramJeu);

        // Texte "Nombre de joueurs"
        SDL_Rect paramJoueurs;
        paramJoueurs.x = 600;paramJoueurs.y = 400;paramJoueurs.w = 300;paramJoueurs.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,"Nombre de joueurs : ",jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&paramJoueurs);

        // Texte "Nombre d'ordinateurs"
        SDL_Rect paramBot;
        paramBot.x = 600;paramBot.y = 500;paramBot.w = 300;paramBot.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,"Nombre d'ordinateurs : ",jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&paramBot);

        // Texte "Lancer le jeu"
        SDL_Rect paramDemarrer;
        paramDemarrer.x = 600;paramDemarrer.y = 600;paramDemarrer.w = 300;paramDemarrer.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,"Lancer le jeu",jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&paramDemarrer);


        // tant qu'il y a des evenements à traiter (cette boucle n'est pas bloquante)
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
                case SDL_QUIT:     // Si l'utilisateur a clique sur la croix de fermeture
                    quit = true;
                    break;          
			    case SDL_KEYDOWN:  // Si une touche est enfoncee
                    if(events.key.keysym.scancode==SDL_SCANCODE_Q) quit = true;
                    break;
                case SDL_MOUSEBUTTONUP: // Si le bouton de la souris est relevé
                    if (events.button.x>600 && events.button.x<700 && events.button.y>300 && events.button.y<350) // clic sur la ligne "Jeu"
                    {
                        sdlAffChoixJeu();
                    }
                    if (events.button.x>600 && events.button.x<900 && events.button.y>400 && events.button.y<450) // clic sur la ligne "Nombre de joueurs"
                    {
                        
                    }
                    if (events.button.x>600 && events.button.x<900 && events.button.y>500 && events.button.y<550) // clic sur la ligne "Nombre d'ordinateurs"
                    {
                        
                    }
                    if (events.button.x>600 && events.button.x<900 && events.button.y>600 && events.button.y<650) // clic sur la ligne "Lancer le jeu"
                    {
                        sdlBoucle();
                    }
                    break;
			}
		}
        // on permute les deux buffers (cette fonction ne doit se faire qu'une seule fois dans la boucle)
        SDL_RenderPresent(renderer);
    }
}

void sdlJeu::sdlAffChoixJeu()
{
    // Rectangle noir par dessus
    SDL_Rect jeux;
    jeux.x = 500;jeux.y = 200;jeux.w = 1000;jeux.h = 500;
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer,&jeux);

    // Texte "Choix du jeu"
    SDL_Rect titre;
    titre.x = 700;titre.y = 200;titre.w = 600;titre.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Choix du jeu",jaune));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&titre);


}





void sdlJeu::sdlBoucleJeu () 
{

}

