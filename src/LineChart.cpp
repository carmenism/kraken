#include "LineChart.h"
#include "ChartPoint.h"
#include "LineChartAxis.h"
#include "LineChartLegend.h"
#include "PrintText.h"
#include <limits>
#include <iostream>
#include <QtOpenGL>
#include <GL/glut.h>

LineChart::LineChart() : Chart2D() {
    seriesList = new ChartPointSeriesList();
    axes = new LineChartAxisList();
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

    updateActualSize();

    displayGhost = true;
    displayGhostAsLine = false;
}

LineChart::~LineChart() {
    while (!seriesList->empty()) {
        ChartPointSeries *s = seriesList->back();
        seriesList->pop_back();
        delete s;
    }

    while (!axes->empty()) {
        LineChartAxis *a = axes->back();
        axes->pop_back();
        delete a;
    }

    delete axes;
    delete seriesList;
    delete legend;
}

void LineChart::setUpAxes() {
    axes->push_back(new LineChartAxis(this, AXIS_BOTTOM));
    axes->push_back(new LineChartAxis(this, AXIS_TOP));
    axes->push_back(new LineChartAxis(this, AXIS_LEFT));
    axes->push_back(new LineChartAxis(this, AXIS_RIGHT));
    axes->at(AXIS_TOP)->displayOff();
    axes->at(AXIS_RIGHT)->displayOff();
}

void LineChart::updateActualSize() {
    innerWidth = width;
    innerHeight = height;

    innerWidth = innerWidth - axes->at(AXIS_RIGHT)->getSize();
    innerWidth = innerWidth - axes->at(AXIS_LEFT)->getSize();
    innerHeight = innerHeight - axes->at(AXIS_TOP)->getSize();
    innerHeight = innerHeight - axes->at(AXIS_BOTTOM)->getSize();

    if (displayTitle) {
        innerHeight = innerHeight - fontHeight - fontHeight / 3;
    }

    offsetX = axes->at(AXIS_LEFT)->getSize();
    offsetY = axes->at(AXIS_BOTTOM)->getSize();
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
        if (axes->at(AXIS_TOP)->getDisplay()) {
            titlePos = titlePos + axes->at(AXIS_TOP)->getSize();
        }
        if (displayTitle) {
            glColor4f(0, 0, 0, 1);
            PrintText::drawStrokeText(title, innerWidth / 2, titlePos, fontHeight, HORIZ_CENTER);
        }
        drawSelected();
    glPopMatrix();
}



void LineChart::drawAxes() {
    drawXAxis(axes->at(AXIS_BOTTOM));
    drawXAxis(axes->at(AXIS_TOP));
    
    drawYAxis(axes->at(AXIS_LEFT));
    drawYAxis(axes->at(AXIS_RIGHT));
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
    FOREACH_LINECHARTAXISP(it, axes) {
        (*it)->setFontHeight(h);
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
                    float lastMinY = (*it)->getLastMinimumValueY();
                    float lastMaxY = (*it)->getLastMaximumValueY();
                
                    if (globalMinY > lastMinY) {
                        globalMinY = lastMinY;
                    }

                    if (globalMaxY < lastMaxY) {
                        globalMaxY = lastMaxY;
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
    glPolygonMode(GL_FRONT, GL_LINE);  
    glLineWidth(1.0);
    glColor4f(0.5, 0.5, 0.5, 1);

    glBegin(GL_LINE_LOOP);
        glVertex2f( 0, 0 );
        glVertex2f( 0, innerHeight );
        glVertex2f( innerWidth, innerHeight );
        glVertex2f( innerWidth, 0 );
    glEnd();
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

ChartPointList *LineChart::getPoints() {
    ChartPointList *newList = new ChartPointList();

    FOREACH_POINTSERIESP(it, seriesList) {
        if ((*it)->getDisplay()) {
            newList->insert(newList->end(), (*it)->getPoints()->begin(), (*it)->getPoints()->end());
        }
    }

    return newList;
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
    return axes->at(AXIS_BOTTOM);
}

LineChartAxis *LineChart::getTopAxis() {
    return axes->at(AXIS_TOP);
}

LineChartAxis *LineChart::getLeftAxis() {
    return axes->at(AXIS_LEFT);
}

LineChartAxis *LineChart::getRightAxis() {
    return axes->at(AXIS_RIGHT);
}

bool LineChart::getBottomAxisDisplay() {
    return axes->at(AXIS_BOTTOM)->getDisplay();
}

bool LineChart::getTopAxisDisplay() {
    return axes->at(AXIS_TOP)->getDisplay();
}

bool LineChart::getLeftAxisDisplay() {
    return axes->at(AXIS_LEFT)->getDisplay();
}

bool LineChart::getRightAxisDisplay() {
    return axes->at(AXIS_RIGHT)->getDisplay();
}

void LineChart::setBottomAxisDisplay(bool d) {
    axes->at(AXIS_BOTTOM)->setDisplay(d);
}

void LineChart::setTopAxisDisplay(bool d) {
    axes->at(AXIS_TOP)->setDisplay(d);
}

void LineChart::setLeftAxisDisplay(bool d) {
    axes->at(AXIS_LEFT)->setDisplay(d);
}

void LineChart::setRightAxisDisplay(bool d) {
    axes->at(AXIS_RIGHT)->setDisplay(d);
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
        maxValue = max(maxValue, seriesList->at(i)->getFinalValue());
    }

    return maxValue;
}