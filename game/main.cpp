
#include "../stein/math/StreamUtils.h"
#include "../stein/stein.hpp"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {

/* Voici la classe principale de notre jeu: elle doit gérer les intéractions entre les différentes parties du jeu
//
//	gestion:	initialisation de l'ensemble
//			gestion des intéractions
//			appel des fonctions de rendu graphiques
*/	

    try {
       	GLlaunch();
        return EXIT_SUCCESS;
    } catch (exception &e) {
        cerr << e.what() << endl;
    } catch (...) {
        cerr << "Something very bad happened..." << endl;
    }
    return EXIT_FAILURE;
}

