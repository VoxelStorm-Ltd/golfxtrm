#include "worldcomponents.h"
#include "golfer.h"
#include "holdable.h"

extern universe *root;
extern golfer *player;

void golfcourse::render() {
  // draw the terrain
  landscape->render();
  // draw the trees and other furniture
  // TODO
}

void world::render() {
  // give us a basic horizon, sky, atmospheric stuff
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  // ground
  glColor4f(0.75, 0.75, 0.25, 1);
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3i(0, 1, 0);
  glVertex3d(player->bodyposition.x - horizondistance, -0.01, player->bodyposition.z - horizondistance);
  glVertex3d(player->bodyposition.x + horizondistance, -0.01, player->bodyposition.z - horizondistance);
  glVertex3d(player->bodyposition.x - horizondistance, -0.01, player->bodyposition.z + horizondistance);
  glVertex3d(player->bodyposition.x + horizondistance, -0.01, player->bodyposition.z + horizondistance);
  glEnd();
  // sky
  glColor4f(skyred, skygreen, skyblue, 1);
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3i(0, -1, 0);
  glVertex3d(player->bodyposition.x - horizondistance, 60, player->bodyposition.z - horizondistance);
  glVertex3d(player->bodyposition.x + horizondistance, 60, player->bodyposition.z - horizondistance);
  glVertex3d(player->bodyposition.x - horizondistance, 60, player->bodyposition.z + horizondistance);
  glVertex3d(player->bodyposition.x + horizondistance, 60, player->bodyposition.z + horizondistance);
  glEnd();

  // iterate through the courses
  for(int i=0; i < numcourses; ++i) {
    course[i]->render();
  }

  // iterate through the players on this planet
  for(boost::ptr_vector<golfer>::iterator i = players.begin(); i != players.end(); ++i) {
    i->render(); // no indirection needed since this is boost's ptr_vector
  }

  // iterate through the items and draw those
  for(boost::ptr_vector<holdable>::iterator i = items.begin(); i != items.end(); ++i) {
    i->render();
  }
}

void universe::render() {
  /// Renders every planet in this universe
  // search the universe for worlds to render
  for (int p = 0; p < numplanets; ++p) {
    planet[p]->render();
  }
}
