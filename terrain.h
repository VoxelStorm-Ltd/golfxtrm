#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include "vmath.h"

class terrain {                 /// object for handling the terrain heightmap
public:
  Vector3d origin;      // the map's origin's coordinates
  Vector3d bounds;      // the map's bounding box

  terrain() {                            /// default constructor
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
  }
};


#endif // TERRAIN_H_INCLUDED
