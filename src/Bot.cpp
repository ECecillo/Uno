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
    nom = "Bot ";
    nom += to_string(numeroBot);
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

int Bot::carteMemeValeurTalon(const Jeu &jeu) const
{
    // Compte le nombre de Carte avec la couleur
    int nbCarteValeur = 0;
    for (int i = 0, n = jeu.joueursBot[numeroBot].main.size(); i < n; i++)
    {
        if (jeu.joueursBot[numeroBot].main[i].getValeur() == jeu.talon.back().getValeur())
        {
            int couleurCarte = jeu.joueursBot[numeroBot].main[i].getCouleur();
            // nous donne une couleur entre 1 et 4.
            switch (jeu.talon.back().getCouleur())
            {
            case 1:
                int k = 0;
                while (jeu.joueursBot[numeroBot].main[k].getCouleur() == 1) // Si on est dans le cas d'une valeur rouge.
                {
                    // Si on a des cartes spéciales dans cette couleur alors on met une valeur plus élevé à jouer cette couleur.
                    if (jeu.joueursBot[numeroBot].main[k].getValeur() == 10)
                    {
                        nbCarteValeur + 2;
                        k++;
                    }
                    else if (jeu.joueursBot[numeroBot].main[k].getValeur() == 11 ||
                             jeu.joueursBot[numeroBot].main[k].getValeur() == 12)
                    {
                        nbCarteValeur + 5;
                        k++;
                    }
                    // On gère le cas des cartes spéciales +4 et joker qui ne représentent pas une valeur pour la couleur.
                    else if (jeu.joueursBot[numeroBot].main[k].getValeur() == 13 ||
                             jeu.joueursBot[numeroBot].main[k].getValeur() == 14)
                        k++;
                    else
                    {
                        nbCarteValeur++;
                        k++;
                    }
                }
                break;
            case 2:
                int k = nbCarteRouge + 1;
                while (jeu.joueursBot[numeroBot].main[k].getCouleur() == 2) // Si on est dans le cas d'une valeur rouge.
                {
                    // Si on a des cartes spéciales dans cette couleur alors on met une valeur plus élevé à jouer cette couleur.
                    if (jeu.joueursBot[numeroBot].main[k].getValeur() == 10)
                    {
                        nbCarteValeur + 2;
                    }
                    else if (jeu.joueursBot[numeroBot].main[k].getValeur() == 11 ||
                             jeu.joueursBot[numeroBot].main[k].getValeur() == 12)
                        nbCarteValeur + 5;
                    // On gère le cas des cartes spéciales +4 et joker qui ne représentent pas une valeur pour la couleur.
                    else if (jeu.joueursBot[numeroBot].main[k].getValeur() == 13 ||
                             jeu.joueursBot[numeroBot].main[k].getValeur() == 14)
                        k++;
                    else
                    {
                        nbCarteValeur++;
                        k++;
                    }
                }
                break;
            case 3:
                int k = nbCarteRouge + nbCarteVert + 1;
                while (jeu.joueursBot[numeroBot].main[k].getCouleur() == 3) // Si on est dans le cas d'une valeur rouge.
                {
                    // Si on a des cartes spéciales dans cette couleur alors on met une valeur plus élevé à jouer cette couleur.
                    if (jeu.joueursBot[numeroBot].main[k].getValeur() == 10)
                    {
                        nbCarteValeur + 2;
                    }
                    else if (jeu.joueursBot[numeroBot].main[k].getValeur() == 11 ||
                             jeu.joueursBot[numeroBot].main[k].getValeur() == 12)
                        nbCarteValeur + 5;
                    // On gère le cas des cartes spéciales +4 et joker qui ne représentent pas une valeur pour la couleur.
                    else if (jeu.joueursBot[numeroBot].main[k].getValeur() == 13 ||
                             jeu.joueursBot[numeroBot].main[k].getValeur() == 14)
                        k++;
                    else
                    {
                        nbCarteValeur++;
                        k++;
                    }
                }
                break;
            case 4:
                int k = jeu.joueursBot[numeroBot].main.size();
                while (jeu.joueursBot[numeroBot].main[k].getCouleur() == 4) // Si on est dans le cas d'une valeur rouge.
                {
                    // Si on a des cartes spéciales dans cette couleur alors on met une valeur plus élevé à jouer cette couleur.
                    if (jeu.joueursBot[numeroBot].main[k].getValeur() == 10)
                    {
                        nbCarteValeur + 2;
                    }
                    else if (jeu.joueursBot[numeroBot].main[k].getValeur() == 11 ||
                             jeu.joueursBot[numeroBot].main[k].getValeur() == 12)
                        nbCarteValeur + 5;
                    // On gère le cas des cartes spéciales +4 et joker qui ne représentent pas une valeur pour la couleur.
                    else if (jeu.joueursBot[numeroBot].main[k].getValeur() == 13 ||
                             jeu.joueursBot[numeroBot].main[k].getValeur() == 14)
                        k--;
                    else
                    {
                        nbCarteValeur++;
                        k--;
                    }
                }
                break;

            default:
                break;
            }
        }
    }
    // On ressort avec une variable qui est un "score" de façon à comparer les avantages entre jouer la couleur de la carte du talon ou changer de couleur.
    return nbCarteValeur;
}

void Bot::choixJeu(const Jeu &jeu)
{
    int nbCarteValeur, nbCarteCouleur;

    nbCarteCouleur = carteMemeCouleurTalon(jeu);
    nbCarteValeur = carteMemeValeurTalon(jeu);
    // On compare les deux en prenant en compte que c'est mieux de finir une couleur avant d'en changer
    

}
 
void Bot::testRegression()
{
    assert(main.size() >= 7);
    assert(numeroBot>=0);
    assert(!nom.empty());

    // Affiche les cartes dans la main du bot.
    for (auto & i : main) {
       cout << "La valeur est "<< i.getValeur() << " et la couleur " << i.getCouleur() << endl;
    }

}
