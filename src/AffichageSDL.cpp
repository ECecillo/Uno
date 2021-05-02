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
    window = SDL_CreateWindow("UNO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2000, 1000, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
	
    

}
    
sdlJeu::~sdlJeu () {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Affiche la salle d'attente
void sdlJeu::sdlAffSalleAttente (SDL_Window * param, SDL_Renderer * rendererParam, unsigned int variante, unsigned int nombreJoueurs, unsigned int nombreIA) 
{
    SDL_SetRenderDrawColor(rendererParam, 0, 0, 0, 255);
    SDL_RenderClear(rendererParam);
    
    // Texte "Jeu"
    SDL_Rect texte;
    texte.x = 100;texte.y = 50;texte.w = 100;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Jeu : ",jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam,font_im.getTexture(),NULL,&texte);

    texte.x = 220;texte.y = 50;texte.w = 200;texte.h = 50;
    switch (variante)
    {
        case 1: 
            font_im.setSurface(TTF_RenderText_Solid(font,"Jeu Classique",jaune));
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
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam,font_im.getTexture(),NULL,&texte);

    // Texte "Nombre de joueurs"
    texte.x = 100;texte.y = 150;texte.w = 300;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Nombre de joueurs : ",jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam,font_im.getTexture(),NULL,&texte);

    texte.x = 420;texte.y = 150;texte.w = 50;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,to_string(nombreJoueurs).c_str(),jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam,font_im.getTexture(),NULL,&texte);

    // Texte "Nombre d'ordinateurs"
    texte.x = 100;texte.y = 250;texte.w = 300;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Nombre d'ordinateurs : ",jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam,font_im.getTexture(),NULL,&texte);

    texte.x = 420;texte.y = 250;texte.w = 50;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,to_string(nombreIA).c_str(),jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam,font_im.getTexture(),NULL,&texte);

    // Texte "Lancer le jeu"
    texte.x = 100;texte.y = 350;texte.w = 300;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Lancer le jeu",jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam,font_im.getTexture(),NULL,&texte);

    SDL_RenderPresent(rendererParam);
}
            
