#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class objloader
{
  public:
    ifstream *file;
    objloader(string filename);
    void load();
    virtual ~objloader();
  protected:
  private:
};

#endif // OBJLOADER_H
