#include "Chart.h"
#include "PrintText.h"
#include <GL/glut.h>

Chart::Chart() {
    x = 0;
    y = 0;
    fontHeight = 14;
    title = "";
    displayTitle = true;
}

void Chart::draw() {
    glPushMatrix();
        glTranslatef(x, y, 0); 
                
        drawAtOrigin();
    glPopMatrix();
}

void Chart::drawToPick() {
    glPushMatrix();
        glTranslatef(x, y, 0); 
        
        drawToPickAtOrigin();
    glPopMatrix();
}
