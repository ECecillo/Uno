#ifndef MENU_H
#define MENU_H

#include <string.h>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

/**
* @brief
* Classe gérant le menu et les choix d'options.
* 
*/
class Menu
{
private:
	vector<string> nomMenu;		   // map (Jouer,Règlages,Quitter,Logs,Règles)
	vector<string> nomOptions;		   // Résolution, Changer Pseudo, Son.
	vector<string> resolutionOptions; // 1920 x 1080, 720 x 576, 1280 x 720,
	vector<string> sonOptions;		   // i = 0 à 11

public:
	Menu();
	~Menu();

	void initNomMenu();
	void initNomOptions();
	void initResolutionOptions();
	void initSonOptions();


	string getNomMenu(int i);
	string getOptions(int i);
	string getResolution(int i);
	string getSon(int i);

	void testRegression();

};

#endif
