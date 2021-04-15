#include <Bot.h>

using namespace std;

Bot::Bot()
{
    numeroBot = 0;
    nbCarteRouge = 0;
    nbCarteVert = 0;
    nbCarteBleu = 0;
    nbCarteJaune = 0;
    nom = "";
}

Bot::Bot(const unsigned int num)
{
    numeroBot = num;
    nbCarteRouge = 0;
    nbCarteVert = 0;
    nbCarteBleu = 0;
    nbCarteJaune = 0;
    nom = "Bot ";
    nom += to_string(numeroBot);
}
// Trie les cartes du joueur par couleur puis par valeur
void Bot::trierMain()
{
    Carte cMin;
    int indMin;
    for (int i = 0; i < main.size(); i++)
    {
        cMin = main[i];
        indMin = i;
        for (int j = i + 1; j < main.size(); j++)
            if (main[j] < cMin)
            {
                cMin = main[j];
                indMin = j;
            }
        main.insert(main.begin() + i, cMin);
        main.erase(main.begin() + indMin + 1);
    }
}

int Bot::carteMemeCouleurTalon(const Jeu &jeu) const
{
    // Selon la couleur de la carte du talon on retourne les variables.
    switch (jeu.talon.back().getCouleur())
    {
    case 1:
        return nbCarteRouge;
        break;
    case 2:
        return nbCarteVert;
        break;
    case 3:
        return nbCarteBleu;
        break;
    case 4:
        return nbCarteJaune;
        break;

    default:
        break;
    }
}

int Bot::carteMemeValeurTalon(const Jeu &jeu, char &c) const
{
    // variable contenant le nombre de Carte avec la couleur de la carte qui a la meme valeur que celle du talon.
    int nbCarteMemeCouleur = 0;
    // On créer une nouvelle variable pour mettre le bonne indice du bot que l'on regarde.
    int indexBot = numeroBot - 1;
    for (int i = 0; i < jeu.joueursBot[indexBot].main.size(); i++)
    {
        if (jeu.joueursBot[indexBot].main[i].getValeur() == jeu.talon.back().getValeur()
            && jeu.joueursBot[indexBot].main[i].getCouleur() != jeu.talon.back().getCouleur())
        {
            // On récupère la Couleur de la carte qui a la même valeur que talon
            int couleurCarte = jeu.joueursBot[indexBot].main[i].getCouleur();
            // Variable pour boucle tant que.
            int k = 0;
            // variable temp qui permet de compter le nombre de carte que l'on comparera si jamais au nombre de carte déjà trouvé dans d'autre couleur.
            int compareValeur = 0;
            switch (couleurCarte)
            {
            case 1:
                k = 0;
                while (jeu.joueursBot[indexBot].main[k].getCouleur() == 1) // Si on est dans le cas d'une valeur rouge.
                {
                    // Si on a des cartes spéciales dans cette couleur alors on met une valeur plus élevé à jouer cette couleur.
                    if (jeu.joueursBot[indexBot].main[k].getValeur() == 10)
                    {
                        compareValeur = compareValeur + 2;
                        k++;
                    }
                    else if (jeu.joueursBot[indexBot].main[k].getValeur() == 11 ||
                             jeu.joueursBot[indexBot].main[k].getValeur() == 12)
                    {
                        compareValeur = compareValeur + 5;
                        k++;
                    }
                    // On gère le cas des cartes spéciales +4 et joker qui ne représentent pas une valeur pour la couleur.
                    else if (jeu.joueursBot[indexBot].main[k].getValeur() == 13 ||
                             jeu.joueursBot[indexBot].main[k].getValeur() == 14)
                        k++;
                    else
                    {

                        compareValeur++;
                        k++;
                    }
                }
                // Si le nombre de carte est supérieur au nombre de carte déjà trouvé alors on garde la plus grande.
                if (compareValeur > nbCarteMemeCouleur)
                {
                    nbCarteMemeCouleur = compareValeur;
                    c = 'r';
                }

                break;
            case 2:
                k = nbCarteRouge;
                while (jeu.joueursBot[indexBot].main[k].getCouleur() == 2) // Si on est dans le cas d'une valeur rouge.
                {

                    // Si on a des cartes spéciales dans cette couleur alors on met une valeur plus élevé à jouer cette couleur.
                    if (jeu.joueursBot[indexBot].main[k].getValeur() == 10)
                    {
                        compareValeur = compareValeur + 2;
                        k++;
                    }
                    else if (jeu.joueursBot[indexBot].main[k].getValeur() == 11 ||
                             jeu.joueursBot[indexBot].main[k].getValeur() == 12)
                    {
                        compareValeur = compareValeur + 5;
                        k++;
                    }
                    // On gère le cas des cartes spéciales +4 et joker qui ne représentent pas une valeur pour la couleur.
                    else if (jeu.joueursBot[indexBot].main[k].getValeur() == 13 ||
                             jeu.joueursBot[indexBot].main[k].getValeur() == 14)
                        k++;
                    else
                    {
                        compareValeur++;
                        k++;
                    }
                }
                if (compareValeur > nbCarteMemeCouleur)
                {
                    nbCarteMemeCouleur = compareValeur;
                    c = 'v';
                }

                break;
            case 3:
                k = nbCarteRouge + nbCarteVert;
                while (jeu.joueursBot[indexBot].main[k].getCouleur() == 3 && k < jeu.joueursBot[indexBot].main.size()) // Si on est dans le cas d'une valeur rouge.
                {

                    // Si on a des cartes spéciales dans cette couleur alors on met une valeur plus élevé à jouer cette couleur.
                    if (jeu.joueursBot[indexBot].main[k].getValeur() == 10)
                    {
                        compareValeur = compareValeur + 2;
                        k++;
                    }
                    else if (jeu.joueursBot[indexBot].main[k].getValeur() == 11 ||
                             jeu.joueursBot[indexBot].main[k].getValeur() == 12)
                    {
                        compareValeur = compareValeur + 5;
                        k++;
                    }
                    // On gère le cas des cartes spéciales +4 et joker qui ne représentent pas une valeur pour la couleur.
                    else if (jeu.joueursBot[indexBot].main[k].getValeur() == 13 ||
                             jeu.joueursBot[indexBot].main[k].getValeur() == 14)
                        k++;
                    else
                    {
                        compareValeur++;
                        k++;
                    }
                }
                if (compareValeur > nbCarteMemeCouleur)
                {
                    nbCarteMemeCouleur = compareValeur;
                    c = 'b';
                }

                break;
            case 4:
                k = jeu.joueursBot[indexBot].main.size() - 1;
                while (jeu.joueursBot[indexBot].main[k].getCouleur() == 4) // Si on est dans le cas d'une valeur Jaune.
                {
                    cout << jeu.joueursBot[indexBot].main[k].getValeur() << endl;
                    // Si on a des cartes spéciales dans cette couleur alors on met une valeur plus élevé à jouer cette couleur.
                    if (jeu.joueursBot[indexBot].main[k].getValeur() == 10)
                    {
                        compareValeur = compareValeur + 2;
                        k--;
                    }
                    else if (jeu.joueursBot[indexBot].main[k].getValeur() == 11 ||
                             jeu.joueursBot[indexBot].main[k].getValeur() == 12)
                    {
                        compareValeur = compareValeur + 5;
                        k--;
                    }
                    // On gère le cas des cartes spéciales +4 et joker qui ne représentent pas une valeur pour la couleur.
                    else if (jeu.joueursBot[indexBot].main[k].getValeur() == 13 ||
                             jeu.joueursBot[indexBot].main[k].getValeur() == 14)
                        k--;
                    else
                    {
                        compareValeur++;
                        k--;
                    }
                }
                if (compareValeur > nbCarteMemeCouleur)
                {
                    nbCarteMemeCouleur = compareValeur;
                    c = 'j';
                }

                break;

            default:
                break;
            }
        }
    }
    return nbCarteMemeCouleur;
}

