
#include "Perso.hpp"

#include <cstdlib>

using namespace std;

Perso::Perso() : PointVie(100) {

	position = new double[3];

};

void Perso::setPosition(const double &x, const double &y, const double &z){

	position[0] = x;
	position[1] = y;
	position[2] = z;
};


void Perso::setPosition(const double new_position[3]){

		position[0] = new_position[0];
		position[1] = new_position[1];
		position[2] = new_position[2];


};

void Perso::setMove(const double &x, const double &y, const double &z){

	position[0] += x;
	position[1] += y;
	position[2] += z;
};


void Perso::setMove(const double new_position[3]){

		position[0] += new_position[0];
		position[1] += new_position[1];
		position[2] += new_position[2];

};

void Perso::getPosition(double &x, double &y, double &z) const {

	x = position[0];
	y = position[1];
	z = position[2];
}

double * Perso::getPosition() {

	double now_position[3];

	now_position[0] = position[0];
	now_position[1] = position[1];
	now_position[2] = position[2];

	return position;
}

