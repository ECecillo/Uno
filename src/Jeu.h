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
	//Joueur *joueurs;

	/**
	 * @brief 
	 * Nombre de Joueurs dans la partie.
	 */
	unsigned int nombreJoueurs;

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
	int joueurActif;

	// ==============================================================================================================

	Jeu();

	Jeu(int, int, char[]);

	~Jeu();

	void initJeu();

	/**
	 * @brief 
	 * Initialise la pioche.
	 */
	void initCarte();

	/**
	 * @brief 
	 * 
	 * @param v 
	 * @return true : on a encore 
	 * @return false 
	 */
	bool chercheValeurZero(vector<vector<int>> v);

	void afficherJoueurActif();

	bool carteValide(Carte);

	void piocheVide();

	void boucleJeu();

	bool testUno();

	void joueUno();

	void contreUno();

	void piocherCarte();

	void poserCarte();

	/**
	 * @brief 
	 * Affiche un message et fais en sorte que numéro joueur puisse joueur.
	 * @return int 
	 */
	int quiJoue();

	void testRegression();
};

#endif
