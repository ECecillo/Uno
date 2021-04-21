#include "VarianteSuite.h"

VarianteSuite::VarianteSuite():Jeu() {}

VarianteSuite::VarianteSuite(const unsigned int nbjoueurs, const unsigned int nbIA = 0):Jeu(nbjoueurs, nbIA) {}

VarianteSuite::~VarianteSuite() 
{
    {
    delete[] joueurs;
    joueurs = NULL;
    sensJeu = 1;
    joueurActif = 0;
    nombreJoueurs = 0;
}
}

void VarianteSuite::poserCarte(unsigned int &indiceCarte, string &messageErreur)
{
    if (carteValide(joueurs[joueurActif].main[indiceCarte]))
    {                                                       // La carte qu'il veut poser est valide
        bool suite = testSuite(indiceCarte,joueurs[joueurActif].main); //est-ce que la carte que le joueur joue fait partie d'une suite?
        char reponse='O';
        talon.push(joueurs[joueurActif].main[indiceCarte]); // On pousse la carte que le joueur voulait jouer.
        joueurs[joueurActif].main.erase(joueurs[joueurActif].main.begin() + indiceCarte);
        // le joueur va choisir de jouer ou pas la carte suivante de la suite
        if (suite)
        {
            while (reponse=='O' && joueurs[joueurActif].main[indiceCarte].getValeur()<7) //
            {
                cout << "Veux-tu jouer la carte suivante? O/N ";
                cin >> reponse;
                cout << endl;
                if (reponse=='O')
                {
                    talon.push(joueurs[joueurActif].main[indiceCarte]); // l'indice reste le même comme les cartes ont été décalées dans la main triée
                    joueurs[joueurActif].main.erase(joueurs[joueurActif].main.begin() + indiceCarte);
                }
            }
        }

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
            if (joueurActif == nombreJoueurs) // Si On passe le tour du dernier joueur on revient au premier.
                joueurActif = 0;
            joueurActif++;
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
            break;
        case 14:
            break;
        }
        if (testUno() == false)
            termineTour();

    }
    else
    {

        messageErreur = "Cette carte ne peut pas être déposée.";
        // Voir si on ajoute d'autre message.
        cout << messageErreur << endl;
    }
}

// retourne vrai si dans m, la carte d'indice ind fait partie de la suite de 0 à 7 de la même couleur
bool VarianteSuite::testSuite(unsigned int ind,vector<Carte> m)
{
    bool test=(m[ind].getValeur()<=7);
    unsigned int carteSerie = 0; //1ère valeur de la série à tester
    int i = 0;
    while (i<m.size() && test && carteSerie<=7)
    {
        if (m[i].getCouleur()>m[ind].getCouleur()) test=false;
        else
            if (m[i].getCouleur()==m[ind].getCouleur() && m[i].getValeur()==carteSerie) carteSerie++;
        return test;
    }
}