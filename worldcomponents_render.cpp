#include "worldcomponents.h"
#include "golfer.h"
#include "holdable.h"
#include "landscape_features.h"

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
  glColor4fv(groundcolour);
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3i(0, 1, 0);
  glVertex3d(player->bodyposition.x - horizondistance, -0.01, player->bodyposition.z - horizondistance);
  glVertex3d(player->bodyposition.x + horizondistance, -0.01, player->bodyposition.z - horizondistance);
  glVertex3d(player->bodyposition.x - horizondistance, -0.01, player->bodyposition.z + horizondistance);
  glVertex3d(player->bodyposition.x + horizondistance, -0.01, player->bodyposition.z + horizondistance);
  glEnd();
  // sky
  glColor4fv(skycolour);
  //glColor4f(0, 0, 1, 1);
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

  // iterate through the terrain features
  for(boost::ptr_vector<feature>::iterator i = features.begin(); i != features.end(); ++i) {
    i->render(); // no indirection needed since this is boost's ptr_vector
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

void golfer::render5() {          /// draw this fellow using an indexed VBO with VAA and VAO
  glColor4f(0.25,0.25,0.25,1);
  Vector3d fulcrum_to_hands;
  Vector3d fulcrum_to_shoulder;
  double armsectionlength = (armlength + armshoulderoffset) / 2;
  fulcrum_to_hands.z = armlength;
  fulcrum_to_hands.rotate(0, armsyaw, 0);

  glPushMatrix();   // body
  {
    glTranslated(bodyposition.x, bodyposition.y, bodyposition.z);
    glRotated(bodyyaw, 0, -1, 0);

    glBindVertexArray(vao_body);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glPushMatrix();   // hands
    {
      glColor4f(skincolour.r, skincolour.g, skincolour.b, 1);
      glTranslated(armfulcrum.x, armfulcrum.y, armfulcrum.z);
      glRotated(armspitch, -1,  0, 0);
      glRotated(armsyaw,    0, -1, 0);
      glBindVertexArray(vao_hands);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
      if(helditem != NULL) {  // render the held item
        glPushMatrix();
        {
          glTranslated(0, 0, -armlength);
          glRotated((armspitch+112)*0.4, -1, 0, 0);   // (75/0.4)-75 = 112
          //std::cout << armspitch << std::endl;
          helditem->renderlocal();
        }
        glPopMatrix();
      }
    }
    glPopMatrix();
    glPushMatrix();   // right upper arm
    {
      glColor4f(0.25,0.25,0.25,1);
      fulcrum_to_shoulder.x = armshoulderoffset;
      Vector3d shoulder_to_hands = fulcrum_to_hands - fulcrum_to_shoulder;
      double armangle = atan2(shoulder_to_hands.x, shoulder_to_hands.z) * 180 / M_PI;
      double elbowangle = acos((shoulder_to_hands.length()/2) / armsectionlength) * 180 / M_PI;
      glTranslated(armfulcrum.x + armshoulderoffset, armfulcrum.y, armfulcrum.z);
      glRotated(armspitch, -1, 0, 0);
      glRotated(armangle + elbowangle, 0, -1, 0);
      glBindVertexArray(vao_arms);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
      glPushMatrix();   // right forearm
      {
        glTranslated(0, 0, -armsectionlength);
        glRotated(elbowangle * 2, 0, 1, 0);
        glBindVertexArray(vao_arms);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      }
      glPopMatrix();
    }
    glPopMatrix();
    glPushMatrix();   // left upper arm
    {
      fulcrum_to_shoulder.x = -armshoulderoffset;
      Vector3d shoulder_to_hands = fulcrum_to_hands - fulcrum_to_shoulder;
      double armangle = atan2(shoulder_to_hands.x, shoulder_to_hands.z) * 180 / M_PI;
      double elbowangle = acos((shoulder_to_hands.length()/2) / armsectionlength) * 180 / M_PI;
      glTranslated(armfulcrum.x - armshoulderoffset, armfulcrum.y, armfulcrum.z);
      glRotated(armspitch, -1, 0, 0);
      glRotated(armangle - elbowangle, 0, -1, 0);
      glBindVertexArray(vao_arms);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
      glPushMatrix();   // left forearm
      {
        glTranslated(0, 0, -armsectionlength);
        glRotated(-elbowangle * 2, 0, 1, 0);
        glBindVertexArray(vao_arms);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      }
      glPopMatrix();
    }
    glPopMatrix();
    if(isplayer) {
      // anything we draw for internal view should go here
    } else {
      glPushMatrix();   // head
      {
        glColor4f(skincolour.r, skincolour.g, skincolour.b, 1);
        glTranslated(headfulcrum.x, headfulcrum.y, headfulcrum.z);
        glRotated(headpitch, -1, 0, 0);
        glTranslated(eyeleveloffset.x, eyeleveloffset.y, eyeleveloffset.z);
        glRotated(headyaw, 0, -1, 0);
        glBindVertexArray(vao_head);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      }
      glPopMatrix();
    }
  }
  glPopMatrix();
}
