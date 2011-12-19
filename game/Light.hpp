
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "../stein/GLHeaders.hpp"

#define NULL_ShaderId 10000

// tmp dev
#include <iostream>


class Light {

	public:

		Light();
		Light(const GLuint &shader);
		~Light();

		void setPosition(const double &x, const double &y, const double &z);
		void setPosition(const double new_position[3]);

		void setMove(const double &x, const double &y, const double &z);
		void setMove(const double new_position[3]);

		void setShaderId(const GLuint &shader);

		void getPosition(double &x, double &y, double &z) const;
		double * getPosition();

	protected:

		GLuint shaderId;

	private:

		double * position;
		GLfloat * color;

};

class Phong : public Light {

	public:

		Phong(const GLuint &shader);
		~Phong();

		void setPhongAmbient(const GLfloat vec4[4]);
		void setPhongDiffuse(const GLfloat vec4[4]);
		void setPhongSpecular(const GLfloat vec4[4]);
		void setPhongData(const GLfloat &new_ka, const GLfloat &new_kd, const GLfloat &new_ks, const GLfloat &new_shininess);

	private:

		GLfloat * ambient;
		GLfloat * diffuse;
		GLfloat * specular;
		GLfloat ka;
		GLfloat kd;
		GLfloat ks;
		GLfloat shininess;

};

#endif

