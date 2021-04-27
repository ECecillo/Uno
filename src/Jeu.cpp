#include <iostream>
#include <vector>
#include <stack> // Pour les piles.
#include <set>   // utile dans initCarte
#include <cassert>
#include <algorithm>
#include <string.h>
#include <Jeu.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <chrono>
#include <Bot.h>

using namespace std;

// constructeur par défaut
Jeu::Jeu()
{
    joueurs = NULL;
    joueursBot = NULL;
    nombreJoueurs = 0;
    nombreIA = 0;
    sensJeu = 1;

    joueurActif = 0;
    finPartie = false;
    finTour = false;
    statut_Uno = false;
}

Jeu::~Jeu()
{
    delete[] joueurs;
    joueurs = NULL;

    delete[] joueursBot;
    joueursBot = NULL;

    sensJeu = 1;
    joueurActif = 0;
    nombreJoueurs = 0;
    nombreIA = 0;
}

Jeu::Jeu(const unsigned int nbjoueurs, const unsigned int nbIA = 0)
{
    initCarte();
    nombreJoueurs = nbjoueurs;
    nombreIA = nbIA;
    // création du tableau joueurs
    joueurs = new Joueur[nombreJoueurs];
    // Création du tableau de Bots
    joueursBot = new Bot[nombreIA];

    // Initialisation du tableau de joueurs
    for (unsigned int i = 0; i < nombreJoueurs; i++)
    {
        Joueur joueur(i + 1);
        joueurs[i] = joueur;
    }

    // Initialisation du tableau de bots.
    for (unsigned int i = 0; i < nombreIA; i++)
    {
        Bot bot(i + 1);
        joueursBot[i] = bot;
    }

    joueurActif = rand() % (nombreJoueurs + nombreIA); // On génère un numéro de joueur aléatoire pour le début de la partie.
    sensJeu = 1;                                       // On tournera à gauche.
    distribueCarte();                                  // On donne les cartes au joueurs
    initTalon();                                       // On initialise le Talon.
    finTour = false;
    statut_Uno = false;
    finPartie = false;
    //nombreJoueurs += nombreIA; // On regroupe le nombre d'IA avec le nombre de joueurs.
    unsigned int pos = (180 - (nombreJoueurs - 1) * 11 - (nombreJoueurs - 2)) / 2;

    for (unsigned int i = 0; i < nombreJoueurs; i++)
    {
        for (unsigned int j = 0; j < nombreJoueurs - 1; j++)
        {
            joueurs[i].insererCarteAdversairePositionJ(pos + 12 * j, joueurs[(i + 1 + j) % nombreJoueurs].numeroJoueur);
        }
    }
    modifAdversairesTxt();
}

// Initialise la pioche
void Jeu::initCarte()
{
    unsigned int i, j = 0;

    vector<Carte> jeuCarte;
    for (i = 1; i < 5; i++) // Couleur
    {
        jeuCarte.push_back(Carte(0, i)); // On met les quatres 0 de chaque couleur à chaque fois que l'on change de couleur.
        for (j = 1; j < 25; j++)         // Numéro
            if (j > 12)
                jeuCarte.push_back(Carte(j - 12, i));
            else
                jeuCarte.push_back(Carte(j, i));
    }

    for (i = 1; i < 5; i++)       // Couleur : [1] Rouge, [2] Vert, [3] Bleu, [4] jaune
        for (j = 13; j < 15; j++) // Numéro
            // 14 : changement de couleur,
            // 13 : carte +4.
            jeuCarte.push_back(Carte(j, i));

    /*     unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(jeuCarte.begin(), jeuCarte.end(), std::default_random_engine(seed));

    unsigned int l = 0;
    do
    {
        pioche.push(jeuCarte[l]);
        l++;
    } while (l <=jeuCarte.size()); 
*/
    srand((unsigned int)time(NULL));
    int Ind;
    set<int>::iterator it;
    set<int> indicesJeuCarte;
    while (pioche.size() != 108)
    {
        Ind = rand() % 108;
        it = indicesJeuCarte.find(Ind);
        if (it == indicesJeuCarte.end())
        {
            indicesJeuCarte.insert(Ind);
            pioche.push(jeuCarte[Ind]);
        }
    }
}

