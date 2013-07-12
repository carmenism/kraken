#include <GL/glut.h>
#include "Color.h"
#include "ShadowedRectangle.h"

ShadowedRectangle::ShadowedRectangle() {
    x = 0;
	y = 0;
    width = 100;
    height = 12;  
    border = 2;
}

void ShadowedRectangle::draw() {
    float leftX = x;
    float leftY = y;
    float rightX = x + width;// + 2 * border;
    float rightY = y + height;// + 2 * border;

	//glDisable(GL_DEPTH_TEST);

    glPolygonMode( GL_FRONT, GL_FILL ); 

	glColor4f(color->r, color->g, color->b, color->a);
	glRectf(leftX,  leftY,
            rightX, rightY);

    // Shadow
	glColor4f(0.0, 0.0, 0.0, shadowAlpha);
	glRectf(leftX,  leftY, 
            rightX, leftY + border);
	glRectf(rightX - border, leftY + border,
            rightX,          rightY);
	
    // Highlight
	glColor4f(1.0, 1.0, 1.0, highlightAlpha);
	glRectf(leftX,          leftY + border, 
            leftX + border, rightY);
	glRectf(leftX + border,  rightY - border,
            rightX - border, rightY);
}

bool ShadowedRectangle::containsPoint(float x, float y) {
    if (x < this->x || x > this->x + width) {
        return false;
    }
	
    if (y < this->y || y > this->y + height) {
        return false;
    }

    return true;
}
