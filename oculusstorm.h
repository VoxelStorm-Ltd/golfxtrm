#ifndef OCULUSSTORM_H_INCLUDED
#define OCULUSSTORM_H_INCLUDED

// A lightweight class to manage the Oculus Rift
// Written by Eugene Hopkinson for VoxelStorm Ltd, 2013

#include <GL/glew.h>
#include <GL/gl.h>
#include <OVR.h>
#include "vmath.h"

class oculusstorm {
public:
  bool enabled;

  static constexpr float gravity = 9.81;
  unsigned int viewport_width;
  unsigned int viewport_height;
  float ipd;
  float ipd_half;
  float ild;
  float ild_half;
  float aspectratio;
  float fov;

  GLfloat nearplane;
  GLfloat farplane;

  Matrix4f projection_left;
  Matrix4f projection_right;
  Matrix4f viewadjust_left;
  Matrix4f viewadjust_right;

  OVR::Ptr<OVR::DeviceManager>    manager;
  OVR::Ptr<OVR::HMDDevice>        device;
  OVR::Ptr<OVR::SensorDevice>     sensor;
  OVR::SensorFusion               *sensorfusion;
  OVR::HMDInfo                    hmdinfo;
  bool                            infoloaded;
  OVR::Util::Render::StereoConfig stereoconfig;

  oculusstorm(float farplane, float nearplane = 0.0);
  ~oculusstorm();

  void dumpinfo();
  Quatf getquat();
  Matrix4f getmatrix();
  Matrix4f convertmatrix(OVR::Matrix4f ovrmatrix);
  void cachematrices();
  void setup_left();
  void setup_right();
};

#endif // OCULUSSTORM_H_INCLUDED