// Préparation de la partie
void sdlJeu::sdlUno () 
{
    //Remplir l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    im_salleAttente.draw(renderer,0,0,2000,1000);
    SDL_RenderPresent(renderer);

    SDL_Window * param = SDL_CreateWindow("Paramètres du jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (param == NULL) {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl; 
        SDL_Quit(); 
        exit(1);
    }
    SDL_Renderer * rendererParam = SDL_CreateRenderer(param,-1,SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(rendererParam);

    SDL_Event event;
	bool quit = false;

    /*Uint32 t = SDL_GetTicks(), nt;*/

    unsigned int variante=1; // entre 1 et 6
    unsigned int nombreJoueurs=2; // entre 1 et 9
    unsigned int nombreIA=0; //entre 0 et 2
    bool choix;
    int sourisX;
    int sourisY;

    

    sdlAffSalleAttente (param, rendererParam, variante, nombreJoueurs, nombreIA);
	// tant que ce n'est pas la fin ...
	while (!quit) 
    {
        
        // attend un évènement à traiter
        while (SDL_PollEvent(&event)) 
        {
            sourisX = event.button.x;
            sourisY = event.button.y;
            cout << sourisX << " " << sourisY << endl;
            cout << "debut while Uno" << endl;
            switch (event.type)
            {
                case SDL_QUIT:     // Si l'utilisateur a clique sur la croix de fermeture
                    quit = true;
                    break;          
                case SDL_KEYDOWN:  // Si une touche est enfoncee
                    if(event.key.keysym.scancode==SDL_SCANCODE_Q) quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN: // Si le bouton de la souris est relevé
                    cout << "case" << endl;
                    if (sourisX>100 && sourisX<320 && sourisY>50 && sourisY<100) // clic sur la ligne "Jeu"
                    {
                        variante = sdlAffChoixJeu();
                        sdlAffSalleAttente (param, rendererParam, variante, nombreJoueurs, nombreIA);
                        cout << "variante=" << variante << endl;
                    }
                    if (sourisX>100 && sourisX<470 && sourisY>150 && sourisY<200) // clic sur la ligne "Nombre de joueurs"
                    {
                        nombreJoueurs = sdlAffChoixJoueurs();
                        cout << "nombre joueurs " << nombreJoueurs << endl;
                        sdlAffSalleAttente (param, rendererParam, variante, nombreJoueurs, nombreIA);
                    }
                    if (event.button.x>100 && event.button.x<470 && event.button.y>250 && event.button.y<300) // clic sur la ligne "Nombre d'ordinateurs"
                    {
                        nombreIA = sdlAffChoixOrdinateurs();
                        cout << "nombre bots " << nombreIA << endl;
                        sdlAffSalleAttente (param, rendererParam, variante, nombreJoueurs, nombreIA);
                    }
                    if (event.button.x>100 && event.button.x<400 && event.button.y>350 && event.button.y<400) // clic sur la ligne "Lancer le jeu"
                    {
                        SDL_DestroyRenderer(rendererParam);
                        SDL_DestroyWindow(param);
                        quit = true;
                        
                    }
                    break;
                default: 
                    break;
            }
        }
    }
    switch (variante)
    {
        case 1: {Jeu jeu(nombreJoueurs,nombreIA);
                sdlBoucleJeu (jeu);
                break;}
        case 2: {VarianteCumul jeu(nombreJoueurs,nombreIA);
                sdlBoucleJeu (jeu);
                break;}
        case 3: {VarianteDoublon jeu(nombreJoueurs,nombreIA);
                sdlBoucleJeu (jeu);
                break;}
        case 4: {VarianteEchange jeu(nombreJoueurs,nombreIA);
                sdlBoucleJeu (jeu);
                break;}
        case 5: {VarianteSuite jeu(nombreJoueurs,nombreIA);
                sdlBoucleJeu (jeu);
                break;}
        case 6: {VarianteTourne jeu(nombreJoueurs,nombreIA);
                sdlBoucleJeu (jeu);
                break;}
        default: break;
    }
}

// Fait choisir la variante
unsigned int sdlJeu::sdlAffChoixJeu()
{
    unsigned int choixVariante;
    SDL_Window * choixJeu = SDL_CreateWindow("Choix du jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (choixJeu == NULL) {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl; 
        SDL_Quit(); 
        exit(1);
    }

    SDL_Renderer * rendererJeu = SDL_CreateRenderer(choixJeu,-1,SDL_RENDERER_ACCELERATED);

    SDL_Rect variante;
    variante.x = 50;variante.y = 50;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Classique",jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu,font_im.getTexture(),NULL,&variante);

    variante.x = 50;variante.y = 120;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Variante Cumul",jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu,font_im.getTexture(),NULL,&variante);

    variante.x = 50;variante.y = 190;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Variante Doublon",jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu,font_im.getTexture(),NULL,&variante);

    variante.x = 50;variante.y = 260;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Variante Echange",jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu,font_im.getTexture(),NULL,&variante);

    variante.x = 50;variante.y = 330;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Variante Suite",jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu,font_im.getTexture(),NULL,&variante);

    variante.x = 50;variante.y = 400;variante.w = 300;variante.h = 50;
    font_color.r = 0;font_color.g = 0;font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font,"Variante Tourne",jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu,font_im.getTexture(),NULL,&variante);

    SDL_RenderPresent(rendererJeu);

    SDL_Event eventJeu;
    bool choixFait=false;
    int sourisX;
    int sourisY;

    while (!choixFait)
    {
        while (SDL_PollEvent(&eventJeu) && eventJeu.type == SDL_MOUSEBUTTONDOWN) 
        {
            sourisX = eventJeu.button.x;
            sourisY = eventJeu.button.y;
            cout << "choix variante " << sourisX << " " << sourisY << endl;
            if(eventJeu.type==SDL_MOUSEBUTTONDOWN)
            {
                if (sourisX>50 && sourisX<350 && sourisY>50 && sourisY<450) // clic sur une variante
                {
                    choixVariante = (sourisY-50)/70+1;
                    choixFait = true;
                }
            }
        }
    }
    SDL_DestroyRenderer(rendererJeu);
    SDL_DestroyWindow(choixJeu);
    return choixVariante;
}

