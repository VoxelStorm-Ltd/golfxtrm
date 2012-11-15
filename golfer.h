#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "vmath.h"
#include "globalvars_client_extern.h"
#include "worldcomponents.h"

class holdable;   // forward declaration

class golfer {                 /// all of the player's properties
public:
  double bodymass;            // how heavy this fellow is in total - for force calcs (Kg)
  double headmass;            // self-explanatory, useful for severing (Kg)
  double armsmass;            // how much the arms weigh, held out (Kg)

  Vector3d headfulcrum;       // head fulcrim position (base of the neck)
  Vector3d eyeleveloffset;    // the eye position within the head
  Vector3d armfulcrum;        // where the arms pivot about
  double armlength;           // how far away the hand-hold point is from fulcrum
  double armshoulderoffset;   // how far the shoulder is from fulcrum

  Vector3d bodyposition;      // current location of standing spot in 3D world (m)
  Vector3d bodyvelocity;      // current velocity (m/s)
  Vector3d moveforce;         // (velocity change) wanted right now (N)

  double bodyyaw;             // body facing rotation
  double bodyyawvelocity;     // how fast the body is already turning right
  double bodymomentofinertia; // the body's moment of inertia resisting torque

  double headyaw;             // head facing rotation relative to inclined neck
  double headyawvelocity;     // how fast the head is already turning right
  double headpitch;           // neck inclination angle down
  double headpitchvelocity;   // how fast the neck is already tilting down
  double headmomentofinertia; // in reality this would differ per axis

  double armsyaw;             // arms facing rotation relative to front of body
  double armsyawvelocity;     // how fast the arms are already turning right
  double armspitch;           // arm inclination angle down from horizontal
  double armspitchvelocity;   // how fast the arms are already tilting down
  double armsmomentofinertia; // in reality this would differ per axis

  double yawtorque;           // current view left-right rotation force (N)
  double pitchtorque;         // current view up-down rotation force (N)

  double headyawlimit;        // how far we can turn our head to the right/left
  double headpitchuplimit;    // how far we can tilt our neck back
  double headpitchdownlimit;  // how far we can tilt our neck forward
  double armsyawlimit;        // how far we can turn our arms (more than the head)
  double armspitchuplimit;    // how far we can tilt our arms back over our head
  double armspitchdownlimit;  // how far we can tilt our arms down in front
  double headyawdeadzone;     // how far we turn our head without the body following

  double bodyyawtorquelimit;  // how fast the body can turn
  double headdampingcoefficient; // how much head rotation velocity is lost per second
  double bodyyawdampingcoefficient; // how much body rotation velocity is lost per second

  double walkspeed;           // maximum speed on the ground when not sprinting
  double strafespeed;         // maximum sideways movement ground speed
  double walkrunspeed;        // maximum speed on the ground while sprinting
  double straferunspeed;      // maximum sideways movement ground sprint speed
  double walkspeed_sq;        // cached for easier comparison
  double walkrunspeed_sq;

  double maxforce_walk;       // maximum movement force we exert when walking (N)
  double maxforce_walkstrafe; // maximum movement force we exert when strafing (N)
  double maxforce_run;        // maximum movement force we exert when running (N)
  double maxforce_runstrafe;  // maximum movement force we exert when run-strafing (N)
  double maximpulse_jump;     // impulse delivered instantly, not as a force

  double cda;                 // coefficient of drag * crossectional area

  Vector3f skincolour;        // keep this adjustable per golfer

  enum golferstatetype {
    GOLFER_STANDING,
    GOLFER_WALKING,
    GOLFER_RUNNING,
    GOLFER_JUMPING,
    GOLFER_FREEFALL
  };
  golferstatetype state;      // for friction control, animation, etc

  golfcourse *currentcourse;  // where we are
  world *currentplanet;       // what planet, is this? ;)
  bool isplayer;              // is this controlled by the player?
  holdable *helditem;         // what we're holding (NULL for empty hand)
  double swinglength;         // our current swing length, determined by club

