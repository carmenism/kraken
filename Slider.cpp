#include <GL/glut.h>
#include <iostream>
#include "Slider.h"
#include "Color.h"
#include "PrintText.h"

Slider::Slider(std::string title, float min, float max, float start) {
    this->title = title;
    this->minValue = min;
    this->maxValue = max;
    main = new ShadowedRectangle();
    main->setBorder(2);
    main->setWidth(100);
    main->setHeight(12 + 2 * main->getBorder());
    
    cursor = new ShadowedRectangle();
    cursor->setWidth(20);
    cursor->setHeight(12);
    cursor->setBorder(2);
    Color *curColor = cursor->getColor();
    curColor->r = 0.65;
    curColor->g = 0.65;
    curColor->b = 0.65;

	active = false;

    displayTitle = true;
    titleFontHeight = 12;
    titlePosition = POS_RIGHT;

    displayLabels = false;
    labelsBelow = true;
    labelFontHeight = 10;

    setValue(start);
    cursor->setLocation(main->getInnerX() + curX, main->getInnerY());

    valueHistory.push_back(start);
}

void Slider::undo() {
    if (valueHistory.size() > 1) {
        valueHistory.pop_back();
        float undoValue = valueHistory.back();

        setValue(undoValue);
    }
}

void Slider::reset() {
    float resetValue = valueHistory.front();
    valueHistory.clear();
    valueHistory.push_back(resetValue);
    setValue(resetValue);
}

void Slider::draw() {    
    main->draw();
    cursor->setLocation(main->getInnerX() + curX, main->getInnerY());
    cursor->draw();

    if (displayTitle) {
        drawTitle();
    }

    if (displayLabels) {
        drawLabels();
    }
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

        valueHistory.push_back(getValue());

        return true;
    }

    return false;
}

bool Slider::pointInCursor(float x, float y) {
    cursor->setLocation(main->getInnerX() + curX, main->getInnerY());
    return cursor->containsPoint(x, y);
}

bool Slider::mouseMoved(float x, float y) {
    if (active) {
		float dx = x - startX;
		curX = startCurX + dx;

        if (curX < 0) {
            curX = 0;
        }

        if (curX > main->getInnerWidth() - cursor->getWidth()) {
            curX = main->getInnerWidth() - cursor->getWidth();
        }

        return true;
	}

	return false;
}

float Slider::getValue() {
    return positionToValue(curX);
}

void Slider::setValue(float value) {    
    curX = valueToPosition(value);
}

void Slider::setWidth(float w) {
    float value = getValue();
    main->setWidth(w);
    setValue(value);
}

void Slider::setHeight(float h) {
    main->setHeight(h);
    cursor->setHeight(main->getInnerHeight());
}

void Slider::drawTitle() {
    float x;
    float y;
    int horiz;
    int vert;

    if (titlePosition == POS_RIGHT) {
        x = main->getX() + main->getWidth() + main->getBorder();
        y = main->getY() + main->getHeight() / 2;
        horiz = HORIZ_LEFT;
        vert = VERT_CENTER;
    } else if (titlePosition == POS_LEFT) {
        x = main->getX() - 2 * main->getBorder();
        y = main->getY() + main->getHeight() / 2;
        horiz = HORIZ_RIGHT;
        vert = VERT_CENTER;
    } else if (titlePosition == POS_ABOVE) {
        x = main->getX();
        y = main->getY() + main->getHeight() + 2 * main->getBorder();
        horiz = HORIZ_LEFT;
        vert = VERT_BOTTOM;
    } else {
        x = main->getX();
        y = main->getY() - main->getBorder();
        horiz = HORIZ_LEFT;
        vert = VERT_TOP;
    }

    glColor4f(0.0, 0.0, 0.0, 1.0);
    PrintText::drawStrokeText(title, x, y, titleFontHeight, horiz, vert);
}

void Slider::drawLabels() {
    float value = minValue;

    while (value <= maxValue) {
        std::string label = toStr(value);
        float x = main->getInnerX() + valueToPosition(value);
        float y = main->getY() - main->getBorder();
        int vertPos = VERT_TOP;

        if (!labelsBelow) {
            y = main->getY() + main->getHeight() + main->getBorder();
            vertPos = VERT_BOTTOM;
        }

        glColor4f(0.0, 0.0, 0.0, 1.0);        
        PrintText::drawStrokeText(label, x, y, labelFontHeight, HORIZ_CENTER, vertPos);
        
        value = value + labelInterval;
    }
}

float Slider::valueToPosition(float value) {
    float range = maxValue - minValue;
    float distToStart = value - minValue;
    float percentage = distToStart / range;
    
    return percentage * (main->getInnerWidth() - cursor->getWidth());
}

float Slider::positionToValue(float position) {
    float actualWidth = main->getInnerWidth() - cursor->getWidth();
    float percent = position / actualWidth;

    return minValue + percent * (maxValue - minValue);
}