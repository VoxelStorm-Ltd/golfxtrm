#ifndef PROGRESSBAR_H_INCLUDED
#define PROGRESSBAR_H_INCLUDED

//#include <iostream>

//#include "globaldefs.h"
//#include "globalvars_client.h"
/*
  Generic little widget to draw various kinds of progress bars in an
  OpenGL context, ranging from basic debugging text cout to various
  shiny special effects.
*/

void drawprogressbar(progressbartype type, double value, double maxvalue) {
  if(type == PROGRESSBAR_CONSOLE) {   // text-only debugging version - minimal resource use
    short percent = (value * 100) / maxvalue;
    //std::cout <<"Progress: "<<percent<<"%"<<std::endl;
    std::cout <<"\rProgress: "<<percent<<"%";
  } else if(type == PROGRESSBAR_HORIZONTAL) {  // basic small rectangular progress bar
    glLoadIdentity();
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_DEPTH_BUFFER_BIT);
    glTranslatef(0,0,-2);
    GLfloat backbar[]  = {0,0,0,1};
    GLfloat specular[] = {0,0,0,1};
    GLfloat emission[] = {0,0,0,1};
    glMateriali(GL_FRONT, GL_SHININESS, 5);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, backbar);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(-1.05,-0.09,0);
    glVertex3f(-1.05,0.09,0);
    glVertex3f(1.05,-0.09,0);
    glVertex3f(1.05,0.09,0);
    glEnd();

    GLfloat frontbar[]  = {0.25,1,0.25,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, frontbar);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(1,-0.04,0.01);
    glVertex3f(1,0.04,0.01);
    glVertex3f(1-((2*value)/maxvalue),-0.04,0.01);
    glVertex3f(1-((2*value)/maxvalue),0.04,0.01);
    glEnd();

    glfwSwapBuffers();

  } else if(type == PROGRESSBAR_DISC) {  // partial disc
    int angle = (value*360)/maxvalue;

    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0,0,-2);
    GLfloat backbar[]  = {0,0,0,1};
    GLfloat specular[] = {0,0,0,1};
    GLfloat emission[] = {0,0,0,1};
    glMateriali(GL_FRONT, GL_SHININESS, 5);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, backbar);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    gluDisk(gluNewQuadric(), 0.5, 0.8, 360, 1);

    GLfloat frontbar[]  = {1,0,0,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, frontbar);
    gluPartialDisk(gluNewQuadric(), 0.7, 0.75, angle, 1, 0, angle);

    glfwSwapBuffers();

    // put things back the way they were
    glEnable(GL_DEPTH_TEST);
  } else if(type == PROGRESSBAR_TILTEDDISC) {  // partial disc, tilted
    double angle = (value*360)/maxvalue;

    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRotatef(45, -1,  0, 0);
    glTranslatef(0,2,-1);
    GLfloat backbar[]  = {0,0,0,1};
    GLfloat specular[] = {0,0,0,1};
    GLfloat emission[] = {0,0,0,1};
    glMateriali(GL_FRONT, GL_SHININESS, 5);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, backbar);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    gluDisk(gluNewQuadric(), 0.6, 1.6, 360, 1);

    GLfloat frontbar1[]  = {0,0.5,0,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, frontbar1);
    gluPartialDisk(gluNewQuadric(), 1.35, 1.375, angle, 1, 0, angle);
    //GLfloat frontbar[]  = {0.25,1,0.25,1};
    GLfloat frontbar[]  = {0,1,0,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, frontbar);
    gluPartialDisk(gluNewQuadric(), 1.4, 1.5, angle, 1, 0, angle);

    glfwSwapBuffers();

    // put things back the way they were
    glEnable(GL_DEPTH_TEST);
  } else if(type == PROGRESSBAR_TILTINGDISC) {  // partial disc, tilting progressively
    // (perfect for transitioning between 2 and 3)
    double level = value/maxvalue;
    double angle = level * 360;

    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRotatef(45*level, -1*level,  0, 0);
    glTranslatef(0,2*level,-1-(3-(level*3)));
    GLfloat backbar[]  = {0,0,0,1};
    GLfloat specular[] = {0,0,0,1};
    GLfloat emission[] = {0,0,0,1};
    glMateriali(GL_FRONT, GL_SHININESS, 5);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, backbar);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    gluDisk(gluNewQuadric(), 1-(0.4*level), 1.6, 360, 1);

    GLfloat frontbar1[]  = {0.5,0.4,0,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, frontbar1);
    gluPartialDisk(gluNewQuadric(), 1.35, 1.375, angle, 1, 0, angle);
    //GLfloat frontbar[]  = {0.25,1,0.25,1};
    GLfloat frontbar[]  = {1,0.8,0,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, frontbar);
    gluPartialDisk(gluNewQuadric(), 1.4, 1.5, angle, 1, 0, angle);

    glfwSwapBuffers();

    // put things back the way they were
    glEnable(GL_DEPTH_TEST);
  }
}

#endif // PROGRESSBAR_H_INCLUDED
