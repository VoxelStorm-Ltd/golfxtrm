#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/gl.h>
#include "vmath.h"

class terrain {                 /// object for handling the terrain heightmap
public:
  Vector3d origin;            // the map's origin's coordinates
  Vector3d bounds;            // the map's bounding box

  double heightmap[256*256];  // the heightmap data
  int gridwidth;              // the x and z resolution of the heightmap as above

  int randomseed;             // the fixed seed for this course

  GLuint vao;                 // vertex array object
  GLuint vbo;                 // vertex buffer object
  GLuint ibo;                 // element buffer object (index buffer object)
  GLuint numtris;           // number of triangles in the index

  terrain() {                            /// default constructor
    origin.x = 0;
    origin.y = 0;
    origin.z = 0;
    bounds.x = 200;
    bounds.z = 200;
    bounds.y = 60;
    gridwidth = 256;

    randomseed = 1337;

    // initialise the heightmap
    srand(randomseed);                  // seed the random generator predictably
    double xcentre = gridwidth / 2;
    double zcentre = gridwidth / 2;
    for(int x = 0; x < gridwidth; ++x) {
      for(int z = 0; z < gridwidth; ++z) {
        if((x == 0) || (x == gridwidth - 1) || (z == 0) || (z == gridwidth - 1)) {
          heightmap[(x * gridwidth) + z] = 0;   // keep the edge skirt down for smoothness
        } else {
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

    vao = vbo = ibo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    update_vbo(); // generate the vbo ready for first run

    glBindVertexArray(vao);             // set up the VAO's state
    glBindBuffer(GL_ARRAY_BUFFER,             vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, ibo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);
  }

  void update_vbo() {   /// update the VBO from the current grid heightmap
    GLfloat  vbodata[gridwidth * gridwidth * 3];
    std::vector<GLuint> indices;
    numtris = 0;

    for(int xgrid = 0; xgrid < gridwidth; ++xgrid) {
      for(int zgrid = 0; zgrid < gridwidth; ++zgrid) {
        // populate the vertex locations
        int vbo_offset = (xgrid * gridwidth * 3) + (zgrid * 3);
        vbodata[vbo_offset    ] = origin.x + ((double)xgrid / gridwidth * bounds.x);
        vbodata[vbo_offset + 1] = heightmap[(xgrid * gridwidth) + zgrid];
        vbodata[vbo_offset + 2] = origin.z + ((double)zgrid / gridwidth * bounds.z);
        //std::cout << "DEBUG: " << vbodata[vbo_offset    ] << " " << vbodata[vbo_offset + 1] << " " << vbodata[vbo_offset + 2] << std::endl;

        // populate the triangles
        if((xgrid < gridwidth - 1) && (zgrid < gridwidth - 1)) {
          indices.push_back(( xgrid      * (gridwidth)) +  zgrid     );
          indices.push_back(( xgrid      * (gridwidth)) + (zgrid + 1));
          indices.push_back(((xgrid + 1) * (gridwidth)) + (zgrid + 1));
          indices.push_back(((xgrid + 1) * (gridwidth)) + (zgrid + 1));
          indices.push_back(((xgrid + 1) * (gridwidth)) +  zgrid     );
          indices.push_back(( xgrid      * (gridwidth)) +  zgrid     );
          numtris += 2;
        }
      }
    }

    glBindBuffer(GL_ARRAY_BUFFER,         vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata), vbodata, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numtris * 3 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,         0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
    render5();
  }

  void render1() {      /// draw the terrain based on world coords (immediate mode)
    double xpolysize = bounds.x / gridwidth;
    double zpolysize = bounds.z / gridwidth;
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

  void render2() {      /// draw the terrain based on grid coords (immediate mode)
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

  void render3() {      /// draw the terrain using an indexed VBO
    glColor4f(0.75, 0.75, 0.25, 1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void render4() {      /// draw the terrain using an indexed VBO with VAA
    glColor4f(0.75, 0.75, 0.25, 1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void render5() {      /// draw the terrain using an indexed VBO with VAA and VAO
    glColor4f(0.75, 0.75, 0.25, 1);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};


#endif // TERRAIN_H_INCLUDED