  GLuint vao_body;            // vertex array object for the body
  GLuint vao_hands;           // vertex array object for the hands
  GLuint vao_arms;            // vertex array object for the arms
  GLuint vao_head;            // vertex array object for the head
  GLuint vbo_body;            // vertex buffer object for the body
  GLuint vbo_hands;           // vertex buffer object for the hands
  GLuint vbo_arms;            // vertex buffer object for the arms
  GLuint vbo_head;            // vertex buffer object for the head
  GLuint ibo;                 // element buffer object (index buffer object)

  golfer(golfcourse *course,
         double xpos, double ypos, double zpos) {   /// specific constructor
    currentcourse = course;
    isplayer = false;
    bodyposition.x = xpos;
    bodyposition.y = ypos;
    bodyposition.z = zpos;
    // the vectors initialise themselves automatically so skip those
    bodyyaw = 0;
    headyaw = 0;
    headpitch = 0;
    armsyaw = 0;
    armspitch = 0;
    bodyyawvelocity = 0;
    headyawvelocity = 0;
    headpitchvelocity = 0;
    armsyawvelocity = 0;
    armspitchvelocity = 0;
    yawtorque = 0;
    pitchtorque = 0;
    helditem = NULL;
    swinglength = armlength;

    // biometrics
    bodymass = 70.8;                            // official biometric average for Europe
    headmass = 5;                               // rough official biometric
    armsmass = 3.216 * 2;                       // official biometric
    headfulcrum.y = 1.38;                       // DIY biometric guess
    eyeleveloffset.y = 1.6095 - headfulcrum.y;  // official average human standing eye height
    armlength = 0.60;                           // DIY biometric guess - hand distance from centre fulcrum
    armfulcrum.x = 0;                           // DIY guesses
    armfulcrum.y = 1.45;
    armfulcrum.z = 0;
    armshoulderoffset = 0.32;                   // DIY guess
    headyawlimit = 90;                          // DIY biometric approximation (including eye angles etc)
    headpitchuplimit = 75;                      // something reasonable to minimise neck-back effects
    headpitchdownlimit = 89;                    // consider making this >90?
    armsyawlimit = 160;                         // DIY guess - can point arms all the way behind yourself
    headyawdeadzone = 15;                       // arbitrary comfort value
    armspitchuplimit = 160;                     // DIY guess - arms can go back over your head
    armspitchdownlimit = 75;                    // limited for basic realism while holding clubs
    bodymomentofinertia = 1.18668836;           // 103.0 lb.in.sec.2 from US military data
    headmomentofinertia = 0.015;                // ~150 kg.cm^2 from US naval data
    armsmomentofinertia = 0.05014;              // -250.7 kg.m^2 from biomechanics paper
    headdampingcoefficient = 10;                // arbitrary comfort value
    bodyyawtorquelimit = 2000;                  // arbitrary comfort value
    bodyyawdampingcoefficient = 3;              // arbitrary comfort value
    walkspeed = 1.50876;                        // average walking
    walkrunspeed = 6.25856;                     // average male running speed
    strafespeed = walkspeed * 0.75;             // generous guess
    straferunspeed = walkrunspeed * 0.75;       // generous guess
    walkspeed_sq = walkspeed * walkspeed;       // caching
    walkrunspeed_sq = walkrunspeed * walkrunspeed;
    maxforce_walk = bodymass * walkspeed / 1;   // calculated from accelerating to top speed in n seconds
    maxforce_walkstrafe = maxforce_walk * 0.75; // as above
    maxforce_run = bodymass * walkrunspeed / 2; // calculated from accelerating to top speed in n seconds
    maxforce_runstrafe = maxforce_run * 0.75;   // as above
    maximpulse_jump = 236;                      // from neuromechanics paper (N.s)
    cda = 0.3963;                               // calculated as (2*71*9.8)/(1.2041*(54^2)) to 4dp (TV ~= 54m/s)
    skincolour.r = 0.937;                       // average caucasian skin swatch apparently
    skincolour.g = 0.815;
    skincolour.b = 0.811;

    // rendering data
    // body:
    float top    = 1.5;
    float bottom = 0;
    float left   = -0.25;
    float right  = 0.25;
    float front  = 0.10;
    float back   = -0.10;
    GLfloat vbodata[] = {
      left,  bottom, back,    // 0
      left,  bottom, front,   // 1
      left,  top,    back,    // 2
      left,  top,    front,   // 3
      right, bottom, back,    // 4
      right, bottom, front,   // 5
      right, top,    back,    // 6
      right, top,    front    // 7
    };
    GLuint ibodata[] = {
      6,4,0, 0,2,6,   // front
      3,1,5, 5,7,3,   // back
      2,0,1, 1,3,2,   // left
      7,5,4, 4,6,7,   // right
      2,6,7, 7,3,2,   // top
      5,4,0, 0,1,5    // bottom
    };


    // hands:
    top    = 0.04;
    bottom = -0.04;
    left   = -0.04;
    right  = 0.04;
    front  = -armlength - 0.04;
    back   = -armlength + 0.04;
    GLfloat vbodata_hands[] = {
      left,  bottom, back,    // 0
      left,  bottom, front,   // 1
      left,  top,    back,    // 2
      left,  top,    front,   // 3
      right, bottom, back,    // 4
      right, bottom, front,   // 5
      right, top,    back,    // 6
      right, top,    front    // 7
    };

    // arm sections:
    top    = 0.05;
    bottom = -0.05;
    left   = -0.05;
    right  = 0.05;
    front  = -((armlength + armshoulderoffset) / 2) + 0.05;
    back   = +0.05;
    GLfloat vbodata_arms[] = {
      left,  bottom, back,    // 0
      left,  bottom, front,   // 1
      left,  top,    back,    // 2
      left,  top,    front,   // 3
      right, bottom, back,    // 4
      right, bottom, front,   // 5
      right, top,    back,    // 6
      right, top,    front    // 7
    };
    // if he's our avatar, skip drawing the head and neck
    // head:
    top    = 0.12;
    bottom = -0.12;
    left   = -0.075;
    right  = 0.075;
    front  = -0.12;
    back   = 0.09;
    GLfloat vbodata_head[] = {
      left,  bottom, back,    // 0
      left,  bottom, front,   // 1
      left,  top,    back,    // 2
      left,  top,    front,   // 3
      right, bottom, back,    // 4
      right, bottom, front,   // 5
      right, top,    back,    // 6
      right, top,    front    // 7
    };

    // rendering setup
    vao_body = vao_hands = vao_arms = vao_head = 0;
    vbo_body = vbo_hands = vbo_arms = vbo_head = 0;
    ibo = 0;
    if(hasvao) {
      glGenVertexArrays(1, &vao_body);
      glGenVertexArrays(1, &vao_hands);
      glGenVertexArrays(1, &vao_arms);
      glGenVertexArrays(1, &vao_head);
    }
    glGenBuffers(1, &vbo_body);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER,         vbo_body);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata), vbodata, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,         0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if(hasvao) {
      glBindVertexArray(vao_body);             // set up the VAO's state
    }
    glBindBuffer(GL_ARRAY_BUFFER,         vbo_body);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    if(hasvao) {
      glBindVertexArray(0);
    }

    // arm sections
    glGenBuffers(1, &vbo_arms);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER,         vbo_arms);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata_arms), vbodata_arms, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,         0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if(hasvao) {
      glBindVertexArray(vao_arms);             // set up the VAO's state
    }
    glBindBuffer(GL_ARRAY_BUFFER,         vbo_arms);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    if(hasvao) {
      glBindVertexArray(0);
    }

    // hands
    glGenBuffers(1, &vbo_hands);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER,         vbo_hands);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata_hands), vbodata_hands, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,         0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if(hasvao) {
      glBindVertexArray(vao_hands);             // set up the VAO's state
    }
    glBindBuffer(GL_ARRAY_BUFFER,         vbo_hands);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    if(hasvao) {
      glBindVertexArray(0);
    }

    // head
    glGenBuffers(1, &vbo_head);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER,         vbo_head);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata_head), vbodata_head, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,         0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if(hasvao) {
      glBindVertexArray(vao_head);             // set up the VAO's state
    }
    glBindBuffer(GL_ARRAY_BUFFER,         vbo_head);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    if(hasvao) {
      glBindVertexArray(0);
    }

    // add it to the pointer vector of the home planet
    currentplanet = currentcourse->parentplanet;
    currentplanet->players.push_back(this);
  }

  ~golfer() {
    //currentplanet->players.erase(std::find(currentplanet->players.begin(), currentplanet->players.end(), this));
    //currentplanet->players.release(std::find(currentplanet->players.begin(), currentplanet->players.end(), this));
    // why won't the above work?
  }

  void update(double timedelta);

  void render() {           /// alias function for preferred render method
    render5();
  }
