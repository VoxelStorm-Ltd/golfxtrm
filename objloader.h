#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "globalvars_client_extern.h"

using namespace std;

class objloader {
  public:

    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLuint>  faces;

    ifstream *file;
    objloader(string filename);
    void load();
    virtual ~objloader();
  protected:
  private:
};

#endif // OBJLOADER_H
