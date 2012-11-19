#include <cstdlib>
#include <iostream>
#include <cstring>
#include <iostream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "version.h"

#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/gl.h>
#include <irrKlang.h>
#include <SOIL.h>
#include "vmath.h"

#include "globaldefs.h"
#include "globalvars_client.h"
#include "terrain.h"
#include "worldcomponents.h"
#include "landscape_features.h"
#include "golfer.h"
#include "particle.h"
#include "holdable.h"

#include "global_objects.h"
#include "objloader.h"

//#include "progressbar.h"


using namespace std;

void login();
void init(bool fullscreen, bool largewindow, bool skipintro);
void shutdown(int return_code, string errorstring);
void mainloop();

void controls();
void GLFWCALL controlcallback(int key, int action);
void physics(double timedelta);
void draw();

int main(int argc, char* argv[]) {
  bool fullscreen, largewindow, skipintro;
  fullscreen = false;
  largewindow = true;
  skipintro = false;
  cout << "Starting up " << argv[0] << " with " << argc << " arguments" << endl;
  for (int i = 1; i < argc; i++) {
    cout << "Parsing commandline option " << i << endl;
    if (i != argc) {  // Check that we haven't finished parsing already
      cout << "arg " << i << " = " << argv[i] << endl;
      string thisargstring = string(argv[i]);
      if (thisargstring == "-skipintro") {
        skipintro = true;
        cout << "  skipping intro" << endl;
      } else if (thisargstring == "-fullscreen") {
        fullscreen = true;
        largewindow = false;
      } else if (thisargstring == "-largewindow") {
        fullscreen = false;
        largewindow = true;
      } else if (thisargstring == "-smallwindow") {
        fullscreen = false;
        largewindow = false;
      } else if (thisargstring == "-novao") {  // force VAO disable
        hasvao = false;
      } else if (thisargstring == "-width") {  // manual screen size setting
        windowwidth = atoi(argv[i + 1]);
      } else if (thisargstring == "-height") {
        windowheight = atoi(argv[i + 1]);
      } else if (thisargstring == "-timespeed") {
        timesetspeed = atof(argv[i + 1]);
      } else if (thisargstring == "-golfballspeed") {
        physicsspeed = atof(argv[i + 1]);
      }
    }
  }
  #ifdef SKIPINTRO
  skipintro = true;   // skip the intro in debug mode
  #endif
  init(fullscreen, largewindow, skipintro);

  mainloop();
  shutdown(0, "");
}

