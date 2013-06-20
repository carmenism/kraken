#include "GraphAxis.h"
#include <QtOpenGL>
#include <iostream>

GraphAxis::GraphAxis() {
    minorTickSpacing = 1.0;
    majorTickSpacing = 5.0;

    minorTickLength = 5.0;
    majorTickLength = 10.0;

    displayMinorTicks = true;
    displayMajorTicks = true;
}

GraphAxis::GraphAxis(float minV, float maxV) {
    minValue = minV;
    maxValue = maxV;

    minorTickSpacing = 1.0;
    majorTickSpacing = 5.0;

    minorTickLength = 10.0;
    majorTickLength = 20.0;

    displayMinorTicks = true;
    displayMajorTicks = true;
}

GraphAxis::~GraphAxis() {

}

void GraphAxis::draw(float graphLength) {
    if (displayMinorTicks) {
        drawTicks(graphLength, minorTickSpacing, minorTickLength);
    }

    if (displayMajorTicks) {
        drawTicks(graphLength, majorTickSpacing, majorTickLength);
    }

    std::cout << "DRAWING AXIS" << std::endl;
    std::cout << "min v " << minValue << std::endl;
    std::cout << "max v " << maxValue << std::endl;
}

void GraphAxis::drawTicks(float graphLength, float tickSpacing, float tickLength) {
    float value = minValue;
    
    glLineWidth(1.0);
    glColor4f(0.0, 0.0, 0.0, 1.0);

    while (value < maxValue) {

        float pos = valueToPosition(graphLength, value);

        std::cout << "value: " << value << ", pos: " << pos << std::endl;

        glBegin(GL_LINES);
            glVertex3f(pos, 0.0, 0.0);
            glVertex3f(pos, tickLength, 0.0);
        glEnd();

        value = value + tickSpacing;
    }


}

float GraphAxis::valueToPosition(float length, float value) {
    float range = maxValue - minValue;
    float distFromMin = value - minValue;

    return distFromMin * length / range;
}