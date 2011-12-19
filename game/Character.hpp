
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//lib tmp pour dev:
#include <iostream>

using namespace std;

/* cette classe doit gerer l'ensemble des déplacement du personnage et de ces attributs */

class Character {

	public:

		Character();
		~Character();

		void setPosition(const double &x, const double &y, const double &z);
		void setPosition(const double new_position[3]);

		void setMove(const double &x, const double &y, const double &z);
		void setMove(const double new_position[3]);

		void getPosition(double &x, double &y, double &z) const;
		double * getPosition();

		bool setLife(int variation);

	private:

		unsigned int pointVie;
		unsigned int nbreBombes; // Peinture rapprochee
		unsigned int nbrePots; // Peinture projetee
		double * position;

};

#endif

