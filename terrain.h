#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/gl.h>
#include "vmath.h"
#include "globalvars_client_extern.h"

class terrain {                 /// object for handling the terrain heightmap
public:
  Vector3d origin;            // the map's origin's coordinates
  Vector3d bounds;            // the map's bounding box

  double heightmap[256*256];  // the heightmap data
  int gridwidth;              // the x and z resolution of the heightmap as above

  int randomseed;             // the fixed seed for this course

  GLuint vao;                 // vertex array object
  GLuint vbo;                 // vertex buffer object
  GLuint vbo_n;               // vertex buffer object for normals
  GLuint ibo;                 // element buffer object (index buffer object)
  GLuint numtris;             // number of triangles in the index

  terrain(Vector3d teeposition, Vector3d holeposition) {  /// default constructor
    std::cout << "        Initialising terrain..." << std::endl;
    origin.x = 0;
    origin.y = 0;
    origin.z = 0;
    bounds.x = 200;
    bounds.z = 200;
    bounds.y = 60;
    gridwidth = 128;

    randomseed = 1337;

    // initialise the heightmap
    std::cout << "          Generating heightmap..." << std::endl;
    srand(randomseed);                  // seed the random generator predictably
    double xcentre = gridwidth / 2;
    double zcentre = gridwidth / 2;

    double heightscale = 20;

    for(int x = 0; x < gridwidth; ++x) {
      for(int z = 0; z < gridwidth; ++z) {
        if((x == 0) || (x == gridwidth - 1) || (z == 0) || (z == gridwidth - 1)) {
          heightmap[(x * gridwidth) + z] = 0;   // keep the edge skirt down for smoothness
        } else {
          //double centredist = sqrt(pow(x - xcentre, (double)2) + pow(z - zcentre, (double)2));
          double centredist_sq = (pow(x - xcentre, (double)2) + pow(z - zcentre, (double)2));
          double offsetheight = heightscale - ((centredist_sq / gridwidth * heightscale * 2) / (gridwidth / 2));

          double holedist_sq = pow(x - holeposition.x, (double)2) + pow(z - holeposition.z, (double)2);
          if(holedist_sq < 100) {
            offsetheight = holeposition.y;
          }

          if(offsetheight > 0) {
            heightmap[(x * gridwidth) + z] = offsetheight + (((double)rand()/(double)RAND_MAX) * 0.25);
          } else {
            heightmap[(x * gridwidth) + z] = 0;
          }
        }
      }
    }

    vao = vbo = vbo_n = ibo = 0;
    if(hasvao) {
      std::cout << "          Creating VAO..." << std::endl;
      glGenVertexArrays(1, &vao);
    }
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vbo_n);
    glGenBuffers(1, &ibo);
    update_vbo(); // generate the vbo ready for first run

