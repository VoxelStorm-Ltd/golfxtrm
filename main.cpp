#include <cstdlib>
#include <iostream>
//#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <math.h>
#define _USE_MATH_DEFINES
#include "version.h"

#include <GL/glfw.h>
#include "vmath.h"

#include "globaldefs.h"
#include "globalvars_client.h"
#include "terrain.h"
#include "worldcomponents.h"
#include "golfer.h"
#include "holdable.h"

//#include "progressbar.h"



using namespace std;

void login();
void init();
void shutdown(int return_code);
void mainloop();

void controls();
void GLFWCALL controlcallback(int key, int action);
void physics(world *thisplanet);
void draw();

universe *root;             // this holds everything
golfer *player;             // the golfer entity that we control

int main() {
  init();
  mainloop();
  shutdown(0);
}

void init() {       /// all the one-time initialisation we need for the engine
  // initialise the opengl window
  if(glfwInit() != GL_TRUE) shutdown(1);
  // 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
  if(glfwOpenWindow(windowwidth, windowheight, 8, 8, 8, 0, 8, 0, GLFW_WINDOW) != GL_TRUE) shutdown(1);

  GLFWvidmode desktopmode;
  glfwGetDesktopMode(&desktopmode);
  short winfinalposx = (desktopmode.Width  / 2) - (windowwidth  / 2);
  short winfinalposy = (desktopmode.Height / 2) - (windowheight / 2);

  glfwSetWindowPos(winfinalposx,winfinalposy);

  char titleprefix[] = "GolfXTRM alpha ";
  char titlestring[100];
  strcpy(titlestring,titleprefix);
  strcat(titlestring,AutoVersion::FULLVERSION_STRING);
  strcat(titlestring," built ");
  strcat(titlestring,AutoVersion::DATE);
  strcat(titlestring,"/");
  strcat(titlestring,AutoVersion::MONTH);
  strcat(titlestring,"/");
  strcat(titlestring,AutoVersion::YEAR);
  //glfwSetWindowTitle(titlestring);
  glfwSetWindowTitle("GolfXTRM alpha: Loading...");

  // set the projection matrix to a normal frustum with a max depth of 5000
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect_ratio = ((float)windowheight) / windowwidth;
  glFrustum(-camfov, camfov, -camfov * aspect_ratio, camfov * aspect_ratio, camnearplane, camfarplane);
  glMatrixMode(GL_MODELVIEW);

  glFrontFace(GL_CCW);      // set up counter-clockwise polygon winding
  //glCullFace(GL_BACK);      // may be redundant to cull back-faces
  glEnable(GL_DEPTH_TEST);  // go on, use the zbuffer
  glEnable(GL_DITHER);      // may marginally increase shading quality
  glEnable(GL_LIGHTING);    // obviously we want lighting... right?
  //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);  // enable local lighting
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);  // to make flat shading ok
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);  // ensure one-sided lighting

  //glEnable(GL_MINMAX);      // allow min and max colour tables for HDR effects

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //filled
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //wireframe
  //glShadeModel(GL_SMOOTH);  //SMOOTH or FLAT
  glShadeModel(GL_FLAT);    //may look more spectacular for a cube world

  /*// fog settings
  glEnable(GL_FOG);
  //GLfloat fogcolour[] = {0.15, 0.15, 0.1, 1};
  glFogfv(GL_FOG_COLOR, fogcolour);
  glFogi(GL_FOG_MODE, GL_EXP); // GL_LINEAR GL_EXP GL_EXP2
  glFogf(GL_FOG_DENSITY, 0.001);  // only used for exponential fog
  glFogi(GL_FOG_START, 10);     // only used for linear fog
  glFogi(GL_FOG_END, 4000);*/

  /*// temporary material definition
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);*/

  // set up some lights
  //GLfloat ambientlightcol[] = {.25,0,0,1};
  GLfloat ambientlightcol[] = {ambientred,ambientgreen,ambientblue,1};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientlightcol); //global ambient
  //glLightfv(GL_LIGHT0, GL_AMBIENT, ambientlightcol);
  GLfloat directionallightdiff[] = {0.8,0.7,0.5,1};
  glLightfv(GL_LIGHT0, GL_DIFFUSE, directionallightdiff);
  GLfloat directionallightspec[] = {1,1,0,1};
  glLightfv(GL_LIGHT0, GL_SPECULAR, directionallightspec);
  glEnable(GL_LIGHT0);
  //glLightfv(GL_LIGHT1, GL_POSITION, directionallightdir);
  //glLightfv(GL_LIGHT1, GL_DIFFUSE, directionallightdiff);
  //glLightfv(GL_LIGHT1, GL_SPECULAR, directionallightspec);
  //glEnable(GL_LIGHT1);

  srand(1337);   // seed the random generator predictably

  // create and populate the universe
  glfwSetWindowTitle("GolfXTRM alpha: Growing grass...");
  root = new universe();
  root->addplanet(0);


  glfwSetWindowTitle(titlestring);  // set the title to the main run's title

  glfwSetKeyCallback(controlcallback);    // activate the control callback
  glfwEnable(GLFW_KEY_REPEAT);            // enable key repeats

  glfwEnable(GLFW_STICKY_KEYS);     // capture all keystrokes even if we're slow
  glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);  // and clicks
  glfwDisable(GLFW_MOUSE_CURSOR);         // hide the mouse

  // these must be absolutely last:
  glfwSetMousePos(windowwidth/2, windowheight/2);   //centre the mouse before the main loop
  glfwSetTime(0.0);   //reset the timer for the start of the main loop

  cout << "Initialisation complete." << endl;
}

