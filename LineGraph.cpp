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

        GraphMarkerSeries *series = new GraphMarkerSeries(labels.at(i).toStdString(), x, y);
        Color *c = Color::getEvenlyDistributedColor(matrix.size(), i);
        series->setColor(c);

        graph->addMarkerSeries(series);
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

    setUpAxes();

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

        GraphMarkerSeries *series = new GraphMarkerSeries(labels.at(i).toStdString(), x, y);
        Color *c = Color::getEvenlyDistributedColor(matrix.size(), i);
        series->setColor(c);

        addMarkerSeries(series);
    } 

    legend = new GraphLegend(this);

    setUpAxes();

    setHeight(480);
    setWidth(600);
    setLineWidths(2);
    setMarkersSize(4);
    displayMarkersOn();
    displayLegendOn();
}

LineGraph::~LineGraph() {

}

void LineGraph::setUpAxes() {
    axes.push_back(new GraphAxis(AXIS_BOTTOM));
    axes.push_back(new GraphAxis(AXIS_TOP));
    axes.push_back(new GraphAxis(AXIS_LEFT));
    axes.push_back(new GraphAxis(AXIS_RIGHT));
    axes[AXIS_TOP]->displayOff();
    axes[AXIS_RIGHT]->displayOff();
}

void LineGraph::setValues(QList<QList<double>> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        seriesList[i]->setValues(x, y);
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
    drawXAxis(axes[AXIS_BOTTOM]);
    drawXAxis(axes[AXIS_TOP]);
    
    drawYAxis(axes[AXIS_LEFT]);
    drawYAxis(axes[AXIS_RIGHT]);
}

void LineGraph::drawXAxis(GraphAxis *axisX) {
    if (axisX->getDisplay()) {
        float xInterval = calculateIntervalSize(globalMinX, globalMaxX);
        
        axisX->setMinimumValue(globalMinX);
        axisX->setMaximumValue(globalMaxX);
        axisX->setMajorTickSpacing(xInterval);
        axisX->setMinorTickSpacing(xInterval / 5);

        axisX->draw(width, height);
    }
}

void LineGraph::drawYAxis(GraphAxis *axisY) {
    if (axisY->getDisplay()) {
        float yInterval = calculateIntervalSize(globalMinY, globalMaxY);

        axisY->setMinimumValue(0);
        axisY->setMaximumValue(globalMaxY);
        axisY->setMajorTickSpacing(yInterval);
        axisY->setMinorTickSpacing(yInterval / 5);

        axisY->draw(width, height);
    }
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

    FOREACH_MARKERSERIES(it, seriesList) {
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
    FOREACH_MARKERSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->draw(width, height, globalMaxX, globalMaxY);
        }
    }
}

void LineGraph::drawLabels() {
    FOREACH_MARKERSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->drawLabels();
        }
    }
}   

void LineGraph::drawToPickLines() {
    FOREACH_MARKERSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->drawToPick(width, height, globalMaxX, globalMaxY);
        }
    }
}

void LineGraph::addMarkerSeries(GraphMarkerSeries *series) {
    seriesList.push_back(series);
}

GraphMarkerList *LineGraph::getMarkers() {
    GraphMarkerList *newList = new GraphMarkerList();

    FOREACH_MARKERSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            FOREACH_MARKERP(it2, (*it)->getMarkers()) {
                newList->push_back((*it2));
            }
        }
    }

    return newList;
}

void LineGraph::setLineWidths(float w) {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->setLineWidth(w);
    }
}

void LineGraph::setDisplayMarkers(bool d) {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->setDisplayMarkers(d);
    }
}

void LineGraph::displayMarkersOn() {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->displayMarkersOn();
    }
}

void LineGraph::displayMarkersOff() {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->displayMarkersOff();
    }
}

void LineGraph::setMarkersSize(float s) {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->setMarkerSize(s);
    }
}

void LineGraph::displayAsAreas() {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->displayAsAreaOn();
    }
}

void LineGraph::displayAsLines() {  
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->displayAsAreaOff();
    }
}

GraphAxis *LineGraph::getBottomAxis() {
    return axes[AXIS_BOTTOM];
}

GraphAxis *LineGraph::getTopAxis() {
    return axes[AXIS_TOP];
}

GraphAxis *LineGraph::getLeftAxis() {
    return axes[AXIS_LEFT];
}

GraphAxis *LineGraph::getRightAxis() {
    return axes[AXIS_RIGHT];
}

bool LineGraph::getBottomAxisDisplay() {
    return axes[AXIS_BOTTOM]->getDisplay();
}

bool LineGraph::getTopAxisDisplay() {
    return axes[AXIS_TOP]->getDisplay();
}

bool LineGraph::getLeftAxisDisplay() {
    return axes[AXIS_LEFT]->getDisplay();
}

bool LineGraph::getRightAxisDisplay() {
    return axes[AXIS_RIGHT]->getDisplay();
}

void LineGraph::setBottomAxisDisplay(bool d) {
    axes[AXIS_BOTTOM]->setDisplay(d);
}

void LineGraph::setTopAxisDisplay(bool d) {
    axes[AXIS_TOP]->setDisplay(d);
}

void LineGraph::setLeftAxisDisplay(bool d) {
    axes[AXIS_LEFT]->setDisplay(d);
}

void LineGraph::setRightAxisDisplay(bool d) {
    axes[AXIS_RIGHT]->setDisplay(d);
}