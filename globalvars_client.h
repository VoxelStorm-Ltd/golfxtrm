#ifndef GLOBALVARS_CLIENT_H_INCLUDED
#define GLOBALVARS_CLIENT_H_INCLUDED

#include <GL/glfw.h>

#include "globaldefs.h"

// user settings
int windowwidth = 800;
int windowheight = 600;
bool fullscreen = false;
int mouseinvert = 1;      // multiplier: -1 to invert
double camyawperpixel   = 10;     // mouse sensitivity
double campitchperpixel = 10;
double fpsmin = 25;         // minimum requested fps
double fpsmax = 30;         // maximum requested fps
double fpscap = 60;         // fps we don't want to ever exceed
double timedeltamax = 1/fpsmin; // decrease detail until we hit this time
double timedeltamin = 1/fpsmax; // increase detail until we hit this time
double timedeltamincap = 1/fpscap; // if we complete rendering before, just sleep
double camfov = 0.5;    // field of view - larger numbers are wider

// non-editable interface settings
double rotations_per_tick = .02;
double walkspeed = 100;
double walkrunspeed = 20000;
double strafespeed = 80;
double straferunspeed = 15000;
double camfriction = 0.99;   // multiplier for speed each physics tick
double cammu = 0.05;         // threshold to stop moving things moving

// non-editable world settings (later server-fed)
GLfloat fogcolour[] = {0.85, 0.85, 0.9, 1};
float skyred   = 0.6;
float skygreen = 0.6;
float skyblue  = 0.6;
float ambientred   = 0.0;
float ambientgreen = 0.0;
float ambientblue  = 0.5;

GLfloat sundirection[] = {0.5,1,0.25,0};

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

double camposx = 0;
double camposy = 1.6095;  // average human standing eye height
double camposz = 0;
double camposxlast = camposx;   // for collision reaction
double camposylast = camposy;
double camposzlast = camposz;

double camspeedx = 0;
double camspeedy = 0;
double camspeedz = 0;
double campitch = 0;
double camyaw   = 0;
double camnearplane = 0.5;  // default near and far clipping planes
double camfarplane  = 30000000;

int mousex = 0;       // track mouse movement from screen centre
int mousey = 0;

#endif // GLOBALVARS_CLIENT_H_INCLUDED