/*
  void render1() {          /// draw this chap as a simple immediate mode cuboid
    glColor4f(1,1,0,1);

    // body
    double top    = bodyposition.y + 1.5;
    double bottom = bodyposition.y;
    double left   = bodyposition.x - 0.25;
    double right  = bodyposition.x + 0.25;
    double front  = bodyposition.z + 0.10;
    double back   = bodyposition.z - 0.10;
    glBegin(GL_TRIANGLES);
      glNormal3i(0,     0,      1);
      glVertex3d(left,	bottom,	front);  // front face
      glVertex3d(right,	bottom,	front);
      glVertex3d(right,	top,	  front);
      glVertex3d(right,	top,	  front);
      glVertex3d(left,	top,	  front);
      glVertex3d(left,	bottom,	front);

      glNormal3i(0,     0,      -1);
      glVertex3d(right,	bottom,	back);  // back face
      glVertex3d(left,	bottom,	back);
      glVertex3d(left,	top,	  back);
      glVertex3d(left,	top,	  back);
      glVertex3d(right,	top,	  back);
      glVertex3d(right,	bottom,	back);

      glNormal3i(-1,    0,      0);
      glVertex3d(left,	bottom,	back);  // left face
      glVertex3d(left,	bottom,	front);
      glVertex3d(left,	top,	  front);
      glVertex3d(left,	top,	  front);
      glVertex3d(left,	top,	  back);
      glVertex3d(left,	bottom,	back);

      glNormal3i(1,     0,      0);
      glVertex3d(right,	bottom,	front);  // right face
      glVertex3d(right,	bottom,	back);
      glVertex3d(right,	top,	  back);
      glVertex3d(right,	top,	  back);
      glVertex3d(right,	top,	  front);
      glVertex3d(right,	bottom,	front);

      glNormal3i(0,     1,      0);
      glVertex3d(left,	top,	  front);  // top face
      glVertex3d(right,	top,	  front);
      glVertex3d(right,	top,	  back);
      glVertex3d(right,	top,	  back);
      glVertex3d(left,	top,	  back);
      glVertex3d(left,	top,	  front);

      glNormal3i(0,     -1,     0);
      glVertex3d(left,	bottom,	front);  // bottom face
      glVertex3d(left,	bottom,	back);
      glVertex3d(right,	bottom,	back);
      glVertex3d(right,	bottom,	back);
      glVertex3d(right,	bottom,	front);
      glVertex3d(left,	bottom,	front);
    glEnd();
  }

  void render2() {          /// draw this fellow using an indexed vertex array
    glColor4f(1,1,0,1);

    double top    = bodyposition.y + 1.5;
    double bottom = bodyposition.y;
    double left   = bodyposition.x - 0.25;
    double right  = bodyposition.x + 0.25;
    double front  = bodyposition.z + 0.10;
    double back   = bodyposition.z - 0.10;
    GLfloat vbodata[] = {
      left,  bottom, back,
      left,  bottom, front,
      left,  top,    back,
      left,  top,    front,
      right, bottom, back,
      right, bottom, front,
      right, top,    back,
      right, top,    front,
    };

    GLuint indices[] = {
      1,5,7, 7,3,1,
      4,0,2, 3,6,4,
      0,1,3, 3,2,0,
      5,4,6, 6,7,5,
      3,7,6, 6,2,3,
      1,0,4, 4,5,1
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vbodata);

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
  }

  void render3() {          /// draw this fellow using an indexed VBO
    glColor4f(1,1,0,1);

    glPushMatrix();
    glTranslated(bodyposition.x, bodyposition.y, bodyposition.z);
    glRotated(bodyyaw, 0, -1, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glPopMatrix();
  }

  void render4() {          /// draw this fellow using an indexed VBO with VAA
    glColor4f(1,1,0,1);

    glPushMatrix();
    glTranslated(bodyposition.x, bodyposition.y, bodyposition.z);
    glRotated(bodyyaw, 0, -1, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glPopMatrix();
  }
*/

  void render5();   // external

};


#endif // PLAYER_H_INCLUDED
