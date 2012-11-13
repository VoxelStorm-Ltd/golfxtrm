#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <GL/glew.h>
#include <GL/glfw.h>
#include "vmath.h"

class terrain {                 /// object for handling the terrain heightmap
public:
  Vector3d origin;      // the map's origin's coordinates
  Vector3d bounds;      // the map's bounding box

  double heightmap[256*256];  // the heightmap data
  int gridwidth;        // the x and z resolution of the heightmap as above

  int randomseed;       // the fixed seed for this course

  GLuint vao;           // vertex array object
  GLuint vbo;           // vertex buffer object

  terrain() {                            /// default constructor
    origin.x = 0;
    origin.y = 0;
    origin.z = 0;
    bounds.x = 200;
    bounds.y = 60;
    bounds.z = 200;
    gridwidth = 256;

    randomseed = 1337;

    // initialise the heightmap
    srand(randomseed);                  // seed the random generator predictably
    for(int x = 0; x < gridwidth; ++x) {
      for(int z = 0; z < gridwidth; ++z) {
        if((x == 0) || (x == gridwidth - 1) || (z == 0) || (z == gridwidth - 1)) {
          heightmap[(x * gridwidth) + z] = 0;   // keep the edge skirt down for smoothness
        } else {
          double xcentre = gridwidth / 2;
          double zcentre = gridwidth / 2;
          //double centredist = sqrt(pow(x - xcentre, (double)2) + pow(z - zcentre, (double)2));
          double centredist = (pow(x - xcentre, (double)2) + pow(z - zcentre, (double)2)) / gridwidth * 20;
          double offsetheight = 10 - (centredist/(gridwidth/2));
          if(offsetheight > 0) {
            heightmap[(x * gridwidth) + z] = offsetheight + (((double)rand()/(double)RAND_MAX) * 0.25);
          } else {
            heightmap[(x * gridwidth) + z] = 0;
          }

        }
      }
    }
  }

  double get_height_at(double x, double z) {
    /// return the ground height at these coordinates
    // check if the coords are inside our bounds or not
    if(x >= origin.x && x <= origin.x + bounds.x &&
       z >= origin.z && z <= origin.z + bounds.z) {
      // interpolate real x and z coords to the grid
      int xgrid = (x - origin.x) / bounds.x * gridwidth;
      int zgrid = (z - origin.z) / bounds.z * gridwidth;
      return origin.y + heightmap[(xgrid * gridwidth) + zgrid];
    } else {
      return origin.y;
    }
  }

  Vector3d get_slope_at(double x, double z) {
    /// return the downward slope vector at this spot
    Vector3d result;

    result.x = (double)0;
    result.y = (double)1;
    result.z = (double)0;

    return result;
  }

  double get_hardness_at(double x, double z) {
    /// return springiness (per-second) coefficient
    // FPS = player/ball reacts to all slopes instantly
    // 1 = player/ball reacts to all slopes within 1 sec
    // <0.98 = player/ball start to sink (> gravity)
    return 10;   // how quickly things come back up from the ground
  }

  void update(double timedelta) {
    // nothing to do here currently
  }

  double get_friction_at(double x, double z) {
    /// return the coefficient of friction at the current spot
    return 0.0409;    // from physics forum for ball on golf green
  }

  void render() {
    /// alias function to render the terrain using the preferred method
    render1();
    render2();
  }

  void render1() {     /// draw the terrain based on world coords
    double xpolysize = bounds.x / gridwidth;
    double zpolysize = bounds.z / gridwidth;
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(0.75, 0.75, 0.25, 1);
    for(double x = origin.x; x < origin.x + bounds.x - xpolysize; x += xpolysize) {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3i(0, 1, 0);
      for(double z = origin.z; z < origin.z + bounds.z; z += zpolysize) {
        glVertex3d(x, origin.y + get_height_at(x, z), z);
        glVertex3d(x + xpolysize, origin.y + get_height_at(x + xpolysize, z), z);
      }
      glEnd();
    }
  }

  void render2() {     /// draw the terrain based on grid coords
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(0.75, 0.75, 0.25, 1);
    for(int xgrid = 0; xgrid < gridwidth - 1; ++xgrid) {
      double xpolysize = bounds.x / gridwidth;
      double x = origin.x + (xgrid * xpolysize);
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3i(0, 1, 0);
      for(int zgrid = 0; zgrid < gridwidth; ++zgrid) {
        double zpolysize = bounds.z / gridwidth;
        double z = origin.z + (zgrid * zpolysize);
        double y1 = origin.y + heightmap[(xgrid * gridwidth) + zgrid];
        double y2 = origin.y + heightmap[((xgrid + 1) * gridwidth) + zgrid];
        glVertex3d(x, y1, z);
        glVertex3d(x + xpolysize, y2, z);
      }
      glEnd();
    }
  }
};


#endif // TERRAIN_H_INCLUDED
