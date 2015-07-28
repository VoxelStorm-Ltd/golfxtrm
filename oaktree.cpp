//#include <cstdlib>
#include "landscape_features.h"
#include "worldcomponents.h"
#include "globalvars_client_extern.h"

oaktree::oaktree(world *parentplanet, double x, double y, double z, oaktreetype treetype, int rseed) {      /// default constructor
  currentplanet = parentplanet;
  currentplanet->features.push_back(this);

  randomseed = rseed;
  std::srand(randomseed);

  position.x = x;
  position.y = y;
  position.z = z;

  maxwidth = 15;
  maxheight = 20;
  //growthrate = 0.000001;  // reaches full height in about 30 days
  growthrate = 0.00000003;  // reaches full height in about 9 years
  //growthrate = 0.000000001;  // reaches full height in about 90 years

  if(treetype == OAKTREE_STANDARD) {
    name = "perfect oak tree";
    description = "A stately, slow-groing tree with a heavy cube top and a thick cube trunk.  This one has ideal proportions.";
    height     = 7;   // default sizes
    bottom     = 2;
    width      = 3;
    trunkwidth = width / 5;
  } else if(treetype == OAKTREE_RANDOM) {
    name = "oak tree";
    description = "A stately, slow-groing tree with a heavy cube top and a thick cube trunk.";
    // set its size to something random
    height     = 4 + ((float)rand() / RAND_MAX * 8) - 1;
    bottom     = 0.5 + ((float)rand() / RAND_MAX * 0.5);
    width      = 3 + ((float)rand() / RAND_MAX * 0.8) - 0.4;
    trunkwidth = width / 5;
    rotation = Quaternion<double>::fromAxisRot(Vector3d(0,1,0), rand() % 360);
  } else if(treetype == OAKTREE_SAPLING) {
    name = "perfect oak tree sapling";
    description = "A stately, slow-groing tree with a heavy cube top and a thick cube trunk.  This one grew from a sapling of ideal proportions.";
    // make it tiny
    height     = 0.7;
    bottom     = 0.05;
    width      = 0.3;
    trunkwidth = width / 5;
  } else if(treetype == OAKTREE_SAPLING_RANDOM) {
    name = "oak tree sapling";
    description = "A stately, slow-groing tree with a heavy cube top and a thick cube trunk.  This one grew from a sapling.";
    // make it tiny and random
    height     = 0.4 + ((float)rand() / RAND_MAX * 0.8) - 0.1;
    bottom     = 0.025 + ((float)rand() / RAND_MAX * 0.025);
    width      = 0.3 + ((float)rand() / RAND_MAX * 0.08) - 0.04;
    trunkwidth = width / 5;
    rotation = Quaternion<double>::fromAxisRot(Vector3d(0,1,0), rand() % 360);
  }
  if(bottom < 1.5) {
    collisionoffset.y = ((height - bottom) / 2) + bottom;
    boundingradius = width * 1.6;
  } else {  // it's tall enough for us to only collide with the trunk
    collisionoffset.y = 1;
    boundingradius = trunkwidth;
  }

  // rendering setup
  vao = vbo = vbo_n = ibo = 0;
  if(hasvao) {
    glGenVertexArrays(1, &vao);
  }
  glGenBuffersARB(1, &vbo);
  glGenBuffersARB(1, &vbo_n);
  glGenBuffersARB(1, &ibo);
  //std::cout << "DEBUG oaktree buffers: " << vbo << " " << vbo_n << " " << ibo << std::endl;

  updatevbo();
}

oaktree::~oaktree() {     /// destructor
}

void oaktree::updatevbo() {
  srand(randomseed);
  // rendering data
  // body:
  GLfloat vbodata[] = {
    -trunkwidth, -1,      -trunkwidth,    // 0   // sides of trunk
    -trunkwidth, -1,      trunkwidth,     // 1
    -trunkwidth, bottom, -trunkwidth,    // 2
    -trunkwidth, bottom, trunkwidth,     // 3
    trunkwidth,  -1,      -trunkwidth,    // 4
    trunkwidth,  -1,      trunkwidth,     // 5
    trunkwidth,  bottom, -trunkwidth,    // 6
    trunkwidth,  bottom, trunkwidth,     // 7

    -width, bottom, -width,     // 8    // sides of crown
    -width, bottom, width,      // 9
    width,  bottom, -width,     // 10
    width,  bottom, width,      // 11
    -width, height, -width,     // 12
    -width, height, width,      // 13
    width,  height, -width,     // 14
    width,  height, width,      // 15

    -width, bottom, -width,     // 16   // bottom of crown
    -width, bottom, width,      // 17
    width,  bottom, -width,     // 18
    width,  bottom, width,      // 19

    -width, height, -width,     // 20   // top of crown
    -width, height, width,      // 21
    width,  height, -width,     // 22
    width,  height, width,      // 23
  };
  GLfloat vbodata_n[] = {   // the last vertex of a triangle defines its normal
    0, 0, -1,   // 0    front
    -1,0, 0,    // 1    left
    -1,0, 0,    // 2    left
    0, 0, 1,    // 3    back
    1, 0, 0,    // 4    right
    0, 0, 1,    // 5    back
    0, 0, -1,   // 6    front
    1, 0, 0,    // 7    right

    0, 0, -1,   // 8    front
    -1,0, 0,    // 9    left
    1, 0, 0,    // 10   right
    0, 0, 1,    // 11   back
    -1,0, 0,    // 12   left
    0, 0, 1,    // 13   back
    0, 0, -1,   // 14   front
    1, 0, 0,    // 15   right

    0, -1, 0,   // 16   bottom
    0, -1, 0,   // 17
    0, -1, 0,   // 18
    0, -1, 0,   // 19

    0,  1, 0,   // 20   top
    0,  1, 0,   // 21
    0,  1, 0,   // 22
    0,  1, 0,   // 23
  };
  GLuint ibodata[] = {
    6,4,0, 0,2,6,   // front
    3,1,5, 5,7,3,   // back
    2,0,1, 1,3,2,   // left
    7,5,4, 4,6,7,   // right

    14,12, 8,  8,10,14,   // front
    11, 9,13, 13,15,11,   // back
    10,14,15, 15,11,10,   // right
    9, 13,12, 12, 8, 9,   // left

    19,18,16, 16,17,19,   // bottom
    23,22,20, 20,21,23,   // top
  };
  numtris = 20;

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

void oaktree::update(double timespeed, double timedelta) {
  grow(growthrate * timespeed * timedelta);
}

void oaktree::grow(double amount) {
  srand(randomseed);
  /// grow "rate" percent of this tree's height per second
  if(height < maxheight) {
    height += (height * amount) + (amount*0.5);
    width += (width * amount);
    trunkwidth += (trunkwidth * amount);
    bottom += (bottom * amount) + (amount*0.5);
    updatevbo();
  } else if(width < maxwidth) {
    width += (width * amount);
    //trunkwidth += (trunkwidth * amount);
    bottom += (bottom * amount);
    updatevbo();
  } else {
    // time to fall over
  }
  if(bottom < 1.5) {
    collisionoffset.y = ((height - bottom) / 2) + bottom;
    boundingradius = width * 1.6;
  } else {  // it's tall enough for us to only collide with the trunk
    collisionoffset.y = 1;
    boundingradius = trunkwidth;
  }
}

void oaktree::render() {
  glPushMatrix();
  {
    glTranslated(position.x, position.y, position.z);
    glMultMatrixd(rotation.transform());
    glColor4f(0.75, 0.75, 0.25, 1);
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
  glPopMatrix();
}
