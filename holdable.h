#ifndef HOLDABLE_H_INCLUDED
#define HOLDABLE_H_INCLUDED

class holdable {
public:
  golfer *held_by;          // who is holding this, NULL for lying/flying in the world
  bool at_rest;             // whether it's settled (in which case don't do further physics with it)

  double mass;              // how much it weighs (kg)
  double momentofinertia;   // moment of inertia (hand-hold as fulcrum)
  Vector3d cog;             // centre of gravity relative to hand-hold
  double airresistance;     // how much its flight is slowed by atmospheric drag
  double slideresistance;   // how much its slide along the ground is slowed

  std::string name;         // what it's called
  std::string description;  // longer description

  Vector3d bbox_start;      // one corner of the bounding box
  Vector3d bbox_end;        // other corner of the bounding box

  world *currentplanet;     // what planet are you even on?

  Vector3d position;        // location in space
  Vector3d velocity;        // movement velocity
  Quatd rotation;           // current rotation
  Quatd angularvelocity;    // rotational velocity


  holdable(world *parentplanet) {              /// default constructor
    held_by = NULL;
    at_rest = true;
    mass = 0;
    momentofinertia = 0;
    name = "object";
    description = "Some sort of object that hasn't been properly defined yet.";

    currentplanet = parentplanet;
    currentplanet->items.push_back(this);
  }

  ~holdable() {
    //currentplanet->items.erase(std::find(currentplanet->items.begin(), currentplanet->items.end(), this));
    // why won't that work?
  }

  void push(Vector3d impulse) {  /// apply a one-off impulse to this object
    // TODO: apply max holding force, enable knocking out of hand
    velocity += impulse / mass;   // applied directly as a one-off, no delta time considered
  }

  void push(Vector3d impulse, double targetx, double targety, double targetz) {
    /// apply a one-off impulse offset from centre of this object
    // TODO: apply max holding force, enable knocking out of hand
    velocity += impulse / mass;   // applied directly as a one-off, no delta time considered

    // TODO: calculate offset from COG and apply rotational acceleration
  }

  void update(double timedelta, Vector3d impulse) {
    /// update position and velocity based on force and time delta
    // only update if it's free in the air, not hand-held
    if(held_by == NULL) {
      position += (velocity * timedelta);

      // TODO: apply quaternion rotation
    }
  }

  void render() {                           /// draw this item in the world
  }
};


#endif // HOLDABLE_H_INCLUDED
