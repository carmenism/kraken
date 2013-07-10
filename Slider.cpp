#include <GL/glut.h>
#include <iostream>
#include "Slider.h"
#include "Color.h"
#include "PrintText.h"

Slider::Slider(std::string label, float min, float max, float start) {
    this->label = label;
    this->minValue = min;
    this->maxValue = max;
    
    border = 2.0f;
    cursorWidth = 20.0f;

	cornerX = 0;
	cornerY = 0;
    width = 100 + cursorWidth;
    height = 12.0f;

    setValue(start);
	active = false;

    mainColor = new Color(0.5, 0.5, 0.5, 1.0);
    cursorColor = new Color(0.6, 0.6, 0.6, 1.0);
    shadowAlpha = 0.65;
    highlightAlpha = 0.65;

    fontHeight = 12;
}

void Slider::draw() {
    glColor4f(0.0, 0.0, 0.0, 1.0);
    PrintText::drawStrokeText(label, cornerX - 5, cornerY + height / 2, fontHeight, HORIZ_RIGHT, VERT_CENTER, 0);

    float leftX = cornerX;
    float leftY = cornerY;
    float rightX = cornerX + width + 2 * border;
    float rightY = cornerY + height + 2 * border;

	//glDisable(GL_DEPTH_TEST);

    glPolygonMode( GL_FRONT, GL_FILL ); 

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
	
	//glEnable(GL_DEPTH_TEST);
}

bool Slider::mousePressed(float x, float y) {    
    if (pointInCursor(x, y) && !active) {
        active = true;
        startX = x;
	    startCurX = curX;

        return true;
    }

    return false;
}

bool Slider::mouseReleased() {
    if (active) {
        active = false;

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
    if (active) {
		float dx = x - startX;
		curX = startCurX + dx;

        if (curX < border) {
            curX = border;
        }

        if (curX > (border + width - cursorWidth)) {
            curX = border + width - cursorWidth;
        }

        return true;
	}

	return false;
}

float Slider::getValue() {
    float percent = ((curX - border) / (width - cursorWidth));

    return minValue + percent * (maxValue - minValue);
}

void Slider::setValue(float value) {
    float range = maxValue - minValue;
    float distToStart = value - minValue;
    float percentage = distToStart / range;
    
    curX = border + percentage * (width - cursorWidth);
}

void Slider::setWidth(float w) {
    float value = getValue();
    width = w; 
    setValue(value);
}