#include "SingleSpeciesLineChart.h"
#include "LineChartAxis.h"
#include "PrintText.h"
#include "Color.h"
#include "AbsoluteSizesChart.h"
#include <GL/glut.h>

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

    if (displayXAxisLabels) {
        setHeight(74 + 8 * 2);
    } else {
        setHeight(66);
    }
    displayLegendOff();
    displayTitleOff();

    axes->at(AXIS_BOTTOM)->setLabel("Year");
    axes->at(AXIS_BOTTOM)->setDisplayLabel(displayXAxisLabels);
    axes->at(AXIS_BOTTOM)->setDisplayTickLabels(displayXAxisLabels);

    axes->at(AXIS_LEFT)->setLabel("Biomass (mt)");
    axes->at(AXIS_LEFT)->displayLabelOn();

    updateActualSize();

    absChart = new AbsoluteSizesChart(this);

    displayAbsoluteSizes = false;

    displayHarvest = true;
    
    harvest = new LineChart();
    harvest->setDisplayTitle(false);
    harvest->getLeftAxis()->setDisplay(false);
    harvest->getRightAxis()->setDisplay(true);
    harvest->getRightAxis()->setLabel("Harvest (mt)");
    harvest->getRightAxis()->displayLabelOn();
    harvest->getBottomAxis()->displayLabelOff();
    harvest->getBottomAxis()->displayTickLabelsOff();
    harvest->setDisplayLegend(false);
    harvest->setAxesFontHeight(9);

    ChartPointSeries *harvestSeries = new ChartPointSeries(harvest, label, x, yHarvest);
    harvestSeries->setColor(&Color::black);

    harvest->addPointSeries(harvestSeries);
    harvest->setMarkersSize(3);
    harvest->setAdjustYAxisToData(false);
    harvest->setGlobalMinY(0);
    harvest->setGlobalMaxY(25000);
}


SingleSpeciesLineChart::~SingleSpeciesLineChart() {
    delete absChart;
    delete harvest;
}

void SingleSpeciesLineChart::setValues(std::vector<float> *x, std::vector<float> *yBiomass, std::vector<float> *yHarvest) {
    seriesList->front()->setValues(x, yBiomass);
    harvest->getPointSeriesList()->front()->setValues(x, yHarvest);
}

void SingleSpeciesLineChart::draw() {
    if (displayHarvest) {
        float w = harvest->getRightAxis()->getSize();
        harvest->setLocation(xPos + offsetX, yPos + offsetY);
        harvest->setWidth(getActualWidth() + w);
        harvest->setHeight(getActualHeight());
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
}

void SingleSpeciesLineChart::drawAtOrigin() {
    if (displayChart) {
        LineChart::drawAtOrigin();
    }

    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(sideLabel, -10, offsetY + actualHeight / 2, fontHeight, HORIZ_RIGHT, VERT_CENTER);
}

/*void SingleSpeciesLineChart::drawToPickAtOrigin() {
    LineChart::drawToPickAtOrigin();

    glPushMatrix();
        glTranslatef(offsetX, offsetY, 0);
         
    glPopMatrix();
}*/

Color *SingleSpeciesLineChart::getColor() {
    return seriesList->front()->getColor();
}

std::vector<AbsoluteSizeIndicator *> *SingleSpeciesLineChart::getAbsPoints() {
    return absChart->getPoints();
}