#include "ChartPointSeries.h"
#include "ChartPoint.h"
#include "Color.h"
#include "LineChart.h"
#include "PrintText.h"
#include "LineChart.h"
#include <QtOpenGL>

#define NUM_RECTS 75
#define START_ALPHA 0.45
#define END_ALPHA 0.05
#define D_ALPHA (START_ALPHA - END_ALPHA) / NUM_RECTS

ChartPointSeries::ChartPointSeries(LineChart *chart, std::string label, std::vector<float> x, std::vector<float> y) {
    this->chart = chart;
    this->label = label;
    
    points = new ChartPointList();

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
        points->push_back(point);

        if (max == NULL || max->getY() < point->getY()) {
            max = point;
        }
        if (min == NULL || min->getY() > point->getY()) {
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

    if (points->size() != y.size()) {
        throw "to change marker values you must specify one values for each marker";
    }

    max = NULL;
    min = NULL;

    for (int i = 0; i < points->size(); i++) {
        points->at(i)->setX(x[i]);
        points->at(i)->setY(y[i]);

        if (max == NULL || max->getY() < points->at(i)->getY()) { 
            //|| max->getLastValueY() < points[i]->getLastValueY()) {
            max = points->at(i);
        }

        if (min == NULL || min->getY() > points->at(i)->getY()) {
            //|| min->getLastValueY() > points[i]->getLastValueY()) {
            min = points->at(i);
        }
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

void ChartPointSeries::drawSelected() {
    FOREACH_POINTP(it, points) {
        (*it)->drawSelected();
    }
}

void ChartPointSeries::drawToPick() {
    if (display) {
        calculatePointLocations();

        FOREACH_POINTP(it, points) {
            (*it)->drawToPick();
        } 
    }
}

void ChartPointSeries::calculatePointLocations() {
    FOREACH_POINTP(it, points) {
        (*it)->calculateLocation(); 
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
    ChartPoint *last = NULL;
   
    glPolygonMode(GL_FRONT, GL_LINE);
    glLineWidth(lineWidth);
    glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
    
    glBegin(GL_LINES);
        FOREACH_POINTP(it, points) {      
            if (last != NULL) {
                // draw line to last
                glVertex2f(last->getPositionX(), last->getPositionY());
                glVertex2f((*it)->getPositionX(), (*it)->getPositionY());                
            }

            last = *it;
        }
    glEnd();    
    glLineWidth(1);

    if (displayMarkers) {
        FOREACH_POINTP(it, points) {
            (*it)->draw();
        }            
    }
}

void ChartPointSeries::drawAsArea() {
    ChartPoint *last = NULL;

    glPolygonMode( GL_FRONT, GL_FILL );
    glColor4f(lineColor->r, lineColor->g, lineColor->b, lineColor->a);
    
    FOREACH_POINTP(it, points) {   
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

void ChartPointSeries::drawGhost() {
    glEnable(GL_SCISSOR_TEST);
    float startX = chart->getOffsetX() + chart->getXLocation();
    float startY = chart->getOffsetY() + chart->getYLocation();
    float w = chart->getActualWidth();
    float h = chart->getActualHeight();

    glScissor(startX, startY, w, h);

    ChartPoint *last = NULL;

    glPolygonMode( GL_FRONT, GL_FILL );
    //glBegin( GL_POLYGON );
    FOREACH_POINTP(it, points) {   
        if (last != NULL) {
            float leftX = last->getPositionX();
            float rightX = (*it)->getPositionX();

            float startLeftY = last->getPositionY();
            float startRightY = (*it)->getPositionY();
            float endRightY = (*it)->getLastPositionY();
            float endLeftY = last->getLastPositionY();

            float rightDiffY = endRightY - startRightY;
            float leftDiffY = endLeftY - startLeftY;

            float rightY = rightDiffY / NUM_RECTS;
            float leftY = leftDiffY / NUM_RECTS;

            for (int i = 0; i < NUM_RECTS; i++) {
                glColor4f(lineColor->r, lineColor->g, lineColor->b, START_ALPHA - i * D_ALPHA);

                glBegin( GL_POLYGON );
                    glVertex2f(leftX, startLeftY + i * leftY); // top
                    glVertex2f(rightX, startRightY + i * rightY);
                    glVertex2f(rightX, startRightY + (i+1) * rightY);//bottom   
                    glVertex2f(leftX, startLeftY + (i+1) * leftY); 
                glEnd();
            }
        }
        
        last = *it;
    }
    //glEnd( );
    
    glDisable(GL_SCISSOR_TEST);
}

float ChartPointSeries::getMinimumValueX() {
    return points->front()->getX();
}

float ChartPointSeries::getMaximumValueX() {
    return points->back()->getX();
}

float ChartPointSeries::getMinimumValueY() {
    return min->getY();//min(min->getValueY(), min->getLastValueY());
}

float ChartPointSeries::getMaximumValueY() {
    return max->getY();//max(max->getValueY(), max->getLastValueY());
}

void ChartPointSeries::setMarkerShape(int shape) {
    FOREACH_POINTP(it, points) {
        (*it)->setShape(shape);
    }

    legendPoint->setShape(shape);
}

void ChartPointSeries::setMarkerSize(float size) {
    FOREACH_POINTP(it, points) {
        (*it)->setSize(size);
    }

    legendPoint->setSize(size);
}

void ChartPointSeries::setMarkerBorderColor(Color *color) {
    FOREACH_POINTP(it, points) {
        (*it)->setBorderColor(color);
    }

    legendPoint->setBorderColor(color);
}

void ChartPointSeries::setMarkerFillColor(Color *color) {
    FOREACH_POINTP(it, points) {
        (*it)->setFillColor(color);
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

void ChartPointSeries::captureLastValues() {
    FOREACH_POINTP(it, points) {
        (*it)->captureLastValues();
    }
}

