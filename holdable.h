#ifndef HOLDABLE_H_INCLUDED
#define HOLDABLE_H_INCLUDED

class holdable {
public:
  golfer *held_by;          // who is holding this, NULL for lying/flying in the world
  bool at_rest;             // whether it's settled (in which case don't do further physics with it)

  double mass;              // how much it weighs (kg)
  double momentofinertia;   // moment of inertia (hand-hold as fulcrum)
  Vector3d cog;             // centre of gravity relative to hand-hold
  double airresistance;     // how much its flight is slowed by atmospheric drag
  double slideresistance;   // how much its slide along the ground is slowed

  std::string name;         // what it's called
  std::string description;  // longer description

  Vector3d bbox_start;      // one corner of the bounding box
  Vector3d bbox_end;        // other corner of the bounding box

  world *currentplanet;     // what planet are you even on?

  Vector3d position;        // location in space
  Vector3d velocity;        // movement velocity
  Quatd rotation;           // current rotation
  Quatd angularvelocity;    // rotational velocity

  GLuint vao;               // vertex array object
  GLuint numtris;         // number of triangles in the VBO

  holdable() {                                /// default constructor
    held_by = NULL;
    at_rest = true;

    currentplanet = NULL;
  }

  holdable(world *parentplanet) {             /// specific constructor
    held_by = NULL;
    at_rest = true;
    mass = 0;
    momentofinertia = 0;
    name = "object";
    description = "Some sort of object that hasn't been properly defined yet.";

    currentplanet = parentplanet;
    currentplanet->items.push_back(this);
  }

  ~holdable() {
    //currentplanet->items.erase(std::find(currentplanet->items.begin(), currentplanet->items.end(), this));
    //currentplanet->items.release(std::find(currentplanet->items.begin(), currentplanet->items.end(), this));
    // why won't that work?
    //holdable* ptr = this;
    //currentplanet->items.erase(std::find_if(currentplanet->items.begin(),
    //                                        currentplanet->items.end(),
    //                                        [ptr](const holdable &other)
    //                                        {return ptr == &other;} ));
  }

  virtual void push(Vector3d impulse) {  /// apply a one-off impulse to this object
    // TODO: apply max holding force, enable knocking out of hand
    velocity += impulse / mass;   // applied directly as a one-off, no delta time considered
  }

  virtual void push(Vector3d impulse, double targetx, double targety, double targetz) {
    /// apply a one-off impulse offset from centre of this object
    // TODO: apply max holding force, enable knocking out of hand
    velocity += impulse / mass;   // applied directly as a one-off, no delta time considered

    // TODO: calculate offset from COG and apply rotational acceleration
  }

  virtual void update(double timedelta) {
    /// update position and velocity based on time delta
    // only update if it's free in the air, not hand-held
    if(held_by == NULL) {
      position += (velocity * timedelta);

      // TODO: apply quaternion rotation
    }
  }

  virtual void render() {                           /// draw this item in the world
  }
};

class golfclub : public holdable {
public:

  golfclub(world *parentplanet) {
    held_by = NULL;
    at_rest = true;
    mass = 0;
    momentofinertia = 0;
    name = "golf club";
    description = "A long stick with a heavy end for hitting small balls with.";

    currentplanet = parentplanet;
    currentplanet->items.push_back(this);

    // rendering data
    // body:
    double top    = 0.9;
    double bottom = -0.1;
    double left   = -0.01;
    double right  = 0.01;
    double front  = 0.01;
    double back   = -0.01;
    GLfloat vbodata[] = {
      left,  bottom, back,    // 0
      left,  bottom, front,   // 1
      left,  top,    back,    // 2
      left,  top,    front,   // 3
      right, bottom, back,    // 4
      right, bottom, front,   // 5
      right, top,    back,    // 6
      right, top,    front    // 7
    };
    GLuint ibodata[] = {
      6,4,0, 0,2,6,   // front
      3,1,5, 5,7,3,   // back
      2,0,1, 1,3,2,   // left
      7,5,4, 4,6,7,   // right
      2,6,7, 7,3,2,   // top
      5,4,0, 0,1,5    // bottom
    };
    numtris = 12;

    // rendering setup
    vao = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER,         vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER,             sizeof(vbodata), vbodata, GL_STATIC_DRAW);
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

  void render() {
    /// alias function to render this object using the preferred method
    render1();
  }

  void render1() {      /// draw the terrain using an indexed VBO with VAA and VAO
    if(held_by == NULL) {
      // only draw objects that aren't held by someone (leave it to their own renderer otherwise)
      glPushMatrix();
        glTranslated(position.x, position.y, position.z);
        //glRotated(bodyyaw, 0, -1, 0);
        Matrix3d rotmatrix = rotation.rotMatrix();
        //std::cout << rotmatrix.toString() << std::endl;
        glColor4f(1, 1, 1, 1);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, numtris*3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      glPopMatrix();
    }
  }
};


#endif // HOLDABLE_H_INCLUDED
