#include <cstdlib>
#include <iostream>
//#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <math.h>
#define _USE_MATH_DEFINES
#include "version.h"

#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/gl.h>
#include "vmath.h"

#include "globaldefs.h"
#include "globalvars_client.h"
#include "terrain.h"
#include "worldcomponents.h"
#include "golfer.h"
#include "holdable.h"

#include "global_objects.h"

//#include "progressbar.h"



using namespace std;

void login();
void init();
void shutdown(int return_code, string errorstring);
void mainloop();

void controls();
void GLFWCALL controlcallback(int key, int action);
void physics(double timedelta);
void draw();

int main() {
  init();
  mainloop();
  shutdown(0, "");
}

void init() {       /// all the one-time initialisation we need for the engine
  // initialise the opengl window
  if(glfwInit() != GL_TRUE) shutdown(1, "GLFW failed to initialise");
  if(glfwOpenWindow(windowwidth, windowheight, 8, 8, 8, 0, 8, 0, GLFW_WINDOW) != GL_TRUE) shutdown(1, "GLFW failed to open a window");

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

  // globalise the opengl extensions we want to use
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK) shutdown(1, "GLEW failed to initialise");

  // set up the frustum
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect_ratio = ((float)windowheight) / windowwidth;
  glFrustum(-camfov * camnearplane,
            camfov * camnearplane,
            -camfov * camnearplane * aspect_ratio,
            camfov  * camnearplane * aspect_ratio,
            camnearplane, camfarplane);
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
  //glPolygonMode(GL_FRONT, GL_FILL);  //filled
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //wireframe
  //glShadeModel(GL_SMOOTH);  //SMOOTH or FLAT
  glShadeModel(GL_FLAT);    //may look more spectacular for a cube world

  // fog settings
  glEnable(GL_FOG);
  GLfloat fogcolour[] = {fogred, foggreen, fogblue, 1};
  glFogfv(GL_FOG_COLOR, fogcolour);
  glFogi(GL_FOG_MODE, GL_LINEAR); // GL_LINEAR GL_EXP GL_EXP2
  glFogf(GL_FOG_DENSITY, 0.03);  // only used for exponential fog
  glFogi(GL_FOG_START, 2);     // only used for linear fog
  glFogi(GL_FOG_END, 100);

  /*// temporary material definition
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);*/
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  // set up some lights
  GLfloat ambientlightcol[] = {ambientred, ambientgreen, ambientblue, 1};
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
  root = new universe();      // create the global universe object
  root->addplanet(0);         // populate it with a default planet
  player = new golfer(root->planet[0]->course[0], 0, 0, 0);   // our player
  player->bodyyaw = 155;
  golfer *caddy = new golfer(root->planet[0]->course[0], 5, 0, 8); // the caddy
  caddy->bodyyaw = -15;

  glfwSetWindowTitle(titlestring);  // set the title to the main run's title

  glfwSetKeyCallback(controlcallback);    // activate the control callback
  glfwEnable(GLFW_KEY_REPEAT);            // enable key repeats

  glfwEnable(GLFW_STICKY_KEYS);     // capture all keystrokes even if we're slow
  glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);  // and clicks
  glfwDisable(GLFW_MOUSE_CURSOR);         // hide the mouse

  //glfwSwapInterval(1);    // activate vsync

  // these must be absolutely last:
  glfwSetMousePos(windowwidth/2, windowheight/2);   //centre the mouse before the main loop
  glfwSetTime(0.0);   //reset the timer for the start of the main loop

  cout << "Initialisation complete." << endl;
}

void shutdown(int return_code, string errorstring) {  /// close everything gracefully before exit
  if(return_code == 0) {
    cout << "Exiting on command - have a nice day!" << endl;
  } else {
    cout << "Fatal error: " << errorstring << endl;
  }
  glfwTerminate();  // shut down the window
  exit(return_code);
}


void mainloop() {   /// the main rendering loop
  while(keeprunning) {                                  // cheap infinite loop
    timelasttickstart = glfwGetTime();

    controls(); // run the control polling loop, if it's time

    timelasttickend = glfwGetTime();
    timedeltatick = timelasttickend - timelasttickstart;          // tick delta

    // render the frame, if it's time to do so
    timelastrenderstart = glfwGetTime();
    draw();                                                 // do the rendering
    timelasttotal = timelastrenderend;
    timelastrenderend = glfwGetTime();
    timedeltarender = timelastrenderend - timelastrenderstart;  // render delta
    timedeltatotal = timelastrenderend - timelasttotal;         // total delta
    //timedeltaaverage = (timedeltaaverage + timedeltatotal) / 2;  // update the rolling average
    timedeltaaverage = ((timedeltaaverage * 100) + timedeltatotal) / 101;  // update the rolling average

    physics(timedeltatotal);  // run the physics for this tick, if it's time

    // framerate capping - don't do this if we're using vsync
    /*double timetowait = 0;
    if(timedeltatotal < timedeltamincap) {      // exceeding the fps limit
      timetowait = timedeltamincap - timedeltatotal;
      glfwSleep(timetowait);
    }*/

    //cout << "FPS " << (int)(1 / timedeltaaverage) << " Dt: " << (int)(timedeltatick*100/timedeltatotal) << "% Dr: " << (int)(timedeltarender*100/timedeltatotal) << " D " << timedeltaaverage << " Slp " << timetowait << "s Ttl " << glfwGetTime() - timelasttickstart << " Trg " << timedeltamincap << endl;
    //cout << "Coords " << (int)player->bodyposition.x << ":" << (int)player->bodyposition.y << ":" << (int)player->bodyposition.z << endl;
  }
}


