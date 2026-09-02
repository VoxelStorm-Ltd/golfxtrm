#ifndef LANDSCAPE_FEATURES_H_INCLUDED
#define LANDSCAPE_FEATURES_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vectorstorm/vector/vector3.h"
#include "vectorstorm/quat/quat.h"

class world;                                                                    // forward dec

class feature {
public:
  vector3d position;                                                            // where its origin is in the world
  quatd rotation;                                                               // how it's oriented
  vector3d bbox_start;                                                          // one corner of the bounding box
  vector3d bbox_end;                                                            // other corner of the bounding box

  vector3d collisionoffset;                                                     // centre of the collision sphere
  double boundingradius;                                                        // bounding sphere radius

  std::string name;                                                             // what it's called
  std::string description;                                                      // longer description

  world *currentplanet;                                                         // what planet are you even on?

  int randomseed;                                                               // the fixed seed for this object

  GLuint vao;                                                                   // vertex array object
  GLuint vbo;                                                                   // vertex buffer object
  GLuint vbo_n;                                                                 // vertex buffer object for normals
  GLuint ibo;                                                                   // index buffer object
  GLuint numtris;                                                               // number of triangles in the VBO

  enum featuretype {
    TEE,
    HOLE,
    FLAG,
    FIRTREE,
    FIRTREESAPLING,
    OAKTREE,
    OAKTREESAPLING,
    ASHTREE
  };

  feature();
  virtual ~feature();
  virtual void update(double timespeed, double timedelta);
  virtual void render();
};


class firtree : public feature {
public:
  float maxheight;                                                              // the tallest these can get
  float maxwidth;                                                               // the widest they can get
  double growthrate;                                                            // how fast it grows in % per second

  float height;                                                                 // overall height
  float bottom;                                                                 // bottom of the foliage from the ground
  float width;                                                                  // width of the bottom section / 2
  float trunkwidth;                                                             // thickness of the trunk / 2

  enum firtreetype {
    FIRTREE_STANDARD,
    FIRTREE_RANDOM,
    FIRTREE_SAPLING,
    FIRTREE_SAPLING_RANDOM
  };

  firtree(world *parentplanet, double x, double y, double z, firtreetype treetype, int rseed);
  virtual ~firtree();
  void updatevbo();
  void update(double timespeed, double timedelta);
  void grow(double amount);
  void render();
};


class oaktree : public feature {
public:
  float maxheight;                                                              // the tallest these can get
  float maxwidth;                                                               // the widest they can get
  double growthrate;                                                            // how fast it grows in % per second

  float height;                                                                 // overall height
  float bottom;                                                                 // bottom of the foliage from the ground
  float width;                                                                  // width of the bottom section / 2
  float trunkwidth;                                                             // thickness of the trunk / 2

  enum oaktreetype {
    OAKTREE_STANDARD,
    OAKTREE_RANDOM,
    OAKTREE_SAPLING,
    OAKTREE_SAPLING_RANDOM
  };

  oaktree(world *parentplanet, double x, double y, double z, oaktreetype treetype, int rseed);
  virtual ~oaktree();
  void updatevbo();
  void update(double timespeed, double timedelta);
  void grow(double amount);
  void render();
};


class ashtree : public feature {
public:
  float maxheight;                                                              // the tallest these can get
  float maxwidth;                                                               // the widest they can get
  double growthrate;                                                            // how fast it grows in % per second

  float height;                                                                 // overall height
  float bottom;                                                                 // bottom of the foliage from the ground
  float width;                                                                  // width of the bottom section / 2
  float trunkwidth;                                                             // thickness of the trunk / 2

  struct branch {
    vector3f start;
    vector3f end;
    float thickness;
    bool grow_now;
    //branch *parent;
    unsigned int parent;                                                        // index
  };

  ashtree(world *parentplanet, double x, double y, double z, int rseed);
  virtual ~ashtree();
  void updatevbo();
  void update(double timespeed, double timedelta);
  void grow(double amount);
  void render();
};


#endif // LANDSCAPE_FEATURES
