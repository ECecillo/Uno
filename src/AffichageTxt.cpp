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
    if (jeu.joueurActif >= jeu.nombreJoueurs)
    {
        return;
    }
    else
    {
        const Joueur &ter = jeu.joueurs[jeu.joueurActif];

        win.clear();

        // Affichage de la pioche, talon, passer ...
        for (unsigned int x = 0; x < ter.getHaut(); ++x)
            for (unsigned int y = 0; y < ter.getLarg(); ++y)
                win.prepaFenetre(x, y, ter.getXY(x, y));

        win.dessine();
    }
}

void txtBoucle(Jeu &jeu)
{
    // Creation d'une nouvelle fenetre en mode texte
    // => fenetre de dimension et position (WIDTH,HEIGHT,STARTX,STARTY)
    //Fenetre win(jeu.joueurs[jeu.joueurActif].getHaut(), jeu.joueurs[jeu.joueurActif].getLarg());
    Fenetre win(38, 180);

    bool ok = true;
    int c;
    //txtAff(win, jeu); // On initialise le jeu avec les éléments principaux.

    do
    {
        txtAff(win, jeu); // On initialise le jeu avec les éléments principaux.
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);
#endif // WIN32
        jeu.finTour = false;

        jeu.joueurs[jeu.joueurActif].indiceEtoile = 0;
        jeu.MaJTableJoueurActifDebutTour(); // Modif rendu main joueur, adversaire et talon.
        jeu.annonceGagnant(); // Annonce le gagnant de la partie.
        if(jeu.finPartie == true)
            return;
        while (jeu.finTour == false || jeu.finPartie == false) // Tant que l'on a pas terminé le tour.
        {
            //cout << "Joueur Actif " << jeu.joueurActif << endl;
            txtAff(win, jeu);                   // On initialise le jeu avec les éléments principaux.
            jeu.MaJTableJoueurActifDebutTour(); // Modif rendu main joueur, adversaire et talon.
            c = win.getCh();                    // On récupère le caractère de la touche appuyé et on le met dans c.

            if (jeu.joueurActif >= jeu.nombreJoueurs)
            {
                sleep(1);
                // Appelle choix joueur.
                int indexBot = jeu.joueurActif - jeu.nombreJoueurs;
                jeu.joueursBot[indexBot].choixJeu(jeu);
            }
            if (jeu.statut_Uno)
            {
                cout << "========== Un des joueurs peut jouer UNO !!! ===============" << endl;
                sleep(1.0);
                c = win.getCh(); // On récupère le caractère de la touche appuyé et on le met dans c.
                while (jeu.statut_Uno)
                {
                    sleep(1);
                    if (c == 'u' && jeu.joueurActif < jeu.nombreJoueurs)
                    { // Si le joueur humain appuie sur U pour uno, avant le sleep alors il continue de jouer.
                        jeu.actionJoueur('u');
                    }
                    else if (c == 'c' && jeu.joueurActif >= jeu.nombreJoueurs)
                    { // Si le joueur Humain fait un contre Uno alors que le bot est en situation de Uno, le bot pioche.
                        jeu.Uno(c);
                    }
                    else if (jeu.joueurActif >= jeu.nombreJoueurs && (c != 'c' || c != 'u'))
                    {
                        jeu.Uno('u');
                    }
                    // Si le joueur Humain n'appuie pas sur une des touches après le sleep alors l'ordi fait un contre uno.
                    else if ((c != 'c' || c != 'u') && jeu.joueurActif < jeu.nombreJoueurs)
                    {
                        jeu.Uno('c');
                    }
                }
            }

            switch (c)
            {
            case 'a':
                cout << "Indice etoile " << jeu.joueurs[jeu.joueurActif].indiceEtoile << endl;
                jeu.actionJoueur('a');
                break;
            case 'd':
                cout << "Indice etoile " << jeu.joueurs[jeu.joueurActif].indiceEtoile << endl;

                jeu.actionJoueur('d');
                break;
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
