#include "VarianteCumul.h"

VarianteCumul::VarianteCumul() : Jeu() {}

VarianteCumul::VarianteCumul(const unsigned int nbjoueurs, const unsigned int nbIA = 0) : Jeu(nbjoueurs, nbIA) 
{
    casPart = 0;
}

VarianteCumul::~VarianteCumul()
{
    delete[] joueurs;
    joueurs = NULL;
    sensJeu = 1;
    joueurActif = 0;
    nombreJoueurs = 0;
}
// On compare la carte que l'on a passé en paramètre à celle qui est actuellement retourné sur le talon.
bool VarianteCumul::carteValide(const Carte c) const
{
    bool chercheCouleur = false;
    if (c.getValeur() == 13)
    {
        unsigned int i = 0;
        while (i < joueurs[joueurActif].main.size() && !chercheCouleur)
        {
            if (talon.back().getCouleur() == joueurs[joueurActif].main[i].getCouleur())
                chercheCouleur = true;
            i++;
        }
    }
    return (c.getValeur() == talon.back().getValeur()) ||
           (c.getCouleur() == talon.back().getCouleur() && (talon.back().getValeur() != 12 || casPart % 2 != 0 || casPart <= 0)) || // bloquer le cas du +2 sur le talon
           (c.getValeur() == 12 && (talon.back().getValeur() != 13 && casPart % 2 != 0)) || // bloquer le +2 sur +4
           (c.getValeur() == 14 && talon.back().getValeur() != 13 && talon.back().getValeur() != 12) || // le joker ne peut pas se poser sur un +2 ou un +4
           (c.getValeur() == 13 && talon.back().getValeur() != 13 && (talon.back().getValeur() == 12 || chercheCouleur == false)); //on peut jouer +4 sur n'importe quelle carte si on n'a pas la couleur ou sur +2
}

void VarianteCumul::poserCarte(const unsigned int &indiceCarte, string &messageErreur)
{
    cout << "poser cumul" << endl;
    assert(indiceCarte >= 0);
    if (joueurActif >= nombreJoueurs)
    {
        int indexBot = joueurActif - nombreJoueurs;
        if (carteValide(joueursBot[indexBot].main[indiceCarte]))
        {

            talon.push(joueursBot[indexBot].main[indiceCarte]); // On pousse la carte que le joueur voulait jouer.
            joueursBot[indexBot].main.erase(joueursBot[indexBot].main.begin() + indiceCarte);
            cout << "La nouvelle carte du talon est : " << talon.back().getValeur() << " et sa couleur est : " << (talon.back()).getCouleur() << endl;
            bool carteSpeciale = false;
            if (testUno() == false) // Si on est pas dans le cas du Uno
            {
                int newIndice = -1;
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
                    if (joueurActif == nombreJoueurs + nombreIA - 1 && sensJeu == 1) // Si On passe le tour du dernier joueur on revient au premier.
                        joueurActif = 0;
                    else if (joueurActif == nombreJoueurs + nombreIA - 1 && sensJeu == 0)
                    {
                        joueurActif--;
                    }
                    else if (joueurActif == 0 && sensJeu == 0)
                        joueurActif = nombreIA + nombreJoueurs - 1;
                    joueurActif++;

                    termineTour();
                    break;
                case 12:
                    termineTour();

                    casPart += 2;
                    carteSpeciale = true;
                    termineTour();
                    break;
                case 13:
                    carteSpeciale = true;
                    joueursBot[indexBot].setCartePlus4(newIndice);
                    //termineTour();

                    casPart += 4;
                    termineTour();

                    break;
                case 14:
                    joueursBot[indexBot].setCarteJoker(newIndice);
                    termineTour();
                    carteSpeciale = true;

                    break;
                }
                termineTour();
                if (joueursBot[indexBot].main.size() == 0)
                    annonceGagnant();
                return;
            }
            termineTour(); 
        }
        else
        {
            messageErreur = "Cette carte ne peut pas être déposée.";
        }
    }
    else // joueur humain
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
                termineTour();
                break;
            case 12:
                casPart += 2;
                cout << casPart << "après +2" << endl;
                break;
            case 13:
                casPart += 4;
                cout << casPart << "après +4" << endl;
                break;
            case 14:
                break;
            }
            termineTour();
        }
        else
        {

            messageErreur = "Cette carte ne peut pas être déposée.";
            // Voir si on ajoute d'autre message.
            cout << messageErreur << endl;
        }
    }
}

