#include "LineGraph.h"
#include "LineGroup.h"
#include "GraphMarker.h"
#include <limits>
#include <iostream>
#include <QtOpenGL>
#include "Color.h"

LineGraph::LineGraph() {
    width = 400;
    height = 300;

    std::vector<float> x, y, y2;

    x.push_back(0);
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    x.push_back(4);
    x.push_back(5);
    x.push_back(6);
    x.push_back(7);
    x.push_back(8);
    x.push_back(9);

    y.push_back(20);
    y.push_back(30);
    y.push_back(80);
    y.push_back(16);
    y.push_back(37);
    y.push_back(22);
    y.push_back(76);
    y.push_back(66);
    y.push_back(53);
    y.push_back(98);

    y2.push_back(60);
    y2.push_back(34);
    y2.push_back(34);
    y2.push_back(54);
    y2.push_back(87);
    y2.push_back(123);
    y2.push_back(88);
    y2.push_back(44);
    y2.push_back(3);
    y2.push_back(16);

    LineGroup *lg = new LineGroup("Test", x, y);
    LineGroup *lg2 = new LineGroup("Test", x, y2);
    lg2->setMarkerShape(SHAPE_SQUARE);
    lines.push_back(lg);
    lines.push_back(lg2);

    Color *c = Color::getUnassignedColor();
    lg->setColor(c);

    Color *c2 = Color::getUnassignedColor();
    lg2->setColor(c2);
}

LineGraph::~LineGraph() {

}

void LineGraph::redraw() {
    glBegin(GL_LINE_LOOP);
        glVertex2f( 0, 0 );
        glVertex2f( 0, height );
        glVertex2f( width, height );
        glVertex2f( width, 0 );
    glEnd();

    LineGroupList it;
    
    float globalMaxX = -1 * (std::numeric_limits<float>::max)();

    float globalMinY = (std::numeric_limits<float>::max)();
    float globalMaxY = -1 * (std::numeric_limits<float>::max)();

    FOREACH_LINEGROUP(it, lines) {
        if ((*it)->getDisplay()) {
            float maxX = (*it)->getMaximumValueX();

            float minY = (*it)->getMinimumValueY();
            float maxY = (*it)->getMaximumValueY();

            if (globalMaxX < maxX) {
                globalMaxX = maxX;
            }

            if (globalMinY > minY) {
                globalMinY = minY;
            }

            if (globalMaxY < maxY) {
                globalMaxY = maxY;
            }
        }
    }

    std::cout << "max y: " << globalMaxY << std::endl;
    //std::cout << "round down max y: " << roundDown(globalMaxY) << std::endl;
    globalMaxY = round(globalMaxY);
    std::cout << "max y: " << globalMaxY << std::endl;

    FOREACH_LINEGROUP(it, lines) {
        if ((*it)->getDisplay()) {
            (*it)->redraw(width, height, globalMaxX, globalMaxY);
        }
    }
}

float LineGraph::round(float number) {
    float up = roundUp(number);
    float down = roundDown(up - number);

    return up - down;
}

float LineGraph::roundUp(float number) {
    int exponent = (int) floor(log10(number));
    float val = number * pow(10.0, -exponent);

    if (val > 5.0) {
        val = 10.0;
    } else if (val > 2.0) {
        val = 5.0;
    } else if (val > 1.0) {
        val = 2.0;
    }

    val = val * pow(10.0, exponent);

    return val;
}

float LineGraph::roundDown(float number) {
    float f1 = 1 * pow(10, (floor(log10(number / 1))));
    float f2 = 2 * pow(10, (floor(log10(number / 2))));
    float f5 = 5 * pow(10, (floor(log10(number / 5))));

    float max = max(f1, f2);
    max = max(max, f5);

    return max;
}