#include "ChartPointSeries.h"
#include "ChartPoint.h"
#include "Point.h"
#include "Circle.h"
#include "Color.h"
#include "LineChart.h"
#include "PrintText.h"
#include "LineChart.h"
#include <GL/glut.h>

#define NUM_RECTS 75
#define START_ALPHA 0.45
#define END_ALPHA 0.05
#define D_ALPHA (START_ALPHA - END_ALPHA) / NUM_RECTS

ChartPointSeries::ChartPointSeries(LineChart *chart, std::string label, std::vector<float> *x, std::vector<float> *y) {
    marker = new Circle();
    marker->setBorderWidth(0);
    marker->setSize(8, 8);
    
    this->chart = chart;
    this->label = label;
    display = true;
    displayMarkers = true;
    displayAsArea = false;

    if (x->size() != y->size()) {
        throw "should be equal number of x and y values";
    }

    if (x->empty() || y->empty()) {
        throw "should not be empty values for x or y";
    }

    size = x->size();
    points = new ChartPoint *[size];

    max = NULL;
    min = NULL;

    for (unsigned int i = 0; i < size; i++) {
        points[i] = new ChartPoint(chart, label, x->at(i), y->at(i));
        
        if (max == NULL || max->getY() < points[i]->getY()) {
            max = points[i];
        }
        if (min == NULL || min->getY() > points[i]->getY()) {
            min = points[i];
        }

        points[i]->setMarker(marker);
    }
    
    previousMin = min;
    previousMax = max;

    lineColor = new Color(Color::black);
    lineWidth = 1.0;
}

ChartPointSeries::~ChartPointSeries() {
    for (int i = 0; i < size; i++) {
        delete points[i];
    }

    delete[] points;
    delete marker;

    delete lineColor;
}

