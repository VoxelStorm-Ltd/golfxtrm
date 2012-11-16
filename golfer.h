#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <GL/glew.h>
#include <GL/glfw.h>
#include "vmath.h"

class holdable;   // forward declaration
class golfcourse;
class world;

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

  golfer(golfcourse *course, double xpos, double ypos, double zpos);
  ~golfer();
  void update(double timedelta);
  void render();
  /*
  void render1();
  void render2();
  void render3();
  void render4();
  */
  void render5();   // external

};


#endif // PLAYER_H_INCLUDED
