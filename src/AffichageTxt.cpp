#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // WIN32
#include "Terrain.h"

#include <Jeu.h>

void txtAff(Terrain &win, const Jeu &jeu)
{
    const Joueur &ter = jeu.getConstEnv();

    win.clear();

    // Affichage de la pioche, talon, passer ...
    for (int x = 0; x < ter.getLarg(); ++x)
        for (int y = 0; y < ter.getHaut(); ++y)
            win.print(x, y, ter.getXY(x, y));

    win.draw();
}

void txtBoucle(Jeu &jeu)
{
    // Creation d'une nouvelle fenetre en mode texte
    // => fenetre de dimension et position (WIDTH,HEIGHT,STARTX,STARTY)
    Terrain win(jeu.getConstEnv().getLarg(), jeu.getConstEnv().getHaut());

    bool ok = true;
    int c;

    do
    {
        txtAff(win, jeu); // On initialise le jeu avec les éléments principaux.

#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);
#endif // WIN32

        while (jeu.finTour == false) // Tant que l'on a pas terminé le tour.
        {
            jeu.MaJTableJoueurActifDebutTour(); // Modif rendu main joueur, adversaire et talon.
            c = win.getCh(); // On récupère le caractère de la touche appuyé et on le met dans c.

            jeu.actionJoueur(c, 0, 0); // Selon la touche entrée dans le terminale on active une action. (cf Jeu.cpp)
            jeu.termineTour();         // On change le num joueurActif pour actualiser au prochain tour le jeu.
            jeu.finTour = true;        // On arrête le while.
        }

        c = win.getCh(); // On récupère le caractère de la touche appuyé et on le met dans c.
        if (c == 'q')
        {
            ok = true;
            break;
        }

    } while (ok || jeu.finPartie == false);
}