void init(bool fullscreen, bool largewindow, bool skipintro) {       /// all the one-time initialisation we need for the engine
  // initialise the opengl window
  if(glfwInit() != GL_TRUE) shutdown(1, "GLFW failed to initialise");
  GLFWvidmode desktopmode;
  glfwGetDesktopMode(&desktopmode);

  if(fullscreen) {
    if(glfwOpenWindow(desktopmode.Width, desktopmode.Height, desktopmode.RedBits, desktopmode.GreenBits, desktopmode.BlueBits, 0, 24, 0, GLFW_FULLSCREEN) != GL_TRUE) shutdown(1, "GLFW failed to open a window");
  } else if(largewindow) {
    windowwidth  = desktopmode.Width  - 80;
    windowheight = desktopmode.Height - 80;
    if(windowwidth < 800) {
      windowwidth = 800;
    }
    if(windowheight < 750) {
      windowheight = 750;
    }
    if(glfwOpenWindow(windowwidth, windowheight, desktopmode.RedBits, desktopmode.GreenBits, desktopmode.BlueBits, 0, 24, 0, GLFW_WINDOW) != GL_TRUE) shutdown(1, "GLFW failed to open a window");
    int winfinalposx = (desktopmode.Width  / 2) - (windowwidth  / 2);
    int winfinalposy = 10;
    glfwSetWindowPos(winfinalposx,winfinalposy);
  } else {
    cout << "Starting in " << windowwidth << "x" << windowheight << " with bit depth " << desktopmode.RedBits << "," << desktopmode.GreenBits << "," << desktopmode.BlueBits << endl;
    if(glfwOpenWindow(windowwidth, windowheight, desktopmode.RedBits, desktopmode.GreenBits, desktopmode.BlueBits, 0, 24, 0, GLFW_WINDOW) != GL_TRUE) shutdown(1, "GLFW failed to open a window");
    int winfinalposx = (desktopmode.Width  / 2) - (windowwidth  / 2);
    int winfinalposy = (desktopmode.Height / 2) - (windowheight / 2);
    glfwSetWindowPos(winfinalposx,winfinalposy);
  }

  char titleprefix[] = "GolfXTRM beta ";
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
  glfwSetWindowTitle("GolfXTRM beta: Loading...");

  // globalise the opengl extensions we want to use
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK) shutdown(1, "GLEW failed to initialise");
  glewExperimental = GL_TRUE;
  cout << "GL_VERSION:  " << glGetString(GL_VERSION) << endl;
  cout << "GL_VENDOR:   " << glGetString(GL_VENDOR) << endl;
  cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << endl;
  //cout << "GL_ARB_vertex_array_object:     " << GL_ARB_vertex_array_object << endl;
  //cout << "GL_ATI_vertex_array_object:     " << GL_ATI_vertex_array_object << endl;
  //cout << "GL_VERTEX_ARRAY_OBJECT_AMD:     " << GL_VERTEX_ARRAY_OBJECT_AMD << endl;
  if(!GLEW_ARB_vertex_array_object) {
    cout << "GLEW_ARB_vertex_array_object not available..." << endl;
    if(!GLEW_ATI_vertex_array_object) {
      if(!GLEW_APPLE_vertex_array_object) {
        hasvao = false;
      }
    }
  }
  if(glGenVertexArrays == 0) {
    std::cout << "glGenVertexArrays does not appear to be supported." << std::endl;
    hasvao = false;
  }

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
  glFogfv(GL_FOG_COLOR, Vector4f(fogred, foggreen, fogblue, 1));
  //glFogi(GL_FOG_MODE, GL_LINEAR); // GL_LINEAR GL_EXP GL_EXP2
  glFogi(GL_FOG_MODE, GL_EXP); // GL_LINEAR GL_EXP GL_EXP2
  glFogf(GL_FOG_DENSITY, 0.02);  // only used for exponential fog
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
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Vector4f(ambientred, ambientgreen, ambientblue, 1)); //global ambient
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

  glfwSetKeyCallback(controlcallback);    // activate the control callback
  //glfwEnable(GLFW_KEY_REPEAT);            // enable key repeats
  glfwDisable(GLFW_KEY_REPEAT);            // enable key repeats

  glfwEnable(GLFW_STICKY_KEYS);     // capture all keystrokes even if we're slow
  glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);  // and clicks
  //glfwDisable(GLFW_MOUSE_CURSOR);         // hide the mouse

  glfwSwapInterval(1);    // activate vsync

  cout << "Starting sound engine... " << endl;
  soundengine = irrklang::createIrrKlangDevice();
  if(soundengine) {
    cout << "Sound engine started." << endl;
    if(!skipintro) {
    // play one of several random intro sounds
    /*srand(glfwGetTime());
    short introtune = rand() % 3;
    if(introtune == 0) {
      soundengine->play2D("GolfXTRM - Anton Riehl - The Green Flash.ogg", false);  // introductory, flutey
    } else if(introtune == 1) {
      soundengine->play2D("GolfXTRM - Anton Riehl - Fresh Dew.ogg", false);    // relieved, xylophone
    } else {
      soundengine->play2D("GolfXTRM - Anton Riehl - Amazing Lift.ogg", false); // triumphant orchestral
    }*/
    soundengine->play2D("GolfXTRM - Anton Riehl - The Green Flash.ogg", false);  // introductory, flutey
    }
  } else {
    cout << "Sound engine failed to start!" << endl;
  }

  // load and display the splash screen
  GLuint splashscreen1 = SOIL_load_OGL_texture(
    "GolfXTRM splash screen.png",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_POWER_OF_TWO
    //| SOIL_FLAG_MIPMAPS
    //| SOIL_FLAG_INVERT_Y
    //| SOIL_FLAG_MULTIPLY_ALPHA
    //| SOIL_FLAG_COMPRESS_TO_DXT
    | SOIL_FLAG_DDS_LOAD_DIRECT
    //| SOIL_FLAG_NTSC_SAFE_RGB
    //| SOIL_FLAG_CoCg_Y
    //| SOIL_FLAG_TEXTURE_RECTANGLE
  );
  float splashsize = 750 / 2;
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, splashscreen1);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);  // optimised for inverse alpha
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
    glLoadIdentity();
    glOrtho(-windowwidth/2, windowwidth/2, -windowheight/2, windowheight/2, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    {
      glLoadIdentity();
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);
      //glTranslated(0,0,-1.4);
      glBegin(GL_QUADS);
      {
        glTexCoord2f(0, 0);
        glVertex3d(-splashsize, splashsize, -1);
        glTexCoord2f(1, 0);
        glVertex3d(splashsize, splashsize, -1);
        glTexCoord2f(1, 1);
        glVertex3d(splashsize, -splashsize, -1);
        glTexCoord2f(0, 1);
        glVertex3d(-splashsize, -splashsize, -1);
      }
      glEnd();
      // back to perspective mode
      glDisable(GL_BLEND);
      glEnable(GL_DEPTH_TEST);  // go on, use the zbuffer again
      glEnable(GL_LIGHTING);
      glMatrixMode(GL_PROJECTION);
    }
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
	}
	glPopMatrix();
  glfwDisable(GLFW_AUTO_POLL_EVENTS);   // don't poll the controls on this draw
  glfwSwapBuffers();
  glBindTexture(GL_TEXTURE_2D, NULL);
  glDisable(GL_TEXTURE_2D);
  double splashtime;
  if(skipintro) {
    splashtime = 0;
  } else {
    splashtime = 12;  // how long to show it for
  }

  srand(1337);   // seed the random generator predictably

  // create and populate the universe
  glfwSetWindowTitle("GolfXTRM beta: Growing grass...");
  root = new universe();      // create the global universe object
  root->addplanet(0);         // populate it with a default planet
  root->planet[0]->addcourse(0, Vector3d(0,0,0), Vector3d(205,9,227));  // add the default course
  //root->planet[0]->course[0]->holeposition = Vector3d(25, 10, 25);  // this does nothing here

  glfwSetWindowTitle("GolfXTRM beta: Loading prehistoric fauna...");
  objloader *raptor = new objloader("assets/raptor.obj"); // keep this on the heap
  cout << "Loading raptor... " << endl;
  raptor->load();
  cout << "Raptor loaded." << endl;


  glfwSetWindowTitle("GolfXTRM beta: Walking to the course...");
  player = new golfer(root->planet[0]->course[0],
                      root->planet[0]->course[0]->teeposition.x,
                      root->planet[0]->course[0]->teeposition.y,
                      root->planet[0]->course[0]->teeposition.z-2);   // our player
  player->bodyyaw = 155;
  player->isplayer = true;    // give us control
  golfclub *randomgroundclub = new golfclub(root->planet[0]);
  randomgroundclub->rotate(golfclub::AXIS_X, 90);
  randomgroundclub->position.x = 1;
  randomgroundclub->position.z = 22;
  glfwSetWindowTitle("GolfXTRM beta: Polishing clubs...");
  player->helditem = new golfclub(root->planet[0]);
  player->helditem->held_by = player;
  player->swinglength = player->armlength + player->helditem->bbox_end.y;
  //glfwSetWindowTitle("GolfXTRM beta: Tipping the caddy...");
  //golfer *caddy = new golfer(root->planet[0]->course[0], 5, 0, 8); // the caddy
  //caddy->bodyyaw = -15;
  //caddy->helditem = new golfclub(root->planet[0]);
  //caddy->helditem->held_by = caddy;
  //caddy->swinglength = player->armlength + caddy->helditem->bbox_end.y;

  golfball *ball = new golfball(root->planet[0]);
  ball->position.y = ball->radius;

  glfwSetWindowTitle("GolfXTRM beta: Planting specific trees...");
  firtree *thistree1 = new firtree(root->planet[0], 48, root->planet[0]->course[0]->landscape->get_height_at(48, 32), 32, firtree::FIRTREE_STANDARD, 1);
  //oaktree *thistree2 = new oaktree(root->planet[0], 28, root->planet[0]->course[0]->landscape->get_height_at(28, 52), 52, oaktree::OAKTREE_STANDARD, 1);
  //ashtree *thistree3 = new ashtree(root->planet[0], 4, root->planet[0]->course[0]->landscape->get_height_at(18, 22), 2, 2);
  //firtree *thistree4 = new firtree(root->planet[0], 2, root->planet[0]->course[0]->landscape->get_height_at(2, 2), 2, firtree::FIRTREE_STANDARD, 1);
  //oaktree *thistree5 = new oaktree(root->planet[0], 2, root->planet[0]->course[0]->landscape->get_height_at(2, 2), 2, oaktree::OAKTREE_STANDARD, 1);
  //oaktree *thistree6 = new oaktree(root->planet[0], 2, root->planet[0]->course[0]->landscape->get_height_at(2, 0), 0, oaktree::OAKTREE_SAPLING, 1);

  srand(1337);
  glfwSetWindowTitle("GolfXTRM beta: Planting random trees...");
  for(int i = 0; i < 40; ++i) {
    double xpos = (rand() % (int)root->planet[0]->course[0]->landscape->bounds.x) - root->planet[0]->course[0]->landscape->bounds.x * 0.5;
    double zpos = (rand() % (int)root->planet[0]->course[0]->landscape->bounds.z) - root->planet[0]->course[0]->landscape->bounds.z * 0.5;
    new firtree(root->planet[0], xpos, root->planet[0]->course[0]->landscape->get_height_at(xpos, zpos), zpos, firtree::FIRTREE_RANDOM, rand());
  }
  for(int i = 0; i < 1000; ++i) {
    double xpos = (rand() % (int)root->planet[0]->course[0]->landscape->bounds.x) - root->planet[0]->course[0]->landscape->bounds.x * 0.5;
    double zpos = (rand() % (int)root->planet[0]->course[0]->landscape->bounds.z) - root->planet[0]->course[0]->landscape->bounds.z * 0.5;
    new firtree(root->planet[0], xpos, root->planet[0]->course[0]->landscape->get_height_at(xpos, zpos), zpos, firtree::FIRTREE_SAPLING_RANDOM, rand());
  }
  for(int i = 0; i < 40; ++i) {
    double xpos = (rand() % (int)root->planet[0]->course[0]->landscape->bounds.x) - root->planet[0]->course[0]->landscape->bounds.x * 0.5;
    double zpos = (rand() % (int)root->planet[0]->course[0]->landscape->bounds.z) - root->planet[0]->course[0]->landscape->bounds.z * 0.5;
    new oaktree(root->planet[0], xpos, root->planet[0]->course[0]->landscape->get_height_at(xpos, zpos), zpos, oaktree::OAKTREE_SAPLING_RANDOM, rand());
  }
  if(root->planet[0]->introon) {
    root->planet[0]->course[0]->landscape->populatefeatures(2, feature::FIRTREESAPLING, 1.4, 0.5);
    root->planet[0]->course[0]->landscape->populatefeatures(3, feature::OAKTREESAPLING, 1.6, 0.1);
  } else {
    root->planet[0]->course[0]->landscape->populatefeatures(2, feature::FIRTREE, 1.4, 0.5);
    root->planet[0]->course[0]->landscape->populatefeatures(3, feature::OAKTREE, 1.6, 0.1);
  }

  glfwSetWindowTitle(titlestring);  // set the title to the main run's title

  cout << "Initialisation complete in " << glfwGetTime() << " seconds." << endl;
  glfwSleep(splashtime - glfwGetTime()); // show the splash screen for the rest of our splash time
  //while(glfwGetTime() < splashtime) {
  //  // lazy-wait while refreshing through the splash screen
  //  glfwSleep(0.1);
  //}
  glfwEnable(GLFW_AUTO_POLL_EVENTS);
  glfwDisable(GLFW_MOUSE_CURSOR);         // hide the mouse
  soundengine->play2D("GolfXTRM - Skin Walker - Versus (Slowed Down x4).ogg", true);

  // these must be absolutely last:
  glfwSetMousePos(windowwidth/2, windowheight/2);   //centre the mouse before the main loop
  glfwSetTime(0.0);   //reset the timer for the start of the main loop
}

