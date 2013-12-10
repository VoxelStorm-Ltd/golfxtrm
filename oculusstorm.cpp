#include "oculusstorm.h"
#include <iostream>
#include <cmath>
#include "vmath.h"

oculusstorm::oculusstorm()
  : enabled(false),
    manager(nullptr),
    device(nullptr),
    sensor(nullptr),
    sensorfusion(nullptr),
    infoloaded(false),
    viewport_width(640),
    viewport_height(800),
    ipd(6.4),
    ipd_half(3.2),
    aspectratio(1280 / (2 * 800)),
    nearplane(10),
    farplane(100000) {
  /// Default constructor
  // oculus rift initialisation
  std::cout << "Oculus: Initialising Oculus Rift..." << std::endl;
  OVR::System::Init();
  manager = *OVR::DeviceManager::Create();
  device = *manager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();
  if(device) {
    std::cout << "Oculus: Device found" << std::endl;
    infoloaded = device->GetDeviceInfo(&hmdinfo);
    sensor = *device->GetSensor();
  } else {
    std::cout << "Oculus: Device not found" << std::endl;
    sensor = *manager->EnumerateDevices<OVR::SensorDevice>().CreateDevice();
  }
  if(sensor) {
    std::cout << "Oculus: Sensor found" << std::endl;
    sensorfusion.AttachToSensor(sensor);
    enabled = true;
  } else {
    std::cout << "Oculus: Sensor not found" << std::endl;
  }

  // setup:
  sensorfusion.EnableMotionTracking();   // make sure motion tracking is enabled
  sensorfusion.SetGravityEnabled(true);  // gravity correction

  if(infoloaded) {
    std::cout << " DisplayDeviceName: "      << hmdinfo.DisplayDeviceName << std::endl;
    std::cout << " ProductName: "            << hmdinfo.ProductName << std::endl;
    std::cout << " Manufacturer: "           << hmdinfo.Manufacturer << std::endl;
    std::cout << " Version: "                << hmdinfo.Version << std::endl;
    std::cout << " HResolution: "            << hmdinfo.HResolution << std::endl;
    std::cout << " VResolution: "            << hmdinfo.VResolution << std::endl;
    std::cout << " HScreenSize: "            << hmdinfo.HScreenSize << std::endl;
    std::cout << " VScreenSize: "            << hmdinfo.VScreenSize << std::endl;
    std::cout << " VScreenCenter: "          << hmdinfo.VScreenCenter << std::endl;
    std::cout << " EyeToScreenDistance: "    << hmdinfo.EyeToScreenDistance << std::endl;
    std::cout << " LensSeparationDistance: " << hmdinfo.LensSeparationDistance << std::endl;
    std::cout << " InterpupillaryDistance: " << hmdinfo.InterpupillaryDistance << std::endl;
    std::cout << " DistortionK[0]: "         << hmdinfo.DistortionK[0] << std::endl;
    std::cout << " DistortionK[1]: "         << hmdinfo.DistortionK[1] << std::endl;
    std::cout << " DistortionK[2]: "         << hmdinfo.DistortionK[2] << std::endl;
    std::cout << "--------------------------" << std::endl;
  }

  // cache some important stuff
  viewport_width  = hmdinfo.HResolution / 2;
  viewport_height = hmdinfo.VResolution;
  ipd = hmdinfo.InterpupillaryDistance;
  ipd_half = ipd / 2;
  ild = hmdinfo.LensSeparationDistance / hmdinfo.HScreenSize;
  ild_half = ild / 2;
  aspectratio = hmdinfo.HResolution / (2.0f * hmdinfo.VResolution);
  fov = 2.0f * atan(hmdinfo.VScreenSize / (2.0f * hmdinfo.EyeToScreenDistance));
  std::cout << "aspect ratio  = " << aspectratio << std::endl;
  std::cout << "field of view = " << fov << std::endl;
  std::cout << "interpupillary distance (m) = " << ipd << std::endl;
  std::cout << "interlens distance (pixels) = " << ild << std::endl;

  OVR::Matrix4f perspectivematrix = OVR::Matrix4f::PerspectiveRH(fov, aspectratio, 10, 10000);

  //h = (4 * ((hmdinfo.HScreenSize / 4) - (hmdinfo.LensSeparationDistance / 2))) / hmdinfo.HScreenSize;
  // simplified:
  //h = (hmdinfo.HScreenSize - (hmdinfo.LensSeparationDistance * 2)) / hmdinfo.HScreenSize;
  // further simplified:
  //h = 1 / (1 - (2 * hmdinfo.HScreenSize * hmdinfo.LensSeparationDistance));

  stereoconfig.SetFullViewport(OVR::Util::Render::Viewport(0, 0, viewport_width, viewport_height));
  stereoconfig.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
  stereoconfig.SetHMDInfo(hmdinfo);
  stereoconfig.SetDistortionFitPointVP(-1.0f, 0.0f);
  float renderScale = stereoconfig.GetDistortionScale();

  OVR::Util::Render::StereoEyeParams eyeparams_left  = stereoconfig.GetEyeRenderParams(OVR::Util::Render::StereoEye_Left);
  OVR::Util::Render::StereoEyeParams eyeparams_right = stereoconfig.GetEyeRenderParams(OVR::Util::Render::StereoEye_Right);
  OVR::Util::Render::Viewport viewport_left  = eyeparams_left.VP;
  OVR::Util::Render::Viewport viewport_right = eyeparams_right.VP;
  projection_left  = Matrix4f::fromRowMajorArray(*eyeparams_left.Projection.M);
  projection_right = Matrix4f::fromRowMajorArray(*eyeparams_right.Projection.M);
  viewadjust_left  = Matrix4f::fromRowMajorArray(*eyeparams_left.ViewAdjust.M);
  viewadjust_right = Matrix4f::fromRowMajorArray(*eyeparams_right.ViewAdjust.M);
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
  OVR::Vector3f acceleration = sensorfusion.GetAcceleration();
  OVR::Quatf orientation     = sensorfusion.GetOrientation();
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
  OVR::Quatf const orientation = sensorfusion.GetOrientation();
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

void oculusstorm::setup_left() {
  /// Set up frustrum and matrices for left eye
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                             // reset projection matrix
  glViewport(0, 0, viewport_width, viewport_height);

  glFrustum(nearplane * -fov * (1 + ild_half),
            nearplane *  fov * (1 - ild_half),
            nearplane * -fov * aspectratio,
            nearplane *  fov * aspectratio,
            nearplane, farplane);

  glTranslatef(ipd_half, 0.0, 0.0);
  //glMultMatrixf(projection_left);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();                             // reset view matrix
  //glTranslatef(-ipd_half, 0, 0);
}

void oculusstorm::setup_right() {
  /// Set up frustrum and matrices for right eye
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                             // reset projection matrix
  glViewport(viewport_width, 0, viewport_width, viewport_height);

  glFrustum(nearplane * -fov * (1 - ild_half),
            nearplane *  fov * (1 + ild_half),
            nearplane * -fov * aspectratio,
            nearplane *  fov * aspectratio,
            nearplane, farplane);

  glTranslatef(-ipd_half, 0.0, 0.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();                             // reset view matrix
  //glTranslatef(ipd_half, 0, 0);
}