void ChartPointSeries::setValues(std::vector<float> *x, std::vector<float> *y) {
    if (x->size() != y->size()) {
        throw "should be equal number of x and y values";
    }

    if (x->empty() || y->empty()) {
        throw "should not be empty values for x or y";
    }   

    if (size != y->size()) {
        throw "to change marker values you must specify one value for each marker";
    }

    previousMin = NULL;
    previousMax = NULL;

    max = NULL;
    min = NULL;

    for (unsigned int i = 0; i < size; i++) {
        points[i]->setX(x->at(i));
        points[i]->setY(y->at(i));

        if (max == NULL || max->getY() < points[i]->getY()) { 
            max = points[i];
        }

        if (min == NULL || min->getY() > points[i]->getY()) {
            min = points[i];
        }

        if (previousMax == NULL || previousMax->getY() < points[i]->getPrevious()->getY()) { 
            previousMax = points[i]->getPrevious();
        }

        if (previousMin == NULL || previousMin->getY() > points[i]->getPrevious()->getY()) {
            previousMin = points[i]->getPrevious();
        }
    }
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

void ChartPointSeries::drawSelected() {
    for (int p = 0; p < size; p++) {
        points[p]->drawSelected();
    }
}

void ChartPointSeries::drawToPick() {
    if (display) {
        calculatePointLocations();

        for (int p = 0; p < size; p++) {
            points[p]->drawToPick();
        } 
    }
}

void ChartPointSeries::calculatePointLocations() {
    for (int p = 0; p < size; p++) {
        points[p]->calculateLocation(); 
    } 
}

float ChartPointSeries::drawInLegend(float x, float y, float lineLength, float spacing, float h) {
    glPushMatrix();
        glTranslatef(x, y, 0);
        glTranslatef(lineLength / 2.0 + spacing, h / 2, 0);

        glEnable(GL_LINE_SMOOTH);
        glLineWidth(lineWidth);
        glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
        glBegin(GL_LINES);
            glVertex2f(-lineLength / 2.0, 0);
            glVertex2f(lineLength / 2.0, 0);
        glEnd();
        glLineWidth(1);
        glDisable(GL_LINE_SMOOTH);

        if (displayMarkers) {
            marker->setX(0);
            marker->setY(0);

            marker->draw();
        }

        int posX = lineLength / 2.0 + spacing;
        int posY = 0;

        glColor4f(0, 0, 0, 1);
        PrintText::drawStrokeText(label, posX, posY, h, HORIZ_LEFT, VERT_CENTER);
    glPopMatrix();

    return PrintText::strokeWidth(label, h) + 3 * spacing + lineLength;
}

void ChartPointSeries::drawAsLines() {
    glEnable(GL_LINE_SMOOTH);
    glPolygonMode(GL_FRONT, GL_LINE);
    glLineWidth(lineWidth);
    glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
    
    glBegin(GL_LINE_STRIP);
        for (int p = 0; p < size; p++) {      
            glVertex2f(points[p]->getPositionX(), points[p]->getPositionY());
        }
    glEnd();    
    glLineWidth(1);
    glDisable(GL_LINE_SMOOTH);

    if (displayMarkers) {
        for (int p = 0; p < size; p++) { 
            points[p]->draw();
        }            
    }
}

void ChartPointSeries::drawAsArea() {
    glPolygonMode( GL_FRONT, GL_FILL );
    glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
    
    glBegin( GL_QUAD_STRIP );

    for (int p = 0; p < size; p++) {  
        glVertex2f(points[p]->getPositionX(), points[p]->getPositionY()); 
        glVertex2f(points[p]->getPositionX(), 0); 
    }

    glEnd();
}

void ChartPointSeries::drawGhostAsLine() {   
    glPolygonMode(GL_FRONT, GL_LINE);
    glLineWidth(lineWidth);
    glColor4f(0.5, 0.5, 0.5, 1);
    //glColor4f(lineColor->r, lineColor->g, lineColor->b, 1);

    glPushAttrib(GL_ENABLE_BIT); 
    glLineStipple(1, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    
    glBegin(GL_LINE_STRIP);
        for (int p = 0; p < size; p++) {
            glVertex2f(points[p]->getPreviousPositionX(), points[p]->getPreviousPositionY());    
        }
    glEnd();    
    glLineWidth(1);

    glDisable(GL_LINE_STIPPLE);
    glPopAttrib();
}

void ChartPointSeries::drawGhostAsBlend() {
    glPolygonMode( GL_FRONT, GL_FILL );
    
    ChartPoint *last = NULL;
    for (int p = 0; p < size; p++) {
        if (last != NULL) {
            float leftX = last->getPositionX();
            float rightX = points[p]->getPositionX();

            float startLeftY = last->getPositionY();
            float startRightY = points[p]->getPositionY();
            float endRightY = points[p]->getPreviousPositionY();
            float endLeftY = last->getPreviousPositionY();

            float rightDiffY = endRightY - startRightY;
            float leftDiffY = endLeftY - startLeftY;

            float rightY = rightDiffY / (NUM_RECTS - 1);
            float leftY = leftDiffY / (NUM_RECTS - 1);

            glBegin( GL_QUAD_STRIP );

            for (int i = 0; i < NUM_RECTS; i++) {
                glColor4f(lineColor->r, lineColor->g, lineColor->b, START_ALPHA - i * D_ALPHA);

                glVertex2f(rightX, startRightY + i * rightY);   
                glVertex2f(leftX, startLeftY + i * leftY); 
            }

            glEnd();
        }
        
        last = points[p];
    }

    // more efficient code but draws weirdly?
    /*glBegin( GL_QUAD_STRIP );
    FOREACH_POINTP(it, points) {          
        float x = (*it)->getPositionX();
        float currentY = (*it)->getPositionY();
        float previousY = (*it)->getPreviousPositionY();

        glColor4f(lineColor->r, lineColor->g, lineColor->b, END_ALPHA);
        glVertex2f(x, previousY);

        glColor4f(lineColor->r, lineColor->g, lineColor->b, START_ALPHA);
        glVertex2f(x, currentY);        
    }
    glEnd();*/
}

float ChartPointSeries::getMinimumValueX() {
    return points[0]->getX();
}

float ChartPointSeries::getMaximumValueX() {
    return points[size - 1]->getX();
}

float ChartPointSeries::getMinimumValueY() {
    return min->getY();
}

float ChartPointSeries::getMaximumValueY() {
    return max->getY();
}

float ChartPointSeries::getPreviousMinimumValueY() {
    return previousMin->getY();
}

float ChartPointSeries::getPreviousMaximumValueY() {
    return previousMax->getY();
}

void ChartPointSeries::setMarkerSize(float mySize) {
    marker->setSize(mySize, mySize);
}

void ChartPointSeries::setMarkerBorderColor(Color *color) {
    marker->setBorderColor(color);
}

void ChartPointSeries::setMarkerFillColor(Color *color) {
    marker->setFillColor(color);
}

void ChartPointSeries::setColor(Color *c) {
    getLineColor()->copyValues(c);
    marker->getBorderColor()->copyValues(c);
    marker->getFillColor()->copyValues(c);
}

Color *ChartPointSeries::getColor() {
    return lineColor;
}

void ChartPointSeries::capturePreviousValues() {
    //lastMin = min;
    //lastMax = max;

    for (int i = 0; i < size; i++) {
        points[i]->capturePreviousValues();
    }
}

int ChartPointSeries::getSize() {
    return size;
}

float ChartPointSeries::getPercentIncreaseOfFinalValue() {
    return points[size - 1]->getPercentIncreaseFromPrevious();
}

float ChartPointSeries::getFinalValue() {
    return points[size - 1]->getY();
}

float ChartPointSeries::getPreviousFinalValue() {
    return points[size - 1]->getPreviousValue();
}

float ChartPointSeries::getStartValue() {
    return points[0]->getY();
}