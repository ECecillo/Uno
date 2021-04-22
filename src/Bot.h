#ifndef BOT_H
#define BOT_H

#include <Jeu.h>
#include <string.h>
#include <Carte.h>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <unistd.h>

class Jeu;

class Bot
{
public:
    /**
         * @brief
         * Numéro du bot
         */
    unsigned int numeroBot;

    /**
     * @brief
     * Nom du bot.
     */
    string nom;

    /**
		* @brief
		* Tableau dynamique contenant les cartes du bot.
		*
		*/
    vector<Carte> main;

    /**
     * @brief Construit un nouvelle objet Bot.
     *
     */
    Bot();

    /**
     * @brief Construit un nouvelle objet Bot avec le numéro du bot.
     *
     * @param num : Numéro du bot.
     */
    Bot(const unsigned int num);

    /**
      * @brief 
      * Trie le vecteur main du bot.
      * 
      */
    void trierMain();

    /**
         * @brief 
         * Procédure qui posera la carte que l'on aura choisit dans choix Jeu.
         */
    void poserCarte() const;

    /**
      * @brief 
      * Compte le nombre de carte qui ont la couleur du talon.
      * @return entier.
      */
    int carteMemeCouleurTalon(const Jeu &) const;

    /**
      * @brief 
      * Compte le nombre de Carte de la couleur de la carte qui a la même valeur que celle du Talon.
      * @param tab : tableau dans lequelle on met le nombre de carte par couleur.
      */
    int carteMemeValeurTalon(const Jeu &, int &c, int &indiceCarte);

    /**
     * @brief 
     * Fonction qui renvoie la couleur avec le plus de carte.
     * @return int 
     */
    int couleurAvecPlusDeCarte() const;

    /**
     * @brief 
     * D'après les fonctions carteMemeCouleur et CarteMemeValeur on dit à l'ordinateur quelle carte il va jouer.
     */
    void choixJeu(Jeu &);

    /**
     * @brief 
     * Selon l'entier passé en paramètre on déclenchera poserCarte().
     * @param couleur : Couleur de la carte qui sera joué.
     * @param valeurCarteSpeciale : Dans le cas où on va jouer une carte 13 ou 14.
     */
    void joueCouleurSelonEntier(Jeu &jeu, int couleur, int valeurCarteSpeciale);

    void changeCouleurCarte(Jeu &jeu, const int );

    void changeIndiceCarteSpeciale(int indiceCarte);

    void testRegression(Jeu &);

    void setCarteRouge();
    void setCarteVert();
    void setCarteBleu();
    void setCarteJaune();
    void setCarteJoker(int i);
    void setCartePlus4(int i);

    int getCarteRouge();
    int getCarteVert();
    int getCarteBleu();
    int getCarteJaune();
    int getCarteJoker();
    int getCartePlus4();

private:
    int nbCarteRouge;
    int nbCarteVert;
    int nbCarteBleu;
    int nbCarteJaune;
    int indCarteJoker; //
    int indCartePlus4;
    unsigned int indChoixCarte; // Indice qui dit quelle carte le bot a joué.
};

#endif
