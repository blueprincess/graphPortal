// Application.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Application.hpp"
#include "Scene.hpp"
#include "Tools.hpp"
#include "Camera.hpp"

#include <iostream>
#include <cmath>
#include <sstream>


// Default constructor
Application::Application()
{
    this->init();
}

/*--------------------------------------
QUESTION B - WIREFRAME EFFECT
--------------------------------------*/
void Application::fonction_wireframe(){
GLint params[2];
glGetIntegerv(GL_POLYGON_MODE, params);

	if (params[0]==GL_FILL){
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
}

// Cleans before the application can be closed
Application::~Application()
{

    /* La musique est terminée, on la libère */
    Mix_FreeMusic(Music);

    /* On libère le matériel */
    Mix_CloseAudio();	
	
    SDL_RemoveTimer(this->animate2Timer);
    SDL_Quit();
    if (scene!=NULL) delete scene;
}


// Sets the application parameters and does all the initialisation
void Application::init()
{
    this->scene=NULL;
    
    // False as long as we don't want to quit the application
    this->done=false;
    
    // BackgroundColor
	GLfloat black[]={0.0, 0.0, 0.0, 1.0};
    this->setBackgroundColor(black);
    
	this->moveFlags[0]=0.0;
	this->moveFlags[1]=0.0;
	this->moveFlags[2]=0.0;
	
	// Times measurements initialisation
	this->cntFrame=0;
	uint64_t lastStartTime=0;
	uint64_t frameDuration=0; 
	
    // Move counter
    this->cntAnimation=0;
    
    // Mouse is seeable at first
    this->mouse=true;
    
    // Window size initialization (for windowed mode)
	this->windowedWidth=800;
	this->windowedHeight=600;
	
	// Mouse position, pressed position and scroll data initilaization
    // Positions : floats, origin in center, sides at +/-1 or more
	this->xMousePosition=0.0;
	this->yMousePosition=0.0;
	this->xMouseLeftDownPosition=0.0; 
	this->yMouseLeftDownPosition=0.0;

	this->scroll=0;

	this->moveForward=false;
	this->moveBackward=false;
	this->moveLeft=false;
	this->moveRight=false;
	
	// Initialisation of SDL and creation of OpenGL context
    initSDLOpenGL();
    
    // Customize a few OpenGL and SDL states (after context creation)
    customizeStates();
}


// Inits SDL and OpenGL context, sets a few states
void Application::initSDLOpenGL()
{
    // Initialization of SDL
    
        // Initialize timer, audio, video, CD_ROM, and joystick.
    int sdlError=SDL_Init(SDL_INIT_EVERYTHING);
    if (sdlError<0) 
		std::cout<<"Unable to init SDL : "<<SDL_GetError()<<std::endl;

    	// Sets openGL parameters before opening the draw context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);    // Double buffering
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);		// Depth buffer size of 16-bit
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);    // Color components size of 8-bit each
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	// Operating systems uniform keyboard handling 
	// (overhead, disable for performance : param->0)
	SDL_EnableUNICODE(1);	

    // Capture of screen size (for fullscreen mode)
    
    const SDL_VideoInfo* videoInfo=SDL_GetVideoInfo();
	this->fullScreenWidth=videoInfo->current_w;
	this->fullScreenHeight=videoInfo->current_h;

	// Creation of the openGL draw context
	
		// Window size
	this->width=this->windowedWidth;
	this->height=this->windowedHeight;	
	    // Options about the OpenGL window for SDL_SetVideoMode(...)  
	this->videoModeFlags = SDL_OPENGL | SDL_RESIZABLE; // Resizable OpenGL display
        // Specifies the size and other options about the window and OpenGL context
     this->drawContext = SDL_SetVideoMode(this->width, this->height, 32, SDL_OPENGL | SDL_FULLSCREEN);

	// SON

	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	
	Music = Mix_LoadMUS("../sound/son.mp3");

	// Play Music
		
	Mix_PlayMusic(Music, -1);
}


