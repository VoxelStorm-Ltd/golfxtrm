#include "objloader.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

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
      boost::split(toks, line, boost::is_any_of(" "));

      //cout << toks[0] << endl;
    }
  }
}

objloader::~objloader() {
  this->file->close();
}
