#include "LineChartAxis.h"
#include "LineChart.h"
#include "PrintText.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <GL/glut.h>

LineChartAxis::LineChartAxis(LineChart *chart, int type) {
    this->chart = chart;
    axisType = type;    

    minorTickLength = 5.0;
    majorTickLength = 10.0;

    display = true;
    displayMinorTicks = true;
    displayMajorTicks = true;
    displayTickLabels = true;
    displayLabel = true;

    switch (axisType) {
        case AXIS_LEFT:      
        case AXIS_RIGHT:
            minorTickSpacing = 10000.0;
            majorTickSpacing = 50000.0;
            break;
        case AXIS_TOP:
        case AXIS_BOTTOM:
        default:            
            minorTickSpacing = 1.0;
            majorTickSpacing = 5.0;
            break;
    }

    font = GLUT_BITMAP_HELVETICA_10;

    fontHeight = 12;
}

LineChartAxis::~LineChartAxis() {

}

void LineChartAxis::draw() {
    glPushMatrix();
        float axisLength;

        switch (axisType) {
            case AXIS_TOP:
                axisLength = chart->getInnerWidth();
                glTranslatef(0, chart->getInnerHeight(), 0);
                glScalef(1.0, -1.0, 1.0);
                break;
            case AXIS_LEFT:            
                axisLength = chart->getInnerHeight();
                glRotatef(90, 0, 0, 1);
                glScalef(1.0, -1.0, 1.0);
                break;
            case AXIS_RIGHT:
                axisLength = chart->getInnerHeight();
                glTranslatef(chart->getInnerWidth(), 0, 0);
                glRotatef(90, 0, 0, 1);
                break;
            case AXIS_BOTTOM:
            default:
                axisLength = chart->getInnerWidth();
                break;
        }

        if (maxValue - minValue == 0) {
            maxValue = 1;
            setMajorTickSpacing(1.0);
            setMinorTickSpacing(1.0 / 5.0);
        } else {
            float interval = calculateIntervalSize(axisLength);       
            setMajorTickSpacing(interval);
            setMinorTickSpacing(interval / 5.0);
        }

        if (displayMinorTicks) {
            drawTicks(axisLength, minorTickSpacing, minorTickLength);
        }

        if (displayMajorTicks) {
            drawTicks(axisLength, majorTickSpacing, majorTickLength);
        }        
    glPopMatrix();

    if (displayTickLabels) {
       drawTickLabels(axisLength);
    }

    if (displayLabel) {
        drawLabel(axisLength);
    }
}

void LineChartAxis::drawLabel(float axisLength) {
    glColor4f(0.0, 0.0, 0.0, 1.0);

    float x, y;
    float fh = fontHeight;

    while (PrintText::strokeWidth(label, fh) > axisLength) {
        fh = fh - 0.5;
    }

    switch (axisType) {
        case AXIS_TOP:
            x = chart->getInnerWidth() / 2;
            if (displayTickLabels) {
                y = chart->getInnerHeight() + 2 * fontHeight / 3 + fontHeight;
            } else {
                y = chart->getInnerHeight() + fontHeight / 3;
            }
            PrintText::drawStrokeText(label, x, y, fh, HORIZ_CENTER, VERT_BOTTOM, false, 0);
            break;
        case AXIS_LEFT:            
            if (displayTickLabels) {
                x = -2 * fontHeight / 3 - fontHeight;
            } else {
                x = -fontHeight / 3;
            }
            y = chart->getInnerHeight() / 2;
            PrintText::drawStrokeText(label, x, y, fh, HORIZ_CENTER, VERT_BOTTOM, false, 90);
            break;
        case AXIS_RIGHT:
            if (displayTickLabels) {
                x = chart->getInnerWidth() + 2 * fontHeight / 3 + fontHeight;
            } else {
                x = chart->getInnerWidth() + fontHeight / 3;
            }
            y = chart->getInnerHeight() / 2;
            PrintText::drawStrokeText(label, x, y, fh, HORIZ_CENTER, VERT_BOTTOM, false, -90);
            break;
        case AXIS_BOTTOM:
        default:
            x = chart->getInnerWidth() / 2;
            if (displayTickLabels) {
                y = - 2 * fontHeight / 3 - fontHeight;
            } else {
                y = - fontHeight / 3;
            }
            PrintText::drawStrokeText(label, x, y, fh, HORIZ_CENTER, VERT_TOP, false, 0);
            break;
    }
}

