#include "holdable.h"
#include "worldcomponents.h"
#include "particle.h"

holdable::holdable() {                                /// default constructor
  held_by = NULL;
  at_rest = true;

  currentplanet = NULL;
}

holdable::holdable(world *parentplanet) {             /// specific constructor
  held_by = NULL;
  at_rest = true;
  mass = 0;
  momentofinertia = 0;
  name = "object";
  description = "Some sort of object that hasn't been properly defined yet.";

  currentplanet = parentplanet;
  currentplanet->items.push_back(this);
}

holdable::~holdable() {
  //currentplanet->items.erase(std::find(currentplanet->items.begin(), currentplanet->items.end(), this));
  //currentplanet->items.release(std::find(currentplanet->items.begin(), currentplanet->items.end(), this));
  // why won't that work?
  //holdable *ptr = this;
  //currentplanet->items.erase(std::find_if(currentplanet->items.begin(),
  //                                        currentplanet->items.end(),
  //                                        [ptr](holdable const &other)
  //                                        {return ptr == &other;} ));
}

void holdable::push(Vector3d impulse) {  /// apply a one-off impulse to this object
  // TODO: if held, apply max holding force, enable knocking out of hand
  velocity += impulse / mass;   // applied directly as a one-off, no delta time considered
}

void holdable::push(Vector3d impulse, Vector3d impactpoint) {
  /// apply a one-off impulse offset from centre of this object
  // TODO: apply max holding force, enable knocking out of hand
  velocity += impulse / mass;   // applied directly as a one-off, no delta time considered

  // TODO: calculate offset from COG and apply rotational acceleration
}

void holdable::impact(holdable *target, golfer *actor, double distance) {
}

void holdable::update(double timedelta) {
  /// update position and velocity based on time delta
  // only update if it's free in the air, not hand-held
  if(held_by == NULL) {
    if(!at_rest) {
      // gravitational force
      velocity.y -= (currentplanet->gravity * timedelta);     // acceleration

      Vector3d newposition = position + (velocity * timedelta);
      //std::cout << "DEBUG last settings " << position.x << " " << position.y << " " << position.z << " " << newposition.x << " " << newposition.y << " " <<newposition.z << std::endl;
      new golfballtrail(currentplanet, position, newposition);  // particle trails
      position = newposition;

      // air resistance and wind effect (combined)
      Vector3d thisveldiff = velocity - currentplanet->windvelocity;
      if(thisveldiff.x == 0 && thisveldiff.y == 0 && thisveldiff.z == 0) {
        if(currentplanet->windvelocity.x == 0 && currentplanet->windvelocity.y == 0 && currentplanet->windvelocity.z == 0) {
          // no calculations necessary
        } else {
          double thisdragimpulse = (0.5 * cda * currentplanet->airdensity * thisveldiff.lengthSq() * timedelta) / mass ;
          Vector3d thisdragdecel;
          thisdragdecel = currentplanet->windvelocity;
          thisdragdecel.normalize();
          thisdragdecel *= thisdragimpulse;
          velocity += thisdragdecel;
        }
      } else {
        double thisdragimpulse = (0.5 * cda * currentplanet->airdensity * thisveldiff.lengthSq() * timedelta) / mass ;
        Vector3d thisdragdecel;
        thisdragdecel = thisveldiff;
        thisdragdecel.normalize();
        thisdragdecel = Vector3d(0,0,0) - (thisdragdecel * thisdragimpulse);
        velocity += thisdragdecel;
      }

      // ground collision
      double groundheight = currentplanet->get_height_at(position.x, position.z);
      if(position.y - boundingradius <= groundheight) {
        // smoothly bring us back up to the ground
        double distancebelowground = (groundheight - (position.y - boundingradius));
        if(distancebelowground > 0) {
          double amounttomove = (distancebelowground * currentplanet->get_hardness_at(position.x, position.y) * timedelta);
          if(amounttomove > distancebelowground || std::isnan(amounttomove)) {
            amounttomove = distancebelowground;
          }
          position.y += amounttomove;
        }
        if(velocity.y < 0) {                // stop downwards motion
          velocity.y = 0;
        }
        // apply rolling resistance (different to sliding friction)
        double thisfrictionimpulse = mass * currentplanet->get_friction_at(position.x, position.z) * timedelta;  // mass cancels
        Vector3d thisdragdecel = velocity;
        thisdragdecel.normalize();
        thisdragdecel = thisdragdecel * -1 * (thisfrictionimpulse
                                              + ((currentplanet->get_grass_depth_at(position.x,position.z)
                                                  * 120) * velocity.length() * timedelta));
        if(!std::isnan(thisdragdecel.y)) {
          //std::cout << "dragdecel: " << thisdragdecel.x << ":" << thisdragdecel.y << ":" << thisdragdecel.z << ", " << thisfrictionimpulse << std::endl;
          //std::cout << "Vel: " << velocity.length() * 2.23693629 << "mph Friction: " << thisdragdecel.length() << " " << std::endl;
          velocity += thisdragdecel;
          if(velocity.length() < currentplanet->get_min_velocity_at(position.x, position.z)) {
            velocity.x = velocity.y = velocity.z = 0;
            std::cout << name << " came to rest at " << position.x << " " << position.y << " " << position.z << std::endl;
            at_rest = true;
          }
        }
      }
      //std::cout << "height: " << position.y << "velocity: " << velocity.y << std::endl;
      // TODO: slope effects

      // TODO: apply quaternion rotation
    }
  }
}

void holdable::rotate(axistype axis, double angle) {
  if(axis == AXIS_X) {
    rotation += Quaternion<double>::fromAxisRot(Vector3d(1,0,0), angle);
    rotation.normalize();
  } else if (axis == AXIS_Y) {
    rotation += Quaternion<double>::fromAxisRot(Vector3d(0,1,0), angle);
    rotation.normalize();
  } else if (axis == AXIS_Z) {
    rotation += Quaternion<double>::fromAxisRot(Vector3d(0,0,1), angle);
    rotation.normalize();
  }
}

void holdable::render() {       /// draw this item in the world
}

void holdable::renderlocal() {  /// draw this item in a hand or container
}