void shutdown(int return_code, string errorstring) {  /// close everything gracefully before exit
  if(return_code == 0) {
    cout << "Exiting on command - have a nice day!" << endl;
  } else {
    cout << "Fatal error: " << errorstring << endl;
  }
  soundengine->drop();  // shut down the sound engine
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
  //if(glfwGetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {      //jump/fly up
  //  player->state = golfer::GOLFER_JUMPING;
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
    if(key == GLFW_KEY_SPACE) {        // space to jump
      player->state = golfer::GOLFER_JUMPING;
    } else if(key == 'O') {        // O and P switch render modes
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //filled
      cout << "Switched to filled render mode" << endl;
    } else if(key == 'P') {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //wireframe
      cout << "Switched to wireframe render mode" << endl;
    } else if(key == 'B') {
      golfball *ball = new golfball(root->planet[0]);
      Vector3d positionvector = Vector3d(0,0,-0.6);
      positionvector.rotate(0,-player->bodyyaw,0);
      ball->position = player->bodyposition + positionvector;
      //ball->position.y += ball->radius;
      ball->position.y += 1.5;
      ball->at_rest = false;
      cout << "Dropped another ball" << endl;
    } else if(key == GLFW_KEY_ESC) {         // escape to quit
      keeprunning = false;
      cout << "Stop requested..." << endl;
    }
  }
}

void physics(double timedelta) {    /// update entity and player locations
  if(timedelta > 0.1) {
    root->update(0.1);             // clamp our time delta
  } else {
    root->update(timedelta);        // carry out the global physics update
  }
}

void draw() {
  // clear the buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset view matrix
  glLoadIdentity();

  // translate us to the player's viewpoint
  glTranslated(0, -player->eyeleveloffset.y, 0);
  glRotatef(player->headyaw,   0, 1, 0);
  glRotatef(player->headpitch, 1, 0, 0);
  glTranslated(0, -player->headfulcrum.y, 0);
  glRotatef(player->bodyyaw,   0, 1, 0);
  glTranslated(-player->bodyposition.x,
               -player->bodyposition.y,
               -player->bodyposition.z);

  // tell the universe to go render itself
  root->render();

  // do the buffer shuffle
  glfwSwapBuffers();
}
