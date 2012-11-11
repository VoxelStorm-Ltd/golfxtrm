#ifndef WORLDCOMPONENTS_H_INCLUDED
#define WORLDCOMPONENTS_H_INCLUDED

#include <boost/ptr_container/ptr_vector.hpp>

class golfer; // forward dec
class holdable;
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

};


class world {     /// world (planet) objects containing golf courses
public:
  golfcourse *course[18];
  int numcourses;

  double gravity;                       // downward acceleration in m/s^2

  boost::ptr_vector<golfer> players;    // all the players on this planet
  boost::ptr_vector<holdable> items;    // all the loose items on this planet

  world() {                                 /// default constructor
    gravity = 9.800;
    numcourses = 18;
  }

  void addcourse(int coursenum) {           /// add a golf course to this planet
    course[coursenum] = new golfcourse(this);
    ++numcourses;
  }

  void render() {
  }
};


class universe {  /// universe objects - there's only one of these - contains worlds
public:
  world *planet[1];

  int numplanets;

  universe() {                              /// default constructor
    // big bang!
    numplanets = 0;
  }

  void addplanet(int worldnum) {            /// add a planet to this universe
    planet[worldnum] = new world();
    ++numplanets;
  }

  void render() {
    /// Renders every planet in this universe
    // search the universe for worlds to render
    for (int p = 0; p < numplanets; ++p) {
      planet[p]->render();
    }
  }
};


#endif // WORLDCOMPONENTS_H_INCLUDED
