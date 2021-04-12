#ifndef JEU_H
#define JEU_H

#include <stack> // Pour les piles.
#include <queue>


#include <Joueur.h>
#include <Carte.h>


using namespace std;

/**
* @brief 
* Classe gérant les actions du jeu.
*/
class Jeu
{
private:
public:
	/**
	* @brief 
	* Pile du jeu de cartes.
	*/
	stack<Carte> pioche;

	/**
	* @brief 
	* Talon du jeu de cartes sur lequel les joueurs joueront leurs cartes.
	*   
	*/
	queue<Carte> talon;

	/**
	* @brief 
	* Tableau de Joueurs.
	*/
	Joueur *joueurs;

	/**
	* @brief 
	* Nombre de Joueurs dans la partie.
	*/
	unsigned int nombreJoueurs;

	/**
	* @brief 
	* Nombre d'ordinateurs dans la partie.
	*/
	unsigned int nombreIA;

	/**
	* @brief 
	* 0 ou 1 :
	* 0 on joue vers la droite.
	* 1 on joue vers la gauche.
	*/
	unsigned int sensJeu;

	/**
	* @brief 
	* Numéro du joueur qui est en train de jouer.
	*/
	unsigned int joueurActif;

	/**
	* @brief 
	* Booléen qui permettra de dire que la partie est terminée.
	*/
	bool finPartie;

	/**
	* @brief 
	* Booléen qui permet de signifier que c'est la fin du tour.
	* true : tour terminé on déclenche termineTour().
	* false : tour pas terminé.
	*/
	bool finTour;

	/**
	* @brief 
	* Variable qui permettra faire une boucle tant que
	* qui attendra une touche d'un des joueurs C ou U pour pouvoir relancer le Jeu.
	*/
	bool statut_Uno;

	// =========================================== FONCTION / Procédure ===============================================================

	/**
	* @brief 
	* Constructeur par défaut.
	* 
	*/
	Jeu();

	/**
	* @brief 
	* Destructeur par défaut.
	* 
	*/
	~Jeu();

	/**
	* @brief Construit un objet Jeu avec un tableau de joueurs en paramètre 
	* Constructeur: initialise le nombre de joueurs, le tableau de joueurs, distribue les cartes, complète la table de chaque joueur
	* 
	* @param 
	* [in] entier naturel: nombre de joueurs 
	* @param  
	* [in] entier naturel: nombre de joueurs 
	*
	*/
	Jeu(const unsigned int nbjoueurs, const unsigned int nbIA); 

	/**
	* @brief 
	* Initialise la pioche.
	*/
	void initCarte();

	/**
	* @brief 
	* Initialise le Talon à partir de la pioche.
	*/
	void initTalon();

	/**
	* @brief 
	* Distribue les 7 cartes nécessaires à chaque joueur au début du jeu.
	*/
	void distribueCarte();

	/**
	* @brief 
	* Vérifie si la carte qui va être posée est valide ou non.
	* 
	* @param 
	* [in] carte à comparer avec le talon
	*
	* @return
	* true si la carte est valide.
	*/
	bool carteValide(const Carte) const;

	/**
	* @brief 
	* On met dans la main du joueur une carte provenant de la pioche.
	*/
	void piocherCarte();

	/**
	* @brief 
	* Le joueur met une carte dans le Talon et on enlève la carte de la main du joueur.
	*
	* @param 
	* [in] entier naturel: indice, dans la main, de la carte que le joueur veut poser.
	* @param 
	* messageErreur : Message d'erreur à afficher avec l'interface Graphique.
	*/
	void poserCarte(const unsigned int& indiceCarte, string &messageErreur);

	/**
	* @brief 
	* Actions clavier du joueur.
	*
	* @param 
	* [in] caractère: touche qui a été appuyée.
	* @param 
	* [in].
	* @param 
	* [in].
	*
	*/
	void actionJoueur(const char action, const int x, const int y);

	/**
	* @brief 
	* Vérifie que la pioche est vide.
	*
	* @return 
	* Booléen: true si la pioche est vide, false sinon.
	*
	*/
	bool piocheVide(); 

	/**
	* @brief 
	* Si la pioche est vide, réinitialisation avec le talon.
	*	
	*/
	void relancePiocheJeu();

	/**
	* @brief 
	* Teste si le joueur actif est en situation de dire Uno
	*
	* @return 
	* Booléen: true s'il reste 1 seule carte au joueur, false sinon. 
	* 
	*/
	bool testUno();

	/**
	* @brief 
	* Le joueur a appuyé avant sur ESPACE ce qui provoque le UNO,
	* on termine le tour et les autres joueurs ne peuvent plus dire contre Uno.
	*/
	void Uno(int c);

	/**
	* @brief 
	* Les joueurs ont cliqués sur CONTRE UNO avant le joueur concerné,
	* le joueur qui n'a pas appuyé doit donc piocher 2 cartes et le tour se termine.
	*
	*/
	void contreUno();
	
	/**
	* @brief 
	* Change le joueur actif, et termine le tour.
	* 
	*/
	void termineTour();

	

	/**
	* @brief 
	* Actualise le nombre de cartes des  adversaires dans la table des joueurs pour la version txt.
	* 
	*/
	void modifAdversairesTxt();
		

	/**
	* @brief 
	* Actualise la table du joueur actif au début de son tour. 
	*
	*/	
	void MaJTableJoueurActifDebutTour();

	/**
	* @brief 
	* Teste les fonctions et procédures du module. 
	*
	*/
	void testRegression();
};

#endif
