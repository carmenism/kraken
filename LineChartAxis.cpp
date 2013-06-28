#include "LineChartAxis.h"
#include "LineChart.h"
#include "PrintText.h"
#include <QtOpenGL>
#include <iostream>

LineChartAxis::LineChartAxis(LineChart *chart, int type) {
    this->chart = chart;
    axisType = type;    

    minorTickLength = 5.0;
    majorTickLength = 10.0;

    display = true;
    displayMinorTicks = true;
    displayMajorTicks = true;
    displayLabels = true;

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
}

LineChartAxis::~LineChartAxis() {

}

void LineChartAxis::draw() {
    glPushMatrix();
        float axisLength;

        switch (axisType) {
            case AXIS_TOP:
                axisLength = chart->getWidth();
                glTranslatef(0, chart->getHeight(), 0);
                glScalef(1.0, -1.0, 1.0);
                break;
            case AXIS_LEFT:            
                axisLength = chart->getHeight();
                glRotatef(90, 0, 0, 1);
                glScalef(1.0, -1.0, 1.0);
                break;
            case AXIS_RIGHT:
                axisLength = chart->getHeight();
                glTranslatef(chart->getWidth(), 0, 0);
                glRotatef(90, 0, 0, 1);
                break;
            case AXIS_BOTTOM:
            default:
                axisLength = chart->getWidth();
                break;
        }

        if (displayMinorTicks) {
            drawTicks(axisLength, minorTickSpacing, minorTickLength);
        }

        if (displayMajorTicks) {
            drawTicks(axisLength, majorTickSpacing, majorTickLength);
        }        
    glPopMatrix();

    if (displayLabels) {
       drawLabels();
    }
}

void LineChartAxis::drawLabels() {
    glColor4f(0.0, 0.0, 0.0, 1.0);

    float value = minValue;
    float x, y;

    while (value <= maxValue) {
        std::string label = getLabel(value);
        switch (axisType) {
            case AXIS_TOP:
                x = valueToPosition(chart->getWidth(), value);
                y = chart->getHeight() + 5;
                PrintText::printCenteredAt(x, y, label, false, false, GLUT_BITMAP_HELVETICA_10);
                break;
            case AXIS_LEFT:            
                x = -5;
                y = valueToPosition(chart->getHeight(), value);
                PrintText::printAlignedRightCenteredAt(x, y, label, false, false, GLUT_BITMAP_HELVETICA_10);
                break;
            case AXIS_RIGHT:
                x = chart->getWidth() + 5;
                y = valueToPosition(chart->getHeight(), value);
                PrintText::printVerticallyCenteredAt(x, y, label, false, false, GLUT_BITMAP_HELVETICA_10);
                break;
            case AXIS_BOTTOM:
            default:
                x = valueToPosition(chart->getWidth(), value);
                y = -10;
                PrintText::printCenteredAt(x, y, label, false, false, GLUT_BITMAP_HELVETICA_10);
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