void Jeu::initTalon()
{
    talon.push(pioche.top()); // Carte mis dans la talon, celle sur laquelle on va jouer.
    pioche.pop();             // Elle dans la file, on la supprime de la pile.
    for (int i = 0; i < nombreJoueurs; i++)
        joueurs[i].modifTalonPiocheTxt(talon, pioche);
}
// 7 cartes par joueur et par Bot.
void Jeu::distribueCarte()
{
    for (unsigned int i = 0; i < nombreJoueurs; i++)
    {
        for (unsigned int j = 0; j < 7; j++)
        {
            joueurs[i].main.push_back(pioche.top());
            pioche.pop();
        }
        joueurs[i].modifMainTxt();
    }
    // Distribution pour les bots.
    for (unsigned int i = 0; i < nombreIA; i++)
    {
        for (unsigned int j = 0; j < 7; j++)
        {
            joueursBot[i].main.push_back(pioche.top());
            // On récupère le nombre de carte de chaque couleur à chaque ajout.
            definieCouleurBot(joueursBot[i], pioche.top());
            pioche.pop();
        }
        joueursBot[i].trierMain();
    }
}
void Jeu::definieCouleurBot(Bot &tabJoueursBot, Carte &c)
{
    switch (c.getCouleur())
    {
    case 1:
        tabJoueursBot.setCarteRouge();

        break;
    case 2:
        tabJoueursBot.setCarteVert();

        break;
    case 3:
        tabJoueursBot.setCarteBleu();

        break;
    case 4:
        tabJoueursBot.setCarteJaune();

        break;

    default:
        break;
    }
}

// true si la carte est valide
bool Jeu::carteValide(const Carte c) const
{
    bool chercheCouleur = false;
    //cout << "Carte Valide" << endl;
    //si carte +4, on regarde dans la main du joueur s'il y a une carte de la même couleur que celle du talon
    if (c.getValeur() == 13)
    {
        unsigned int i = 0;
        while (i < joueurs[joueurActif].main.size() && !chercheCouleur)
        {
            if (talon.back().getCouleur() == joueurs[joueurActif].main[i].getCouleur())
                chercheCouleur = true;
            i++;
        }
    }
    return (c.getValeur() == talon.back().getValeur()) ||
           (c.getCouleur() == talon.back().getCouleur()) ||
           (c.getValeur() == 14) ||
           (c.getValeur() == 13); // On compare la carte que l'on a passé en paramètre à celle qui est actuellement retourné sur le talon.
}

