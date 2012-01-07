#include "Boid.hpp"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <math.h>

using namespace std;

float ran() {
	return float(rand()) / RAND_MAX;
}


void initBoids(Boids * boids, GLuint firstUnitID, GLuint nbUnits, Scene * scene){

	boids->firstUnitId = firstUnitID;
	boids->nbUnit = nbUnits;

	boids->unitVelocities = new GLfloat[3*nbUnits];
	boids->unitPositions = new GLfloat[3*nbUnits];

	boids->scene = scene;
	
	GLfloat translationMatrix[16];
	GLfloat translationVector[3];

	//initialisation de manière aléatoire des positions et vélocités
	for(GLuint unit=0; unit <  boids->nbUnit; ++unit){

		float reducVitesse=0.01;

		boids->unitVelocities[3*unit]=reducVitesse*ran();
		boids->unitVelocities[3*unit+1]=reducVitesse*ran();
		boids->unitVelocities[3*unit+2]=reducVitesse*ran();

		boids->unitPositions[3*unit]=5*ran();
		boids->unitPositions[3*unit+1]=5*ran();
		boids->unitPositions[3*unit+2]=5*ran();

		setToIdentity(translationMatrix);

		translationVector[0]=0;
		translationVector[1]=0;
		translationVector[2]=0;

		translationVector[0]=boids->unitPositions[3*unit];
		translationVector[1]=boids->unitPositions[3*unit+1];
		translationVector[2]=boids->unitPositions[3*unit+2];

		setToTranslate(translationMatrix,translationVector);
	
		boids->scene->setDrawnObjectModel(boids->firstUnitId + unit, translationMatrix );

		if(unit!=0)
		scene->addObjectToDraw(firstUnitID + unit);
	
	
	}



}

void updateBoids(Boids * boids){

	GLfloat v1[]={0,0,0};
	GLfloat v2[]={0,0,0};
	GLfloat v3[]={0,0,0};

	GLfloat * centre = v1;
	GLfloat * velocity = v2;
	GLfloat * c = v3;

	float reducGeneral = 0.01;
	float reducSeparation = 0.05;
	float reducAlignement = 0.5;

	GLfloat up[3];
	GLfloat x[3];
	GLfloat y[3];
	GLfloat z[3];


	//On parcours l'ensemble des oiseaux
	for(GLuint unit=0; unit < boids->nbUnit; ++unit){

		cohesion(boids,unit,centre);
		alignement(boids,unit,velocity);
		separation(boids,unit,c);

		boids->unitVelocities[3*unit] += (v1[0] + v2[0]*reducAlignement + v3[0]*reducSeparation)*reducGeneral;	//X
		boids->unitVelocities[3*unit+1] += (v1[1] + v2[1]*reducAlignement + v3[1]*reducSeparation)*reducGeneral;	//Y
		boids->unitVelocities[3*unit+2] += (v1[2] + v2[2]*reducAlignement + v3[2]*reducSeparation)*reducGeneral;	//Z

		boids->unitPositions[3*unit]+= boids->unitVelocities[3*unit];		//X
		boids->unitPositions[3*unit+1]+= boids->unitVelocities[3*unit+1];	//Y
		boids->unitPositions[3*unit+2]+= boids->unitVelocities[3*unit+2];	//Z

		GLfloat translationMatrix[16];
		setToIdentity(translationMatrix);

		GLfloat translationVector[3];
		translationVector[0]=boids->unitPositions[3*unit];
		translationVector[1]=boids->unitPositions[3*unit+1];
		translationVector[2]=boids->unitPositions[3*unit+2];

		setToTranslate(translationMatrix,translationVector);
	
				//***   on change l'orientation du poisson en fct de la direction   ***//

		z[0]=boids->unitVelocities[3*unit];
		z[1]=boids->unitVelocities[3*unit+1];
		z[2]=boids->unitVelocities[3*unit+2];
		normalize(z);

		up[0]=0.0;
		up[1]=1.0;
		up[2]=0.0;
		
		x[0]=z[1]*up[2]-z[2]*up[1];
		x[1]=z[2]*up[0]-z[0]*up[2];
		x[2]=z[0]*up[1]-z[1]*up[0];
		normalize(x);

		y[0]=z[1]*x[2]-z[2]*x[1];
		y[1]=z[2]*x[0]-z[0]*x[2];
		y[2]=z[0]*x[1]-z[1]*x[0];
		normalize(y);

		GLfloat rotationMatrix[]={x[0], x[1],  x[2],  0.0,
                 y[0], y[1], y[2],  0.0,
                 z[0],  z[0],  z[2], 0.0,
                 0.0,  0.0,  0.0,  1.0};  

		multMatrixBtoMatrixA(translationMatrix, rotationMatrix);

		boids->scene->setDrawnObjectModel(boids->firstUnitId + unit, translationMatrix );

	}

}

