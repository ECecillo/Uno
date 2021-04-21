#include "VarianteDoublon.h"

VarianteDoublon::VarianteDoublon():Jeu() {}

VarianteDoublon::VarianteDoublon(const unsigned int nbjoueurs, const unsigned int nbIA = 0):Jeu(nbjoueurs, nbIA) {}

VarianteDoublon::~VarianteDoublon() 
{
    {
    delete[] joueurs;
    joueurs = NULL;
    sensJeu = 1;
    joueurActif = 0;
    nombreJoueurs = 0;
}
}

void VarianteDoublon::poserCarte(unsigned int &indiceCarte, string &messageErreur)
{
    if (carteValide(joueurs[joueurActif].main[indiceCarte]))
    {                                                       // La carte qu'il veut poser est valide
        talon.push(joueurs[joueurActif].main[indiceCarte]); // On pousse la carte que le joueur voulait jouer.
        joueurs[joueurActif].main.erase(joueurs[joueurActif].main.begin() + indiceCarte);
        // si le joueur a le doublon de la carte qu'il vient de jouer, on lui propose de la jouer
        int indice = indiceDoublon(talon.back(),joueurs[joueurActif].main);
        if (indice>=0)
        {
            char reponse;
            cout << "Veux-tu jouer la carte doublon? O/N" << endl;
            cin >> reponse;
            if (reponse == 'O') 
            {
                talon.push(joueurs[joueurActif].main[indice]); // On pousse le doublon.
                joueurs[joueurActif].main.erase(joueurs[joueurActif].main.begin() + indice);
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

// retourne l'indice dans m du doublon de la carte c, -1 sinon
int VarianteDoublon::indiceDoublon(Carte c,vector<Carte> m)
{
    for(int i=0; i<m.size(); i++)
    {
        if ((c.getValeur()==13 && m[i].getValeur()==13)||(c.getValeur()==14 && m[i].getValeur()==14)) //cas joker et +4, pas besoin de tester la couleur
        {
            return i;
        }
        else
            if(c.getValeur()==m[i].getValeur() && c.getCouleur()==m[i].getCouleur()) return i;
            
    }
    return -1;
}