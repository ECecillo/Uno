// g++ simple.cpp -lSDL2 -lSDL2_ttf -lSDL2_image

#include <cassert>
#include <time.h>
#include <stdlib.h>
#include "VarianteDoublon.h"
#include "VarianteEchange.h"
#include "VarianteTourne.h"
#include "VarianteSuite.h"
#include "VarianteCumul.h"
#include "AffichageSDL.h"
#include <iostream>




const int TAILLE_SPRITE = 32;

const SDL_Color noir={0,0,0};
const SDL_Color bleu={0,0,255};
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


/**
    La classe g�rant le jeu avec un affichage SDL
*/


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
	
    // JEU
    Jeu jeu;

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

    unsigned int variante=1; // entre 1 et 6
    unsigned int nombreJoueurs=2; // entre 1 et 9
    unsigned int nombreIA=0; //entre 0 et 2


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

        paramJeu.x = 720;paramJeu.y = 300;paramJeu.w = 100;paramJeu.h = 50;
        switch (variante)
        {
            case 1: 
                font_im.setSurface(TTF_RenderText_Solid(font,"Classique",jaune));
                break;
            case 2:
                font_im.setSurface(TTF_RenderText_Solid(font,"Variante Cumul",jaune));
                break;
            case 3:
                font_im.setSurface(TTF_RenderText_Solid(font,"Variante Doublon",jaune));
                break;
            case 4:
                font_im.setSurface(TTF_RenderText_Solid(font,"Variante Echange",jaune));
                break;
            case 5:
                font_im.setSurface(TTF_RenderText_Solid(font,"Variante Suite",jaune));
                break;
            case 6:
                font_im.setSurface(TTF_RenderText_Solid(font,"Variante Tourne",jaune));
                break;
            default:
                break;
        }
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&paramJeu);

        // Texte "Nombre de joueurs"
        SDL_Rect paramJoueurs;
        paramJoueurs.x = 600;paramJoueurs.y = 400;paramJoueurs.w = 300;paramJoueurs.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,"Nombre de joueurs : ",jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&paramJoueurs);

        paramJoueurs.x = 920;paramJoueurs.y = 400;paramJoueurs.w = 50;paramJoueurs.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,to_string(nombreJoueurs).c_str(),jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&paramJoueurs);

        // Texte "Nombre d'ordinateurs"
        SDL_Rect paramBot;
        paramBot.x = 600;paramBot.y = 500;paramBot.w = 300;paramBot.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,"Nombre d'ordinateurs : ",jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&paramBot);

        paramBot.x = 920;paramBot.y = 500;paramBot.w = 50;paramBot.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,to_string(nombreIA).c_str(),jaune));
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
                        bool choix = false;
                        while (!choix)
                        {
                            sdlAffChoixJeu();
                            while (SDL_PollEvent(&events))
                            {
                                if(events.type==SDL_MOUSEBUTTONUP)
                                {
                                    if (events.button.x>545 && events.button.x<645 && events.button.y>250 && events.button.y<670) // clic sur une variante
                                    {
                                        variante = (events.button.y-250)/70;
                                        choix = true;
                                    }
                                }

                            }
                        }
                    }
                    if (events.button.x>600 && events.button.x<900 && events.button.y>400 && events.button.y<450) // clic sur la ligne "Nombre de joueurs"
                    {
                        bool choix = false;
                        while (!choix)
                        {
                            sdlAffChoixJoueurs();
                            while (SDL_PollEvent(&events))
                            {
                                if(events.type==SDL_MOUSEBUTTONUP)
                                {
                                    if (events.button.x>600 && events.button.x<900 && events.button.y>400 && events.button.y<450) // clic sur la ligne avec les nombres
                                    {
                                        nombreJoueurs = (events.button.x-445)/100;
                                        choix = true;
                                    }
                                }

                            }
                        }
                    }
                    if (events.button.x>600 && events.button.x<900 && events.button.y>500 && events.button.y<550) // clic sur la ligne "Nombre d'ordinateurs"
                    {
                        bool choix = false;
                        while (!choix)
                        {
                            sdlAffChoixOrdinateurs();
                            while (SDL_PollEvent(&events))
                            {
                                if(events.type==SDL_MOUSEBUTTONUP)
                                {
                                    if (events.button.x>600 && events.button.x<900 && events.button.y>500 && events.button.y<550) // clic sur la ligne avec les nombres
                                    {
                                        nombreIA = (events.button.x-545)/100;
                                        choix = true;
                                    }
                                }

                            }
                        }
                    }
                    /*if (events.button.x>600 && events.button.x<900 && events.button.y>600 && events.button.y<650) // clic sur la ligne "Lancer le jeu"
                    {
                        sdlBoucleJeu();
                    }*/
                    break;
			}
		}
        // on permute les deux buffers (cette fonction ne doit se faire qu'une seule fois dans la boucle)
        SDL_RenderPresent(renderer);
        cout << "nombre joueurs " << nombreJoueurs << endl;
    }
}