void shutdown(int return_code) {  /// close everything gracefully before exit
  cout << "Exiting on command - have a nice day!" << endl;
  glfwTerminate();  // shut down the window
  exit(return_code);
}


void mainloop() {   /// the main rendering loop
  while(keeprunning) {                                  // cheap infinite loop
    timelasttickstart = glfwGetTime();

    controls(); // run the control polling loop, if it's time

    physics(root->planet[0]);  // run the physics for this tick, if it's time

    timelasttickend = glfwGetTime();
    timedeltatick = timelasttickend - timelasttickstart;          // tick delta

    // render the frame, if it's time to do so
    timelastrenderstart = glfwGetTime();
    draw();                                                 // do the rendering
    timelasttotal = timelastrenderend;
    timelastrenderend = glfwGetTime();
    timedeltarender = timelastrenderend - timelastrenderstart;  // render delta
    timedeltatotal = timelastrenderend - timelasttotal;         // total delta
    timedeltaaverage = (timedeltaaverage + timedeltatotal) / 2;  // update the rolling average

    // LOD and time adjustments
    double timetowait = 0;
    if(timedeltatotal < timedeltamincap) {      // exceeding the fps limit
      timetowait = timedeltamincap - (glfwGetTime() - timelasttickstart);
      glfwSleep(timetowait);
    }

    //cout << "FPS " << (int)(1 / timedeltatotal) << " Dt: " << (int)(timedeltatick*100/timedeltatotal) << "% Dr: " << (int)(timedeltarender*100/timedeltatotal) << " D " << timedeltaaverage << " Slept " << timetowait << "ms" << endl;
    //cout << "Coords X:" << (int)camposx << " Y:" << (int)camposy <<" Z:" << (int)camposz << " camyaw: " << camyaw << " campitch: " << campitch << endl;
  }
}


