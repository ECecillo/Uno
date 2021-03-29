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

Jeu::Jeu(const vector<Joueur> &joueur, const vector<bool> variante, const unsigned int nbIA)
{
    initCarte();
    nombreJoueurs = joueur.size();
    nombreIA = nbIA;

    default_random_engine re(time(0));
    uniform_int_distribution<int> distrib{1, nombreJoueurs + nombreIA};

    joueurActif = distrib(re); // On génère un numéro de joueur aléatoire pour le début de la partie.

    sensJeu = 1;      // On tournera à gauche.
    distribueCarte(); // On donne les cartes au joueurs
    initTalon();      // On initialise le Talon.
    finTour = false;
    
    
    if (finTour) {
        termineTour();
    }
    if (testUno())
    {
        actionJoueur('s'); // x et y à 0 car on a pas besoin de coord ici.
    }
    unsigned int pos = (180-(nbjoueurs-1)*11-(nbjoueurs-2))/2;

    for (unsigned int i = 0; i<nbjoueurs; i++)
    {
        for (unsigned int j = 0; j<nbjoueurs-1; j++)
        {
            joueurs[i].insererCarteAdversairePositionJ(pos+12*j,joueurs[i-1-j].numeroJoueur);
            joueurs[i].tableJoueur[4][pos+12*j+4] = (joueurs[i-1-j].main.size()) / 10;
            joueurs[i].tableJoueur[4][pos+12*j+5] = (joueurs[i-1-j].main.size()) % 10;
        }
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
        joueurs[i].modifMainTxt();
    }
}

bool Jeu::carteValide(const Carte c) const
{
    return c == talon.front(); // On compare la carte que l'on a passé en paramètre à celle qui est actuelement retourné sur le talon.
}

void Jeu::piocherCarte()
{
    joueurs[joueurActif].main.push_back(pioche.top());
    joueurs[joueurActif].modifMainTxt();
    joueurs[joueurActif].modifTalonPioche();
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
        unsigned int indiceCarte; // Indice de de la carte qui sera joué.
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
        joueurs[joueurActif].modifMainTxt();
        // On appelle la F°/Proc qui met à jour la carte sur laquelle on joue.
        joueurs[joueurActif].modifTalonPioche();
    }
    else
    {
        messageErreur = "Cette carte ne peut pas être déposé."; 
        // Voir si on ajoute d'autre message.
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

// à insérer dans la boucle pour la version txt
void Jeu::MaJTableJoueurActifDebutTour()
{
    joueurs[joueurActif].modifAdversairesTxt();
    joueurs[JoueurActif].modifTalonPioche();
}

void Jeu::testRegression()
{
    new Jeu jeuTest = Jeu;

    // test du constructeur
    assert (jeuTest.nombreJoueurs==0);
    assert (jeuTest.sensJeu==1);
    assert (jeuTest.joueurActif==0);

    // test de piocheVide
    assert (jeuTest.piocheVide());

    
    // test de initCarte
    jeuTest.initCarte();
    assert (jeuTest.pioche.size()==104);

    // test de initTalon
    assert (jeuTest.pioche.size()==103);
    assert (jeuTest.talon.size()==1);
    
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
    Carte t = jeuTest.talon.front();
    Carte c1(t.getValeur(), 4);
    Carte c2(8,t.getCouleur());
    Carte c3(3,1);
    Carte c4(5,2);
    Carte c5(4,3);
    assert (jeuTest.carteValide(c1));
    assert (jeuTest.carteValide(c2));
    assert (!jeuTest.carteValide(c3) || !jeuTest.carteValide(c4) || !jeuTest.carteValide(c5));

    // test de poserCarte

    

    // test de termineTour
    nombreIA=0;
    jeuTest.termineTour();
    assert(jeuTest.joueurActif == 1);
    jeuTest.joueurActif = 2;
    jeuTest.termineTour();
    assert(jeuTest.joueurActif == 0);

    // relancePiocheJeu
    

    }
}