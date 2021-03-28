#ifndef TERRAIN_H
#define TERRAIN_H

//! \brief une fenêtre texte est un tableau 2D de caractères
class Terrain {

private:

    int dimx;       //!< \brief largeur
    int dimy;       //!< \brief heuteur
    char* ter;      //!< \brief stocke le contenu de la fenêtre dans un tableau 1D mais on y accede en 2D

public:

	Terrain (int dx, int dy);
	~Terrain();
	void clear (char c=' ');
	void print (int x, int y, char c);
	void print (int x, int y, char* c);
	void draw (int x=0, int y=0);
	void pause();
	char getCh();

};

void termClear ();

#endif
