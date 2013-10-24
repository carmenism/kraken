#include "MonteCarloLineChart.h"
#include "Color.h"
#include "LineChart.h"
#include "LineChartAxis.h"
#include "LineChartLegend.h"
#include "ChartPointSeries.h"
#include "PrintText.h"
#include <GL/glut.h>

MonteCarloLineChart::MonteCarloLineChart(std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex) 
: LineChart() {      
    sideLabel = label;
    Color *c = Color::getEvenlyDistributedColor(numGuilds, guildIndex);
    semiTransparentColor = new Color(c->r, c->g, c->b, 0.075);

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
}

MonteCarloLineChart::~MonteCarloLineChart() {
    delete semiTransparentColor;
}

void MonteCarloLineChart::drawAtOrigin() {
    LineChart::drawAtOrigin();
    
    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(sideLabel, -10, offsetY + innerHeight / 2, fontHeight, HORIZ_RIGHT, VERT_CENTER);
}

void MonteCarloLineChart::addSemiTransparentPointSeries(int simNum, std::vector<float> *x, std::vector<float> *y) {
    addPointSeries(simNum, x, y, semiTransparentColor);
}

void MonteCarloLineChart::addBlackPointSeries(int simNum, std::vector<float> *x, std::vector<float> *y) {
    addPointSeries(simNum, x, y, &Color::black);
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