#include <iostream>
#include <stack> // Pour les piles.
#include <cassert>
#include <algorithm>
#include <string.h>
#include <vector>
#include <Jeu.h>
#include <unistd.h>
#include <time.h>
#include <random>
#include <chrono>

using namespace std;

Jeu::Jeu()
{
    joueurs = NULL;
    nombreJoueurs = 0;
    sensJeu = 1;
    nombreIA = 0;

    joueurActif = 0;
    finPartie = false;
    finTour = false;
    showUno = false;
}

Jeu::~Jeu()
{

    delete[] joueurs;
    joueurs = NULL;

    nombreJoueurs = 0;
}

const Joueur &Jeu::getConstEnv() const
{
    return *joueurs;
}

Jeu::Jeu(const unsigned int nbjoueurs, const unsigned int nbIA = 0)
{
    initCarte();
    nombreJoueurs = nbjoueurs;
    nombreIA = nbIA;

    // création du tableau joueurs
    Joueur *joueurs = new Joueur[nombreJoueurs];
    for (unsigned int i = 0; i < nombreJoueurs; i++)
    {
        Joueur joueur(i + 1);
        joueurs[i] = joueur;
    }

    default_random_engine re(time(0));
    uniform_int_distribution<int> distrib{1, nombreJoueurs + nombreIA};

    joueurActif = distrib(re); // On génère un numéro de joueur aléatoire pour le début de la partie.

    sensJeu = 1;      // On tournera à gauche.
    distribueCarte(); // On donne les cartes au joueurs
    initTalon();      // On initialise le Talon.
    finTour = false;

    unsigned int pos = (180 - (nombreJoueurs - 1) * 11 - (nombreJoueurs - 2)) / 2;

    for (unsigned int i = 0; i < nombreJoueurs; i++)
    {
        for (unsigned int j = 0; j < nombreJoueurs - 1; j++)
        {
            joueurs[i].insererCarteAdversairePositionJ(pos + 12 * j, joueurs[(i + 1 + j) % nombreJoueurs].numeroJoueur);
            joueurs[i].tableJoueur[4][pos + 12 * j + 4] = (joueurs[(i + 1 + j) % nombreJoueurs].main.size()) / 10;
            joueurs[i].tableJoueur[4][pos + 12 * j + 5] = (joueurs[(i + 1 + j) % nombreJoueurs].main.size()) % 10;
        }
    }
}

void Jeu::distribueCarte()
{
    for (unsigned int i = 0; i < nombreJoueurs; i++)
    {
        for (unsigned int j = 0; j < 7; j++)
        {
            joueurs[i].main[j] = pioche.top();
            pioche.pop();
        }
        joueurs[i].modifMainTxt();
    }
}

bool Jeu::carteValide(const Carte c) const
{
    bool chercheCouleur = false;
    if (c.getValeur() == 13)
    {
        unsigned int i = 0;
        while (i < joueurs[joueurActif].main.size() && !chercheCouleur)
        {
            if ((c.getCouleur() == joueurs[joueurActif].main[i].getCouleur()))
                chercheCouleur = true;
            i++;
        }
    }
    return (c.getValeur() == talon.front().getValeur()) ||
           (c.getCouleur() == talon.front().getCouleur()) ||
           (c.getValeur() == 14) ||
           (c.getValeur() == 13 && chercheCouleur == false); // On compare la carte que l'on a passé en paramètre à celle qui est actuellement retourné sur le talon.
}

void Jeu::piocherCarte()
{
    joueurs[joueurActif].main.push_back(pioche.top());
    joueurs[joueurActif].modifMainTxt();
    joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
}

void Jeu::actionJoueur(const char action, const int x = 0, const int y = 0) // Fenêtre
{
    switch (action)
    {
    case 'r':
        if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
            (talon.front()).setCouleur(1);
    case 'v':
        if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
            (talon.front()).setCouleur(2);
    case 'b':
        if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
            (talon.front()).setCouleur(3);
    case 'j':
        if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
            (talon.front()).setCouleur(4);
    case 'a':
        if (joueurs[joueurActif].indiceEtoile == 0) // Si on est déjà à l'indice 0 on bouge pas.
        {
            break;
        }
        else
        {
            joueurs[joueurActif].indiceEtoile--;
        }
        break;
    case 'd':
        // On déplace le curseur * à droite.
        if (joueurs[joueurActif].indiceEtoile == joueurs[joueurActif].main.size()) // Si on est déjà à l'indice 0 on bouge pas.
        {
            break;
        }
        else
        {
            joueurs[joueurActif].indiceEtoile++;
        }

        break;
    /* case 'c':
        if ((x >= (5/16*dimx) && y >= (3/8*dimy))&&
        ( x <= (7/16*dimx) && y <= (5/8*dimy)))
        { // On clique sur la pioche.
            piocherCarte();
        }
        if ((x >= (13/16*dimx) && y >= (dimy*7/16)) &&
        (x <= 15/16*dimx) && (y <= (9/16*dimy)) ) 
        { // On clique sur passer.
        }
        if ((x >= (1/32*dimx) && y >= (dimy*1/32)) &&
        (x <= (1/4*dimx) && (y <= (1/6*dimy))))
        { // On clique sur Paramètres.
        
        }
    */
    case 'u':
        // uno.

        break;
    case 'c':
        // Contre Uno.
    case 'p':
        // On Pioche.
        piocherCarte();
    case 'e':
    {
        // On appuie sur la touche entrée = poser carte.
        // Fonction qui renvoie l'indice où est l'étoile.

        unsigned int indiceCarte = joueurs[joueurActif].main[joueurActif].positionEtoile; // Indice de de la carte qui sera joué.
        string er;                                                                        //Message d'erreur à afficher.
        poserCarte(indiceCarte, er);

        break;
    }
    default:
        break;
    }
}

