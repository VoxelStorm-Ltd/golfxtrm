#ifndef GLOBALVARS_CLIENT_H_INCLUDED
#define GLOBALVARS_CLIENT_H_INCLUDED

#include <GL/glfw.h>
#include "globaldefs.h"

// user settings
extern int windowwidth;
extern int windowheight;
extern bool fullscreen;
extern int mouseinvert;
extern double camyawperpixel;
extern double campitchperpixel;
extern double fpsmin;
extern double fpsmax;
extern double fpscap;
extern double timedeltamax;
extern double timedeltamin;
extern double timedeltamincap;
extern double camfov;

// non-editable interface settings
extern double rotations_per_tick;
extern double walkspeed;
extern double walkrunspeed;
extern double strafespeed;
extern double straferunspeed;
extern double camfriction;
extern double cammu;

// non-editable world settings (later server-fed)
extern GLfloat fogcolour[];
extern float fogred;
extern float foggreen;
extern float fogblue;
extern float skyred;
extern float skygreen;
extern float skyblue;
extern float ambientred;
extern float ambientgreen;
extern float ambientblue;

extern GLfloat sundirection[];

// dynamic global variables
extern bool keeprunning;

extern double timelastrenderstart;
extern double timelastrenderend;
extern double timelasttickstart;
extern double timelasttickend;
extern double timelasttotal;
extern double timedeltarender;
extern double timedeltatick;
extern double timedeltatotal;
extern double timedeltaaverage;

extern inputmodetype inputmode;

extern double camposx;
extern double camposy;
extern double camposz;
extern double camposxlast;
extern double camposylast;
extern double camposzlast;

extern double camspeedx;
extern double camspeedy;
extern double camspeedz;
extern double campitch;
extern double camyaw;
extern double camnearplane;
extern double camfarplane;

extern int mousex;
extern int mousey;

#endif // GLOBALVARS_CLIENT_H_INCLUDED
