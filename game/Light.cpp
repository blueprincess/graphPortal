
#include "Light.hpp"

#include <cstdlib>

Light::Light(const GLuint &shader,const unsigned int &lightType) : shaderId(shader), type(lightType) {

	position = new double[3];
	color = new GLfloat[3];

}

Light::~Light() {

	delete position;
	delete color;

}

void Light::setPosition(const double &x, const double &y, const double &z){

	position[0] = x;
	position[1] = y;
	position[2] = z;
};


void Light::setPosition(const double new_position[3]){

		position[0] = new_position[0];
		position[1] = new_position[1];
		position[2] = new_position[2];


};

void Light::setMove(const double &x, const double &y, const double &z){

	position[0] += x;
	position[1] += y;
	position[2] += z;
};


void Light::setMove(const double new_position[3]){

		position[0] += new_position[0];
		position[1] += new_position[1];
		position[2] += new_position[2];

};

void Light::setShaderId(const GLuint &shader){

	shaderId = shader;

};

void Light::getPosition(double &x, double &y, double &z) const {

	x = position[0];
	y = position[1];
	z = position[2];
}

double * Light::getPosition() {

	double now_position[3];

	now_position[0] = position[0];
	now_position[1] = position[1];
	now_position[2] = position[2];

	return position;
}

