#include "LineChart.h"
#include "ChartPoint.h"
#include "Color.h"
#include "LineChartAxis.h"
#include "LineChartLegend.h"
#include <QList>
#include <QStringList>
#include <QString>
#include <limits>
#include <iostream>
#include <QtOpenGL>
#include <GL/glut.h>

LineChart *LineChart::createGraph(QList<QList<double>> matrix, QStringList labels) {
    LineChart *chart = new LineChart();

    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        GraphMarkerSeries *series = new GraphMarkerSeries(labels.at(i).toStdString(), x, y);
        Color *c = Color::getEvenlyDistributedColor(matrix.size(), i);
        series->setColor(c);

        chart->addMarkerSeries(series);
    }

    chart->setHeight(480);
    chart->setWidth(600);
    chart->setLineWidths(2);
    chart->setMarkersSize(4);
    chart->displayMarkersOn();

    return chart;
}

LineChart::LineChart() {
    width = 400;
    height = 300;

    setUpAxes();

    legend = new LineChartLegend(this);

    displayLegend = true;
}

LineChart::LineChart(QList<QList<double>> matrix, QStringList labels) {
    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        GraphMarkerSeries *series = new GraphMarkerSeries(labels.at(i).toStdString(), x, y);
        Color *c = Color::getEvenlyDistributedColor(matrix.size(), i);
        series->setColor(c);

        addMarkerSeries(series);
    } 

    legend = new LineChartLegend(this);

    setUpAxes();

    setHeight(480);
    setWidth(600);
    setLineWidths(2);
    setMarkersSize(4);
    displayMarkersOn();
    displayLegendOn();
}

LineChart::~LineChart() {

}

void LineChart::setUpAxes() {
    axes.push_back(new LineChartAxis(AXIS_BOTTOM));
    axes.push_back(new LineChartAxis(AXIS_TOP));
    axes.push_back(new LineChartAxis(AXIS_LEFT));
    axes.push_back(new LineChartAxis(AXIS_RIGHT));
    axes[AXIS_TOP]->displayOff();
    axes[AXIS_RIGHT]->displayOff();
}

void LineChart::setValues(QList<QList<double>> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        seriesList[i]->setValues(x, y);
    }
}

void LineChart::draw() {
    glPushMatrix();
        glTranslatef(50, 50, 0);
        drawBoundary();    
        calculateGlobalBounds();
        drawAxes();
        drawLines();  
        drawLabels();

        if (displayLegend) {
            legend->draw(width + 5, height / 2, 15, 5, GLUT_BITMAP_HELVETICA_10);
        }
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
        float xInterval = calculateIntervalSize(globalMinX, globalMaxX);
        
        axisX->setMinimumValue(globalMinX);
        axisX->setMaximumValue(globalMaxX);
        axisX->setMajorTickSpacing(xInterval);
        axisX->setMinorTickSpacing(xInterval / 5);

        axisX->draw(width, height);
    }
}

void LineChart::drawYAxis(LineChartAxis *axisY) {
    if (axisY->getDisplay()) {
        float yInterval = calculateIntervalSize(globalMinY, globalMaxY);

        axisY->setMinimumValue(0);
        axisY->setMaximumValue(globalMaxY);
        axisY->setMajorTickSpacing(yInterval);
        axisY->setMinorTickSpacing(yInterval / 5);

        axisY->draw(width, height);
    }
}

float LineChart::calculateIntervalSize(float min, float max) {
    float range = max - min;
    float tempInterval = range / 8.0;

    return roundUp(tempInterval);
}

void LineChart::drawToPick() {
    glPushMatrix();
    glTranslatef(50, 50, 0);
 
    calculateGlobalBounds();
    drawToPickLines();

    glPopMatrix();
}


void LineChart::calculateGlobalBounds() {
    globalMinX = (std::numeric_limits<float>::max)();
    globalMaxX = -1 * (std::numeric_limits<float>::max)();

    globalMinY = (std::numeric_limits<float>::max)();
    globalMaxY = -1 * (std::numeric_limits<float>::max)();

    FOREACH_MARKERSERIES(it, seriesList) {
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

            if (globalMinY > minY) {
                globalMinY = minY;
            }

            if (globalMaxY < maxY) {
                globalMaxY = maxY;
            }
        }
    }

    globalMaxY = globalMaxY + 0.05 * (globalMaxY - globalMinY);
}

void LineChart::drawBoundary() {  
    glPolygonMode(GL_FRONT, GL_LINE);  
    glLineWidth(1.0);
    glColor4f(0.5, 0.5, 0.5, 1);

    glBegin(GL_LINE_LOOP);
        glVertex2f( 0, 0 );
        glVertex2f( 0, height );
        glVertex2f( width, height );
        glVertex2f( width, 0 );
    glEnd();
}

void LineChart::drawLines() {
    FOREACH_MARKERSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->draw(width, height, globalMaxX, globalMaxY);
        }
    }
}

void LineChart::drawLabels() {
    FOREACH_MARKERSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->drawLabels();
        }
    }
}   

void LineChart::drawToPickLines() {
    FOREACH_MARKERSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            (*it)->drawToPick(width, height, globalMaxX, globalMaxY);
        }
    }
}

void LineChart::addMarkerSeries(GraphMarkerSeries *series) {
    seriesList.push_back(series);
}

ChartPointList *LineChart::getPoints() {
    ChartPointList *newList = new ChartPointList();

    FOREACH_MARKERSERIES(it, seriesList) {
        if ((*it)->getDisplay()) {
            FOREACH_POINTP(it2, (*it)->getPoints()) {
                newList->push_back((*it2));
            }
        }
    }

    return newList;
}

void LineChart::setLineWidths(float w) {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->setLineWidth(w);
    }
}

void LineChart::setDisplayMarkers(bool d) {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->setDisplayMarkers(d);
    }
}

void LineChart::displayMarkersOn() {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->displayMarkersOn();
    }
}

void LineChart::displayMarkersOff() {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->displayMarkersOff();
    }
}

void LineChart::setMarkersSize(float s) {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->setMarkerSize(s);
    }
}

void LineChart::displayAsAreas() {
    FOREACH_MARKERSERIES(it, seriesList) {       
        (*it)->displayAsAreaOn();
    }
}

void LineChart::displayAsLines() {  
    FOREACH_MARKERSERIES(it, seriesList) {       
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