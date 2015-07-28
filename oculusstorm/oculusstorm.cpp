#include "oculusstorm.h"
#include <iostream>
#include <cmath>
#include "vmath.h"

oculusstorm::oculusstorm(float farplane, float nearplane)
  : enabled(false),
    viewport_width(640),
    viewport_height(800),
    ipd(6.4),
    ipd_half(3.2),
    aspectratio(1280.0f / (2.0f * 800.0f)),
    nearplane(nearplane == 0.0f ? 0.2f : nearplane),  // 20cm default clip plane
    farplane(farplane),
    manager(nullptr),
    device(nullptr),
    sensor(nullptr),
    sensorfusion(nullptr),
    infoloaded(false) {
  /// Default constructor
  // oculus rift initialisation
  std::cout << "Oculus: Initialising Oculus Rift..." << std::endl;
  OVR::System::Init();
  //OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));
  try {
    manager = *OVR::DeviceManager::Create();
  } catch(std::exception &e) {
    std::cout << "Oculus: Exception while creating device manager: " << e.what() << std::endl;
    return;
  }
  //manager->SetMessageHandler(this);
  try {
    device.Clear();
    device = *manager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();
  } catch(std::exception &e) {
    std::cout << "Oculus: Exception while creating hmd device: " << e.what() << std::endl;
  }
  if(device) {
    std::cout << "Oculus: Device found" << std::endl;
    infoloaded = device->GetDeviceInfo(&hmdinfo);
    sensor.Clear();
    sensor = *device->GetSensor();
  } else {
    std::cout << "Oculus: Device not found" << std::endl;
    sensor = *manager->EnumerateDevices<OVR::SensorDevice>().CreateDevice();
  }
  if(!sensor) {
    std::cout << "Oculus: Sensor not found" << std::endl;
    return;
  }

  std::cout << "Oculus: Sensor found" << std::endl;
  //sensorfusion.AttachToSensor(sensor);
  sensorfusion = new OVR::SensorFusion(sensor);
  if(!sensorfusion) {
    std::cout << "Oculus: Failed to initialise sensorfusion" << std::endl;
    return;
  }
  if(!infoloaded) {
    std::cout << "Oculus: Could not load device info" << std::endl;
    return;
  }
  enabled = true;

  // setup:
  sensorfusion->EnableMotionTracking();     // make sure motion tracking is enabled
  sensorfusion->SetGravityEnabled(true);    // gravity correction

  std::cout << "  DisplayDeviceName: "      << hmdinfo.DisplayDeviceName << std::endl;
  std::cout << "  ProductName: "            << hmdinfo.ProductName << std::endl;
  std::cout << "  Manufacturer: "           << hmdinfo.Manufacturer << std::endl;
  std::cout << "  Version: "                << hmdinfo.Version << std::endl;
  std::cout << "  HResolution: "            << hmdinfo.HResolution << std::endl;
  std::cout << "  VResolution: "            << hmdinfo.VResolution << std::endl;
  std::cout << "  HScreenSize: "            << hmdinfo.HScreenSize << std::endl;
  std::cout << "  VScreenSize: "            << hmdinfo.VScreenSize << std::endl;
  std::cout << "  VScreenCenter: "          << hmdinfo.VScreenCenter << std::endl;
  std::cout << "  EyeToScreenDistance: "    << hmdinfo.EyeToScreenDistance << std::endl;
  std::cout << "  LensSeparationDistance: " << hmdinfo.LensSeparationDistance << std::endl;
  std::cout << "  InterpupillaryDistance: " << hmdinfo.InterpupillaryDistance << std::endl;
  std::cout << "  DistortionK[0]: "         << hmdinfo.DistortionK[0] << std::endl;
  std::cout << "  DistortionK[1]: "         << hmdinfo.DistortionK[1] << std::endl;
  std::cout << "  DistortionK[2]: "         << hmdinfo.DistortionK[2] << std::endl;

  // cache some important stuff
  viewport_width  = hmdinfo.HResolution / 2;
  viewport_height = hmdinfo.VResolution;
  ipd = hmdinfo.InterpupillaryDistance;
  ipd_half = ipd / 2.0f;
  ild = hmdinfo.LensSeparationDistance / hmdinfo.HScreenSize;
  ild_half = ild / 2.0f;
  aspectratio = (static_cast<float>(hmdinfo.HResolution) / 2.0f) / static_cast<float>(hmdinfo.VResolution);
  fov = 2.0f * std::atan(hmdinfo.VScreenSize / (2.0f * hmdinfo.EyeToScreenDistance));
  std::cout << "  aspect ratio  = " << aspectratio << std::endl;
  std::cout << "  field of view = " << fov << std::endl;
  std::cout << "  interlens distance (ratio)  = " << ild << std::endl;

  //h = (4 * ((hmdinfo.HScreenSize / 4) - (hmdinfo.LensSeparationDistance / 2))) / hmdinfo.HScreenSize;
  // simplified:
  //h = (hmdinfo.HScreenSize - (hmdinfo.LensSeparationDistance * 2)) / hmdinfo.HScreenSize;
  // further simplified:
  //h = 1 / (1 - (2 * hmdinfo.HScreenSize * hmdinfo.LensSeparationDistance));

  stereoconfig.SetFullViewport(OVR::Util::Render::Viewport(0, 0, viewport_width, viewport_height));
  stereoconfig.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
  stereoconfig.SetHMDInfo(hmdinfo);
  stereoconfig.SetDistortionFitPointVP(-1.0f, 0.0f);
  //float renderScale = stereoconfig.GetDistortionScale();

  // this lies and reports 320x800 instead of 640x800
  //OVR::Util::Render::StereoEyeParams eyeparams_left  = stereoconfig.GetEyeRenderParams(OVR::Util::Render::StereoEye_Left);
  //OVR::Util::Render::StereoEyeParams eyeparams_right = stereoconfig.GetEyeRenderParams(OVR::Util::Render::StereoEye_Right);
  //OVR::Util::Render::Viewport viewport_left  = eyeparams_left.VP;
  //OVR::Util::Render::Viewport viewport_right = eyeparams_right.VP;
  //std::cout << "  viewport left  = " << viewport_left.w << "x" << viewport_left.h << " at " << viewport_left.x << "," << viewport_left.y << std::endl;
  //std::cout << "  viewport right = " << viewport_right.w << "x" << viewport_right.h << " at " << viewport_right.x << "," << viewport_right.y << std::endl;
  //projection_left  = Matrix4f::fromRowMajorArray(*eyeparams_left.Projection.M);
  //projection_right = Matrix4f::fromRowMajorArray(*eyeparams_right.Projection.M);
  //viewadjust_left  = Matrix4f::fromRowMajorArray(*eyeparams_left.ViewAdjust.M);
  //viewadjust_right = Matrix4f::fromRowMajorArray(*eyeparams_right.ViewAdjust.M);
  //std::cout << projection_left.M[0][0] << " " << projection_left.M[0][1] << " " << projection_left.M[0][2] << std::endl;
  //std::cout << projection_left  << std::endl;
  //std::cout << projection_right << std::endl;
  //std::cout << viewadjust_left  << std::endl;
  //std::cout << viewadjust_right << std::endl;
}

