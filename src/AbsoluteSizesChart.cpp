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

    ChartPointSeriesList *list = lineChart->getPointSeriesList();
    ChartPoint **lineChartPoints = list->front()->getPoints();

    size = list->front()->getSize();
    points = new AbsoluteSizeIndicator *[size];

    globalMinX = (std::numeric_limits<float>::max)();
    globalMaxX = -1 * (std::numeric_limits<float>::max)();

    for (unsigned int i = 0; i < list->front()->getSize(); i++) {
        std::string label = lineChartPoints[i]->getLabel();
        float x = lineChartPoints[i]->getX();
        float y = lineChartPoints[i]->getY();

        if (globalMinX > x) {
            globalMinX = x;
        }

        if (globalMaxX < x) {
            globalMaxX = x;
        }

        points[i] = new AbsoluteSizeIndicator(label, x, y);
    }

    std::vector<float> sampleValues;
    sampleValues.push_back(10000);
    sampleValues.push_back(50000);
    sampleValues.push_back(100000);
    sampleValues.push_back(500000);
    legend = new AbsoluteSizeLegend(this, sampleValues);
}

AbsoluteSizesChart::~AbsoluteSizesChart() {
    for (int i = 0; i < size; i++) {
        delete points[i];
    }

    delete[] points;
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

    for (int p = 0; p < size; p++) {
        points[p]->setDisplay(false);
    }

    unsigned int i = startIndex;

    while (i < size) {
        points[i]->setDisplay(true);
        points[i]->drawSelected();
    
        i = i + indexInterval;
    }
}

void AbsoluteSizesChart::drawAtOrigin() {
    unsigned int i = startIndex;
    ChartPointSeriesList *list = lineChart->getPointSeriesList();
    ChartPoint **lineChartPoints = list->front()->getPoints();

    for (int p = 0; p < size; p++) {
        points[p]->setDisplay(false);
    }

    while (i < size) {
        points[i]->setDisplay(true);
        points[i]->setValueY(lineChartPoints[i]->getY());

        positionPoint(points[i]);
        points[i]->draw();

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
    ChartPoint **lineChartPoints = list->front()->getPoints();
  
    while (i < size) {
        points[i]->setValueY(lineChartPoints[i]->getY());

        positionPoint(points[i]);
        points[i]->drawToPick();

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