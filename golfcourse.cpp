//#include <cstdlib>
#include "worldcomponents.h"
#include "terrain.h"
#include <iostream>

golfcourse::golfcourse(world *parent, vector3d const &tee, vector3d const &hole, unsigned int randomseed)
  : teeposition(tee), holeposition(hole) {                      /// default constructor
  std::cout << "      Initialising new golf course..." << std::endl;
  srand(randomseed);
  parentplanet = parent;

  double detail = 2;                                                            // detail level

  double terrainsize = 400;
  double terraingridwidth = 100 * detail;
  landscape = new terrain(this, randomseed, teeposition, holeposition, terrainsize, terraingridwidth);
  srand(randomseed);
  std::cout << "        Planting trees..." << std::endl;
  for(int i = 0; i < 40; ++i) {
    double xpos = (rand() % (int)(landscape->bounds.x * 1.1)) - (landscape->bounds.x * 0.55);
    double zpos = (rand() % (int)(landscape->bounds.z * 1.1)) - (landscape->bounds.z * 0.55);
    new firtree(parent, xpos, landscape->get_height_at(xpos, zpos), zpos, firtree::FIRTREE_RANDOM, rand());
  }
  for(int i = 0; i < 1000; ++i) {
    double xpos = (rand() % (int)(landscape->bounds.x * 1.5)) - (landscape->bounds.x * 0.75);
    double zpos = (rand() % (int)(landscape->bounds.z * 1.5)) - (landscape->bounds.z * 0.75);
    new firtree(parent, xpos, landscape->get_height_at(xpos, zpos), zpos, firtree::FIRTREE_SAPLING_RANDOM, rand());
  }
  for(int i = 0; i < 40; ++i) {
    double xpos = rand() % (int)((landscape->bounds.x) - landscape->bounds.x * 0.5);
    double zpos = rand() % (int)((landscape->bounds.z) - landscape->bounds.z * 0.5);
    new oaktree(parent, xpos, landscape->get_height_at(xpos, zpos), zpos, oaktree::OAKTREE_SAPLING_RANDOM, rand());
  }
  if(parent->introon) {
    landscape->populatefeatures(2, feature::FIRTREESAPLING, 1.4, 1/detail);
    landscape->populatefeatures(3, feature::OAKTREESAPLING, 1.6, 0.2/detail);
  } else {
    landscape->populatefeatures(2, feature::FIRTREE, 1.4, 0.25);
    landscape->populatefeatures(3, feature::OAKTREE, 1.6, 0.05);
  }

  std::cout << "      Golf course initialised" << std::endl;
}

double golfcourse::get_height_at(double x, double z) {
  return landscape->get_height_at(x, z);
}

double golfcourse::get_friction_at(double x, double z) {
  return landscape->get_friction_at(x, z);
}

double golfcourse::get_hardness_at(double x, double z) {
  return landscape->get_hardness_at(x, z);
}

double golfcourse::get_grass_depth_at(double x, double z) {
  return landscape->get_grass_depth_at(x, z);
}

double golfcourse::get_min_velocity_at(double x, double z) {
  return landscape->get_min_velocity_at(x, z);
}
