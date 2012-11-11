#ifndef HOLDABLE_H_INCLUDED
#define HOLDABLE_H_INCLUDED

class holdable {
public:
  golfer *held_by;      // who is holding this, NULL for lying/flying in the world
  bool at_rest;         // whether it's settled (in which case don't do further physics with it)

  double mass;          // how much it weighs (kg)
  double inertia;       // moment of inertia (hand-hold as fulcrum)
  Vector3d cog;         // centre of gravity relative to hand-hold

  Vector3d bbox_start;  // one corner of the bounding box
  Vector3d bbox_end;    // other corner of the bounding box

  Vector3d velocity;      // movement velocity
  Quatd angularvelocity;  // rotational velocity

  holdable() {          // default constructor
    held_by = NULL;
    at_rest = true;
    mass = 0;
    inertia = 0;
  }
};


#endif // HOLDABLE_H_INCLUDED
