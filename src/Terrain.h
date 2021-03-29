#ifndef TERRAIN_H
#define TERRAIN_H

//! \brief une fen�tre texte est un tableau 2D de caract�res
class Terrain {

private:

/**
 	* @brief 
 	*  Tableau dynamique de chaine de caractères
	*  (Jouer,Reprendre,Quitter,Logs,Règles) 
 	*/
	vector<int> NomsMenu;

	/**
	 * @brief 
	 * Largeur de la fenêtre.
	 */
	int dimx;
	/**
	 * @brief 
	 * Hauteur de la fenêtre.
	 */
	int dimy;

	/**
	 * @brief 
	 * stocke le contenu de la fenêtre dans un tableau 1D,
	 * mais on y accede en 2D.
	 */
    char* ter;

public:

	/**
	 * @brief Construit un nouvellle Objet Terrain
	 * 
	 * @param dx 
	 * @param dy 
	 */
	Terrain (int dx, int dy);
	/**
	 * @brief Détruit l'objet terrain.
	 * 
	 */
	~Terrain();
	/**
	 * @brief 
	 * 
	 * @param c 
	 */
	void clear (char c=' ');
	/**
	 * @brief 
	 * 
	 * @param x 
	 * @param y 
	 * @param c 
	 */
	void print (int x, int y, char c);
	/**
	 * @brief 
	 * 
	 * @param x 
	 * @param y 
	 * @param c 
	 */
	void print (int x, int y, char* c);
	/**
	 * @brief 
	 * 
	 * @param x 
	 * @param y 
	 */
	void draw (int x=0, int y=0);
	/**
	 * @brief 
	 * 
	 */
	void pause();
	/**
	 * @brief Get the Ch object
	 * 
	 * @return char 
	 */
	char getCh();

};

void termClear ();

#endif

