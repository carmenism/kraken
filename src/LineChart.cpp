#include "LineChart.h"
#include "ChartPoint.h"
#include "LineChartAxis.h"
#include "LineChartLegend.h"
#include "PrintText.h"
#include <limits>
#include <iostream>
#include <GL/glut.h>

LineChart::LineChart() : Chart2D() {
    numberAxes = 4;

    seriesList = new ChartPointSeriesList();
    axes = new LineChartAxis *[numberAxes];
    adjustYAxisToData = true;

    legend = new LineChartLegend(this);
    legend->setDisplay(false);

    setUpAxes();

    setHeight(460);
    setWidth(600);
    setLineWidths(1);
    setMarkersSize(5);
    displayMarkersOn();
    
    offsetX = 40;
    offsetY = 40;
    innerWidth = 10;
    innerHeight = 10;

    updateActualSize();

    displayGhost = true;
    displayGhostAsLine = false;

    allPickables = new std::vector<Pickable *>();
}

LineChart::~LineChart() {
    while (!seriesList->empty()) {
        ChartPointSeries *s = seriesList->back();
        seriesList->pop_back();
        delete s;
    }

    delete[] axes;
    delete seriesList;
    delete legend;
    delete allPickables;
}

void LineChart::setUpAxes() {
    axes[BOTTOM] = new LineChartAxis(this, BOTTOM);
    axes[TOP] = new LineChartAxis(this, TOP);
    axes[LEFT] = new LineChartAxis(this, LEFT);
    axes[RIGHT] = new LineChartAxis(this, RIGHT);
    axes[TOP]->displayOff();
    axes[RIGHT]->displayOff();
}

void LineChart::updateActualSize() {
    innerWidth = width;
    innerHeight = height;

    innerWidth = innerWidth - axes[RIGHT]->getSize();
    innerWidth = innerWidth - axes[LEFT]->getSize();
    innerHeight = innerHeight - axes[TOP]->getSize();
    innerHeight = innerHeight - axes[BOTTOM]->getSize();

    if (displayTitle) {
        innerHeight = innerHeight - fontHeight - fontHeight / 3;
    }

    offsetX = axes[LEFT]->getSize();
    offsetY = axes[BOTTOM]->getSize();
}

void LineChart::setWidth(float w) {
    width = w;
    updateActualSize();
}

void LineChart::setHeight(float h){
    height = h;
    updateActualSize();
}

void LineChart::drawAtOrigin() {
    updateActualSize();

    glPushMatrix();
        glTranslatef(offsetX, offsetY, 0);
        calculateGlobalBounds();
        drawBoundary(); 
        drawAxes();
        drawLines();  
        
        if (legend->getDisplay()) {
            legend->setLocation(innerWidth + 5, height / 2);
            legend->draw();
        }

        float titlePos = innerHeight + fontHeight / 3;
        if (axes[TOP]->getDisplay()) {
            titlePos = titlePos + axes[TOP]->getSize();
        }
        if (displayTitle) {
            glColor4f(0, 0, 0, 1);
            PrintText::drawStrokeText(title, innerWidth / 2, titlePos, fontHeight, HORIZ_CENTER);
        }
        drawSelected();
    glPopMatrix();
}



void LineChart::drawAxes() {
    drawXAxis(axes[BOTTOM]);
    drawXAxis(axes[TOP]);
    
    drawYAxis(axes[LEFT]);
    drawYAxis(axes[RIGHT]);
}

void LineChart::drawXAxis(LineChartAxis *axisX) {
    if (axisX->getDisplay()) {
        axisX->setMinimumValue(globalMinX);
        axisX->setMaximumValue(globalMaxX);
        
        axisX->draw();
    }
}

void LineChart::setLegendFontHeight(float h) {
    legend->setFontHeight(h);
}

void LineChart::setAxesFontHeight(float h) {
    for (int i = 0; i < numberAxes; i++) {
        axes[i]->setFontHeight(h);
    }   
}

void LineChart::drawYAxis(LineChartAxis *axisY) {
    if (axisY->getDisplay()) {
        axisY->setMinimumValue(0);
        axisY->setMaximumValue(globalMaxY);

        axisY->draw();
    }
}

void LineChart::drawToPickAtOrigin() {
    glPushMatrix();
        glTranslatef(offsetX, offsetY, 0);
     
        calculateGlobalBounds();
        drawToPickLines();
    glPopMatrix();
}

void LineChart::calculateGlobalBounds() {
    globalMinX = (std::numeric_limits<float>::max)();
    globalMaxX = -1 * (std::numeric_limits<float>::max)();

    if (adjustYAxisToData) {
        globalMinY = (std::numeric_limits<float>::max)();
        globalMaxY = -1 * (std::numeric_limits<float>::max)();
    }

    FOREACH_POINTSERIESP(it, seriesList) {
        if ((*it)->getDisplay()) {
            float minX = (*it)->getMinimumValueX();
            float maxX = (*it)->getMaximumValueX();

            float minY = (*it)->getMinimumValueY();
            float maxY = (*it)->getMaximumValueY();

            if (globalMinX > minX) {
                globalMinX = minX;
            }

            if (globalMaxX < maxX) {
                globalMaxX = maxX;
            }
          
            if (adjustYAxisToData) {
                if (globalMinY > minY) {
                    globalMinY = minY;
                }

                if (globalMaxY < maxY) {
                    globalMaxY = maxY;
                }

                if (displayGhost) {
                    float prevMinY = (*it)->getPreviousMinimumValueY();
                    float prevMaxY = (*it)->getPreviousMaximumValueY();
                
                    if (globalMinY > prevMinY) {
                        globalMinY = prevMinY;
                    }

                    if (globalMaxY < prevMaxY) {
                        globalMaxY = prevMaxY;
                    }
                }
            }
        }
    }

    if (adjustYAxisToData) {
        globalMaxY = globalMaxY + 0.05 * (globalMaxY - globalMinY);
    }
}

