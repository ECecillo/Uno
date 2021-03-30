#ifndef AFFICHAGE_SDL
#define AFFICHAGE_SDL

#include <vector>
#include <string>

using namespace std;
class AffichageSDL
{
private:
	/**
 	* @brief 
 	*  Tableau dynamique de chaine de caractères
	*  (Jouer,Reprendre,Quitter,Logs,Règles) 
 	*/
	vector<string> NomsMenu;

public:
	void boucleJeu();
};

#endif
