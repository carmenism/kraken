#include "Button.h"
#include "PrintText.h"
#include "Color.h"

Button::Button(std::string label) : ShadowedRectangle() {
    this->label = label;
    labelColor = new Color(0, 0, 0, 1);
    labelHoveredColor = new Color(1, 1, 1, 1);
    labelActiveColor = new Color(1, 0, 0, 1);

    hovered = false;
    active = false;

    color->r = 0.65;
    color->g = 0.65;
    color->b = 0.65;
}

Button::~Button() {

}

void Button::draw() {
    if (active) {
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

    if (hovered) {
        c = labelHoveredColor;
    }
    if (active) {
        c = labelActiveColor;
    }
    
	glColor4f(c->r, c->g, c->b, c->a);
    PrintText::drawStrokeText(label, cenX, cenY, h, HORIZ_CENTER, VERT_CENTER);
}

bool Button::mouseMoved(float x, float y) {
    if (ShadowedRectangle::containsPoint(x, y)) {
        hovered = true;
    } else {
        hovered = false;
    }

    return hovered;
}

bool Button::mousePressed(float x, float y) {
    if (ShadowedRectangle::containsPoint(x, y)) {
        active = true;
    } else {
        active = false;
    }

    return active;
}

bool Button::mouseReleased(float x, float y) {
    if (active) {
        active = false;

        if (ShadowedRectangle::containsPoint(x, y)) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}