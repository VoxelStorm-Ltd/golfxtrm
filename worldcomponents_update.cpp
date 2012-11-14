#include "worldcomponents.h"
#include "golfer.h"
#include "holdable.h"
#include "landscape_features.h"

extern universe *root;
extern golfer *player;

void golfcourse::update(double timedelta) {
  landscape->update(timedelta);
}

void world::update(double timedelta) {
  /// Runs physics updates on every object on this planet with different frequencies
  // check if we're due for an update yet for the less critical actions
  double timenow = glfwGetTime();
  if(timenow > updatenexttime) {
    updatenexttime = timenow + updatetime;

    // update meteorological visuals here
    timeofday += timespeed * updatetime;
    if(timeofday > 86400) {
      int dayspassed = timeofday / 86400;
      timeofday = (int)timeofday % 86400;
      //++calendardate;
      calendardate += dayspassed;
    }

    double skybrightness = sin(timeofday / (double)86400 * M_PI);
    glFogfv(GL_FOG_COLOR, fogcolour);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientcolour * skybrightness);

    // search the universe for worlds to update
    // iterate through the courses
    for(int i=0; i < numcourses; ++i) {
      course[i]->update(updatetime);
    }
    for(boost::ptr_vector<feature>::iterator i = features.begin(); i != features.end(); ++i) {
      i->update(timespeed, updatetime);
    }
    // this is a decent place for an FPS counter because of the time delay
    std::cout << "FPS " << (int)(1 / timedeltaaverage) << " Time: " << (int)((int)timeofday / 3600) << ":" << (int)(((int)timeofday / 60) % 60) << ":" << (int)((int)timeofday % 60) << " Day: " << calendardate << " Rate: " << timespeed << "x" << std::endl;

    // intro time zoom
    if(timespeed > 3600) {
      timespeed -= (timespeed * 0.5 * updatetime);
    } else {
      timespeed = 1;
      updatetime = 5;
    }
  }

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
  for (int p = 0; p < numplanets; ++p) {
    planet[p]->update(timedelta);
  }
}