// Fait choisir le nombre de joueurs
unsigned int sdlJeu::sdlAffChoixJoueurs()
{
    unsigned int Joueurs;
    SDL_Window * choixJoueurs = SDL_CreateWindow("Choix du nombre de joueurs", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (choixJoueurs == NULL) {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl; 
        SDL_Quit(); 
        exit(1);
    }

    SDL_Renderer * rendererJoueurs = SDL_CreateRenderer(choixJoueurs,-1,SDL_RENDERER_ACCELERATED);
    
    SDL_Rect choix;
    choix.x = 50;choix.y = 200;choix.w = 1000;choix.h = 500;
    SDL_SetRenderDrawColor(rendererJoueurs,0,0,0,255);
    SDL_RenderFillRect(rendererJoueurs,&choix);
    for (int i=0; i<9; i++)
    {
        SDL_Rect paramJoueurs;
        paramJoueurs.x = 50+100*i;paramJoueurs.y = 200;paramJoueurs.w = 50;paramJoueurs.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,to_string(i+1).c_str(),jaune));
        font_im.loadFromCurrentSurface(rendererJoueurs);
        SDL_RenderCopy(rendererJoueurs,font_im.getTexture(),NULL,&paramJoueurs);
    }
    SDL_RenderPresent(rendererJoueurs);

    SDL_Event eventJoueurs;
    bool choixFait=false;
    int sourisX;
    int sourisY;

    while (!choixFait)
    {
        while (SDL_PollEvent(&eventJoueurs) && eventJoueurs.type == SDL_MOUSEBUTTONDOWN) 
        {
            sourisX = eventJoueurs.button.x;
            sourisY = eventJoueurs.button.y;
            cout << "choix joueurs " << sourisX << " " << sourisY << endl;
            if(eventJoueurs.type==SDL_MOUSEBUTTONDOWN)
            {
                if (sourisX>50 && sourisX<900 && sourisY>200 && sourisY<250) // clic sur une variante
                {
                    Joueurs = (sourisX-50)/100+1;
                    choixFait = true;
                }
            }
        }
    }
    SDL_DestroyRenderer(rendererJoueurs);
    SDL_DestroyWindow(choixJoueurs);
    return Joueurs;

}

// Fait choisir le nombre de bots
unsigned int sdlJeu::sdlAffChoixOrdinateurs()
{
    unsigned int nombreIA;
    SDL_Window * choixBots = SDL_CreateWindow("Choix du nombre d'ordinateurs", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (choixBots == NULL) {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl; 
        SDL_Quit(); 
        exit(1);
    }

    SDL_Renderer * rendererBots = SDL_CreateRenderer(choixBots,-1,SDL_RENDERER_ACCELERATED);
    
    SDL_Rect choix;
    choix.x = 500;choix.y = 200;choix.w = 1000;choix.h = 500;
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer,&choix);
    for (int i=0; i<3; i++)
    {
        SDL_Rect paramBots;
        paramBots.x = 150+100*i;paramBots.y = 200;paramBots.w = 50;paramBots.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,to_string(i).c_str(),jaune));
        font_im.loadFromCurrentSurface(rendererBots);
        SDL_RenderCopy(rendererBots,font_im.getTexture(),NULL,&paramBots);
    }
    SDL_RenderPresent(rendererBots);

    SDL_Event eventBots;
    bool choixFait=false;
    int sourisX;
    int sourisY;

    while (!choixFait)
    {
        while (SDL_PollEvent(&eventBots) && eventBots.type == SDL_MOUSEBUTTONDOWN) 
        {
            sourisX = eventBots.button.x;
            sourisY = eventBots.button.y;
            cout << "choix bots " << sourisX << " " << sourisY << endl;
            if(eventBots.type==SDL_MOUSEBUTTONDOWN)
            {
                if (sourisX>50 && sourisX<450 && sourisY>200 && sourisY<250) // clic sur une variante
                {
                    nombreIA = (sourisX-150)/100;
                    choixFait = true;
                }
            }
        }
    }
    SDL_DestroyRenderer(rendererBots);
    SDL_DestroyWindow(choixBots);
    return nombreIA;
}

