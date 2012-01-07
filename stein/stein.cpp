// main.c
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "GLHeaders.hpp"
#include "Application.hpp"
#include "Builders.hpp"
#include "Tools.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Boid.hpp"

#include <iostream>
#include <vector>
#include <string>

// Entry point in the program
void GLlaunch()
{
    std::cout<<std::endl<<std::endl<<"________Template OpenGL3________"<<std::endl<<std::endl;  
    //__________________________________________________________________________ 
    
    // Application creation
    std::cout<<"  Application creation"<<std::endl;
    Application * application=new Application();
    //__________________________________________________________________________ 

	// Scene creation
    std::cout<<"    Scene creation"<<std::endl;
    Scene * scene=new Scene();
    application->setScene(scene);
    GLfloat red[]={1.0, 0.0, 0.0, 1.0};
    scene->setDefaultColor(red); // Color for the objects drawn in the scene
    
    //__________________________________________________________________________

    // Camera position and orientation
    std::cout<<"    Camera settings"<<std::endl;
    scene->camera->c[2]=1.2; //1.1 Position of the camera
    
    scene->camera->updateView();

    //__________________________________________________________________________

    // Creation of the shaders
    std::cout<<"    Shaders reading, compiling and linking"<<std::endl;
    
    // Compilation and storage of the program
    std::vector<std::string> files;

    // The special shaders used for mac os are only necesary until Snow Leopard
    // From Leon on, mac os drivers version for Opengl is 3.2
    // Therefore the shaders in folder oldGLSL become out-of-date

    // One very simple shader (one color everywhere on the object)
	files.push_back("../shaders/simpleShader.glsl");
    GLuint simpleShaderID=loadProgram(files); files.pop_back();
    scene->setDefaultShaderID(simpleShaderID);
    
    // One coloring shader (colors per vertex)
	files.push_back("../shaders/colorShader.glsl");
    GLuint colorShaderID=loadProgram(files); files.pop_back();
    
    /*--------------------------------------
	CREATION D'UN SHADER D'ILLUMINATION CLASSIQUE
	--------------------------------------*/
   
    //One lighting shader
    #ifdef __APPLE__
    	files.push_back("../shaders/oldGLSL/shaderTools.glsl");
    	files.push_back("../shaders/oldGLSL/lightingShader.glsl");
    #else
    	files.push_back("../shaders/shaderTools.glsl");
    		files.push_back("../shaders/lightingShader.glsl");
    #endif
    GLuint lightingShaderID=loadProgram(files); files.pop_back();
    scene->setDefaultShaderID(lightingShaderID);
    // Uniforms filling
    glUseProgram(lightingShaderID);
    	//Material creation and to shader
    GLfloat ambient[]={1.0,1.0,1.0,1.0}; GLfloat ka=0.01;
    GLfloat diffuse[]={1.0,1.0,1.0,1.0}; GLfloat kd=1.0;
    GLfloat specular[]={1.0,1.0,1.0,1.0}; GLfloat ks=2.0; GLfloat shininess=5.0;
    setMaterialInShader(lightingShaderID, ambient, diffuse, specular, ka, kd, ks, shininess);

    ////
    //One texturing shader
    #ifdef __APPLE__
    	files.push_back("../shaders/oldGLSL/lightingTexturingShader.glsl");
    #else
    	files.push_back("../shaders/lightingTexturingShader.glsl");
    #endif
GLuint lightingTextureShaderID=loadProgram(files); files.pop_back();

	
    ////
    // Uniforms filling
    glUseProgram(lightingTextureShaderID);
    setMaterialInShader(lightingTextureShaderID, ambient, diffuse, specular, ka, kd, ks, shininess);
    setTextureUnitsInShader(lightingTextureShaderID);
GLuint houseTextureDiffuseID=loadTexture("../textures/house_diffuse.ppm");
      GLuint houseTextureSpecularID=loadTexture("../textures/house_spec.ppm");
    
    //__________________________________________________________________________ 
    
 	/*--------------------------------------
	CREATION D'UNE LUMIERE
	--------------------------------------*/
	
	//Light creation
	GLfloat lightPosition[]={0.0,0.0,5.0,1.0}; GLfloat lightPower=0.2;    //Light set by myself to 5.0
	scene->setLight(lightPosition, lightPower);
	


	//__________________________________________________________________________ 

    // Creation of the objects to store
    std::cout<<"    Generic objects creation and storage :"<<std::endl;




		////***** affichage des triangle*****////



      Object * objectbuildTriangle=new Object(GL_TRIANGLES); 
      GLuint storedObjectbuildTriangleID=scene->storeObject(objectbuildTriangle);

      Object * objectbuildSky=new Object(GL_TRIANGLES); 
      GLuint storedObjectbuildSkyID=scene->storeObject(objectbuildSky);
	buildSphere(objectbuildSky, 50.0, 10, 10);


    //__________________________________________________________________________
    
    // Object building
    std::cout<<"    Generic objects building :"<<std::endl;

	bool smoothObjectFlag=true;
	std::string fileName="../objs/poisson.obj";
    


	//GLfloat decalageTriangle[3]={0.0, 0.0, 0.0}; //Definition du vecteur de decalage
    	//buildTriangle(objectbuildTriangle, decalageTriangle);
	buildObjectGeometryFromOBJ(objectbuildTriangle,fileName, smoothObjectFlag);



    //__________________________________________________________________________
      

      
    // Objects we want to see
    std::cout<<"    Objects to draw setting"<<std::endl;

	GLuint SkyID=scene->addObjectToDraw(storedObjectbuildSkyID);

	scene->setDrawnObjectShaderID(SkyID, lightingShaderID);

	GLfloat SkyColor[4]={0.1, 0.1, 1.0, 1.0};
	scene->setDrawnObjectColor(SkyID, SkyColor);
    
	GLuint firstID=scene->addObjectToDraw(storedObjectbuildTriangleID);
	
    	scene->setDrawnObjectShaderID(firstID, lightingShaderID);

	GLuint UNIT_MAX=50;

	GLuint currentID=0;

	for(GLuint nbreTriangle=0 ; nbreTriangle<(UNIT_MAX-1) ; nbreTriangle++){
		
		currentID=scene->addObjectToDraw(storedObjectbuildTriangleID);
	
					//Textures
	 	scene->setDrawnObjectShaderID(currentID, lightingTextureShaderID);
		scene->setDrawnObjectTextureID(currentID, 0, houseTextureDiffuseID);
		scene->setDrawnObjectTextureID(currentID, 1, houseTextureSpecularID);
	
	}

//----------------------------------------------------------------------------------
/*				BOID						*/
//----------------------------------------------------------------------------------

Boids * oiseau;

oiseau = new Boids();

application->oiseau=oiseau;

initBoids(oiseau, firstID, UNIT_MAX, scene);


//----------------------------------------------------------------------------------
    
GLfloat c[]={0.0, 0.5, 10.0, 1.0};
GLfloat aim[]={0.0, 0.0, 0.0, 1.0};
GLfloat up[]={0.0, 1.0, 0.0, 0.0};
scene->camera->lookAt(c, aim , up);




    // Other informations
    // Background color creation
    GLfloat black[]={0.0, 0.0, 0.0, 1.0};
    application->setBackgroundColor(black);//changed background here
    
    //__________________________________________________________________________
    
    // Errors checking
    std::cout<<"    Errors cheking before the loop"<<std::endl;
    printGlErrors(); std::cout<<std::endl;
    //__________________________________________________________________________  
      
    // Loop start !
    std::cout<<"    Looping !! "<<std::endl;
    application->lastStartTime=getTime();
    application->initTimers();
    application->loop();
    printGlErrors(); std::cout<<std::endl;
    //__________________________________________________________________________

    // Cleaning and finishing
    std::cout<<"    Cleaning before leaving"<<std::endl; 
    delete application;
	deleteBoids(oiseau);
    delete oiseau;
   
}
