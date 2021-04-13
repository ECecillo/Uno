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

using namespace std;

// constructeur par défaut
Jeu::Jeu()
{
    joueurs = NULL;
    nombreJoueurs = 0;
    sensJeu = 1;
    nombreIA = 0;

    joueurActif = 0;
    finPartie = false;
    finTour = false;
    statut_Uno = false;
}

Jeu::~Jeu()
{
    delete[] joueurs;
    joueurs = NULL;
    sensJeu = 1;
    joueurActif = 0;
    nombreJoueurs = 0;
}

Jeu::Jeu(const unsigned int nbjoueurs, const unsigned int nbIA = 0)
{
    initCarte();
    nombreJoueurs = nbjoueurs;
    nombreIA = nbIA;
    // création du tableau joueurs
    joueurs = new Joueur[nombreJoueurs];
    for (unsigned int i = 0; i < nombreJoueurs; i++)
    {
        Joueur joueur(i + 1);
        joueurs[i] = joueur;
    }
    joueurActif = rand() % nombreJoueurs; // On génère un numéro de joueur aléatoire pour le début de la partie.
    sensJeu = 1;                          // On tournera à gauche.
    distribueCarte();                     // On donne les cartes au joueurs
    initTalon();                          // On initialise le Talon.
    finTour = false;
    statut_Uno = false;
    finPartie = false;

    unsigned int pos = (180 - (nombreJoueurs - 1) * 11 - (nombreJoueurs - 2)) / 2;

    for (unsigned int i = 0; i < nombreJoueurs; i++)
    {
        for (unsigned int j = 0; j < nombreJoueurs - 1; j++)
        {
            joueurs[i].insererCarteAdversairePositionJ(pos + 12 * j, joueurs[(i + 1 + j) % nombreJoueurs].numeroJoueur);
            /*if (joueurs[(i + 1 + j) % nombreJoueurs].main.size()>=10)
                joueurs[i].tableJoueur[4][pos + 12 * j + 4] = (joueurs[(i + 1 + j) % nombreJoueurs].main.size()) / 10;
            joueurs[i].tableJoueur[4][pos + 12 * j + 5] = (joueurs[(i + 1 + j) % nombreJoueurs].main.size()) % 10;*/
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
// 7 cartes par joueur
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
}

// true si la carte est valide
bool Jeu::carteValide(const Carte c) const
{
    bool chercheCouleur = false;
    cout << "Carte Valide" << endl;
    //si carte +4, on regarde dans la main du joueur s'il y a une carte de la même couleur que celle du talon
    /* if (c.getValeur() == 13)
    {
        unsigned int i = 0;
        while (i < joueurs[joueurActif].main.size() && !chercheCouleur)
        {
            if (talon.back().getCouleur() == joueurs[joueurActif].main[i].getCouleur())
                chercheCouleur = true;
            i++;
        }
    } */
    cout << talon.back().getValeur() << talon.back().getCouleur() << endl;
    return (c.getValeur() == talon.back().getValeur()) ||
           (c.getCouleur() == talon.back().getCouleur()) ||
           (c.getValeur() == 14) ||
           (c.getValeur() == 13); // On compare la carte que l'on a passé en paramètre à celle qui est actuellement retourné sur le talon.
}

// Met une carte de la pioche dans la main du joueur
void Jeu::piocherCarte()
{
    joueurs[joueurActif].main.push_back(pioche.top());
    cout << "Valeur :" << pioche.top().getValeur() << " et la couleur " << pioche.top().getCouleur() << endl;
    //termineTour();
    pioche.pop();
    joueurs[joueurActif].modifMainTxt();
    joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
}

void Jeu::poserCarte(const unsigned int &indiceCarte, string &messageErreur)
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
            if (joueurActif == nombreJoueurs - 1) // Si On passe le tour du dernier joueur on revient au premier.
                joueurActif = 0;
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

            termineTour();

            for (unsigned int i = 0; i < 4; i++)
                piocherCarte();
            break;
        case 14:
            carteSpeciale = true;

            break;
        }

        if (carteSpeciale)
            return;
        if (testUno() == false)
        {
            termineTour();
        }
    }
    else
    {

        messageErreur = "Cette carte ne peut pas être déposée.";
        // Voir si on ajoute d'autre message.
        cout << messageErreur << endl;
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
        cout << "Hello" << endl;
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
    if (joueurs[joueurActif].main.size() == 1) // Il reste 1 carte au joueur.
    {
        // On met l'affichage des boutons...
        statut_Uno = true;
        return statut_Uno;
    }
}

// Actions Uno et contreUno
void Jeu::Uno(int c)
{

    switch (c)
    {
    case 'u':
        actionJoueur('u');
        statut_Uno = false;
        break;
    case 'c':
        actionJoueur('c');
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
        if (joueurActif == nombreJoueurs - 1)
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
            joueurActif = nombreJoueurs - 1;
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
    joueurs[joueurActif].modifMainTxt();
    modifAdversairesTxt();
    joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
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

    // Test Affichage Joueur.cpp
    /*cout << joueurs->getHaut() << joueurs->getLarg() << endl;*/
    /* for(unsigned int i = 0; i < joueurs->getHaut(); i++)
        for(unsigned int j = 0; j < joueurs->getLarg(); j++)
            cout << joueurs[joueurActif].tableJoueur[i][j] << endl;
 */
}