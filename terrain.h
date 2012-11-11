#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

class terrain {                 /// object for handling the terrain heightmap
public:
  terrain() {                            // default constructor
  }

  double get_height_at(double x, double z) {
    /// return the ground height at these coordinates
    return (double)0;
  }

  Vector3d get_slope_at(double x, double z) {
    /// return the downward slope vector at this spot
    Vector3d thisvector;

    thisvector.x = (double)0;
    thisvector.y = (double)1;
    thisvector.z = (double)0;

    return thisvector;
  }
};


#endif // TERRAIN_H_INCLUDED
