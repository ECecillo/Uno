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
    default_random_engine re(time(0));
    uniform_int_distribution<int> distrib{1, nombreJoueurs + nombreIA};
    joueurActif = distrib(re); // On génère un numéro de joueur aléatoire pour le début de la partie.
    sensJeu = 1;               // On tournera à gauche.
    distribueCarte();          // On donne les cartes au joueurs
    initTalon();               // On initialise le Talon.
    finTour = false;
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

bool Jeu::carteValide(const Carte c) const
{
    bool chercheCouleur = false;
    //si carte +4, on regarde dans la main du joueur s'il y a une carte de la même couleur que celle du talon
    if (c.getValeur() == 13)
    {
        unsigned int i = 0;
        while (i < joueurs[joueurActif].main.size() && !chercheCouleur)
        {
            if (talon.front().getCouleur() == joueurs[joueurActif].main[i].getCouleur())
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
        //if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
        cout << "Hello" << endl;
        (talon.front()).setCouleur(1);
    case 'v':
        //if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
        
        (talon.front()).setCouleur(2);
    case 'b':
        //if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
        (talon.front()).setCouleur(3);

    case 'j':
        //if ((talon.front()).getValeur() == 13 || (talon.front()).getValeur() == 14)
        (talon.front()).setCouleur(4);
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
        }

        break;
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
        // On appuie sur la touche e = poser carte.
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
            if(joueurActif == nombreJoueurs) // Si On passe le tour du dernier joueur on revient au premier.
                joueurActif = 0;
            joueurActif++;
            break;
        case 12:
            joueurActif++;
            piocherCarte();
            piocherCarte();
            break;
        case 13: // Pioche 4 carte.
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

/* bool testUno()
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

// à insérer dans la boucle pour la version txt
void Jeu::MaJTableJoueurActifDebutTour()
{
    assert(sensJeu == 0 || sensJeu == 1);
    joueurs[joueurActif].modifMainTxt();
    modifAdversairesTxt();
    joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
}

void Jeu::testRegression()
{
    // test du constructeur
    //for (int i=0; i<nombreJoueurs; i++)
    //    assert(joueurs[i].main.size()==7);
    assert(sensJeu == 1);

    // test de piocheVide
    //assert(piocheVide());

    // test de initCarte
    //cout << pioche.size() << endl;
    //assert(pioche.size() == 108);
    //assert(talon.size() == 1);

    //distribueCarte();
    cout << "Numéro du joueur 1 est : " << joueurs[0].nom << endl;

    /*    
   stack<Carte> temp;
    while (pioche.empty() == false)
    {
        temp.push(pioche.top());
        pioche.pop();
    }  
 
    while (temp.empty() == false)
    {
        Carte t = temp.top();
        cout << "La valeur de la carte est "<<t.getValeur() << " et la couleur est " << t.getCouleur() << endl;
        temp.pop();
    }
    cout << "===================================================== " << endl;
	cout<<endl; 
*/
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