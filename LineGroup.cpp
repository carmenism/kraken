#include "LineGroup.h"
#include "GraphMarker.h"
#include "Color.h"
#include "PrintText.h"
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

void LineGroup::setValues(std::vector<float> x, std::vector<float> y) {
    if (x.size() != y.size()) {
        throw "should be equal number of x and y values";
    }

    if (x.empty() || y.empty()) {
        throw "should not be empty values for x or y";
    }   

    if (markers.size() != y.size()) {
        throw "to change marker values you must specify one values for each marker";
    }

    for (int i = 0; i < markers.size(); i++) {
        markers[i]->setValueX(x[i]);
        markers[i]->setValueY(y[i]);
    }
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

float LineGroup::drawInLegend(float x, float y, float lineLength, float spacing, void *font) {
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
            float origX = markers[0]->getPositionX();
            float origY = markers[0]->getPositionY();

            markers[0]->setPositionX(0);
            markers[0]->setPositionY(0);

            markers[0]->draw();

            markers[0]->setPositionX(origX);
            markers[0]->setPositionY(origY);
        }

        int posX = lineLength / 2.0 + spacing;
        int posY = 0;

        glColor4f(0, 0, 0, 1);
        PrintText::printVerticallyCenteredAt(posX, posY, label, false, false, font);
    glPopMatrix();

    return PrintText::printingWidth(label, font) + 3 * spacing + lineLength;
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

