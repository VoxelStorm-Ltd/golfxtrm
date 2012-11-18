#ifndef GLOBALVARS_CLIENT_H_INCLUDED
#define GLOBALVARS_CLIENT_H_INCLUDED

#include <GL/glew.h>
#include <GL/glfw.h>
#include "globaldefs.h"

// user settings
int windowwidth = 1000;
int windowheight = 750;
bool fullscreen = false;
int mouseinvert = 1;      // multiplier: -1 to invert
double camyawperpixel   = 8;     // mouse sensitivity
double campitchperpixel = 8;
double fpsmin = 25;         // minimum requested fps
double fpsmax = 30;         // maximum requested fps
double fpscap = 60;         // fps we don't want to ever exceed
double timedeltamax = 1/fpsmin; // decrease detail until we hit this time
double timedeltamin = 1/fpsmax; // increase detail until we hit this time
double timedeltamincap = 1/fpscap; // if we complete rendering before, just sleep
double updatefreq = 20;     // how many times to update the world per second

double timesetspeed = 120;
double physicsspeed = 1;

// non-editable interface settings
double rotations_per_tick = .02;

// non-editable world settings (later server-fed)
//GLfloat fogcolour[] = {0.85, 0.85, 0.9, 1};
float fogred   = 0.98;
float foggreen = 0.92;
float fogblue  = 0.50;
float skyred   = 0.90;
float skygreen = 0.95;
float skyblue  = 0.67;
float ambientred   = 0.7;
float ambientgreen = 0.7;
float ambientblue  = 0.7;

/*float fogred   = 0.98;
float foggreen = 0.92;
float fogblue  = 0.9;
float skyred   = 0.90;
float skygreen = 0.95;
float skyblue  = 1.0;*/

// backwards compatibility
bool hasvao = true;       // whether to disable use of vertex array objects

// dynamic global variables
bool keeprunning = true;

double timelastrenderstart = glfwGetTime(); // last render started at
double timelastrenderend   = glfwGetTime(); // last render ended at
double timelasttickstart   = glfwGetTime(); // last game tick started at
double timelasttickend     = glfwGetTime(); // last game tick ended at
double timelasttotal       = glfwGetTime(); // last main loop roll ended at
double timedeltarender  = 0.005; // time spent rendering the last frame
double timedeltatick    = 0.005; // time spent calculating the last tick
double timedeltatotal   = 0.01;  // time taken between two complete loops (1/FPS)
double timedeltaaverage = 0.01;  // rolling average of the delta time

inputmodetype inputmode = INPUTMODE_MOVING_HEAD;

double campitch = 0;
double camyaw   = 0;
double camfov = 1;          // field of view - larger numbers are wider
double camnearplane = 0.1;  // default near and far clipping planes
double camfarplane  = 1000;

int mousex = 0;       // track mouse movement from screen centre
int mousey = 0;

#endif // GLOBALVARS_CLIENT_H_INCLUDED
