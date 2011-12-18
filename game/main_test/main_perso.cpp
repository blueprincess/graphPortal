
#include "Perso.hpp"

#include "../boids/BoidsApplication.h"
#include <stein/math/StreamUtils.h>

#include <iostream>

#include <vector>

using namespace std;

int main(int argc, char **argv) {

// TEST Classe Perso

    try {

	Perso a;
	a.setPosition(0.0,1.2,5.8);

	double x,y,z;

	a.setMove(10.0,8.8,4.2);

	a.getPosition(x,y,z);

	cout << x << "," << y  << "," << z << endl;

	double tab[3];

	tab[0] = 1.;
	tab[1] = 2.;
	tab[2] = 3.;

	a.setPosition(tab);

	a.setMove(tab);

	double * position;

	position=a.getPosition();

	cout << position[0] << "," << position[1]  << "," << position[2] << endl;


		
        return EXIT_SUCCESS;

    } catch (exception &e) {
        cerr << e.what() << endl;
    } catch (...) {
        cerr << "Something very bad happened..." << endl;
    }
    return EXIT_FAILURE;
}

