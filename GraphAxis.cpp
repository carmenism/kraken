#include "GraphAxis.h"
#include "PrintText.h"
#include <QtOpenGL>
#include <iostream>

GraphAxis::GraphAxis(int type) {
    axisType = type;    

    minorTickLength = 5.0;
    majorTickLength = 10.0;

    displayMinorTicks = true;
    displayMajorTicks = true;

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

GraphAxis::~GraphAxis() {

}

void GraphAxis::draw(float graphWidth, float graphHeight) {
    glPushMatrix();
        float axisLength;

        switch (axisType) {
            case AXIS_TOP:
                axisLength = graphWidth;
                glTranslatef(0, graphHeight, 0);
                glScalef(1.0, -1.0, 1.0);
                break;
            case AXIS_LEFT:            
                axisLength = graphHeight;
                glRotatef(90, 0, 0, 1);
                glScalef(1.0, -1.0, 1.0);
                break;
            case AXIS_RIGHT:
                axisLength = graphHeight;
                glTranslatef(graphWidth, 0, 0);
                glRotatef(90, 0, 0, 1);
                break;
            case AXIS_BOTTOM:
            default:
                axisLength = graphWidth;
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
       drawLabels(graphWidth, graphHeight);
    }
}


void GraphAxis::drawLabels(float graphWidth, float graphHeight) {
    switch (axisType) {
        case AXIS_TOP:
            drawHorizontalLabels(graphWidth, graphHeight + 5);
            break;
        case AXIS_LEFT:            
            drawLeftLabels(graphHeight, -5);
            break;
        case AXIS_RIGHT:
            drawRightLabels(graphWidth, graphHeight, 5);
            break;
        case AXIS_BOTTOM:
        default:
            drawHorizontalLabels(graphWidth, -10);
            break;
    } 
}    

void GraphAxis::drawRightLabels(float graphWidth, float graphHeight, float horizOffset) {
    float value = minValue;

    while (value <= maxValue) {
        float pos = valueToPosition(graphHeight, value);

        PrintText::printVerticallyCenteredAt(graphWidth + horizOffset, pos, getLabel(value), false, false, GLUT_BITMAP_HELVETICA_10);

        value = value + majorTickSpacing;
    }
}

void GraphAxis::drawLeftLabels(float graphHeight, float horizOffset) {
    float value = minValue;

    while (value <= maxValue) {
        float pos = valueToPosition(graphHeight, value);

        PrintText::printAlignedRightCenteredAt(horizOffset, pos, getLabel(value), false, false, GLUT_BITMAP_HELVETICA_10);

        value = value + majorTickSpacing;
    }
}

void GraphAxis::drawHorizontalLabels(float graphWidth, float vertOffset) {
    float value = minValue;

    while (value <= maxValue) {
        float pos = valueToPosition(graphWidth, value);

        PrintText::printCenteredAt(pos, vertOffset, getLabel(value), false, false, GLUT_BITMAP_HELVETICA_10);

        value = value + majorTickSpacing;
    }
}

void GraphAxis::drawTicks(float axisLength, float tickSpacing, float tickLength) {
    float value = minValue;
    
    glLineWidth(1.0);
    glColor4f(0.0, 0.0, 0.0, 1.0);

    while (value <= maxValue) {
        float pos = valueToPosition(axisLength, value);

        glBegin(GL_LINES);
            glVertex3f(pos, 0.0, 0.0);
            glVertex3f(pos, tickLength, 0.0);
        glEnd();

        value = value + tickSpacing;
    }
}

float GraphAxis::valueToPosition(float axisLength, float value) {
    float range = maxValue - minValue;
    float distFromMin = value - minValue;

    return distFromMin * axisLength / range;
}


std::string GraphAxis::getLabel(float value) {
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