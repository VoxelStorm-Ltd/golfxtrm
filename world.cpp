#include "worldcomponents.h"
#include "terrain.h"

world::world() {                                 /// default constructor
  std::cout << "    Initialising new planet..." << std::endl;
  updatenexttime = 0;                 // this is ready for an update asap

  gravity = 9.800;
  airdensity = 1.2041;
  horizondistance = 1200;

  #ifdef INTRO
  introon = true;
  #else
  introon = false;
  #endif

  if(introon) {
    timespeed = 31556926;   // 1 year per second
    timeofday = 18 * 60 * 60;
    calendardate = 100;

    //featureupdatefreq = (double)1/(double)5;
    featureupdatefreq = (double)60;
  } else {
    //timespeed = 0;          // frozen
    //timespeed = 1;          // realtime
    timespeed = 60;         // 1 minute per second
    //timespeed = 3600;       // 1 hour per second
    //timespeed = 7200;       // 2 hours per second
    //timespeed = 86400;      // 1 day per second
    //timespeed = 2592000;    // 1 month per second
    //timespeed = 31556926;   // 1 year per second
    timeofday = 5 * 60 * 60;    // 8am
    calendardate = 150;

    featureupdatefreq = (double)1/(double)5;
    //featureupdatefreq = 10;
  }
  updatetime = 1 / featureupdatefreq; // time from frequency

  summergrasscolour   = Vector4f(0.75, 0.75, 0.25, 1);
  summerskycolour     = Vector4f(0.90, 0.95, 0.67, 1);
  summerfogcolour     = Vector4f(0.98, 0.92, 0.50, 1);
  summerclearcolour   = summerskycolour;
  summerambientcolour = Vector4f(0.7, 0.7, 0.7, 1);
  wintergrasscolour   = Vector4f(1, 1, 1, 1);
  winterskycolour     = Vector4f(0.3, 0.6, 1, 1);
  winterfogcolour     = Vector4f(0.9, 0.95, 1, 1);
  winterclearcolour   = winterfogcolour;
  winterambientcolour = Vector4f(0.9, 0.9, 0.85, 1);

  grasscolour   = summergrasscolour;
  skycolour     = summerskycolour;
  fogcolour     = summerfogcolour;
  clearcolour   = summerclearcolour;
  ambientcolour = summerambientcolour;

  sundirection = Vector3f(0, 0, 1);
  sunambient   = Vector4f(0, 0, 0, 1);
  sundiffuse   = Vector4f(1, 1, 1, 1);
  sunspecular  = Vector4f(1, 1, 1, 1);

  //windvelocity.x = 10;

  numcourses = 0;
  //addcourse(0, Vector3d(0,0,0), Vector3d(50,0,50));   // no point having less than 1 course
  std::cout << "    Planet initialised" << std::endl;
}

void world::addcourse(int coursenum, Vector3d teeposition, Vector3d holeposition) {
  /// add a golf course to this planet
  course[coursenum] = new golfcourse(this, teeposition, holeposition);
  ++numcourses;
}

golfcourse *world::get_course_at(double x, double z) {
  // iterate through the courses
  for(int i=0; i < numcourses; ++i) {
    // check the bounding coordinates
    double courseoriginx = course[i]->landscape->origin.x;
    double courseoriginz = course[i]->landscape->origin.z;
    if(x > courseoriginx &&
       z > courseoriginz &&
       x < course[i]->landscape->bounds.x - courseoriginx &&
       z < course[i]->landscape->bounds.z - courseoriginz) {
      return course[i];
    }
  }
  return NULL;
}

double world::get_height_at(double x, double z) {
  golfcourse *thiscourse = get_course_at(x, z);
  if(thiscourse != NULL) {
    return thiscourse->get_height_at(x, z);
  }
  return 0;
}

double world::get_friction_at(double x, double z) {
  golfcourse *thiscourse = get_course_at(x, z);
  if(thiscourse != NULL) {
    return thiscourse->get_friction_at(x, z);
  }
  return 0.0409;
}

double world::get_hardness_at(double x, double z) {
  golfcourse *thiscourse = get_course_at(x, z);
  if(thiscourse != NULL) {
    return thiscourse->get_hardness_at(x, z);
  }
  return 10;
}

double world::get_grass_depth_at(double x, double z) {
  golfcourse *thiscourse = get_course_at(x, z);
  if(thiscourse != NULL) {
    return thiscourse->get_grass_depth_at(x, z);
  }
  return 0.1;
}

double world::get_min_velocity_at(double x, double z) {
  golfcourse *thiscourse = get_course_at(x, z);
  if(thiscourse != NULL) {
    return thiscourse->get_min_velocity_at(x, z);
  }
  return 0.1;
}