// Affiche une carte à une certaine position
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

// Affiche la table du joueur, indiceJoueur est l'indice dans le tableau jeu.joueurs
void sdlJeu::sdlAffJoueur (Jeu & jeu, unsigned int indiceJoueur)
{
    //Remplir l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect texte;

    // affiche les adversaires
    for (unsigned int i=0; i<jeu.nombreJoueurs+jeu.nombreIA-1; i++)
    {
        int positionCarte=(2000 - (130*(jeu.nombreJoueurs+jeu.nombreIA-1)-20))/2 + 130*i;

        // nom adversaire
        texte.x = positionCarte;texte.y = 0;texte.w = 110;texte.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font,jeu.joueurs[(jeu.joueurActif + 1 + i) % (jeu.nombreJoueurs+jeu.nombreIA)].nom.c_str(),jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&texte);

        // carte adversaire
        im_carte.loadFromFile("data/dosvide.png",renderer);
        im_carte.draw(renderer,positionCarte,50,110,157);

        // nombre de cartes adversaires
        texte.x = positionCarte+40;texte.y = 113;texte.w = 30;texte.h = 30;
        font_im.setSurface(TTF_RenderText_Solid(font,to_string(jeu.joueurs[(jeu.joueurActif + 1 + i) % (jeu.nombreJoueurs+jeu.nombreIA)].main.size()).c_str(),jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&texte);
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

    texte.x = 626;texte.y = 353;texte.w = 50;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Uno",noir));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&texte);

    // Contre Uno
    im_carte.loadFromFile("data/cartecontreuno.png",renderer);
    im_carte.draw(renderer,1270,323,157,110);

    texte.x = 1318;texte.y = 338;texte.w = 80;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Contre",noir));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&texte);
    texte.x = 1313;texte.y = 373;texte.w = 50;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Uno",noir));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&texte);
    

    // affiche le nom et la main du joueur actif

    // le nom
    texte.x = 0;texte.y = 550;texte.w = 110;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,jeu.joueurs[indiceJoueur].nom.c_str(),jaune));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&texte);

    // la main
    unsigned int tailleMain = jeu.joueurs[indiceJoueur].main.size();
    for (int i=0; i<tailleMain; i++)
    {
        sdlAffCarte(jeu.joueurs[indiceJoueur].main[i],110*(i%17),600+200*(i/17));
    }
    SDL_RenderPresent(renderer);
}


