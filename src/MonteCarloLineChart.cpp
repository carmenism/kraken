#include "MonteCarloLineChart.h"
#include "Color.h"
#include "LineChart.h"
#include "LineChartAxis.h"
#include "LineChartLegend.h"
#include "ChartPointSeries.h"
#include "PrintText.h"
#include "Statistics.h"
#include <limits>
#include <GL/glut.h>

MonteCarloLineChart::MonteCarloLineChart(std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex) 
: LineChart() {      
    displayOriginalLine = false;
    displayStreaks = true;

    sideLabel = label;
    originalLineColor = Color::getEvenlyDistributedColor(numGuilds, guildIndex);
    semiTransparentColor = new Color(originalLineColor->r, originalLineColor->g, originalLineColor->b, 0.075);

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

    fontHeight = 12;    

    displayGhost = false;

    stats = new Statistics(this);
}

MonteCarloLineChart::~MonteCarloLineChart() {
    delete semiTransparentColor;
    delete originalLineColor;
    delete stats;
}

void MonteCarloLineChart::drawAtOrigin() {
    Color *oc = NULL;
    //if (stats->getDisplayErrorBands()) {
    //    oc = &Color::black;
    //}

    if (!seriesList->empty()) {
        for (unsigned int i = 0; i < seriesList->size() - 1; i++) {
            seriesList->at(i)->setDisplay(displayStreaks);
        }

        seriesList->at(seriesList->size() - 1)->setDisplay(displayOriginalLine);

        if (stats->getDisplayErrorBands()) {
            oc = seriesList->at(seriesList->size() - 1)->getColor();
            seriesList->at(seriesList->size() - 1)->setColor(&Color::black);
        }
    }

    LineChart::drawAtOrigin();
    
    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(sideLabel, -10, offsetY + innerHeight / 2, fontHeight, HORIZ_RIGHT, VERT_CENTER);

    if (!displayStreaks) {
        stats->draw();
    }

    if (oc != NULL) {
        seriesList->at(seriesList->size() - 1)->setColor(oc);
    }
}

void MonteCarloLineChart::addSemiTransparentPointSeries(int simNum, std::vector<float> *x, std::vector<float> *y) {
    addPointSeries(simNum, x, y, semiTransparentColor);
}

void MonteCarloLineChart::addBlackPointSeries(int simNum, std::vector<float> *x, std::vector<float> *y) {
    addPointSeries(simNum, x, y, originalLineColor);
}

void MonteCarloLineChart::addPointSeries(int simNum, std::vector<float> *x, std::vector<float> *y, Color *color) {
    if (!seriesList->empty() && simNum < seriesList->size()) {
        seriesList->at(simNum)->setValues(x, y);
    } else {
        ChartPointSeries *series = new ChartPointSeries(this, "", x, y);    
        seriesList->push_back(series);
        series->setColor(color);
    }
}

void MonteCarloLineChart::updateStatistics() {
    stats->recalculate();
}

void MonteCarloLineChart::calculateGlobalBounds() {
    globalMinX = (std::numeric_limits<float>::max)();
    globalMaxX = -1 * (std::numeric_limits<float>::max)();

    if (adjustYAxisToData) {
        globalMinY = (std::numeric_limits<float>::max)();
        globalMaxY = -1 * (std::numeric_limits<float>::max)();
    }

    FOREACH_POINTSERIESP(it, seriesList) {
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

    if (adjustYAxisToData) {
        globalMaxY = globalMaxY + 0.05 * (globalMaxY - globalMinY);
    }
}

bool MonteCarloLineChart::getDisplayBoxPlots() {
    return stats->getDisplayBoxPlots();
}

void MonteCarloLineChart::setDisplayBoxPlots(bool d) {
    stats->setDisplayBoxPlots(d);
}

void MonteCarloLineChart::displayBoxPlotsOn() {
    stats->displayBoxPlotsOn();
}

void MonteCarloLineChart::displayBoxPlotsOff() {
    stats->displayBoxPlotsOff();
}

bool MonteCarloLineChart::getDisplayErrorBands() {
    return stats->getDisplayErrorBands();
}

void MonteCarloLineChart::setDisplayErrorBands(bool d) {
    stats->setDisplayErrorBands(d);
}

void MonteCarloLineChart::displayErrorBandsOn() {
    stats->displayErrorBandsOn();
}

void MonteCarloLineChart::displayErrorBandsOff() {
    stats->displayErrorBandsOff();
}

bool MonteCarloLineChart::getDisplayErrorBars() {
    return stats->getDisplayErrorBars();
}

void MonteCarloLineChart::setDisplayErrorBars(bool d) {
    stats->setDisplayErrorBars(d);
}

void MonteCarloLineChart::displayErrorBarsOn() {
    stats->displayErrorBarsOn();
}

void MonteCarloLineChart::displayErrorBarsOff() {
    stats->displayErrorBarsOff();
}
    
void MonteCarloLineChart::useQuartiles() {
    stats->useQuartiles();
}

void MonteCarloLineChart::useStandardDeviations() {
    stats->useStandardDeviations();
}

bool MonteCarloLineChart::getUsingQuartiles() {
    return stats->getUsingQuartiles();
}

bool MonteCarloLineChart::getUsingStandardDeviations() {
    return stats->getUsingStandardDeviations();
}

bool MonteCarloLineChart::getDisplayMedianLine() {
    return stats->getDisplayMedianLine();
}

void MonteCarloLineChart::setDisplayMedianLine(bool d) {
    stats->setDisplayMedianLine(d);
}

void MonteCarloLineChart::displayMedianLineOn() {
    setDisplayMedianLine(true);
}

void MonteCarloLineChart::displayMedianLineOff() {
    setDisplayMedianLine(false);
}

bool MonteCarloLineChart::getDisplayMeanLine() {
    return stats->getDisplayMeanLine();
}

void MonteCarloLineChart::setDisplayMeanLine(bool d) {
    stats->setDisplayMeanLine(d);
}

void MonteCarloLineChart::displayMeanLineOn() {
    setDisplayMeanLine(true);
}

void MonteCarloLineChart::displayMeanLineOff() {
    setDisplayMeanLine(false);
}