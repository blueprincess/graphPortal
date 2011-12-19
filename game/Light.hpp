
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "../stein/GLHeaders.hpp"

//	Definition des différents type de lumiere

#define lightPhong 1

// tmp dev
#include <iostream>


class Light {

	public:
		Light(const GLuint &shader,const unsigned int &lightType);
		~Light();

		void setPosition(const double &x, const double &y, const double &z);
		void setPosition(const double new_position[3]);

		void setMove(const double &x, const double &y, const double &z);
		void setMove(const double new_position[3]);

		void setShaderId(const GLuint &shader);

		void getPosition(double &x, double &y, double &z) const;
		double * getPosition();

	private:
		GLuint shaderId;
		const unsigned int type;

		double * position;
		GLfloat * color;
		GLfloat * data;

};

#endif