void sdlJeu::sdlAffChoixJeu()
{
    SDL_Rect choix;
    choix.x = 500;choix.y = 200;choix.w = 1000;choix.h = 500;
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer,&choix);

    SDL_Rect variante;
    variante.x = 545;variante.y = 250;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Classique",jaune));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&variante);

    variante.x = 545;variante.y = 320;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Variante Cumul",jaune));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&variante);

    variante.x = 545;variante.y = 390;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Variante Doublon",jaune));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&variante);

    variante.x = 545;variante.y = 460;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Variante Echange",jaune));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&variante);

    variante.x = 545;variante.y = 530;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Variante Suite",jaune));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&variante);

    variante.x = 545;variante.y = 600;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Variante Tourne",jaune));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&variante);
}


void sdlJeu::sdlAffChoixJoueurs()
{
    SDL_Rect choix;
    choix.x = 500;choix.y = 200;choix.w = 1000;choix.h = 500;
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer,&choix);
    for (int i=1; i<10; i++)
    {
        SDL_Rect paramJoueurs;
        paramJoueurs.x = 445+100*i;paramJoueurs.y = 400;paramJoueurs.w = 50;paramJoueurs.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,to_string(i).c_str(),jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&paramJoueurs);
    }
}

void sdlJeu::sdlAffChoixOrdinateurs()
{
    SDL_Rect choix;
    choix.x = 500;choix.y = 200;choix.w = 1000;choix.h = 500;
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer,&choix);
    for (int i=0; i<3; i++)
    {
        SDL_Rect paramJoueurs;
        paramJoueurs.x = 545+100*i;paramJoueurs.y = 500;paramJoueurs.w = 50;paramJoueurs.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,to_string(i).c_str(),jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&paramJoueurs);
    }
}

void sdlJeu::sdlAffCarte (const Carte & c, int positionX, int positionY)
{
    int carteEntier;
    char nomFichier[20];
    string nomCarte;
    char const * nomImage;
    if (c.getValeur()==13)
        {
            im_carte.loadFromFile("data/+4.png",renderer);
            im_carte.draw(renderer,positionX,positionY,110,157);
        }
        if (c.getValeur()==14)
        {
            im_carte.loadFromFile("data/joker.png",renderer);
            im_carte.draw(renderer,positionX,positionY,110,157);
        }
        if (c.getValeur()<=12)
        {
            carteEntier = 10 * c.getValeur() + c.getCouleur();
            nomCarte = to_string(carteEntier);
            nomImage = nomCarte.c_str();
            strcpy (nomFichier,"data/");
            strcat (nomFichier,nomImage);
            strcat (nomFichier,".png");
            im_carte.loadFromFile(nomFichier,renderer);
            im_carte.draw(renderer,positionX,positionY,110,157);
        }     
}

