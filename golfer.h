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
  holdable *helditem;         // what we're holding (NULL for empty hand)

  GLuint vao;                 // vertex array object
  GLuint vbo;                 // vertex buffer object
  GLuint ibo;                 // element buffer object (index buffer object)

  golfer(golfcourse *course,
         double xpos, double ypos, double zpos) {   /// specific constructor
    currentcourse = course;
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

    // biometrics
    bodymass = 70.8;                            // official biometric average for Europe
    headmass = 5;                               // rough official biometric
    armsmass = 3.216 * 2;                       // official biometric
    headfulcrum.y = 1.38;                       // DIY biometric guess
    eyeleveloffset.y = 1.6095 - headfulcrum.y;  // official average human standing eye height
    armlength = 0.68;                           // DIY biometric guess
    headyawlimit = 90;                          // DIY biometric approximation (including eye angles etc)
    headpitchuplimit = 75;                      // something reasonable to minimise neck-back effects
    headpitchdownlimit = 89;                    // consider making this >90?
    armsyawlimit = 160;                         // DIY guess - can point arms all the way behind yourself
    headyawdeadzone = 15;                       // arbitrary comfort value
    armspitchuplimit = 160;                     // DIY guess - arms can go back over your head
    armspitchdownlimit = 80;                    // limited for basic realism while holding clubs
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
    maxforce_walk = bodymass * walkspeed / 2.5; // calculated from accelerating to top speed in 2.5s
    maxforce_walkstrafe = maxforce_walk * 0.75; // as above
    maxforce_run = bodymass * walkrunspeed / 5; // calculated from accelerating to top speed in 5s
    maxforce_runstrafe = maxforce_run * 0.75;   // as above
    maximpulse_jump = 236;                      // from neuromechanics paper (N.s)
    cda = 0.3963;                               // calculated as (2*71*9.8)/(1.2041*(54^2)) to 4dp (TV ~= 54m/s)

    // rendering data
    // body
    double top    = 1.5;
    double bottom = 0;
    double left   = -0.25;
    double right  = 0.25;
    double front  = 0.10;
    double back   = -0.10;
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
    GLushort ibodata[] = {
      6,4,0, 0,2,6,   // front
      3,1,5, 5,7,3,   // back
      2,0,1, 1,3,2,   // left
      7,5,4, 4,6,7,   // right
      2,6,7, 7,3,2,   // top
      5,4,0, 0,1,5    // bottom
    };
    // if he's our avatar, skip drawing the head and neck
    //if(this == player) {
      // anything we draw for internal view should go here
    //} else {
      //
    //}

    // rendering setup
    vao = vbo = ibo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER,             vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, ibo);
    glBufferData(GL_ARRAY_BUFFER,             sizeof(vbodata), vbodata, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,         0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);             // set up the VAO's state
    glBindBuffer(GL_ARRAY_BUFFER,             vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, ibo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);

    // add it to the pointer vector of the home planet
    currentplanet = currentcourse->parentplanet;
    currentplanet->players.push_back(this);
  }

  ~golfer() {
    //currentplanet->players.erase(std::find(currentplanet->players.begin(), currentplanet->players.end(), this));
    //currentplanet->players.release(std::find(currentplanet->players.begin(), currentplanet->players.end(), this));
    // why won't the above work?
  }

  void update(double timedelta) {
    /// update position and velocity based on force and time delta
    // come on and move your body... impulse application
    bodyvelocity += (moveforce * timedelta) / bodymass;   // impulse

    if(inputmode == INPUTMODE_MOVING_HEAD_AND_BODY) {
      // default walk-around mode - move the head
      headyawvelocity   += (yawtorque   * timedelta) / headmomentofinertia;
      headpitchvelocity += (pitchtorque * timedelta) / headmomentofinertia;
      // put our arms down and centered
      //armsyawvelocity += (yawtorque * timedelta) / armsmomentofinertia;
      // attempt to center the body on the head
      if(headyaw > headyawdeadzone || headyaw < -headyawdeadzone || state == GOLFER_WALKING || state == GOLFER_RUNNING) {
        double thistorque = headyaw / 90 * bodyyawtorquelimit;
        if(thistorque > bodyyawtorquelimit) {
          thistorque = bodyyawtorquelimit;
        } else if(thistorque < -bodyyawtorquelimit) {
          thistorque = -bodyyawtorquelimit;
        }
        bodyyawvelocity += (thistorque * timedelta) / bodymomentofinertia;
      }
    } else if(inputmode == INPUTMODE_MOVING_HEAD_AND_ARMS) {
      // swing / aim / interact mode - move the arms and head
      armsyawvelocity   += (yawtorque   * timedelta) / headmomentofinertia;
      armspitchvelocity += (pitchtorque * timedelta) / headmomentofinertia;
      // head follows arms precisely
      headyaw = armsyaw;
      headpitch = armspitch;
      // body stays still
    } else if(inputmode == INPUTMODE_MOVING_ARMS) {  // INPUTMODE_MOVING_ARMS
      // swing / interact with fixed view - move the arms only
      armsyawvelocity   += (yawtorque   * timedelta) / headmomentofinertia;
      armspitchvelocity += (pitchtorque * timedelta) / headmomentofinertia;
      // body stays still
      // head stays still
    } else {          // INPUTMODE_MOVING_HEAD
      // like default mode but leave body frozen and arms where they are
      headyawvelocity   += (yawtorque   * timedelta) / headmomentofinertia;
      headpitchvelocity += (pitchtorque * timedelta) / headmomentofinertia;
    }

    // gravitational force
    bodyvelocity.y -= (currentplanet->gravity * timedelta);     // acceleration

    // the inertially driven motions
    //std::cout << "DEBUGv:" << bodyvelocity.x << ":" << bodyvelocity.y << ":" << bodyvelocity.z << ":" << std::endl;
    //std::cout << "DEBUGp:" << bodyposition.x << ":" << bodyposition.y << ":" << bodyposition.z << ":" << std::endl;
    bodyposition += bodyvelocity      * timedelta;
    bodyyaw      += bodyyawvelocity   * timedelta;
    headyaw      += (headyawvelocity - bodyyawvelocity) * timedelta;   // compensate for body rotation
    armsyaw      += armsyawvelocity   * timedelta;
    armspitch    += armspitchvelocity * timedelta;
    if(headpitch > 0) {
      headpitch += (headpitchvelocity - (abs(bodyyawvelocity) / 4)) * timedelta;
    } else {
      headpitch += (headpitchvelocity + (abs(bodyyawvelocity) / 4)) * timedelta;
    }

    // internal damping
    double thisheaddampingamount = 1 - (headdampingcoefficient * timedelta);
    headyawvelocity   *= thisheaddampingamount;
    headpitchvelocity *= thisheaddampingamount;
    armsyawvelocity   *= thisheaddampingamount;
    headyawvelocity   *= thisheaddampingamount;
    bodyyawvelocity   *= 1 - (bodyyawdampingcoefficient * timedelta);

    // air resistance and wind effect (combined)
    Vector3d thisveldiff = bodyvelocity - currentplanet->windvelocity;
    if(thisveldiff.x == 0 && thisveldiff.y == 0 && thisveldiff.z == 0) {
      if(currentplanet->windvelocity.x == 0 && currentplanet->windvelocity.y == 0 && currentplanet->windvelocity.z == 0) {
        // no calculations necessary
      } else {
        double thisdragimpulse = (0.5 * cda * currentplanet->airdensity * thisveldiff.lengthSq() * timedelta) / bodymass ;
        Vector3d thisdragdecel;
        thisdragdecel = currentplanet->windvelocity;
        thisdragdecel.normalize();
        thisdragdecel *= thisdragimpulse;
        //std::cout << "Vel: " << bodyvelocity.length() * 2.23693629 << "mph Wind: " << currentplanet->windvelocity.length() * 2.23693629 << "mph Rel: " << thisveldiff.length() * 2.23693629 << "mph " << std::endl;
        bodyvelocity += thisdragdecel;
      }
    } else {
      double thisdragimpulse = (0.5 * cda * currentplanet->airdensity * thisveldiff.lengthSq() * timedelta) / bodymass ;
      Vector3d thisdragdecel;
      thisdragdecel = thisveldiff;
      thisdragdecel.normalize();
      thisdragdecel = Vector3d(0,0,0) - (thisdragdecel * thisdragimpulse);
      //std::cout << "Vel: " << bodyvelocity.length() * 2.23693629 << "mph Wind: " << currentplanet->windvelocity.length() * 2.23693629 << "mph Rel: " << thisveldiff.length() * 2.23693629 << "mph " << std::endl;
      bodyvelocity += thisdragdecel;
    }

    // ground collision
    double groundheight = currentcourse->get_height_at(bodyposition.x, bodyposition.z);
    if(bodyposition.y <= groundheight) {
      //bodyposition.y = groundheight;
      // smoothly bring us back up to the ground
      bodyposition.y = bodyposition.y + ((groundheight - bodyposition.y) * currentcourse->get_hardness_at(bodyposition.x, bodyposition.y) * timedelta);
      if(bodyvelocity.y < 0) {                // stop downwards motion
        bodyvelocity.y = 0;
      }
      // apply ground friction
      if(((state == GOLFER_STANDING) && (bodyvelocity.lengthSq() > 0.0001 )) ||
         ((state == GOLFER_WALKING) && (bodyvelocity.lengthSq() > walkspeed)) ||
         ((state == GOLFER_RUNNING) && (bodyvelocity.lengthSq() > walkrunspeed))) {
        double thisfrictionimpulse = bodymass * currentcourse->get_friction_at(bodyposition.x, bodyposition.z) * timedelta;  // mass cancels
        Vector3d thisdragdecel = bodyvelocity;
        thisdragdecel.normalize();
        thisdragdecel = Vector3d(0,0,0) - (thisdragdecel * thisfrictionimpulse);
        //std::cout << "dragdecel: " << thisdragdecel.x << ":" << thisdragdecel.y << ":" << thisdragdecel.z << ", " << thisfrictionimpulse << std::endl;
        //std::cout << "Vel: " << bodyvelocity.length() * 2.23693629 << "mph Friction: " << thisdragdecel.length() << " " << std::endl;
        bodyvelocity += thisdragdecel;
      } else if(state == GOLFER_JUMPING) {    // apply an impulse upwards
        bodyvelocity.y += (maximpulse_jump / bodymass);
        state = GOLFER_FREEFALL;              // the natural conclusion to jumping
      }
    }

    // wrapping and clamping
    if(bodyyaw > 360) {                       // wrap body rotation
      bodyyaw -= 360;
    } else if(bodyyaw < 0) {
      bodyyaw += 360;
    }
    if(headyaw > headyawlimit) {                       // clamp head rotation
      headyaw = headyawlimit;
    } else if(headyaw < -headyawlimit) {
      headyaw = -headyawlimit;
    }
    if(headpitch > headpitchdownlimit) {
      headpitch = headpitchdownlimit;
    } else if(headpitch < -headpitchuplimit) {
      headpitch = -headpitchuplimit;
    }

    moveforce.x = 0;
    moveforce.y = 0;
    moveforce.z = 0;
  }


  void render() {           /// alias function for preferred render method
    render5();
  }

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

    // if he's our avatar, skip drawing the head and neck
    //if(this == player) {
      // anything we draw for internal view should go here
    //} else {
      //
    //}
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

    GLushort indices[] = {
      1,5,7, 7,3,1,
      4,0,2, 3,6,4,
      0,1,3, 3,2,0,
      5,4,6, 6,7,5,
      3,7,6, 6,2,3,
      1,0,4, 4,5,1
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vbodata);

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLushort), GL_UNSIGNED_SHORT, indices);

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

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

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

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glPopMatrix();
  }

  void render5() {          /// draw this fellow using an indexed VBO with VAA and VAO
    glColor4f(1,1,0,1);

    glPushMatrix();
    glTranslated(bodyposition.x, bodyposition.y, bodyposition.z);
    glRotated(bodyyaw, 0, -1, 0);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

    glPopMatrix();
  }
};


#endif // PLAYER_H_INCLUDED
