#ifndef JEU_H
#define JEU_H

#include <stack> // Pour les piles.
#include <queue>


#include <Joueur.h>
#include <Terrain.h>
#include <Carte.h>


using namespace std;

class Jeu
{
private:
public:
	/**
	 * @brief 
	 * Pile du jeux de cartes.
	 */
	stack<Carte> pioche;

	/**
	 * @brief 
	 * Talon du jeux de cartes sur lequels les joueurs joueront leurs cartes.
	 * A la fin si on a épuisé la Pioche, on empile ce qu'il y a dans le tas dans la pioche.  
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
	 * Nombre d'ordinateur dans la partie.
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
	 * Entier [1....4] :
	 * 1.Rouge.
	 * 2.Vert.
	 * 3.Bleu. 
	 * 4.Jaune.
	 */
	//unsigned int couleurActive; On va plutot utiliser les infos que l'on range dans le talon, on accèdera aux infos grâce à back() pour la file.

	/**
	 * @brief 
	 * Numéro du joueur qui est en train de jouer.
	 */
	unsigned int joueurActif;

	/**
	 * @brief 
	 * Booleen qui permettra de dire que la Partie est terminé.
	 */
	bool finPartie;

	/**
	 * @brief 
	 * Bool qui permet de signifier que c'est la fin du tour.
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

	Jeu();

	/**
	 * @brief Construit un objet Jeu avec un tableau de joueurs en paramètre 
	 * (où on peut récupérer le nombre de joueurs ...)
	 * 
	 * @param joueur 
	 * @param variante 
	 */
	Jeu(const unsigned int nbjoueurs, const unsigned int nbIA); // const vector<Joueur> &joueur, const vector<bool> variante,

	~Jeu();

	void initJeu();

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
	 * 
	 */
	void afficherJoueurActif();

	/**
	 * @brief Vérifie si la carte qui va être posé est valide ou non.
	 * 
	 * @return true 
	 * @return false 
	 */
	bool carteValide(const Carte) const;

	/**
	 * @brief 
	 * Vérifie que la pioche est vide.
	 * @return true : La pioche est vide.
	 * @return false : la pioche n'est pas vide.
	 */
	bool piocheVide(); 

	/**
	 * @brief 
	 * Si la pioche est vide et que le jeu n'est pas terminé on réinit la pioche avec Talon.
	 */
	void relancePiocheJeu();

	void boucleJeu();

	/**
	 * @brief 
	 * 
	 * @return true : Il ne reste que 1 seul carte au joueur, on peut afficher un message pour faire un uno. 
	 * @return false : Le joueur ne peut pas encore "dire" UNO. 
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
	 */
	void contreUno();

	/**
	 * @brief 
	 * On met dans la main du joueur une carte provenant de la pioche.
	 */
	void piocherCarte();

	/**
	 * @brief 
	 * Le joueur met une carte dans le Talon et on enlève la carte posé de la main du joueur.
	 * @param indiceCarte : l'indice de la carte que le joueur veut poser.
	 * @param messageErreur : Message d'erreur à afficher avec l'interface Graphique.
	 */
	void poserCarte(unsigned int& indiceCarte, string &messageErreur);

	/**
	 * @brief 
	 * Affiche un message et fais en sorte que numéro joueur puisse joueur.
	 * @return int 
	 */
	int quiJoue();

	void termineTour();

	void actionJoueur(const char action, const int x, const int y);

	/**
	* @brief 
	* Actualise les adversaires dans tableJoueur pour la version txt.
	* 
	* @param
	*/
	void modifAdversairesTxt();
		

	/**
	 * @brief 
	 * Actualise tableJoueur au début de son tour. 
	 */	
	void MaJTableJoueurActifDebutTour();


	void testRegression();
};

#endif
