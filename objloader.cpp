#include "objloader.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include "globalvars_client_extern.h"

using namespace std;

objloader::objloader(string filename) {
  this->file = new ifstream(filename.c_str());
}

void objloader::load() {
  string line;

  if (this->file->is_open()) {
    while(this->file->good()) {
      getline(*file, line);

      vector<string> toks;
      boost::split(toks, line, boost::is_any_of(" /"));

      if (toks[0] == "v" && toks.size() >= 4) {
        vertices.push_back(::atof(toks[1].c_str()));
        vertices.push_back(::atof(toks[2].c_str()));
        vertices.push_back(::atof(toks[3].c_str()));
      } else if (toks[0] == "vn" && toks.size() >= 4) {
        normals.push_back(::atof(toks[1].c_str()));
        normals.push_back(::atof(toks[2].c_str()));
        normals.push_back(::atof(toks[3].c_str()));
      } else if (toks[0] == "f" && toks.size() >= 7) {
        for (int i = 0; i < 6; i++) {
          faces.push_back(::atoi(toks[i].c_str()));
        }
      }
    }
    cout << "Vertices: " << vertices.size() << endl;
    cout << "Normals: "  << normals.size()  << endl;
    cout << "Faces: "    << faces.size()    << endl;
  }
}

objloader::~objloader() {
  this->file->close();
}