oculusstorm::~oculusstorm() {
  /// Default destructor
  sensor.Clear();
  device.Clear();
  manager.Clear();
  OVR::System::Destroy();
}

void oculusstorm::dumpinfo() {
  /// Dump current data to stdout
  OVR::Vector3f acceleration = sensorfusion->GetAcceleration();
  OVR::Quatf orientation     = sensorfusion->GetOrientation();
  //OVR::Quatf orientation     = sensorfusion.GetPredictedOrientation(0.01);

  float yaw, pitch, roll;
  orientation.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &pitch, &roll);

  std::cout << "Yaw: "     << OVR::RadToDegree(yaw) <<
               ", Pitch: " << OVR::RadToDegree(pitch) <<
               ", Roll: "  << OVR::RadToDegree(roll) <<
               " X=" << acceleration.x <<
               " Y=" << acceleration.y <<
               " Z=" << acceleration.z << std::endl;
}

Quatf oculusstorm::getquat() {
  /// Fetch the rotation quaternion
  OVR::Quatf const orientation = sensorfusion->GetOrientation();
  // translate from OVR quat to vmath quat (w + Xi + Yj + Zk)
  return Quatf(orientation.w, orientation.x, orientation.y, orientation.z);
}

Matrix4f oculusstorm::getmatrix() {
  /// Generate a matrix with a yaw offset
  return getquat().transform();
}

Matrix4f oculusstorm::convertmatrix(OVR::Matrix4f ovrmatrix) {
  /// Convert from OVR row-major matrices to vmath opengl-compatible column-major
  return Matrix4f::fromRowMajorArray(*ovrmatrix.M);
}

void oculusstorm::cachematrices() {
  /// Pre-compute and cache the left and right eye projection matrices
  /// This must only be called once a valid opengl context has been acquired
  //OVR::Matrix4f perspectivematrix = OVR::Matrix4f::PerspectiveRH(fov, aspectratio, nearplane, farplane);
  //projection_right = convertmatrix(perspectivematrix_r);
  // better to do it ourselves:

  glMatrixMode(GL_PROJECTION);
  float const top    = std::tan(fov * 0.5f) * nearplane;
  float const bottom = -top;
  float left, right;

  // left eye
  glLoadIdentity();
  left   = aspectratio * bottom * (1.0f + ild_half);
  right  = aspectratio * top    * (1.0f - ild_half);
  glFrustum(left, right, bottom, top, nearplane, farplane);
  glTranslatef(ipd_half, 0.0, 0.0);
  glGetFloatv(GL_PROJECTION_MATRIX, projection_left.data.data());

  // right eye
  glLoadIdentity();
  left   = aspectratio * bottom * (1.0f - ild_half);
  right  = aspectratio * top    * (1.0f + ild_half);
  glFrustum(left, right, bottom, top, nearplane, farplane);
  glTranslatef(-ipd_half, 0.0, 0.0);
  glGetFloatv(GL_PROJECTION_MATRIX, projection_right.data.data());

}

void oculusstorm::setup_left() {
  /// Set up frustrum and matrices for left eye
  glMatrixMode(GL_PROJECTION);
  glViewport(0, 0, viewport_width, viewport_height);
  glScissor( 0, 0, viewport_width, viewport_height);

  glLoadMatrixf(projection_left);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();                             // reset view matrix
}

void oculusstorm::setup_right() {
  /// Set up frustrum and matrices for right eye
  glMatrixMode(GL_PROJECTION);
  glViewport(viewport_width, 0, viewport_width, viewport_height);
  glScissor( viewport_width, 0, viewport_width, viewport_height);   // note: requires glEnable(GL_SCISSOR_TEST);

  glLoadMatrixf(projection_right);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();                             // reset view matrix
}
