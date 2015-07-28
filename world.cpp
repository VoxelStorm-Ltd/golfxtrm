#include "worldcomponents.h"

//#include <cstdlib>
#include "terrain.h"
#include "globalvars_client_extern.h"

world::world() {                                 /// default constructor
  std::cout << "    Initialising new planet..." << std::endl;
  updatenexttime = 0;                 // this is ready for an update asap

  gravity = 9.800;
  airdensity = 1.2041;
  horizondistance = 800;
  skyheight = 60;

  introon = false;
  #ifdef INTRO
  //introon = true;
  #endif

  if(introon) {
    timespeed = 31556926;   // 1 year per second
    timeofday = 18 * 60 * 60;
    calendardate = 100;

    //featureupdatefreq = (double)1/(double)5;
    featureupdatefreq = (double)60;
  } else {
    //timespeed = 0;          // frozen
    //timespeed = 1;          // realtime
    //timespeed = 60;         // 1 minute per second
    //timespeed = 120;        // 2 minutes per second
    //timespeed = 3600;       // 1 hour per second
    //timespeed = 7200;       // 2 hours per second
    //timespeed = 86400;      // 1 day per second
    //timespeed = 2592000;    // 1 month per second
    //timespeed = 31556926;   // 1 year per second
    timespeed = timesetspeed;
    timeofday = 5 * 60 * 60;    // 8am
    calendardate = 150;

    featureupdatefreq = (double)1/(double)5;
    //featureupdatefreq = 10;
  }
  updatetime = 1 / featureupdatefreq; // time from frequency

  summergrasscolour   = Vector4f(0.75, 0.75, 0.25, 1);
  summerskycolour     = Vector4f(0.90, 0.95, 0.67, 1);
  summerfogcolour     = Vector4f(0.98, 0.92, 0.50, 1);
  summerclearcolour   = summerskycolour;
  summerambientcolour = Vector4f(0.7, 0.7, 0.7, 1);
  wintergrasscolour   = Vector4f(1, 1, 1, 1);
  winterskycolour     = Vector4f(0.3, 0.6, 1, 1);
  winterfogcolour     = Vector4f(0.9, 0.95, 1, 1);
  winterclearcolour   = winterfogcolour;
  winterambientcolour = Vector4f(0.9, 0.9, 0.85, 1);

  grasscolour   = summergrasscolour;
  skycolour     = summerskycolour;
  fogcolour     = summerfogcolour;
  clearcolour   = summerclearcolour;
  ambientcolour = summerambientcolour;

  sundirection = Vector3f(0, 0, 1);
  sunambient   = Vector4f(0, 0, 0, 1);
  sundiffuse   = Vector4f(1, 1, 1, 1);
  sunspecular  = Vector4f(1, 1, 1, 1);

  //windvelocity.x = 10;

  numcourses = 0;
  //addcourse(0, Vector3d(0,0,0), Vector3d(50,0,50));   // no point having less than 1 course

  vao = vbo = vbo_n = ibo = 0;
  vao_sky = vbo_sky = vbo_n_sky = ibo_sky = 0;
  if(hasvao) {
    glGenVertexArrays(1, &vao);
    glGenVertexArrays(1, &vao_sky);
  }
  glGenBuffersARB(1, &vbo);
  glGenBuffersARB(1, &vbo_n);
  glGenBuffersARB(1, &ibo);
  glGenBuffersARB(1, &vbo_sky);
  glGenBuffersARB(1, &vbo_n_sky);
  glGenBuffersARB(1, &ibo_sky);
  update_vbo();     // get the VBOs in place for terrain and sky rendering

  std::cout << "    Planet initialised" << std::endl;
}

void world::addcourse(int coursenum, Vector3d teeposition, Vector3d holeposition, unsigned int randomseed) {
  /// add a golf course to this planet
  course.push_back(new golfcourse(this, teeposition, holeposition, randomseed));
  ++numcourses;
}

golfcourse *world::get_course_at(double x, double z) {
  // iterate through the courses
  for(int i=0; i < numcourses; ++i) {
    // check the bounding coordinates
    double courseoriginx = course[i]->landscape->origin.x;
    double courseoriginz = course[i]->landscape->origin.z;
    if(x > courseoriginx &&
       z > courseoriginz &&
       x < course[i]->landscape->bounds.x - courseoriginx &&
       z < course[i]->landscape->bounds.z - courseoriginz) {
      return course[i];
    }
  }
  return NULL;
}

