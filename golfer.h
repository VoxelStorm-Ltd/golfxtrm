#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "globalvars_client_extern.h"

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

  Vector3d bodyposition;      // current location in 3D world (m)
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

  double yawtorque;           // left-right rotation force (N)
  double pitchtorque;         // up-down rotation force (N)

  double headyawlimit;        // how far we can turn our head to the right/left
  double headpitchuplimit;    // how far we can tilt our neck back
  double headpitchdownlimit;  // how far we can tilt our neck forward
  double armsyawlimit;        // how far we can turn our arms (more than the head)
  double armspitchuplimit;    // how far we can tilt our arms back over our head
  double armspitchdownlimit;  // how far we can tilt our arms down in front

  double walkspeed;           // maximum speed on the ground when not sprinting
  double strafespeed;         // maximum sideways movement ground speed
  double walkrunspeed;        // maximum speed on the ground while sprinting
  double straferunspeed;      // maximum sideways movement ground sprint speed


  golfcourse *currentcourse;  // where we are
  world *currentplanet;       // what planet, is this? ;)
  holdable *helditem;         // what we're holding (NULL for empty hand)

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
    headpitchuplimit = 89;                      // <90 to avoid gimbal lock
    headpitchdownlimit = 89;                    // consider making this >90?
    armsyawlimit = 160;                         // DIY guess - can point arms all the way behind yourself
    armspitchuplimit = 160;                     // DIY guess - arms can go back over your head
    armspitchdownlimit = 80;                    // limited for basic realism while holding clubs
    bodymomentofinertia = 1.18668836;           // 103.0 lb.in.sec.2 from US military data
    headmomentofinertia = 0.015;                // ~150 kg.cm^2 from US naval data
    armsmomentofinertia = 0.05014;              // -250.7 kg.m^2 from biomechanics paper
    walkspeed = 1.50876;                        // average walking
    walkrunspeed = 6.25856;                     // average male running speed
    strafespeed = walkspeed * 0.75;             // generous guess
    straferunspeed = walkrunspeed * 0.75 ;      // generous guess

    // add it to the pointer vector of the home planet
    currentplanet = currentcourse->parentplanet;
    currentplanet->players.push_back(this);
  }

  ~golfer() {
    //currentplanet->players.erase(std::find(currentplanet->players.begin(), currentplanet->players.end(), this));
    // why won't the above work?
  }

  void update(double timedelta) {
    /// update position and velocity based on force and time delta
    // come on and move your body... impulse application
    bodyvelocity += (moveforce * timedelta) / bodymass;   // impulse

    if(inputmode == INPUTMODE_MOVING_HEAD) {
      // default walk-around mode - move the head
      headyawvelocity += (yawtorque * timedelta) / headmomentofinertia;
      // put our arms down and centered
      // attempt to center the body on the head
      bodyyawvelocity += (yawtorque * timedelta) / bodymomentofinertia;

    } else if(inputmode == INPUTMODE_MOVING_HEAD_AND_ARMS) {
      // swing / aim / interact mode - move the arms and head
      // head follows arms precisely
      // body stays still

    } else {  // INPUTMODE_MOVING_ARMS
      // swing / interact with fixed view - move the arms only
      // body stays still
      // head stays still

    }

    // the inertially driven motions
    bodyposition += (bodyvelocity * timedelta);
    bodyyaw += (bodyyawvelocity * timedelta);
    headyaw += (headyawvelocity * timedelta);

    // wrapping and clamping
    if(bodyyaw > 360) {                       // wrap body rotation
      bodyyaw -= 360;
    } else if(bodyyaw < 0) {
      bodyyaw += 360;
    }
    if(headyaw > 360) {                       // clamp head rotation
      headyaw -= 360;
    } else if(headyaw < 0) {
      headyaw += 360;
    }
  }

  void render() {                             /// draw this chap
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(1,1,0,1);

    // body
    double top = 1.5;
    double bottom = 0;
    double left = -0.25;
    double right = 0.25;
    double front = 0.10;
    double back = -0.10;
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
  }
};


#endif // PLAYER_H_INCLUDED
