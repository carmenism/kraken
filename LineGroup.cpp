#include "LineGroup.h"
#include "GraphMarker.h"
#include "Color.h"
#include <QtOpenGL>


LineGroup::LineGroup(std::string label, std::vector<float> x, std::vector<float> y) {
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
        GraphMarker *marker = new GraphMarker(x[i], y[i]);
        markers.push_back(marker);

        if (max == NULL || max->getValueY() < marker->getValueY()) {
            max = marker;
        }
        if (min == NULL || min->getValueY() > marker->getValueY()) {
            min = marker;
        }
    }

    lineColor = &Color::black;
    lineWidth = 1.0;

    displayAsArea = false;
}

LineGroup::~LineGroup() {
    
}

void LineGroup::draw(float graphWidth, float graphHeight,
                     float maxValueX,  float maxValueY) {
    if (display) {
        recalculateMarkerLocations(graphWidth, graphHeight, maxValueX, maxValueY);

        if (displayAsArea) {
            drawAsArea();
        } else {
            drawAsLines();
        }
    }
}

void LineGroup::drawLabels() {
    FOREACH_MARKER(it, markers) {
        (*it)->drawLabel();
    }
}

void LineGroup::drawToPick(float graphWidth, float graphHeight,
                           float maxValueX,  float maxValueY) {
    if (display) {
        recalculateMarkerLocations(graphWidth, graphHeight, maxValueX, maxValueY);

        FOREACH_MARKER(it, markers) {
            (*it)->drawToPick();
        } 
    }
}

void LineGroup::recalculateMarkerLocations(float graphWidth, 
        float graphHeight, float maxValueX, float maxValueY) {
    FOREACH_MARKER(it, markers) {
        (*it)->calculateLocation(graphWidth, graphHeight, maxValueX, maxValueY); 
    } 
}

void LineGroup::drawAsLines() {
    GraphMarker *last = NULL;
   
    glPolygonMode(GL_FRONT, GL_LINE);
    glLineWidth(lineWidth);
    glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
    
    glBegin(GL_LINES);
        FOREACH_MARKER(it, markers) {      
            if (last != NULL) {
                // draw line to last
                glVertex2f(last->getPositionX(), last->getPositionY());
                glVertex2f((*it)->getPositionX(), (*it)->getPositionY());                
            }

            last = *it;
        }
    glEnd();

    if (displayMarkers) {
        FOREACH_MARKER(it, markers) {
            (*it)->draw();
        }            
    }
}

void LineGroup::drawAsArea() {
    GraphMarker *last = NULL;

    glPolygonMode( GL_FRONT, GL_FILL );
    glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
    
    FOREACH_MARKER(it, markers) {   
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

float LineGroup::getMinimumValueX() {
    return markers[0]->getValueX();
}

float LineGroup::getMaximumValueX() {
    return markers[markers.size() - 1]->getValueX();
}

float LineGroup::getMinimumValueY() {
    return min->getValueY();
}

float LineGroup::getMaximumValueY() {
    return max->getValueY();
}

void LineGroup::setMarkerShape(int shape) {
    GraphMarkerIterator it;

    FOREACH_MARKER(it, markers) {
        (*it)->setShape(shape);
    }
}

void LineGroup::setMarkerSize(float size) {
    GraphMarkerIterator it;

    FOREACH_MARKER(it, markers) {
        (*it)->setSize(size);
    }
}

void LineGroup::setMarkerBorderColor(Color *color) {
    GraphMarkerIterator it;

    FOREACH_MARKER(it, markers) {
        (*it)->setBorderColor(color);
    }
}

void LineGroup::setMarkerFillColor(Color *color) {
    GraphMarkerIterator it;

    FOREACH_MARKER(it, markers) {
        (*it)->setFillColor(color);
    }
}

void LineGroup::setColor(Color *c) {
    setLineColor(c);
    setMarkerBorderColor(c);
    setMarkerFillColor(c);
}