// Fait choisir la couleur à jouer (après joker et +4)
unsigned int sdlJeu::choixCouleur()
{
    SDL_Window * choixCouleur = SDL_CreateWindow("Quelle couleur?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 200, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (choixCouleur == NULL) {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl; 
        SDL_Quit(); 
        exit(1);
    }

    SDL_Renderer * rendererCouleur = SDL_CreateRenderer(choixCouleur,-1,SDL_RENDERER_ACCELERATED);

    Image im_choix;
    im_choix.loadFromFile("data/questioncouleur.png",rendererCouleur);
    im_choix.draw(rendererCouleur,50,21,110,157);

    SDL_Rect texte;
    texte.x = 210;texte.y = 75;texte.w = 100;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Rouge",jaune));
    font_im.loadFromCurrentSurface(rendererCouleur);
    SDL_RenderCopy(rendererCouleur,font_im.getTexture(),NULL,&texte);
    
    texte.x = 340;texte.y = 75;texte.w = 100;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Vert",jaune));
    font_im.loadFromCurrentSurface(rendererCouleur);
    SDL_RenderCopy(rendererCouleur,font_im.getTexture(),NULL,&texte);
   
    texte.x = 470;texte.y = 75;texte.w = 100;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Bleu",jaune));
    font_im.loadFromCurrentSurface(rendererCouleur);
    SDL_RenderCopy(rendererCouleur,font_im.getTexture(),NULL,&texte);
    
    texte.x = 600;texte.y = 75;texte.w = 100;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Jaune",jaune));
    font_im.loadFromCurrentSurface(rendererCouleur);
    SDL_RenderCopy(rendererCouleur,font_im.getTexture(),NULL,&texte);

    SDL_RenderPresent(rendererCouleur);

    SDL_Event eventCouleur;
    bool choixFait=false;
    int sourisX;
    int sourisY;
    unsigned int couleur;

    while (!choixFait)
    {
        while (SDL_PollEvent(&eventCouleur) && eventCouleur.type == SDL_MOUSEBUTTONDOWN) 
        {
            sourisX = eventCouleur.button.x;
            sourisY = eventCouleur.button.y;
            cout << "choix joueurs " << sourisX << " " << sourisY << endl;
            if(eventCouleur.type==SDL_MOUSEBUTTONDOWN)
            {
                if (sourisX>210 && sourisX<700 && sourisY>75 && sourisY<232) // clic sur une couleur
                {
                    couleur = (sourisX-210)/130+1;
                    choixFait = true;
                }
            }
        }
    }
    SDL_DestroyRenderer(rendererCouleur);
    SDL_DestroyWindow(choixCouleur);
    return couleur;    

}

// affiche une carte de la couleur qui a été choisie
void sdlJeu::sdlAffCouleurChoisie(unsigned int couleur)
{
    char nomFichier[20];
    string nomCouleur;
    char const * nomImage;
    nomCouleur = to_string(couleur);
    nomImage = nomCouleur.c_str();
    strcpy (nomFichier,"data/carte");
    strcat (nomFichier,nomImage);
    strcat (nomFichier,".png");
    im_carte.loadFromFile(nomFichier,renderer);
    im_carte.draw(renderer,1090,300,110,157);
    SDL_RenderPresent(renderer);
}

void sdlJeu::situationUno(Jeu & jeu)
{
    jeu.statut_Uno = true;
    bool attendre = true;
    SDL_Event event;
    int temps0 = SDL_GetTicks();
    while (attendre)
    {
        if (SDL_GetTicks() - temps0 < 2000) // temps inférieur à 2000 ms pour cliquer sur Uno
        {
            while (SDL_PollEvent(&event) && event.type == SDL_MOUSEBUTTONDOWN) 
            {
                int sourisX = event.button.x;
                int sourisY = event.button.y;
                if (sourisX>573 && sourisX<730 && sourisY>323 && sourisY<433) // clic sur Uno
                {
                    jeu.statut_Uno = false;
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                    SDL_Rect texte;
                    texte.x = 400;texte.y = 300;texte.w = 1200;texte.h = 400;
                    font_im.setSurface(TTF_RenderText_Solid(font,"UNOoooooo",jaune));
                    font_im.loadFromCurrentSurface(renderer);
                    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&texte);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(1000);
                    attendre = false;
                }
            }
        }
        else
        {
            attendre = false;
        }
    }
}