void controls() {
  short keyspressed = 0;
  double thismoveforce = 0;
  double thisstrafeforce = 0;
  golfer::golferstatetype movestate;

  if(glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS) {              // shift to sprint
    thismoveforce   = player->maxforce_run;
    thisstrafeforce = player->maxforce_runstrafe;
    movestate = golfer::GOLFER_RUNNING;
  } else {
    thismoveforce   = player->maxforce_walk;
    thisstrafeforce = player->maxforce_walkstrafe;
    movestate = golfer::GOLFER_WALKING;
  }

  player->state = golfer::GOLFER_STANDING;
  if(glfwGetKey('W') == GLFW_PRESS) {                  // wasd for movement
    player->moveforce.x += sin(player->bodyyaw * M_PI / 180) * thismoveforce;
    player->moveforce.z -= cos(player->bodyyaw * M_PI / 180) * thismoveforce;
    keyspressed++;
    player->state = movestate;
  }
  if(glfwGetKey('S') == GLFW_PRESS) {
    player->moveforce.x -= sin(player->bodyyaw * M_PI / 180) * thisstrafeforce; // you can't run as fast
    player->moveforce.z += cos(player->bodyyaw * M_PI / 180) * thisstrafeforce; // backwards as forwards
    keyspressed++;
    player->state = movestate;
  }
  if(glfwGetKey('A') == GLFW_PRESS) {
    player->moveforce.x -= cos(player->bodyyaw * M_PI / 180) * thisstrafeforce;
    player->moveforce.z -= sin(player->bodyyaw * M_PI / 180) * thisstrafeforce;
    keyspressed++;
    player->state = movestate;
  }
  if(glfwGetKey('D') == GLFW_PRESS) {
    player->moveforce.x += cos(player->bodyyaw * M_PI / 180) * thisstrafeforce;
    player->moveforce.z += sin(player->bodyyaw * M_PI / 180) * thisstrafeforce;
    keyspressed++;
    player->state = movestate;
  }
  if(glfwGetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {      //jump/fly up
    player->state = golfer::GOLFER_JUMPING;
  }
  //if(glfwGetKey('X') == GLFW_PRESS) {                 //crouch/fly down
    // we can't actually produce downwards motion...
    //player->moveforce.y -= thismoveforce;
  //}

  if(keyspressed > 1) {     //allow for relatively smooth diagonal strafe
    player->moveforce /= keyspressed;
  }

  // convert mouse movements to camera rotation
  glfwGetMousePos(&mousex, &mousey);
  //player->headyaw += (mousex-(windowwidth / 2)) * camyawperpixel;
  //player->headpitch += (mousey-(windowheight / 2)) * campitchperpixel * mouseinvert;
  player->yawtorque   = (mousex-(windowwidth  / 2)) * camyawperpixel;
  player->pitchtorque = (mousey-(windowheight / 2)) * campitchperpixel * mouseinvert;
  glfwSetMousePos(windowwidth / 2, windowheight / 2);   //reset the mouse immediately after

  // poll mouse buttons
  if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
    if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
      // both buttons held down
      inputmode = INPUTMODE_MOVING_HEAD;
    } else {
      // LMB held down
      inputmode = INPUTMODE_MOVING_ARMS;
    }
  } else {
    if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
      // RMB held down
      inputmode = INPUTMODE_MOVING_HEAD_AND_ARMS;
    } else {
      // no buttons pressed
      inputmode = INPUTMODE_MOVING_HEAD_AND_BODY;
    }
  }
}

void GLFWCALL controlcallback(int key, int action) {
  if(action == GLFW_PRESS) {   // here are all the down-presses we care about
    if(key == GLFW_KEY_ESC) {         // escape to quit
      keeprunning = false;
      cout << "Stop requested..." << endl;
    } else if(key == 'O') {        // O and P switch render modes
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //filled
      cout << "Switched to filled render mode" << endl;
    } else if(key == 'P') {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //wireframe
      cout << "Switched to wireframe render mode" << endl;
    }
  }
}

void physics(double timedelta) {   /// update entity and player locations
  root->update(timedelta);   // carry out the global physics update
}

void draw() {
  // clear the buffer
  glClearColor(skyred, skygreen, skyblue, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset view matrix
  glLoadIdentity();

  // rotate the view
  //glRotatef(player->headpitch, 1, 0, 0);
  //glRotatef(player->headyaw,   0, 1, 0);

  // translate us from the player's viewpoint
  glTranslated(0, -player->eyeleveloffset.y, 0);
  glRotatef(player->headyaw,   0, 1, 0);
  glRotatef(player->headpitch, 1, 0, 0);
  glTranslated(0, -player->headfulcrum.y, 0);
  glRotatef(player->bodyyaw,   0, 1, 0);
  glTranslated(-player->bodyposition.x,
               -player->bodyposition.y,
               -player->bodyposition.z);

  // light the scene
  glLightfv(GL_LIGHT0, GL_POSITION, sundirection);

  // tell the universe to go render itself
  root->render();

  // do the buffer shuffle
  glfwSwapBuffers();
}
