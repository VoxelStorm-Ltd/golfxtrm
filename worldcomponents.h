#ifndef WORLDCOMPONENTS_H_INCLUDED
#define WORLDCOMPONENTS_H_INCLUDED

#include <boost/ptr_container/ptr_vector.hpp>
#include "terrain.h"
#include "globalvars_client_extern.h"

class golfer;   // forward decs
class holdable;
class feature;
class world;

class golfcourse {  /// the overall landscape object
public:
  world *parentplanet;
  terrain *landscape;

  Vector3d origin;                      // where this is located in the world

  Vector3d teeposition;                 // where we tee off from
  Vector3d holeposition;                // where the hole is

  golfcourse(world *parent) {                      /// default constructor
    parentplanet = parent;
    landscape = new terrain();
  }

  void update(double timedelta);
  void render();

  double get_height_at(double x, double z) {
    return landscape->get_height_at(x, z);
  }

  double get_friction_at(double x, double z) {
    return landscape->get_friction_at(x, z);
  }

  double get_hardness_at(double x, double z) {
    return landscape->get_hardness_at(x, z);
  }
};


class world {     /// world (planet) objects containing golf courses
public:
  golfcourse *course[18];
  int numcourses;
  double horizondistance;

  double gravity;                       // downward acceleration in m/s^2
  double airdensity;                    // ya rly, for drag calculations (kg/m^3)
  Vector3d windvelocity;                // also used for air resistance (m/s)

  boost::ptr_vector<golfer> players;    // all the players on this planet
  boost::ptr_vector<holdable> items;    // all the loose items on this planet
  boost::ptr_vector<feature> features;  // all the permanent fixtures (trees etc)

  world() {                                 /// default constructor
    gravity = 9.800;
    airdensity = 1.2041;
    horizondistance = 1200;

    //windvelocity.x = 10;

    numcourses = 0;
    addcourse(0);   // no point having less than 1 course
  }

  void addcourse(int coursenum) {           /// add a golf course to this planet
    course[coursenum] = new golfcourse(this);
    ++numcourses;
  }

  void update(double timedelta);
  void render();
};


class universe {  /// universe objects - there's only one of these - contains worlds
public:
  world *planet[1];

  int numplanets;

  double updatetime;              // how long to wait between updates (1/above)
  double updatenexttime;          // what time the next update is due

  universe() {                              /// default constructor
    // big bang!
    numplanets = 0;

    updatetime = 1 / updatefreq;  // set this from the global
    updatenexttime = 0;           // this is ready for an update asap
  }

  void addplanet(int worldnum) {            /// add a planet to this universe
    planet[worldnum] = new world();
    ++numplanets;
  }

  void update(double timedelta);
  void render();
};


#endif // WORLDCOMPONENTS_H_INCLUDED
