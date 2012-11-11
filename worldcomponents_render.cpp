#include "worldcomponents.h"
#include "golfer.h"
#include "holdable.h"

void golfcourse::render() {
  // draw the terrain
  landscape.render();
  // draw the trees and other furniture
  // TODO
}

void world::render() {
  // give us a basic horizon, sky, atmospheric stuff

  // iterate through the courses
  for(int i=0; i < numcourses; ++i) {
    course[numcourses]->render();
  }

  // iterate through the players on this planet
  for(boost::ptr_vector<golfer>::iterator i = players.begin(); i != players.end(); ++i) {
    i->render(); // no indirection needed since this is boost's ptr_vector
  }

  // iterate through the items and draw those
  for(boost::ptr_vector<holdable>::iterator i = items.begin(); i != items.end(); ++i) {
    i->render();
  }
}

void universe::render() {
  /// Renders every planet in this universe
  // search the universe for worlds to render
  for (int p = 0; p < numplanets; ++p) {
    planet[p]->render();
  }
}
