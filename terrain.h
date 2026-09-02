#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vectorstorm/vector/vector3.h"
#include "vectorstorm/vector/vector4.h"
#include "landscape_features.h"

class golfcourse;                                                               // forward dec

class terrain {                 /// object for handling the terrain heightmap
public:
  golfcourse *parent;                                                           // what course this belongs to

  vector3d origin;                                                              // the map's origin's coordinates
  vector3d bounds;                                                              // the map's bounding box

  //double heightmap[200*200];                                                  // the heightmap data
  //double **heightmap;                                                         // the heightmap data on the heap
  std::vector<double> heightmap;                                                // the heightmap data as a vector
  int gridwidth;                                                                // the x and z resolution of the heightmap as above

  int randomseed;                                                               // the fixed seed for this course

  GLuint vao;                                                                   // vertex array object
  GLuint vbo;                                                                   // vertex buffer object
  GLuint vbo_n;                                                                 // vertex buffer object for normals
  GLuint ibo;                                                                   // element buffer object (index buffer object)
  GLuint numtris;                                                               // number of triangles in the index

  terrain(golfcourse *parent, unsigned int randomseed, vector3d const &teeposition, vector3d const &holeposition, double size, double gridwidth);
  ~terrain();
  void update_vbo();
  void populatefeatures(int randomseed, feature::featuretype whatfeature);
  void populatefeatures(int randomseed, feature::featuretype whatfeature, float threshold, float probability);
  double get_height_at(double x, double z);
  vector3d get_slope_at(double x, double z);
  double get_hardness_at(double x, double z);
  void update(double timedelta);
  double get_friction_at(double x, double z);
  double get_grass_depth_at(double x, double z);
  double get_min_velocity_at(double x, double z);
  void render(vector4f const &basecolour);
  void render1(vector4f const &basecolour);
  void render2(vector4f const &basecolour);
  void render3(vector4f const &basecolour);
  void render4(vector4f const &basecolour);
  void render5(vector4f const &basecolour);
};


#endif // TERRAIN_H_INCLUDED