// Met une carte de la pioche dans la main du joueur
void Jeu::piocherCarte()
{
    if (joueurActif >= nombreJoueurs) // Si on a un bot pas besoin de modif affichage.
    {
        int indexBot = joueurActif - nombreJoueurs;
        unsigned int indCarte;
        joueursBot[indexBot].main.push_back(pioche.top());
        switch (pioche.top().getCouleur())
        {
        case 1:
            cout << "On pioche une carte Rouge" << endl;
            cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteRouge() << endl;
            joueursBot[indexBot].setCarteRouge();
            cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteRouge() << endl;

            if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
            {
                indCarte = joueursBot[indexBot].getCarteRouge() - 1;
                sleep(1);
                joueursBot[indexBot].setCarteJoker(indCarte);
            }

            else if (pioche.top().getValeur() == 13)
            {
                indCarte = joueursBot[indexBot].getCarteRouge() - 1;
                sleep(1);
                joueursBot[indexBot].setCartePlus4(indCarte);
            }
            break;
        case 2:
            cout << "On pioche une carte Vert" << endl;
            cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteVert() << endl;
            joueursBot[indexBot].setCarteVert();
            cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteVert() << endl;

            if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
            {
                indCarte = joueursBot[indexBot].getCarteVert() - 1;
                sleep(1);
                joueursBot[indexBot].setCarteJoker(indCarte);
            }

            else if (pioche.top().getValeur() == 13)
            {
                indCarte = joueursBot[indexBot].getCarteVert() - 1;
                sleep(1);
                joueursBot[indexBot].setCartePlus4(indCarte);
            }
            break;
        case 3:
            cout << "On pioche une carte Bleu" << endl;
            cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteBleu() << endl;
            joueursBot[indexBot].setCarteBleu();
            cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteBleu() << endl;

            if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
            {
                indCarte = joueursBot[indexBot].getCarteBleu() - 1;
                sleep(1);
                joueursBot[indexBot].setCarteJoker(indCarte);
            }

            else if (pioche.top().getValeur() == 13)
            {
                indCarte = joueursBot[indexBot].getCarteBleu() - 1;
                sleep(1);
                joueursBot[indexBot].setCartePlus4(indCarte);
            }
            break;
        case 4:
            cout << "On pioche une carte Jaune" << endl;
            cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteJaune() << endl;
            joueursBot[indexBot].setCarteJaune();
            cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteJaune() << endl;

            if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
            {
                indCarte = joueursBot[indexBot].getCarteJaune() - 1;
                sleep(1);
                joueursBot[indexBot].setCarteJoker(indCarte);
            }

            else if (pioche.top().getValeur() == 13)
            {
                indCarte = joueursBot[indexBot].getCarteJaune() - 1;
                sleep(1);
                joueursBot[indexBot].setCartePlus4(indCarte);
            }
            break;

        default:
            break;
        }
        pioche.pop();
        joueursBot[indexBot].trierMain();
        return;
    }
    //cout << "Valeur :" << pioche.top().getValeur() << " et la couleur " << pioche.top().getCouleur() << endl;
    //termineTour();
    else
    {
        joueurs[joueurActif].main.push_back(pioche.top());
        pioche.pop();
        joueurs[joueurActif].modifMainTxt();
        joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
    }
}

