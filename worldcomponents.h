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

  golfcourse(world *parent, Vector3d tee, Vector3d hole)
    : teeposition(tee), holeposition(hole) {                      /// default constructor
    std::cout << "      Initialising new golf course..." << std::endl;
    parentplanet = parent;
    landscape = new terrain(teeposition, holeposition);
    std::cout << "      Golf course initialised" << std::endl;
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

  double featureupdatefreq;       // how often to update features
  double updatetime;              // how long to wait between updates (1/above)
  double updatenexttime;          // what time the next update is due

  double timespeed;               // how fast time passes on this world
  double timeofday;               // in seconds since midnight
  int calendardate;               // days since we started

  bool introon;                   // whether to run a world-gen intro period

  double gravity;                       // downward acceleration in m/s^2
  double airdensity;                    // ya rly, for drag calculations (kg/m^3)
  Vector3d windvelocity;                // also used for air resistance (m/s)

  Vector4f grasscolour;                // colour of the grass / snow / whatever
  Vector4f skycolour;                   // colour of the sky lid
  Vector4f clearcolour;                 // colour of the opengl background
  Vector4f fogcolour;                   // colour of the distance fog
  Vector4f ambientcolour;               // colour of the backgrass light

  Vector4f summergrasscolour;          // seasonal colours
  Vector4f summerskycolour;
  Vector4f summerfogcolour;
  Vector4f summerclearcolour;
  Vector4f summerambientcolour;
  Vector4f wintergrasscolour;
  Vector4f winterskycolour;
  Vector4f winterfogcolour;
  Vector4f winterclearcolour;
  Vector4f winterambientcolour;

  Vector3f sundirection;                // vector representing sun angle
  Vector4f sunambient;                  // sun light colours
  Vector4f sundiffuse;
  Vector4f sunspecular;

  boost::ptr_vector<golfer> players;    // all the players on this planet
  boost::ptr_vector<holdable> items;    // all the loose items on this planet
  boost::ptr_vector<feature> features;  // all the permanent fixtures (trees etc)

  world() {                                 /// default constructor
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
    addcourse(0, Vector3d(0,0,0), Vector3d(50,0,50));   // no point having less than 1 course
    std::cout << "    Planet initialised" << std::endl;
  }

  void addcourse(int coursenum, Vector3d teeposition, Vector3d holeposition) {
    /// add a golf course to this planet
    course[coursenum] = new golfcourse(this, teeposition, holeposition);
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
    std::cout << "  Initialising the universe" << std::endl;
    // big bang!
    numplanets = 0;

    updatetime = 1 / updatefreq;  // set this from the global
    updatenexttime = 0;           // this is ready for an update asap
    std::cout << "  Universe initialised" << std::endl;
  }

  void addplanet(int worldnum) {            /// add a planet to this universe
    planet[worldnum] = new world();
    ++numplanets;
  }

  void update(double timedelta);
  void render();
};


#endif // WORLDCOMPONENTS_H_INCLUDED
