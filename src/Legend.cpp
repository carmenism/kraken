#include "Legend.h"
#include <GL/glut.h>

Legend::Legend() {
    fontHeight = 12;
    x = 0;
    y = 0;
    padding = 5;
    display = false;
}

Legend::~Legend() {

}

void Legend::draw() {
    glPushMatrix();
        glTranslatef(x, y, 0); 
        drawAtOrigin();
        drawBoundary();
    glPopMatrix();
}