void cohesion(Boids * boids, GLuint unitId, GLfloat * centre){

	float module;

	//On parcours l'ensemble des oiseaux
	for(GLuint unit=0; unit < boids->nbUnit; ++unit){

		//On exclut l'oiseau considerer
		if (unit!=unitId){
				module = sqrt(pow((boids->unitPositions[3*unit] - boids->unitPositions[3*unitId]),2)
					+ pow((boids->unitPositions[3*unit+1] - boids->unitPositions[3*unitId+1]),2)
					+ pow((boids->unitPositions[3*unit+2] - boids->unitPositions[3*unitId+2]),2));
		
			if(module<10){

				//On ajoute les coordonnées de l'oiseau dans centre
				centre[0]+=boids->unitPositions[3*unit]; //X
				centre[1]+=boids->unitPositions[3*unit +1]; //Y
				centre[2]+=boids->unitPositions[3*unit +2]; //Z
			}
		}
	}
	
	//On fait la moyenne pour avoir le barycentre
	centre[0]=centre[0]/(boids->nbUnit - 1);
	centre[1]=centre[1]/(boids->nbUnit - 1);
	centre[2]=centre[2]/(boids->nbUnit - 1);

	centre[0] = (centre[0]-boids->unitPositions[3*unitId]) / 10.;//X
	centre[1] = (centre[1]-boids->unitPositions[3*unitId+1]) / 10.;//Y
	centre[2] = (centre[2]-boids->unitPositions[3*unitId+2]) / 10.;//Z

}

void alignement(Boids * boids, GLuint unitId, GLfloat * velocity){

	float module;

	//On parcours l'ensemble des oiseaux
	for(GLuint unit=0; unit < boids->nbUnit; ++unit){

		//On exclut l'oiseau considerer
		if (unit!=unitId){
				module = sqrt(pow((boids->unitPositions[3*unit] - boids->unitPositions[3*unitId]),2)
					+ pow((boids->unitPositions[3*unit+1] - boids->unitPositions[3*unitId+1]),2)
					+ pow((boids->unitPositions[3*unit+2] - boids->unitPositions[3*unitId+2]),2));
		
			if(module<10){

				//On ajoute les coordonnées de l'oiseau dans centre
				velocity[0]+=boids->unitVelocities[3*unit]; //X
				velocity[1]+=boids->unitVelocities[3*unit +1]; //Y
				velocity[2]+=boids->unitVelocities[3*unit +2]; //Z
			}
		}
	}
	
	//On fait la moyenne pour avoir le barycentre
	velocity[0]=velocity[0]/(boids->nbUnit - 1);
	velocity[1]=velocity[1]/(boids->nbUnit - 1);
	velocity[2]=velocity[2]/(boids->nbUnit - 1);

	velocity[0] = (velocity[0]-boids->unitVelocities[3*unitId]) / 8.;//X
	velocity[1] = (velocity[1]-boids->unitVelocities[3*unitId+1]) / 8.;//Y
	velocity[2] = (velocity[2]-boids->unitVelocities[3*unitId+2]) / 8.;//Z
	
}

//! règle de la séparation
void separation(Boids * boids, GLuint unitId, GLfloat * c){

	float module;

	for(GLuint unit=0; unit < boids->nbUnit; ++unit){

		if(unit!=unitId){
			module = sqrt(pow((boids->unitPositions[3*unit] - boids->unitPositions[3*unitId]),2)
					+ pow((boids->unitPositions[3*unit+1] - boids->unitPositions[3*unitId+1]),2)
					+ pow((boids->unitPositions[3*unit+2] - boids->unitPositions[3*unitId+2]),2));
		
			if(module<1){

				c[0]-= (boids->unitPositions[3*unit] - boids->unitPositions[3*unitId]);
				c[1]-= (boids->unitPositions[3*unit+1] - boids->unitPositions[3*unitId+1]);
				c[2]-= (boids->unitPositions[3*unit+2] - boids->unitPositions[3*unitId+2]);
			}
		}
	}

}

void deleteBoids(Boids * boids){

delete boids->unitVelocities;

delete boids->unitPositions;

}


