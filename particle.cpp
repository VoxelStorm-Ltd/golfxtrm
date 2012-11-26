#include <vector>
#include "particle.h"
#include "globalvars_client_extern.h"
#include "worldcomponents.h"

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

void particle::update(double timedelta) {
  age += timedelta;
  //if(age > lifespan) {
    //currentplanet->particles.erase(std::find(currentplanet->particles.begin(), currentplanet->particles.end(), this));
    //currentplanet->items.release(std::find(currentplanet->items.begin(), currentplanet->items.end(), this));
    // why won't that work?
    //holdable* ptr = this;
    //currentplanet->items.erase(std::find_if(currentplanet->items.begin(),
    //                                        currentplanet->items.end(),
    //                                        [ptr](const holdable &other)
    //                                        {return ptr == &other;} ));
    //delete this;
  //}
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
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glEnableClientState(GL_VERTEX_ARRAY);
      glBindBufferARB(GL_ARRAY_BUFFER, vbo);
      glVertexPointer(3, GL_FLOAT, 0, 0);
      glEnableClientState(GL_NORMAL_ARRAY);
      glBindBufferARB(GL_ARRAY_BUFFER, vbo_n);
      glNormalPointer(GL_FLOAT, 0, 0);
      glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_NORMAL_ARRAY);
      glBindBufferARB(GL_ARRAY_BUFFER, 0);
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
  glPopMatrix();
}

golfballtrail::golfballtrail(world *parentplanet, Vector3d start, Vector3d end) {
  age = 0;
  lifespan = 3;

  size = 0.04;
  position = end;
  Vector3d offset = start - end;
  //std::cout << "start:  " << start.x << " " << start.y << " " << start.z << " " << std::endl;
  //std::cout << "end:    " << end.x << " " << end.y << " " << end.z << " " << std::endl;
  //std::cout << "offset: " << offset.x << " " << offset.y << " " << offset.z << " " << std::endl;

  GLfloat vbodata[] = {
    0,  0  - size,  0,
    0,  0  + size,  0,
    offset.x, offset.y - size, offset.z,
    offset.x, offset.y + size, offset.z,
    0  - size,  0,  0,
    0  + size,  0,  0,
    offset.x - size, offset.y, offset.z,
    offset.x + size, offset.y, offset.z,
    0,  0,  0  - size,
    0,  0,  0  + size,
    offset.x, offset.y, offset.z - size,
    offset.x, offset.y, offset.z + size,
  };
  GLfloat vbodata_n[] = {
    /*0, -1,0,
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
    0, 0, 1,*/
    1,1,1,
    1,1,1,
    1,1,1,
    1,1,1,
    1,1,1,
    1,1,1,
    1,1,1,
    1,1,1,
    1,1,1,
    1,1,1,
    1,1,1,
    1,1,1,
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

  glBindBufferARB(GL_ARRAY_BUFFER,         vbo);
  glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata), vbodata, GL_STATIC_DRAW);
  glBindBufferARB(GL_ARRAY_BUFFER,         0);
  glBindBufferARB(GL_ARRAY_BUFFER,         vbo_n);
  glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata_n), vbodata_n, GL_STATIC_DRAW);
  glBindBufferARB(GL_ARRAY_BUFFER,         0);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

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

  currentplanet = parentplanet;
  currentplanet->particles.push_back(this);
}

golfballtrail::~golfballtrail() {
  //dtor
}

void golfballtrail::render() {
  /// draw the particle using an indexed VBO with VAA and VAO
  glPushMatrix();
    glTranslated(position.x, position.y, position.z);
    glMultMatrixd(rotation.transform());
    //float size = 1 - (age / lifespan);
    float size = 2.1 - (2 * age / lifespan);
    glScalef(size, size, size);
    if(hasvao) {
      glColor4f(1, 1, 1, 1);
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    } else {
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glEnableClientState(GL_VERTEX_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glVertexPointer(3, GL_FLOAT, 0, 0);
      glEnableClientState(GL_NORMAL_ARRAY);
      glBindBufferARB(GL_ARRAY_BUFFER, vbo_n);
      glNormalPointer(GL_FLOAT, 0, 0);
      glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_NORMAL_ARRAY);
      glBindBufferARB(GL_ARRAY_BUFFER, 0);
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
  glPopMatrix();
}
