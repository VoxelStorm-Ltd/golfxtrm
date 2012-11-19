#include <cstdlib>
#include "terrain.h"
#include "globalvars_client_extern.h"
#include "perlin.h"
#include "landscape_features.h"
#include "worldcomponents.h"

terrain::terrain(golfcourse *parentcourse, Vector3d teeposition, Vector3d holeposition) {  /// default constructor
  std::cout << "        Initialising terrain..." << std::endl;
  origin.x = 0;
  origin.y = 0;
  origin.z = 0;
  bounds.x = 400;
  bounds.z = 400;
  bounds.y = 60;
  gridwidth = 200;

  parent = parentcourse;

  randomseed = 1337;

  // initialise the heightmap
  std::cout << "          Generating heightmap..." << std::endl;
  //heightmap = new double*[gridwidth*gridwidth];
  srand(randomseed);                  // seed the random generator predictably

  Perlin *testmap = new Perlin(4,            // octaves - 1 to 16 (~4-8)
                               4,            // noise freq (~1-8)
                               1,            // amplitude (1 returns -1 to 1)
                               randomseed+1);  // seed

  Perlin *largemap = new Perlin(4, 4, 1, randomseed);
  Perlin *smoothmap = new Perlin(2, 4, 1, randomseed);
  Perlin *lumpy = new Perlin(8, 8, 1, randomseed);
  Perlin *roughnessmap = new Perlin(4, 4, 1, randomseed+1);

  double xcentre = gridwidth / 2;
  double zcentre = gridwidth / 2;

  double heightscale = 10;

  double holepositiongridx = holeposition.x / bounds.x * (double)gridwidth;
  double holepositiongridz = holeposition.z / bounds.z * (double)gridwidth;

  double greensize = 15;
  double greenmargin = 20;

  for(int x = 0; x < gridwidth; ++x) {
    for(int z = 0; z < gridwidth; ++z) {
      if((x == 0) || (x == gridwidth - 1) || (z == 0) || (z == gridwidth - 1)) {
        //heightmap[(x * gridwidth) + z] = 0;   // keep the edge skirt down for smoothness
        heightmap.push_back(0);   // keep the edge skirt down for smoothness
      } else {
        double randfactor = 0;
        ///double randfactor = 0.2;

        //double centredist = sqrt(pow(x - xcentre, (double)2) + pow(z - zcentre, (double)2));
        double centredist_sq = (pow(x - xcentre, (double)2) + pow(z - zcentre, (double)2));
        //double offsetheight = heightscale - ((centredist_sq / gridwidth * heightscale * 2) / (gridwidth / 2));
        double centredist_qu = centredist_sq * centredist_sq;
        double offsetheight = heightscale - ((centredist_qu * heightscale * 2) / (gridwidth * gridwidth * gridwidth * 25));
        //offsetheight *= heightscale;

        float perlinheightresult = largemap->get((float)x / (float)gridwidth, (float)z / (float)gridwidth) + 1;
        float perlinroughnessresult = largemap->get((float)x / (float)gridwidth, (float)z / (float)gridwidth) + 1;

        //offsetheight = perlinheightresult * heightscale;
        offsetheight *= perlinheightresult;
        if(perlinroughnessresult < 1.1 && perlinroughnessresult > 0.9) {
          randfactor = 0.5;
        } else {
          randfactor = 0;
        }

        double holedist = sqrt(pow(x - holepositiongridx, (double)2) + pow(z - holepositiongridz, (double)2));
        if(holedist < greensize / bounds.x * (double)gridwidth) {
          offsetheight = holeposition.y;
          randfactor = 0.01;
        } else if(holedist < (greensize + greenmargin) / bounds.x * (double)gridwidth) {
          double difference = (holedist - (greensize / bounds.x * (double)gridwidth)) / (greenmargin / bounds.x * (double)gridwidth);
          if(difference > 1) {
            difference = 1;
          }
          double thisoffsetheight = offsetheight;
          if(thisoffsetheight < 0) {
            thisoffsetheight = 0;
          }
          double thisdifference = holeposition.y - thisoffsetheight;
          offsetheight = holeposition.y - (difference * thisdifference);
          randfactor = 0.06;
        }

        if(offsetheight > 0) {
          //heightmap[(x * gridwidth) + z] = offsetheight + (((double)rand()/(double)RAND_MAX) * randfactor);
          heightmap.push_back(offsetheight + (((double)rand()/(double)RAND_MAX) * randfactor));
        } else {
          //heightmap[(x * gridwidth) + z] = ((double)rand()/(double)RAND_MAX) * randfactor;
          //heightmap.push_back(((double)rand()/(double)RAND_MAX) * randfactor);
          heightmap.push_back(0);
        }
      }
    }
  }

  vao = vbo = vbo_n = ibo = 0;
  if(hasvao) {
    std::cout << "          Creating VAO..." << std::endl;
    glGenVertexArrays(1, &vao);
  }
  std::cout << "          Generating vertex buffer" << std::endl;
  glGenBuffers(1, &vbo);
  std::cout << "          Generating normal buffer" << std::endl;
  glGenBuffers(1, &vbo_n);
  std::cout << "          Generating index buffer" << std::endl;
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

terrain::~terrain() {   /// destructor

}

void terrain::update_vbo() {   /// update the VBO from the current grid heightmap
  std::cout << "          Assigning buffers" << std::endl;
  std::vector<GLfloat> vbodata;
  vbodata.assign(gridwidth * gridwidth * 6, (GLfloat)0);
  std::vector<GLfloat> vbodata_n;
  vbodata_n.assign(gridwidth * gridwidth * 6, (GLfloat)0);
  std::vector<GLuint> indices;

  numtris = 0;

  std::cout << "          Filling buffers" << std::endl;
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

  std::cout << "          Uploading " << vbodata.size() << " vertices to vertex buffer" << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER,         vbo);
  glBufferData(GL_ARRAY_BUFFER,         vbodata.size() * sizeof(GLfloat), &vbodata[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,         0);
  std::cout << "          Uploading " << vbodata_n.size() << " normals to normal buffer" << std::endl;
  glBindBuffer(GL_ARRAY_BUFFER,         vbo_n);
  glBufferData(GL_ARRAY_BUFFER,         vbodata_n.size() * sizeof(GLfloat), &vbodata_n[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,         0);
  std::cout << "          Uploading " << indices.size() << " indices for " << numtris << " triangles to index buffer" << std::endl;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if(hasvao) {
    std::cout << "          Setting up VAO" << std::endl;
    glBindVertexArray(vao);             // set up the VAO's state

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindVertexArray(0);
  } else {
    std::cout << "          Not using VAO" << std::endl;
  }
}

void terrain::populatefeatures(int randomseed, feature::featuretype whatfeature) {
  /// call the feature populator with the default probability and density settings
  float threshold = 1.3;    // minimum perlin result for a forest
  float probability = 0.5;  // likelihood of making a tree in any one grid square
  populatefeatures(randomseed, whatfeature, threshold, probability);
}

void terrain::populatefeatures(int randomseed, feature::featuretype whatfeature, float threshold, float probability) {
  Perlin *testmap = new Perlin(4,            // octaves - 1 to 16 (~4-8)
                               8,            // noise freq (~1-8)
                               1,            // amplitude (1 returns -1 to 1)
                               randomseed);  // seed


  double gridscale = bounds.x / gridwidth;

  srand(randomseed);

  for(int xgrid = 0; xgrid < gridwidth; ++xgrid) {
    for(int zgrid = 0; zgrid < gridwidth; ++zgrid) {
      float perlinresult = testmap->get((float)xgrid / (float)gridwidth, (float)zgrid / (float)gridwidth) + 1;
      if(perlinresult > threshold && ((float)rand() / RAND_MAX) <= probability) {
        double xpos = (xgrid + ((double)rand() / RAND_MAX)) * gridscale;
        double zpos = (zgrid + ((double)rand() / RAND_MAX)) * gridscale;

        if(whatfeature == feature::FIRTREE) {
          new firtree(parent->parentplanet, xpos, get_height_at(xpos, zpos), zpos, firtree::FIRTREE_RANDOM, rand());
        } else if(whatfeature == feature::FIRTREESAPLING) {
          new firtree(parent->parentplanet, xpos, get_height_at(xpos, zpos), zpos, firtree::FIRTREE_SAPLING_RANDOM, rand());
        } else if(whatfeature == feature::OAKTREE) {
          new oaktree(parent->parentplanet, xpos, get_height_at(xpos, zpos), zpos, oaktree::OAKTREE_RANDOM, rand());
        } else if(whatfeature == feature::OAKTREESAPLING) {
          new oaktree(parent->parentplanet, xpos, get_height_at(xpos, zpos), zpos, oaktree::OAKTREE_SAPLING_RANDOM, rand());
        } else if(whatfeature == feature::ASHTREE) {
          new ashtree(parent->parentplanet, xpos, get_height_at(xpos, zpos), zpos, rand());
        }
      }
    }
  }
}

double terrain::get_height_at(double x, double z) {
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

Vector3d terrain::get_slope_at(double x, double z) {
  /// return the downward slope vector at this spot
  Vector3d result;

  result.x = (double)0;
  result.y = (double)1;
  result.z = (double)0;

  return result;
}

double terrain::get_hardness_at(double x, double z) {
  /// return springiness (per-second) coefficient
  // FPS = player/ball reacts to all slopes instantly
  // 1 = player/ball reacts to all slopes within 1 sec
  // <0.98 = player/ball start to sink (> gravity)
  return 10;   // how quickly things come back up from the ground
}

void terrain::update(double timedelta) {
  // nothing to do here currently
}

double terrain::get_friction_at(double x, double z) {
  /// return the coefficient of friction at the current spot
  return 0.0409;    // from physics forum for ball on golf green
}

double terrain::get_grass_depth_at(double x, double z) {
  /// return the depth of grass, for friction calculations as well as visuals
  return 0.01;    // placeholder
}

double terrain::get_min_velocity_at(double x, double z) {
  /// return the depth of grass, for friction calculations as well as visuals
  return 0.1;    // placeholder
}

void terrain::render(Vector4f basecolour) {
  /// alias function to render the terrain using the preferred method
  //render2(basecolour); // DEBUG
  if(hasvao) {
    render5(basecolour);
  } else {
    render3(basecolour);
  }
}

void terrain::render1(Vector4f basecolour) {      /// draw the terrain based on world coords (immediate mode)
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

void terrain::render2(Vector4f basecolour) {      /// draw the terrain based on grid coords (immediate mode)
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

void terrain::render3(Vector4f basecolour) {      /// draw the terrain using an indexed VBO
  glColor4fv(basecolour);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glEnableClientState(GL_NORMAL_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
  glNormalPointer(GL_FLOAT, 0, 0);

  glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void terrain::render4(Vector4f basecolour) {      /// draw the terrain using an indexed VBO with VAA
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

void terrain::render5(Vector4f basecolour) {      /// draw the terrain using an indexed VBO with VAO
  glColor4fv(basecolour);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
