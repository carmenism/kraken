#include "GraphMarkerSeries.h"
#include "GraphMarker.h"
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
        GraphMarker *marker = new GraphMarker(label, x[i], y[i]);
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

void GraphMarkerSeries::setValues(std::vector<float> x, std::vector<float> y) {
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

GraphMarkerSeries::~GraphMarkerSeries() {
    
}

void GraphMarkerSeries::draw(float graphWidth, float graphHeight,
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

void GraphMarkerSeries::drawLabels() {
    FOREACH_MARKER(it, markers) {
        (*it)->drawLabel();
    }
}

void GraphMarkerSeries::drawToPick(float graphWidth, float graphHeight,
                                   float maxValueX,  float maxValueY) {
    if (display) {
        recalculateMarkerLocations(graphWidth, graphHeight, maxValueX, maxValueY);

        FOREACH_MARKER(it, markers) {
            (*it)->drawToPick();
        } 
    }
}

void GraphMarkerSeries::recalculateMarkerLocations(float graphWidth, 
        float graphHeight, float maxValueX, float maxValueY) {
    FOREACH_MARKER(it, markers) {
        (*it)->calculateLocation(graphWidth, graphHeight, maxValueX, maxValueY); 
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

void GraphMarkerSeries::drawAsLines() {
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

void GraphMarkerSeries::drawAsArea() {
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

float GraphMarkerSeries::getMinimumValueX() {
    return markers[0]->getValueX();
}

float GraphMarkerSeries::getMaximumValueX() {
    return markers[markers.size() - 1]->getValueX();
}

float GraphMarkerSeries::getMinimumValueY() {
    return min->getValueY();
}

float GraphMarkerSeries::getMaximumValueY() {
    return max->getValueY();
}

void GraphMarkerSeries::setMarkerShape(int shape) {
    FOREACH_MARKER(it, markers) {
        (*it)->setShape(shape);
    }
}

void GraphMarkerSeries::setMarkerSize(float size) {
    FOREACH_MARKER(it, markers) {
        (*it)->setSize(size);
    }
}

void GraphMarkerSeries::setMarkerBorderColor(Color *color) {
    FOREACH_MARKER(it, markers) {
        (*it)->setBorderColor(color);
    }
}

void GraphMarkerSeries::setMarkerFillColor(Color *color) {
    FOREACH_MARKER(it, markers) {
        (*it)->setFillColor(color);
    }
}

void GraphMarkerSeries::setColor(Color *c) {
    setLineColor(c);
    setMarkerBorderColor(c);
    setMarkerFillColor(c);
}

