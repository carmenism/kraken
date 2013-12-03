#include "SingleSpeciesLineChart.h"
#include "LineChartAxis.h"
#include "PrintText.h"
#include "Color.h"
#include "AbsoluteSizesChart.h"
#include "LineChartLegend.h"
#include "AbsoluteSizeLegend.h"
#include <GL/glut.h>
#include <iostream>

SingleSpeciesLineChart::SingleSpeciesLineChart(std::vector<float> *x, std::vector<float> *yBiomass, std::vector<float> *yHarvest, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex) 
: LineChart() {      
    sideLabel = label;
    ChartPointSeries *biomassSeries = new ChartPointSeries(this, label, x, yBiomass);
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

    axes->at(AXIS_BOTTOM)->setLabel("Year");
    axes->at(AXIS_BOTTOM)->setDisplayLabel(displayXAxisLabels);
    axes->at(AXIS_BOTTOM)->setDisplayTickLabels(displayXAxisLabels);

    axes->at(AXIS_LEFT)->setLabel("Biomass (mt)");
    axes->at(AXIS_LEFT)->displayLabelOn();

    updateActualSize();

    absChart = new AbsoluteSizesChart(this);
    absChart->setDisplayLegend(displayXAxisLabels);

    displayAbsoluteSizes = false;

    displayHarvest = false;
    
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

    ChartPointSeries *harvestSeries = new ChartPointSeries(harvest, label, x, yHarvest);
    harvestSeries->setColor(c);

    harvest->addPointSeries(harvestSeries);
    harvest->setMarkersSize(6);

    fontHeight = 12;
}


SingleSpeciesLineChart::~SingleSpeciesLineChart() {
    delete absChart;
    delete harvest;
}

ChartPointList *SingleSpeciesLineChart::getPoints() {
    if (displayHarvest) {
        return harvest->getPoints();
    }

    return LineChart::getPoints();
}

void SingleSpeciesLineChart::setValues(std::vector<float> *x, std::vector<float> *yBiomass, std::vector<float> *yHarvest) {
    seriesList->front()->setValues(x, yBiomass);
    harvest->getPointSeriesList()->front()->setValues(x, yHarvest);
}

void SingleSpeciesLineChart::draw() {
    if (displayHarvest) {
        float w = harvest->getLeftAxis()->getSize();
        harvest->setLocation(xPos, yPos + offsetY);
        harvest->setWidth(getInnerWidth() + w);
        harvest->setHeight(getInnerHeight());

        if (harvest->getBottomAxis()->getDisplayLabel()) {
            harvest->setLocation(xPos, yPos);
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

void SingleSpeciesLineChart::drawToPick() {
    LineChart::drawToPick();
    
    if (displayAbsoluteSizes && !displayChart) {
        absChart->drawToPick();
    }    

    if (displayHarvest) {
        harvest->drawToPick();
    }
}

void SingleSpeciesLineChart::drawAtOrigin() {
    if (displayChart) {
        LineChart::drawAtOrigin();
    }

    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(sideLabel, -10, offsetY + innerHeight / 2, fontHeight, HORIZ_RIGHT, VERT_CENTER);
}

Color *SingleSpeciesLineChart::getColor() {
    return seriesList->front()->getColor();
}

std::vector<AbsoluteSizeIndicator *> *SingleSpeciesLineChart::getAbsPoints() {
    return absChart->getPoints();
}

void SingleSpeciesLineChart::setAbsLegendLocation(float x, float y) {
    absChart->setLegendLocation(x, y);
}

float SingleSpeciesLineChart::getPercentIncreaseOfLastPoint() {
    return seriesList->front()->getPercentIncreaseOfLastPoint();
}