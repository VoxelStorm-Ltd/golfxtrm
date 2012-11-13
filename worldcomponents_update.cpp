#include "worldcomponents.h"
#include "golfer.h"
#include "holdable.h"

extern universe *root;
extern golfer *player;

void golfcourse::update(double timedelta) {
  landscape->update(timedelta);
}

void world::update(double timedelta) {
  // update meteorological visuals here too

  // iterate through the courses
  /*for(int i=0; i < numcourses; ++i) {
    course[i]->update(timedelta);
  }*/

  // iterate through the players on this planet
  for(boost::ptr_vector<golfer>::iterator i = players.begin(); i != players.end(); ++i) {
    i->update(timedelta); // no indirection needed since this is boost's ptr_vector
  }

  // iterate through the items and update those
  for(boost::ptr_vector<holdable>::iterator i = items.begin(); i != items.end(); ++i) {
    i->update(timedelta);
  }
}

void universe::update(double timedelta) {
  /// Runs physics update for every planet in this universe
  // check if we're due for an update yet
  /*double timenow = glfwGetTime();
  if(timenow > updatenexttime) {
    updatenexttime = timenow + updatetime;
    // search the universe for worlds to update
    for (int p = 0; p < numplanets; ++p) {
      planet[p]->update(updatetime);
    }
    // this is a decent place for an FPS counter because of the time delay
    std::cout << "FPS " << (int)(1 / timedeltaaverage) << std::endl;
  } else {
    // just update the player, not the rest of the planet
    player->update(timedelta);
  }*/

  for (int p = 0; p < numplanets; ++p) {
    planet[p]->update(timedelta);
  }
}
