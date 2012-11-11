#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

class holdable; // forward declaration

class golfer {                 /// all of the player's properties
public:
  double bodymass;            // how heavy this fellow is - for force calcs (Kg)
  double headmass;            // self-explanatory, useful for severing  (Kg)
  double armsmass;            // how much the arms weigh, held out (Kg)

  Vector3d bodyposition;      // current location in 3D world (m)
  Vector3d bodyvelocity;      // current velocity (m/s)
  Vector3d moveforce;         // (velocity change) wanted right now (N)

  double bodyyaw;             // body facing rotation
  double bodyyawvelocity;     // how fast the body is already turning right

  double headyaw;             // head facing rotation relative to inclined neck
  double headyawvelocity;     // how fast the head is already turning right
  double headpitch;           // neck inclination angle down
  double headpitchvelocity;   // how fast the neck is already tilting down

  double yawtorque;           // left-right rotation force (N)
  double pitchtorque;         // up-down rotation force (N)

  golfcourse *currentcourse;  // where we are
  holdable *helditem;         // what we're holding (NULL for empty hand)

  golfer(golfcourse *course,
         double xpos, double ypos, double zpos) {   // specific constructor
    currentcourse = course;
    bodyposition.x = xpos;
    bodyposition.y = ypos;
    bodyposition.z = zpos;
    // the Vector3's initialise themselves automatically
    bodyyaw = 0;
    headyaw = 0;
    headpitch = 0;
    bodyyawvelocity = 0;
    headyawvelocity = 0;
    headpitchvelocity = 0;

    yawtorque = 0;
    pitchtorque = 0;

    helditem = NULL;
  }

  void update(double timedelta) {
    /// update position and velocity based on force and time delta
    bodyvelocity += (moveforce * timedelta) / bodymass;   // impulse
    bodyposition += (bodyvelocity * timedelta);

    bodyyawvelocity += (yawtorque * timedelta);
    bodyyaw += (bodyyawvelocity * timedelta);
    if(bodyyaw > 360) {                       // wrap
      bodyyaw -= 360;
    } else if(bodyyaw < 0) {
      bodyyaw += 360;
    }
  }
};


#endif // PLAYER_H_INCLUDED
