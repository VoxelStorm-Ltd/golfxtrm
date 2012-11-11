#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <GL/glew.h>
#include <GL/glfw.h>
#include "vmath.h"

class terrain {                 /// object for handling the terrain heightmap
public:
  Vector3d origin;      // the map's origin's coordinates
  Vector3d bounds;      // the map's bounding box

  GLuint vao;           // vertex array object
  GLuint vbo;           // vertex buffer object

  terrain() {                            /// default constructor
    origin.x = 0;
    origin.y = 0;
    origin.z = 0;
    bounds.x = 100;
    bounds.y = 100;
    bounds.z = 100;
  }

  double get_height_at(double x, double z) {
    /// return the ground height at these coordinates
    return (double)0;
  }

  Vector3d get_slope_at(double x, double z) {
    /// return the downward slope vector at this spot
    Vector3d result;

    result.x = (double)0;
    result.y = (double)1;
    result.z = (double)0;

    return result;
  }

  void render() {                         /// draw the terrain grid
    double polysize = 1;
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(0.75, 0.75, 0.25, 1);
    for(double x = origin.x; x < bounds.x; x += polysize) {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3i(0, 1, 0);
      for(double z = origin.z; z < bounds.z; z += polysize) {
        glVertex3d(x,            origin.y, z);
        glVertex3d(x + polysize, origin.y, z);
      }
      glEnd();
    }
  }
};


#endif // TERRAIN_H_INCLUDED