void sdlJeu::situationContreUno(Jeu & jeu)
{
    bool attendre = true;
    SDL_Event event;
    int temps0 = SDL_GetTicks();
    while (attendre)
    {
        if (SDL_GetTicks() - temps0 < 3000) // temps inférieur à 2000 ms pour cliquer sur Uno
        {
            while (SDL_PollEvent(&event) && event.type == SDL_MOUSEBUTTONDOWN) 
            {
                int sourisX = event.button.x;
                int sourisY = event.button.y;
                cout << sourisX << " " << sourisY << endl;
                if (sourisX>1270 && sourisX<1427 && sourisY>323 && sourisY<433) // clic sur Contre Uno
                {
                    jeu.statut_Uno = false;
                    cout << jeu.statut_Uno << "après clic" << endl;
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                    SDL_Rect texte;
                    texte.x = 400;texte.y = 300;texte.w = 1200;texte.h = 400;
                    font_im.setSurface(TTF_RenderText_Solid(font,"Contre UNOoooooo",jaune));
                    font_im.loadFromCurrentSurface(renderer);
                    SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&texte);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(1000);
                    attendre = false;
                }
            }
        }
        else
        {
            attendre = false;
        }
        
    }
}

void sdlJeu::sdlEchange(Jeu & jeu)
{
    SDL_Window * choixEchange = SDL_CreateWindow("Quel joueur?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 200, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (choixEchange == NULL) {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl; 
        SDL_Quit(); 
        exit(1);
    }

    SDL_Renderer * rendererEchange = SDL_CreateRenderer(choixEchange,-1,SDL_RENDERER_ACCELERATED);

    SDL_Rect texte;
    texte.x = 250;texte.y = 25;texte.w = 500;texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font,"Echange avec le joueur",jaune));
    font_im.loadFromCurrentSurface(rendererEchange);
    SDL_RenderCopy(rendererEchange,font_im.getTexture(),NULL,&texte);
    int pos=(1000-100*(jeu.nombreJoueurs-1))/2;
    for (int i=0; i<jeu.nombreJoueurs-1; i++)
    {
        texte.x = pos+100*i;texte.y = 125;texte.w = 100;texte.h = 50;
        if(i<jeu.joueurActif)
        {
            font_im.setSurface(TTF_RenderText_Solid(font,jeu.joueurs[i].nom.c_str(),jaune));
        }
        else
        {
            font_im.setSurface(TTF_RenderText_Solid(font,jeu.joueurs[i+1].nom.c_str(),jaune));
        }
        font_im.loadFromCurrentSurface(rendererEchange);
        SDL_RenderCopy(rendererEchange,font_im.getTexture(),NULL,&texte);
    }
    SDL_RenderPresent(rendererEchange);

    SDL_Event event;
    bool choixFait=false;
    int sourisX;
    int sourisY;
    unsigned int indice;

    while (!choixFait)
    {
        while (SDL_PollEvent(&event) && event.type == SDL_MOUSEBUTTONDOWN) 
        {
            sourisX = event.button.x;
            sourisY = event.button.y;
            cout << "choix joueurs " << sourisX << " " << sourisY << endl;
            if(event.type==SDL_MOUSEBUTTONDOWN)
            {
                if (sourisX>0 && sourisX<100*(jeu.nombreJoueurs-1) && sourisY>125 && sourisY<175) // clic sur un nom
                {
                    indice = (sourisX-pos)/100;
                    if (indice>=jeu.joueurActif) indice++;
                    choixFait = true;
                }
            }
        }
    }
    jeu.joueurs[jeu.joueurActif].main.swap(jeu.joueurs[indice].main);
    jeu.casPart = -1;
    jeu.termineTour();
    SDL_DestroyRenderer(rendererEchange);
    SDL_DestroyWindow(choixEchange);
}

