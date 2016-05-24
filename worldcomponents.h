#ifndef WORLDCOMPONENTS_H_INCLUDED
#define WORLDCOMPONENTS_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vmath.h"

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

  Vector3d origin;                                                              // where this is located in the world

  Vector3d teeposition;                                                         // where we tee off from
  Vector3d holeposition;                                                        // where the hole is

  golfcourse(world *parent, Vector3d tee, Vector3d hole, unsigned int randomseed);

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
  Vector3d windvelocity;                                                        // also used for air resistance (m/s)

  Vector4f grasscolour;                                                         // colour of the grass / snow / whatever
  Vector4f skycolour;                                                           // colour of the sky lid
  Vector4f clearcolour;                                                         // colour of the opengl background
  Vector4f fogcolour;                                                           // colour of the distance fog
  Vector4f ambientcolour;                                                       // colour of the backgrass light

  Vector4f summergrasscolour;                                                   // seasonal colours
  Vector4f summerskycolour;
  Vector4f summerfogcolour;
  Vector4f summerclearcolour;
  Vector4f summerambientcolour;
  Vector4f wintergrasscolour;
  Vector4f winterskycolour;
  Vector4f winterfogcolour;
  Vector4f winterclearcolour;
  Vector4f winterambientcolour;

  Vector3f sundirection;                                                        // vector representing sun angle
  Vector4f sunambient;                                                          // sun light colours
  Vector4f sundiffuse;
  Vector4f sunspecular;

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
  void addcourse(int coursenum, Vector3d teeposition, Vector3d holeposition, unsigned int randomseed);
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
