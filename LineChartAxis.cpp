#include "LineChartAxis.h"
#include "LineChart.h"
#include "PrintText.h"
#include <QtOpenGL>
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
    displayLabel = false;

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
                axisLength = chart->getActualWidth();
                glTranslatef(0, chart->getActualHeight(), 0);
                glScalef(1.0, -1.0, 1.0);
                break;
            case AXIS_LEFT:            
                axisLength = chart->getActualHeight();
                glRotatef(90, 0, 0, 1);
                glScalef(1.0, -1.0, 1.0);
                break;
            case AXIS_RIGHT:
                axisLength = chart->getActualHeight();
                glTranslatef(chart->getActualWidth(), 0, 0);
                glRotatef(90, 0, 0, 1);
                break;
            case AXIS_BOTTOM:
            default:
                axisLength = chart->getActualWidth();
                break;
        }

        float interval = calculateIntervalSize(axisLength);       
        setMajorTickSpacing(interval);
        setMinorTickSpacing(interval / 5.0);

        if (displayMinorTicks) {
            drawTicks(axisLength, minorTickSpacing, minorTickLength);
        }

        if (displayMajorTicks) {
            drawTicks(axisLength, majorTickSpacing, majorTickLength);
        }        
    glPopMatrix();

    if (displayTickLabels) {
       drawTickLabels();
    }

    if (displayLabel) {
        drawLabel();
    }
}

void LineChartAxis::drawLabel() {
    glColor4f(0.0, 0.0, 0.0, 1.0);

    float x, y;

    switch (axisType) {
        case AXIS_TOP:
            x = chart->getActualWidth() / 2;
            if (displayTickLabels) {
                y = chart->getActualHeight() + 2 * fontHeight / 3 + fontHeight;
            } else {
                y = chart->getActualHeight() + fontHeight / 3;
            }
            PrintText::drawStrokeText(label, x, y, fontHeight, HORIZ_CENTER, VERT_BOTTOM);
            break;
        case AXIS_LEFT:            
            if (displayTickLabels) {
                x = -2 * fontHeight / 3 - fontHeight;
            } else {
                x = -fontHeight / 3;
            }
            y = chart->getActualHeight() / 2;
            PrintText::drawStrokeText(label, x, y, fontHeight, HORIZ_CENTER, VERT_BOTTOM, 90);
            break;
        case AXIS_RIGHT:
            if (displayTickLabels) {
                x = chart->getActualWidth() + 2 * fontHeight / 3 + fontHeight;
            } else {
                x = chart->getActualWidth() + fontHeight / 3;
            }
            y = chart->getActualHeight() / 2;
            PrintText::drawStrokeText(label, x, y, fontHeight, HORIZ_CENTER, VERT_BOTTOM, -90);
            break;
        case AXIS_BOTTOM:
        default:
            x = chart->getActualWidth() / 2;
            if (displayTickLabels) {
                y = - 2 * fontHeight / 3 - fontHeight;
            } else {
                y = - fontHeight / 3;
            }
            PrintText::drawStrokeText(label, x, y, fontHeight, HORIZ_CENTER, VERT_TOP);
            break;
    }
}

void LineChartAxis::drawTickLabels() {
    glColor4f(0.0, 0.0, 0.0, 1.0);

    float value = minValue;
    float x, y;

    while (value <= maxValue) {
        std::string label = getLabel(value);
        switch (axisType) {
            case AXIS_TOP:
                x = valueToPosition(chart->getActualWidth(), value);
                y = chart->getActualHeight() + fontHeight / 3;
                PrintText::drawStrokeText(label, x, y, fontHeight, HORIZ_CENTER, VERT_BOTTOM);
                break;
            case AXIS_LEFT:            
                x = -fontHeight / 3;
                y = valueToPosition(chart->getActualHeight(), value);
                PrintText::drawStrokeText(label, x, y, fontHeight, HORIZ_CENTER, VERT_BOTTOM, 90);
                break;
            case AXIS_RIGHT:
                x = chart->getActualWidth() + fontHeight / 3;
                y = valueToPosition(chart->getActualHeight(), value);
                PrintText::drawStrokeText(label, x, y, fontHeight, HORIZ_CENTER, VERT_BOTTOM, -90);
                break;
            case AXIS_BOTTOM:
            default:
                x = valueToPosition(chart->getActualWidth(), value);
                y = -fontHeight / 3;
                PrintText::drawStrokeText(label, x, y, fontHeight, HORIZ_CENTER, VERT_TOP);
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

std::string LineChartAxis::getLabel(float value) {
    int intValue = (int) value;

    if (intValue != 0 && intValue == value) {
        if (intValue % 1000000 == 0) {
            float newValue = value / 1000000;

            return toStr(newValue) + "M";
        } else if (intValue % 1000 == 0) {
            float newValue = value / 1000;

            return toStr(newValue) + "k";
        }
    }

    return toStr(value);
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
    float m = max(f(num, 1), f(num, 2));

    return max(m, f(num, 5));
}

float LineChartAxis::g(float num, float c) { 
    return c * pow(10, ceil(log10(num / c)));
}

float LineChartAxis::roundUp(float num) { 
    float m = min(g(num, 1), g(num, 2));
    
    return min(m, g(num, 5));
}

float LineChartAxis::getSize() {
    if (!display) {
        return 0;
    }

    if (displayTickLabels && displayLabel) {
        return 3 * fontHeight;
    }

    if (!displayTickLabels && !displayLabel) {
        return 3 * fontHeight / 2;
    }

    return fontHeight + 2 * fontHeight / 3;
}
