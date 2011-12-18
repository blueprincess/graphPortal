
#ifndef _GRAPHISME_H_
#define _GRAPHISME_H_

#include "Perso.hpp"

#include "../stein/math/Matrix4f.hpp"

using namespace std;
using namespace stein;

/* Gestion Graphique du jeu
//
//	cette classe cointiendra (avec probablement de futures sous classes: textures ...) l'ensemble des fonction qui gèront les effets et rendus graphisques
*/

class Graphisme {

/* A faire
		chargement obj 3ds
		chargement map (solution technique non élaboré actuellement)
		gestion textures

	à créer part petites fonctions combinées
*/

	public:
		Graphisme();
		void createMatrixCamera(Perso a);


	private:

		Matrix4f * cameraPerso; // Matrix of character's view



};

#endif

