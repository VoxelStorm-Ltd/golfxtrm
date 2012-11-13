#ifndef LANDSCAPE_FEATURES_H_INCLUDED
#define LANDSCAPE_FEATURES_H_INCLUDED

class feature {
public:
  Vector3d position;            // where its origin is in the world
  Vector3d bbox_start;          // one corner of the bounding box
  Vector3d bbox_end;            // other corner of the bounding box

  world *currentplanet;         // what planet are you even on?

  GLuint vao;                   // vector array object
  GLuint numtris;               // number of triangles in the VBO

  feature() {      /// default constructor
    currentplanet = NULL;
  }

  ~feature() {     /// destructor
  }

  virtual void render() {
  }
};


class firtree : public feature {
public:

  firtree(world *parentplanet, double x, double y, double z) {      /// default constructor
    currentplanet = parentplanet;
    currentplanet->features.push_back(this);

    position.x = x;
    position.y = y;
    position.z = z;

    // rendering data
    // body:
    float height     = 5 + ((float)rand() / RAND_MAX * 8) - 1;
    float bottom     = 1.0 + ((float)rand() / RAND_MAX * 0.5);
    float width      = 1 + ((float)rand() / RAND_MAX * 0.8) - 0.6;
    float trunkwidth = 0.3 + ((float)rand() / RAND_MAX * 0.4) - 0.2;
    GLfloat vbodata[] = {
      -trunkwidth, 0,      -trunkwidth,    // 0
      -trunkwidth, 0,      trunkwidth,     // 1
      -trunkwidth, bottom, -trunkwidth,    // 2
      -trunkwidth, bottom, trunkwidth,     // 3
      trunkwidth,  0,      -trunkwidth,    // 4
      trunkwidth,  0,      trunkwidth,     // 5
      trunkwidth,  bottom, -trunkwidth,    // 6
      trunkwidth,  bottom, trunkwidth,     // 7

      -width, bottom, -width,     // 8
      -width, bottom, width,      // 9
      width,  bottom, -width,     // 10
      width,  bottom, width,      // 11

      0,      height, 0           // 12

    };
    GLuint ibodata[] = {
      6,4,0, 0,2,6,   // front
      3,1,5, 5,7,3,   // back
      2,0,1, 1,3,2,   // left
      7,5,4, 4,6,7,   // right
      2,6,7, 7,3,2,   // top
      5,4,0, 0,1,5,   // bottom

      8, 10,11, 11, 9, 8,   // bottom

      10, 8,12,        // front
      8,  9,12,        // left
      9, 11,12,        // back
      11, 8,12         // right

    };
    numtris = 18;

    // rendering setup
    vao = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER,         vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER,         sizeof(vbodata), vbodata, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibodata), ibodata, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,         0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);             // set up the VAO's state
    glBindBuffer(GL_ARRAY_BUFFER,         vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);
  }

  ~firtree() {     /// destructor
  }

  void render() {
    glPushMatrix();
    {
      glTranslated(position.x, position.y, position.z);
      glColor4f(0.75, 0.75, 0.25, 1);
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }
    glPopMatrix();
  }
};


#endif // LANDSCAPE_FEATURES
