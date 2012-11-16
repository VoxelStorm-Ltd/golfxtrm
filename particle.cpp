#include "particle.h"
#include "globalvars_client_extern.h"
//#include "worldcomponents.h"

particle::particle() {
}

particle::particle(world *parentplanet) {
  size = 0.01;

  age = 0;
  lifespan = 1;

  vao = vbo = vbo_n = ibo = 0;
  if(hasvao) {
    glGenVertexArrays(1, &vao);
  }
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &vbo_n);
  glGenBuffers(1, &ibo);

  numtris = 0;

  currentplanet = parentplanet;
  //currentplanet->particles.push_back(this);
}

particle::~particle() {
  //dtor
}

void particle::render() {
  /// draw the particle using an indexed VBO with VAA and VAO
  glPushMatrix();
    glTranslated(position.x, position.y, position.z);
    glMultMatrixd(rotation.transform());
    if(hasvao) {
      glColor4f(1, 1, 1, 1);
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    } else {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glEnableClientState(GL_VERTEX_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glVertexPointer(3, GL_FLOAT, 0, 0);
      glEnableClientState(GL_NORMAL_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
      glNormalPointer(GL_FLOAT, 0, 0);
      glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
      glDisableClientState(GL_VERTEX_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
  glPopMatrix();
}

golfballtrail::golfballtrail() {
}

golfballtrail::golfballtrail(Vector3d start, Vector3d end) {
  size = 0.04;
  Vector3d offset = end - start;

  GLfloat vbodata[] = {
    start.x,  start.y  - size,  start.z,
    start.x,  start.y  + size,  start.z,
    offset.x, offset.y - size, offset.z,
    offset.x, offset.y + size, offset.z,
    start.x  - size,  start.y,  start.z,
    start.x  + size,  start.y,  start.z,
    offset.x - size, offset.y, offset.z,
    offset.x + size, offset.y, offset.z,
    start.x,  start.y,  start.z  - size,
    start.x,  start.y,  start.z  + size,
    offset.x, offset.y, offset.z - size,
    offset.x, offset.y, offset.z + size,
  };
  GLfloat vbodata_n[] = {
    0, -1,0,
    0, 1, 0,
    0, -1,0,
    0, 1, 0,
    -1,0, 0,
    1, 0, 0,
    -1,0, 0,
    1, 0, 0,
    0, 0, -1,
    0, 0, 1,
    0, 0, -1,
    0, 0, 1,
  };
  GLuint ibodata[] = {
    0, 1, 2,  2, 3, 0,
    4, 5, 6,  6, 7, 4,
    8, 9,10, 10,11, 8,
  };
  numtris = 4;

  // rendering setup
  vao = vbo = vbo_n = ibo = 0;
  if(hasvao) {
    glGenVertexArrays(1, &vao);
  }
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &vbo_n);
  glGenBuffers(1, &ibo);

  glBindBuffer(GL_ARRAY_BUFFER,         vbo);
  glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata), vbodata, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,         0);
  glBindBuffer(GL_ARRAY_BUFFER,         vbo_n);
  glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata_n), vbodata_n, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,         0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if(hasvao) {
    glBindVertexArray(vao);             // set up the VAO's state
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
    glNormalPointer(GL_FLOAT, 0, 0);
    glBindVertexArray(0);
  }
}

golfballtrail::~golfballtrail() {
  //dtor
}

