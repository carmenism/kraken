#include "SingleSpeciesLineChart.h"
#include "LineChartAxis.h"
#include "PrintText.h"
#include "Color.h"
#include "AbsoluteSizesChart.h"
#include <GL/glut.h>

SingleSpeciesLineChart::SingleSpeciesLineChart(std::vector<float> x, std::vector<float> y, std::string label, bool displayXAxisLabels, int numGuilds, int guildIndex) 
: LineChart() {      
    sideLabel = label;
    ChartPointSeries *series = new ChartPointSeries(this, label, x, y);
    Color *c = Color::getEvenlyDistributedColor(numGuilds, guildIndex);
    series->setColor(c);

    addPointSeries(series);

    setLineWidths(2);
    setMarkersSize(6);
    setWidth(400);
    setAxesFontHeight(9);
    setLegendFontHeight(12);
    setFontHeight(11);

    if (displayXAxisLabels) {
        setHeight(75 + 8 * 2);
    } else {
        setHeight(75);
    }
    displayLegendOff();
    displayTitleOff();

    axes[AXIS_BOTTOM]->setLabel("Year");
    axes[AXIS_BOTTOM]->setDisplayLabel(displayXAxisLabels);
    axes[AXIS_BOTTOM]->setDisplayTickLabels(displayXAxisLabels);

    axes[AXIS_LEFT]->setLabel("Biomass (mt)");
    axes[AXIS_LEFT]->displayLabelOn();

    updateActualSize();

    absChart = new AbsoluteSizesChart(this);
}

void SingleSpeciesLineChart::setValues(std::vector<float> x, std::vector<float> y) {
    seriesList[0]->setValues(x, y);
}

void SingleSpeciesLineChart::draw() {
    absChart->draw();

    LineChart::draw();
}

void SingleSpeciesLineChart::drawAtOrigin() {
    LineChart::drawAtOrigin();
       
    glColor4f(0, 0, 0, 1);
    PrintText::drawStrokeText(sideLabel, -10, offsetY + actualHeight / 2, fontHeight, HORIZ_RIGHT, VERT_CENTER);
}

Color *SingleSpeciesLineChart::getColor() {
    return seriesList[0]->getColor();
}