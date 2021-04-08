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
    const Joueur &ter = jeu.joueurs[jeu.joueurActif];

    win.clear();

    // Affichage de la pioche, talon, passer ...
    for (unsigned int x = 0; x < ter.getHaut(); ++x)
        for (unsigned int y = 0; y < ter.getLarg(); ++y)
            win.print(x, y, ter.getXY(x, y));

    win.draw();
}

void txtBoucle(Jeu &jeu)
{
    // Creation d'une nouvelle fenetre en mode texte
    // => fenetre de dimension et position (WIDTH,HEIGHT,STARTX,STARTY)
    Terrain win(jeu.joueurs[jeu.joueurActif].getHaut(), jeu.joueurs[jeu.joueurActif].getLarg());

    bool ok = true;
    int c;
    txtAff(win, jeu); // On initialise le jeu avec les éléments principaux.

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
            txtAff(win, jeu);                   // On initialise le jeu avec les éléments principaux.
            jeu.MaJTableJoueurActifDebutTour(); // Modif rendu main joueur, adversaire et talon.
            //cout << "On passe " << endl;
            c = win.getCh(); // On récupère le caractère de la touche appuyé et on le met dans c.

            switch (c)
            {
            case 'a':
                jeu.actionJoueur('a', 0, 0);
                break;
            case 'd':
                jeu.actionJoueur('d', 0, 0);
                break;
            case 'r':
                jeu.actionJoueur('h', 0, 0);
                break;
            case 'v':
                jeu.actionJoueur('v', 0, 0);
            case 'b':
                jeu.actionJoueur('b', 0, 0);
            case 'j':
                jeu.actionJoueur('j', 0, 0);
            case 'c':
                jeu.actionJoueur('c', 0, 0);
            case 'u':
                jeu.actionJoueur('u', 0, 0);
            case 'p':
                jeu.actionJoueur('p', 0, 0);
            case 'e':
                jeu.actionJoueur('e', 0, 0);
                break;
            case 'q':
                ok = false;
                break;
            }
        }

    } while (ok || jeu.finPartie == false);
}
