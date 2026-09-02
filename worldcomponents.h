#ifndef WORLDCOMPONENTS_H_INCLUDED
#define WORLDCOMPONENTS_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vectorstorm/vector/vector3.h"
#include "vectorstorm/vector/vector4.h"

class golfer;                                                                   // forward decs
class holdable;
class feature;
class world;
class particle;
class terrain;

class golfcourse {  /// the overall landscape object
public:
  world *parentplanet;
  terrain *landscape;

  vector3d origin;                                                              // where this is located in the world

  vector3d teeposition;                                                         // where we tee off from
  vector3d holeposition;                                                        // where the hole is

  golfcourse(world *parent, vector3d const &tee, vector3d const &hole, unsigned int randomseed);

  void update(double timedelta);
  void render();

  double get_height_at(double x, double z);
  double get_friction_at(double x, double z);
  double get_hardness_at(double x, double z);
  double get_grass_depth_at(double x, double z);
  double get_min_velocity_at(double x, double z);
};

class world {     /// world (planet) objects containing golf courses
public:
  std::vector<golfcourse*> course;

  int numcourses;
  double horizondistance;
  double skyheight;

  double featureupdatefreq;                                                     // how often to update features
  double updatetime;                                                            // how long to wait between updates (1/above)
  double updatenexttime;                                                        // what time the next update is due

  double timespeed;                                                             // how fast time passes on this world
  double timeofday;                                                             // in seconds since midnight
  int calendardate;                                                             // days since we started

  bool introon;                                                                 // whether to run a world-gen intro period

  double gravity;                                                               // downward acceleration in m/s^2
  double airdensity;                                                            // ya rly, for drag calculations (kg/m^3)
  vector3d windvelocity;                                                        // also used for air resistance (m/s)

  vector4f grasscolour;                                                         // colour of the grass / snow / whatever
  vector4f skycolour;                                                           // colour of the sky lid
  vector4f clearcolour;                                                         // colour of the opengl background
  vector4f fogcolour;                                                           // colour of the distance fog
  vector4f ambientcolour;                                                       // colour of the backgrass light

  vector4f summergrasscolour;                                                   // seasonal colours
  vector4f summerskycolour;
  vector4f summerfogcolour;
  vector4f summerclearcolour;
  vector4f summerambientcolour;
  vector4f wintergrasscolour;
  vector4f winterskycolour;
  vector4f winterfogcolour;
  vector4f winterclearcolour;
  vector4f winterambientcolour;

  vector3f sundirection;                                                        // vector representing sun angle
  vector4f sunambient;                                                          // sun light colours
  vector4f sundiffuse;
  vector4f sunspecular;

  std::vector<golfer*> players;                                                 // all the players on this planet
  std::vector<holdable*> items;                                                 // all the loose items on this planet
  std::vector<feature*> features;                                               // all the permanent fixtures (trees etc)
  std::vector<particle*> particles;// all the particles and temporary effects

  GLuint vao;                                                                   // vertex array object
  GLuint vbo;                                                                   // vertex buffer object
  GLuint vbo_n;                                                                 // vertex buffer object for normals
  GLuint ibo;                                                                   // element buffer object (index buffer object)
  GLuint numtris;                                                               // number of triangles in the index

  GLuint vao_sky;                                                               // vertex array object
  GLuint vbo_sky;                                                               // vertex buffer object
  GLuint vbo_n_sky;                                                             // vertex buffer object for normals
  GLuint ibo_sky;                                                               // element buffer object (index buffer object)
  GLuint numtris_sky;                                                           // number of triangles in the index

  world();
  void addcourse(int coursenum, vector3d const &teeposition, vector3d const &holeposition, unsigned int randomseed);
  golfcourse *get_course_at(double x, double z);
  double get_height_at(double x, double z);
  double get_friction_at(double x, double z);
  double get_hardness_at(double x, double z);
  double get_grass_depth_at(double x, double z);
  double get_min_velocity_at(double x, double z);
  void update(double timedelta);
  void update_vbo();
  void render();
};


class universe {  /// universe objects - there's only one of these - contains worlds
public:
  std::vector<world*> planet;                                                   // planets that belong to this

  int numplanets;

  double updatetime;                                                            // how long to wait between updates (1/above)
  double updatenexttime;                                                        // what time the next update is due

  universe();
  void addplanet(int worldnum);
  void update(double timedelta);
  void render();
};


#endif // WORLDCOMPONENTS_H_INCLUDED