    if(hasvao) {
      glBindVertexArray(vao);             // set up the VAO's state

      /*glBindBuffer(GL_ARRAY_BUFFER,             vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);*/

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glEnableClientState(GL_VERTEX_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glVertexPointer(3, GL_FLOAT, 0, 0);
      glEnableClientState(GL_NORMAL_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
      glNormalPointer(GL_FLOAT, 0, 0);

      glBindVertexArray(0);
    }
    std::cout << "        Terrain initialised" << std::endl;
  }

  void update_vbo() {   /// update the VBO from the current grid heightmap
    GLfloat vbodata[gridwidth * gridwidth * 6];
    GLfloat vbodata_n[gridwidth * gridwidth * 6];
    std::vector<GLuint> indices;
    numtris = 0;

    for(int xgrid = 0; xgrid < gridwidth; ++xgrid) {
      for(int zgrid = 0; zgrid < gridwidth; ++zgrid) {
        // populate the vertex locations
        int vbo_offset = ((xgrid * gridwidth * 3) + (zgrid * 3)) * 2;
        vbodata[vbo_offset    ] = origin.x + ((double)xgrid / gridwidth * bounds.x);
        vbodata[vbo_offset + 1] = heightmap[(xgrid * gridwidth) + zgrid];
        vbodata[vbo_offset + 2] = origin.z + ((double)zgrid / gridwidth * bounds.z);
        vbodata[vbo_offset + 3] = origin.x + ((double)(xgrid + 1) / gridwidth * bounds.x);
        if(xgrid == gridwidth - 1 || zgrid == gridwidth - 1) {
          vbodata[vbo_offset + 4] = 1;
        } else {
          vbodata[vbo_offset + 4] = heightmap[((xgrid + 1) * gridwidth) + (zgrid + 1)];
        }
        vbodata[vbo_offset + 5] = origin.z + ((double)(zgrid + 1 ) / gridwidth * bounds.z);

        // populate the normals (z vector cross product x vector)
        Vector3f thisnormal;
        if(xgrid == gridwidth - 1 || zgrid == gridwidth - 1) {
          thisnormal = Vector3f(0,1,0);
        } else {
          thisnormal = Vector3f(0, heightmap[(xgrid * gridwidth) + (zgrid + 1)] - heightmap[(xgrid * gridwidth) + zgrid], 1)
            .crossProduct(Vector3f(1, heightmap[((xgrid+1) * gridwidth) + zgrid] - heightmap[(xgrid * gridwidth) + zgrid], 0));    // normal is z cross x
          thisnormal.normalize();
        }
        vbodata_n[vbo_offset    ] = thisnormal.x;
        vbodata_n[vbo_offset + 1] = thisnormal.y;
        vbodata_n[vbo_offset + 2] = thisnormal.z;
        vbodata_n[vbo_offset + 3] = thisnormal.x;
        vbodata_n[vbo_offset + 4] = thisnormal.y;
        vbodata_n[vbo_offset + 5] = thisnormal.z;

        // populate the triangles
        if((xgrid < gridwidth - 1) && (zgrid < gridwidth - 1)) {
          /*indices.push_back(( xgrid      * (gridwidth)) +  zgrid     );
          indices.push_back(( xgrid      * (gridwidth)) + (zgrid + 1));
          indices.push_back(((xgrid + 1) * (gridwidth)) + (zgrid + 1));
          indices.push_back(((xgrid + 1) * (gridwidth)) + (zgrid + 1));
          indices.push_back(((xgrid + 1) * (gridwidth)) +  zgrid     );
          indices.push_back(( xgrid      * (gridwidth)) +  zgrid     );*/
          indices.push_back((((xgrid + 1) * (gridwidth)) + (zgrid    )) * 2);
          indices.push_back((( xgrid      * (gridwidth)) + (zgrid + 1)) * 2);
          indices.push_back((( xgrid      * (gridwidth)) +  zgrid     ) * 2);
          indices.push_back((( xgrid      * (gridwidth)) + (zgrid + 1)) * 2    );
          indices.push_back((((xgrid + 1) * (gridwidth)) + (zgrid    )) * 2    );
          indices.push_back((( xgrid      * (gridwidth)) +  zgrid     ) * 2 + 1);
          numtris += 2;
        }
      }
    }

    glBindBuffer(GL_ARRAY_BUFFER,         vbo);
    glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata), vbodata, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,         0);
    glBindBuffer(GL_ARRAY_BUFFER,         vbo_n);
    glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata_n), vbodata_n, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,         0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numtris * 3 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if(hasvao) {
      glBindVertexArray(vao);             // set up the VAO's state

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glEnableClientState(GL_VERTEX_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glVertexPointer(3, GL_FLOAT, 0, 0);
      glEnableClientState(GL_NORMAL_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
      glNormalPointer(GL_FLOAT, 0, 0);

      glBindVertexArray(0);
    }
  }

  double get_height_at(double x, double z) {
    /// return the ground height at these coordinates
    // check if the coords are inside our bounds or not
    if(x >= origin.x && x <= origin.x + bounds.x &&
       z >= origin.z && z <= origin.z + bounds.z) {
      // interpolate real x and z coords to the grid
      //double xgridinterp = (x - origin.x) / bounds.x * gridwidth;
      //double zgridinterp = (z - origin.z) / bounds.z * gridwidth;
      double gridsquarewidth = bounds.x / gridwidth;
      //int xgrid = xgridinterp / gridsquarewidth;
      //int zgrid = zgridinterp / gridsquarewidth;
      int xgrid = (x - origin.x) / gridsquarewidth;
      int zgrid = (z - origin.z) / gridsquarewidth;
      //double xgridremainder = fmod((x - origin.x), gridsquarewidth) / gridsquarewidth;
      //double zgridremainder = fmod((z - origin.z), gridsquarewidth) / gridsquarewidth;

      double g    = heightmap[ (xgrid      * gridwidth) +  zgrid   ];
      double gx   = heightmap[((xgrid + 1) * gridwidth) +  zgrid   ];
      double gz   = heightmap[ (xgrid      * gridwidth) + (zgrid+1)];
      double gxz  = heightmap[((xgrid + 1) * gridwidth) + (zgrid+1)];
      //double igx  = ((gx   - g  ) * xgridremainder) + g;
      //double igzx = ((gxz  - gz ) * xgridremainder) + gz;
      //double ig   = ((igzx - igx) * zgridremainder) + igx;

      //std::cout << "coords " << x << " " << z << " ";
      //std::cout << "grid " << xgrid << " " << zgrid << " ";
      //std::cout << "remainder " << xgridremainder << " " << zgridremainder << std::endl;
      //std::cout << "g, gx, gz, gxz " << g << " " << gx << " " << gz << " " << gxz << std::endl;
      //std::cout << "igx, igzx, ig " << igx << " " << igzx << " " << ig << std::endl;

      //return ig;
      double largestvalue = 0;
      if(g > largestvalue) {
        largestvalue = g;
      }
      if(gx > largestvalue) {
        largestvalue = gx;
      }
      if(gz > largestvalue) {
        largestvalue = gz;
      }
      if(gxz > largestvalue) {
        largestvalue = gxz;
      }
      return largestvalue;
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

  double get_grass_depth_at(double x, double z) {
    /// return the depth of grass, for friction calculations as well as visuals
    return 0.01;    // placeholder
  }

  double get_min_velocity_at(double x, double z) {
    /// return the depth of grass, for friction calculations as well as visuals
    return 0.1;    // placeholder
  }

  void render(Vector4f basecolour) {
    /// alias function to render the terrain using the preferred method
    //render2(basecolour); // DEBUG
    if(hasvao) {
      render5(basecolour);
    } else {
      render3(basecolour);
    }
  }

  void render1(Vector4f basecolour) {      /// draw the terrain based on world coords (immediate mode)
    double xpolysize = bounds.x / gridwidth;
    double zpolysize = bounds.z / gridwidth;
    glColor4fv(basecolour);
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

  void render2(Vector4f basecolour) {      /// draw the terrain based on grid coords (immediate mode)
    glColor4fv(basecolour);
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

  void render3(Vector4f basecolour) {      /// draw the terrain using an indexed VBO
    glColor4fv(basecolour);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void render4(Vector4f basecolour) {      /// draw the terrain using an indexed VBO with VAA
    glColor4fv(basecolour);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void render5(Vector4f basecolour) {      /// draw the terrain using an indexed VBO with VAO
    glColor4fv(basecolour);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};


#endif // TERRAIN_H_INCLUDED
