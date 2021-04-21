#include <Bot.h>

using namespace std;

Bot::Bot()
{
    numeroBot = 0;
    nbCarteRouge = 0;
    nbCarteVert = 0;
    nbCarteBleu = 0;
    nbCarteJaune = 0;
    indCarteJoker = -1;
    indCartePlus4 = -1;
    nom = "";
}

Bot::Bot(const unsigned int num)
{
    numeroBot = num;
    nbCarteRouge = 0;
    nbCarteVert = 0;
    nbCarteBleu = 0;
    nbCarteJaune = 0;
    indCarteJoker = -1;
    indCartePlus4 = -1;
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

int Bot::carteMemeValeurTalon(const Jeu &jeu, int &c, int &indiceCarte)
{
    cout << "Fonction Carte meme valeur talon " << endl;
    // variable contenant le nombre de Carte avec la couleur de la carte qui a la meme valeur que celle du talon.
    int nbCarteMemeCouleur = 0;
    // On créer une nouvelle variable pour mettre le bonne indice du bot que l'on regarde.
    int indexBot = numeroBot - 1;
    cout << " CARTE MEME VALEUR ########### L'actuel carte du talon est : " << jeu.talon.back().getValeur() << " et sa couleur est : " << (jeu.talon.back()).getCouleur() << endl;
    for (int i = 0; i < jeu.joueursBot[indexBot].main.size(); i++)
    {
        if (jeu.joueursBot[indexBot].main[i].getValeur() == jeu.talon.back().getValeur() && jeu.joueursBot[indexBot].main[i].getCouleur() != jeu.talon.back().getCouleur())
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
                    c = 1;
                    indiceCarte = i;
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
                    c = 2;
                    indiceCarte = i;
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
                    c = 3;
                    indiceCarte = i;
                }

                break;
            case 4:
                k = jeu.joueursBot[indexBot].main.size() - 1;
                while (jeu.joueursBot[indexBot].main[k].getCouleur() == 4) // Si on est dans le cas d'une valeur Jaune.
                {
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
                    c = 4;
                    indiceCarte = i;
                }

                break;

            default:
                break;
            }
        }
        else if (jeu.joueursBot[indexBot].main[i].getValeur() == 13 && indCartePlus4 == -1)
        { // On regarde si la carte est un +4 et si on a pas déjà un autre +4 dans la main.
            // edit : Si on a déjà un joker on en prend pas un autre.
            indCartePlus4 = i;
        }
        else if (jeu.joueursBot[indexBot].main[i].getValeur() == 14 && indCarteJoker == -1)
        { // On regarde si la carte est un Joker et si on a pas déjà un autre joker dans la main.
            // edit : Si on a déjà un joker on en prend pas un autre.
            indCarteJoker = i;
        }
    }
    return nbCarteMemeCouleur;
}

