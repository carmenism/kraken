#include "GraphMarkerSeries.h"
#include "ChartPoint.h"
#include "Color.h"
#include "PrintText.h"
#include <QtOpenGL>


GraphMarkerSeries::GraphMarkerSeries(std::string label, std::vector<float> x, std::vector<float> y) {
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
        ChartPoint *point = new ChartPoint(label, x[i], y[i]);
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
}

void GraphMarkerSeries::setValues(std::vector<float> x, std::vector<float> y) {
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

GraphMarkerSeries::~GraphMarkerSeries() {
    
}

void GraphMarkerSeries::draw(float chartWidth, float chartHeight,
                             float maxValueX,  float maxValueY) {
    if (display) {
        calculatePointLocations(chartWidth, chartHeight, maxValueX, maxValueY);

        if (displayAsArea) {
            drawAsArea();
        } else {
            drawAsLines();
        }
    }
}

void GraphMarkerSeries::drawLabels() {
    FOREACH_POINT(it, points) {
        (*it)->drawLabel();
    }
}

void GraphMarkerSeries::drawToPick(float chartWidth, float chartHeight,
                                   float maxValueX,  float maxValueY) {
    if (display) {
        calculatePointLocations(chartWidth, chartHeight, maxValueX, maxValueY);

        FOREACH_POINT(it, points) {
            (*it)->drawToPick();
        } 
    }
}

void GraphMarkerSeries::calculatePointLocations(float chartWidth, 
        float chartHeight, float maxValueX, float maxValueY) {
    FOREACH_POINT(it, points) {
        (*it)->calculateLocation(chartWidth, chartHeight, maxValueX, maxValueY); 
    } 
}

float GraphMarkerSeries::drawInLegend(float x, float y, float lineLength, float spacing, void *font) {
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
            float origX = points[0]->getPositionX();
            float origY = points[0]->getPositionY();

            points[0]->setPositionX(0);
            points[0]->setPositionY(0);

            points[0]->draw();

            points[0]->setPositionX(origX);
            points[0]->setPositionY(origY);
        }

        int posX = lineLength / 2.0 + spacing;
        int posY = 0;

        glColor4f(0, 0, 0, 1);
        PrintText::printVerticallyCenteredAt(posX, posY, label, false, false, font);
    glPopMatrix();

    return PrintText::printingWidth(label, font) + 3 * spacing + lineLength;
}

void GraphMarkerSeries::drawAsLines() {
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

void GraphMarkerSeries::drawAsArea() {
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

float GraphMarkerSeries::getMinimumValueX() {
    return points[0]->getValueX();
}

float GraphMarkerSeries::getMaximumValueX() {
    return points[points.size() - 1]->getValueX();
}

float GraphMarkerSeries::getMinimumValueY() {
    return min->getValueY();
}

float GraphMarkerSeries::getMaximumValueY() {
    return max->getValueY();
}

void GraphMarkerSeries::setMarkerShape(int shape) {
    FOREACH_POINT(it, points) {
        (*it)->setShape(shape);
    }
}

void GraphMarkerSeries::setMarkerSize(float size) {
    FOREACH_POINT(it, points) {
        (*it)->setSize(size);
    }
}

void GraphMarkerSeries::setMarkerBorderColor(Color *color) {
    FOREACH_POINT(it, points) {
        (*it)->setBorderColor(color);
    }
}

void GraphMarkerSeries::setMarkerFillColor(Color *color) {
    FOREACH_POINT(it, points) {
        (*it)->setFillColor(color);
    }
}

void GraphMarkerSeries::setColor(Color *c) {
    setLineColor(c);
    setMarkerBorderColor(c);
    setMarkerFillColor(c);
}

