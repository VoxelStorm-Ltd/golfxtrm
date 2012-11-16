#include "holdable.h"
#include "worldcomponents.h"
#include "golfer.h"
#include "globalvars_client_extern.h"

golfclub::golfclub(world *parentplanet) {
  held_by = NULL;
  at_rest = true;
  mass = 1;
  momentofinertia = 0;
  name = "golf club";
  description = "A long stick with a heavy end for hitting small balls with.";

  currentplanet = parentplanet;
  currentplanet->items.push_back(this);

  bbox_start = Vector3d(-0.01,-0.1,-0.01);
  bbox_end = Vector3d(0.01,0.95,0.1);

  // rendering data
  // body:
  float top    = 0.9;
  float bottom = -0.1;
  float left   = -0.01;
  float right  = 0.01;
  float front  = 0.01;
  float back   = -0.01;
  float headlength = 0.1;
  float headdepth = 0.05;
  GLfloat vbodata[] = {
    left,  bottom, back,    // 0
    left,  bottom, front,   // 1
    left,  top,    back,    // 2
    left,  top,    front,   // 3
    right, bottom, back,    // 4
    right, bottom, front,   // 5
    right, top,    back,    // 6
    right, top,    front,   // 7

    left,  top+headdepth, back,        // 8
    left,  top+headdepth, headlength,  // 9
    left,  top,           back,        // 10
    left,  top,           headlength,  // 11
    right, top+headdepth, back,        // 12
    right, top+headdepth, headlength,  // 13
    right, top,           back,        // 14
    right, top,           headlength   // 15
  };
  GLuint ibodata[] = {
    6,4,0, 0,2,6,   // front
    3,1,5, 5,7,3,   // back
    2,0,1, 1,3,2,   // left
    7,5,4, 4,6,7,   // right
    2,6,7, 7,3,2,   // top
    5,4,0, 0,1,5,   // bottom

    14,12, 8,  8,10,14,  // front
    10, 8, 9,  9,11,10,  // left
    15,13,12, 12,14,15,  // right
    10,14,13, 15,11,10,  // top
    13,12, 8,  8, 9,13,  // bottom
  };
  numtris = 22;

  // rendering setup
  vao = vbo = vbo_n = ibo = 0;
  if(hasvao) {
    glGenVertexArrays(1, &vao);
  }
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &vbo_n);
  glGenBuffers(1, &ibo);

  glBindBuffer(GL_ARRAY_BUFFER,         vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata), vbodata, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,         0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if(hasvao) {
    glBindVertexArray(vao);             // set up the VAO's state
  }
  glBindBuffer(GL_ARRAY_BUFFER,         vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  if(hasvao) {
    glBindVertexArray(0);
  }
}

golfclub::~golfclub() {     /// destructor
}

void golfclub::render() {     /// draw the object using an indexed VBO with VAA and VAO
  if(held_by == NULL) {
    // only draw objects that aren't held by someone (leave it to their own renderer otherwise)
    glPushMatrix();
      glTranslated(position.x, position.y, position.z);
      glMultMatrixd(rotation.transform());
      //glRotated(bodyyaw, 0, -1, 0);
      //Matrix3d rotmatrix = rotation.rotMatrix();
      //glLoadMatrixd(rotation.rotMatrix());
      //GLdouble temp[16];
      //glGetDoublev(GL_MODELVIEW_MATRIX, temp);
      //std::cout << temp[0] << " " << temp[1] << " " << temp[2] << " " << temp[3] << " " << std::endl;
      //std::cout << temp[4] << " " << temp[5] << " " << temp[6] << " " << temp[7] << " " << std::endl;
      //std::cout << temp[8] << " " << temp[9] << " " << temp[10] << " " << temp[11] << " " << std::endl;
      //std::cout << temp[12] << " " << temp[13] << " " << temp[14] << " " << temp[15] << " " << std::endl;
      //std::cout << "---" << std::endl;
      //std::cout << rotation.rotMatrix().toString() << std::endl;
      if(hasvao) {
        glColor4f(1, 1, 1, 1);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      } else {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      }
    glPopMatrix();
  }
}

void golfclub::renderlocal() {  /// draw the object using an indexed VBO with VAA and VAO
  // we have no position, just render where we are
  glColor4f(1, 1, 1, 1);
  if(hasvao) {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}

void golfclub::impact(holdable *target, golfer *actor, double distance) {
  // rotational velocity -> linear velocity
  double pushleft = actor->armsyawvelocity * distance;
  double pushdown = actor->armspitchvelocity * distance;
  Vector3d pushvector = Vector3d(pushleft, pushdown, 0) * 0.001;
  if(pushvector.length() > 70 * target->mass) {  // cap the launch speed
    pushvector.normalize();
    pushvector *= 70 * target->mass;  // momentum - from average golf launch speeds
  }
  //std::cout << "vector 1        " << pushvector.x << " " << pushvector.y << " " << pushvector.z << std::endl;
  pushvector.rotate(0, 0, actor->armsyaw);
  //std::cout << "vector 2        " << pushvector.x << " " << pushvector.y << " " << pushvector.z << std::endl;
  pushvector.rotate(actor->armspitch, 0, 0);
  //std::cout << "vector 3        " << pushvector.x << " " << pushvector.y << " " << pushvector.z << std::endl;
  pushvector.rotate(0, -actor->bodyyaw, 0);
  if(actor->armsyawvelocity < 0) {  // club loft effect (only on lofted side)
    //pushvector.rotate(-45, 0, 0);
    pushvector.y = pushvector.y + (pushvector.length());
  }
  target->at_rest = false;
  target->push(pushvector);
  std::cout << "Impacted " << target->name << " " << pushvector.x << " " << pushvector.y << " " << pushvector.z << std::endl;
}
