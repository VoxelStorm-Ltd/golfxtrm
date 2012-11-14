#ifndef LANDSCAPE_FEATURES_H_INCLUDED
#define LANDSCAPE_FEATURES_H_INCLUDED

class feature {
public:
  Vector3d position;            // where its origin is in the world
  Vector3d bbox_start;          // one corner of the bounding box
  Vector3d bbox_end;            // other corner of the bounding box

  world *currentplanet;         // what planet are you even on?

  int randomseed;               // the fixed seed for this object

  GLuint vao;                   // vertex array object
  GLuint vbo;                   // vertex buffer object
  GLuint ibo;                   // index buffer object
  GLuint numtris;               // number of triangles in the VBO

  feature() {      /// default constructor
    currentplanet = NULL;
  }

  ~feature() {     /// destructor
  }

  virtual void update(double timespeed, double timedelta) {
  }

  virtual void render() {
  }
};


class firtree : public feature {
public:
  float maxheight;        // the tallest these can get
  float maxwidth;         // the widest they can get
  double growthrate;      // how fast it grows in % per second

  float height;           // overall height
  float bottom;           // bottom of the foliage from the ground
  float width;            // width of the bottom section / 2
  float trunkwidth;       // thickness of the trunk / 2

  enum firtreetype {
    FIRTREE_STANDARD,
    FIRTREE_RANDOM,
    FIRTREE_SAPLING,
    FIRTREE_SAPLING_RANDOM
  };

  firtree(world *parentplanet, double x, double y, double z, firtreetype treetype, int rseed) {      /// default constructor
    currentplanet = parentplanet;
    currentplanet->features.push_back(this);

    randomseed = rseed;
    srand(randomseed);

    position.x = x;
    position.y = y;
    position.z = z;

    maxwidth = 10;
    maxheight = 40;
    //growthrate = 0.000001;  // reaches full height in about 30 days
    growthrate = 0.00000003;  // reaches full height in about 3 years
    //growthrate = 0.000000003;  // reaches full height in about 30 years

    if(treetype == FIRTREE_STANDARD) {
      height     = 5;   // default sizes
      bottom     = 1;
      width      = 1;
      trunkwidth = width / 5;
    } else if(treetype == FIRTREE_RANDOM) {
      // set its size to something random
      height     = 5 + ((float)rand() / RAND_MAX * 8) - 1;
      bottom     = 1.0 + ((float)rand() / RAND_MAX * 0.5);
      width      = 1 + ((float)rand() / RAND_MAX * 0.8) - 0.4;
      trunkwidth = width / 5;
    } else if(treetype == FIRTREE_SAPLING) {
      // make it tiny
      height     = 0.5;
      bottom     = 0.1;
      width      = 0.1;
      trunkwidth = width / 5;
    } else if(treetype == FIRTREE_SAPLING_RANDOM) {
      // make it tiny and random
      height     = 0.5 + ((float)rand() / RAND_MAX * 0.8) - 0.1;
      bottom     = 0.1 + ((float)rand() / RAND_MAX * 0.05);
      width      = 0.1 + ((float)rand() / RAND_MAX * 0.08) - 0.04;
      trunkwidth = width / 5;
    }
    // rendering setup
    vao = 0;
    vbo = 0;
    ibo = 0;
    if(hasvao) {
      glGenVertexArrays(1, &vao);
    }
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    updatevbo();
  }

  ~firtree() {     /// destructor
  }

  void updatevbo() {
    srand(randomseed);
    // rendering data
    // body:
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

  void update(double timespeed, double timedelta) {
    grow(growthrate * timespeed * timedelta);
  }

  void grow(double amount) {
    srand(randomseed);
    /// grow "rate" percent of this tree's height per second
    if(height < maxheight) {
      height += (height * amount);
      width += (width * amount);
      trunkwidth += (trunkwidth * amount);
      bottom += (bottom * amount);
      updatevbo();
    } else if(width < maxwidth) {
      width += (width * amount);
      //trunkwidth += (trunkwidth * amount);
      bottom += (bottom * amount);
      updatevbo();
    } else {
      // time to fall over
    }
  }

  void render() {
    glPushMatrix();
    {
      glTranslated(position.x, position.y, position.z);
      glColor4f(0.75, 0.75, 0.25, 1);
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
    glPopMatrix();
  }
};


class ashtree : public feature {
public:
  float maxheight;        // the tallest these can get
  float maxwidth;         // the widest they can get
  double growthrate;      // how fast it grows in % per second

  float height;           // overall height
  float bottom;           // bottom of the foliage from the ground
  float width;            // width of the bottom section / 2
  float trunkwidth;       // thickness of the trunk / 2

  ashtree(world *parentplanet, double x, double y, double z, int rseed) {      /// default constructor
    currentplanet = parentplanet;
    currentplanet->features.push_back(this);

    randomseed = rseed;
    srand(randomseed);

    position.x = x;
    position.y = y;
    position.z = z;

    maxwidth = 10;
    maxheight = 40;
    //growthrate = 0.000001;  // reaches full height in about 30 days
    growthrate = 0.00000003;  // reaches full height in about 3 years
    //growthrate = 0.000000003;  // reaches full height in about 30 years

    height     = 5;   // default sizes
    bottom     = 1.5;
    width      = 3.5;
    trunkwidth = 0.1;

    // rendering setup
    vao = 0;
    vbo = 0;
    ibo = 0;
    if(hasvao) {
      glGenVertexArrays(1, &vao);
    }
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    updatevbo();
  }

  ~ashtree() {     /// destructor
  }

  void updatevbo() {
    srand(randomseed);
    // rendering data
    std::vector<Vector3f> leaves;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for(int i = 0; i < 100; ++i) {
      Vector3f leaf;
      leaf.x = (float)rand() * width  / (float)RAND_MAX;
      leaf.y = ((float)rand() * (height - bottom) / (float)RAND_MAX) + bottom;
      leaf.z = (float)rand() * width  / (float)RAND_MAX;

      float leafsize = 0.1;

      leaves.push_back(leaf);
      vertices.push_back(leaf.x + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.y + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.z + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.x + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.y + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.z + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.x + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.y + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.z + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      indices.push_back((i*3));
      indices.push_back((i*3)+1);
      indices.push_back((i*3)+2);
    }
    numtris = indices.size() / 3;


    for(int i = 0; i < 100; ++i) {

    }

    glBindBuffer(GL_ARRAY_BUFFER,         vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER,         vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numtris * 3 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
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

  void update(double timespeed, double timedelta) {
    //grow(growthrate * timespeed * timedelta);
  }

  void grow(double amount) {
    /// grow "rate" percent of this tree's height per second
    srand(randomseed);
    if(height < maxheight) {
      height += (height * amount);
      width += (width * amount);
      trunkwidth += (trunkwidth * amount);
      bottom += (bottom * amount);
      updatevbo();
    } else if(width < maxwidth) {
      width += (width * amount);
      //trunkwidth += (trunkwidth * amount);
      bottom += (bottom * amount);
      updatevbo();
    } else {
      // time to fall over
    }
  }

  void render() {
    glPushMatrix();
    {
      glTranslated(position.x, position.y, position.z);
      glColor4f(0.75, 0.75, 0.25, 1);
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
    glPopMatrix();
  }
};


#endif // LANDSCAPE_FEATURES
