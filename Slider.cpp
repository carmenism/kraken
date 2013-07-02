#include <GL/glut.h>
#include <iostream>
#include "Slider.h"
#include "Color.h"

Slider::Slider(float x, float y, float w, float start) {
    border = 2.0f;
    cursorWidth = 20.0f;

	cornerX = x;
	cornerY = y;
    width = w + cursorWidth;
    height = 15.0f;

	curX = border + start * width;
	state = INACTIVE;

    mainColor = new Color(0.5, 0.5, 0.5, 1.0);
    cursorColor = new Color(0.6, 0.6, 0.6, 1.0);
    shadowAlpha = 0.65;
    highlightAlpha = 0.65;
}


void Slider::draw() {
    float leftX = cornerX;
    float leftY = cornerY;
    float rightX = cornerX + width + 2 * border;
    float rightY = cornerY + height + 2 * border;

	glDisable(GL_DEPTH_TEST);

	glColor4f(mainColor->r, mainColor->g, mainColor->b, mainColor->a);
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

    float curLeftX = cornerX + curX;
    float curLeftY = cornerY + border;
    float curRightX = cornerX + curX + cursorWidth;
    float curRightY = cornerY + border + height;

	// Cursor
	glColor4f(cursorColor->r, cursorColor->g, cursorColor->b, cursorColor->a);
	glRectf(curLeftX,  curLeftY, 
            curRightX, curRightY);

    // Cursor shadow
    glColor4f(0.0, 0.0, 0.0, shadowAlpha);
	glRectf(curLeftX,  curLeftY,
            curRightX, curLeftY + border);
	glRectf(curRightX - border, curLeftY + border,
            curRightX,          curRightY);
	
    // Cursor highlight
    glColor4f(1.0, 1.0, 1.0, highlightAlpha);
	glRectf(curLeftX,          curLeftY + border, 
            curLeftX + border, curLeftY + height);
    glRectf(curLeftX + border,  curRightY - border,
            curRightX - border, curRightY);
	
	glEnable(GL_DEPTH_TEST);
}

bool Slider::mousePressed(float x, float y) {    
    if (pointInCursor(x, y) && state == INACTIVE) {
        state = ACTIVE;
        startX = x;
	    startCurX = curX;

        return true;
    }

    return false;
}

bool Slider::mouseReleased() {
    if (state == ACTIVE) {
        state = INACTIVE;

        return true;
    }

    return false;
}

bool Slider::pointInCursor(float x, float y) {
    float curLeftX = cornerX + curX;
    float curRightX = cornerX + curX + cursorWidth;

    if (x < curLeftX || x > curRightX) {
        return false;
    }
	
    float curLeftY = cornerY + border;
    float curRightY = cornerY + border + height;

    if (y < curLeftY || y > curRightY) {
        return false;
    }

    return true;
}

bool Slider::mouseMoved(float x, float y) {
    if (state == ACTIVE) {
		float dx = x - startX;
		curX = startCurX + dx;

        if (curX < border) {
            curX = border;
        }

        if (curX > (border + width - cursorWidth)) {
            curX = border + width - cursorWidth;
        }

        std::cerr << getValue() << "\n";

        return true;
	}

	return false;
}

float Slider::getValue() {
    return ((curX - border) / (width - cursorWidth));
}

void Slider::setValue(float value) {
    curX = border + value * width;
}