void Jeu::poserCarte(const unsigned int &indiceCarte, string &messageErreur)
{
    assert(indiceCarte >= 0);
    if (joueurActif >= nombreJoueurs) // alors on a affaire à un bot.
    {
        int indexBot = joueurActif - nombreJoueurs;
        cout << "L'actuel carte du talon est : " << talon.back().getValeur() << " et sa couleur est : " << (talon.back()).getCouleur() << endl;
        cout << indiceCarte << endl;
        cout << "Le bot " << indexBot << " joue" << endl;
        cout << "La carte choisit a pour valeur : " << joueursBot[indexBot].main[indiceCarte].getValeur() << " et pour couleur " << joueursBot[indexBot].main[indiceCarte].getCouleur() << endl;
        if (carteValide(joueursBot[indexBot].main[indiceCarte]))
        {

            //cout << "La carte du bot est valide" << endl;          // La carte qu'il veut poser est valide
            talon.push(joueursBot[indexBot].main[indiceCarte]); // On pousse la carte que le joueur voulait jouer.

            joueursBot[indexBot].main.erase(joueursBot[indexBot].main.begin() + indiceCarte);
            cout << "La nouvelle carte du talon est : " << talon.back().getValeur() << " et sa couleur est : " << (talon.back()).getCouleur() << endl;
            bool carteSpeciale = false;
            if (testUno() == false) // Si on est pas dans le cas du Uno
            {
                int newIndice = -1;
                // gestion des cartes spéciales
                switch ((talon.back()).getValeur())
                {
                case 10:
                    cout << "Inverse" << endl;
                    if (sensJeu == 1)
                        sensJeu = 0;
                    else
                        sensJeu = 1;
                    break;
                case 11:
                    if (joueurActif == nombreJoueurs + nombreIA - 1 && sensJeu == 1) // Si On passe le tour du dernier joueur on revient au premier.
                        joueurActif = 0;
                    else if (joueurActif == nombreJoueurs + nombreIA - 1 && sensJeu == 0)
                    {
                        joueurActif--;
                    }
                    else if (joueurActif == 0 && sensJeu == 0)
                        joueurActif = nombreIA + nombreJoueurs - 1;
                    joueurActif++;

                    termineTour();
                    break;
                case 12:
                    termineTour();

                    piocherCarte();
                    piocherCarte();
                    carteSpeciale = true;
                    termineTour();
                    break;
                case 13:
                    carteSpeciale = true;
                    joueursBot[indexBot].setCartePlus4(newIndice);
                    termineTour();

                    for (unsigned int i = 0; i < 4; i++)
                        piocherCarte();
                    termineTour();

                    break;
                case 14:
                    joueursBot[indexBot].setCarteJoker(newIndice);
                    termineTour();
                    carteSpeciale = true;

                    break;
                }
                if (!carteSpeciale)
                    termineTour();
                if (joueursBot[indexBot].main.size() == 0)
                    annonceGagnant();
                return;
            }
            /* if (carteSpeciale && testUno() != false)
                termineTour(); */
        }
        else
        {
            messageErreur = "Cette carte ne peut pas être déposée.";
        }
    }
    else // On a un joueur normal.
    {
        if (carteValide(joueurs[joueurActif].main[indiceCarte]))
        {                                                       // La carte qu'il veut poser est valide
            talon.push(joueurs[joueurActif].main[indiceCarte]); // On pousse la carte que le joueur voulait jouer.
            joueurs[joueurActif].main.erase(joueurs[joueurActif].main.begin() + indiceCarte);

            // On appelle la fonction/Procédure qui efface le cadre de la carte et le texte.
            joueurs[joueurActif].modifMainTxt();
            // On appelle la F°/Proc qui met à jour la carte sur laquelle on joue.
            joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
            bool carteSpeciale = false;
            if (testUno() == false)
            {
                // gestion des cartes spéciales
                switch ((talon.back()).getValeur())
                {
                case 10:
                    if (sensJeu == 1)
                        sensJeu = 0;
                    else
                        sensJeu = 1;
                    break;
                case 11:
                    if (joueurActif == nombreJoueurs + nombreIA - 1 && sensJeu == 1) // Si On passe le tour du dernier joueur on revient au premier.
                        joueurActif = 0;
                    else if (joueurActif == nombreJoueurs + nombreIA - 1 && sensJeu == 0)
                    {
                        joueurActif--;
                    }
                    else if (joueurActif == 0 && sensJeu == 0)
                        joueurActif = nombreIA + nombreJoueurs - 1;
                    joueurActif++;

                    break;
                case 12:
                    termineTour();

                    piocherCarte();
                    piocherCarte();
                    carteSpeciale = true;
                    break;
                case 13:
                    carteSpeciale = true;
                    if (joueurActif == nombreJoueurs + nombreIA - 1 && sensJeu == 1) // Si On passe le tour du dernier joueur on revient au premier.
                        joueurActif = 0;
                    else if (joueurActif == nombreJoueurs + nombreIA - 1 && sensJeu == 0)
                    {
                        joueurActif--;
                    }
                    else if (joueurActif == 0 && sensJeu == 0)
                        joueurActif = nombreIA + nombreJoueurs - 1;
                    joueurActif++;
                    //termineTour();

                    for (unsigned int i = 0; i < 4; i++)
                        piocherCarte();
                    termineTour();
                    break;
                case 14:
                    termineTour();
                    carteSpeciale = true;

                    break;
                }
                if (joueurs[joueurActif].main.size() == 0)
                    annonceGagnant();
                if (carteSpeciale)
                    return;
                termineTour();
                return;
            }
        }
        else
        {

            messageErreur = "Cette carte ne peut pas être déposée.";
            // Voir si on ajoute d'autre message.
            cout << messageErreur << endl;
        }
    }
}