void Bot::choixJeu(const Jeu &jeu)
{
    int nbCarteCouleur, nbCarteValeur;

    nbCarteCouleur = carteMemeCouleurTalon(jeu); // Nombre de carte meme couleur que talon.
    char couleur;                                // Caractère qui permettra de savoir quelle couleur a le plus de carte.
    nbCarteValeur = carteMemeValeurTalon(jeu, couleur);
    // On compare les deux en prenant en compte que c'est mieux de finir une couleur avant d'en changer
    if (!(nbCarteCouleur == 0) && !(nbCarteValeur == 0)) // Cas où on peut jouer.
    {
        if (nbCarteCouleur > nbCarteValeur)
        {
            // On joue sur les cartes avec la couleur du talon.
            // string er;
            // PoserCarte();
        }
    }
    else
    {
        // piocher.
    }
}

void Bot::setCarteRouge()
{
    nbCarteRouge++;
}
void Bot::setCarteVert()
{
    nbCarteVert++;
}
void Bot::setCarteBleu()
{
    nbCarteBleu++;
}
void Bot::setCarteJaune()
{
    nbCarteJaune++;
}

void Bot::testRegression(const Jeu &jeu)
{
    assert(main.size() >= 7);
    assert(numeroBot >= 0);
    assert(!nom.empty());
    trierMain();
    // Affiche les cartes dans la main du bot.
    for (auto &i : main)
    {
        cout << "La valeur est " << i.getValeur() << " et la couleur " << i.getCouleur() << endl;
    }
    //cout << "Nombre de carte de couleur Rouge " << nbCarteRouge << endl;
    //cout << "Nombre de carte de couleur Vert" << nbCarteVert << endl;
    //cout << "Nombre de carte de couleur Bleu" << nbCarteBleu << endl;
    //cout << "Nombre de carte de couleur Jaune" << nbCarteJaune << endl;

    cout << "La carte du talon est " << jeu.talon.back().getValeur() << " et la couleur est " << jeu.talon.back().getCouleur() << endl;
    int CouleurTalon = carteMemeCouleurTalon(jeu);
    char c;
    int nbValeur = carteMemeValeurTalon(jeu, c);

    cout << "Le nombre de carte qui ont la couleur du talon : " << CouleurTalon << endl;
    cout << "Le nombre de carte qui ont la valeur du talon : " << nbValeur << endl;
    switch (c)
    {
    case 'r':
        cout << "La couleur est Rouge" << endl;
        break;
    case 'v':
        cout << "La couleur est Vert" << endl;
        break;
    case 'b':
        cout << "La couleur est Bleu" << endl;
        break;
    case 'j':
        cout << "La couleur est Jaune" << endl;
        break;
    }
}
