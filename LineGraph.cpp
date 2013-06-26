#include "LineGraph.h"
#include "GraphMarker.h"
#include "Color.h"
#include "GraphAxis.h"
#include "GraphLegend.h"
#include <QList>
#include <QStringList>
#include <QString>
#include <limits>
#include <iostream>
#include <QtOpenGL>
#include <GL/glut.h>

LineGraph *LineGraph::createGraph(QList<QList<double>> matrix, QStringList labels) {
    LineGraph *graph = new LineGraph();

    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        LineGroup *lg = new LineGroup(labels.at(i).toStdString(), x, y);
        Color *c = Color::getEvenlyDistributedColor(matrix.size(), i);
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

    axisX = new GraphAxis(AXIS_BOTTOM);
    axisY = new GraphAxis(AXIS_LEFT);   
    legend = new GraphLegend(this);

    displayLegend = true;
}

LineGraph::LineGraph(QList<QList<double>> matrix, QStringList labels) {
    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        LineGroup *lg = new LineGroup(labels.at(i).toStdString(), x, y);
        Color *c = Color::getEvenlyDistributedColor(matrix.size(), i);
        lg->setColor(c);

        addLine(lg);
    }
    
    axisX = new GraphAxis(AXIS_BOTTOM);
    axisY = new GraphAxis(AXIS_LEFT);   
    legend = new GraphLegend(this);

    setHeight(480);
    setWidth(600);
    setLineWidths(2);
    setMarkersSize(4);
    displayMarkersOn();
    displayLegendOn();
}

LineGraph::~LineGraph() {

}


void LineGraph::setValues(QList<QList<double>> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        lines[i]->setValues(x, y);
    }
}

void LineGraph::draw() {
    glPushMatrix();
        glTranslatef(50, 50, 0);
        drawBoundary();    
        calculateGlobalBounds();
        drawAxes();
        drawLines();  
        drawLabels();

        if (displayLegend) {
            legend->draw(width + 5, height / 2, 15, 5, GLUT_BITMAP_HELVETICA_10);
        }
    glPopMatrix();
}

void LineGraph::drawAxes() {
    float xInterval = calculateIntervalSize(globalMinX, globalMaxX);
    float yInterval = calculateIntervalSize(globalMinY, globalMaxY);

    axisX->setMinimumValue(globalMinX);
    axisX->setMaximumValue(globalMaxX);
    axisX->setMajorTickSpacing(xInterval);
    axisX->setMinorTickSpacing(xInterval / 5);

    axisY->setMinimumValue(0);
    axisY->setMaximumValue(globalMaxY);
    axisY->setMajorTickSpacing(yInterval);
    axisY->setMinorTickSpacing(yInterval / 5);

    axisX->draw(width, height);
    axisY->draw(width, height);
}

float LineGraph::calculateIntervalSize(float min, float max) {
    float range = max - min;
    float tempInterval = range / 8.0;

    return roundUp(tempInterval);
}

void LineGraph::drawToPick() {
    glPushMatrix();
    glTranslatef(50, 50, 0);
 
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

    globalMaxY = globalMaxY + 0.05 * (globalMaxY - globalMinY);
}

void LineGraph::drawBoundary() {  
    glPolygonMode(GL_FRONT, GL_LINE);  
    glLineWidth(1.0);
    glColor4f(0.5, 0.5, 0.5, 1);

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
        (*it)->setMarkerSize(s);
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