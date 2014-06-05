#include "SmallMultiple.h"
#include "LineChartAxis.h"
#include "PrintText.h"
#include "Color.h"
#include "AbsoluteSizesChart.h"
#include "LineChartLegend.h"
#include "AbsoluteSizeLegend.h"
#include <GL/glut.h>
#include <iostream>

SmallMultiple::SmallMultiple(std::vector<float> *xValues, std::vector<float> *yBiomass, std::vector<float> *yHarvest, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex) 
: LineChart() {      
    sideLabel = label;
    ChartPointSeries *biomassSeries = new ChartPointSeries(this, label, xValues, yBiomass);
    Color *c = Color::getEvenlyDistributedColor(numGuilds, guildIndex);
    biomassSeries->setColor(c);
    
    addPointSeries(biomassSeries);

    setLineWidths(2);
    setMarkersSize(6);
    setWidth(400);
    setAxesFontHeight(9);
    setLegendFontHeight(12);
    setFontHeight(11);

    legend->setDisplay(false);
    displayTitleOff();

    axes->at(BOTTOM)->setLabel("Year");
    axes->at(BOTTOM)->setDisplayLabel(displayXAxisLabels);
    axes->at(BOTTOM)->setDisplayTickLabels(displayXAxisLabels);

    axes->at(LEFT)->setLabel("Biomass (mt)");
    axes->at(LEFT)->displayLabelOn();

    updateActualSize();

    absChart = new AbsoluteSizesChart(this);
    absChart->setDisplayLegend(displayXAxisLabels);

    displayAbsoluteSizes = true;
    displayHarvest = false;
    displayChart = true;
    
    harvest = new LineChart();
    harvest->setDisplayTitle(false);
    harvest->getRightAxis()->setDisplay(false);
    harvest->getLeftAxis()->setDisplay(true);
    harvest->getLeftAxis()->displayLabelOn();
    harvest->getLeftAxis()->setLabel("Harvest (mt)");
    harvest->getBottomAxis()->setLabel("Year");
    harvest->getBottomAxis()->displayLabelOn();
    harvest->getBottomAxis()->setDisplayLabel(displayXAxisLabels);
    harvest->getBottomAxis()->setDisplayTickLabels(displayXAxisLabels);
    harvest->setAxesFontHeight(9);

    ChartPointSeries *harvestSeries = new ChartPointSeries(harvest, label, xValues, yHarvest);
    harvestSeries->setColor(c);

    harvest->addPointSeries(harvestSeries);
    harvest->setMarkersSize(6);

    fontHeight = 12;
}


SmallMultiple::~SmallMultiple() {
    delete absChart;
    delete harvest;
}

std::vector<Pickable *> *SmallMultiple::getPickables() {
    if (displayHarvest) {
        return harvest->getPickables();
    }

    return LineChart::getPickables();
}

void SmallMultiple::setValues(std::vector<float> *xValues, std::vector<float> *yBiomass, std::vector<float> *yHarvest) {
    seriesList->front()->setValues(xValues, yBiomass);
    harvest->getPointSeriesList()->front()->setValues(xValues, yHarvest);
}

void SmallMultiple::draw() {
    if (displayHarvest) {
        float w = harvest->getLeftAxis()->getSize();
        harvest->setLocation(x, y + offsetY);
        harvest->setWidth(getInnerWidth() + w);
        harvest->setHeight(getInnerHeight());

        if (harvest->getBottomAxis()->getDisplayLabel()) {
            harvest->setLocation(x, y);
            float h = harvest->getBottomAxis()->getSize();
            harvest->setHeight(getInnerHeight() + h);
        }

        harvest->draw();
    }

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

    if (displayHarvest) {
        harvest->drawToPick();
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

std::vector<AbsoluteSizeIndicator *> *SmallMultiple::getAbsPoints() {
    return absChart->getPoints();
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