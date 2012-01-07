#ifndef _BOID_
#define _BOID_

#include "GLHeaders.hpp"
#include "Scene.hpp"
#include "Tools.hpp"

#include <vector>


//! Contient un groupe d’unités
struct Boids
{
//! Pointeur vers la scène du template
	Scene * scene;

//! Nombre d’unités
	 GLuint nbUnit;

//! Position des unités [nbUnit * 3]
	GLfloat * unitPositions;

//! Vitesse/Direction de chaque unité [nbUnit*3]
	GLfloat * unitVelocities;

//! Offset dans scene->drawnObjects
	GLuint firstUnitId;
};

//renvoie aléatoire d'un float
float ran();

//! Initialise une structure Boids
void initBoids(Boids * boids, //! Structure à initialiser
	GLuint firstUnitID, //! Offset dans scene->drawnObjects
	GLuint nbUnits, //! Nombre d’unités
	Scene * scene); //! Objet Scene utilisé pour le rendu

//! Remplit la matrice model d’une unité
void setUnitModel(Boids * boids,
		GLuint unitId, //! Identifiant de l’unité
		GLfloat * model); //! Nouvelle matrice model

//! Déplace toutes les unités selon les règles d’animation
void updateBoids(Boids * boids);

	//! règle de la cohésion
	void cohesion(Boids * boids, GLuint unitId, GLfloat * centre);

	//! règle de l'alignement
	void  alignement(Boids * boids, GLuint unitId, GLfloat * velocity);

	//! règle de la séparation
	void separation(Boids * boids, GLuint unitId, GLfloat * c);


//! Libère la mémoire allouée
void deleteBoids(Boids * boids);


#endif