void sdlJeu::sdlAffJoueurActif (Jeu & jeu)
{
    //Remplir l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect joueur;
    SDL_Rect nombreCartes;

    // affiche les adversaires
    for (unsigned int i=0; i<jeu.nombreJoueurs+jeu.nombreIA-1; i++)
    {
        int positionCarte=(2000 - (130*(jeu.nombreJoueurs+jeu.nombreIA-1)-20))/2 + 130*i;

        // nom adversaire
        joueur.x = positionCarte;joueur.y = 0;joueur.w = 110;joueur.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,jeu.joueurs[(jeu.joueurActif + 1 + i) % (jeu.nombreJoueurs+jeu.nombreIA)].nom.c_str(),jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&joueur);

        // carte adversaire
        im_carte.loadFromFile("data/dosvide.png",renderer);
        im_carte.draw(renderer,positionCarte,50,110,157);

        // nombre de cartes adversaires
        nombreCartes.x = positionCarte+40;nombreCartes.y = 113;nombreCartes.w = 30;nombreCartes.h = 30;
        font_im.setSurface(TTF_RenderText_Solid(font,to_string(jeu.joueurs[(jeu.joueurActif + 1 + i) % (jeu.nombreJoueurs+jeu.nombreIA)].main.size()).c_str(),jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&nombreCartes);
    }
    
    // affiche le centre
    // pioche
    im_carte.loadFromFile("data/dos.png",renderer);
    im_carte.draw(renderer,800,300,110,157);

    // talon
    sdlAffCarte(jeu.talon.back(),1090,300);

    // Uno
    im_carte.loadFromFile("data/carteuno.png",renderer);
    im_carte.draw(renderer,573,323,157,110);

    nombreCartes.x = 626;nombreCartes.y = 353;nombreCartes.w = 50;nombreCartes.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Uno",noir));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&nombreCartes);

    // Contre Uno
    im_carte.loadFromFile("data/cartecontreuno.png",renderer);
    im_carte.draw(renderer,1270,323,157,110);

    nombreCartes.x = 1318;nombreCartes.y = 338;nombreCartes.w = 80;nombreCartes.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Contre",noir));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&nombreCartes);
    nombreCartes.x = 1313;nombreCartes.y = 373;nombreCartes.w = 50;nombreCartes.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Uno",noir));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&nombreCartes);
    

    // affiche le nom et la main du joueur actif

    // le nom
    joueur.x = 0;joueur.y = 550;joueur.w = 110;joueur.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,jeu.joueurs[jeu.joueurActif].nom.c_str(),jaune));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&joueur);

    // la main
    for (int i=0; i<(jeu.joueurs[jeu.joueurActif].main).size(); i++)
    {
        sdlAffCarte(jeu.joueurs[jeu.joueurActif].main[i],110*i,600);
    }
    SDL_RenderPresent(renderer);
}



void sdlJeu::sdlBoucleJeu (unsigned int variante, unsigned int nbJ, unsigned int nbB) 
{
    switch (variante)
    {
        case 1: {Jeu jeu(nbJ,nbB);
                sdlAffJoueurActif (jeu);
                break;}
        case 2: {VarianteCumul jeu(nbJ,nbB);
                sdlAffJoueurActif (jeu);
                break;}
        case 3: {VarianteDoublon jeu(nbJ,nbB);
                sdlAffJoueurActif (jeu);
                break;}
        case 4: {VarianteEchange jeu(nbJ,nbB);
                sdlAffJoueurActif (jeu);
                break;}
        case 5: {VarianteSuite jeu(nbJ,nbB);
                sdlAffJoueurActif (jeu);
                break;}
        case 6: {VarianteTourne jeu(nbJ,nbB);
                sdlAffJoueurActif (jeu);
                break;}
        default: break;
    }

    SDL_Event events;
	bool quit = false;

    Uint32 t = SDL_GetTicks(), nt;

	// tant que ce n'est pas la fin ...
	while (!quit) {
        
        //Remplir l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // affiche les cartes des adversaires
   
    
    
        
        
        
    



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
            }
        }
    }
}

/*



/*void sdlJeu::sdlBoucle () {
    SDL_Event events;
	bool quit = false;

    Uint32 t = SDL_GetTicks(), nt;

	// tant que ce n'est pas la fin ...
	while (!quit) {

		// tant qu'il y a des evenements à traiter (cette boucle n'est pas bloquante)
		while (SDL_PollEvent(&events)) {
			if (events.type == SDL_QUIT) quit = true;           // Si l'utilisateur a clique sur la croix de fermeture
			else if (events.type == SDL_KEYDOWN) 
            {              // Si une touche est enfoncee
                bool mangePastille = false;
				switch (events.key.keysym.scancode) 
                {
                    // case SDL_SCANCODE_UP:
                    // 	mangePastille = jeu.actionClavier('b');    // car Y inverse
                    // 	break;
                    case SDL_SCANCODE_ESCAPE:
                    case SDL_SCANCODE_Q:
                        quit = true;
                        break;
                    default: break;
				}
			}
		}

		// on affiche le jeu sur le buffer cach�
		sdlAff();

		// on permute les deux buffers (cette fonction ne doit se faire qu'une seule fois dans la boucle)
        SDL_RenderPresent(renderer);
	}
}*/