void Bot::joueCouleurSelonEntier(Jeu &jeu, int couleur, int carteSpeciale)
{
    string er;
    srand(time(NULL));
    if (carteSpeciale >= 0) // On change la couleur du joker ou du plus 4 avec la meilleur couleur a jouer.
        main[carteSpeciale].setCouleur(couleur);
    int max; // Variable temporaire servant à définir dans quelle intervalle on génère val aléa.
    int min;
    switch (couleur) // On joue en fonction de la carte du talon.
    {
    case 1:
        cout << "JoueCouleurSelonEntier Rouge" << endl;
        if (carteSpeciale >= 0)
        {
            nbCarteRouge--;
            jeu.poserCarte(carteSpeciale, er);
        }
        else
        {
            cout << "Le nombre de carte Rouge est " << nbCarteRouge << endl;
            if (nbCarteRouge == 1)
            {
                nbCarteRouge--;
                jeu.poserCarte(0, er);
            }
            else
            {
                max = rand() % nbCarteRouge;
                nbCarteRouge--;
                jeu.poserCarte(max, er);
            }
        }
        break;
    case 2:
        cout << "JoueCouleurSelonEntier Vert" << endl;
        //cout << "Nombre carte Rouge " << nbCarteVert << endl;

        if (carteSpeciale >= 0)
        {
            nbCarteVert--;
            jeu.poserCarte(carteSpeciale, er);
        }

        else
        {
            max = nbCarteRouge + nbCarteVert - 1;
            min = nbCarteRouge;
            cout << "Le nombre de carte Rouge est " << nbCarteRouge << endl;
            cout << "Le nombre de carte Verte est " << nbCarteVert << endl;
            cout << "Le max est " << max << endl;
            cout << "Le min est " << min << endl;
            nbCarteVert--;
            if (min == max)
                jeu.poserCarte(max, er);
            else
                jeu.poserCarte((rand() % (max - min) + min), er);
        }
        break;
    case 3:
        cout << "JoueCouleurSelonEntier Bleu" << endl;
        //cout <<  "Nombre carte Rouge " << nbCarteBleu << endl;

        if (carteSpeciale >= 0)
        {
            nbCarteBleu--;
            jeu.poserCarte(carteSpeciale, er);
        }
        else
        {
            max = nbCarteRouge + nbCarteVert + nbCarteBleu - 1;
            min = nbCarteRouge + nbCarteVert;
            cout << "Le nombre de carte Rouge est " << nbCarteRouge << endl;
            cout << "Le nombre de carte Verte est " << nbCarteVert << endl;
            cout << "Le nombre de carte Bleu est " << nbCarteBleu << endl;
            cout << "Le max est " << max << endl;
            cout << "Le min est " << min << endl;
            nbCarteBleu--;
            if (min == max)
                jeu.poserCarte(max, er);
            else if (nbCarteRouge == 0 && nbCarteVert == 0)
                jeu.poserCarte(max, er);
            else
                jeu.poserCarte((rand() % (max - min) + min), er);
        }
        break;
    case 4:

        cout << "JoueCouleurSelonEntier Jaune" << endl;
        //cout <<  "Nombre carte Rouge " << nbCarteJaune << endl;
        if (carteSpeciale >= 0)
        {
            nbCarteJaune--;
            jeu.poserCarte(carteSpeciale, er);
        }
        else
        {
            max = nbCarteRouge + nbCarteVert + nbCarteBleu + nbCarteJaune - 1;
            min = nbCarteRouge + nbCarteVert + nbCarteBleu;
            cout << "Le nombre de carte Rouge est " << nbCarteRouge << endl;
            cout << "Le nombre de carte Verte est " << nbCarteVert << endl;
            cout << "Le nombre de carte Bleu est " << nbCarteBleu << endl;
            cout << "Le nombre de carte Jaune est " << nbCarteJaune << endl;
            cout << "Le max est " << max << endl;
            cout << "Le min est " << min << endl;
            nbCarteJaune--;
            if (min == max)
                jeu.poserCarte(max, er);
            else
                jeu.poserCarte((rand() % (max - min) + min), er);
        }
        break;

    default:
        break;
    }
}
void Bot::changeIndiceCarteSpeciale()
{
    // Comme les indices des jokers sont absolues car on les récups lors de la distrib des cartes,
    // On doit les modifier pour ne pas avoir un segfault.
    if (indCarteJoker > 0 && indCartePlus4 == -1)
        indCarteJoker--;
    else if (indCarteJoker == -1 && indCartePlus4 > 0)
        indCartePlus4--;
    else if (indCarteJoker > 0 && indCartePlus4 > 0)
    {
        indCartePlus4--;
        indCarteJoker--;
    }
}

