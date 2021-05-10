#include "VarianteSuite.h"

VarianteSuite::VarianteSuite() : Jeu() {}

VarianteSuite::VarianteSuite(const unsigned int nbjoueurs, const unsigned int nbIA = 0) : Jeu(nbjoueurs, nbIA) {}

VarianteSuite::~VarianteSuite()
{
    delete[] joueurs;
    joueurs = NULL;
    sensJeu = 1;
    joueurActif = 0;
    nombreJoueurs = 0;
}

void VarianteSuite::poserCarte(const unsigned int &indiceCarte, string &messageErreur)
{
    if (joueurActif >= nombreJoueurs)
    {
        int indexBot = joueurActif - nombreJoueurs;
        if (carteValide(joueursBot[indexBot].main[indiceCarte]))
        {
            //cout << "La carte du bot est valide" << endl;          // La carte qu'il veut poser est valide
            talon.push(joueursBot[indexBot].main[indiceCarte]); // On pousse la carte que le joueur voulait jouer.
            testSuite(indiceCarte, joueursBot[indexBot].main);
            joueursBot[indexBot].main.erase(joueursBot[indexBot].main.begin() + indiceCarte);
            cout << "La nouvelle carte du talon est : " << talon.back().getValeur() << " et sa couleur est : " << (talon.back()).getCouleur() << endl;
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

                    piocherCarte();
                    piocherCarte();
                    termineTour();
                    break;
                case 13:
                    joueursBot[indexBot].setCartePlus4(newIndice);
                    termineTour();

                    for (unsigned int i = 0; i < 4; i++)
                        piocherCarte();
                    termineTour();

                    break;
                case 14:
                    joueursBot[indexBot].setCarteJoker(newIndice);
                    termineTour();

                    break;
                }
                termineTour();
                if (joueursBot[indexBot].main.size() == 0)
                    annonceGagnant();
                return;
            }
            /* if (carteSpeciale && testUno() != false)
                termineTour(); */
        }
        else
        {
            messageErreur = "Cette carte ne peut pas être déposée.";
        }
    }
    else
    {
        if (carteValide(joueurs[joueurActif].main[indiceCarte]))
        { // La carte qu'il veut poser est valide
            //char reponse = 'O';
            talon.push(joueurs[joueurActif].main[indiceCarte]); // On pousse la carte que le joueur voulait jouer.
            // la carte fait partie d'une suite de 0 à 7, et qu'elle est plus petite que 7, au delà le joueur ne pourra pas jouer plusieurs cartes
            if (testSuite(indiceCarte, joueurs[joueurActif].main) && talon.back().getValeur() < 7) casPart = 5; 
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
                termineTour();

                piocherCarte();
                piocherCarte();
                break;
            case 13:
                termineTour();

                for (unsigned int i = 0; i < 4; i++)
                    piocherCarte();
                termineTour();
                break;
            case 14:
                break;
            }
            if (casPart != 5) termineTour();
        }
        else
        {

            messageErreur = "Cette carte ne peut pas être déposée.";
            // Voir si on ajoute d'autre message.
            cout << messageErreur << endl;
        }
    }
}

// retourne vrai si dans main, la carte d'indice ind fait partie de la suite de 0 à 7 de la même couleur
bool VarianteSuite::testSuite(unsigned int ind, vector<Carte> main)
{
    bool test = (main[ind].getValeur() <= 7);
    unsigned int carteSerie = 0; //1ère valeur de la série à tester
    unsigned int i = 0;
    while (i < main.size() && test && carteSerie <= 7)
    {
        if (main[i].getCouleur() > main[ind].getCouleur())
            test = false;
        else if (main[i].getCouleur() == main[ind].getCouleur() && main[i].getValeur() == carteSerie)
            carteSerie++;
        i++;
    }
    if (carteSerie <= 7) test = false;
    return test;
/*
    char reponse = 'O';
    int k = 0;
    // Si on a bien 2 chiffres au moins à la suite
    //cout <<main[ind].getValeur() << endl;
    bool test = (main[ind].getValeur() <= 7) && (main[ind].getValeur() + 1 == main[ind + 1].getValeur());
    if (test) // Si la valeur est bien entre 0 et 7.
    {
        for (int i = ind; i < main.size() - 1; i++)
        { // On parcours la main du joueur.
            if (main[ind].getCouleur() == main[i].getCouleur())
            { // On est dans le cas où on va parcourir les cartes de même couleur à partir de l'indice choisis.
                if (main[ind].getValeur() == main[i].getValeur())
                {
                    k = i; // Je récupère l'indice à partir duquel je vais regarder la suite.
                    while ((main[k + 1].getValeur() == main[k].getValeur() + 1) && (main[k + 1].getValeur() <= 7) && reponse == 'O')
                    { // La valeur à i + 1 suit bien l'ordre croissant.
                        if (joueurActif >= nombreJoueurs)
                        { // Si on est un bot
                            reponse = 'O';
                        }
                        else if (joueurActif < nombreJoueurs)
                        { // On est un joueur donc on doit saisir notre choix.
                            cout << "Veux-tu jouer la carte suivante? saisir O/N puis entrer";
                            cin >> reponse;
                            cout << endl;
                        }
                        if (toupper(reponse) == 'O')
                        {
                            cout << "On met la carte " << main[k + 1].getValeur() << endl;
                            // On pose la carte suivante.
                            talon.push(main[k + 1]);
                            main.erase(main.begin() + (k + 1));
                            k++; // On incrémente le k pour continuer à regarder si on a une série.
                        }
                    }
                    return;
                }
            }
        }
    }*/
}