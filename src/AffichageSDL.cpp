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

#define Mix_LoadWAV(file) Mix_LoadWAV_RW(SDL_RWFromFile(file, "rb"), 1)
#define Mix_PlayChannel(channel, chunk, loops) Mix_PlayChannelTimed(channel, chunk, loops, -1)

template <typename T>
constexpr T LARGEUR_ECRAN{1920};

template <typename T>
constexpr T HAUTEUR_ECRAN{1080};

const int TAILLE_SPRITE = 32;

const SDL_Color noir = {0, 0, 0};
const SDL_Color bleu = {0, 0, 255};
const SDL_Color jaune = {255, 255, 0};

float temps()
{
    return float(SDL_GetTicks()) / CLOCKS_PER_SEC; // conversion des ms en secondes en divisant par 1000
}

// ============= CLASS IMAGE =============== //
//! \brief Pour gérer une image avec SDL2

Image::Image()
{
    surface = NULL;
    texture = NULL;
    has_changed = false;
}

void Image::loadFromFile(const char *filename, SDL_Renderer *renderer)
{
    surface = IMG_Load(filename);
    if (surface == NULL)
    {
        string nfn = string("../") + filename;
        cout << "Error: cannot load " << filename << ". Trying " << nfn << endl;
        surface = IMG_Load(nfn.c_str());
        if (surface == NULL)
        {
            nfn = string("../") + nfn;
            surface = IMG_Load(nfn.c_str());
        }
    }
    if (surface == NULL)
    {
        cout << "Error: cannot load " << filename << endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Surface *surfaceCorrectPixelFormat = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(surface);
    surface = surfaceCorrectPixelFormat;

    texture = SDL_CreateTextureFromSurface(renderer, surfaceCorrectPixelFormat);
    if (texture == NULL)
    {
        cout << "Error: problem to create the texture of " << filename << endl;
        SDL_Quit();
        exit(1);
    }
}

void Image::loadFromCurrentSurface(SDL_Renderer *renderer)
{
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        cout << "Error: problem to create the texture from surface " << endl;
        SDL_Quit();
        exit(1);
    }
}

void Image::draw(SDL_Renderer *renderer, int x, int y, int w, int h)
{
    int ok;
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = (w < 0) ? surface->w : w;
    r.h = (h < 0) ? surface->h : h;

    if (has_changed)
    {
        ok = SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
        assert(ok == 0);
        has_changed = false;
    }

    ok = SDL_RenderCopy(renderer, texture, NULL, &r);
    assert(ok == 0);
}

SDL_Texture *Image::getTexture() const { return texture; }

void Image::setSurface(SDL_Surface *surf) { surface = surf; }

// ============= CLASS SDLJEU =============== //

/**
    La classe g�rant le jeu avec un affichage SDL
*/

sdlJeu::sdlJeu() : window(nullptr), renderer(nullptr), font(nullptr)
{
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    if (TTF_Init() != 0)
    {
        cout << "Erreur lors de l'initialisation de la SDL_ttf : " << TTF_GetError() << endl;
        SDL_Quit();
        exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        cout << "No sound !!!" << endl;
        this->~sdlJeu();
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        SDL_Quit();
        exit(1);
    }
    HauteurEcran = 0;
    LargeurEcran = 0;
    lireFichierRes(); // Lis le fichier et initialise les données membres pour la fenêtre.

    //HauteurEcran = 1080;
    //LargeurEcran = 1920;

    cout << HauteurEcran << endl;
    cout << LargeurEcran << endl;

    // Creation de la fenetre
    window = SDL_CreateWindow("UNO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LargeurEcran, HauteurEcran, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // IMAGES
    fondMenu.loadFromFile("data/uno1.bmp", renderer);

    // FONTS
    font = TTF_OpenFont("data/DejaVuSansCondensed.ttf", 100);
    if (font == NULL)
    {
        cout << "Failed to load DejaVuSansCondensed.ttf! SDL_TTF Error: " << TTF_GetError() << endl;
        SDL_Quit();
        exit(1);
    }
    fontTexte = TTF_OpenFont("data/DejaVuSansCondensed.ttf", 60);

    //SDL_SetRenderDrawColor(renderer, 168, 230, 255, 255);
    //SDL_RenderClear(renderer);

    // Son
    Mix_AllocateChannels(3);                    // Alloue 3 cannaux
    choixVolume = 5;                            // Notre échelle étant de 0 à 10.
    volume = MIX_MAX_VOLUME / 30 * choixVolume; // On met le son à 5 de base (Moitié).

    cout << volume << endl;
    Mix_Volume(0, volume);             // Mets la musique salle attente et de Jeu au niveau volume.
    Mix_Volume(1, MIX_MAX_VOLUME / 4); // Mets le son 25 %

    sons[0] = Mix_LoadWAV("data/sounds/musiqueMenu.wav");
    sons[1] = Mix_LoadWAV("data/sounds/passageSalleAttente.wav");
    sons[2] = Mix_LoadWAV("data/sounds/selection.wav"); // On charge la musique.
    sons[3] = Mix_LoadWAV("data/sounds/valide.wav");
    for (int i = 0; i < 4; i++)
    {
        if (sons[i] == NULL)
        {
            cout << "Failed to load son.wav! SDL_mixer Error: " << Mix_GetError() << endl;
            this->~sdlJeu();
            exit(1);
        }
    }
    Mix_PlayChannel(0, sons[0], 1);
}

sdlJeu::~sdlJeu()
{
    TTF_CloseFont(font);
    TTF_CloseFont(fontTexte);
    for (int i = 0; i < 3; i++)
        Mix_FreeChunk(sons[i]); // Libére la mémoire allouer pour le son
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void sdlJeu::lireFichierRes()
{                               // Lis le fichier Résolution pour définir résolution de la fenêtre au démarrage.
    string ligne;               // La ligne lue dans le fichier.
    stringstream largeurStream; // Permettra de convertir notre string en entier.
    stringstream hauteurStream; // Permettra de convertir notre string en entier.
    ifstream monFichier("resolution.txt");
    if (monFichier.is_open())
    {
        getline(monFichier, ligne); // donne le rés de la première ligne.
        largeurStream << ligne;
        largeurStream >> LargeurEcran;
        getline(monFichier, ligne); // donne le rés de la première ligne.
        hauteurStream << ligne;
        hauteurStream >> HauteurEcran;
        monFichier.close();
    }
    else
    {
        cout << "Erreur ouverture du fichier" << endl;
        exit(EXIT_FAILURE);
    }
}
void sdlJeu::modifFichierRes(int largeur, int hauteur)
{
    ofstream monFichier("resolution.txt");
    if (monFichier.is_open())
    {
        monFichier << to_string(largeur) + "\n";
        monFichier << to_string(hauteur) + "\n";
        monFichier.close();
    }
    else
    {
        cout << "Le fichier n'a pas pu être modifier." << endl;
        exit(EXIT_FAILURE);
    }
}

// Affiche la salle d'attente
void sdlJeu::sdlAffSalleAttente(SDL_Window *param, SDL_Renderer *rendererParam, unsigned int variante, unsigned int nombreJoueurs, unsigned int nombreIA)
{
    SDL_SetRenderDrawColor(rendererParam, 0, 0, 0, 255);
    SDL_RenderClear(rendererParam);

    // Texte "Jeu"
    SDL_Rect texte;
    texte.x = 100;
    texte.y = 50;
    texte.w = 100;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Jeu : ", jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam, font_im.getTexture(), NULL, &texte);

    texte.x = 220;
    texte.y = 50;
    texte.w = 200;
    texte.h = 50;
    switch (variante)
    {
    case 1:
        font_im.setSurface(TTF_RenderText_Solid(font, "Jeu Classique", jaune));
        break;
    case 2:
        font_im.setSurface(TTF_RenderText_Solid(font, "Variante Cumul", jaune));
        break;
    case 3:
        font_im.setSurface(TTF_RenderText_Solid(font, "Variante Doublon", jaune));
        break;
    case 4:
        font_im.setSurface(TTF_RenderText_Solid(font, "Variante Echange", jaune));
        break;
    case 5:
        font_im.setSurface(TTF_RenderText_Solid(font, "Variante Suite", jaune));
        break;
    case 6:
        font_im.setSurface(TTF_RenderText_Solid(font, "Variante Tourne", jaune));
        break;
    default:
        break;
    }
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam, font_im.getTexture(), NULL, &texte);

    // Texte "Nombre de joueurs"
    texte.x = 100;
    texte.y = 150;
    texte.w = 300;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Nombre de joueurs : ", jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam, font_im.getTexture(), NULL, &texte);

    // Texte qui récupère la variable nombreJoueur et affiche le numéro.
    texte.x = 420;
    texte.y = 150;
    texte.w = 50;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, to_string(nombreJoueurs).c_str(), jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam, font_im.getTexture(), NULL, &texte);

    // Texte "Nombre d'ordinateurs"
    texte.x = 100;
    texte.y = 250;
    texte.w = 300;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Nombre d'ordinateurs : ", jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam, font_im.getTexture(), NULL, &texte);

    // Texte qui récupère la variable nombreIA et affiche le numéro.
    texte.x = 420;
    texte.y = 250;
    texte.w = 50;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, to_string(nombreIA).c_str(), jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam, font_im.getTexture(), NULL, &texte);

    // Texte "Lancer le jeu"
    texte.x = 100;
    texte.y = 350;
    texte.w = 300;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Lancer le jeu", jaune));
    font_im.loadFromCurrentSurface(rendererParam);
    SDL_RenderCopy(rendererParam, font_im.getTexture(), NULL, &texte);

    SDL_RenderPresent(rendererParam);
}

