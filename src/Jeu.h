#ifndef _JEU_

class Jeu
{
private:
	unsigned int nombreJoueurs;

public:
	pioche;		   //pile de Cartes
	talon;		   //pile de Cartes
	joueurs;	   //tableau de Joueurs
	sensJeu;	   //entier
	couleurActive; //chaine de caractères
	joueurActif;   //lien sur Joueur

	Jeu();

	Jeu(int, int, char[]);

	~Jeu();

	void initJeu();

	void afficherJoueurActif();

	bool carteValide(Carte);

	void piocheVide();

	void boucleJeu();

	bool testUno();

	void joueUno();

	void contreUno();

	void piocherCarte();

	void poserCarte();

	void testRegression();
};

#endif
