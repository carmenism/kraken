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
    float size = 8; 
    float thickness = 2.0;

    glLineWidth(thickness);
    glColor3f(0.0,0.0,0.0);

    glBegin(GL_LINES); 
        glVertex2f(size / 2.0, 0.0);	 
        glVertex2f(-size / 2.0, 0.0);

        glVertex2f(0.0, -size / 2.0);
        glVertex2f(0.0, size / 2.0);	
    glEnd();
}

void Animatable::drawMinus() {
    float width = 9;
    float thickness = 3;

    glLineWidth(thickness);
    glColor3f(1.0,1.0,1.0);

    glBegin(GL_LINE_LOOP); 
        glVertex2f(width / 2.0, 0.0);	 
        glVertex2f(-width / 2.0, 0.0);	
    glEnd();
    
    glLineWidth(1.0);
    glColor3f(0.0,0.0,0.0);
    
    glBegin(GL_LINE_LOOP); 
        glVertex2f(width / 2.0, thickness / 2.0);	 
        glVertex2f(width / 2.0, -thickness / 2.0);	 
        glVertex2f(-width / 2.0, -thickness / 2.0); 
        glVertex2f(-width / 2.0, thickness / 2.0);		
    glEnd();
}