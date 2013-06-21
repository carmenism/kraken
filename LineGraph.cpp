#include "LineGraph.h"
#include "GraphMarker.h"
#include <limits>
#include <iostream>
#include <QtOpenGL>
#include "Color.h"
#include "GraphAxis.h"
#include <QList>

LineGraph *LineGraph::createGraph(QList<QList<double>> matrix) {
    LineGraph *graph = new LineGraph();

    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> x;
        std::vector<float> y;

        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        LineGroup *lg = new LineGroup("Test", x, y);
        Color *c = Color::getUnassignedColor();
        lg->setColor(c);

        graph->addLine(lg);
    }

    graph->setHeight(480);
    graph->setWidth(600);
    graph->setLineWidths(2);
    graph->setMarkersSize(4);
    graph->displayMarkersOn();

    return graph;
}

LineGraph::LineGraph() {
    width = 400;
    height = 300;

    /*std::vector<float> x, y, y2;

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

    y.push_back(42);
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

    displayAsAreas();*/

    axisX = new GraphAxis(AXIS_BOTTOM);
    axisY = new GraphAxis(AXIS_LEFT);    
}

LineGraph::~LineGraph() {

}

void LineGraph::draw() {
    glPushMatrix();
    glTranslatef(50, 50, 0);

    drawBoundary();    
    calculateGlobalBounds();
    drawLines();  
    drawLabels();

    axisX->setMinimumValue(globalMinX);
    axisX->setMaximumValue(globalMaxX);
    axisX->draw(width, height);

    axisY->setMinimumValue(globalMinY);
    axisY->setMaximumValue(globalMaxY);
    axisY->draw(width, height);

    glPopMatrix();
}

void LineGraph::drawToPick() {
    glPushMatrix();
    glTranslatef(50, 50, 0);

    //drawBoundary();    
    calculateGlobalBounds();
    drawToPickLines();

    glPopMatrix();
}


void LineGraph::calculateGlobalBounds() {
    globalMinX = (std::numeric_limits<float>::max)();
    globalMaxX = -1 * (std::numeric_limits<float>::max)();

    globalMinY = (std::numeric_limits<float>::max)();
    globalMaxY = -1 * (std::numeric_limits<float>::max)();

    FOREACH_LINEGROUP(it, lines) {
        if ((*it)->getDisplay()) {
            float minX = (*it)->getMinimumValueX();
            float maxX = (*it)->getMaximumValueX();

            float minY = (*it)->getMinimumValueY();
            float maxY = (*it)->getMaximumValueY();

            if (globalMinX > minX) {
                globalMinX = minX;
            }

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

    globalMaxY = 1.075 * globalMaxY;//round(globalMaxY);
}

void LineGraph::drawBoundary() {  
    glPolygonMode(GL_FRONT, GL_LINE);   
    glColor4f(0, 0, 0, 1);

    glBegin(GL_LINE_LOOP);
        glVertex2f( 0, 0 );
        glVertex2f( 0, height );
        glVertex2f( width, height );
        glVertex2f( width, 0 );
    glEnd();
}

void LineGraph::drawLines() {
    FOREACH_LINEGROUP(it, lines) {
        if ((*it)->getDisplay()) {
            (*it)->draw(width, height, globalMaxX, globalMaxY);
        }
    }
}

void LineGraph::drawLabels() {
    FOREACH_LINEGROUP(it, lines) {
        if ((*it)->getDisplay()) {
            (*it)->drawLabels();
        }
    }
}   

void LineGraph::drawToPickLines() {
    FOREACH_LINEGROUP(it, lines) {
        if ((*it)->getDisplay()) {
            (*it)->drawToPick(width, height, globalMaxX, globalMaxY);
        }
    }
}

float LineGraph::round(float number) {
    float up = roundUp(number);
    float down = roundDown(up - number);

    return up - down;
}

void LineGraph::addLine(LineGroup *line) {
    lines.push_back(line);
    //line.setColor(Color::getUnassignedColor());
}

//http://stackoverflow.com/questions/6364908/
float LineGraph::f(float num, float c) {
    return c * pow(10, floor(log10(num / c)));
}

float LineGraph::roundDown(float num) { 
    float max = max(f(num, 1), f(num, 2));

    return max(max, f(num, 5));
}

float LineGraph::g(float num, float c) { 
    return c * pow(10, ceil(log10(num / c)));
}

float LineGraph::roundUp(float num) { 
    float min = min(g(num, 1), g(num, 2));
    
    return min(min, g(num, 5));
}

GraphMarkerList *LineGraph::getMarkers() {
    GraphMarkerList *newList = new GraphMarkerList();

    FOREACH_LINEGROUP(it, lines) {
        if ((*it)->getDisplay()) {
            FOREACH_MARKERP(it2, (*it)->getMarkers()) {
                newList->push_back((*it2));
            }
        }
    }

    return newList;
}

void LineGraph::setLineWidths(float w) {
    FOREACH_LINEGROUP(it, lines) {       
        (*it)->setLineWidth(w);
    }
}

void LineGraph::setDisplayMarkers(bool d) {
    FOREACH_LINEGROUP(it, lines) {       
        (*it)->setDisplayMarkers(d);
    }
}

void LineGraph::displayMarkersOn() {
    FOREACH_LINEGROUP(it, lines) {       
        (*it)->displayMarkersOn();
    }
}

void LineGraph::displayMarkersOff() {
    FOREACH_LINEGROUP(it, lines) {       
        (*it)->displayMarkersOff();
    }
}

void LineGraph::setMarkersSize(float s) {
    FOREACH_LINEGROUP(it, lines) {       
        (*it)->setMarkerSize(5.0);
    }
}

void LineGraph::displayAsAreas() {
    FOREACH_LINEGROUP(it, lines) {       
        (*it)->displayAsAreaOn();
    }
}

void LineGraph::displayAsLines() {  
    FOREACH_LINEGROUP(it, lines) {       
        (*it)->displayAsAreaOff();
    }
}