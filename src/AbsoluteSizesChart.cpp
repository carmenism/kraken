#include "AbsoluteSizesChart.h"
#include "AbsoluteSizeIndicator.h"
#include "AbsoluteSizeLegend.h"
#include "ChartPointSeries.h"
#include "ChartPoint.h"
#include "LineChart.h"
#include <GL/glut.h>
#include <limits>

AbsoluteSizesChart::AbsoluteSizesChart(LineChart *lineChart) {
    this->lineChart = lineChart;
    this->startIndex = 0;
    this->indexInterval = 5;
    this->scalingFactor = 0.01f;

    points = new std::vector<AbsoluteSizeIndicator *>();

    ChartPointSeriesList *list = lineChart->getPointSeriesList();
    ChartPointList *lineChartPoints = list->front()->getPoints();

    globalMinX = (std::numeric_limits<float>::max)();
    globalMaxX = -1 * (std::numeric_limits<float>::max)();

    for (unsigned int i = 0; i < lineChartPoints->size(); i++) {
        std::string label = lineChartPoints->at(i)->getLabel();
        float x = lineChartPoints->at(i)->getX();
        float y = lineChartPoints->at(i)->getY();

        if (globalMinX > x) {
            globalMinX = x;
        }

        if (globalMaxX < x) {
            globalMaxX = x;
        }

        AbsoluteSizeIndicator *a = new AbsoluteSizeIndicator(label, x, y);
        points->push_back(a);
    }

    std::vector<float> sampleValues;
    sampleValues.push_back(10000);
    sampleValues.push_back(50000);
    sampleValues.push_back(100000);
    sampleValues.push_back(500000);
    legend = new AbsoluteSizeLegend(this, sampleValues);
}

AbsoluteSizesChart::~AbsoluteSizesChart() {
    while (!points->empty()) {
        AbsoluteSizeIndicator *p = points->back();
        points->pop_back();
        delete p;
    }

    delete points;
    delete legend;
}

void AbsoluteSizesChart::draw() {
    if (legend->getDisplay()) {
        legend->draw();
    }

    setX(lineChart->getX() + lineChart->getOffsetX());
    setY(lineChart->getY() + lineChart->getOffsetY());
    width = lineChart->getInnerWidth();
    height = lineChart->getInnerHeight();

    glPushMatrix();
        glTranslatef(getX(), getY(), 0); 
                
        drawAtOrigin();
    glPopMatrix();

    unsigned int i = startIndex;
    
    while (i < points->size()) {
        points->at(i)->drawSelected();
    
        i = i + indexInterval;
    }
}

void AbsoluteSizesChart::drawAtOrigin() {
    unsigned int i = startIndex;
    ChartPointSeriesList *list = lineChart->getPointSeriesList();
    ChartPointList *lineChartPoints = list->front()->getPoints();

    while (i < points->size()) {
        points->at(i)->setValueY(lineChartPoints->at(i)->getY());

        positionPoint(points->at(i));
        points->at(i)->draw();

        i = i + indexInterval;
    }
}

void AbsoluteSizesChart::positionPoint(AbsoluteSizeIndicator *point) {
    float radius = getRadiusFromValue(point->getValueY());
    float diameter = radius * 2;

    float posX = point->getValueX() * width / globalMaxX;
    float posY = height / 2;

    point->setSize(diameter, diameter);
    point->setLocation(posX, posY);
}

void AbsoluteSizesChart::drawToPick() {
    setX(lineChart->getX() + lineChart->getOffsetX());
    setY(lineChart->getY() + lineChart->getOffsetY());
    width = lineChart->getInnerWidth();
    height = lineChart->getInnerHeight();

    glPushMatrix();
        glTranslatef(getX(), getY(), 0); 
                
        drawToPickAtOrigin();
    glPopMatrix();
}

void AbsoluteSizesChart::drawToPickAtOrigin() {
    unsigned int i = startIndex;
    ChartPointSeriesList *list = lineChart->getPointSeriesList();
    ChartPointList *lineChartPoints = list->front()->getPoints();
  
    while (i < points->size()) {
        points->at(i)->setValueY(lineChartPoints->at(i)->getY());

        positionPoint(points->at(i));
        points->at(i)->drawToPick();

        i = i + indexInterval;
    }
}

float AbsoluteSizesChart::getRadiusFromValue(float value) {
    return radiusFromArea(value * scalingFactor);
}

float AbsoluteSizesChart::radiusFromArea(float area) {
    return sqrt(area /  3.14159265359);
}

bool AbsoluteSizesChart::getDisplayLegend() {
    return legend->getDisplay();
}

void AbsoluteSizesChart::setDisplayLegend(bool d) {
    legend->setDisplay(d);
}

void AbsoluteSizesChart::displayLegendOn() {
    legend->displayOn();
}

void AbsoluteSizesChart::displayLegendOff() {
    legend->displayOff();
}

void AbsoluteSizesChart::setLegendLocation(float x, float y) {
    legend->setLocation(x - legend->getWidth(), y);
}