// Customize a few OPenGL states to fit the application's needs
void Application::customizeStates()
{
   // Puts the window top-left corner at the following coordinates 
    // Resizing doesn't impact so it is not very usefull
	//putenv("SDL_VIDEO_WINDOW_POS=100,0"); 
	 
    	// Glew initialisation : to register all available extentions
    GLenum glewError=glewInit();
    if (glewError!=GLEW_OK)
		std::cout<<"GLEW Error : "<<glewGetErrorString(glewError)<<std::endl;
	
	// Mouse motion will not generate events
	// Instead we will check the position ourselves when we need them
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
		

	// Initialization of the mouse position in the middle of the window	

	    // WarpMouse changes the mouse position and 
        // generates motion events which we need to ignore.
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_WarpMouse(this->width/2, this->height/2);
        // After, we can reactivate the mouse motion events
        // But we instead choose to check directly the position 
        // ourselves when we need it (in the camera update)
        // It is better then to disable the unused events
    SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);


    	// Depth test
    glEnable(GL_DEPTH_TEST);
    
        // Decides the background color used after this call
    GLfloat black[]={0.0, 0.0, 0.0, 1.0};
    setBackgroundColor(black);
    
        // Sets the with of the lines
    glLineWidth(2); 
    
        // Disables culling
    //glDisable(GL_CULL_FACE);
}


// Sets one scene for the application to draw
void Application::setScene(Scene * scene)
{
	this->scene=scene;
	resize(this->width, this->height);
}


// Initializes timers
void Application::initTimers()
{
    //Each timer event has an arbitrairy int ID
    
   this->animate2EventID=2; // Identifies our move loop event
    
    // Creates a timer to send a move event every 60 ms    
    this->animate2Timer=SDL_AddTimer(30, genericTimer, (void*)(&(this->animate2EventID)));   
}


// Create an user event, give it the id (int) passed in "param",
// and registers it : it should now be send every "interval" set of time
Uint32 genericTimer(Uint32 interval, void* param)
{
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = *(int *)param; 
    event.user.data1 = 0;
    event.user.data2 = 0;
    SDL_PushEvent(&event);
    return interval;
}


// Adapts the drawing to the new size of the window
// resize doesn't work on Mac os (Windows ?)
void Application::resize(GLuint w, GLuint h)
{
    std::cout<<"Window resize  : ["<<w<<","<<h<<"]"<<std::endl;
    
    this->width=w;
    this->height=h;
	
//---	
GLfloat fovy=M_PI/3.0;
GLfloat aspectRatio=this->width/GLfloat(this->height);
this->scene->camera->setPerspectiveFromAngle(fovy, aspectRatio);

//----
	//SDL_VideoMode update (restart the OpenGL context on windows, does not work on mac os...)
	#ifdef __APPLE__ | WIN32 // was not tested
        //this->drawContext = SDL_SetVideoMode(this->width, this->height, 0, this->videoModeFlags);
        //std::cout<<SDL_GetError()<<std::endl;
        //this->customizeStates();
    #else
        this->drawContext = SDL_SetVideoMode(this->width, this->height, 0, this->videoModeFlags);
    #endif
	
    
    // Viewport transformation update to fit initial window size
    glViewport(0, 0, this->width, this->height);
}


// Sets the background color
void Application::setBackgroundColor(GLfloat * color)
{
    this->backgroundColor[0]=color[0];
    this->backgroundColor[1]=color[1];
    this->backgroundColor[2]=color[2];
    this->backgroundColor[3]=color[3];
    glClearColor(color[0], color[1], color[2], color[3]);
}

void Application::fonction_change_background(){
	changeColor(this->backgroundColor);
	setBackgroundColor(this->backgroundColor);
}

// Prints the frame duration and framerate (both averaged over 10 frames)
void Application::printFPS()
{
    float milliseconds=((float)(this->frameDuration)/1000.0);
    float FPS=1000.0/milliseconds; // Unit : [s^-1]= 1/seconds
    std::cout<<"frame : "<<milliseconds<<" ms       FPS : "<<FPS<<std::endl;
    
    // To print fps in the title bar
    //std::ostringstream title; title.precision(3); title.width(5);
    //title<<"frame : "<<milliseconds<<" ms       FPS : "<<FPS;
    //SDL_WM_SetCaption(title.str().data(), 0);
}


// Distributes task for the "user" kind of events 
// For example : rendrFrame action occurs if a timer event is passed
void Application::handleUserEvent(SDL_Event& event)
{
    int eventID=event.user.code;
    

  if (eventID==animate2EventID)
        animate2();
  
}

