#include <iostream>
#include <stack> // Pour les piles.
#include <cassert>
#include <algorithm>
#include <string.h>
#include <vector>
#include <time.h>
#include <Jeu.h>
#include <unistd.h>

using namespace std;

Jeu::~Jeu()
{

    //delete[] joueurs;
    //joueurs = NULL;
}

Jeu::Jeu()
{
    //joueurs = NULL;
    nombreJoueurs = 0;
    sensJeu = 1;

    joueurActif = 0;
}

void Jeu::initJeu()
{
}

/* bool chercheValeurZero (vector<vector<int>> vec) {
    for(auto& v: vec) {
        // On cherche si dans le vecteur on a encore un 0, si on en a 1 en cherchant alors vecteur pas encore complétement à 0.
        return find(v.begin(), v.end(), 0) != v.end(); 
    }
}
 */

void Jeu::initCarte()
{
    /* vector<vector<int>> jeuCarte(15, vector<int>(4)); // Créer un vecteur [14][4]

    // Version avec une boucle for
    unsigned int i, j = 0;
    // Génère le tableau d'occurence
    for (i = 0; i < 15; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (i == 13 || i == 14)
            {
                // 12 : carte +4,
                // 13 : changement de couleur.
                cout << "Contenu tableau jeuCarte[" << i << "][" << j << "] : " << jeuCarte[i][j] << endl;
                jeuCarte[i][j] = 1;
            }
            else
            {
                jeuCarte[i][j] = 2;
                cout << "Contenu tableau jeuCarte[" << i << "][" << j << "] : " << jeuCarte[i][j] << endl;
            }
        }
    } */
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

    // vecteur jeuCarte[109].

    srand(time(NULL));

    Carte tmp;
    vector<int> indiceChoisis;
    unsigned int k = 0;
    int n = 0;
    do
    {
        i = rand() % (108 - k);
        if (find(indiceChoisis.begin(), indiceChoisis.end(), i) != indiceChoisis.end())
        {
            // TODO : faire un tableau dans lequelle on met des nombres aléatoire entre 0 et 108.
            //cout << i << endl;
            n++;
            cout << n << endl;

            continue;
        }
        indiceChoisis.push_back(i);
        //tmp = jeuCarte[i];

        //jeuCarte[i] = jeuCarte[k]; // A supprimer ?
        pioche.push(jeuCarte[i]);
        k++;
        // ensuite on échange la carte i avec la carte K
    } while (k <= 108);
    /* do
    {
        i = rand() % 14 + 1;
        j = rand() % 4 + 1;

        if (jeuCarte[i][j] == 0)
        {
            cmpt++;
            continue;
        }
        //pioche.push(Carte(i, j));

        jeuCarte[i][j] -= 1;

    } while (cmpt != 56); */
}

void Jeu::testRegression()
{

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