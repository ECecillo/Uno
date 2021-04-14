#include "VarianteCumul.h"

VarianteCumul::VarianteCumul():Jeu(), cumulCarteAPiocher(0) {}

VarianteCumul::VarianteCumul(const unsigned int nbjoueurs, const unsigned int nbIA = 0):Jeu(nbjoueurs, nbIA) {}

VarianteCumul::~VarianteCumul() 
{
    {
    delete[] joueurs;
    joueurs = NULL;
    sensJeu = 1;
    joueurActif = 0;
    nombreJoueurs = 0;
}
}

bool VarianteCumul::carteValide(const Carte c) const // On compare la carte que l'on a passé en paramètre à celle qui est actuellement retourné sur le talon.
{
    bool chercheCouleur = false;
    //si carte +4, on regarde dans la main du joueur s'il y a une carte de la même couleur que celle du talon
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
           (c.getCouleur() == talon.back().getCouleur()) ||
           (c.getValeur() == 14) ||
           (c.getValeur() == 13 && talon.back().getValeur() != 13 && (talon.back().getValeur()==12 || chercheCouleur == false));    //on peut jouer +4 sur n'importe quelle carte si on n'a pas la couleur ou sur +2
}

void VarianteCumul::poserCarte(unsigned int &indiceCarte, string &messageErreur)
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
            if (joueurActif == nombreJoueurs) // Si On passe le tour du dernier joueur on revient au premier.
                joueurActif = 0;
            joueurActif++;
            break;
        case 12:
            cumulCarteAPiocher += 2;
            termineTour();
            break;
        case 13:
            cumulCarteAPiocher += 4;
            termineTour();
            break;
        case 14:
            break;
        }
        /*if (testUno() == false)
            termineTour();*/
    }
    else
    {

        messageErreur = "Cette carte ne peut pas être déposée.";
        // Voir si on ajoute d'autre message.
        cout << messageErreur << endl;
    }
}

void VarianteCumul::piocherCarte()
{
    if (cumulCarteAPiocher == 0) // cas classique, 1 carte à piocher
    {
        joueurs[joueurActif].main.push_back(pioche.top());
        pioche.pop();
    }
    else // cas après des +2 et +4
    {
        for (int i = 0; i<cumulCarteAPiocher; i++)
        {
            joueurs[joueurActif].main.push_back(pioche.top());
            pioche.pop();
        }
        cumulCarteAPiocher = 0;
    }
    joueurs[joueurActif].modifMainTxt();
    joueurs[joueurActif].modifTalonPiocheTxt(talon, pioche);
}