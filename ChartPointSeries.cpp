#include "ChartPointSeries.h"
#include "ChartPoint.h"
#include "Color.h"
#include "LineChart.h"
#include "PrintText.h"
#include "LineChart.h"
#include <QtOpenGL>


ChartPointSeries::ChartPointSeries(LineChart *chart, std::string label, std::vector<float> x, std::vector<float> y) {
    this->chart = chart;
    this->label = label;
    
    if (x.size() != y.size()) {
        throw "should be equal number of x and y values";
    }

    if (x.empty() || y.empty()) {
        throw "should not be empty values for x or y";
    }

    max = NULL;
    min = NULL;

    for (int i = 0; i < x.size(); i++) {
        ChartPoint *point = new ChartPoint(chart, label, x[i], y[i]);
        points.push_back(point);

        if (max == NULL || max->getValueY() < point->getValueY()) {
            max = point;
        }
        if (min == NULL || min->getValueY() > point->getValueY()) {
            min = point;
        }
    }

    lineColor = &Color::black;
    lineWidth = 1.0;

    displayAsArea = false;

    legendPoint = new ChartPoint(chart, "", -1, -1);
}

void ChartPointSeries::setValues(std::vector<float> x, std::vector<float> y) {
    if (x.size() != y.size()) {
        throw "should be equal number of x and y values";
    }

    if (x.empty() || y.empty()) {
        throw "should not be empty values for x or y";
    }   

    if (points.size() != y.size()) {
        throw "to change marker values you must specify one values for each marker";
    }

    for (int i = 0; i < points.size(); i++) {
        points[i]->setValueX(x[i]);
        points[i]->setValueY(y[i]);
    }
}

ChartPointSeries::~ChartPointSeries() {
    
}

void ChartPointSeries::draw() {
    if (display) {
        calculatePointLocations();

        if (displayAsArea) {
            drawAsArea();
        } else {
            drawAsLines();
        }
    }
}

void ChartPointSeries::drawLabels() {
    FOREACH_POINT(it, points) {
        (*it)->drawLabel();
    }
}

void ChartPointSeries::drawToPick() {
    if (display) {
        calculatePointLocations();

        FOREACH_POINT(it, points) {
            (*it)->drawToPick();
        } 
    }
}

void ChartPointSeries::calculatePointLocations() {
    FOREACH_POINT(it, points) {
        (*it)->calculateLocation(); 
    } 
}

float ChartPointSeries::drawInLegend(float x, float y, float lineLength, float spacing, void *font) {
    float h = PrintText::printingHeight(font);

    glPushMatrix();
        glTranslatef(x, y, 0);
        glTranslatef(lineLength / 2.0 + spacing, h / 2, 0);

        glLineWidth(lineWidth);
        glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
        glBegin(GL_LINES);
            glVertex2f(-lineLength / 2.0, 0);
            glVertex2f(lineLength / 2.0, 0);
        glEnd();

        if (displayMarkers) {
            legendPoint->setPositionX(0);
            legendPoint->setPositionY(0);

            legendPoint->draw();
        }

        int posX = lineLength / 2.0 + spacing;
        int posY = 0;

        glColor4f(0, 0, 0, 1);
        PrintText::printVerticallyCenteredAt(posX, posY, label, false, false, font);
    glPopMatrix();

    return PrintText::printingWidth(label, font) + 3 * spacing + lineLength;
}

void ChartPointSeries::drawAsLines() {
    ChartPoint *last = NULL;
   
    glPolygonMode(GL_FRONT, GL_LINE);
    glLineWidth(lineWidth);
    glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
    
    glBegin(GL_LINES);
        FOREACH_POINT(it, points) {      
            if (last != NULL) {
                // draw line to last
                glVertex2f(last->getPositionX(), last->getPositionY());
                glVertex2f((*it)->getPositionX(), (*it)->getPositionY());                
            }

            last = *it;
        }
    glEnd();

    if (displayMarkers) {
        FOREACH_POINT(it, points) {
            (*it)->draw();
        }            
    }
}

void ChartPointSeries::drawAsArea() {
    ChartPoint *last = NULL;

    glPolygonMode( GL_FRONT, GL_FILL );
    glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
    
    FOREACH_POINT(it, points) {   
        if (last != NULL) {
            glBegin( GL_POLYGON );
                glVertex2f(last->getPositionX(), 0);
                glVertex2f(last->getPositionX(), last->getPositionY());
                glVertex2f((*it)->getPositionX(), (*it)->getPositionY()); 
                glVertex2f((*it)->getPositionX(), 0);                 
            glEnd();
        }
        
        last = *it;
    }
}

float ChartPointSeries::getMinimumValueX() {
    return points[0]->getValueX();
}

float ChartPointSeries::getMaximumValueX() {
    return points[points.size() - 1]->getValueX();
}

float ChartPointSeries::getMinimumValueY() {
    return min->getValueY();
}

float ChartPointSeries::getMaximumValueY() {
    return max->getValueY();
}

void ChartPointSeries::setMarkerShape(int shape) {
    FOREACH_POINT(it, points) {
        (*it)->setShape(shape);
    }

    legendPoint->setShape(shape);
}

void ChartPointSeries::setMarkerSize(float size) {
    FOREACH_POINT(it, points) {
        (*it)->setSize(size);
    }

    legendPoint->setSize(size);
}

void ChartPointSeries::setMarkerBorderColor(Color *color) {
    FOREACH_POINT(it, points) {
        (*it)->setBorderColor(color);
    }

    legendPoint->setBorderColor(color);
}

void ChartPointSeries::setMarkerFillColor(Color *color) {
    FOREACH_POINT(it, points) {
        (*it)->setFillColor(color);
    }

    legendPoint->setFillColor(color);
}

void ChartPointSeries::setColor(Color *c) {
    setLineColor(c);
    setMarkerBorderColor(c);
    setMarkerFillColor(c);
}
