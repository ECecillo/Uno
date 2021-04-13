#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // WIN32
#include "Fenetre.h"

#include <Jeu.h>

void changeCouleurCarte(Jeu &jeu, Fenetre win)
{
    int indiceCarte = jeu.joueurs[jeu.joueurActif].indiceEtoile;
    int numeroCarte = jeu.joueurs[jeu.joueurActif].main[indiceCarte].getValeur();
    int c;

    if (numeroCarte == 13 || numeroCarte == 14)
    {
        bool choixCouleur = true;
        cout << "### Vous devez choisir une nouvelle couleur sur laquelle vous jouerez ###" << endl;
        while (choixCouleur)
        {
            c = win.getCh();
            switch (c)
            {
            case 'r':
                cout << "Je suis la touche R" << endl;
                //if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                jeu.actionJoueur('r');
                choixCouleur = false;
                break;
            case 'v':
                cout << "Je suis la touche V" << endl;
                //if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                jeu.actionJoueur('v');
                choixCouleur = false;
                break;

            case 'b':
                cout << "Je suis la touche B" << endl;
                //if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                jeu.actionJoueur('b');
                choixCouleur = false;
                break;

            case 'j':
                cout << "Je suis la touche J" << endl;
                //if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                jeu.actionJoueur('j');
                choixCouleur = false;
                break;
            }
        }
    }
}

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
    //txtAff(win, jeu); // On initialise le jeu avec les éléments principaux.

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
                cout << "========== Un des joueurs peut jouer UNO !!! ==============="
                     << "# Vous avez 2 secondes pour appuyer sur U ou C # " << endl;
                jeu.Uno(c);
            }

            switch (c)
            {
            case 'a':
                jeu.actionJoueur('a');
                break;
            case 'd':
                jeu.actionJoueur('d');
                break;
            /* case 'r':
                cout << "Je suis la touche R" << endl;
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('r');
                break;
            case 'v':
                cout << "Je suis la touche V" << endl;
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('v');
                break;

            case 'b':
                cout << "Je suis la touche B" << endl;
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('b');
                break;

            case 'j':
                cout << "Je suis la touche J" << endl;
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('j');
                break; */
            case 'p':
                cout << "Je suis la touche P" << endl;
                jeu.actionJoueur('p');
                break;
            case 'e':
                cout << "Je suis la touche E" << endl;
                changeCouleurCarte(jeu, win);
                jeu.actionJoueur('e');

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
