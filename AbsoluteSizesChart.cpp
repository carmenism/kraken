#include "AbsoluteSizesChart.h"
#include "AbsoluteSizeIndicator.h"
#include "ChartPointSeries.h"
#include "ChartPoint.h"
#include "LineChart.h"
#include <QtOpenGL>
#include <limits>

AbsoluteSizesChart::AbsoluteSizesChart(LineChart *lineChart) {//, std::vector<float> x, std::vector<float> y) : Chart2D() {
    this->lineChart = lineChart;
    this->startIndex = 0;
    this->indexInterval = 5;

    points = new std::vector<AbsoluteSizeIndicator *>();

    ChartPointSeriesList *list = lineChart->getPointSeriesList();
    ChartPointList *lineChartPoints = list->front()->getPoints();

    globalMinX = (std::numeric_limits<float>::max)();
    globalMaxX = -1 * (std::numeric_limits<float>::max)();

    for (unsigned int i = 0; i < lineChartPoints->size(); i++) {
        float x = lineChartPoints->at(i)->getX();
        float y = lineChartPoints->at(i)->getY();

        if (globalMinX > x) {
            globalMinX = x;
        }

        if (globalMaxX < x) {
            globalMaxX = x;
        }

        AbsoluteSizeIndicator *a = new AbsoluteSizeIndicator(x, y);
        points->push_back(a);
    }
}

void AbsoluteSizesChart::draw() {
    setXLocation(lineChart->getXLocation() + lineChart->getOffsetX());
    setYLocation(lineChart->getYLocation() + lineChart->getOffsetY());
    width = lineChart->getActualWidth();
    height = lineChart->getActualHeight();

    glPushMatrix();
        glTranslatef(getXLocation(), getYLocation(), 0); 
                
        drawAtOrigin();
    glPopMatrix();
}

void AbsoluteSizesChart::drawToPickAtOrigin() {

}

void AbsoluteSizesChart::drawAtOrigin() {
    unsigned int i = startIndex;
    ChartPointSeriesList *list = lineChart->getPointSeriesList();
    ChartPointList *lineChartPoints = list->front()->getPoints();

    while (i < points->size()) {
        points->at(i)->setValueY(lineChartPoints->at(i)->getY());

        float radius = radiusFromArea(points->at(i)->getValueY() / 100);
        float diameter = radius * 2;

        float posX = points->at(i)->getValueX() * width / globalMaxX;
        float posY = height / 2;

        points->at(i)->setSize(diameter, diameter);
        points->at(i)->setLocation(posX, posY);
        points->at(i)->draw();

        i = i + indexInterval;
    }
}

void AbsoluteSizesChart::calculateMaxRadius() {
    unsigned int i = startIndex;
    int number = 0;

    while (i > points->size()) {
        number++;
        i = i + indexInterval;
    }

    float spacing = 5;
    float widthWithoutSpacing = width - spacing * number; 

    maxRadius = widthWithoutSpacing / (number * 2);
}   

float AbsoluteSizesChart::radiusFromArea(float area) {
    return sqrt(area / M_PI);
}