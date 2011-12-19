
#include "Lumiere.hpp"

#include <cstdlib>

Lumiere::Lumiere(GLuint shader, unsigned int lumiereType) : shaderId(shader), type(lumiereType) {

	position = new double[3];
	color = new GLfloat[3];

}

Lumiere::~Lumiere() {

	delete position;
	delete color;

}

void Lumiere::setPosition(const double &x, const double &y, const double &z){

	position[0] = x;
	position[1] = y;
	position[2] = z;
};


void Lumiere::setPosition(const double new_position[3]){

		position[0] = new_position[0];
		position[1] = new_position[1];
		position[2] = new_position[2];


};

void Lumiere::setMove(const double &x, const double &y, const double &z){

	position[0] += x;
	position[1] += y;
	position[2] += z;
};


void Lumiere::setMove(const double new_position[3]){

		position[0] += new_position[0];
		position[1] += new_position[1];
		position[2] += new_position[2];

};

void Lumiere::getPosition(double &x, double &y, double &z) const {

	x = position[0];
	y = position[1];
	z = position[2];
}

double * Lumiere::getPosition() {

	double now_position[3];

	now_position[0] = position[0];
	now_position[1] = position[1];
	now_position[2] = position[2];

	return position;
}