void LineChart::drawBoundary() {  
    glDisable(GL_LINE_SMOOTH);
    glPolygonMode(GL_FRONT, GL_LINE);  
    glLineWidth(1.0);
    glColor4f(0.5, 0.5, 0.5, 1);

    glBegin(GL_LINE_LOOP);
        glVertex2f( 0, 0 );
        glVertex2f( 0, innerHeight );
        glVertex2f( innerWidth, innerHeight );
        glVertex2f( innerWidth, 0 );
    glEnd();
    glEnable(GL_LINE_SMOOTH);
}

void LineChart::drawLines() {
    if (displayGhost) {
        FOREACH_POINTSERIESP(it, seriesList) {
            if ((*it)->getDisplay()) {
                if (displayGhostAsLine) {
                    (*it)->drawGhostAsLine();
                } else {
                    (*it)->drawGhostAsBlend();
                }
            }
        }
    }

    FOREACH_POINTSERIESP(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->draw();
        }
    }
}

void LineChart::drawSelected() {
    FOREACH_POINTSERIESP(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->drawSelected();
        }
    }
}   

void LineChart::drawToPickLines() {
    FOREACH_POINTSERIESP(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->drawToPick();
        }
    }
}

void LineChart::addPointSeries(ChartPointSeries *series) {
    seriesList->push_back(series);
}

std::vector<Pickable *> *LineChart::getPickables() {
    //ChartPointList *newList = new ChartPointList();

    allPickables->clear();

    FOREACH_POINTSERIESP(it, seriesList) {
        if ((*it)->getDisplay()) {
            ChartPoint **allPoints = (*it)->getPoints();
            
            for (int i = 0; i < (*it)->getSize(); i++) {
                allPickables->push_back(allPoints[i]);
            }
            //allPickables->insert(allPickables->end(), (*it)->getPoints()->begin(), (*it)->getPoints()->end());
        }
    }

    return allPickables;
    //return newList;
}

void LineChart::setLineWidths(float w) {
    FOREACH_POINTSERIESP(it, seriesList) {       
        (*it)->setLineWidth(w);
    }
}

void LineChart::setDisplayMarkers(bool d) {
    FOREACH_POINTSERIESP(it, seriesList) {       
        (*it)->setDisplayMarkers(d);
    }
}

void LineChart::displayMarkersOn() {
    FOREACH_POINTSERIESP(it, seriesList) {       
        (*it)->displayMarkersOn();
    }
}

void LineChart::displayMarkersOff() {
    FOREACH_POINTSERIESP(it, seriesList) {       
        (*it)->displayMarkersOff();
    }
}

void LineChart::setMarkersSize(float s) {
    FOREACH_POINTSERIESP(it, seriesList) {       
        (*it)->setMarkerSize(s);
    }
}

void LineChart::displayAsAreas() {
    FOREACH_POINTSERIESP(it, seriesList) {       
        (*it)->displayAsAreaOn();
    }
}

void LineChart::displayAsLines() {  
    FOREACH_POINTSERIESP(it, seriesList) {       
        (*it)->displayAsAreaOff();
    }
}

LineChartAxis *LineChart::getBottomAxis() {
    return axes[BOTTOM];
}

LineChartAxis *LineChart::getTopAxis() {
    return axes[TOP];
}

LineChartAxis *LineChart::getLeftAxis() {
    return axes[LEFT];
}

LineChartAxis *LineChart::getRightAxis() {
    return axes[RIGHT];
}

bool LineChart::getBottomAxisDisplay() {
    return axes[BOTTOM]->getDisplay();
}

bool LineChart::getTopAxisDisplay() {
    return axes[TOP]->getDisplay();
}

bool LineChart::getLeftAxisDisplay() {
    return axes[LEFT]->getDisplay();
}

bool LineChart::getRightAxisDisplay() {
    return axes[RIGHT]->getDisplay();
}

void LineChart::setBottomAxisDisplay(bool d) {
    axes[BOTTOM]->setDisplay(d);
}

void LineChart::setTopAxisDisplay(bool d) {
    axes[TOP]->setDisplay(d);
}

void LineChart::setLeftAxisDisplay(bool d) {
    axes[LEFT]->setDisplay(d);
}

void LineChart::setRightAxisDisplay(bool d) {
    axes[RIGHT]->setDisplay(d);
}

void LineChart::capturePreviousValues() {
    FOREACH_POINTSERIESP(it, seriesList) {
        (*it)->capturePreviousValues();
    }
}

float LineChart::getLegendWidth() {
    return legend->getWidth();
}

float LineChart::getLargestFinalValue() {
    float maxValue = -1 * (std::numeric_limits<float>::max)();

    for (unsigned int i = 0; i < seriesList->size(); i++) {
        maxValue = std::max(maxValue, seriesList->at(i)->getFinalValue());
    }

    return maxValue;
}