// Distributes task for the "key" kind of events 
// For example : std::cout when b key is pressed
// down is true when the key is pressed, false when released
void Application::handleKeyEvent(SDL_keysym& keysym, bool down)
{
 
 if (down)
    {
        switch(keysym.sym)
        {
        	case SDLK_ESCAPE:
          		this->done=true;
          	break;
          	
          	case SDLK_f :
          	    std::cout<<"Key \"f\" was pressed."<<std::endl;
          	    printFPS();
          	break;

		default:break;
	
      	}
    }
 if (!down)
    {
	 switch(keysym.sym)
         {
          default:break;
	 }
}

float unit=-1.0;
if (down) unit=1.0;

switch (keysym.sym)
{
	case SDLK_UP:
	case SDLK_z://forward
		this->moveFlags[2]-=unit;
	break;

	case SDLK_DOWN:
	case SDLK_s://backward
		this->moveFlags[2]+=unit;
	break;
	
	case SDLK_LEFT:
	case SDLK_q://left
		this->moveFlags[0]-=unit;
	break;

	case SDLK_RIGHT:	
	case SDLK_d://right
		this->moveFlags[0]+=unit;
	break;

	case SDLK_PAGEDOWN:
	case SDLK_c://down
		this->moveFlags[1]-=unit;
	break;

	case SDLK_PAGEUP:
	case SDLK_SPACE://up
		this->moveFlags[1]+=unit;
	break;

	default:
	break;

}

}

/*--------------------------------------
QUESTION H - DEPLACEMENT DE LA SOURIS ET CHANGEMENT DE COULEUR
--------------------------------------*/
void Application::handleMouseMotionEvent(Uint16 x, Uint16 y)
{
   //std::cout<<"Position de la souris X:" << x << " et Y:" << y <<std::endl;
	//http://www.siteduzero.com/tutoriel-3-4988-controle-avance-de-la-camera-partie-2-2.html
	//http://www.swiftless.com/tutorials/opengl/camera2.html
	
	//En position relative : (Utiliser Uint16 x et Uint16 y en parametres)	
	//GLfloat changeRed = fabs((2*(abs(x)/100.0))-1); //composante R changeante (Bornes entre 0 et 1)
	//GLfloat changeGreen = fabs((2*(abs(y)/100.0))-1); //composante V changeante (Bornes entre 0 et 1)
	/*EXPLICATION CALCUL*/
	/*On divise la valeur absolue de la position relative de la souris par 100 (valeur qui semble marcher au mieux) afin de la ramener entre 0 et 1 */
	/*On la multiplie par 2 pour la borner entre 0 et 2, puis on enleve 1 pour qu'elle soit comprise entre -1 et 1*/
	/*Enfin, on prend la valeur absolue de la valeur calculé pour arriver avec un nombre entre 0 et 1*/

	//En position absolue	
	/*GLfloat changeRed = fabs((2*(abs(x)/800.0))-1); //composante R changeante (Bornes entre 0 et 1)
	GLfloat changeGreen = fabs((2*(abs(y)/600.0))-1); //composante V changeante (Bornes entre 0 et 1)
		std::cout<<"COULEUR1:" << changeRed <<std::endl;
		std::cout<<"COULEUR2:" << changeGreen <<std::endl;
	GLfloat color[]={changeRed, changeGreen, 0.0, 1.0}; //Couleur dynamique
	
	for (unsigned int i=0;i<=this->scene->nbStoredObjects;i++){//Pour tous les objets de la scene ...
		this->scene->setDrawnObjectColor(i, color);
	}*/



}


// Listens to events during the whole time of the application
// and distributes corresponding tasks
void Application::handleEvent(SDL_Event& event)
{
    switch(event.type) 
    {
        // User events
        case SDL_USEREVENT:
            handleUserEvent(event);
            break;
                
        // Key presses
        case SDL_KEYDOWN:
            handleKeyEvent(event.key.keysym, true);
            break;
                
        // Key releases    
        case SDL_KEYUP:
            handleKeyEvent(event.key.keysym, false);
            break;                  
            
        // Quit event (for example sent when the window is closed)
        case SDL_QUIT:
            this->done=true;
            break;
	
		//Question H (MouseMotion)
		case SDL_MOUSEMOTION:
			handleMouseMotionEvent(event.motion.x,event.motion.y);
            break;

	// Window resizeSDL_Event& event
        case  SDL_VIDEORESIZE:
            this->windowedWidth=event.resize.w;
            this->windowedHeight=event.resize.h;
            resize(this->windowedWidth, this->windowedHeight);
            break;
            
                
        default:
            break;
    }
}