void controls() {
  short keyspressed = 0;
  double camaccelx = 0;
  double camaccely = 0;
  double camaccelz = 0;
  double delta_move, delta_strafe;

  if(glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS) {              // shift to walk
    delta_move   = timedeltatotal * walkrunspeed;
    delta_strafe = timedeltatotal * straferunspeed;
  } else {
    delta_move   = timedeltatotal * walkspeed;
    delta_strafe = timedeltatotal * strafespeed;
  }

  if(glfwGetKey('W') == GLFW_PRESS) {                  // wasd for movement
    camaccelx =  sin(camyaw*M_PI/180) * delta_move;
    camaccelz = -cos(camyaw*M_PI/180) * delta_move;
    keyspressed++;
  }
  if(glfwGetKey('S') == GLFW_PRESS) {
    camaccelx -= sin(camyaw*M_PI/180) * delta_strafe; // you can't run as fast
    camaccelz += cos(camyaw*M_PI/180) * delta_strafe; // backwards as forwards
    keyspressed++;
  }
  if(glfwGetKey('A') == GLFW_PRESS) {
    camaccelx -= cos(camyaw*M_PI/180) * delta_strafe;
    camaccelz -= sin(camyaw*M_PI/180) * delta_strafe;
    keyspressed++;
  }
  if(glfwGetKey('D') == GLFW_PRESS) {
    camaccelx += cos(camyaw*M_PI/180) * delta_strafe;
    camaccelz += sin(camyaw*M_PI/180) * delta_strafe;
    keyspressed++;
  }
  if(glfwGetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {      //jump/fly up
    camaccely += delta_move;
  }
  if(glfwGetKey('X') == GLFW_PRESS) {                 //crouch/fly down
    camaccely -= delta_move;
  }

  if(keyspressed > 1) {     //allow for relatively smooth diagonal strafe
    camaccelx /= keyspressed;
    camaccelz /= keyspressed;
  }

  camspeedx = camaccelx;
  camspeedy = camaccely;
  camspeedz = camaccelz;

  // convert mouse movements to camera rotation
  glfwGetMousePos(&mousex, &mousey);
  camyaw = camyaw + ((mousex-(windowwidth/2)) * camyawperpixel * timedeltatotal);
  campitch = campitch + ((mousey-(windowheight/2)) * campitchperpixel * mouseinvert * timedeltatotal);
  glfwSetMousePos(windowwidth/2, windowheight/2);   //reset the mouse immediately after
  if(camyaw > 360)              //wrap the camera yaw angle
    camyaw = camyaw - 360;
  else if(camyaw < 0)
    camyaw = camyaw + 360;
  if(campitch > 85)             //and clamp the camera pitch angle
    campitch = 85;
  else if(campitch < -85)
    campitch = -85;
}

void GLFWCALL controlcallback(int key, int action) {
  if(action == GLFW_PRESS) {   // here are all the down-presses we care about
    if(key == GLFW_KEY_ESC) {         // escape to quit
      keeprunning = false;
      cout << "Stop requested..." << endl;
    }
  }
}

void physics(world *thisplanet) {   /// update entity and player locations

  camposxlast = camposx;
  camposylast = camposy;
  camposylast = camposy;

  // camera movements
  camposx += camspeedx * timedeltatotal;     //inertial motion
  camposy += camspeedy * timedeltatotal;
  camposz += camspeedz * timedeltatotal;

  //camspeedx *= camfriction;           //primitive friction
  //camspeedy *= camfriction;
  //camspeedz *= camfriction;

  if(fabs(camspeedx) < cammu) camspeedx = 0;    //minimum bound clamping
  if(fabs(camspeedy) < cammu) camspeedy = 0;
  if(fabs(camspeedz) < cammu) camspeedz = 0;
}

void draw() {
  // clear the buffer
  glClearColor(skyred, skygreen, skyblue, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset view matrix
  glLoadIdentity();

  // rotate the view
  glRotatef(campitch, 1, 0, 0);
  glRotatef(camyaw,   0, 1, 0);

  // take us to the current coords
  glTranslated(-camposx, -camposy, -camposz);

  // light the scene
  glLightfv(GL_LIGHT0, GL_POSITION, sundirection);

  // tell the universe to go render itself
  root->render();

  // do the buffer shuffle
  glfwSwapBuffers();
}
