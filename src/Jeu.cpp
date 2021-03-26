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

Jeu::~Jeu()
{

    delete[] joueurs;
    joueurs = NULL;

    nombreJoueurs = 0;
}

Jeu::Jeu()
{
    joueurs = NULL;
    nombreJoueurs = 0;
    sensJeu = 1;

    joueurActif = 0;
    finPartie = false;
}

Jeu::Jeu(const vector<Joueur> &joueur, const vector<bool> variante, const unsigned int nbIA)
{
    initCarte();
    nombreJoueurs = joueur.size();
    //nombreIA = nbIA;

    default_random_engine re(time(0));
    uniform_int_distribution<int> distrib{1, nombreJoueurs + nombreIA};

    joueurActif = distrib(re); // On génère un numéro de joueur aléatoire pour le début de la partie.

    sensJeu = 1;      // On tournera à gauche.
    distribueCarte(); // On donne les cartes au joueurs
    initTalon();      // On initialise le Talon.

    if (testUno())
    {
        actionJoueur('s'); // x et y à 0 car on a pas besoin de coord ici.
    }
}

void Jeu::distribueCarte()
{
    for (int i = 0; i < nombreJoueurs; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            joueurs[i].main[j] = pioche.top();
            pioche.pop();
        }
    }
}

bool Jeu::carteValide(const Carte c) const
{
    return c == talon.front(); // On compare la carte que l'on a passé en paramètre à celle qui est actuelement retourné sur le talon.
}

void Jeu::piocherCarte()
{
    joueurs[joueurActif].main.push_back(pioche.top());
}

void Jeu::actionJoueur(const char action, const Carte c = Carte(), const int x = 0, const int y = 0) // Fenêtre
{
    switch (action)
    {
    case 'q':
        // On déplace le curseur * à gauche.

        break;
    case 'd':
        // On déplace le curseur * à droite.

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
    case 's': // On appuie sur espace
        // On doit regarder quelle joueur à appuyer sur espace en premier.
        break;
    case 'e':
    {
        // On appuie sur la touche entrée.
        unsigned int indiceCarte; // Indice de de la carte à faire disparaitre.
        string er;                //Message d'erreur à afficher.
        poserCarte(c, indiceCarte, er);

        break;
    }
    default:
        break;
    }
}

void Jeu::poserCarte(const Carte c, unsigned int &indiceCarte, string &messageErreur)
{
    if (carteValide(c))
    {                  // La carte qu'il veut poser est valide
        talon.push(c); // On pousse la carte que le joueur voulait jouer.
        joueurs[joueurActif].main.erase(joueurs[joueurActif].main.begin() + indiceCarte);
        // On appelle la fonction/Procédure qui efface le cadre de la carte et le texte.

        // On appelle la F°/Proc qui met à jour la carte sur laquelle on joue.
    }
    else
    {
        messageErreur = "Cette carte ne peut pas être déposé.";
    }
}

bool Jeu::testUno()
{
    //
    showUno = true;
}

void Jeu::termineTour()
{
    if (joueurActif == nombreJoueurs + nombreIA)
    {
        joueurActif = 0;
    }
    else
    {
        joueurActif++;
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
            // 13 : changement de couleur,
            // 14 : carte +4.
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

void Jeu::testRegression()
{

    // On regarde ce que l'on met dans la pile pour vérifier que l'initialisation se fait correctement.
    if (pioche.empty())
    {
        cout << "La pioche est vide il y a un problème dans l'initialisation des cartes." << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        stack<Carte> temp;
        while (pioche.empty() == false)
        {
            temp.push(pioche.top());
            pioche.pop();
        }
        while (temp.empty() == false)
        {
            Carte t = temp.top();
            cout << "La valeur de la carte est :" << t.getValeur() << ", la couleur de la carte est : " << t.getCouleur() << endl;
            temp.pop();

            // To restore contents of
            // the original stack.
            pioche.push(t);
        }
    }
}