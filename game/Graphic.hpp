
#ifndef _Graphic_H_
#define _Graphic_H_

#include "Character.hpp"
#include "Map.hpp"

#include "../stein/math/Matrix4f.hpp"

using namespace std;
using namespace stein;

/* Gestion Graphique du jeu
//
//	cette classe cointiendra (avec probablement de futures sous classes: textures ...) l'ensemble des fonction qui gèront les effets et rendus graphisques
*/

class Graphic {

/* A faire
		chargement obj 3ds
		chargement map (solution technique non élaboré actuellement)
		gestion textures

	à créer par petites fonctions combinées
*/

	public:
		Graphic();
		void createMatrixCamera(Character a);


	private:

		Matrix4f * cameraCharacter; // Matrix of character's view



};

#endif