void sdlJeu::sdlBoucleJeu (Jeu & jeu) 
{
     //Remplir l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    //sdlAffJoueurActif(jeu);

    SDL_Event event;
	bool quit = false;
    int sourisX;
    int sourisY;
    unsigned int couleur=0;
    bool couleurChangee = false;
    bool joueurChange = true;
    unsigned int indiceJoueur = jeu.joueurActif;

	// tant que ce n'est pas la fin ...
	while (!quit) {
        if (joueurChange)
        {
            sdlAffJoueur(jeu, jeu.joueurActif);
            if (couleur != 0 && couleurChangee) sdlAffCouleurChoisie(couleur);
            if (jeu.statut_Uno)
            {
                cout << jeu.statut_Uno << "avant situation Contre Uno" << endl;
                situationContreUno(jeu);
                cout << jeu.statut_Uno << "après situation Contre Uno" << endl;
                if (!jeu.statut_Uno)
                {
                    jeu.joueurs[indiceJoueur].main.push_back(jeu.pioche.top());
                    jeu.pioche.pop();
                    jeu.joueurs[indiceJoueur].main.push_back(jeu.pioche.top());
                    jeu.pioche.pop();
                }
                else jeu.statut_Uno = false;
            }
            
            indiceJoueur = jeu.joueurActif;
            joueurChange = false;
        }
        
        while (SDL_PollEvent(&event))
        {
            sourisX = event.button.x;
            sourisY = event.button.y;
            switch (event.type)
            {
                case SDL_QUIT:     // Si l'utilisateur a cliqué sur la croix de fermeture
                    quit = true;
                    break;          
			    case SDL_KEYDOWN:  // Si une touche est enfoncée
                    if(event.key.keysym.scancode==SDL_SCANCODE_Q) quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN: // Si le bouton de la souris est appuyé
                    joueurChange = true;
                    if (sourisX>800 && sourisX<910 && sourisY>300 && sourisY<457) // clic sur la pioche
                    {
                        jeu.piocherCarte();
                        jeu.termineTour();
                        sdlAffJoueur(jeu, indiceJoueur);

                        if (couleur != 0 && couleurChangee) sdlAffCouleurChoisie(couleur);
                    }
                    if (sourisX>0 && sourisX<1870 && sourisY>600 && sourisY<957) // clic sur une carte de la main
                    {
                        couleur = 0;
                        string messageErreur;
                        unsigned int indiceCarte;
                        if (sourisY<757) indiceCarte=sourisX/110;
                        if (sourisY>800) indiceCarte = 17+sourisX/110;
                        
                        if (jeu.carteValide(jeu.joueurs[jeu.joueurActif].main[indiceCarte])) 
                        {
                            couleurChangee = false;
                            if (jeu.joueurs[jeu.joueurActif].main[indiceCarte].getValeur()==13 ||jeu.joueurs[jeu.joueurActif].main[indiceCarte].getValeur()==14)
                            {
                                couleur = choixCouleur();
                                couleurChangee = true;
                                jeu.joueurs[jeu.joueurActif].main[indiceCarte].setCouleur(couleur);
                            }
                        }
                        jeu.poserCarte(indiceCarte, messageErreur);
                        if (jeu.casPart==3) sdlEchange(jeu);
                        sdlAffJoueur(jeu, indiceJoueur);
                        if (couleur != 0 && couleurChangee) sdlAffCouleurChoisie(couleur);
                        if (jeu.joueurs[indiceJoueur].main.size() == 1)
                        {
                           cout << jeu.statut_Uno << "avant situation Uno" << endl;
                           situationUno(jeu); 
                           cout << jeu.statut_Uno << "après situation Uno" << endl;
                        }    
                    }
                    break;
                default: break;
            }
        }
        SDL_Delay(500);

        if (jeu.joueurs[indiceJoueur].gagnant()) 
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_Rect texte;
            texte.x = 400;texte.y = 100;texte.w = 1200;texte.h = 400;
            font_im.setSurface(TTF_RenderText_Solid(font,"Good Game",jaune));
            font_im.loadFromCurrentSurface(renderer);
            SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&texte);
            texte.x = 500;texte.y = 500;texte.w = 800;texte.h = 400;
            font_im.setSurface(TTF_RenderText_Solid(font,jeu.joueurs[indiceJoueur].nom.c_str(),jaune));
            font_im.loadFromCurrentSurface(renderer);
            SDL_RenderCopy(renderer,font_im.getTexture(),NULL,&texte);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            quit=true;
        }
    }

}