// Actions clavier du joueur
void Jeu::actionJoueur(const char action) // Fenêtre
{
    int indiceCarte = joueurs[joueurActif].indiceEtoile;

    switch (action)
    {
    case 'r':
        //if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
        /* if(joueurActif >= nombreJoueurs)
            joueursBot[joueurActif].main[] */

        joueurs[joueurActif].main[indiceCarte].setCouleur(1);

        break;
    case 'v':
        //if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
        joueurs[joueurActif].main[indiceCarte].setCouleur(2);

        break;
    case 'b':
        //if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
        joueurs[joueurActif].main[indiceCarte].setCouleur(3);

        break;
    case 'j':
        //if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
        joueurs[joueurActif].main[indiceCarte].setCouleur(4);

        break;
    case 'a':
        if (joueurs[joueurActif].indiceEtoile == 0) // Si on est déjà à l'indice 0 on bouge pas.
        {
            break;
        }
        else
        {
            joueurs[joueurActif].indiceEtoile--;
            break;
        }
        break;
    case 'd':
        // On déplace le curseur * à droite.
        if (joueurs[joueurActif].indiceEtoile == joueurs[joueurActif].main.size() - 1) // Si on est déjà à l'indice 0 on bouge pas.
        {
            break;
        }
        else
        {
            joueurs[joueurActif].indiceEtoile++;
            break;
        }
        break;
    case 'u':
        // uno.
        cout << "Bien joué il ne vous reste qu'une carte à poser pour gagner !!!" << endl;
        termineTour();
        break;
    case 'c':
        // Contre Uno.
        piocherCarte();
        piocherCarte();
        termineTour();
        // Passer un bool statut_Uno en vraie pour se barrer d'une boucle attendre touche.

        break;
    case 'p':
        // On Pioche.
        piocherCarte();
        termineTour();
        break;
    case 'e':
    {
        // On appuie sur la touche e = poser carte.
        // Fonction qui renvoie l'indice où est l'étoile.
        unsigned int indiceCarte = joueurs[joueurActif].indiceEtoile; // Indice de de la carte qui sera joué.
        string er;                                                    //Message d'erreur à afficher.
        poserCarte(indiceCarte, er);

        break;
    }
    default:
        break;
    }
}

// true si la pioche est vide
bool Jeu::piocheVide()
{
    return pioche.empty() == true;
}
// Réinitialisation de la pioche avec le talon
void Jeu::relancePiocheJeu()
{
    // Préalablement : on vérifie que la pioche est vide et que le bool fin != true (fin de la partie).
    while (talon.empty() != true)
    {
        // Tant que le talon n'est pas vite on met des cartes.
        pioche.push(talon.back());
        talon.pop();
    }
    // On réinit le talon avec la dernière carte ajouté à la pioche.
    initTalon();
}

// Teste si je le joueur actif est en situation de dire Uno
bool Jeu::testUno()
{
    if (joueurActif >= nombreJoueurs)
    {
        int indexBot = joueurActif - nombreJoueurs;
        if (joueursBot[indexBot].main.size() == 1) // Si il reste 1 carte dans la main du bot.
        {
            statut_Uno = true;
            return statut_Uno;
        }
    }
    else
    {
        if (joueurs[joueurActif].main.size() == 1) // Il reste 1 carte au joueur.
        {
            // On met l'affichage des boutons...
            statut_Uno = true;
            return statut_Uno;
        }
    }
}

// Actions Uno et contreUno
void Jeu::Uno(int c)
{

    switch (c)
    {
    case 'u':
        actionJoueur('u');
        cout << "UNOOOOOOOOOOOO" << endl;
        statut_Uno = false;
        break;
    case 'c':
        actionJoueur('c');
        cout << "Contre UNOOOOOO" << endl;
        statut_Uno = false;
        break;

    default:
        break;
    }
}

