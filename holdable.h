#ifndef HOLDABLE_H_INCLUDED
#define HOLDABLE_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vectorstorm/vector/vector3.h"
#include "vectorstorm/quat/quat.h"

class golfer;                                                                   // forward dec
class world;

class holdable {
public:
  golfer *held_by;                                                              // who is holding this, NULL for lying/flying in the world
  bool at_rest;                                                                 // whether it's settled (in which case don't do further physics with it)

  double mass;                                                                  // how much it weighs (kg)
  double momentofinertia;                                                       // moment of inertia (hand-hold as fulcrum)
  vector3d cog;                                                                 // centre of gravity relative to hand-hold
  double airresistance;                                                         // how much its flight is slowed by atmospheric drag
  double slideresistance;                                                       // how much its slide along the ground is slowed

  double cda;                                                                   // coefficient of drag * crossectional area

  std::string name;                                                             // what it's called
  std::string description;                                                      // longer description

  vector3d bbox_start;                                                          // one corner of the bounding box
  vector3d bbox_end;                                                            // other corner of the bounding box

  double boundingradius;                                                        // collision radius for sphere type collisions

  world *currentplanet;                                                         // what planet are you even on?

  vector3d position;                                                            // location in space
  vector3d velocity;                                                            // movement velocity
  quatd rotation;                                                               // current rotation
  quatd angularvelocity;                                                        // rotational velocity

  GLuint vao;                                                                   // vertex array object
  GLuint vbo;
  GLuint vbo_n;
  GLuint ibo;
  GLuint numtris;                                                               // number of triangles in the VBO

  enum axistype {
    AXIS_X,
    AXIS_Y,
    AXIS_Z
  };

  holdable();
  holdable(world *parentplanet);
  virtual ~holdable();
  virtual void push(vector3d const &impulse);
  virtual void push(vector3d const &impulse, vector3d const &impactpoint);
  virtual void impact(holdable *target, golfer *actor, double distance);
  virtual void update(double timedelta);
  virtual void rotate(axistype axis, double angle);
  virtual void render();
  virtual void renderlocal();
};

class golfclub : public holdable {
public:

  golfclub(world *parentplanet);
  ~golfclub();
  void render();
  void renderlocal();
  void impact(holdable *target, golfer *actor, double distance);
};

class golfball : public holdable {
public:
  double radius;

  golfball(world *parentplanet);
  ~golfball();
  void render();
  void renderlocal();
};
#endif // HOLDABLE_H_INCLUDED
