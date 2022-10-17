#include <cstdlib>
#include <iostream>
#include <cstring>
#include <iostream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "version.h"

#include <thread>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <GL/gl.h>
//#include <SOIL.h>
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

void physics(double timedelta);
void draw();
void controls();
void callback_key(GLFWwindow *thiswindow, int key, int scancode __attribute__((unused)), int action, int mods);
void callback_windowclose(GLFWwindow *thiswindow __attribute__((unused))) {
  /// Callback for handling window close events
  std::cout << "Window closed, exiting" << std::endl;

  _Exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  bool fullscreen, largewindow, skipintro;
  fullscreen = false;
  largewindow = true;
  skipintro = false;
  cout << "Starting up " << argv[0] << " with " << argc << " arguments" << endl;
  for(int i = 1; i < argc; i++) {
    cout << "Parsing commandline option " << i << endl;
    if(i != argc) {                                                             // Check that we haven't finished parsing already
      cout << "arg " << i << " = " << argv[i] << endl;
      string thisargstring = string(argv[i]);
      if(thisargstring == "-skipintro") {
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
      } else if (thisargstring == "-novao") {                                   // force VAO disable
        hasvao = false;
      } else if (thisargstring == "-width") {                                   // manual screen size setting
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
  skipintro = true;                                                             // skip the intro in debug mode
  #endif
  init(fullscreen, largewindow, skipintro);

  mainloop();
  shutdown(0, "");
}

void init(bool fullscreen, bool largewindow, bool skipintro) {       /// all the one-time initialisation we need for the engine
  // initialise the opengl window
  if(glfwInit() != GL_TRUE) {
    std::cout << "ERROR: glfwInit() failed" << std::endl;
    _Exit(EXIT_FAILURE);
  }

  int nummonitors = 0;
  GLFWmonitor **monitor_list = glfwGetMonitors(&nummonitors);
  GLFWmonitor *monitor_primary = glfwGetPrimaryMonitor();
  std::cout << "Monitors: " << nummonitors << std::endl;
  for(int monitornum = 0; monitornum != nummonitors; ++monitornum) {
    GLFWmonitor *thismonitor = monitor_list[monitornum];
    int physicalwidth  = 0;
    int physicalheight = 0;
    int xpos = 0;
    int ypos = 0;
    glfwGetMonitorPhysicalSize(thismonitor, &physicalwidth, &physicalheight);
    glfwGetMonitorPos(thismonitor, &xpos, &ypos);
    const GLFWvidmode *videomode = glfwGetVideoMode(thismonitor);

    std::cout << "Monitor " << monitornum;
    if(thismonitor == monitor_primary) {
      std::cout << " (primary)";
    }
    std::cout << std::endl;
    std::cout << "  Name: " << glfwGetMonitorName(thismonitor) << std::endl;
    std::cout << "  Physical size: " << physicalwidth << " " << physicalheight << std::endl;
    std::cout << "  Position: " << xpos << " " << ypos << std::endl;
    std::cout << "  Mode: " << videomode->width << " " << videomode->height << " " << videomode->refreshRate << std::endl;
  }

  windowwidth  = 1024;
  windowheight = 768;

  /*} else if(fullscreen) {
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
  }*/

  // set up window hints in advance
  //glfwWindowHint(GLFW_RED_BITS,   state->videomode->redBits);
  //glfwWindowHint(GLFW_GREEN_BITS, state->videomode->greenBits);
  //glfwWindowHint(GLFW_BLUE_BITS,  state->videomode->blueBits);
  //glfwWindowHint(GLFW_DEPTH_BITS, 32);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);                         // forward compat disables all deprecated functions - we don't want that
  //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION);
  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  window_main = glfwCreateWindow(windowwidth,
                                 windowheight,
                                 "GolfXTRM",
                                 NULL,
                                 NULL);
  glfwMakeContextCurrent(window_main);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glfwShowWindow(window_main);                                                  // only display the window once in position

  if(!window_main) {
    // exit if this didn't work
    std::cout << "ERROR: glfwOpenWindow returned NULL" << std::endl;
    _Exit(EXIT_FAILURE);
  }
  glfwSetWindowCloseCallback(window_main, callback_windowclose);                // callback for window closing

  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK) {
    std::cout << "ERROR: GLEW returned " << glewInit() << std::endl;
    _Exit(EXIT_FAILURE);
  }
  glewExperimental = GL_TRUE;
  std::cout << "GL_VERSION:  " << glGetString(GL_VERSION)  << std::endl;
  std::cout << "GL_VENDOR:   " << glGetString(GL_VENDOR)   << std::endl;
  std::cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "GL version major " << glfwGetWindowAttrib(window_main, GLFW_CONTEXT_VERSION_MAJOR)
            << " minor "           << glfwGetWindowAttrib(window_main, GLFW_CONTEXT_VERSION_MINOR)
            << " revision "        << glfwGetWindowAttrib(window_main, GLFW_CONTEXT_REVISION)
            << " API "             << glfwGetWindowAttrib(window_main, GLFW_CLIENT_API)
            << " profile  "        << glfwGetWindowAttrib(window_main, GLFW_OPENGL_PROFILE) << std::endl;;
  std::cout << "GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  if(!GLEW_ARB_vertex_array_object) {
    std::cout << "GLEW_ARB_vertex_array_object not available..." << std::endl;
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

  glColor3f(1, 1, 1);                                                           // this may be necessary before enabling lighting
  glFrontFace(GL_CCW);                                                          // set up counter-clockwise polygon winding
  //glCullFace(GL_BACK);                                                        // may be redundant to cull back-faces
  glEnable(GL_DEPTH_TEST);                                                      // go on, use the zbuffer
  glEnable(GL_DITHER);                                                          // may marginally increase shading quality
  glEnable(GL_LIGHTING);                                                        // obviously we want lighting... right?
  //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);                        // enable local lighting
  //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);                         // to make flat shading ok
  //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);                             // ensure one-sided lighting
  //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);                            // slower, checks sign of dot product of normals

  //glEnable(GL_MINMAX);                                                        // allow min and max colour tables for HDR effects

  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //filled
  glPolygonMode(GL_FRONT, GL_FILL);  //filled
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //wireframe
  //glShadeModel(GL_SMOOTH);  //SMOOTH or FLAT
  glShadeModel(GL_FLAT);    //may look more spectacular for a cube world

  // fog settings
  glEnable(GL_FOG);
  glFogfv(GL_FOG_COLOR, Vector4f(fogred, foggreen, fogblue, 1));
  //glFogi(GL_FOG_MODE, GL_LINEAR);                                             // GL_LINEAR GL_EXP GL_EXP2
  glFogi(GL_FOG_MODE, GL_EXP);                                                  // GL_LINEAR GL_EXP GL_EXP2
  glFogf(GL_FOG_DENSITY, 0.02);                                                 // only used for exponential fog
  glFogi(GL_FOG_START, 2);                                                      // only used for linear fog
  glFogi(GL_FOG_END, 100);

  // temporary material definition
  GLfloat mat_specular[] = {0.0, 0.0, 0.0, 1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  //glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);                              // range 0-128
  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
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

  glfwSetKeyCallback(window_main, callback_key);                                // activate the control callback
  //glfwDisable(GLFW_MOUSE_CURSOR);                                             // hide the mouse

  glfwSwapInterval(1);                                                          // activate vsync

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
    //#ifdef NOMUSIC
    //  cout << "Skipping sound engine initialisation" << endl;
    //#else
    //  soundengine->play2D("GolfXTRM - Anton Riehl - The Green Flash.ogg", false); // introductory, flutey
    //#endif
  }

  // load and display the splash screen
  /*
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
  //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);                          // optimised for inverse alpha
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
      glEnable(GL_DEPTH_TEST);                                                  // go on, use the zbuffer again
      glEnable(GL_LIGHTING);
      glMatrixMode(GL_PROJECTION);
    }
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  }
  glPopMatrix();
  glfwSwapBuffers(window_main);
  //glBindTexture(GL_TEXTURE_2D, NULL);
  glDisable(GL_TEXTURE_2D);
  double splashtime;
  if(skipintro) {
    splashtime = 0;
  } else {
    splashtime = 12;                                                            // how long to show it for
  }
  */

  srand(1337);                                                                  // seed the random generator predictably

  // create and populate the universe
  glfwSetWindowTitle(window_main, "GolfXTRM beta: Growing grass...");
  root = new universe();                                                        // create the global universe object
  root->addplanet(0);                                                           // populate it with a default planet
  root->planet[0]->addcourse(0, Vector3d(0,0,0), Vector3d(205,9,227), 1337);    // add the default course

  glfwSetWindowTitle(window_main, "GolfXTRM beta: Loading prehistoric fauna...");
  objloader *raptor = new objloader("assets/raptor.obj");                       // keep this on the heap
  cout << "Loading raptor... " << endl;
  raptor->load();
  cout << "Raptor loaded." << endl;


  glfwSetWindowTitle(window_main, "GolfXTRM beta: Walking to the course...");
  player = new golfer(root->planet[0]->course[0],
                      root->planet[0]->course[0]->teeposition.x,
                      root->planet[0]->course[0]->teeposition.y,
                      root->planet[0]->course[0]->teeposition.z-2);             // our player
  player->bodyyaw = 155;
  player->isplayer = true;                                                      // give us control
  golfclub *randomgroundclub = new golfclub(root->planet[0]);
  randomgroundclub->rotate(golfclub::AXIS_X, 90);
  randomgroundclub->position.x = 1;
  randomgroundclub->position.z = 22;
  glfwSetWindowTitle(window_main, "GolfXTRM beta: Polishing clubs...");
  player->helditem = new golfclub(root->planet[0]);
  player->helditem->held_by = player;
  player->swinglength = player->armlength + player->helditem->bbox_end.y;
  //glfwSetWindowTitle("GolfXTRM beta: Tipping the caddy...");
  //golfer *caddy = new golfer(root->planet[0]->course[0], 5, 0, 8);            // the caddy
  //caddy->bodyyaw = -15;
  //caddy->helditem = new golfclub(root->planet[0]);
  //caddy->helditem->held_by = caddy;
  //caddy->swinglength = player->armlength + caddy->helditem->bbox_end.y;

  golfball *ball = new golfball(root->planet[0]);
  ball->position.y = ball->radius;

  glfwSetWindowTitle(window_main, "GolfXTRM beta: Planting specific trees...");
  //firtree *thistree1 = new firtree(root->planet[0], 48, root->planet[0]->course[0]->landscape->get_height_at(48, 32), 32, firtree::FIRTREE_STANDARD, 1);
  //oaktree *thistree2 = new oaktree(root->planet[0], 28, root->planet[0]->course[0]->landscape->get_height_at(28, 52), 52, oaktree::OAKTREE_STANDARD, 1);
  //ashtree *thistree3 = new ashtree(root->planet[0], 4, root->planet[0]->course[0]->landscape->get_height_at(18, 22), 2, 2);
  //firtree *thistree4 = new firtree(root->planet[0], 2, root->planet[0]->course[0]->landscape->get_height_at(2, 2), 2, firtree::FIRTREE_STANDARD, 1);
  //oaktree *thistree5 = new oaktree(root->planet[0], 2, root->planet[0]->course[0]->landscape->get_height_at(2, 2), 2, oaktree::OAKTREE_STANDARD, 1);
  //oaktree *thistree6 = new oaktree(root->planet[0], 2, root->planet[0]->course[0]->landscape->get_height_at(2, 0), 0, oaktree::OAKTREE_SAPLING, 1);

  glfwSetWindowTitle(window_main, "GolfXTRM beta");                             // set the title to the main run's title

  cout << "Initialisation complete in " << glfwGetTime() << " seconds." << endl;
  //std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((splashtime - glfwGetTime()) * 1000)));
  //while(glfwGetTime() < splashtime) {
  //  // lazy-wait while refreshing through the splash screen
  //  glfwSleep(0.1);
  //}
  glfwSetInputMode(window_main, GLFW_CURSOR, GLFW_CURSOR_DISABLED);             // hide the mouse
  #ifdef NOMUSIC
    cout << "Skipping music" << endl;
  #else
    //soundengine->play2D("Skin Walker - Berserker - Versus.ogg", true);
  #endif

  // these must be absolutely last:
  glfwSetCursorPos(window_main, windowwidth / 2, windowheight / 2);   //centre the mouse before the main loop
  glfwSetTime(0.0);   //reset the timer for the start of the main loop
}

void shutdown(int return_code, string errorstring) {  /// close everything gracefully before exit
  if(return_code == 0) {
    cout << "Exiting on command - have a nice day!" << endl;
  } else {
    cout << "Fatal error: " << errorstring << endl;
  }
  glfwTerminate();                                                              // shut down the window
  exit(return_code);
}


void mainloop() {   /// the main rendering loop
  while(keeprunning) {                                                          // cheap infinite loop
    timelasttickstart = glfwGetTime();

    controls();                                                                 // run the control polling loop, if it's time

    timelasttickend = glfwGetTime();
    timedeltatick = timelasttickend - timelasttickstart;                        // tick delta

    // render the frame, if it's time to do so
    timelastrenderstart = glfwGetTime();
    draw();                                                                     // do the rendering
    timelasttotal = timelastrenderend;
    timelastrenderend = glfwGetTime();
    timedeltarender = timelastrenderend - timelastrenderstart;                  // render delta
    timedeltatotal = timelastrenderend - timelasttotal;                         // total delta
    //timedeltaaverage = (timedeltaaverage + timedeltatotal) / 2;               // update the rolling average
    timedeltaaverage = ((timedeltaaverage * 100) + timedeltatotal) / 101;       // update the rolling average

    physics(timedeltatotal);                                                    // run the physics for this tick, if it's time

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

  if(glfwGetKey(window_main, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {              // shift to sprint
    thismoveforce   = player->maxforce_run;
    thisstrafeforce = player->maxforce_runstrafe;
    movestate = golfer::GOLFER_RUNNING;
  } else {
    thismoveforce   = player->maxforce_walk;
    thisstrafeforce = player->maxforce_walkstrafe;
    movestate = golfer::GOLFER_WALKING;
  }

  player->state = golfer::GOLFER_STANDING;
  if(glfwGetKey(window_main, GLFW_KEY_W) == GLFW_PRESS) {                       // wasd for movement
    player->moveforce.x += std::sin(player->bodyyaw * M_PI / 180) * thismoveforce;
    player->moveforce.z -= std::cos(player->bodyyaw * M_PI / 180) * thismoveforce;
    keyspressed++;
    player->state = movestate;
  }
  if(glfwGetKey(window_main, GLFW_KEY_S) == GLFW_PRESS) {
    player->moveforce.x -= std::sin(player->bodyyaw * M_PI / 180) * thisstrafeforce; // you can't run as fast
    player->moveforce.z += std::cos(player->bodyyaw * M_PI / 180) * thisstrafeforce; // backwards as forwards
    keyspressed++;
    player->state = movestate;
  }
  if(glfwGetKey(window_main, GLFW_KEY_A) == GLFW_PRESS) {
    player->moveforce.x -= std::cos(player->bodyyaw * M_PI / 180) * thisstrafeforce;
    player->moveforce.z -= std::sin(player->bodyyaw * M_PI / 180) * thisstrafeforce;
    keyspressed++;
    player->state = movestate;
  }
  if(glfwGetKey(window_main, GLFW_KEY_D) == GLFW_PRESS) {
    player->moveforce.x += std::cos(player->bodyyaw * M_PI / 180) * thisstrafeforce;
    player->moveforce.z += std::sin(player->bodyyaw * M_PI / 180) * thisstrafeforce;
    keyspressed++;
    player->state = movestate;
  }
  //if(glfwGetKey(window_main, GLFW_KEY_SPACE) == GLFW_PRESS) {      //jump/fly up
  //  player->state = golfer::GOLFER_JUMPING;
  //}

  if(keyspressed > 1) {     //allow for relatively smooth diagonal strafe
    player->moveforce /= keyspressed;
  }

  // convert mouse movements to camera rotation
  glfwGetCursorPos(window_main, &mousex, &mousey);
  //player->headyaw += (mousex-(windowwidth / 2)) * camyawperpixel;
  //player->headpitch += (mousey-(windowheight / 2)) * campitchperpixel * mouseinvert;
  player->yawtorque   = (mousex - (windowwidth  / 2)) * camyawperpixel;
  player->pitchtorque = (mousey - (windowheight / 2)) * campitchperpixel * mouseinvert;
  glfwSetCursorPos(window_main, windowwidth / 2, windowheight / 2);   //reset the mouse immediately after

  // poll mouse buttons
  if(glfwGetMouseButton(window_main, GLFW_MOUSE_BUTTON_LEFT)) {
    if(glfwGetMouseButton(window_main, GLFW_MOUSE_BUTTON_RIGHT)) {
      // both buttons held down
      inputmode = INPUTMODE_MOVING_HEAD;
    } else {
      // LMB held down
      inputmode = INPUTMODE_MOVING_ARMS;
    }
  } else {
    if(glfwGetMouseButton(window_main, GLFW_MOUSE_BUTTON_RIGHT)) {
      // RMB held down
      inputmode = INPUTMODE_MOVING_HEAD_AND_ARMS;
    } else {
      // no buttons pressed
      inputmode = INPUTMODE_MOVING_HEAD_AND_BODY;
    }
  }
}

void callback_key(GLFWwindow *thiswindow, int key, int scancode __attribute__((unused)), int action, int mods) {
  if(action == GLFW_PRESS) {                                                    // here are all the down-presses we care about
    if(key == GLFW_KEY_SPACE) {                                                 // space to jump
      player->state = golfer::GOLFER_JUMPING;
    } else if(key == GLFW_KEY_O) {                                              // O and P switch render modes
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  //filled
      cout << "Switched to filled render mode" << endl;
    } else if(key == GLFW_KEY_P) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //wireframe
      cout << "Switched to wireframe render mode" << endl;
    } else if(key == GLFW_KEY_B) {
      golfball *ball = new golfball(root->planet[0]);
      Vector3d positionvector = Vector3d(0,0,-0.6);
      positionvector.rotate(0,-player->bodyyaw,0);
      ball->position = player->bodyposition + positionvector;
      //ball->position.y += ball->radius;
      ball->position.y += 1.5;
      ball->at_rest = false;
      cout << "Dropped another ball" << endl;
    } else if(key == GLFW_KEY_ESCAPE) {                                         // escape to quit
      keeprunning = false;
      cout << "Stop requested..." << endl;
    }
  }
}

void physics(double timedelta) {    /// update entity and player locations
  if(timedelta > 0.1) {
    root->update(0.1);                                                          // clamp our time delta
  } else {
    root->update(timedelta);                                                    // carry out the global physics update
  }
}

void draw() {
  // clear the buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset view matrix
  glLoadIdentity();

  // poll controls
  glfwPollEvents();

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
  glfwSwapBuffers(window_main);
}