// Change le joueur actif, et metTour à true
void Jeu::termineTour()
{
    cout << "Fin du tour" << endl;
    cout << "Sens du Jeu" << sensJeu << endl;

    switch (sensJeu)
    {
    case 1: // On joue à gauche.
        if (joueurActif == nombreJoueurs + nombreIA - 1)
        {
            joueurActif = 0;
            finTour = true;
        }
        else
        {
            joueurActif++;
            finTour = true;
        }
        break;
    case 0: // On joue à droite
        if (joueurActif == 0)
        {
            joueurActif = nombreJoueurs + nombreIA - 1;
            finTour = true;
        }
        else
        {
            joueurActif--;
            finTour = true;
        }
        break;

    default:
        break;
    }
}
void Jeu::annonceGagnant()
{
    if (joueurActif >= nombreJoueurs && joueursBot[joueurActif - nombreJoueurs].main.size() == 0)
    {
        finPartie = true;
        cout << "C'est le " << joueursBot[joueurActif - nombreJoueurs].nom << " qui a gagné !" << endl;
        return;
    }
    else if (joueurActif < nombreJoueurs && joueurs[joueurActif].main.size() == 0)
    {
        finPartie = true;
        cout << "C'est le " << joueurs[joueurActif].nom << " qui a gagné !" << endl;
        return;
    }
}

// Modifie le nombre de cartes des adversaires dans la main des joueurs
void Jeu::modifAdversairesTxt()
{
    unsigned int pos = (180 - (nombreJoueurs - 1) * 11 - (nombreJoueurs - 2)) / 2;
    for (unsigned int i = 0; i < nombreJoueurs; i++)
    {
        for (unsigned int j = 0; j < nombreJoueurs - 1; j++)
        {
            if (joueurs[(i + 1 + j) % nombreJoueurs].main.size() >= 10)
                joueurs[i].tableJoueur[4][pos + 12 * j + 4] = '0' + (joueurs[(i + 1 + j) % nombreJoueurs].main.size()) / 10;
            joueurs[i].tableJoueur[4][pos + 12 * j + 5] = '0' + (joueurs[(i + 1 + j) % nombreJoueurs].main.size()) % 10;
        }
    }
}

// A insérer dans la boucle pour la version txt
void Jeu::MaJTableJoueurActifDebutTour()
{
    assert(sensJeu == 0 || sensJeu == 1);
    /* 
    int i = 0;
    while (joueurs[joueurActif].nom[i]!='\0') 
    {
        joueurs[joueurActif].tableJoueur[20][i]=joueurs[joueurActif].nom[i];
        i++;
    }
    joueurs[joueurActif].modifMainTxt();
    modifAdversairesTxt();
    joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche); 
    */
    // Ancien
    if (joueurActif >= nombreJoueurs)
    {
        joueurs[0].modifTalonPiocheTxt(talon, pioche);
        return;
    }
    else
    {
        joueurs[joueurActif].modifMainTxt();
        modifAdversairesTxt();
        joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
    }
}

// Teste les fonctions et procédures
void Jeu::testRegression()
{
    // test du constructeur
    assert(sensJeu == 1);

    // test de initCarte
    assert(pioche.size() == 108);

    // test de initTalon
    assert(talon.size() == 1);

    //distribueCarte();
    for (int i = 0; i < nombreJoueurs; i++)
        assert(joueurs[i].main.size() == 7);

    // test de piocheVide
    stack<Carte> temp;
    while (pioche.empty() == false)
    {
        temp.push(pioche.top());
        pioche.pop();
    }
    assert(piocheVide());

    //  test de carteValide
    Carte t = talon.front();
    Carte c1(t.getValeur(), 4);

    Carte c2(8, t.getCouleur());
    Carte c3(3, 1);
    Carte c4(5, 2);
    Carte c5(4, 3);
    assert(carteValide(c1));
    assert(carteValide(c2));
    assert(!carteValide(c3) || !carteValide(c4) || !carteValide(c5));

    // test de poserCarte

    // test de termineTour
    joueurActif = 0;
    termineTour();
    assert(joueurActif == 1);
    joueurActif = 2;
    termineTour();
    assert(joueurActif == 0);

    // relancePiocheJeu

}