#include "worldcomponents.h"
#include "globalvars_client_extern.h"
#include <iostream>

universe::universe() {                              /// default constructor
  std::cout << "  Initialising the universe" << std::endl;
  // big bang!
  numplanets = 0;

  updatetime = 1 / updatefreq;                                                  // set this from the global
  updatenexttime = 0;                                                           // this is ready for an update asap
  std::cout << "  Universe initialised" << std::endl;
}

void universe::addplanet(int worldnum) {            /// add a planet to this universe
  planet.push_back(new world());
  ++numplanets;
}
