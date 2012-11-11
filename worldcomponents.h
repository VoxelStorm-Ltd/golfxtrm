#ifndef WORLDCOMPONENTS_H_INCLUDED
#define WORLDCOMPONENTS_H_INCLUDED

#include "progressbar.h"

class golfcourse {  /// the overall landscape object
public:
  planettile() {    // default constructor
  }
};


class world {     /// world (planet) objects containing golf courses
public:
  golfcourse *courses[18];
  world() {       // default constructor
  }

  void render() {
  }
};


class universe {  /// universe objects - there's only one of these - contains worlds
public:
  world *planet[1];
  int numplanets;
  universe() {                 // default constructor
    // big bang!
    numplanets = 0;
  }

  void addplanet(int worldnum) { // add a planet to this universe
    //planet[worldnum] = new world(0,0,0,16,16);  // fast test size
    //planet[worldnum] = new world(0,0,0,512,512);  // exceeds earth surface
    planet[worldnum] = new world(0,0,0,128,128);  // moon-sized w/ regions
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
