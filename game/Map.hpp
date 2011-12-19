
#ifndef _MAP_H_
#define _MAP_H_

#include "Light.hpp"

using namespace std;

/* Classe de gestion de la map :  fabrication, chargement, type de mur .. */


class Map {

	public:
		Map();
		~Map();

	protected:
		

	private:
		unsigned int nbLights;
		Light * data;


};

#endif

