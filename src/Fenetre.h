#ifndef FENETRE_H
#define FENETRE_H

//! \brief une fen�tre texte est un tableau 2D de caract�res
class Fenetre
{

private:
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
	char *ter;

public:
	/**
	 * @brief Construit un nouvellle Objet Terrain
	 * 
	 * @param dx 
	 * @param dy 
	 */
	Fenetre(int dx, int dy);

	/**
	 * @brief 
	 * 
	 * @param c 
	 */
	void clear(char c = ' ');

	/**
	 * @brief 
	 * 
	 * @param x 
	 * @param y 
	 * @param c 
	 */
	void prepaFenetre(int x, int y, char c);
	
	/**
	 * @brief 
	 * 
	 * @param x 
	 * @param y 
	 */
	void dessine(int x = 0, int y = 0);

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

void termClear();

#endif
