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

void Legend::drawBoundary() { 
    glDisable(GL_LINE_SMOOTH);
    glPolygonMode(GL_FRONT, GL_LINE);  
    glLineWidth(1.0);
    glColor4f(0.5, 0.5, 0.5, 1);

    glBegin(GL_LINE_LOOP);
        glVertex2f( 0, 0 );
        glVertex2f( 0, height );
        glVertex2f( width, height );
        glVertex2f( width, 0 );
    glEnd();
    glEnable(GL_LINE_SMOOTH);
}