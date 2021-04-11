#include "VarianteEchange.h"

VarianteEchange::VarianteEchange():Jeu() {}

VarianteEchange::VarianteEchange(const unsigned int nbjoueurs, const unsigned int nbIA = 0):Jeu(nbjoueurs, nbIA) {}

VarianteEchange::~VarianteEchange() 
{
    {
    delete[] joueurs;
    joueurs = NULL;
    sensJeu = 1;
    joueurActif = 0;
    nombreJoueurs = 0;
}
}

void VarianteEchange::poserCarte(unsigned int &indiceCarte, string &messageErreur)
{
    if (carteValide(joueurs[joueurActif].main[indiceCarte]))
    {                                                       // La carte qu'il veut poser est valide
        talon.push(joueurs[joueurActif].main[indiceCarte]); // On pousse la carte que le joueur voulait jouer.
        joueurs[joueurActif].main.erase(joueurs[joueurActif].main.begin() + indiceCarte);
        // si la carte posée est un 7, le joueur échange sa main avec un autre joueur
        if (talon.back().getValeur()==7)
        {
            unsigned int numJoueur;
            cout << "indique le numéro du joueur avec lequel tu veux échanger ta main: ";
            cin >> numJoueur;
            joueurs[joueurActif].main.swap(joueurs[numJoueur-1].main);
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
