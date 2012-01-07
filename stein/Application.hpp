// Application.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include "GLHeaders.hpp"
#include "Boid.hpp"

// Windowing system SDL
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

// Forward declaration
struct Scene;


// All the initialisation of states and events for SDL and OpenGL
class Application
{
    public:
	Boids * oiseau;

        SDL_TimerID animate2Timer;
        int animate2EventID;                     // ID of the animation timer
	int moveEventID;
        
	    SDL_Surface* drawContext;

	    unsigned int windowedWidth;             // Window dimentions when not fullscreen - horizontal axis
	    unsigned int windowedHeight;            // Window dimentions when not fullscreen - vertical axis
	    unsigned int fullScreenWidth;           // Screen dimentions - horizontal axis
	    unsigned int fullScreenHeight;          // Screen dimentions - vertical axis	
	    unsigned int width;                     // Window actual dimentions - horizontal axis
	    unsigned int height;                    // Window actual dimentions - vertical axis
	    
        Uint32 videoModeFlags;
        
	    GLfloat xMousePosition;                 // Mouse position - horizontal axis (-1 to 1)
	    GLfloat yMousePosition;                 // mouse position - vertical axis (-1 to 1)
	    GLfloat xMouseLeftDownPosition;         // Mouse position - updated only when left button down
	    GLfloat yMouseLeftDownPosition;         // mouse position - updated only when left button down
	    int scroll;                             // scroll value (up : ++, down : --)
	    
	    GLfloat moveFlags[3];
	    bool mouse;                             // True if mouse is seeable
	    bool moveForward;
	    bool moveBackward;
 	    bool moveLeft;
            bool moveRight;
	
	    GLuint cntFrame;                        // Frame counter
	    uint64_t lastStartTime;                 // Time updated every 10 frames
	    uint64_t frameDuration;                 // Frame time duration
	    
	    GLuint cntAnimation;                         // Move counter
	    bool done;                              // True when the window is closed to end the application
        Scene * scene;                          // Scene to draw
        
        GLfloat backgroundColor[4];             // Color of the background;

        Application();
        ~Application();
        
        void init();
        void initSDLOpenGL();
        void customizeStates();
             
        void setScene(Scene * scene);
        
	//Question B
	void fonction_wireframe();

        void initTimers();
	
        void resize(GLuint w, GLuint h);
        void setBackgroundColor(GLfloat * color);
	void fonction_change_background(); //Question C
        void printFPS();

	void moveFPS();
        
        void handleUserEvent(SDL_Event& event);
        void handleKeyEvent(SDL_keysym& keysym, bool down);
	void handleMouseMotionEvent(Uint16 x, Uint16 y);
        void handleEvent(SDL_Event& event);	

        void renderFrame();
        
        void loop();

        void animate2();

	private:
	    Mix_Music* Music;
};

Uint32 genericTimer(Uint32 interval, void* param);

#endif //__APPLICATION_HPP__ 
