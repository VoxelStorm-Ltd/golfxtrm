#include "worldcomponents.h"
#include "terrain.h"

golfcourse::golfcourse(world *parent, Vector3d tee, Vector3d hole)
  : teeposition(tee), holeposition(hole) {                      /// default constructor
  std::cout << "      Initialising new golf course..." << std::endl;
  parentplanet = parent;
  landscape = new terrain(teeposition, holeposition);
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