void VarianteCumul::piocherCarte()
{
    cout << casPart << "dans piocherCarte" << endl;
    if (casPart == 0) // cas classique, 1 carte à piocher
    {
        if (joueurActif >= nombreJoueurs)
        {
            int indexBot = joueurActif - nombreJoueurs;
            unsigned int indCarte;
            joueursBot[indexBot].main.push_back(pioche.top());
            switch (pioche.top().getCouleur())
            {
            case 1:
                cout << "On pioche une carte Rouge" << endl;
                cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteRouge() << endl;
                joueursBot[indexBot].setCarteRouge();
                cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteRouge() << endl;

                if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
                {
                    indCarte = joueursBot[indexBot].getCarteRouge() - 1;
                    sleep(1);
                    joueursBot[indexBot].setCarteJoker(indCarte);
                }

                else if (pioche.top().getValeur() == 13)
                {
                    indCarte = joueursBot[indexBot].getCarteRouge() - 1;
                    sleep(1);
                    joueursBot[indexBot].setCartePlus4(indCarte);
                }
                break;
            case 2:
                cout << "On pioche une carte Vert" << endl;
                cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteVert() << endl;
                joueursBot[indexBot].setCarteVert();
                cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteVert() << endl;

                if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
                {
                    indCarte = joueursBot[indexBot].getCarteVert() - 1;
                    sleep(1);
                    joueursBot[indexBot].setCarteJoker(indCarte);
                }

                else if (pioche.top().getValeur() == 13)
                {
                    indCarte = joueursBot[indexBot].getCarteVert() - 1;
                    sleep(1);
                    joueursBot[indexBot].setCartePlus4(indCarte);
                }
                break;
            case 3:
                cout << "On pioche une carte Bleu" << endl;
                cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteBleu() << endl;
                joueursBot[indexBot].setCarteBleu();
                cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteBleu() << endl;

                if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
                {
                    indCarte = joueursBot[indexBot].getCarteBleu() - 1;
                    sleep(1);
                    joueursBot[indexBot].setCarteJoker(indCarte);
                }

                else if (pioche.top().getValeur() == 13)
                {
                    indCarte = joueursBot[indexBot].getCarteBleu() - 1;
                    sleep(1);
                    joueursBot[indexBot].setCartePlus4(indCarte);
                }
                break;
            case 4:
                cout << "On pioche une carte Jaune" << endl;
                cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteJaune() << endl;
                joueursBot[indexBot].setCarteJaune();
                cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteJaune() << endl;

                if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
                {
                    indCarte = joueursBot[indexBot].getCarteJaune() - 1;
                    sleep(1);
                    joueursBot[indexBot].setCarteJoker(indCarte);
                }

                else if (pioche.top().getValeur() == 13)
                {
                    indCarte = joueursBot[indexBot].getCarteJaune() - 1;
                    sleep(1);
                    joueursBot[indexBot].setCartePlus4(indCarte);
                }
                break;

            default:
                break;
            }
            pioche.pop();
            joueursBot[indexBot].trierMain();
            return;
        }
        else // Joueur normal.
        {
            joueurs[joueurActif].main.push_back(pioche.top());
            pioche.pop();
            joueurs[joueurActif].modifMainTxt();
            joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
        }
    }
    else // cas après des +2 et +4
    {
        if (joueurActif >= nombreJoueurs)
        {
            int indexBot = joueurActif - nombreJoueurs;
            unsigned int indCarte;
            joueursBot[indexBot].main.push_back(pioche.top());
            for (int i = 0; i < casPart; i++)
            {
                switch (pioche.top().getCouleur())
                {
                case 1:
                    cout << "On pioche une carte Rouge" << endl;
                    cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteRouge() << endl;
                    joueursBot[indexBot].setCarteRouge();
                    cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteRouge() << endl;

                    if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
                    {
                        indCarte = joueursBot[indexBot].getCarteRouge() - 1;
                        sleep(1);
                        joueursBot[indexBot].setCarteJoker(indCarte);
                    }

                    else if (pioche.top().getValeur() == 13)
                    {
                        indCarte = joueursBot[indexBot].getCarteRouge() - 1;
                        sleep(1);
                        joueursBot[indexBot].setCartePlus4(indCarte);
                    }
                    break;
                case 2:
                    cout << "On pioche une carte Vert" << endl;
                    cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteVert() << endl;
                    joueursBot[indexBot].setCarteVert();
                    cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteVert() << endl;

                    if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
                    {
                        indCarte = joueursBot[indexBot].getCarteVert() - 1;
                        sleep(1);
                        joueursBot[indexBot].setCarteJoker(indCarte);
                    }

                    else if (pioche.top().getValeur() == 13)
                    {
                        indCarte = joueursBot[indexBot].getCarteVert() - 1;
                        sleep(1);
                        joueursBot[indexBot].setCartePlus4(indCarte);
                    }
                    break;
                case 3:
                    cout << "On pioche une carte Bleu" << endl;
                    cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteBleu() << endl;
                    joueursBot[indexBot].setCarteBleu();
                    cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteBleu() << endl;

                    if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
                    {
                        indCarte = joueursBot[indexBot].getCarteBleu() - 1;
                        sleep(1);
                        joueursBot[indexBot].setCarteJoker(indCarte);
                    }

                    else if (pioche.top().getValeur() == 13)
                    {
                        indCarte = joueursBot[indexBot].getCarteBleu() - 1;
                        sleep(1);
                        joueursBot[indexBot].setCartePlus4(indCarte);
                    }
                    break;
                case 4:
                    cout << "On pioche une carte Jaune" << endl;
                    cout << "Nombre de carte avant ajout : " << joueursBot[indexBot].getCarteJaune() << endl;
                    joueursBot[indexBot].setCarteJaune();
                    cout << "Nombre de carte après ajout : " << joueursBot[indexBot].getCarteJaune() << endl;

                    if (pioche.top().getValeur() == 14) // On ajoute l'indice du joker.
                    {
                        indCarte = joueursBot[indexBot].getCarteJaune() - 1;
                        sleep(1);
                        joueursBot[indexBot].setCarteJoker(indCarte);
                    }

                    else if (pioche.top().getValeur() == 13)
                    {
                        indCarte = joueursBot[indexBot].getCarteJaune() - 1;
                        sleep(1);
                        joueursBot[indexBot].setCartePlus4(indCarte);
                    }
                    break;

                default:
                    break;
                }
                pioche.pop();
                joueursBot[indexBot].trierMain();
                return;
            }
            casPart = -1;
        }
        else
        {
            for (int i = 0; i < casPart; i++)
            {
                joueurs[joueurActif].main.push_back(pioche.top());
                pioche.pop();
            }
            if (casPart % 2 == 1) casPart = 0;
            joueurs[joueurActif].modifMainTxt();
            joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
        }
    }
}