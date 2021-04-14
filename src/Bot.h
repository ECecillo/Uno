#ifndef BOT_H
#define BOT_H

#include <Jeu.h>
#include <string.h>
#include <Carte.h>
#include <algorithm>
#include <iostream>
#include <Jeu.h>
#include <cassert>



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
         * @brief Selon les résultats retournés par les autres fonctions l'IA
         *  posera la carte sur talon.
         *
         */
    void poserCarte() const;

    /**
         * @brief Compte le nombre de carte qui ont la couleur du talon.
         *
         */
    int carteMemeCouleurTalon(const Jeu &jeu) const;

     /**
      * @brief
      * Compte le nombre de Carte de la couleur de la carte qui a la même valeur que celle du Talon.
      * @param jeu
      * @return int
      */
    int carteMemeValeurTalon(const Jeu &jeu) const;

    void choixJeu(const Jeu &jeu);

     void testRegression();

    void setCarteRouge();
    void setCarteVert();
    void setCarteBleu();
    void setCarteJaune();
    void testDeFonctionPourVim();

private:
    int nbCarteRouge;
    int nbCarteVert;
    int nbCarteBleu;
    int nbCarteJaune;

};

#endif
