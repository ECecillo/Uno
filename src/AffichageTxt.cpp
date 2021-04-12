#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // WIN32
#include "Fenetre.h"

#include <Jeu.h>

void txtAff(Fenetre &win, const Jeu &jeu)
{
    const Joueur &ter = jeu.joueurs[jeu.joueurActif];

    win.clear();

    // Affichage de la pioche, talon, passer ...
    for (unsigned int x = 0; x < ter.getHaut(); ++x)
        for (unsigned int y = 0; y < ter.getLarg(); ++y)
            win.prepaFenetre(x, y, ter.getXY(x, y));

    win.dessine();
}

void txtBoucle(Jeu &jeu)
{
    // Creation d'une nouvelle fenetre en mode texte
    // => fenetre de dimension et position (WIDTH,HEIGHT,STARTX,STARTY)
    Fenetre win(jeu.joueurs[jeu.joueurActif].getHaut(), jeu.joueurs[jeu.joueurActif].getLarg());

    bool ok = true;
    int c;
    txtAff(win, jeu); // On initialise le jeu avec les éléments principaux.

    do
    {
        txtAff(win, jeu); // On initialise le jeu avec les éléments principaux.
        if (jeu.finPartie)
            cout << "Fin partie = true ?" << endl;
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);
#endif // WIN32
        jeu.finTour = false;

        //jeu.MaJTableJoueurActifDebutTour(); // Modif rendu main joueur, adversaire et talon.

        while (jeu.finTour == false) // Tant que l'on a pas terminé le tour.
        {
            txtAff(win, jeu);                   // On initialise le jeu avec les éléments principaux.
            jeu.MaJTableJoueurActifDebutTour(); // Modif rendu main joueur, adversaire et talon.
            //cout << "On passe " << endl;
            c = win.getCh(); // On récupère le caractère de la touche appuyé et on le met dans c.
            if(jeu.statut_Uno)
            {
                sleep(2);
                cout << "========== Un des joueurs peut jouer UNO !!! ===============" << 
                "# Vous avez 2 secondes pour appuyer sur U ou C # " << endl;
                jeu.Uno(c);
            }
            
            switch (c)
            {
            case 'a':
                jeu.actionJoueur('a', 0, 0);
                break;
            case 'd':
                jeu.actionJoueur('d', 0, 0);
                break;
            case 'r':
                cout << "Je suis la touche R" << endl;
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('r', 0, 0);
                break;
            case 'v':
                cout << "Je suis la touche V" << endl;
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('v', 0, 0);
                break;

            case 'b':
                cout << "Je suis la touche B" << endl;
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('b', 0, 0);
                break;

            case 'j':
                cout << "Je suis la touche J" << endl;
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('j', 0, 0);
                break;

            /* case 'c':
                cout << "Je suis la touche C" << endl;
                jeu.actionJoueur('c', 0, 0);
            case 'u':
                cout << "Je suis la touche U" << endl;
                jeu.actionJoueur('u', 0, 0); */
            case 'p':
                cout << "Je suis la touche P" << endl;
                jeu.actionJoueur('p', 0, 0);
                break;
            case 'e':
                cout << "Je suis la touche E" << endl;
                jeu.actionJoueur('e', 0, 0);

                break;
            case 'q':
                cout << "Je suis la touche Q" << endl;
                jeu.finTour = true;
                ok = false;
                jeu.finPartie = true;
                break;
            }
            /* if (jeu.finTour)
                cout << "Fin du tour" << endl; */
            /* if(ok)
                cout << "ok = true ?" << endl; */
        }

    } while (ok || !jeu.finPartie);
}