// Clears the current frame buffer (the image on screen) 
// draws the scene in the other available frame buffer (double buffering)
// and prints the new filled frame buffer on screen
void Application::renderFrame()
{
	// Clears the window with current clearing color, clears also the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draws this->scene
	if (this->scene!=NULL)
    	this->scene->drawObjectsOfScene();
    
    // Performs the buffer swap between the current shown buffer, 
    // and the one we just worked on
    SDL_GL_SwapBuffers();
    
    // Gets the average frame duration over 20 frames
    this->cntFrame++;
    if (this->cntFrame%20==0)
    {
        uint64_t time=getTime();
        this->frameDuration=(time-this->lastStartTime)/20LL;
        this->lastStartTime=time;
    }
    
    // Reports any possible glError
    //std::cout<<"renderFrame error"<<std::endl;
    printGlErrors();
}


// Listens to events during the whole time of the application
// and distributes corresponding tasks
void Application::loop()
{
    SDL_Event event;
    
    // While the application is running (this->done==false), 
    while (!this->done)
    {
        // SDL_PollEvent return 1 when there is still events on the stack of events
        // It takes the first event in the queue and writes it in the "event" parameter
        // Then it removes it from the stack.
        while (SDL_PollEvent(&event))
            handleEvent(event); // Checks each event for types and loach corresponding actions
        // Renders the frame
        renderFrame();
    }
}

void Application::moveFPS()
{

	//Moving camera according to mouse moves
	SDL_PumpEvents();
	int mouseRelX, mouseRelY;

	SDL_GetRelativeMouseState(&mouseRelX, &mouseRelY);

	this->xMousePosition+=2.0*mouseRelX/(GLfloat) this-> width;
	this->yMousePosition+=-2.0*mouseRelY/(GLfloat) this-> height;
	GLfloat moveStep = 0.02;
	GLfloat cameraNewPos[3];
	GLfloat moveOnX= this->moveFlags[0]*moveStep;
	GLfloat moveOnY= this->moveFlags[1]*moveStep;
	GLfloat moveOnZ= this->moveFlags[2]*moveStep;

	for(GLuint iCoord=0; iCoord<3; ++iCoord)
	{
		cameraNewPos[iCoord]= this->scene->camera->c[iCoord]
		+this->scene->camera->x[iCoord]*moveOnX
		+this->scene->camera->y[iCoord]*moveOnY
		+this->scene->camera->z[iCoord]*moveOnZ;
	}

	GLfloat angleForWindowWidth= M_PI;
	GLfloat angleForWindowHeight= M_PI/2.0;
	GLfloat angleLong= this->xMousePosition*angleForWindowWidth;
	GLfloat angleLat= this->yMousePosition*angleForWindowHeight;

	//Method with rotates

	GLfloat xAxis[]={1.0, 0.0, 0.0};
	GLfloat yAxis[]={0.0, 1.0, 0.0};
	GLfloat rotateAroundX[16];
	setToRotate(rotateAroundX, -angleLat, xAxis);

	GLfloat rotateAroundY[16];
	setToRotate(rotateAroundY, angleLong, yAxis);

	GLfloat t[]={-cameraNewPos[0], -cameraNewPos[1], -cameraNewPos[2]};
	GLfloat translate[16];
	setToTranslate(translate, t);
	setToIdentity(this->scene->camera->view);
	multMatrixBtoMatrixA(this->scene->camera->view, rotateAroundX);
	multMatrixBtoMatrixA(this->scene->camera->view, rotateAroundY);
	multMatrixBtoMatrixA(this->scene->camera->view, translate);

	for(GLuint iCoord=0; iCoord<3; ++iCoord)
	{
	//Updates the axis with values in view

		this->scene->camera->x[iCoord]= this->scene->camera->view[iCoord*4+0];
		this->scene->camera->y[iCoord]= this->scene->camera->view[iCoord*4+1];
		this->scene->camera->z[iCoord]= this->scene->camera->view[iCoord*4+2];

	//Updates the position of the camera c
		this->scene->camera->c[iCoord]= cameraNewPos[iCoord];
	}	
	



	
}

// Animates an object
void Application::animate2()
{

	moveFPS();

	updateBoids(oiseau);
}



