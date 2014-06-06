#include "Button.h"
#include "PrintText.h"
#include "Color.h"
#include <GL/glut.h>

Button::Button(std::string label) : ShadowedRectangle() {
    this->label = label;
    labelColor = new Color(0, 0, 0, 1);
    labelHoveredColor = new Color(1, 1, 1, 1);
    labelActiveColor = new Color(1, 0, 0, 1);

    /*mouseIsHovering = false;
    mouseIsPressing = false;
    active = true;*/

    color->r = 0.65;
    color->g = 0.65;
    color->b = 0.65;
}

Button::~Button() {
    delete labelColor;
    delete labelHoveredColor;
    delete labelActiveColor;
}

void Button::draw() {
    if (mouseIsPressing) {
        reverseShadow = true;
    } else {
        reverseShadow = false;
    }

    ShadowedRectangle::draw();

    float spacing = 2;
    float h = getInnerHeight() - 2 * spacing;
    float w = PrintText::strokeWidth(label, h);

    if (width < w + 2 * border + 2 * spacing) {
        width = w + 2 * border + 2 * spacing;
    }

    float cenX = x + width / 2;
    float cenY = y + height / 2;

    Color *c = labelColor;

    if (active) {
        if (mouseIsHovering) {
            c = labelHoveredColor;
        }
        if (mouseIsPressing) {
            c = labelActiveColor;
        }
    }
    
	glColor4f(c->r, c->g, c->b, c->a);
    PrintText::drawStrokeText(label, cenX, cenY, h, HORIZ_CENTER, VERT_CENTER);

    if (!active) {
        glColor4f(1, 1, 1, 0.3);
        glRectf(x,         y, 
                x + width, y + height);
    }
}

bool Button::mouseMoved(float mouseX, float mouseY) {   
    if (!active) {
        mouseIsHovering = false;

        return false;
    }

    if (ShadowedRectangle::containsPoint(mouseX, mouseY)) {
        mouseIsHovering = true;
    } else {
        mouseIsHovering = false;
    }

    return mouseIsHovering;
}

bool Button::mousePressed(float x, float y) {
    if (!active) {
        return false;
    }

    if (ShadowedRectangle::containsPoint(x, y)) {
        mouseIsPressing = true;
    } else {
        mouseIsPressing = false;
    }

    return mouseIsPressing;
}

bool Button::mouseReleased(float x, float y) {
    if (!active) {
        return false;
    }

    if (mouseIsPressing) {
        mouseIsPressing = false;

        if (ShadowedRectangle::containsPoint(x, y)) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}
