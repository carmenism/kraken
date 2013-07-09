#include "Chart.h"
#include "PrintText.h"
#include <cmath>
#include <algorithm>
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
        
        glColor4f(0, 0, 0, 1);
        PrintText::drawStrokeText(title, width / 2, height + 5, fontHeight, HORIZ_CENTER);
        
        drawAtOrigin();
    glPopMatrix();
}

float Chart::round(float number) {
    float up = roundUp(number);
    float down = roundDown(up - number);

    return up - down;
}

//http://stackoverflow.com/questions/6364908/
float Chart::f(float num, float c) {
    return c * pow(10, floor(log10(num / c)));
}

float Chart::roundDown(float num) { 
    float m = std::max(f(num, 1), f(num, 2));

    return std::max(m, f(num, 5));
}

float Chart::g(float num, float c) { 
    return c * pow(10, ceil(log10(num / c)));
}

float Chart::roundUp(float num) { 
    float m = std::min(g(num, 1), g(num, 2));
    
    return std::min(m, g(num, 5));
}