void Bot::choixJeu(Jeu &jeu)
{
    cout << "La taille de la main du Bot est : " << main.size() << endl;
    // Affiche les cartes dans la main du bot.
    for (auto &i : main)
    {
        cout << "La valeur est " << i.getValeur() << " et la couleur " << i.getCouleur() << endl;
    }
    sleep(5);
    int nbCarteCouleur, nbCarteValeur;
    int maxNbCouleur = couleurAvecPlusDeCarte(); // Renvoie la couleur avec le plus de carte.
    nbCarteCouleur = carteMemeCouleurTalon(jeu); // Nombre de carte meme couleur que talon.
    int couleur;                                 // Entier qui permettra de savoir quelle couleur a le plus de carte.
    int indiceCarteMemeValeur;
    nbCarteValeur = carteMemeValeurTalon(jeu, couleur, indiceCarteMemeValeur);
    cout << "L'indice de la carte du joker est : " << indCarteJoker << endl;
    cout << "L'indice de la carte du Plus 4 est : " << indCartePlus4 << endl;
    assert(nbCarteCouleur >= 0 || nbCarteValeur >= 0);
    // On compare les deux en prenant en compte que c'est mieux de finir une couleur avant d'en changer
    if (!(nbCarteCouleur == 0) && nbCarteValeur == 0) // Cas où on peut jouer que les cartes mêmes couleurs.
    {
        cout << "Joue carte meme couleur" << endl;
        changeIndiceCarteSpeciale();
        joueCouleurSelonEntier(jeu, jeu.talon.back().getCouleur(), -1);
    }
    else if (!(nbCarteCouleur == 0) && !(nbCarteValeur == 0)) // Cas où on peut jouer Carte Couleur.
    {
        if (nbCarteCouleur > nbCarteValeur || nbCarteCouleur == 1) // Cas où c'est mieux de jouer meme couleur.
        {
            cout << "Joue meme couleur quand on peut jouer meme valeur" << endl;
            changeIndiceCarteSpeciale();
            joueCouleurSelonEntier(jeu, jeu.talon.back().getCouleur(), -1);
        }
        else if (nbCarteCouleur == 2 && nbCarteValeur == 1)
        { // Si on a que 1 de différence mais la possibilité de finir la couleur on fini.
            changeIndiceCarteSpeciale();
            joueCouleurSelonEntier(jeu, jeu.talon.back().getCouleur(), -1);
        }
        else if (nbCarteValeur == 2 && nbCarteCouleur == 1)
        { // Si on a que 1 de différence mais la possibilité de finir la couleur on fini.
            changeIndiceCarteSpeciale();
            joueCouleurSelonEntier(jeu, couleur, -1);
        }
        else // On a plus de carte de la couleur de celle qui a la meme valeur que talon.
        {
            cout << "Joue meme valeur quand on a quand meme couleur" << endl;
            changeIndiceCarteSpeciale();
            joueCouleurSelonEntier(jeu, couleur, indiceCarteMemeValeur);
        }
    }
    else if (!(nbCarteValeur == 0) && nbCarteCouleur == 0) // On ne peut jouer que la carte avec la même valeur.
    {
        // On joue la carte qui a la même valeur.
        cout << "On joue la carte qui a la meme valeur car on peut pas jouer autre chose" << endl;
        changeIndiceCarteSpeciale();
        joueCouleurSelonEntier(jeu, couleur, indiceCarteMemeValeur);
    }
    else if ((nbCarteValeur == 0 && nbCarteCouleur == 0) && indCarteJoker >= 0) // Change couleur.
    {                                                                           // Si on peut pas jouer de carte meme valeur ou couleur talon.
        // changerCouleurCarte(jeu, maxCouleur);
        cout << "Indice du joker est : " << indCarteJoker << endl;
        cout << "La valeur du Joker doit être " << main[indCarteJoker].getValeur() << " la couleur est " << main[indCartePlus4].getCouleur() << endl;
        cout << "On joue le joker car pas d'autres options avec indice : " << indCarteJoker << endl;
        joueCouleurSelonEntier(jeu, maxNbCouleur, indCarteJoker);
        indCarteJoker = -1;
        cout << "suppression Joker" << endl;
    }
    else if ((nbCarteValeur == 0 && nbCarteCouleur == 0) && indCartePlus4 >= 0) // +4
    {
        cout << "Indice du +4 est : " << indCartePlus4 << endl;
        cout << "La valeur du +4 doit être " << main[indCartePlus4].getValeur() << " la couleur est " << main[indCartePlus4].getCouleur() << endl;
        cout << "On joue le joker +4 car pas d'autres options avec indice : " << indCartePlus4 << endl;
        sleep(3);
        joueCouleurSelonEntier(jeu, maxNbCouleur, indCartePlus4);
        indCartePlus4 = -1;
        cout << "suppression +4" << endl;
    }
    else
    {
        cout << "On pioche" << endl;
        // On pioche car on ne peut pas jouer.
        jeu.piocherCarte();
        jeu.termineTour();
    }
}

int Bot::couleurAvecPlusDeCarte() const
{
int tabNbCarte[] = {nbCarteRouge, nbCarteVert, nbCarteBleu, nbCarteJaune};
    int max = tabNbCarte[0];
    int couleur = 0;
    int length = sizeof tabNbCarte / sizeof tabNbCarte[0];
    cout << "Contenu tableau Nombre de carte" << endl;
    for (int i = 0; i < length; i++)
        cout << tabNbCarte[i] << endl; 
    for (int n = 1; n < length; n++)
    {
        if (tabNbCarte[n] > max)
        {
            max = tabNbCarte[n]; // On met à jour le max pour continuer à comparer.
            couleur = n + 1;     // On stock quelle couleur est la plus grande.
        }
    }

    return couleur;
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
void Bot::setCarteJoker(int indiceJoker)
{
    indCarteJoker = indiceJoker;
}
void Bot::setCartePlus4(int indicePlus4)
{
    indCartePlus4 = indicePlus4;
}
int Bot::getCarteRouge()
{
    return nbCarteRouge;
}
int Bot::getCarteVert()
{
    return nbCarteVert;
}
int Bot::getCarteBleu()
{
    return nbCarteBleu;
}
int Bot::getCarteJaune()
{
    return nbCarteJaune;
}
int Bot::getCarteJoker()
{
    //assert(indiceJoker >= 0);
    return indCarteJoker;
}
int Bot::getCartePlus4()
{
    //assert(indicePlus4 >= 0);
    return indCartePlus4;
}

void Bot::testRegression(Jeu &jeu)
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
    int c;
    int indv;
    int nbValeur = carteMemeValeurTalon(jeu, c, indv);

    cout << "Le nombre de carte qui ont la couleur du talon : " << CouleurTalon << endl;
    cout << "Le nombre de carte qui ont la valeur du talon : " << nbValeur << endl;
    switch (c)
    {
    case 1:
        cout << "La couleur est Rouge" << endl;
        break;
    case 2:
        cout << "La couleur est Vert" << endl;
        break;
    case 3:
        cout << "La couleur est Bleu" << endl;
        break;
    case 4:
        cout << "La couleur est Jaune" << endl;
        break;
    }
    // Test sur le choixJeu du bot.
    choixJeu(jeu);
}
