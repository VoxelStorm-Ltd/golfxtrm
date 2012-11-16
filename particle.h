#ifndef PARTICLE_H
#define PARTICLE_H

#include <GL/glew.h>
#include "vmath.h"

class world;  // forward dec

class particle {
public:
  Vector3d position;        // where it is
  Vector3d velocity;        // where it's going
  Quatd rotation;           // how it's oriented

  double size;              // relative size modifier

  double age;               // how long it's been around, in seconds
  double lifespan;          // cull it past this age

  GLuint vao;               // vertex array object
  GLuint vbo;
  GLuint vbo_n;
  GLuint ibo;
  GLuint numtris;           // number of triangles in the VBO

  world *currentplanet;     // what world this belongs to

  particle();
  particle(world *parentplanet);
  virtual ~particle();
  virtual void render();
};

class golfballtrail : particle {
public:
  golfballtrail();
  golfballtrail(Vector3d start, Vector3d end);
  virtual ~golfballtrail();
};


#endif // PARTICLE_H