void LineChartAxis::drawTickLabels(float axisLength) {
    glColor4f(0.0, 0.0, 0.0, 1.0);

    float value = minValue;
    float x, y;
    
    float fh = fontHeight;

    while (PrintText::strokeWidth(label, fh) > axisLength) {
        fh = fh - 0.5;
    }

    while (value <= maxValue) {
        std::string label = numToStr(value);
        switch (axisType) {
            case AXIS_TOP:
                x = valueToPosition(chart->getInnerWidth(), value);
                y = chart->getInnerHeight() + fontHeight / 3;
                PrintText::drawStrokeText(label, x, y, fh, HORIZ_CENTER, VERT_BOTTOM, false, 0);
                break;
            case AXIS_LEFT:            
                x = -fontHeight / 3;
                y = valueToPosition(chart->getInnerHeight(), value);
                PrintText::drawStrokeText(label, x, y, fh, HORIZ_CENTER, VERT_BOTTOM, false, 90);
                break;
            case AXIS_RIGHT:
                x = chart->getInnerWidth() + fontHeight / 3;
                y = valueToPosition(chart->getInnerHeight(), value);
                PrintText::drawStrokeText(label, x, y, fh, HORIZ_CENTER, VERT_BOTTOM, false, -90);
                break;
            case AXIS_BOTTOM:
            default:
                x = valueToPosition(chart->getInnerWidth(), value);
                y = -fontHeight / 3;
                PrintText::drawStrokeText(label, x, y, fh, HORIZ_CENTER, VERT_TOP, false, 0);
                break;
        } 

        value = value + majorTickSpacing;
    }
}  

void LineChartAxis::drawTicks(float axisLength, float tickSpacing, float tickLength) {
    float value = minValue;
    
    glLineWidth(1.0);
    glColor4f(0.5, 0.5, 0.5, 1.0);

    while (value <= maxValue) {
        float pos = valueToPosition(axisLength, value);

        glBegin(GL_LINES);
            glVertex3f(pos, 0.0, 0.0);
            glVertex3f(pos, tickLength, 0.0);
        glEnd();

        value = value + tickSpacing;
    }
}

float LineChartAxis::valueToPosition(float axisLength, float value) {
    float range = maxValue - minValue;
    float distFromMin = value - minValue;

    return distFromMin * axisLength / range;
}

float LineChartAxis::positionToValue(float axisLength, float position) {
    float range = maxValue - minValue;
    float distFromMin = position * range / axisLength;

    return minValue + distFromMin;
}

float LineChartAxis::calculateIntervalSize(float axisLength) {
    int numIntervals = 6;
    float range = maxValue - minValue;
    float tempInterval;

    float threshold;
    if (axisLength > 250) {
        threshold = 10;
    } else {
        threshold = 5;
        //numIntervals = 3;
    }

    while (numIntervals > 0) {
        tempInterval = range / numIntervals;
        float down = roundDown(tempInterval);
        
        if (tempInterval - down > tempInterval / 3) {
            float up = roundDown(tempInterval - down);
            tempInterval = down + up;
        } else {
            tempInterval = down;
        }

        float posA = valueToPosition(axisLength, minValue);
        float posB = valueToPosition(axisLength, minValue + tempInterval  / 5);

        if (posB - posA > threshold) {
            return tempInterval;
        }

        numIntervals--;
    }

    return range;
}


float LineChartAxis::round(float number) {
    float up = roundUp(number);
    float down = roundDown(up - number);

    return up - down;
}

//http://stackoverflow.com/questions/6364908/
float LineChartAxis::f(float num, float c) {
    return c * pow(10, floor(log10(num / c)));
}

float LineChartAxis::roundDown(float num) { 
    float m = std::max(f(num, 1), f(num, 2));

    return std::max(m, f(num, 5));
}

float LineChartAxis::g(float num, float c) { 
    return c * pow(10, ceil(log10(num / c)));
}

float LineChartAxis::roundUp(float num) { 
    float m = std::min(g(num, 1), g(num, 2));
    
    return std::min(m, g(num, 5));
}

float LineChartAxis::getSize() {
    if (!display) {
        return 0;
    }

    if (displayTickLabels && displayLabel) {
        return 3 * fontHeight;
    }

    if (!displayTickLabels && !displayLabel) {
        return 0;//3 * fontHeight / 2;
    }

    return fontHeight + 2 * fontHeight / 3;
}

void LineChartAxis::setMinimumValue(float m) {
    minValue = m;
}

void LineChartAxis::setMaximumValue(float m) {
    maxValue = m;
}