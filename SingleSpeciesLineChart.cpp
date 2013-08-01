#include "SingleSpeciesLineChart.h"
#include "LineChartAxis.h"

SingleSpeciesLineChart::SingleSpeciesLineChart(std::vector<float> x, std::vector<float> y, std::string label) 
: LineChart() {      
    ChartPointSeries *series = new ChartPointSeries(this, label, x, y);
    //Color *c = Color::getEvenlyDistributedColor(matrix.size(), i);
    //series->setColor(c);

    addPointSeries(series);

    setLineWidths(2);
    setMarkersSize(6);
    setWidth(600);
    setHeight(75);
    displayLegendOff();

    axes[AXIS_BOTTOM]->setLabel("Year");
    axes[AXIS_BOTTOM]->displayLabelOn();

    axes[AXIS_LEFT]->setLabel("Biomass (mt)");
    axes[AXIS_LEFT]->displayLabelOn();
}

void SingleSpeciesLineChart::setValues(std::vector<float> x, std::vector<float> y) {
    seriesList[0]->setValues(x, y);
}