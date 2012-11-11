#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED


class golfer {                 /// all of the player's properties
public:
  double bodymass;         // how heavy this fellow is - for force calcs
  double headmass;         // self-explanatory
  double armsmass;         // how much the arms weigh, held out etc

  Vector3d bodyposition;    // current location in 3D world (m)
  Vector3d bodyvelocity;    // current velocity (m/s)
  Vector3d moveforce;       // (velocity change) wanted right now (N)

  double yawtorque;         // left-right rotation force (N)
  double pitchtorque;       // up-down rotation force (N)

  double bodyyaw;           // body facing rotation
  double bodyyawvelocity;   // how fast the body is already turning right

  double headyaw;           // head facing rotation relative to inclined neck
  double headyawvelocity;   // how fast the head is already turning right
  double headpitch;         // neck inclination angle down
  double headpitchvelocity; // how fast the neck is already tilting down


  golfcourse *currentcourse;

  golfer() {   // default constructor
  }

  golfer(double xpos, double ypos, double zpos) {   // specific
    bodyposition.x = xpos;
    bodyposition.y = ypos;
    bodyposition.z = zpos;
    // the Vector3's initialise themselves automatically
    bodyyaw = 0;
    headyaw = 0;
    headpitch = 0;
    bodyyawvelocity = 0;
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
