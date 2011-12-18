
#ifndef _MAP_H_
#define _MAP_H_

using namespace std;

/* Classe de gestion de la map :  fabrication, chargement, type de mur .. */

class Map {

	public:
		virtual Map();
		virtual ~Map();

	protected:
		unsigned int type; // definira le type de l'objet constituant la map

	private:
		


};

#endif

