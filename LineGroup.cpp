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

    //std::vector<float>::const_iterator ix;
    //std::vector<float>::const_iterator iy;

    max = NULL;
    min = NULL;

    //for( ix = x.begin(), iy = y.begin();
    //        ix != x.end() && iy != y.end();
    //        ++ix, ++iy ){
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
}

LineGroup::~LineGroup() {
    
}

void LineGroup::redraw(float graphWidth, float graphHeight,
                       float maxValueX,  float maxValueY) {
    if (display) {
        GraphMarkerIterator it;
        GraphMarker *last = NULL;

        glColor3f(lineColor->r, lineColor->g, lineColor->b);
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
                (*it)->redraw(graphWidth, graphHeight, maxValueX, maxValueY);
            }            
        }
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