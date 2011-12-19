
#include "Light.hpp"

#include <cstdlib>

using namespace std;

/***********************************/
/*                                 */
/*            Light                */
/*                                 */
/***********************************/

Light::Light(const GLuint &shader) : shaderId(shader) {

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
}


void Light::setPosition(const double new_position[3]){

	position[0] = new_position[0];
	position[1] = new_position[1];
	position[2] = new_position[2];


}

void Light::setMove(const double &x, const double &y, const double &z){

	position[0] += x;
	position[1] += y;
	position[2] += z;
}


void Light::setMove(const double new_position[3]){

	position[0] += new_position[0];
	position[1] += new_position[1];
	position[2] += new_position[2];

}

void Light::setShaderId(const GLuint &shader){

	shaderId = shader;

}

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

/***********************************/
/*                                 */
/*            Phong                */
/*                                 */
/***********************************/

Phong::Phong(const GLuint &shader) : Light(shader), ka(0.01), kd(1.0), ks(2.0), shininess(5.0) {

	ambient = new GLfloat[4];

	ambient[0]=1.0;
	ambient[1]=1.0;
	ambient[2]=1.0;
	ambient[3]=1.0;

	diffuse = new GLfloat[4];

	diffuse[0]=1.0;
	diffuse[1]=1.0;
	diffuse[2]=1.0;
	diffuse[3]=1.0;

	specular = new GLfloat[4];

	specular[0]=1.0;
	specular[1]=1.0;
	specular[2]=1.0;
	specular[3]=1.0;


}

Phong::~Phong() {

	delete ambient;
	delete diffuse;
	delete specular;

}

void Phong::setPhongAmbient(const GLfloat vec4[4]){

	ambient[0]=vec4[0];
	ambient[1]=vec4[1];
	ambient[2]=vec4[2];
	ambient[3]=vec4[3];

}

void Phong::setPhongDiffuse(const GLfloat vec4[4]){


	diffuse[0]=vec4[0];
	diffuse[1]=vec4[1];
	diffuse[2]=vec4[2];
	diffuse[3]=vec4[3];

}

void Phong::setPhongSpecular(const GLfloat vec4[4]){

	specular[0]=vec4[0];
	specular[1]=vec4[1];
	specular[2]=vec4[2];
	specular[3]=vec4[3];

}

void Phong::setPhongData(const GLfloat &new_ka, const GLfloat &new_kd, const GLfloat &new_ks, const GLfloat &new_shininess){

	ka = new_ka;
	kd = new_kd;
	ks = new_ks;
	shininess = new_shininess;

}

