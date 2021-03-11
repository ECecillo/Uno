

class Joueur
{
private:
		string nom;
		Carte * main; //vector

	 

public:
		/**
		* @brief Construit une nouvelle carte.
		* 
		*/
		Joueur();

		
		~Joueur();


		void gagnant();


		void afficherMainJoueur();

		
				
};

#endif
