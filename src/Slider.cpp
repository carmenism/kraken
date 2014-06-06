#include <GL/glut.h>
#include <iostream>
#include "Slider.h"
#include "Color.h"
#include "PrintText.h"


Slider::Slider(std::string myTitle, std::vector<float> values, int startIndex) {
    valueType = CUSTOM;
    this->values = values;

    initialize(myTitle, values.at(startIndex));
}

Slider::Slider(std::string myTitle, float min, float max, float start) {
    valueType = LINEAR;
    this->minValue = min;
    this->maxValue = max;

    initialize(myTitle, start);
}

void Slider::initialize(std::string myTitle, float startValue) {
    this->title = myTitle;

    main = new ShadowedRectangle();
    main->setBorder(2);
    main->setWidth(100);
    main->setHeight(12 + 2 * main->getBorder());

    cursor = new Cursor();
    cursor->setWidth(12);
    cursor->setHeight(12);
    cursor->setBorder(2);

    Color *curColor = cursor->getColor();
    curColor->r = 0.65;
    curColor->g = 0.65;
    curColor->b = 0.65;    
    
    displayTitle = true;
    titleFontHeight = 12;
    titlePosition = POS_RIGHT;

    displayLabels = false;
    labelsBelow = true;
    labelFontHeight = 10;
    
    setValue(startValue);
    cursor->setLocation(main->getInnerX() + curX, main->getInnerY());

    valueHistory.push_back(startValue);
}

Slider::~Slider() {
    delete main;
    delete cursor;
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
    if (pointInCursor(x, y) && !mouseIsPressing) {
        mouseIsPressing = true;
        startX = x;
	    startCurX = curX;

        return true;
    }

    return false;
}

bool Slider::mouseReleased(float x, float y) {
    if (mouseIsPressing) {
        mouseIsPressing = false;

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
    if (mouseIsPressing) {
		float dx = x - startX;
		curX = startCurX + dx;

        if (curX < 0) {
            curX = 0;
        }

        if (curX > main->getInnerWidth()) {
            curX = main->getInnerWidth();
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
    if (valueType == LINEAR) {
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
    } else {
        float intervalPercentage = 1.0 / (values.size() - 1);
        float intervalSize = main->getInnerWidth() * intervalPercentage;

        for (unsigned int i = 0; i < values.size(); i++) {
            std::string label = toStr(values.at(i));

            float x = main->getInnerX() + intervalSize * i;
            float y = main->getY() - main->getBorder();

            int vertPos = VERT_TOP;

            if (!labelsBelow) {
                y = main->getY() + main->getHeight() + main->getBorder();
                vertPos = VERT_BOTTOM;
            }

            glColor4f(0.0, 0.0, 0.0, 1.0);        
            PrintText::drawStrokeText(label, x, y, labelFontHeight, HORIZ_CENTER, vertPos);            
        }
    }
}

float Slider::valueToPosition(float value) {
    if (valueType == LINEAR) {
        float range = maxValue - minValue;
        float distToStart = value - minValue;
        float percentage = distToStart / range;
        
        return percentage * main->getInnerWidth();
    }

    // CUSTOM

    if (value == values.front()) {
        return 0;
    } else if (value == values.back()) {
        return main->getInnerWidth();
    }

    int indexLow = -1;
    int indexHigh = -1;
    
    for (int i = 0; i < values.size() - 1; i++) {
        indexLow = i;
        indexHigh = i + 1;
        
        if (values.at(indexHigh) > value) {
            break;
        }
    }

    float valueLow = values.at(indexLow);
    float valueHigh = values.at(indexHigh);

    float distToLow = value - valueLow;
    float percentageToLow = distToLow / (valueHigh - valueLow);
    percentageToLow = std::min(1.0f, percentageToLow);
    
    float intervalSize = 1.0 / (values.size() - 1);
    float percentage = intervalSize * (indexLow + percentageToLow);
 
    return (percentage * main->getInnerWidth()); 
}

float Slider::positionToValue(float position) {
    float percent = position / main->getInnerWidth();

    if (valueType == LINEAR) {
        return minValue + percent * (maxValue - minValue);
    }

    // CUSTOM
    if (percent == 0) {
        return values.front();
    } else if (percent == 1) {
        return values.back();
    }

    float intervalSize = 1.0 / (values.size() - 1);
    int indexLow = int(percent / intervalSize);
    int indexHigh = indexLow + 1;
   
    float distFromLow = (percent - (indexLow * intervalSize)) / intervalSize;
    distFromLow = std::min(1.0f, distFromLow);

    float valueLow = values.at(indexLow);
    float valueHigh = values.at(indexHigh);    

    return valueLow + distFromLow * (valueHigh - valueLow);
}