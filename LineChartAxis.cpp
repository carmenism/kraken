#include "LineChartAxis.h"
#include "PrintText.h"
#include <QtOpenGL>
#include <iostream>

LineChartAxis::LineChartAxis(int type) {
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

void LineChartAxis::draw(float chartWidth, float chartHeight) {
    glPushMatrix();
        float axisLength;

        switch (axisType) {
            case AXIS_TOP:
                axisLength = chartWidth;
                glTranslatef(0, chartHeight, 0);
                glScalef(1.0, -1.0, 1.0);
                break;
            case AXIS_LEFT:            
                axisLength = chartHeight;
                glRotatef(90, 0, 0, 1);
                glScalef(1.0, -1.0, 1.0);
                break;
            case AXIS_RIGHT:
                axisLength = chartHeight;
                glTranslatef(chartWidth, 0, 0);
                glRotatef(90, 0, 0, 1);
                break;
            case AXIS_BOTTOM:
            default:
                axisLength = chartWidth;
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
       drawLabels(chartWidth, chartHeight);
    }
}


void LineChartAxis::drawLabels(float chartWidth, float chartHeight) {
    glColor4f(0.0, 0.0, 0.0, 1.0);

    switch (axisType) {
        case AXIS_TOP:
            drawHorizontalLabels(chartWidth, chartHeight + 5);
            break;
        case AXIS_LEFT:            
            drawLeftLabels(chartHeight, -5);
            break;
        case AXIS_RIGHT:
            drawRightLabels(chartWidth, chartHeight, 5);
            break;
        case AXIS_BOTTOM:
        default:
            drawHorizontalLabels(chartWidth, -10);
            break;
    } 
}    

void LineChartAxis::drawRightLabels(float chartWidth, float chartHeight, float horizOffset) {
    float value = minValue;

    while (value <= maxValue) {
        float pos = valueToPosition(chartHeight, value);

        PrintText::printVerticallyCenteredAt(chartWidth + horizOffset, pos, getLabel(value), false, false, GLUT_BITMAP_HELVETICA_10);

        value = value + majorTickSpacing;
    }
}

void LineChartAxis::drawLeftLabels(float graphHeight, float horizOffset) {
    float value = minValue;

    while (value <= maxValue) {
        float pos = valueToPosition(graphHeight, value);

        PrintText::printAlignedRightCenteredAt(horizOffset, pos, getLabel(value), false, false, GLUT_BITMAP_HELVETICA_10);

        value = value + majorTickSpacing;
    }
}

void LineChartAxis::drawHorizontalLabels(float chartWidth, float vertOffset) {
    float value = minValue;

    while (value <= maxValue) {
        float pos = valueToPosition(chartWidth, value);

        PrintText::printCenteredAt(pos, vertOffset, getLabel(value), false, false, GLUT_BITMAP_HELVETICA_10);

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