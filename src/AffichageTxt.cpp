#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // WIN32
#include "Fenetre.h"
#include "SalleAttente.h"
#include "VarianteDoublon.h"
#include "VarianteEchange.h"
#include "VarianteTourne.h"
#include "VarianteSuite.h"
#include "VarianteCumul.h"
#include "AffichageTxt.h"

#include "Jeu.h"

// affiche la table de jeu sur le terminal
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

// affiche la salle d'attente sur le terminal
void txtAffSalleAttente(Fenetre & winSA, const SalleAttente & s)
{
    winSA.clear();
    for (unsigned int x=0; x<s.haut; x++)
        for (unsigned int y=0; y<s.larg; y++)
            winSA.prepaFenetre(x,y,s.salle[x][y]);
    winSA.dessine();
}

// boucle à partir de la salle d'attente
void txtBoucleDebut(SalleAttente & s)
{
    Fenetre winSA(s.haut,s.larg);
    // on affiche la salle d'attente
    txtAffSalleAttente(winSA,s);
    int c;
    bool finPartie=false;
    while (!finPartie)
    {
        c=winSA.getCh();
        switch (c)
        {
            // l'étoile monte
            case 'q':finPartie = true;
                    break;
            case 't':if (s.etoile>11) {
                        s.salle[s.etoile][49]=' ';
                        s.etoile--;
                        s.MaJFenetreSalle();
                        txtAffSalleAttente(winSA,s);}
                    break;
            // l'étoile descend
            case 'b':if (s.etoile<14) {
                        s.salle[s.etoile][49]=' ';
                        s.etoile++;
                        s.MaJFenetreSalle();
                        txtAffSalleAttente(winSA,s);}
                    break;
            // on valide le choix
            case 'e':switch (s.etoile) {
                        // choix du jeu
                        case 11:s.fenetreSalleRegles();
                                txtAffSalleAttente(winSA,s);
                                s.choixJeu();
                                txtAffSalleAttente(winSA,s);
                                break;
                        // choix du nombre de joueurs
                        case 12:s.choixNombreJoueurs();
                                txtAffSalleAttente(winSA,s);
                                break;
                        // choix du nombre d'IA
                        case 13:s.choixNombreIA();
                                txtAffSalleAttente(winSA,s);
                                break;
                        // on lance la partie
                        case 14:if (s.nombreJoueurs+s.nombreIA>1) // il faut au moins 2 joueurs
                                // création du jeu selon le choix et lancement de la partie
                                switch (s.variante)
                                {
                                    case 1: {Jeu jeu(s.nombreJoueurs,s.nombreIA);
                                            txtBoucle(jeu);
                                            break;}
                                    case 2: {VarianteCumul jeu(s.nombreJoueurs,s.nombreIA);
                                            txtBoucle(jeu);
                                            break;}
                                    case 3: {VarianteDoublon jeu(s.nombreJoueurs,s.nombreIA);
                                            txtBoucle(jeu);
                                            break;}
                                    case 4: {VarianteEchange jeu(s.nombreJoueurs,s.nombreIA);
                                            txtBoucle(jeu);
                                            break;}
                                    case 5: {VarianteSuite jeu(s.nombreJoueurs,s.nombreIA);
                                            txtBoucle(jeu);
                                            break;}
                                    case 6: {VarianteTourne jeu(s.nombreJoueurs,s.nombreIA);
                                            txtBoucle(jeu);
                                            break;}
                                    default: break;
                                finPartie=true;
                                }
                                else cout << "Pas assez de joueurs pour commencer la partie." << endl;
                                break;
                        default: break;
                        }
                    break;
            default: break;
        }
    }
}

// boucle de jeu
void txtBoucle(Jeu & jeu)
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


        while (jeu.finTour == false) // Tant que l'on a pas terminé le tour.
        {
            txtAff(win, jeu);                   // On initialise le jeu avec les éléments principaux.
            jeu.MaJTableJoueurActifDebutTour(); // Modif rendu main joueur, adversaire et talon.

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
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('r', 0, 0);
                break;
            case 'v':
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('v', 0, 0);
                break;

            case 'b':
                if ((jeu.talon.front()).getValeur() == 13 || (jeu.talon.front()).getValeur() == 14)
                    jeu.actionJoueur('b', 0, 0);
                break;

            case 'j':
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
                jeu.actionJoueur('p', 0, 0);
                break;
            case 'e':
                jeu.actionJoueur('e', 0, 0);

                break;
            case 'q':
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
