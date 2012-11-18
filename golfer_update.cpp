#include <cstdlib>
#include <irrKlang.h>
#include "golfer.h"
#include "holdable.h"
#include "landscape_features.h"
#include "worldcomponents.h"
#include "globalvars_client_extern.h"

extern irrklang::ISoundEngine* soundengine;

void golfer::update(double timedelta) {
  /// update position and velocity based on force and time delta
  // come on and move your body... impulse application
  bodyvelocity += (moveforce * timedelta) / bodymass;   // impulse

  if(isplayer) {
    if(inputmode == INPUTMODE_MOVING_HEAD_AND_BODY) {
      // default walk-around mode - move the head
      headyawvelocity   += (yawtorque   * timedelta) / headmomentofinertia;
      headpitchvelocity += (pitchtorque * timedelta) / headmomentofinertia;
      // put our arms down and centered
      armsyawvelocity   += ((0                  - armsyaw)   / 90 * bodyyawtorquelimit * timedelta) / bodymomentofinertia;
      armspitchvelocity += ((armspitchdownlimit - armspitch) / 90 * bodyyawtorquelimit * timedelta) / bodymomentofinertia;
      // attempt to center the body on the head
      if(headyaw > headyawdeadzone || headyaw < -headyawdeadzone || state == GOLFER_WALKING || state == GOLFER_RUNNING) {
        double thistorque = headyaw / 90 * bodyyawtorquelimit;
        if(thistorque > bodyyawtorquelimit) {
          thistorque = bodyyawtorquelimit;
        } else if(thistorque < -bodyyawtorquelimit) {
          thistorque = -bodyyawtorquelimit;
        }
        bodyyawvelocity += (thistorque * timedelta) / bodymomentofinertia;
      }
    } else if(inputmode == INPUTMODE_MOVING_HEAD_AND_ARMS) {
      // swing / aim / interact mode - move the arms and head
      armsyawvelocity   += (yawtorque   * timedelta) / headmomentofinertia;
      armspitchvelocity += (pitchtorque * timedelta) / headmomentofinertia;
      // head follows arms precisely
      headyawvelocity   += ((armsyaw   - headyaw)   / 10 * bodyyawtorquelimit * timedelta) / bodymomentofinertia;
      headpitchvelocity += ((armspitch - headpitch) / 10 * bodyyawtorquelimit * timedelta) / bodymomentofinertia;

      //std::cout << "Arms yaw: " << armsyaw << " pitch: " << armspitch << " yawvel: " << armsyawvelocity << " pitchvel: " << armspitchvelocity << std::endl;
      // body stays still
    } else if(inputmode == INPUTMODE_MOVING_ARMS) {  // INPUTMODE_MOVING_ARMS
      // swing / interact with fixed view - move the arms only
      armsyawvelocity   += (yawtorque   * timedelta) / headmomentofinertia;
      armspitchvelocity += (pitchtorque * timedelta) / headmomentofinertia;
      // body stays still
      // head stays still
    } else {          // INPUTMODE_MOVING_HEAD
      // like default mode but leave body frozen and arms where they are
      headyawvelocity   += (yawtorque   * timedelta) / headmomentofinertia;
      headpitchvelocity += (pitchtorque * timedelta) / headmomentofinertia;
    }
  } else {
    // default walk-around mode - move the head
    headyawvelocity   += (yawtorque   * timedelta) / headmomentofinertia;
    headpitchvelocity += (pitchtorque * timedelta) / headmomentofinertia;
    // put our arms down and centered
    armsyawvelocity   += ((0                  - armsyaw)   / 90 * bodyyawtorquelimit * timedelta) / bodymomentofinertia;
    armspitchvelocity += ((armspitchdownlimit - armspitch) / 90 * bodyyawtorquelimit * timedelta) / bodymomentofinertia;
    // attempt to center the body on the head
    if(headyaw > headyawdeadzone || headyaw < -headyawdeadzone || state == GOLFER_WALKING || state == GOLFER_RUNNING) {
      double thistorque = headyaw / 90 * bodyyawtorquelimit;
      if(thistorque > bodyyawtorquelimit) {
        thistorque = bodyyawtorquelimit;
      } else if(thistorque < -bodyyawtorquelimit) {
        thistorque = -bodyyawtorquelimit;
      }
      bodyyawvelocity += (thistorque * timedelta) / bodymomentofinertia;
    }
  }

  if(std::abs(armsyawvelocity) > 1000) {
    // check if we're already playing a sound
    if(currentsound != NULL) {
      if(currentsound->isFinished()){
        // clear up this sound first
        delete currentsound;
        currentsound = NULL;
        // play one of several random intro sounds
        srand(glfwGetTime());
        short sample = rand() % 4;
        if(sample == 0) {
          currentsound = soundengine->play2D("GolfXTRM - swing1.ogg", false, false, true);
        } else if(sample == 1) {
          currentsound = soundengine->play2D("GolfXTRM - swing2.ogg", false, false, true);
        } else if(sample == 2) {
          currentsound = soundengine->play2D("GolfXTRM - swing3.ogg", false, false, true);
        } else {
          currentsound = soundengine->play2D("GolfXTRM - swing4.ogg", false, false, true);
        }
      }
    } else {
      currentsound = soundengine->play2D("GolfXTRM - swing1.ogg", false, false, true);  // our first swing
    }
  }

  // gravitational force
  bodyvelocity.y -= (currentplanet->gravity * timedelta);     // acceleration

  // perform collision detection here before we make the motions
  // check for club swing area
  // first see what objects, if any, are within our swing radius
  for(std::vector<holdable*>::iterator i = currentplanet->items.begin(); i != currentplanet->items.end(); ++i) {
    if((*i)->held_by == NULL) {    // only check those loose on the ground
      Vector3d difference = (*i)->position - (bodyposition + armfulcrum);
      if(difference.length() < (swinglength + (bodyvelocity.length() * timedelta))) {
        // this is within our sphere of influence for this frame, so check for collision
        difference.rotate(0, bodyyaw, 0);    // first centre this on our body
        //difference.rotate(armspitch, 0, 0);  // tilt back for our current pitch
        //difference.rotate(0, armsyaw, 0);    // finally adjust for our arm position
        // now collision is just a matter of being close enough to 0...
        double targetyaw   = atan2(difference.x, -difference.z) * 180 / M_PI;
        double targetpitch = atan2(-difference.y, -difference.z) * 180 / M_PI;
        // use angles and compare the yaw velocity in that
        double bbox_angle    = atan2(helditem->bbox_end.x, helditem->bbox_end.y + armlength) * 180 / M_PI;
        double bbox_angle_up = atan2(helditem->bbox_end.z, helditem->bbox_end.y + armlength) * 180 / M_PI;
        //std::cout << i->name << " at " << difference.x << " " << difference.y << " " << difference.z << std::endl;
        //std::cout << i->name << " at " << targetyaw << " " << targetpitch << " " << std::endl;
        if(armsyawvelocity < 0) {   // right to left
          if(targetyaw > armsyaw - bbox_angle + (armsyawvelocity * timedelta) - (*i)->boundingradius && targetyaw < armsyaw + bbox_angle + (*i)->boundingradius) {
            //std::cout << i->name << " < " << armsyawvelocity << " target " << targetyaw << " " << targetpitch << " us [" << armsyaw - bbox_angle + (armsyawvelocity * timedelta) << "-" << armsyaw + bbox_angle << "] [" << armspitch - bbox_angle - (armspitchvelocity * timedelta) << "-" << armspitch + bbox_angle << "] " << std::endl;
            //std::cout << "off by " << targetpitch-armspitch << " targetpitch " << targetpitch << " our pitch " << armspitch << std::endl;
            if(armspitchvelocity > 0) {   // up or down
              if(targetpitch > armspitch - bbox_angle_up - (armspitchvelocity * timedelta) - (*i)->boundingradius && targetpitch < armspitch + bbox_angle + (*i)->boundingradius) {
                //std::cout << i->name << " <V " << armspitchvelocity << " target " << targetyaw << " " << targetpitch << armsyaw << " us [" << armspitch - bbox_angle - (armspitchvelocity * timedelta) << "-" << armspitch + bbox_angle << "] " << std::endl;
                //helditem->impact(&(*i), this, difference.length());
                helditem->impact(*i, this, difference.length());
              }
            } else {
              if(targetpitch > armspitch - bbox_angle_up - (*i)->boundingradius && targetpitch < armspitch + bbox_angle - (armspitchvelocity * timedelta) + (*i)->boundingradius) {
                //std::cout << i->name << " <^ " << armspitchvelocity << " target " << targetyaw << " " << targetpitch << armsyaw << " us [" << armspitch - bbox_angle - (armspitchvelocity * timedelta) << "-" << armspitch + bbox_angle << "] " << std::endl;
                helditem->impact(*i, this, difference.length());
              }
            }
          }
        } else {                    // left to right
          if(targetyaw > armsyaw - bbox_angle - (*i)->boundingradius && targetyaw < armsyaw + bbox_angle + (armsyawvelocity * timedelta) + (*i)->boundingradius) {
            //std::cout << i->name << " > " << armsyawvelocity << " target " << targetyaw << " " << targetpitch << " us [" << armsyaw - bbox_angle << "-" << armsyaw + bbox_angle + (armsyawvelocity * timedelta) << "] [" << armspitch - bbox_angle - (armspitchvelocity * timedelta) << "-" << armspitch + bbox_angle << "] " << std::endl;
            //std::cout << "off by " << targetpitch-armspitch << " targetpitch " << targetpitch << " our pitch " << armspitch << std::endl;
            if(armspitchvelocity > 0) {   // up or down
              if(targetpitch > armspitch - bbox_angle_up - (armspitchvelocity * timedelta) - (*i)->boundingradius && targetpitch < armspitch + bbox_angle + (*i)->boundingradius) {
                //std::cout << i->name << " V> " << armspitchvelocity << " target " << targetyaw << " " << targetpitch << armsyaw << " us [" << armspitch - bbox_angle - (armspitchvelocity * timedelta) << "-" << armspitch + bbox_angle << "] " << std::endl;
                helditem->impact(*i, this, difference.length());
              }
            } else {
              if(targetpitch > armspitch - bbox_angle_up - (*i)->boundingradius && targetpitch < armspitch + bbox_angle - (armspitchvelocity * timedelta) + (*i)->boundingradius) {
                //std::cout << i->name << " ^> " << armspitchvelocity << " target " << targetyaw << " " << targetpitch << armsyaw << " us [" << armspitch - bbox_angle - (armspitchvelocity * timedelta) << "-" << armspitch + bbox_angle << "] " << std::endl;
                helditem->impact(*i, this, difference.length());
              }
            }
          }
        }
      }
    }
  }

  // the inertially driven motions
  //std::cout << "DEBUGv:" << bodyvelocity.x << ":" << bodyvelocity.y << ":" << bodyvelocity.z << ":" << std::endl;
  //std::cout << "DEBUGp:" << bodyposition.x << ":" << bodyposition.y << ":" << bodyposition.z << ":" << std::endl;
  bodyposition += bodyvelocity      * timedelta;
  bodyyaw      += bodyyawvelocity   * timedelta;
  headyaw      += (headyawvelocity - bodyyawvelocity) * timedelta;   // compensate for body rotation
  armsyaw      += armsyawvelocity   * timedelta;
  armspitch    += armspitchvelocity * timedelta;
  if(headpitch > 0) {
    headpitch += (headpitchvelocity - (std::abs(bodyyawvelocity) * headpitch      * 0.01)) * timedelta;
  } else {
    headpitch += (headpitchvelocity + (std::abs(bodyyawvelocity) * std::abs(headpitch) * 0.01)) * timedelta;
  }

  // internal damping
  double thisheaddampingamount = 1 - (headdampingcoefficient * timedelta);
  headyawvelocity   *= thisheaddampingamount;
  headpitchvelocity *= thisheaddampingamount;
  armsyawvelocity   *= thisheaddampingamount;
  armspitchvelocity *= thisheaddampingamount;
  headyawvelocity   *= thisheaddampingamount;
  bodyyawvelocity   *= 1 - (bodyyawdampingcoefficient * timedelta);

  // air resistance and wind effect (combined)
  Vector3d thisveldiff = bodyvelocity - currentplanet->windvelocity;
  if(thisveldiff.x == 0 && thisveldiff.y == 0 && thisveldiff.z == 0) {
    if(currentplanet->windvelocity.x == 0 && currentplanet->windvelocity.y == 0 && currentplanet->windvelocity.z == 0) {
      // no calculations necessary
    } else {
      double thisdragimpulse = (0.5 * cda * currentplanet->airdensity * thisveldiff.lengthSq() * timedelta) / bodymass ;
      Vector3d thisdragdecel;
      thisdragdecel = currentplanet->windvelocity;
      thisdragdecel.normalize();
      thisdragdecel *= thisdragimpulse;
      //std::cout << "Vel: " << bodyvelocity.length() * 2.23693629 << "mph Wind: " << currentplanet->windvelocity.length() * 2.23693629 << "mph Rel: " << thisveldiff.length() * 2.23693629 << "mph " << std::endl;
      bodyvelocity += thisdragdecel;
    }
  } else {
    double thisdragimpulse = (0.5 * cda * currentplanet->airdensity * thisveldiff.lengthSq() * timedelta) / bodymass ;
    Vector3d thisdragdecel;
    thisdragdecel = thisveldiff;
    thisdragdecel.normalize();
    thisdragdecel = Vector3d(0,0,0) - (thisdragdecel * thisdragimpulse);
    //std::cout << "Vel: " << bodyvelocity.length() * 2.23693629 << "mph Wind: " << currentplanet->windvelocity.length() * 2.23693629 << "mph Rel: " << thisveldiff.length() * 2.23693629 << "mph " << std::endl;
    bodyvelocity += thisdragdecel;
  }

  // ground collision
  double groundheight = currentcourse->get_height_at(bodyposition.x, bodyposition.z);
  if(bodyposition.y <= groundheight) {
    //bodyposition.y = groundheight;
    // smoothly bring us back up to the ground
    bodyposition.y = bodyposition.y + ((groundheight - bodyposition.y) * currentcourse->get_hardness_at(bodyposition.x, bodyposition.y) * timedelta);
    if(bodyvelocity.y < 0) {                // stop downwards motion
      bodyvelocity.y = 0;
    }
    // apply ground friction
    if(((state == GOLFER_STANDING) && (bodyvelocity.lengthSq() > 0.0001 )) ||
       ((state == GOLFER_WALKING) && (bodyvelocity.lengthSq() > walkspeed_sq)) ||
       ((state == GOLFER_RUNNING) && (bodyvelocity.lengthSq() > walkrunspeed_sq))) {
      double thisfrictionimpulse = bodymass * currentcourse->get_friction_at(bodyposition.x, bodyposition.z) * timedelta;  // mass cancels
      Vector3d thisdragdecel = bodyvelocity;
      thisdragdecel.normalize();
      thisdragdecel = thisdragdecel * -1 * thisfrictionimpulse;
      //std::cout << "dragdecel: " << thisdragdecel.x << ":" << thisdragdecel.y << ":" << thisdragdecel.z << ", " << thisfrictionimpulse << std::endl;
      //std::cout << "Vel: " << bodyvelocity.length() * 2.23693629 << "mph Friction: " << thisdragdecel.length() << " " << std::endl;
      bodyvelocity += thisdragdecel;
    } else if(state == GOLFER_JUMPING) {    // apply an impulse upwards
      bodyvelocity.y += (maximpulse_jump / bodymass);
      state = GOLFER_FREEFALL;              // the natural conclusion to jumping
    }
  }

  // course feature collision (trees etc)
  for(std::vector<feature*>::iterator i = currentplanet->features.begin(); i != currentplanet->features.end(); ++i) {
    // check if it's within our bounding sphere
    Vector3d difference = ((*i)->position + (*i)->collisionoffset) - (bodyposition + (armfulcrum / 2));   // about waist height
    if(difference.length() < boundingradius + (*i)->boundingradius) {
      std::cout << "Collided with " << (*i)->name << std::endl;
      // TODO: add finer checks here
      // our radii are overlapping so bounce us back, directly away from the centre
      double interference = 1-(difference.length() / (boundingradius + (*i)->boundingradius));
      difference.normalize();
      difference *= -1;
      double pushbackaccel = 100;
      bodyvelocity += difference * (interference * pushbackaccel * timedelta);  // brackets ensure fastest computation order
      bodyvelocity *= 1-interference;
    }
  }

  // other golfer collision
  // TODO

  // wrapping and clamping
  if(bodyyaw > 360) {                               // wrap body rotation
    bodyyaw -= 360;
  } else if(bodyyaw < 0) {
    bodyyaw += 360;
  }
  if(headyaw > headyawlimit) {                      // clamp head rotation
    headyaw = headyawlimit;
  } else if(headyaw < -headyawlimit) {
    headyaw = -headyawlimit;
  }
  if(headpitch > headpitchdownlimit) {
    headpitch = headpitchdownlimit;
  } else if(headpitch < -headpitchuplimit) {
    headpitch = -headpitchuplimit;
  }
  if(armsyaw > armsyawlimit) {                      // clamp arms rotation
    armsyaw = armsyawlimit;
  } else if(armsyaw < -armsyawlimit) {
    armsyaw = -armsyawlimit;
  }
  if(armspitch > armspitchdownlimit) {
    armspitch = armspitchdownlimit;
  } else if(armspitch < -armspitchuplimit) {
    armspitch = -armspitchuplimit;
  }

  moveforce.x = 0;
  moveforce.y = 0;
  moveforce.z = 0;
}
