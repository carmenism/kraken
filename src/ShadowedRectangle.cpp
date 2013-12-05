#include <GL/glut.h>
#include "Color.h"
#include "ShadowedRectangle.h"

ShadowedRectangle::ShadowedRectangle() {
    x = 0;
	y = 0;
    width = 100;
    height = 12;  
    border = 2;
    shadowAlpha = 0.65;
    highlightAlpha = 0.65;
    color = new Color(0.5, 0.5, 0.5, 1.0);
    reverseShadow = false;
}

ShadowedRectangle::~ShadowedRectangle() {
    delete color;
}

void ShadowedRectangle::draw() {
    float leftX = x;
    float leftY = y;
    float rightX = x + width;
    float rightY = y + height;

    glPolygonMode( GL_FRONT, GL_FILL ); 

	glColor4f(color->r, color->g, color->b, color->a);
	glRectf(leftX,  leftY,
            rightX, rightY);

    // Shadow
	glColor4f(0.0, 0.0, 0.0, shadowAlpha);
    
    if (reverseShadow) {
        glColor4f(1.0, 1.0, 1.0, highlightAlpha);
    }

	glRectf(leftX,  leftY, 
            rightX, leftY + border);
	glRectf(rightX - border, leftY + border,
            rightX,          rightY);
	
    // Highlight
	glColor4f(1.0, 1.0, 1.0, highlightAlpha);

    if (reverseShadow) {
        glColor4f(0.0, 0.0, 0.0, shadowAlpha);
    }

	glRectf(leftX,          leftY + border, 
            leftX + border, rightY);
	glRectf(leftX + border,  rightY - border,
            rightX - border, rightY);
}

bool ShadowedRectangle::containsPoint(float mouseX, float mouseY) {
    return (mouseX >= this->x && mouseX <= (this->x + width)
         && mouseY >= this->y && mouseY <= (this->y + height));
}
