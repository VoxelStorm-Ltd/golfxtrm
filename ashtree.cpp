#include <cstdlib>
#include "landscape_features.h"
#include "worldcomponents.h"
#include "globalvars_client_extern.h"

ashtree::ashtree(world *parentplanet, double x, double y, double z, int rseed) {      /// default constructor
  currentplanet = parentplanet;
  currentplanet->features.push_back(this);

  randomseed = rseed;
  std::srand(randomseed);

  position.x = x;
  position.y = y;
  position.z = z;

  name = "ash tree";
  description = "A medium-sized spindly tree, on the verge of extinction in Britain due to a new fungal parasite infection.";

  maxwidth = 10;
  maxheight = 40;
  //growthrate = 0.000001;  // reaches full height in about 30 days
  growthrate = 0.00000003;  // reaches full height in about 3 years
  //growthrate = 0.000000003;  // reaches full height in about 30 years

  height     = 5;   // default sizes
  bottom     = 1.5;
  width      = 3.5;
  trunkwidth = 0.1;
  if(bottom < 1.5) {
    collisionoffset.y = ((height - bottom) / 2) + bottom;
    boundingradius = width * 2;
  } else {  // it's tall enough for us to only collide with the trunk
    collisionoffset.y = 1;
    boundingradius = trunkwidth;
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

ashtree::~ashtree() {     /// destructor
}

void ashtree::updatevbo() {
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

void ashtree::update(double timespeed, double timedelta) {
  //grow(growthrate * timespeed * timedelta);
}

void ashtree::grow(double amount) {
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

void ashtree::render() {
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
