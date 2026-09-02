#include "worldcomponents.h"
#include <iostream>
#include "golfer.h"
#include "holdable.h"
//#include "landscape_features.h"
#include "terrain.h"
#include "particle.h"
#include "globalvars_client_extern.h"

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
    // do some timekeeping
    updatenexttime = timenow + updatetime;
    timeofday += timespeed * updatetime;
    if(timeofday > 86400) {
      int dayspassed = timeofday / 86400;
      timeofday = (int)timeofday % 86400;
      //++calendardate;
      calendardate += dayspassed;

      // update seasons if appropriate
      int dayofyear = calendardate % 365;
      if(dayofyear > 90 && dayofyear < 330) {                                   // consider snow december to march
        grasscolour   = summergrasscolour;
        skycolour     = summerskycolour;
        fogcolour     = summerfogcolour;
        clearcolour   = summerclearcolour;
        ambientcolour = summerambientcolour;
      } else {
        grasscolour   = wintergrasscolour;
        skycolour     = winterskycolour;
        fogcolour     = winterfogcolour;
        clearcolour   = winterclearcolour;
        ambientcolour = winterambientcolour;
      }
    }

    if(timeofday > 21600) {
      if(timeofday < 64800) {
        float thisangle = (timeofday + 21600) / (float)43200 * M_PI;
        sundirection.z = -std::cos(thisangle);
        sundirection.y = -std::sin(thisangle);
      } else {
        sundirection.z = -1;
        sundirection.y = 0;
        sundiffuse.r = sundiffuse.g = sundiffuse.b = std::sin((timeofday - 32400) / (float)43200 * M_PI);
        sunspecular = sundiffuse;
      }
    } else {
      sundirection.z = 1;
      sundirection.y = 0;
      sundiffuse.r = sundiffuse.g = sundiffuse.b = std::sin(timeofday / (float)43200 * M_PI);
      sunspecular = sundiffuse;
    }
    //std::cout << "Sun coords " << sundirection.z << " " << sundirection.y << " " << " brightness " << (float)sundiffuse.r << std::endl;

    // update meteorological visuals here
    float skybrightness = std::sin(timeofday / (float)86400 * M_PI);
    glFogfv(GL_FOG_COLOR, fogcolour);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientcolour * skybrightness);
    glClearColor(clearcolour.r, clearcolour.g, clearcolour.b, 1);
    //glLightfv(GL_LIGHT0, GL_POSITION, sundirection);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  sunambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  sundiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunspecular);

    // iterate through the courses
    for(int i=0; i < numcourses; ++i) {
      course[i]->update(updatetime);
    }
    for(std::vector<feature*>::iterator i = features.begin(); i != features.end(); ++i) {
      (*i)->update(timespeed, updatetime);
    }
    // this is a decent place for an FPS counter because of the time delay
    std::cout << "FPS: " << (int)(1 / timedeltaaverage) << " Time: " << (int)((int)timeofday / 3600) << ":" << (int)(((int)timeofday / 60) % 60) << ":" << (int)((int)timeofday % 60) << " Day: " << calendardate << " Rate: " << timespeed << "x" << std::endl;
    std::cout << "Coords: " << player->bodyposition.x << " " << player->bodyposition.y << " " << player->bodyposition.z << std::endl;

    // intro time zoom
    if(introon) {
      if(timespeed > 3600) {
        timespeed -= (timespeed * 0.5 * updatetime);
      } else {
        timespeed = 1;
        updatetime = 5;
        introon = false;
      }
    }
  }

  // iterate through the players on this planet
  for(std::vector<golfer*>::iterator i = players.begin(); i != players.end(); ++i) {
    (*i)->update(timedelta);
  }

  // iterate through the items and update those
  for(std::vector<holdable*>::iterator i = items.begin(); i != items.end(); ++i) {
    (*i)->update(timedelta * physicsspeed);
  }

  // finally update particles, assuming there's enough time
  //std::cout << "particles.size() u " << particles.size() << std::endl;
  //if(particles.size() > 0) {
    for(std::vector<particle*>::iterator i = particles.begin(); i != particles.end(); ++i) {
      //std::cout << "Checking " << *i << std::endl;
      if((*i)->age > (*i)->lifespan) {
        //std::cout << "  Erasing " << *i << std::endl;
        //items.erase(i);
        //particles.erase(i);
        //delete *i;
      } else {
        //std::cout << "  Updating " << *i << std::endl;
        (*i)->update(timedelta);
        //(*i)->update(timedelta / 100);                                          // DEBUG
      }
    }
  //}
}

void universe::update(double timedelta) {
  /// Runs physics update for every planet in this universe
  // search the universe for worlds to update
  for(int p = 0; p < numplanets; ++p) {
    planet[p]->update(timedelta);
  }
}
