#include <Jeu.h>
#include <cassert>
#include <algorithm>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

Jeu::Jeu()
{
}

Jeu::~Jeu()
{

    delete[] joueurs;
    joueurs = NULL;
}

Jeu::Jeu()
{
    joueurs = NULL;
    nombreJoueurs = 0;
    sensJeu = 1;

    joueurActif = 0;
}

void Jeu::initJeu()
{
}

void Jeu::initCarte()
{
    vector<vector<Carte>> jeuCarte(14, vector<Carte>(4)); // Créer un vecteur [14][4]

    // Version avec une boucle for
    unsigned int i, j = 0;
    // Génère le tableau d'occurence
    for (i = 0; i < 14; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (i == 8 || i == 12 || i == 13)
                jeuCarte[i][j] = 1;
            jeuCarte[i][j] = 2;
        }
    }

    srand(time(NULL));

    bool checkValeurVecteur = all_of(jeuCarte.begin(), jeuCarte.end(), [](const vector<int> &checkVector) {
        return all_of(checkVector.begin(), checkVector.end(), [](int x) { return x == 0; });
    });

    do
    {
        i = rand() % 14 + 1;
        j = rand() % 4 + 1;
        int val = jeuCarte[i][j];
        jeuCarte[i][j] -= 1;        
        pioche.push(val);


    } while (jeuCarte[i][j] != 0 && checkValeurVecteur != true);

    // Version avec les fills \-0-/
    /*
    fill(jeuCarte.begin(), jeuCarte.begin()+8, 2);
    fill(jeuCarte.begin()+8, jeuCarte.begin()+9, 1);
    fill(jeuCarte.begin()+9, jeuCarte.end()-2, 2);
    fill(jeuCarte.end()-2, jeuCarte.end(), 1);
  */
}