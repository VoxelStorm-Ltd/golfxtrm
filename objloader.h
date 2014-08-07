#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <fstream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <gl/gl.h>

class objloader {
public:

  std::vector<GLfloat> vertices;
  std::vector<GLfloat> normals;
  std::vector<GLuint>  faces;

  std::ifstream *file;
  objloader(std::string const &filename);
  void load();
  virtual ~objloader();
};

#endif // OBJLOADER_H
