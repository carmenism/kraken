#include "PlotBySpeciesManager.h"
#include "GroupReordering.h"
#include "LineChartAxis.h"
#include "LineChart.h"

PlotBySpeciesManager::PlotBySpeciesManager() {
    spacing = 10;
    topSpace = 10;
    leftSpace = 180;
}

PlotBySpeciesManager::~PlotBySpeciesManager() {
    delete groupReordering;
}

void PlotBySpeciesManager::setChartLocations(float windowWidth, float windowHeight) {
    std::vector<LineChart *> *charts = getCharts();
    int numCharts = charts->size();
    float bottomAxisHeight = charts->at(0)->getBottomAxis()->getSize();
    float height = windowHeight - bottomAxisHeight - spacing * (numCharts + 1) - topSpace;   
    float chartHeight = height / numCharts;
    float maxArc = windowHeight - 2 * spacing - chartHeight - bottomAxisHeight;
    float chartWidth = windowWidth - leftSpace - maxArc;
    float x = leftSpace + maxArc / 2;

    float markerSize = getMarkersSize(chartWidth, chartHeight + bottomAxisHeight);

    charts->at(0)->setLocation(x, spacing);
    charts->at(0)->setWidth(chartWidth);
    charts->at(0)->setHeight(chartHeight + bottomAxisHeight);
    charts->at(0)->setMarkersSize(markerSize);
    
    for (int i = 1; i < charts->size(); i++) {
        float y = spacing + bottomAxisHeight + i * (chartHeight + spacing);
        charts->at(i)->setLocation(x, y);
        charts->at(i)->setWidth(chartWidth);
        charts->at(i)->setHeight(chartHeight);
        charts->at(i)->setMarkersSize(markerSize);
    }
}
