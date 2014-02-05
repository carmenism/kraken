#include "ChartPointSeries.h"
#include "ChartPoint.h"
#include "Point.h"
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
    this->chart = chart;
    this->label = label;
    
    
    if (x->size() != y->size()) {
        throw "should be equal number of x and y values";
    }

    if (x->empty() || y->empty()) {
        throw "should not be empty values for x or y";
    }

    numberPoints = x->size();
    points = new ChartPoint*[numberPoints];

    max = NULL;
    min = NULL;

    for (int i = 0; i < x->size(); i++) {
        ChartPoint *point = new ChartPoint(chart, label, x->at(i), y->at(i));
        points[i] = point;

        if (max == NULL || max->getY() < point->getY()) {
            max = point;
        }
        if (min == NULL || min->getY() > point->getY()) {
            min = point;
        }
    }
    
    previousMin = min;
    previousMax = max;

    lineColor = &Color::black;
    lineWidth = 1.0;

    displayAsArea = false;

    legendPoint = new ChartPoint(chart, "", -1, -1);
}

ChartPointSeries::~ChartPointSeries() {
    delete[] points;
    delete legendPoint;
    delete lineColor;
}

void ChartPointSeries::setValues(std::vector<float> *x, std::vector<float> *y) {
    if (x->size() != y->size()) {
        throw "should be equal number of x and y values";
    }

    if (x->empty() || y->empty()) {
        throw "should not be empty values for x or y";
    }   

    if (points->size() != y->size()) {
        throw "to change marker values you must specify one values for each marker";
    }

    previousMin = NULL;
    previousMax = NULL;

    max = NULL;
    min = NULL;

    for (int i = 0; i < points->size(); i++) {
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
    for (int i = 0; i < numberPoints; i++) {
        points[i]->drawSelected();
    }
}

void ChartPointSeries::drawToPick() {
    if (display) {
        calculatePointLocations();

        for (int i = 0; i < numberPoints; i++) {
            points[i]->drawToPick();
        } 
    }
}

void ChartPointSeries::calculatePointLocations() {
    for (int i = 0; i < numberPoints; i++) {
        points[i]->calculateLocation(); 
    } 
}

float ChartPointSeries::drawInLegend(float x, float y, float lineLength, float spacing, float h) {
    glPushMatrix();
        glTranslatef(x, y, 0);
        glTranslatef(lineLength / 2.0 + spacing, h / 2, 0);

        glLineWidth(lineWidth);
        glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
        glBegin(GL_LINES);
            glVertex2f(-lineLength / 2.0, 0);
            glVertex2f(lineLength / 2.0, 0);
        glEnd();
        glLineWidth(1);

        if (displayMarkers) {
            legendPoint->setPositionX(0);
            legendPoint->setPositionY(0);

            legendPoint->draw();
        }

        int posX = lineLength / 2.0 + spacing;
        int posY = 0;

        glColor4f(0, 0, 0, 1);
        PrintText::drawStrokeText(label, posX, posY, h, HORIZ_LEFT, VERT_CENTER);
    glPopMatrix();

    return PrintText::strokeWidth(label, h) + 3 * spacing + lineLength;
}

void ChartPointSeries::drawAsLines() {   
    glDisable(GL_LINE_SMOOTH);
    glPolygonMode(GL_FRONT, GL_LINE);
    glLineWidth(lineWidth);
    glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
    
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i < numberPoints; i++) {    
            glVertex2f(points[i]->getPositionX(), points[i]->getPositionY());
        }
    glEnd();    
    glLineWidth(1);
    glEnable(GL_LINE_SMOOTH);

    if (displayMarkers) {
        for (int i = 0; i < numberPoints; i++) {
            points[i]->draw();
        }            
    }
}

void ChartPointSeries::drawAsArea() {
    glPolygonMode( GL_FRONT, GL_FILL );
    glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
    
    glBegin( GL_QUAD_STRIP );

    for (int i = 0; i < numberPoints; i++) { 
        glVertex2f(points[i]->getPositionX(), points[i]->getPositionY()); 
        glVertex2f(points[i]->getPositionX(), 0); 
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
        for (int i = 0; i < numberPoints; i++) {
            glVertex2f(points[i]->getPreviousPositionX(), points[i]->getPreviousPositionY());    
        }
    glEnd();    
    glLineWidth(1);

    glDisable(GL_LINE_STIPPLE);
    glPopAttrib();
}

void ChartPointSeries::drawGhostAsBlend() {
    glPolygonMode( GL_FRONT, GL_FILL );
    
    ChartPoint *last = NULL;
    for (int p = 0; p < numberPoints; p++) {  
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

            for (int r = 0; r < NUM_RECTS; r++) {
                glColor4f(lineColor->r, lineColor->g, lineColor->b, START_ALPHA - r * D_ALPHA);

                glVertex2f(rightX, startRightY + r * rightY);   
                glVertex2f(leftX, startLeftY + r * leftY); 
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
    return points->front()->getX();
}

float ChartPointSeries::getMaximumValueX() {
    return points->back()->getX();
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

void ChartPointSeries::setMarkerShape(int shape) {
    for (int i = 0; i < numberPoints; i++) {
        points[i]->setShape(shape);
    }

    legendPoint->setShape(shape);
}

void ChartPointSeries::setMarkerSize(float size) {
    for (int i = 0; i < numberPoints; i++) {
        points[i]->setSize(size);
    }

    legendPoint->setSize(size);
}

void ChartPointSeries::setMarkerBorderColor(Color *color) {
    for (int i = 0; i < numberPoints; i++) {
        points[i]->setBorderColor(color);
    }

    legendPoint->setBorderColor(color);
}

void ChartPointSeries::setMarkerFillColor(Color *color) {
    for (int i = 0; i < numberPoints; i++) {
        points[i]->setFillColor(color);
    }

    legendPoint->setFillColor(color);
}

void ChartPointSeries::setColor(Color *c) {
    setLineColor(c);
    setMarkerBorderColor(c);
    setMarkerFillColor(c);
}

Color *ChartPointSeries::getColor() {
    return lineColor;
}

void ChartPointSeries::capturePreviousValues() {
    //lastMin = min;
    //lastMax = max;

    for (int i = 0; i < numberPoints; i++) {
        points[i]->capturePreviousValues();
    }
}

int ChartPointSeries::size() {
    return points->size();
}

float ChartPointSeries::getPercentIncreaseOfFinalValue() {
    return points[numberPoints - 1]->getPercentIncreaseFromPrevious();
}

float ChartPointSeries::getFinalValue() {
    return points[numberPoints - 1]->getY();
}

float ChartPointSeries::getPreviousFinalValue() {
    return points[numberPoints - 1]->getPreviousValue();
}

float ChartPointSeries::getStartValue() {
    return points[0]->getY();
}