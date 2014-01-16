#include "Animatable.h"
#include <GL/glut.h>

void Animatable::drawSigns(bool positive) {
    if (positive) {
        drawPlus();
    } else {
        drawMinus();
    }
}

void Animatable::drawPlus() {
    glLineWidth(2.0);
    glColor3f(0.0,0.0,0.0);

    glBegin(GL_LINES); 
        glVertex2f(5.0,0.0);	 
        glVertex2f(-5.0,0.0);

        glVertex2f(0.0,-5.0);
        glVertex2f(0.0,5.0);	
    glEnd();
}

void Animatable::drawMinus() {
    glLineWidth(2.0);
    glColor3f(0.0,0.0,0.0);

    glBegin(GL_LINES); 
        glVertex2f(5.0,0.0);	 
        glVertex2f(-5.0,0.0);	
    glEnd();
}