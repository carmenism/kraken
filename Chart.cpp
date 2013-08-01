#include "Chart.h"
#include "PrintText.h"
#include <GL/glut.h>

Chart::Chart() {
    xPos = 0;
    yPos = 0;
    fontHeight = 14;
    title = "";
}

void Chart::draw() {
    glPushMatrix();
        glTranslatef(xPos, yPos, 0); 
                
        drawAtOrigin();
    glPopMatrix();
}

void Chart::drawToPick() {
    glPushMatrix();
        glTranslatef(xPos, yPos, 0); 
        
        drawToPickAtOrigin();
    glPopMatrix();
}
