#include "MonteCarloLineChart.h"
#include "Color.h"
#include "LineChart.h"
#include "LineChartAxis.h"
#include "LineChartLegend.h"
#include "ChartPointSeries.h"
#include "PrintText.h"
#include <GL/glut.h>

MonteCarloLineChart::MonteCarloLineChart(std::vector<float> *x, std::vector<float> *y, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex) 
: LineChart() {      
    sideLabel = label;
    ChartPointSeries *biomassSeries = new ChartPointSeries(this, label, x, y);
    Color *c = Color::getEvenlyDistributedColor(numGuilds, guildIndex);
    Color *color = new Color(c->r, c->g, c->b, 0.1);
    biomassSeries->setColor(color);
    
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

    fontHeight = 12;
}

void MonteCarloLineChart::drawAtOrigin() {
    LineChart::drawAtOrigin();
    
    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(sideLabel, -10, offsetY + innerHeight / 2, fontHeight, HORIZ_RIGHT, VERT_CENTER);
}

void MonteCarloLineChart::setValues(std::vector<float> *x, std::vector<float> *y) {
    seriesList->front()->setValues(x, y);
}

void MonteCarloLineChart::clearValues() {
    while (!seriesList->empty()) {
        ChartPointSeries *p = seriesList->back();
        seriesList->pop_back();
        delete p;
    }

    seriesList->clear();
}

void MonteCarloLineChart::addPointSeries(std::vector<float> *x, std::vector<float> *y) {
    ChartPointSeries *series = new ChartPointSeries(this, "", x, y);
    addPointSeries(series);
}

void MonteCarloLineChart::addPointSeries(ChartPointSeries *series) {
    seriesList->push_back(series);
    series->setColor(seriesList->front()->getColor());
}