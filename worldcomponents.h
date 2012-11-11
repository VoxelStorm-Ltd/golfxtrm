#ifndef WORLDCOMPONENTS_H_INCLUDED
#define WORLDCOMPONENTS_H_INCLUDED

#include "progressbar.h"

class world;  // forward dec

class golfcourse {  /// the overall landscape object
public:
  world *parentplanet;
  terrain *landscape;

  golfcourse(world *parent) {                      // default constructor
    parentplanet = parent;
    landscape = new terrain();
  }
};


class world {     /// world (planet) objects containing golf courses
public:
  golfcourse *course[18];
  int numcourses;

  double gravity;     // downward acceleration in m/s^2

  world() {                                 // default constructor
    gravity = 9.800;
    numcourses = 18;
  }

  void addcourse(int coursenum) {           // add a golf course to this planet
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

  universe() {                              // default constructor
    // big bang!
    numplanets = 0;
  }

  void addplanet(int worldnum) {            // add a planet to this universe
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