void Jeu::poserCarte(unsigned int &indiceCarte, string &messageErreur)
{
    if (carteValide(joueurs[joueurActif].main[indiceCarte]))
    {                                                       // La carte qu'il veut poser est valide
        talon.push(joueurs[joueurActif].main[indiceCarte]); // On pousse la carte que le joueur voulait jouer.
        joueurs[joueurActif].main.erase(joueurs[joueurActif].main.begin() + indiceCarte);

        // On appelle la fonction/Procédure qui efface le cadre de la carte et le texte.
        joueurs[joueurActif].modifMainTxt();
        // On appelle la F°/Proc qui met à jour la carte sur laquelle on joue.
        joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);

        // gestion des cartes spéciales
        switch ((talon.front()).getValeur())
        {
        case 10:
            sensJeu += (-1) * sensJeu;
            break;
        case 11:
            joueurActif++;
            break;
        case 12:
            joueurActif++;
            piocherCarte();
            piocherCarte();
            break;
        case 13:
            joueurActif++;
            for (unsigned int i = 0; i < 4; i++)
                piocherCarte();
            break;
        case 14:
            break;
        }
    }
    else
    {
        messageErreur = "Cette carte ne peut pas être déposée.";
        // Voir si on ajoute d'autre message.
    }
}

/* bool Jeu::testUno()
{
    // si le joueur a 2 cartes il pourra dire Uno après avoir posé 1 carte
    return (joueurs[joueurActif].main).size() == 2;
}
*/

void Jeu::termineTour()
{
    switch (sensJeu)
    {
    case 1: // On joue à gauche.
        if (joueurActif == nombreJoueurs + nombreIA)
        {
            joueurActif = 0;
        }
        else
        {
            joueurActif++;
        }
        break;
    case 0: // On joue à droite
        if (joueurActif == 0)
        {
            joueurActif = nombreJoueurs + nombreIA;
        }
        else
        {
            joueurActif--;
        }
        break;

    default:
        break;
    }
}

void Jeu::initCarte()
{
    unsigned int i, j = 0;

    vector<Carte> jeuCarte;
    for (i = 1; i < 5; i++) // Couleur
    {
        jeuCarte.push_back(Carte(0, i)); // On met les quatres 0 de chaque couleur à chaque fois que l'on change de couleur.
        for (j = 1; j < 25; j++)         // Numéro
        {
            if (j > 12)
            {
                jeuCarte.push_back(Carte(j - 12, i));
            }
            else
            {
                jeuCarte.push_back(Carte(j, i));
            }
        }
    }

    for (i = 1; i < 5; i++) // Couleur : [1] Rouge, [2] Vert, [3] Bleu, [4] jaune
    {
        for (j = 13; j < 15; j++) // Numéro
        {
            // 14 : changement de couleur,
            // 13 : carte +4.
            jeuCarte.push_back(Carte(j, i));
        }
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(jeuCarte.begin(), jeuCarte.end(), std::default_random_engine(seed));

    unsigned int l = 0;
    do
    {
        pioche.push(jeuCarte[l]);
        l++;
    } while (l < jeuCarte.size());
}

void Jeu::initTalon()
{
    talon.push(pioche.top()); // Carte mis dans la talon, celle sur laquelle on va jouer.
    pioche.pop();             // Elle dans la file, on la supprime de la pile.
}

bool Jeu::piocheVide()
{ // Dis si la pioche est vide ou non.
    return pioche.empty() == true;
}

void Jeu::relancePiocheJeu()
{
    // Préalablement : on vérifie que la pioche est vide et que le bool fin != true (fin de la partie).
    while (talon.empty() != true)
    {
        // Tant que le talon n'est pas vite on met des cartes.
        pioche.push(talon.front());
        talon.pop();
    }
    // On réinit le talon avec la dernière carte ajouté à la pioche.
    initTalon();
}

// à insérer dans la boucle pour la version txt
void Jeu::MaJTableJoueurActifDebutTour()
{
    assert(sensJeu == 0 || sensJeu == 1);
    joueurs[joueurActif].modifMainTxt();
    joueurs[joueurActif].modifAdversairesTxt(joueurs, nombreJoueurs);
    joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
}

void Jeu::testRegression()
{
    // test du constructeur
    assert(nombreJoueurs == 0);
    assert(sensJeu == 1);
    assert(joueurActif == 0);

    // test de piocheVide
    assert(piocheVide());

    // test de initCarte
    initCarte();
    assert(pioche.size() == 104);

    // test de initTalon
    assert(pioche.size() == 103);
    assert(talon.size() == 1);

    /* en attente
    
    nombreJoueur=3;
    Joueur joueur1(1,"joueur 1");
    Joueur joueur2(2,"joueur 2");
    Joueur joueur3(3,"joueur 3");


    // test de distribueCarte
    jeuTest.distribuerCarte();
    (assert (jeuTest.joueur1.main).size() == 7);
    (assert (jeuTest.joueur2.main).size() == 7);
    (assert (jeuTest.joueur3.main).size() == 7);

    */

    // test de carteValide
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
    nombreIA = 0;
    termineTour();
    assert(joueurActif == 1);
    joueurActif = 2;
    termineTour();
    assert(joueurActif == 0);

    // relancePiocheJeu
}