// Préparation de la partie
void sdlJeu::sdlUno()
{
    //Remplir l'écran de noir
    SDL_SetRenderDrawColor(renderer, 254, 254, 254, 255);
    SDL_RenderClear(renderer);

    fondMenu.draw(renderer, 0, 0, 2000, 1000);
    SDL_RenderPresent(renderer);

    SDL_Window *param = SDL_CreateWindow("Paramètres du jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (param == NULL)
    {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }
    SDL_Renderer *rendererParam = SDL_CreateRenderer(param, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(rendererParam);

    SDL_Event event;
    bool quit = false;

    /*Uint32 t = SDL_GetTicks(), nt;*/

    unsigned int variante = 1;      // entre 1 et 6
    unsigned int nombreJoueurs = 2; // entre 1 et 9
    unsigned int nombreIA = 0;      //entre 0 et 2
    bool choix;
    int sourisX;
    int sourisY;

    sdlAffSalleAttente(param, rendererParam, variante, nombreJoueurs, nombreIA);
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
            case SDL_WINDOWEVENT:                                // On clique sur la fenêtre.
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) // On appuie sur la croix pour fermer le Jeu.
                    return;                                      // On arrête le Jeu.
                break;
            case SDL_KEYDOWN:                                         // Si une touche est enfoncee
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) // Si la touche est ESC on ferme le Jeu.
                    return;
                break;
            case SDL_MOUSEBUTTONDOWN: // Si le bouton de la souris est relevé
                cout << "case" << endl;
                if (sourisX > 100 && sourisX < 320 && sourisY > 50 && sourisY < 100) // clic sur la ligne "Jeu"
                {
                    variante = sdlAffChoixJeu();
                    sdlAffSalleAttente(param, rendererParam, variante, nombreJoueurs, nombreIA);
                    cout << "variante=" << variante << endl;
                }
                if (sourisX > 100 && sourisX < 470 && sourisY > 150 && sourisY < 200) // clic sur la ligne "Nombre de joueurs"
                {
                    nombreJoueurs = sdlAffChoixJoueurs();
                    cout << "nombre joueurs " << nombreJoueurs << endl;
                    sdlAffSalleAttente(param, rendererParam, variante, nombreJoueurs, nombreIA);
                }
                if (event.button.x > 100 && event.button.x < 470 && event.button.y > 250 && event.button.y < 300) // clic sur la ligne "Nombre d'ordinateurs"
                {
                    nombreIA = sdlAffChoixOrdinateurs();
                    cout << "nombre bots " << nombreIA << endl;
                    sdlAffSalleAttente(param, rendererParam, variante, nombreJoueurs, nombreIA);
                }
                if (event.button.x > 100 && event.button.x < 400 && event.button.y > 350 && event.button.y < 400) // clic sur la ligne "Lancer le jeu"
                {
                    SDL_DestroyRenderer(rendererParam);
                    SDL_DestroyWindow(param);
                    quit = true;
                }

                break;
                break;
            default:
                break;
            }
        }
    }
    switch (variante)
    {
    case 1:
    {
        Jeu jeu(nombreJoueurs, nombreIA);
        sdlBoucleJeu(jeu);
        break;
    }
    case 2:
    {
        VarianteCumul jeu(nombreJoueurs, nombreIA);
        sdlBoucleJeu(jeu);
        break;
    }
    case 3:
    {
        VarianteDoublon jeu(nombreJoueurs, nombreIA);
        sdlBoucleJeu(jeu);
        break;
    }
    case 4:
    {
        VarianteEchange jeu(nombreJoueurs, nombreIA);
        sdlBoucleJeu(jeu);
        break;
    }
    case 5:
    {
        VarianteSuite jeu(nombreJoueurs, nombreIA);
        sdlBoucleJeu(jeu);
        break;
    }
    case 6:
    {
        VarianteTourne jeu(nombreJoueurs, nombreIA);
        sdlBoucleJeu(jeu);
        break;
    }
    default:
        break;
    }
}

