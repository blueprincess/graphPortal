
#ifndef _LUMIERE_H_
#define _LUMIERE_H_

#include "../stein/GLHeaders.hpp"

// tmp dev
#include <iostream>


class Lumiere {

	public:
		Lumiere(GLuint shader, unsigned int lumiereType);
		~Lumiere();

		void setPosition(const double &x, const double &y, const double &z);
		void setPosition(const double new_position[3]);

		void setMove(const double &x, const double &y, const double &z);
		void setMove(const double new_position[3]);

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

