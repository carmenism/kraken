#include "LineGroup.h"
#include "GraphMarker.h"
#include <QtOpenGL>


LineGroup::LineGroup(std::string label, std::vector<float> x, std::vector<float> y) {
    this->label = label;
    
    if (x.size() != y.size()) {
        throw "should be equal number of x and y values";
    }

    if (x.empty() || y.empty()) {
        throw "should not be empty values for x or y";
    }

    std::vector<float>::const_iterator ix;
    std::vector<float>::const_iterator iy;

    max = NULL;
    min = NULL;

    for( ix = x.begin(), iy = y.begin();
            ix != x.end() && iy != y.end();
            ++ix, ++iy ){
        GraphMarker *marker = new GraphMarker(*ix, *iy);
        markers.push_back(marker);

        if (max == NULL || max->getValueY() < marker->getValueY()) {
            max = marker;
        }
        if (min == NULL || min->getValueY() > marker->getValueY()) {
            min = marker;
        }
    }
}

LineGroup::~LineGroup() {
    
}

void LineGroup::redraw(float graphWidth, float graphHeight,
                       float maxValueX,  float maxValueY) {
    if (display) {
        GraphMarkerIterator it;
        GraphMarker *last = NULL;

        FOREACH_MARKER(it, markers) {
            if (displayMarkers) {
                (*it)->redraw(graphWidth, graphHeight, maxValueX, maxValueY);
            }

            if (last != NULL) {
                // draw line to last
                glBegin(GL_LINES);
                    glVertex2f(last->getPositionX(), last->getPositionY());
                    glVertex2f((*it)->getPositionX(), (*it)->getPositionY());
                glEnd();
            }

            last = *it;
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

void LineGroup::setShape(int shape) {
    GraphMarkerIterator it;

    FOREACH_MARKER(it, markers) {
        (*it)->setShape(shape);
    }
}

void LineGroup::setSize(float size) {
    GraphMarkerIterator it;

    FOREACH_MARKER(it, markers) {
        (*it)->setSize(size);
    }
}