double world::get_height_at(double x, double z) {
  golfcourse *thiscourse = get_course_at(x, z);
  if(thiscourse != NULL) {
    return thiscourse->get_height_at(x, z);
  }
  return 0;
}

double world::get_friction_at(double x, double z) {
  golfcourse *thiscourse = get_course_at(x, z);
  if(thiscourse != NULL) {
    return thiscourse->get_friction_at(x, z);
  }
  return 0.0409;
}

double world::get_hardness_at(double x, double z) {
  golfcourse *thiscourse = get_course_at(x, z);
  if(thiscourse != NULL) {
    return thiscourse->get_hardness_at(x, z);
  }
  return 10;
}

double world::get_grass_depth_at(double x, double z) {
  golfcourse *thiscourse = get_course_at(x, z);
  if(thiscourse != NULL) {
    return thiscourse->get_grass_depth_at(x, z);
  }
  return 0.1;
}

double world::get_min_velocity_at(double x, double z) {
  golfcourse *thiscourse = get_course_at(x, z);
  if(thiscourse != NULL) {
    return thiscourse->get_min_velocity_at(x, z);
  }
  return 0.1;
}

void world::update_vbo() {
  std::cout << "      Assigning buffers" << std::endl;
  std::vector<GLfloat> vbodata;
  std::vector<GLfloat> vbodata_n;
  std::vector<GLuint> indices;
  std::vector<GLfloat> vbodata_sky;
  std::vector<GLfloat> vbodata_n_sky;
  std::vector<GLuint> indices_sky;

  numtris = 0;
  numtris_sky = 0;

  // ground grid
  vbodata.push_back(-horizondistance);
  vbodata.push_back(-0.01);
  vbodata.push_back(-horizondistance);
  vbodata.push_back(horizondistance);
  vbodata.push_back(-0.01);
  vbodata.push_back(-horizondistance);
  vbodata.push_back(-horizondistance);
  vbodata.push_back(-0.01);
  vbodata.push_back(horizondistance);
  vbodata.push_back(horizondistance);
  vbodata.push_back(-0.01);
  vbodata.push_back(horizondistance);
  vbodata_n.push_back(0);
  vbodata_n.push_back(1);
  vbodata_n.push_back(0);
  vbodata_n.push_back(0);
  vbodata_n.push_back(1);
  vbodata_n.push_back(0);
  vbodata_n.push_back(0);
  vbodata_n.push_back(1);
  vbodata_n.push_back(0);
  vbodata_n.push_back(0);
  vbodata_n.push_back(1);
  vbodata_n.push_back(0);
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(3);
  indices.push_back(3);
  indices.push_back(2);
  indices.push_back(0);
  numtris = 2;
  // distant mountains
  // draw random mountain range around the perimiter of a circle
  for(double i = 0; i < 360; i += 2) {
    // TODO: make this round
    double thisheight = rand() % 15;
    double thiswidth = thisheight / 4;
    vbodata.push_back(cos((i + thiswidth) * M_PI / 180) * horizondistance / 4);
    vbodata.push_back(-2);
    vbodata.push_back(sin((i + thiswidth) * M_PI / 180) * horizondistance / 4);
    vbodata.push_back(cos((i            ) * M_PI / 180) * horizondistance / 4);
    vbodata.push_back(thisheight - 2);
    vbodata.push_back(sin((i            ) * M_PI / 180) * horizondistance / 4);
    vbodata.push_back(cos((i - thiswidth) * M_PI / 180) * horizondistance / 4);
    vbodata.push_back(-2);
    vbodata.push_back(sin((i - thiswidth) * M_PI / 180) * horizondistance / 4);
    vbodata_n.push_back(-cos((i + thiswidth) * M_PI / 180));
    vbodata_n.push_back(1);
    vbodata_n.push_back(-sin((i + thiswidth) * M_PI / 180));
    vbodata_n.push_back(-cos((i + thiswidth) * M_PI / 180));
    vbodata_n.push_back(1);
    vbodata_n.push_back(-sin((i + thiswidth) * M_PI / 180));
    vbodata_n.push_back(-cos((i + thiswidth) * M_PI / 180));
    vbodata_n.push_back(1);
    vbodata_n.push_back(-sin((i + thiswidth) * M_PI / 180));
    indices.push_back(indices.size()+1);
    indices.push_back(indices.size()+1);
    indices.push_back(indices.size()+1);
    ++numtris;
    // TODO: make mountains snowcapped
  }

  // simple flat sky
  vbodata_sky.push_back(-horizondistance);
  vbodata_sky.push_back(skyheight);
  vbodata_sky.push_back(-horizondistance);
  vbodata_sky.push_back(horizondistance);
  vbodata_sky.push_back(skyheight);
  vbodata_sky.push_back(-horizondistance);
  vbodata_sky.push_back(-horizondistance);
  vbodata_sky.push_back(skyheight);
  vbodata_sky.push_back(horizondistance);
  vbodata_sky.push_back(horizondistance);
  vbodata_sky.push_back(skyheight);
  vbodata_sky.push_back(horizondistance);
  vbodata_n_sky.push_back(0);
  vbodata_n_sky.push_back(-1);
  vbodata_n_sky.push_back(0);
  vbodata_n_sky.push_back(0);
  vbodata_n_sky.push_back(-1);
  vbodata_n_sky.push_back(0);
  vbodata_n_sky.push_back(0);
  vbodata_n_sky.push_back(-1);
  vbodata_n_sky.push_back(0);
  vbodata_n_sky.push_back(0);
  vbodata_n_sky.push_back(-1);
  vbodata_n_sky.push_back(0);
  indices_sky.push_back(0);
  indices_sky.push_back(1);
  indices_sky.push_back(3);
  indices_sky.push_back(3);
  indices_sky.push_back(2);
  indices_sky.push_back(0);
  numtris_sky = 2;

  std::cout << "      Uploading " << vbodata.size() << " vertices to vertex buffer (landscape)" << std::endl;
  glBindBufferARB(GL_ARRAY_BUFFER,         vbo);
  glBufferDataARB(GL_ARRAY_BUFFER,         vbodata.size() * sizeof(GLfloat), &vbodata[0], GL_STATIC_DRAW);
  glBindBufferARB(GL_ARRAY_BUFFER,         0);
  std::cout << "      Uploading " << vbodata_n.size() << " normals to normal buffer (landscape)" << std::endl;
  glBindBufferARB(GL_ARRAY_BUFFER,         vbo_n);
  glBufferDataARB(GL_ARRAY_BUFFER,         vbodata_n.size() * sizeof(GLfloat), &vbodata_n[0], GL_STATIC_DRAW);
  glBindBufferARB(GL_ARRAY_BUFFER,         0);
  std::cout << "      Uploading " << indices.size() << " indices for " << numtris << " triangles to index buffer (landscape)" << std::endl;
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

  std::cout << "      Uploading " << vbodata_sky.size() << " vertices to vertex buffer (sky)" << std::endl;
  glBindBufferARB(GL_ARRAY_BUFFER,         vbo_sky);
  glBufferDataARB(GL_ARRAY_BUFFER,         vbodata_sky.size() * sizeof(GLfloat), &vbodata_sky[0], GL_STATIC_DRAW);
  glBindBufferARB(GL_ARRAY_BUFFER,         0);
  std::cout << "      Uploading " << vbodata_n_sky.size() << " normals to normal buffer (sky)" << std::endl;
  glBindBufferARB(GL_ARRAY_BUFFER,         vbo_n_sky);
  glBufferDataARB(GL_ARRAY_BUFFER,         vbodata_n_sky.size() * sizeof(GLfloat), &vbodata_n_sky[0], GL_STATIC_DRAW);
  glBindBufferARB(GL_ARRAY_BUFFER,         0);
  std::cout << "      Uploading " << indices_sky.size() << " indices for " << numtris_sky << " triangles to index buffer (sky)" << std::endl;
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, ibo_sky);
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, indices_sky.size() * sizeof(GLuint), &indices_sky[0], GL_STATIC_DRAW);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

  if(hasvao) {
    std::cout << "      Setting up VAO" << std::endl;
    glBindVertexArray(vao);             // set up the VAO's state

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindVertexArray(vao_sky);             // set up the VAO's state

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_sky);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sky);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_n_sky);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindVertexArray(0);
  } else {
    std::cout << "      Not using VAO" << std::endl;
  }
}
