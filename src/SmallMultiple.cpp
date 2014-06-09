#include "SmallMultiple.h"
#include "LineChartAxis.h"
#include "PrintText.h"
#include "Color.h"
#include "AbsoluteSizesChart.h"
#include "LineChartLegend.h"
#include "AbsoluteSizeLegend.h"
#include <GL/glut.h>
#include <iostream>

SmallMultiple::SmallMultiple(std::vector<float> *xValues, std::vector<float> *yBiomass, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex) 
: LineChart() {      
    sideLabel = label;
    ChartPointSeries *biomassSeries = new ChartPointSeries(this, label, xValues, yBiomass);
    Color *c = Color::getEvenlyDistributedColor(numGuilds, guildIndex);
    biomassSeries->setColor(c);
    delete c;
    
    addPointSeries(biomassSeries);

    setLineWidths(2);
    setMarkersSize(6);
    setWidth(400);
    setAxesFontHeight(9);
    setLegendFontHeight(12);
    setFontHeight(11);

    legend->setDisplay(false);
    displayTitleOff();

    axes[BOTTOM]->setLabel("Year");
    axes[BOTTOM]->setDisplayLabel(displayXAxisLabels);
    axes[BOTTOM]->setDisplayTickLabels(displayXAxisLabels);

    axes[LEFT]->setLabel("Biomass (mt)");
    axes[LEFT]->displayLabelOn();

    updateActualSize();

    absChart = new AbsoluteSizesChart(this);
    absChart->setDisplayLegend(displayXAxisLabels);

    displayAbsoluteSizes = true;
    displayChart = true;

    fontHeight = 12;
}

SmallMultiple::~SmallMultiple() {
    delete absChart;
}

void SmallMultiple::setValues(std::vector<float> *xValues, std::vector<float> *yBiomass) {
    seriesList->front()->setValues(xValues, yBiomass);
}

void SmallMultiple::draw() {
    if (displayAbsoluteSizes) {
        absChart->draw();
    }

    LineChart::draw();
}

void SmallMultiple::drawToPick() {
    LineChart::drawToPick();
    
    if (displayAbsoluteSizes && !displayChart) {
        absChart->drawToPick();
    }    
}

void SmallMultiple::drawAtOrigin() {
    if (displayChart) {
        LineChart::drawAtOrigin();
    }

    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(sideLabel, -10, offsetY + innerHeight / 2, fontHeight, HORIZ_RIGHT, VERT_CENTER);
}

Color *SmallMultiple::getColor() {
    return seriesList->front()->getColor();
}

AbsoluteSizeIndicator **SmallMultiple::getAbsPoints() {
    return absChart->getPoints();
}

int SmallMultiple::getAbsSize() {
    return absChart->getSize();
}

void SmallMultiple::setAbsLegendLocation(float x, float y) {
    absChart->setLegendLocation(x, y);
}

float SmallMultiple::getPercentIncreaseOfFinalValue() {
    return seriesList->front()->getPercentIncreaseOfFinalValue();
}

float SmallMultiple::getFinalValue() {
    return seriesList->front()->getFinalValue();
}

float SmallMultiple::getPreviousFinalValue() {
    return seriesList->front()->getPreviousFinalValue();
}

float SmallMultiple::getStartValue() {
    return seriesList->front()->getStartValue();
}