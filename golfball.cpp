#include "holdable.h"
#include "worldcomponents.h"
#include "globalvars_client_extern.h"

golfball::golfball(world *parentplanet) {
  held_by = NULL;
  at_rest = true;
  mass = 0.04593;                                                               // official maximum
  radius = 0.04267 / 2;                                                         // official minimum
  radius *= 2;                                                                  // but let's make it bigger for aesthetic reasons
  momentofinertia = 0;
  cda = 0.001716;                                                               // guesstimate based on Cd = 0.3
  name = "golf ball";
  description = "A small hard white ball with strong elastic properties.";

  bbox_start = Vector3d(-radius, -radius, -radius);
  bbox_end   = Vector3d( radius,  radius,  radius);
  boundingradius = radius;

  currentplanet = parentplanet;
  currentplanet->items.push_back(this);

  // icosahedron!
  float t = (1 + std::sqrt(5)) / 2;
  float scale = radius / std::sqrt(1 + (t * t));
  GLfloat vbodata[] = {
    t  * scale, 1  * scale, 0  * scale,
    -t * scale, 1  * scale, 0  * scale,
    t  * scale, -1 * scale, 0  * scale,
    -t * scale, -1 * scale, 0  * scale,
    1  * scale, 0  * scale, t  * scale,
    1  * scale, 0  * scale, -t * scale,
    -1 * scale, 0  * scale, t  * scale,
    -1 * scale, 0  * scale, -t * scale,
    0  * scale, t  * scale, 1  * scale,
    0  * scale, -t * scale, 1  * scale,
    0  * scale, t  * scale, -1 * scale,
    0  * scale, -t * scale, -1 * scale
  };
  GLfloat vbodata_n[] = {
    t,1,0,
    -t,1,0,
    t,-1,0,
    -t,-1,0,
    1,0,t,
    1,0,-t,
    -1,0,t,
    -1,0,-t,
    0,t,1,
    0,-t,1,
    0,t,-1,
    0,-t,-1,
  };
  GLuint ibodata[] = {
    0,8,4,  0,5,10, 2,4,9,  2,11,5,  1,6,8,
    1,10,7, 3,9,6,  2,9,11, 3,9,11,  4,2,0,
    5,0,2,  6,1,3,  7,3,1,  8,6,4,   3,7,11,
    0,10,8, 1,8,10, 9,4,6,  10,5,7,  11,7,5
  };
  numtris = 20;

  // rendering setup
  vao = vbo = vbo_n = ibo = 0;
  if(hasvao) {
    glGenVertexArrays(1, &vao);
  }
  glGenBuffersARB(1, &vbo);
  glGenBuffersARB(1, &vbo_n);
  glGenBuffersARB(1, &ibo);

  glBindBufferARB(GL_ARRAY_BUFFER,         vbo);
  glBufferDataARB(GL_ARRAY_BUFFER,         sizeof(vbodata), vbodata, GL_STATIC_DRAW);
  glBindBufferARB(GL_ARRAY_BUFFER,         0);
  glBindBufferARB(GL_ARRAY_BUFFER,         vbo_n);
  glBufferDataARB(GL_ARRAY_BUFFER,         sizeof(vbodata_n), vbodata_n, GL_STATIC_DRAW);
  glBindBufferARB(GL_ARRAY_BUFFER,         0);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

  if(hasvao) {
    glBindVertexArray(vao);                                                     // set up the VAO's state
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

golfball::~golfball() {     /// destructor
}

void golfball::render() {       /// draw the ball using an indexed VBO with VAA and VAO
  if(held_by == NULL) {
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
}

void golfball::renderlocal() {  /// draw the ball using an indexed VBO with VAA and VAO
  // we have no position, just render where we are
  glColor4f(1, 1, 1, 1);
  if(hasvao) {
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
}