// Fait choisir la variante
unsigned int sdlJeu::sdlAffChoixJeu()
{
    unsigned int choixVariante;
    SDL_Window *choixJeu = SDL_CreateWindow("Choix du jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (choixJeu == NULL)
    {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Renderer *rendererJeu = SDL_CreateRenderer(choixJeu, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect variante;
    variante.x = 50;
    variante.y = 50;
    variante.w = 300;
    variante.h = 50;
    font_color.r = 0;
    font_color.g = 0;
    font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font, "Classique", jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu, font_im.getTexture(), NULL, &variante);

    variante.x = 50;
    variante.y = 120;
    variante.w = 300;
    variante.h = 50;
    font_color.r = 0;
    font_color.g = 0;
    font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font, "Variante Cumul", jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu, font_im.getTexture(), NULL, &variante);

    variante.x = 50;
    variante.y = 190;
    variante.w = 300;
    variante.h = 50;
    font_color.r = 0;
    font_color.g = 0;
    font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font, "Variante Doublon", jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu, font_im.getTexture(), NULL, &variante);

    variante.x = 50;
    variante.y = 260;
    variante.w = 300;
    variante.h = 50;
    font_color.r = 0;
    font_color.g = 0;
    font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font, "Variante Echange", jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu, font_im.getTexture(), NULL, &variante);

    variante.x = 50;
    variante.y = 330;
    variante.w = 300;
    variante.h = 50;
    font_color.r = 0;
    font_color.g = 0;
    font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font, "Variante Suite", jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu, font_im.getTexture(), NULL, &variante);

    variante.x = 50;
    variante.y = 400;
    variante.w = 300;
    variante.h = 50;
    font_color.r = 0;
    font_color.g = 0;
    font_color.b = 255;
    font_im.setSurface(TTF_RenderText_Solid(font, "Variante Tourne", jaune));
    font_im.loadFromCurrentSurface(rendererJeu);
    SDL_RenderCopy(rendererJeu, font_im.getTexture(), NULL, &variante);

    SDL_RenderPresent(rendererJeu);

    SDL_Event eventJeu;
    bool choixFait = false;
    int sourisX;
    int sourisY;

    while (!choixFait)
    {
        while (SDL_PollEvent(&eventJeu) && eventJeu.type == SDL_MOUSEBUTTONDOWN)
        {
            sourisX = eventJeu.button.x;
            sourisY = eventJeu.button.y;
            cout << "choix variante " << sourisX << " " << sourisY << endl;
            if (eventJeu.type == SDL_MOUSEBUTTONDOWN)
            {
                if (sourisX > 50 && sourisX < 350 && sourisY > 50 && sourisY < 450) // clic sur une variante
                {
                    choixVariante = (sourisY - 50) / 70 + 1;
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
    SDL_Window *choixJoueurs = SDL_CreateWindow("Choix du nombre de joueurs", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (choixJoueurs == NULL)
    {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Renderer *rendererJoueurs = SDL_CreateRenderer(choixJoueurs, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect choix;
    choix.x = 50;
    choix.y = 200;
    choix.w = 1000;
    choix.h = 500;
    SDL_SetRenderDrawColor(rendererJoueurs, 0, 0, 0, 255);
    SDL_RenderFillRect(rendererJoueurs, &choix);
    for (int i = 0; i < 9; i++)
    {
        SDL_Rect paramJoueurs;
        paramJoueurs.x = 50 + 100 * i;
        paramJoueurs.y = 200;
        paramJoueurs.w = 50;
        paramJoueurs.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font, to_string(i + 1).c_str(), jaune));
        font_im.loadFromCurrentSurface(rendererJoueurs);
        SDL_RenderCopy(rendererJoueurs, font_im.getTexture(), NULL, &paramJoueurs);
    }
    SDL_RenderPresent(rendererJoueurs);

    SDL_Event eventJoueurs;
    bool choixFait = false;
    int sourisX;
    int sourisY;

    while (!choixFait)
    {
        while (SDL_PollEvent(&eventJoueurs) && eventJoueurs.type == SDL_MOUSEBUTTONDOWN)
        {
            sourisX = eventJoueurs.button.x;
            sourisY = eventJoueurs.button.y;
            cout << "choix joueurs " << sourisX << " " << sourisY << endl;
            if (eventJoueurs.type == SDL_MOUSEBUTTONDOWN)
            {
                if (sourisX > 50 && sourisX < 900 && sourisY > 200 && sourisY < 250) // clic sur une variante
                {
                    Joueurs = (sourisX - 50) / 100 + 1;
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
    SDL_Window *choixBots = SDL_CreateWindow("Choix du nombre d'ordinateurs", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (choixBots == NULL)
    {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Renderer *rendererBots = SDL_CreateRenderer(choixBots, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect choix;
    choix.x = 500;
    choix.y = 200;
    choix.w = 1000;
    choix.h = 500;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &choix);
    for (int i = 0; i < 3; i++)
    {
        SDL_Rect paramBots;
        paramBots.x = 150 + 100 * i;
        paramBots.y = 200;
        paramBots.w = 50;
        paramBots.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font, to_string(i).c_str(), jaune));
        font_im.loadFromCurrentSurface(rendererBots);
        SDL_RenderCopy(rendererBots, font_im.getTexture(), NULL, &paramBots);
    }
    SDL_RenderPresent(rendererBots);

    SDL_Event eventBots;
    bool choixFait = false;
    int sourisX;
    int sourisY;

    while (!choixFait)
    {
        while (SDL_PollEvent(&eventBots) && eventBots.type == SDL_MOUSEBUTTONDOWN)
        {
            sourisX = eventBots.button.x;
            sourisY = eventBots.button.y;
            cout << "choix bots " << sourisX << " " << sourisY << endl;
            if (eventBots.type == SDL_MOUSEBUTTONDOWN)
            {
                if (sourisX > 50 && sourisX < 450 && sourisY > 200 && sourisY < 250) // clic sur une variante
                {
                    nombreIA = (sourisX - 150) / 100;
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
void sdlJeu::sdlAffCarte(const Carte &c, int positionX, int positionY)
{
    int carteEntier;
    char nomFichier[20];
    string nomCarte;
    char const *nomImage;
    if (c.getValeur() == 13)
    {
        im_carte.loadFromFile("data/+4.png", renderer);
        im_carte.draw(renderer, positionX, positionY, 110, 157);
    }
    if (c.getValeur() == 14)
    {
        im_carte.loadFromFile("data/joker.png", renderer);
        im_carte.draw(renderer, positionX, positionY, 110, 157);
    }
    if (c.getValeur() <= 12)
    {
        carteEntier = 10 * c.getValeur() + c.getCouleur();
        nomCarte = to_string(carteEntier);
        nomImage = nomCarte.c_str();
        strcpy(nomFichier, "data/");
        strcat(nomFichier, nomImage);
        strcat(nomFichier, ".png");
        im_carte.loadFromFile(nomFichier, renderer);
        im_carte.draw(renderer, positionX, positionY, 110, 157);
    }
}

// Affiche la table du joueur, indiceJoueur est l'indice dans le tableau jeu.joueurs
void sdlJeu::sdlAffJoueur(Jeu &jeu, unsigned int indiceJoueur)
{
    //Remplir l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect texte;

    // affiche les adversaires
    for (unsigned int i = 0; i < jeu.nombreJoueurs + jeu.nombreIA - 1; i++)
    {
        int positionCarte = (2000 - (130 * (jeu.nombreJoueurs + jeu.nombreIA - 1) - 20)) / 2 + 130 * i;

        // nom adversaire
        texte.x = positionCarte;
        texte.y = 0;
        texte.w = 110;
        texte.h = 50;
        font_im.setSurface(TTF_RenderText_Solid(font, jeu.joueurs[(jeu.joueurActif + 1 + i) % (jeu.nombreJoueurs + jeu.nombreIA)].nom.c_str(), jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer, font_im.getTexture(), NULL, &texte);

        // carte adversaire
        im_carte.loadFromFile("data/dosvide.png", renderer);
        im_carte.draw(renderer, positionCarte, 50, 110, 157);

        // nombre de cartes adversaires
        texte.x = positionCarte + 40;
        texte.y = 113;
        texte.w = 30;
        texte.h = 30;
        font_im.setSurface(TTF_RenderText_Solid(font, to_string(jeu.joueurs[(jeu.joueurActif + 1 + i) % (jeu.nombreJoueurs + jeu.nombreIA)].main.size()).c_str(), jaune));
        font_im.loadFromCurrentSurface(renderer);
        SDL_RenderCopy(renderer, font_im.getTexture(), NULL, &texte);
    }

    // affiche le centre
    // pioche
    im_carte.loadFromFile("data/dos.png", renderer);
    im_carte.draw(renderer, 800, 300, 110, 157);

    // talon
    sdlAffCarte(jeu.talon.back(), 1090, 300);

    // Uno
    im_carte.loadFromFile("data/carteuno.png", renderer);
    im_carte.draw(renderer, 573, 323, 157, 110);

    texte.x = 626;
    texte.y = 353;
    texte.w = 50;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Uno", noir));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer, font_im.getTexture(), NULL, &texte);

    // Contre Uno
    im_carte.loadFromFile("data/cartecontreuno.png", renderer);
    im_carte.draw(renderer, 1270, 323, 157, 110);

    texte.x = 1318;
    texte.y = 338;
    texte.w = 80;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Contre", noir));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer, font_im.getTexture(), NULL, &texte);
    texte.x = 1313;
    texte.y = 373;
    texte.w = 50;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Uno", noir));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer, font_im.getTexture(), NULL, &texte);

    // affiche le nom et la main du joueur actif

    // le nom
    texte.x = 0;
    texte.y = 550;
    texte.w = 110;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, jeu.joueurs[indiceJoueur].nom.c_str(), jaune));
    font_im.loadFromCurrentSurface(renderer);
    SDL_RenderCopy(renderer, font_im.getTexture(), NULL, &texte);

    // la main
    unsigned int tailleMain = jeu.joueurs[indiceJoueur].main.size();
    for (int i = 0; i < tailleMain; i++)
    {
        sdlAffCarte(jeu.joueurs[indiceJoueur].main[i], 110 * (i % 17), 600 + 200 * (i / 17));
    }
    SDL_RenderPresent(renderer);
}

// Fait choisir la couleur à jouer (après joker et +4)
unsigned int sdlJeu::choixCouleur()
{
    SDL_Window *choixCouleur = SDL_CreateWindow("Quelle couleur?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 200, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (choixCouleur == NULL)
    {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Renderer *rendererCouleur = SDL_CreateRenderer(choixCouleur, -1, SDL_RENDERER_ACCELERATED);

    Image im_choix;
    im_choix.loadFromFile("data/questioncouleur.png", rendererCouleur);
    im_choix.draw(rendererCouleur, 50, 21, 110, 157);

    SDL_Rect texte;
    texte.x = 210;
    texte.y = 75;
    texte.w = 100;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Rouge", jaune));
    font_im.loadFromCurrentSurface(rendererCouleur);
    SDL_RenderCopy(rendererCouleur, font_im.getTexture(), NULL, &texte);

    texte.x = 340;
    texte.y = 75;
    texte.w = 100;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Vert", jaune));
    font_im.loadFromCurrentSurface(rendererCouleur);
    SDL_RenderCopy(rendererCouleur, font_im.getTexture(), NULL, &texte);

    texte.x = 470;
    texte.y = 75;
    texte.w = 100;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Bleu", jaune));
    font_im.loadFromCurrentSurface(rendererCouleur);
    SDL_RenderCopy(rendererCouleur, font_im.getTexture(), NULL, &texte);

    texte.x = 600;
    texte.y = 75;
    texte.w = 100;
    texte.h = 50;
    font_im.setSurface(TTF_RenderText_Solid(font, "Jaune", jaune));
    font_im.loadFromCurrentSurface(rendererCouleur);
    SDL_RenderCopy(rendererCouleur, font_im.getTexture(), NULL, &texte);

    SDL_RenderPresent(rendererCouleur);

    SDL_Event eventCouleur;
    bool choixFait = false;
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
            if (eventCouleur.type == SDL_MOUSEBUTTONDOWN)
            {
                if (sourisX > 210 && sourisX < 700 && sourisY > 75 && sourisY < 232) // clic sur une couleur
                {
                    couleur = (sourisX - 210) / 130 + 1;
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
    char const *nomImage;
    nomCouleur = to_string(couleur);
    nomImage = nomCouleur.c_str();
    strcpy(nomFichier, "data/carte");
    strcat(nomFichier, nomImage);
    strcat(nomFichier, ".png");
    im_carte.loadFromFile(nomFichier, renderer);
    im_carte.draw(renderer, 1090, 300, 110, 157);
    SDL_RenderPresent(renderer);
}

void sdlJeu::situationUno(Jeu &jeu)
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
                if (sourisX > 573 && sourisX < 730 && sourisY > 323 && sourisY < 433) // clic sur Uno
                {
                    jeu.statut_Uno = false;
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                    SDL_Rect texte;
                    texte.x = 400;
                    texte.y = 300;
                    texte.w = 1200;
                    texte.h = 400;
                    font_im.setSurface(TTF_RenderText_Solid(font, "UNOoooooo", jaune));
                    font_im.loadFromCurrentSurface(renderer);
                    SDL_RenderCopy(renderer, font_im.getTexture(), NULL, &texte);
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

void sdlJeu::sdlMenu()
{
    SDL_Event events;
    bool isOpen{true};
    bool openReglage{false};
    Menu menu;
    // Police des textes
    TTF_Font *fontMenuTitre = TTF_OpenFont("data/DejaVuSansCondensed.ttf", 110);
    if (LargeurEcran == 1920 && HauteurEcran == 1080)
    {
        fontTexte = TTF_OpenFont("data/DejaVuSansCondensed.ttf", 80);
    }

    // Créer tous les textes pour le Menu en allant récupérer dans le vecteur string dans la classe Menu.
    SDL_Surface *surfTexte[5];
    surfTexte[0] = TTF_RenderText_Blended(fontMenuTitre, "Uno", SDL_Color{255, 255, 255, 255});
    for (int i = 0; i < 4; i++)
    {
        surfTexte[i + 1] = TTF_RenderText_Blended(fontTexte, (menu.getNomMenu(i)).c_str(), SDL_Color{255, 255, 255, 255});
    }
    // Creations des textures pour les textes.
    SDL_Texture *textureTexte[5];
    for (int i = 0; i < 5; i++)
    {
        textureTexte[i] = SDL_CreateTextureFromSurface(renderer, surfTexte[i]);
    }

    // Rectangle pour régler la position des texte.
    SDL_Rect positionTexte[5];
    for (int i = 0; i < 5; i++)
    {
        SDL_QueryTexture(textureTexte[i], nullptr, nullptr, &positionTexte[i].w, &positionTexte[i].h); // Récupere la dimension de la texture
    }

    // Centre la texture du titre sur l'écran avec le rect.
    positionTexte[0].x = (LargeurEcran / 2 - positionTexte[0].w / 2); // on soustrait pour que le texte soit alignée.
    positionTexte[0].y = HauteurEcran / 14;

    // On définie les coordo des textes Jouer, Par rapport à la coordo du titre.
    positionTexte[1].x = positionTexte[0].x + LargeurEcran / 6 - positionTexte[0].x / 2.7; // Le x est toujours le même pour tous les textes.
    positionTexte[1].y = positionTexte[0].y + HauteurEcran / 4;

    // Reglage
    positionTexte[2].x = positionTexte[0].x + LargeurEcran / 6 - positionTexte[0].x / 2.2; // Le x est toujours le même pour tous les textes.
    positionTexte[2].y = positionTexte[0].y + HauteurEcran / 2.6;

    // Regles
    positionTexte[3].x = positionTexte[0].x + LargeurEcran / 6 - positionTexte[0].x / 2.45; // Le x est toujours le même pour tous les textes.
    positionTexte[3].y = positionTexte[0].y + HauteurEcran / 1.9;

    // Quitter
    positionTexte[4].x = positionTexte[0].x + LargeurEcran / 6 - positionTexte[0].x / 2.4; // Le x est toujours le même pour tous les textes.
    positionTexte[4].y = positionTexte[0].y + HauteurEcran / 1.5;

    // On libère la surface.
    for (int i = 0; i < 5; i++)
    {
        SDL_FreeSurface(surfTexte[i]);
    }

    TTF_CloseFont(fontMenuTitre);

    // Chemin + nom Image à charger pour animation.
    string nomImageBase = "data/UnoAnim/76061103e5c846bf867d0ee82f9fcbbd42WnmC6xmFteZPZe-";
    // Variable qui contiendra le nom finale.
    string imageACharger;

    // On créer un tableau de surface et un de texture.
    SDL_Surface *image[15];
    SDL_Texture *textImage[15]; // On créer un tableau de Texture pour chaque image.

    for (int i = 0; i < 15; i++)
    { // On charge les 15 images pour faire le gif.
        imageACharger = nomImageBase + to_string(i) + ".bmp";
        // On convertit en char pour la fonction IMG_LOAD
        image[i] = IMG_Load(imageACharger.c_str());
        if (image == NULL) // On regarde si on a réussi à charger les images.
        {
            cout << "Error: cannot load " << imageACharger << endl;
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
        textImage[i] = SDL_CreateTextureFromSurface(renderer, image[i]);
        SDL_FreeSurface(image[i]); // On libère l'espace de la surface.
    }

    // Rectangle destionation pour la position image dans la fenêtre.
    SDL_Rect dstrect = {
        0,
        0,
        100, 100};
    // Nous permettra de charger chaque image dans le tableau image.
    dstrect.y = HauteurEcran / 3.1;
    int k = 0;
    // Variable qui contiendront la position de la souris.
    int posSourisX;
    int posSourisY;

    // On récupère les événements.
    while (isOpen)
    {
        dstrect.x = (LargeurEcran / 2 + LargeurEcran / 15);

        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_WINDOWEVENT:
                if (events.window.event == SDL_WINDOWEVENT_CLOSE)
                    isOpen = false;
                break;
            case SDL_MOUSEBUTTONDOWN: // Click de souris
                SDL_Log("+clic");
                posSourisX = events.button.x;
                posSourisY = events.button.y;
                //int posReglageX = positionReglage.x;
                //int posReglageY = positionReglage.y;

                //posReglageY += +HAUTEUR_ECRAN<int> / 12;

                //cout << posReglageX << " " << posReglageY << endl;
                //cout << posSourisX << " " << posSourisY << endl;

                // On clique sur jouer et ça lance la salle d'attente (voir si il faut pas enlever des trucs).
                if ((posSourisX > positionTexte[1].x && posSourisY > positionTexte[1].y) &&                              // Point en haut à gauche
                    (posSourisX > positionTexte[1].x && posSourisY < positionTexte[1].y + HAUTEUR_ECRAN<int> / 12) &&    // Point en bas à gauche
                    (posSourisX < (positionTexte[1].x + LARGEUR_ECRAN<int> / 8) && (posSourisY > positionTexte[1].y)) && // Point en haut à droite
                    (posSourisX < (positionTexte[1].x + LARGEUR_ECRAN<int> / 8) && (posSourisY < positionTexte[1].y + HAUTEUR_ECRAN<int> / 12)))
                {
                    cout << "je clique sur Jouer." << endl;
                    //Mix_PlayChannel(1, sons[1], 0); // On joue le son salleAttente.
                }
                // On clique sur Reglage
                if ((posSourisX > positionTexte[2].x && posSourisY > positionTexte[2].y) &&                              // Point en haut à gauche
                    (posSourisX > positionTexte[2].x && posSourisY < positionTexte[2].y + HAUTEUR_ECRAN<int> / 12) &&    // Point en bas à gauche
                    (posSourisX < (positionTexte[2].x + LARGEUR_ECRAN<int> / 8) && (posSourisY > positionTexte[2].y)) && // Point en haut à droite
                    (posSourisX < (positionTexte[2].x + LARGEUR_ECRAN<int> / 8) && (posSourisY < positionTexte[2].y + HAUTEUR_ECRAN<int> / 12)))
                {
                    cout << "Je clique sur réglage" << endl;
                    isOpen = false; // On quitte la boucle.
                    openReglage = true;
                    Mix_PlayChannel(1, sons[2], 0); // On joue le son selection 1 fois.
                }
                // On clique sur Regles
                if ((posSourisX > positionTexte[3].x && posSourisY > positionTexte[3].y) &&                              // Point en haut à gauche
                    (posSourisX > positionTexte[3].x && posSourisY < positionTexte[3].y + HAUTEUR_ECRAN<int> / 12) &&    // Point en bas à gauche
                    (posSourisX < (positionTexte[3].x + LARGEUR_ECRAN<int> / 8) && (posSourisY > positionTexte[3].y)) && // Point en haut à droite
                    (posSourisX < (positionTexte[3].x + LARGEUR_ECRAN<int> / 8) && (posSourisY < positionTexte[3].y + HAUTEUR_ECRAN<int> / 12)))
                {
                    cout << "Je clique sur Regles" << endl;
                    Mix_PlayChannel(1, sons[2], 0); // On joue le son selection 1 fois.
                }
                // On clique sur quitter
                if ((posSourisX > positionTexte[4].x && posSourisY > positionTexte[4].y) &&                              // Point en haut à gauche
                    (posSourisX > positionTexte[4].x && posSourisY < positionTexte[4].y + HAUTEUR_ECRAN<int> / 12) &&    // Point en bas à gauche
                    (posSourisX < (positionTexte[4].x + LARGEUR_ECRAN<int> / 8) && (posSourisY > positionTexte[4].y)) && // Point en haut à droite
                    (posSourisX < (positionTexte[4].x + LARGEUR_ECRAN<int> / 8) && (posSourisY < positionTexte[4].y + HAUTEUR_ECRAN<int> / 12)))
                {
                    cout << "Je clique Quitter" << endl;
                    isOpen = false;
                }
                break;
            }
        }

        // On met à jour l'affichage.
        SDL_SetRenderDrawColor(renderer, 254, 254, 254, 255); // Affiche la couleur de fond.
        SDL_RenderClear(renderer);

        fondMenu.draw(renderer, 0, 0, LargeurEcran, HauteurEcran);
        // Affichage texte, ici du titre.
        //SDL_RenderClear(renderer); // On change sur quelle rendu on fait.
        // Affichage des Textes
        for (int i = 0; i < 5; i++)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // On passe sur la couleur du texte.
            SDL_RenderCopy(renderer, textureTexte[i], nullptr, &positionTexte[i]);
        }

        SDL_RenderCopy(renderer, textImage[k], nullptr, &dstrect); // Affichage Image.
        k++;
        SDL_Delay(50);
        if (k == 14)
            k = 0;

        SDL_RenderPresent(renderer);
    }
    for (int i = 0; i < 5; i++)
    {
        SDL_DestroyTexture(textureTexte[i]);
    }
    if (openReglage)
    {
        sdlReglage(menu);
    }
}

void sdlJeu::sdlReglage(Menu &menu)
{
    SDL_Event events;
    bool isOpen{true};
    bool openMenu{false};

    // Police des textes
    TTF_Font *fontResText = TTF_OpenFont("data/DejaVuSansCondensed.ttf", 45);

    // Créer tous les textes pour les réglages en allant récupérer dans le vecteur string dans la classe Menu.
    SDL_Surface *tabSurfTitre[4];
    for (int i = 0; i < 4; i++)
    {
        tabSurfTitre[i] = TTF_RenderText_Blended(fontTexte, (menu.getOptions(i)).c_str(), SDL_Color{255, 255, 255, 255});
    }
    if (LargeurEcran == 800 && HauteurEcran == 800)
    {
        tabSurfTitre[3] = TTF_RenderText_Blended(fontResText, (menu.getOptions(3)).c_str(), SDL_Color{255, 255, 255, 255});
    }
    // Textes pour les résolutions d'écran.
    SDL_Surface *tabRes[3];
    for (int i = 0; i < 3; i++)
    {
        tabRes[i] = TTF_RenderText_Blended(fontResText, (menu.getResolution(i)).c_str(), SDL_Color{255, 255, 255, 255});
        ;
    }
    // Creations des textures pour les textes.
    SDL_Texture *tabTextureTitre[4];
    for (int i = 0; i < 4; i++)
    {
        tabTextureTitre[i] = SDL_CreateTextureFromSurface(renderer, tabSurfTitre[i]); // Crée la texture qu'on va afficher a partir de la surface
    }

    // Résolutions
    SDL_Texture *tabTextRes[3];
    for (int i = 0; i < 3; i++)
    {
        tabTextRes[i] = SDL_CreateTextureFromSurface(renderer, tabRes[i]); // Crée la texture qu'on va afficher a partir de la surface
    }
    // Creation des textes pour régler le volume du son.
    SDL_Texture *tabSon[11];
    SDL_Surface *surfEchelle;
    SDL_Rect positionEchelle[11];

    for (int i = 0; i < 11; i++)
    {
        surfEchelle = TTF_RenderText_Blended(fontResText, (menu.getSon(i)).c_str(), SDL_Color{255, 255, 255, 255});
        tabSon[i] = SDL_CreateTextureFromSurface(renderer, surfEchelle);
        SDL_QueryTexture(tabSon[i], nullptr, nullptr, &positionEchelle[i].w, &positionEchelle[i].h);
    }

    // Rectangle pour régler la position des textes.
    SDL_Rect tabPositionTitre[4];
    // Postion Résolution texte.
    SDL_Rect tabPositionRes[3];
    SDL_Rect sonSelectionne; // Encadre le son sur lequelle on clique ou qui est déjà actif.

    for (int i = 0; i < 4; i++)
    {
        SDL_QueryTexture(tabTextureTitre[i], nullptr, nullptr, &tabPositionTitre[i].w, &tabPositionTitre[i].h); // Récupere la dimension de la texture
    }

    for (int i = 0; i < 3; i++)
    {
        SDL_QueryTexture(tabTextRes[i], nullptr, nullptr, &tabPositionRes[i].w, &tabPositionRes[i].h); // Récupere la dimension de la texture
    }

    // On définie les coordo des titres Resolutions, Changer Pseudo, Son.
    tabPositionTitre[3].x = LargeurEcran / 30; // Le x est toujours le même pour tous les textes.
    tabPositionTitre[3].y = HauteurEcran / 30;

    tabPositionTitre[0].x = (LargeurEcran / 2 - tabPositionTitre[0].w / 2); // on soustrait pour que le texte soit alignée.
    tabPositionTitre[0].y = HauteurEcran / 14;

    tabPositionTitre[1].x = tabPositionTitre[0].x + LargeurEcran / 6 - tabPositionTitre[0].x / 1.8; // Le x est toujours le même pour tous les textes.
    tabPositionTitre[1].y = tabPositionTitre[0].y + HauteurEcran / 2.6;

    tabPositionTitre[2].x = tabPositionTitre[0].x + LargeurEcran / 6 - tabPositionTitre[0].x / 3.5; // Le x est toujours le même pour tous les textes.
    tabPositionTitre[2].y = HauteurEcran / 1.4;

    //############### Résolutions ###############
    tabPositionRes[0].x = tabPositionTitre[0].x + LargeurEcran / 6 - tabPositionTitre[0].x / 2.7; // Le x est toujours le même pour tous les textes.
    tabPositionRes[0].y = tabPositionTitre[0].y + HauteurEcran / 9;

    tabPositionRes[1].x = tabPositionTitre[0].x + LargeurEcran / 6 - tabPositionTitre[0].x / 2.7; // Le x est toujours le même pour tous les textes.
    tabPositionRes[1].y = tabPositionTitre[0].y + HauteurEcran / 5;

    tabPositionRes[2].x = tabPositionTitre[0].x + LargeurEcran / 6 - tabPositionTitre[0].x / 2.7; // Le x est toujours le même pour tous les textes.
    tabPositionRes[2].y = tabPositionTitre[0].y + HauteurEcran / 3.5;

    // ############## Position des sons.
    positionEchelle[0].x = LargeurEcran / 20;
    positionEchelle[0].y = HauteurEcran / 1.2;

    for (int i = 1; i < 11; i++)
    {
        positionEchelle[i].x = positionEchelle[i - 1].x + LargeurEcran / 12;
        positionEchelle[i].y = positionEchelle[0].y;
        if (i == 5) // choixVolume est pour rappelle la donnée membre qui nous indique sur l'échelle de 0 à 10 le volume de la musique.
        {
            // Position du rectangle repère pour régler le son.
            sonSelectionne.x = positionEchelle[i].x - LargeurEcran / 130;
            sonSelectionne.y = positionEchelle[i].y;
            if (LargeurEcran == 1920 && HauteurEcran == 1080)
            {
                sonSelectionne.w = LargeurEcran / 40;
                sonSelectionne.h = HauteurEcran / 22;
            }
            else if (LargeurEcran == 1280 && HauteurEcran == 720)
            {
                sonSelectionne.w = LargeurEcran / 30;
                sonSelectionne.h = HauteurEcran / 15;
            }
            else
            {
                sonSelectionne.w = LargeurEcran / 20;
                sonSelectionne.h = HauteurEcran / 15;
            }
        }
    }
    // On libère la surface.
    for (int i = 0; i < 4; i++)
    {
        SDL_FreeSurface(tabSurfTitre[i]);
    }
    for (int i = 0; i < 3; i++)
    {
        SDL_FreeSurface(tabRes[i]);
    }
    SDL_FreeSurface(surfEchelle);
    TTF_CloseFont(fontResText);

    // Variable qui contiendront la position de la souris.
    int posSourisX;
    int posSourisY;

    // On récupère les événements.
    while (isOpen)
    {

        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_WINDOWEVENT:
                if (events.window.event == SDL_WINDOWEVENT_CLOSE)
                    isOpen = false;
                break;
            case SDL_MOUSEBUTTONDOWN: // Click de souris
                SDL_Log("+clic");
                posSourisX = events.button.x;
                posSourisY = events.button.y;
                int posResX = positionEchelle[0].x + LARGEUR_ECRAN<int> / 50;
                int posResY = positionEchelle[0].y;

                cout << posResX << " " << posResY << endl;
                cout << posSourisX << " " << posSourisY << endl;

                if ((posSourisX > tabPositionTitre[3].x && posSourisY > tabPositionTitre[3].y) &&                                                      // Point en haut à gauche
                    (posSourisX < (tabPositionTitre[3].x + LARGEUR_ECRAN<int> / 8) && (posSourisY > tabPositionTitre[3].y)) &&                         // Point en haut à droite
                    (posSourisX > tabPositionTitre[3].x && posSourisY < tabPositionTitre[3].y + HAUTEUR_ECRAN<int> / 18) &&                            // Point en bas à gauche
                    (posSourisX < (tabPositionTitre[3].x + LARGEUR_ECRAN<int> / 8) && (posSourisY < tabPositionTitre[3].y + HAUTEUR_ECRAN<int> / 18))) // Point en bas à droite.
                {
                    openMenu = true;
                    isOpen = false;
                }

                // Pour les résolutions
                if ((posSourisX > tabPositionRes[0].x && posSourisY > tabPositionRes[0].y) &&                                                      // Point en haut à gauche
                    (posSourisX < (tabPositionRes[0].x + LARGEUR_ECRAN<int> / 8) && (posSourisY > tabPositionRes[0].y)) &&                         // Point en haut à droite
                    (posSourisX > tabPositionRes[0].x && posSourisY < tabPositionRes[0].y + HAUTEUR_ECRAN<int> / 18) &&                            // Point en bas à gauche
                    (posSourisX < (tabPositionRes[0].x + LARGEUR_ECRAN<int> / 8) && (posSourisY < tabPositionRes[0].y + HAUTEUR_ECRAN<int> / 18))) // Point en bas à droite.
                {
                    modifFichierRes(800, 800);
                    Mix_PlayChannel(1, sons[2], 0); // On joue le son valider 1 fois.
                    // Afficher : La résolution sera changé au redémarrage.
                }
                if ((posSourisX > tabPositionRes[1].x && posSourisY > tabPositionRes[1].y) &&                                                      // Point en haut à gauche
                    (posSourisX < (tabPositionRes[1].x + LARGEUR_ECRAN<int> / 8) && (posSourisY > tabPositionRes[1].y)) &&                         // Point en haut à droite
                    (posSourisX > tabPositionRes[1].x && posSourisY < tabPositionRes[1].y + HAUTEUR_ECRAN<int> / 18) &&                            // Point en bas à gauche
                    (posSourisX < (tabPositionRes[1].x + LARGEUR_ECRAN<int> / 8) && (posSourisY < tabPositionRes[1].y + HAUTEUR_ECRAN<int> / 18))) // Point en bas à droite.
                {
                    // Afficher : La résolution sera changé au redémarrage.
                    modifFichierRes(1280, 720);
                    Mix_PlayChannel(1, sons[2], 0); // On joue le son valider 1 fois.
                }
                if ((posSourisX > tabPositionRes[2].x && posSourisY > tabPositionRes[2].y) &&                                                      // Point en haut à gauche
                    (posSourisX < (tabPositionRes[2].x + LARGEUR_ECRAN<int> / 8) && (posSourisY > tabPositionRes[2].y)) &&                         // Point en haut à droite
                    (posSourisX > tabPositionRes[2].x && posSourisY < tabPositionRes[2].y + HAUTEUR_ECRAN<int> / 18) &&                            // Point en bas à gauche
                    (posSourisX < (tabPositionRes[2].x + LARGEUR_ECRAN<int> / 8) && (posSourisY < tabPositionRes[2].y + HAUTEUR_ECRAN<int> / 18))) // Point en bas à droite.
                {
                    // Afficher : La résolution sera changé au redémarrage.
                    modifFichierRes(1920, 1080);
                    Mix_PlayChannel(1, sons[2], 0); // On joue le son valider 1 fois.
                }
                if ((posSourisX > positionEchelle[0].x && posSourisY > positionEchelle[0].y)) // Si on est dans la partie basse vers l'echelle de son on regarde si on clique sur un nombre.
                {
                    for (int i = 0; i < 11; i++)
                    {
                        if ((posSourisX > positionEchelle[i].x && posSourisY > positionEchelle[i].y) &&                                                       // Point en haut à gauche
                            (posSourisX < (positionEchelle[i].x + LARGEUR_ECRAN<int> / 50) && (posSourisY > positionEchelle[i].y)) &&                         // Point en haut à droite
                            (posSourisX > positionEchelle[i].x && posSourisY < positionEchelle[i].y + HAUTEUR_ECRAN<int> / 18) &&                             // Point en bas à gauche
                            (posSourisX < (positionEchelle[i].x + LARGEUR_ECRAN<int> / 50) && (posSourisY < positionEchelle[i].y + HAUTEUR_ECRAN<int> / 18))) // Point en bas à droite.
                        {
                            choixVolume = i;
                            if (i == 10)
                            {
                                sonSelectionne.x = positionEchelle[i].x - 5;
                                sonSelectionne.y = positionEchelle[i].y;
                                if (LargeurEcran == 1920 && HauteurEcran == 1080)
                                {
                                    sonSelectionne.w = LargeurEcran / 30;
                                    sonSelectionne.h = HauteurEcran / 20;
                                }
                                else if (LargeurEcran == 1280 && HauteurEcran == 720)
                                {
                                    sonSelectionne.w = LargeurEcran / 19;
                                    sonSelectionne.h = HauteurEcran / 15;
                                }
                                else
                                {
                                    sonSelectionne.w = LargeurEcran / 13;
                                    sonSelectionne.h = HauteurEcran / 15;
                                }
                            }
                            else
                            {
                                sonSelectionne.x = positionEchelle[i].x - LargeurEcran / 130;
                                sonSelectionne.y = positionEchelle[i].y;
                                if (LargeurEcran == 1920 && HauteurEcran == 1080)
                                {
                                    sonSelectionne.w = LargeurEcran / 40;
                                    sonSelectionne.h = HauteurEcran / 22;
                                }
                                else if (LargeurEcran == 1280 && HauteurEcran == 720)
                                {
                                    sonSelectionne.w = LargeurEcran / 30;
                                    sonSelectionne.h = HauteurEcran / 15;
                                }
                                else
                                {
                                    sonSelectionne.w = LargeurEcran / 20;
                                    sonSelectionne.h = HauteurEcran / 15;
                                }
                            }
                            Mix_PlayChannel(1, sons[2], 0); // On joue le son valider 1 fois.
                            volume = MIX_MAX_VOLUME / 30 * choixVolume; // On met le son à 5 de base (Moitié).
                            Mix_Volume(0, volume);                      // On modifie le son du cannal actif.
                        }
                    }
                }
            }

            // Affiche l'image.
            // On met à jour l'affichage.
            SDL_SetRenderDrawColor(renderer, 254, 254, 254, 255); // Affiche la couleur de fond.
            SDL_RenderClear(renderer);

            fondMenu.draw(renderer, 0, 0, LargeurEcran, HauteurEcran);
            //SDL_RenderCopy(renderer, fondMenu, nullptr, nullptr); // Affichage du texte.

            //SDL_RenderClear(renderer); // On change sur quelle rendu on fait.
            for (int i = 0; i < 4; i++)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                            // On passe sur la couleur du texte.
                SDL_RenderCopy(renderer, tabTextureTitre[i], nullptr, &tabPositionTitre[i]); // Affichage du texte.
            }
            for (int i = 0; i < 3; i++)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                     // On passe sur la couleur du texte.
                SDL_RenderCopy(renderer, tabTextRes[i], nullptr, &tabPositionRes[i]); // Affichage du texte.
            }

            for (int i = 0; i < 11; i++) // Echelle son [1...10]
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                  // On passe sur la couleur du texte.
                SDL_RenderCopy(renderer, tabSon[i], nullptr, &positionEchelle[i]); // Affichage du texte.
            }

            // Affiche le rectangle qui indique le volume de la musique du Jeu.
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // On passe sur la couleur du texte.
            SDL_RenderDrawRects(renderer, &sonSelectionne, 1);

            SDL_RenderPresent(renderer);
        }
    }
    for (int i = 0; i < 4; i++)
    {
        SDL_DestroyTexture(tabTextureTitre[i]);
    }
    for (int i = 0; i < 3; i++)
    {
        SDL_DestroyTexture(tabTextRes[i]);
    }
    for (int i = 0; i < 10; i++)
        SDL_DestroyTexture(tabSon[i]);
    if (openMenu)
    {
        sdlMenu();
    }
}

void sdlJeu::sdlBoucleJeu(Jeu &jeu)
{
    //Remplir l'écran de noir
    SDL_SetRenderDrawColor(renderer, 254, 254, 254, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    //sdlAffJoueurActif(jeu);

    SDL_Event event;
    bool quit = false;
    int sourisX;
    int sourisY;
    unsigned int couleur = 0;
    bool couleurChangee = false;
    bool joueurChange = true;
    unsigned int indiceJoueur;

    // tant que ce n'est pas la fin ...
    while (!quit)
    {
        if (joueurChange)
        {
            indiceJoueur = jeu.joueurActif;
            sdlAffJoueur(jeu, indiceJoueur);
            if (couleur != 0 && couleurChangee)
                sdlAffCouleurChoisie(couleur);
            joueurChange = false;
        }

        while (SDL_PollEvent(&event))
        {
            sourisX = event.button.x;
            sourisY = event.button.y;
            switch (event.type)
            {
            case SDL_QUIT: // Si l'utilisateur a cliqué sur la croix de fermeture
                quit = true;
                break;
            case SDL_KEYDOWN: // Si une touche est enfoncée
                if (event.key.keysym.scancode == SDL_SCANCODE_Q)
                    quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN: // Si le bouton de la souris est appuyé
                joueurChange = true;
                if (sourisX > 800 && sourisX < 910 && sourisY > 300 && sourisY < 457) // clic sur la pioche
                {
                    jeu.piocherCarte();
                    jeu.termineTour();
                    sdlAffJoueur(jeu, indiceJoueur);

                    if (couleur != 0 && couleurChangee)
                        sdlAffCouleurChoisie(couleur);
                }
                if (sourisX > 0 && sourisX < 1870 && sourisY > 600 && sourisY < 957) // clic sur une carte de la main
                {
                    couleur = 0;
                    string messageErreur;
                    unsigned int indiceCarte;
                    if (sourisY < 757)
                        indiceCarte = sourisX / 110;
                    if (sourisY > 800)
                        indiceCarte = 17 + sourisX / 110;

                    if (jeu.carteValide(jeu.joueurs[jeu.joueurActif].main[indiceCarte]))
                    {
                        couleurChangee = false;
                        if (jeu.joueurs[jeu.joueurActif].main[indiceCarte].getValeur() == 13 || jeu.joueurs[jeu.joueurActif].main[indiceCarte].getValeur() == 14)
                        {
                            couleur = choixCouleur();
                            couleurChangee = true;
                            jeu.joueurs[jeu.joueurActif].main[indiceCarte].setCouleur(couleur);
                        }
                    }
                    jeu.poserCarte(indiceCarte, messageErreur);
                    sdlAffJoueur(jeu, indiceJoueur);
                    if (couleur != 0 && couleurChangee)
                        sdlAffCouleurChoisie(couleur);
                    if (jeu.joueurs[indiceJoueur].main.size() == 1)
                    {
                        situationUno(jeu);
                    }
                }
                break;
            default:
                break;
            }
        }
        SDL_Delay(500);

        if (jeu.joueurs[indiceJoueur].gagnant())
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_Rect texte;
            texte.x = 400;
            texte.y = 100;
            texte.w = 1200;
            texte.h = 400;
            font_im.setSurface(TTF_RenderText_Solid(font, "Good Game", jaune));
            font_im.loadFromCurrentSurface(renderer);
            SDL_RenderCopy(renderer, font_im.getTexture(), NULL, &texte);
            texte.x = 500;
            texte.y = 500;
            texte.w = 800;
            texte.h = 400;
            font_im.setSurface(TTF_RenderText_Solid(font, jeu.joueurs[indiceJoueur].nom.c_str(), jaune));
            font_im.loadFromCurrentSurface(renderer);
            SDL_RenderCopy(renderer, font_im.getTexture(), NULL, &texte);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            quit = true;
        }
    }
}
