
#ifndef _PERSO_H_
#define _PERSO_H_

//lib tmp pour dev:
#include <iostream>

using namespace std;

/* cette classe doit gerer l'ensemble des déplacement du personnage et de ces attributs */

class Perso {

	public:

		Perso();
		~Perso();

		void setPosition(const double &x, const double &y, const double &z);
		void setPosition(const double new_position[3]);

		void setMove(const double &x, const double &y, const double &z);
		void setMove(const double new_position[3]);

		void getPosition(double &x, double &y, double &z) const;
		double * getPosition();

/* A faire:

	fonction de gestion vie:
		setLife(unsigned int valeur) ex: -3 retire 3 point de vie
		bool isLive

*/

	private:
		unsigned int PointVie;
		unsigned int nbreBombes; // Peinture rapprochee
		unsigned int nbrePots; // Peinture projetee
		double * position;

};

#endif

