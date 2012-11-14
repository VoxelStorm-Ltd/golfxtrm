#ifndef LANDSCAPE_FEATURES_H_INCLUDED
#define LANDSCAPE_FEATURES_H_INCLUDED

class feature {
public:
  Vector3d position;            // where its origin is in the world
  Quatd rotation;               // how it's oriented
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


class oaktree : public feature {
public:
  float maxheight;        // the tallest these can get
  float maxwidth;         // the widest they can get
  double growthrate;      // how fast it grows in % per second

  float height;           // overall height
  float bottom;           // bottom of the foliage from the ground
  float width;            // width of the bottom section / 2
  float trunkwidth;       // thickness of the trunk / 2

  enum oaktreetype {
    OAKTREE_STANDARD,
    OAKTREE_RANDOM,
    OAKTREE_SAPLING,
    OAKTREE_SAPLING_RANDOM
  };

  oaktree(world *parentplanet, double x, double y, double z, oaktreetype treetype, int rseed) {      /// default constructor
    currentplanet = parentplanet;
    currentplanet->features.push_back(this);

    randomseed = rseed;
    srand(randomseed);

    position.x = x;
    position.y = y;
    position.z = z;
    rotation = Quaternion<double>::fromAxisRot(Vector3d(0,1,0), rand() % 360);

    maxwidth = 40;
    maxheight = 50;
    //growthrate = 0.000002;  // reaches full height in about 60 days
    growthrate = 0.00000006;  // reaches full height in about 6 years
    //growthrate = 0.000000006;  // reaches full height in about 60 years

    if(treetype == OAKTREE_STANDARD) {
      height     = 5;   // default sizes
      bottom     = 1;
      width      = 1;
      trunkwidth = width / 5;
    } else if(treetype == OAKTREE_RANDOM) {
      // set its size to something random
      height     = 4 + ((float)rand() / RAND_MAX * 8) - 1;
      bottom     = 1.0 + ((float)rand() / RAND_MAX * 0.5);
      width      = 3 + ((float)rand() / RAND_MAX * 0.8) - 0.4;
      trunkwidth = width / 5;
    } else if(treetype == OAKTREE_SAPLING) {
      // make it tiny
      height     = 0.5;
      bottom     = 0.1;
      width      = 0.1;
      trunkwidth = width / 5;
    } else if(treetype == OAKTREE_SAPLING_RANDOM) {
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

  ~oaktree() {     /// destructor
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
      -width, height, -width,     // 12
      -width, height, width,      // 13
      width,  height, -width,     // 14
      width,  height, width,      // 15

    };
    GLuint ibodata[] = {
      6,4,0, 0,2,6,   // front
      3,1,5, 5,7,3,   // back
      2,0,1, 1,3,2,   // left
      7,5,4, 4,6,7,   // right
      2,6,7, 7,3,2,   // top
      5,4,0, 0,1,5,   // bottom

      14,12, 8,  8,10,14,   // front
      11, 9,13, 13,15,11,   // back
      10, 8, 9,  9,11,10,   // left
      15,13,12, 12,14,15,   // right
      10,14,15, 15,11,10,   // top
      13,12,8,   8, 9,13,   // bottom
      8, 10,11, 11, 9, 8,   // bottom
    };
    numtris = 36;

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
      glMultMatrixd(rotation.transform());
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

  struct branch {
    Vector3f start;
    Vector3f end;
    float thickness;
    bool grow_now;
    //branch *parent;
    unsigned int parent;  // index
  };

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

    std::vector<ashtree::branch> branches;
    std::vector<Vector3f> leaves;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    numtris = 0;

    for(int i = 0; i < 4000; ++i) {
      Vector3f leaf;
      leaf.x = ((float)rand() * width * 2 / (float)RAND_MAX) - width;
      leaf.y = ((float)rand() * (height - bottom) / (float)RAND_MAX) + bottom;
      leaf.z = ((float)rand() * width * 2 / (float)RAND_MAX) - width;

      float leafsize = 0.02;

      leaves.push_back(leaf);
      vertices.push_back(leaf.x + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.y + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.z + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      indices.push_back((vertices.size()/3)-1);
      //std::cout << "Triangle " << i << " : adding vertex " << (vertices.size()/3)-1;
      vertices.push_back(leaf.x + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.y + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.z + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      indices.push_back((vertices.size()/3)-1);
      //std::cout << " " << (vertices.size()/3)-1;
      vertices.push_back(leaf.x + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.y + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      vertices.push_back(leaf.z + ((float)rand() * leafsize  / (float)RAND_MAX) - (leafsize / 2));
      indices.push_back((vertices.size()/3)-1);
      //std::cout << " " << (vertices.size()/3)-1 << std::endl;
      ++numtris;
    }

    float growdist     = 0.1;
    float killrange    = 5 * growdist;
    float attractrange = 15 * growdist;

    branch rootbranch;
    rootbranch.start = Vector3f(0,0,0);
    rootbranch.end = Vector3f(0,0,0);
    rootbranch.thickness = 0;
    rootbranch.grow_now = false;
    rootbranch.parent = NULL;
    branches.push_back(rootbranch);
    vertices.push_back(rootbranch.start.x-0.1);
    vertices.push_back(rootbranch.start.y-0.1);
    vertices.push_back(rootbranch.start.z-0.1);
    indices.push_back((vertices.size()/3)-1);
    vertices.push_back(rootbranch.start.x+0.1);
    vertices.push_back(rootbranch.start.y+0.1);
    vertices.push_back(rootbranch.start.z+0.1);
    indices.push_back((vertices.size()/3)-1);
    vertices.push_back(rootbranch.end.x);
    vertices.push_back(rootbranch.end.y);
    vertices.push_back(rootbranch.end.z);
    indices.push_back((vertices.size()/3)-1);
    ++numtris;

    int keeprunningtree = 100;
    while(leaves.size() > 0 && keeprunningtree > 0) {
      std::cout << "=============== turns left " << keeprunningtree << ", leaves left " << leaves.size() << std::endl;
      unsigned int llimit = leaves.size();
      for(unsigned int l = 0; l < llimit; ++l) {
        int closestb = -1;
        float closestdist = 1000;
        for(unsigned int b = 0; b < branches.size(); ++b) {
          Vector3f btol = leaves[l] - branches[b].end;
          float distance = btol.length();
          // find closest tree node to this leaf
          if(distance < closestdist) {
            closestb = b;
            closestdist = distance;
          }
        }
        //std::cout << "Closest branch to leaf " << l << " is branch " << closestb << std::endl;;
        branches[closestb].grow_now = true;
      }
      unsigned int blimit = branches.size();
      for(unsigned int b = 0; b < blimit; ++b) {
        if(branches[b].grow_now) {
          branches[b].grow_now = false;
          // get all vectors of leaves within range of this branch
          Vector3f averagevector = Vector3f(0,0,0);
          int averagecount = 0;
          unsigned int llimit = leaves.size();
          for(unsigned int l = 0; l < llimit; ++l) {
            Vector3f btol = leaves[l] - branches[b].end;
            float distance = btol.length();
            if(distance < attractrange) {
              // normalise them
              Vector3f thisleaf = leaves[l];
              thisleaf.normalize();
              averagevector += thisleaf;
              ++averagecount;
              // check for leaves to remove
              if(distance < killrange) {
                std::cout << "Leaf removal: " << l << std::endl;
                branches[b].end = leaves[l];
                leaves.erase(leaves.begin() + l);
              }
            }
          }
          if(averagecount > 0) {    // check if we have any hits
            // average them
            averagevector /= averagecount;
            // normalise that
            averagevector.normalize();
          } else {
          // check it's valid (we may not be in range of anything)
          //if(std::isnan(averagevector.y)) {
            averagevector.x = (float)0;  // default to growing straight up
            averagevector.y = (float)1;
            averagevector.z = (float)0;
          }
          // multiply that by the extend distance
          averagevector *= growdist;

          // create new branch
          branch newbranch;
          newbranch.start = branches[b].end;
          newbranch.end = branches[b].end + averagevector;
          newbranch.thickness = 0;
          newbranch.grow_now = false;
          newbranch.parent = b;
          branches.push_back(newbranch);
          //std::cout << "Growing new branch " << branches.size() << " from " << b << " towards " << averagecount << " leaves" << std::endl;
        }
      }
      --keeprunningtree;
    }
    for(std::vector<branch>::iterator i = branches.begin(); i != branches.end(); ++i) {
      // add the visual data
      vertices.push_back(i->start.x-0.05);
      vertices.push_back(i->start.y-0.05);
      vertices.push_back(i->start.z-0.05);
      indices.push_back((vertices.size()/3)-1);
      vertices.push_back(i->start.x+0.05);
      vertices.push_back(i->start.y+0.05);
      vertices.push_back(i->start.z+0.05);
      indices.push_back((vertices.size()/3)-1);
      vertices.push_back(i->end.x);
      vertices.push_back(i->end.y);
      vertices.push_back(i->end.z);
      indices.push_back((vertices.size()/3)-1);
      ++numtris;
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
