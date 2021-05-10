# UNO

Uno sur ordinateur, jeu qui s'adresse au grand public, en version classique ou avec l'une des 5 variantes proposées.

Il s'agit de la réalisation finale de l'UE de L2 Informatique "Conception et Développement d'application". 
Le but était de mener un projet de conception et de développement d'application de bout en bout. Le thème étant choisi, il fallait concevoir l'application en choisissant les bons outils, des classes pertinentes, les bonnes librairies, séparer le fonctionnement du jeu et l'affichage, gérer la communication dans l'équipe, tenir les délais et le cahier des charges.


# Auteurs

@**CECILLON Enzo**  11805901
**mail** : enzo.cecillon@etu.univ-lyon1.fr

----
@**RAMPON Ildès**  89064800
**mail** : ildes.rampon@etu.univ-lyon1.fr

----
**Projet complet** 
**GitLab** : 17422


# Compiler le projet avec le makefile
|Fichier|Fonctionnalité|
|--|--|
|src/mainTest.cpp|Teste les fonctions et procédures de Jeu et Bot|
|src/mainTxt.cpp|Version texte du jeu|
|src/mainSDL.cpp|Version graphique du jeu|

# Librairie

Pour pouvoir compiler et exécuter **mainSDL.cpp** il faudra se munir des fichiers sources de **SDL2** que vous pouvez retrouver sur leur site [ici](https://www.libsdl.org/download-2.0.php) et les placer dans le dossier extern à la racine du Projet.

Si vous êtes sous linux veuillez télécharger les paquets suivant :

    $> sudo apt-get install libsdl2-dev libsdl2-image-dev 


    # Pour compiler tous les execs :
    make

    # Compiler les fichiers un à un :
    make (NomFichierMain)

**Remplacer (NomFichierMain) par un des fichiers présent dans le tableau ci-dessus.**


# Exécutables
|Exécutable|Fonctionnalité|
|--|--|
|bin/test|Pour teste les fonctions et procédures de Jeu et Bot|
|uno_txt|Pour jouer en version texte|
|uno_sdl|Pour jouer en version graphique|


# Règles du jeu et des variantes
Jeu d’ambiance de 2 à 10 joueurs à partir de 7 ans.
## Matériel
108 cartes réparties en :
19 cartes numérotés de 0 à 9 par couleur (bleues, vertes, rouges et jaunes)
2 cartes +2 de chaque couleur
2 cartes Inversion par couleur
2 cartes Passer par couleur
1 Joker par couleur
1 Super Joker par couleur
## But du jeu
Pour gagner au Uno, vous devez être le 1er à vous débarrasser de toutes vos cartes.
## Préparation
On distribue 7 par joueur. Les autres cartes sont placées faces cachées et forment la PIOCHE.
La carte du dessus de la PIOCHE est retournée pour constituer le TALON.
## Le principe
Les joueurs doivent recouvrir la carte visible du TALON par une carte : de même
couleur, ou portant le même numéro ou le même symbole que celle-ci (les symboles
représentent des Cartes Action. Voir le chapitre FONCTIONS DES CARTES ACTION.
Si le joueur ne peut pas ou ne veut pas jouer, il PIOCHE une carte. Il passe son tour et conserve cette carte supplémentaire.
## Fonctions des cartes action
### Carte +2
![❤ Règle du uno, apprendre à jouer rapidement !](https://jeu-uno.info/wp-content/uploads/2015/03/carte-plus2.jpg)
Lorsque cette carte est jouée, le joueur suivant doit piocher 2 cartes et passer son tour.
Cette carte ne peut être jouée que sur une carte de la même couleur ou sur une autre carte
+2. Si cette carte est retournée en début de jeu, la même règle s’applique.
### Carte Passer
![❤ Règle du uno, apprendre à jouer rapidement !](https://jeu-uno.info/wp-content/uploads/2015/03/carte-pass.jpg)
Lorsque cette carte est jouée, le joueur suivant doit passer son tour. Cette carte ne peut
être jouée que sur une carte de même couleur ou sur une autre carte Passer. Si cette carte
est retournée en début de jeu, le joueur à la gauche du donneur passe son tour et c’est au
joueur suivant de commencer.
### Carte Inversion
![Règle du Uno - aperçu de la carte inversion uno](https://jeu-uno.info/wp-content/uploads/2015/03/carte-inversion.jpg)
Lorsque cette carte est jouée, le sens du jeu change. Cette carte ne peut être jouée que sur une
carte de la même couleur ou sur une autre carte Inversion. Si cette carte est retournée en
début de jeu, le donneur joue en premier, puis le jeu continue vers la droite au lieu de la
gauche.
Lors d’une partie à deux joueurs, cette carte a le même effet que la carte Passer.
### Carte Joker
![Image carte joker uno](https://jeu-uno.info/wp-content/uploads/2015/03/carte-joker.jpg)
Celui qui joue cette carte peut choisir de changer la couleur (il annonce son choix en
posant la carte) ou de continuer dans la couleur demandée. Cette carte peut être jouée
après n’importe quelle autre carte, même si le joueur a une autre carte qu’il peut jouer. Si
c’est la première carte de la pioche, le joueur placé à gauche du donneur choisit la couleur
de départ et joue la première carte.
### Carte Super Joker
![Image carte joker Uno](https://jeu-uno.info/wp-content/uploads/2015/03/carte-plus4.jpg)
Celui qui joue cette carte peut choisir de changer la couleur ET oblige le joueur suivant à
piocher 4 cartes et à passer son tour.
Attention !
Un joueur ne peut jouer cette carte que s’il ne possède AUCUNE carte de la couleur
demandée COULEUR DEMANDÉE (mais il peut l’utiliser s’il possède une carte Action ou
une carte de chiffre identique).

## Fin de la partie
Lorsqu’un joueur pose son avant-dernière carte, il doit immédiatement annoncer à voix
haute « UNO » (qui signifie « un ») pour indiquer à ses adversaires qu’il n’a plus qu’une seule
carte en main.
S’il oublie et que l’un de ses adversaires le lui fait remarquer (en criant CONTRE UNO)
avant que le joueur suivant n’ait joué, il doit piocher deux cartes de pénalité.

Si la PIOCHE est épuisée avant la fin d’une manche, le TALON est mélangé et le jeu
continue.
## Variantes
### Variante «Cumul»
On peut jouer une carte +2 ou +4 sur une carte +2 et une carte +4 sur une carte +4. Le
joueur qui ne peut pas jouer pioche autant de cartes que la somme des +4 et +2 qui
viennent d’être joués.
### Variante « Doublon »
Si le joueur joue une carte alors qu’il a la deuxième carte identique dans sa main, il peut
jouer le doublon dans le même tour.
### Variante «Echange »
Quand un joueur pose un 7 sur le talon, il peut échanger son jeu avec n’importe quel autre
joueur.
### Variante «Suites»
Si un joueur dispose dans son jeu d’une suite de cartes numérotés (de 0 à 7 de même
couleur, il pourra les enchaîner à son tour de jeu tant que sa suite tient. Par exemple : poser
un 3 jaune puis un 4 puis 5 ainsi de suite.
### Variante «Tourne »
Dès qu’un joueur joue un 0 sur le talon, tous les joueurs doivent passer leur jeu au voisin
suivant selon le sens du jeu.

# Utilisation du jeu
## Version texte
Tout au clavier:
a: gauche
d: droite
t: haut
b: bas
e: entrée
p: passer, piocher
u: uno
c: contre uno
r: rouge
v: vert
j: jaune
b: bleu
q: quitter
## Version graphique
A la souris

### Changement de la résolution de la fenêtre
Lorsque vous changez la résolution dans la version SDL, il est nécessaire de redémarrer le jeu pour que les changements soit pris en compte.

# Organisation du code

**/bin**  Répertoire où les executables (exemple, test et affichage) seront compilés. 
 **/data**  Les images, sons.
**/doc**  Le fichier doxygen qui permet de générer la documentation du code.
**/obj**  Répertoire comportant les compilations intermédiaires (.o).
**/src**  Les classes (.h et .cpp) AffichageSDL, AffichageTxt, Bot, Carte, Fenetre, Jeu, Joueur, Menu, SalleAttente, VarianteCumul, VarianteDoublon, VarianteEchange, VarianteSuite, VarianteTourne, et les 3 main (mainTest.cpp, mainTxt.cpp, mainSDL.cpp)









# Documentation du code

Pour générer la doc sous doxygen il faudra vous munir des paquets nécessaires sous linux:

    # Installer Doxygen:
    sudo apt install doxygen
    
    # Generer la documentation:
    cd doc && doxygen image.doxy
    
    # La page principale générée avec Firefox (ou autre browser):
    firefox html/index.html


Vous pouvez modifier les propriétés de la doc en allant dans le fichier **doc/image.doxy**



