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
    adjustYAxisToData = true;

    legend = new LineChartLegend(this);
    displayLegend = true;

    setUpAxes();

    setHeight(460);
    setWidth(600);
    setLineWidths(1);
    setMarkersSize(5);
    displayMarkersOn();
    displayLegendOn();
    
    offsetX = 40;
    offsetY = 40;

    updateActualSize();
}

LineChart::~LineChart() {

}

void LineChart::setUpAxes() {
    axes.push_back(new LineChartAxis(this, AXIS_BOTTOM));
    axes.push_back(new LineChartAxis(this, AXIS_TOP));
    axes.push_back(new LineChartAxis(this, AXIS_LEFT));
    axes.push_back(new LineChartAxis(this, AXIS_RIGHT));
    axes[AXIS_TOP]->displayOff();
    axes[AXIS_RIGHT]->displayOff();
}

void LineChart::updateActualSize() {
    actualWidth = width;
    actualHeight = height;

    actualWidth = actualWidth - axes[AXIS_RIGHT]->getSize();
    actualWidth = actualWidth - axes[AXIS_LEFT]->getSize();
    actualHeight = actualHeight - axes[AXIS_TOP]->getSize();
    actualHeight = actualHeight - axes[AXIS_BOTTOM]->getSize();

    if (displayTitle) {
        actualHeight = actualHeight - fontHeight - fontHeight / 3;
    }

    offsetX = axes[AXIS_LEFT]->getSize();
    offsetY = axes[AXIS_BOTTOM]->getSize();
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
        
        if (displayLegend) {
            legend->draw(actualWidth + 5, height / 2, 15, 5);
        }

        float titlePos = actualHeight + fontHeight / 3;
        if (axes[AXIS_TOP]->getDisplay()) {
            titlePos = titlePos + axes[AXIS_TOP]->getSize();
        }
        if (displayTitle) {
            glColor4f(0, 0, 0, 1);
            PrintText::drawStrokeText(title, actualWidth / 2, titlePos, fontHeight, HORIZ_CENTER);
        }
        drawSelected();
    glPopMatrix();
}



void LineChart::drawAxes() {
    drawXAxis(axes[AXIS_BOTTOM]);
    drawXAxis(axes[AXIS_TOP]);
    
    drawYAxis(axes[AXIS_LEFT]);
    drawYAxis(axes[AXIS_RIGHT]);
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
    FOREACH_LINECHARTAXIS(it, axes) {
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

    FOREACH_POINTSERIES(it, seriesList) {
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
        glVertex2f( 0, actualHeight );
        glVertex2f( actualWidth, actualHeight );
        glVertex2f( actualWidth, 0 );
    glEnd();
}

void LineChart::drawLines() {
    FOREACH_POINTSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->drawGhost();
        }
    }

    FOREACH_POINTSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->draw();
        }
    }
}

void LineChart::drawSelected() {
    FOREACH_POINTSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->drawSelected();
        }
    }
}   

void LineChart::drawToPickLines() {
    FOREACH_POINTSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->drawToPick();
        }
    }
}

void LineChart::addPointSeries(ChartPointSeries *series) {
    seriesList.push_back(series);
}

ChartPointList *LineChart::getPoints() {
    ChartPointList *newList = new ChartPointList();

    FOREACH_POINTSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            newList->insert(newList->end(), (*it)->getPoints()->begin(), (*it)->getPoints()->end());
        }
    }

    return newList;
}

void LineChart::setLineWidths(float w) {
    FOREACH_POINTSERIES(it, seriesList) {       
        (*it)->setLineWidth(w);
    }
}

void LineChart::setDisplayMarkers(bool d) {
    FOREACH_POINTSERIES(it, seriesList) {       
        (*it)->setDisplayMarkers(d);
    }
}

void LineChart::displayMarkersOn() {
    FOREACH_POINTSERIES(it, seriesList) {       
        (*it)->displayMarkersOn();
    }
}

void LineChart::displayMarkersOff() {
    FOREACH_POINTSERIES(it, seriesList) {       
        (*it)->displayMarkersOff();
    }
}

void LineChart::setMarkersSize(float s) {
    FOREACH_POINTSERIES(it, seriesList) {       
        (*it)->setMarkerSize(s);
    }
}

void LineChart::displayAsAreas() {
    FOREACH_POINTSERIES(it, seriesList) {       
        (*it)->displayAsAreaOn();
    }
}

void LineChart::displayAsLines() {  
    FOREACH_POINTSERIES(it, seriesList) {       
        (*it)->displayAsAreaOff();
    }
}

LineChartAxis *LineChart::getBottomAxis() {
    return axes[AXIS_BOTTOM];
}

LineChartAxis *LineChart::getTopAxis() {
    return axes[AXIS_TOP];
}

LineChartAxis *LineChart::getLeftAxis() {
    return axes[AXIS_LEFT];
}

LineChartAxis *LineChart::getRightAxis() {
    return axes[AXIS_RIGHT];
}

bool LineChart::getBottomAxisDisplay() {
    return axes[AXIS_BOTTOM]->getDisplay();
}

bool LineChart::getTopAxisDisplay() {
    return axes[AXIS_TOP]->getDisplay();
}

bool LineChart::getLeftAxisDisplay() {
    return axes[AXIS_LEFT]->getDisplay();
}

bool LineChart::getRightAxisDisplay() {
    return axes[AXIS_RIGHT]->getDisplay();
}

void LineChart::setBottomAxisDisplay(bool d) {
    axes[AXIS_BOTTOM]->setDisplay(d);
}

void LineChart::setTopAxisDisplay(bool d) {
    axes[AXIS_TOP]->setDisplay(d);
}

void LineChart::setLeftAxisDisplay(bool d) {
    axes[AXIS_LEFT]->setDisplay(d);
}

void LineChart::setRightAxisDisplay(bool d) {
    axes[AXIS_RIGHT]->setDisplay(d);
}

void LineChart::captureLastValues() {
    FOREACH_POINTSERIES(it, seriesList) {
        (*it)->captureLastValues();
    }
}