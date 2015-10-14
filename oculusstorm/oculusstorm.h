#ifndef OCULUSSTORM_H_INCLUDED
#define OCULUSSTORM_H_INCLUDED

// A lightweight class to manage the Oculus Rift
// Written by Eugene Hopkinson for VoxelStorm Ltd, 2013-2015

#include <GL/glew.h>
#include <GL/gl.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#include <OVR.h>
#pragma GCC diagnostic pop
#include "vmath.h"

class oculusstorm {
public:
  static float constexpr gravity = 9.81f;
  unsigned int viewport_width  = 640;
  unsigned int viewport_height = 800;
  float ipd                    = 6.4f;
  float ipd_half               = ipd / 2.0f;
  float ild;
  float ild_half;
  float aspectratio            = static_cast<float>(viewport_width) / static_cast<float>(viewport_height);
  float fov;

  GLfloat nearplane;
  GLfloat farplane;

  Matrix4<GLfloat> projection_left;
  Matrix4<GLfloat> projection_right;
  Matrix4<GLfloat> viewadjust_left;
  Matrix4<GLfloat> viewadjust_right;

  OVR::Ptr<OVR::DeviceManager>    manager       = nullptr;
  OVR::Ptr<OVR::HMDDevice>        device        = nullptr;
  OVR::Ptr<OVR::SensorDevice>     sensor        = nullptr;
  OVR::SensorFusion               *sensorfusion = nullptr;
  OVR::HMDInfo                    hmdinfo;
  OVR::Util::Render::StereoConfig stereoconfig;

  bool                            infoloaded    = false;
  bool                            enabled       = false;

  oculusstorm(float new_farplane, float new_nearplane = 0.0f);
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
