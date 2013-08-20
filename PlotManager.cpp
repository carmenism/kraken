#include "PlotManager.h"
#include "Pickable.h"
#include "ChartPoint.h"

PlotManager::~PlotManager() {

}

void PlotManager::draw(float windowWidth, float windowHeight) {
    std::vector<LineChart *> *charts = getCharts();

    for (int i = 0; i < charts->size(); i++) {
        charts->at(i)->draw();
    }
}

void PlotManager::drawToPick() {
    std::vector<LineChart *> *charts = getCharts();

    for (int i = 0; i < charts->size(); i++) {
        charts->at(i)->drawToPick();
    }
}

std::vector<Pickable *> *PlotManager::getPickables() {
    std::vector<LineChart *> *charts = getCharts();
    std::vector<Pickable *> *allPicks = new std::vector<Pickable *>();

    for (int i = 0; i < charts->size(); i++) {
        ChartPointList *points = charts->at(i)->getPoints();

        allPicks->insert(allPicks->end(), points->begin(), points->end());
    }

    return allPicks;
}

void PlotManager::captureLastValues() {
    std::vector<LineChart *> *charts = getCharts();

    for (int j = 0; j < charts->size(); j++) {
        charts->at(j)->captureLastValues();
    }
}

float PlotManager::getFontSize(float windowWidth, float windowHeight) {
    if (windowWidth < 900) {
        return 8;
    } else if (windowWidth < 1100) {
        return 10;
    } 
    
    return 12;
}

float PlotManager::getMarkersSize(float chartWidth, float chartHeight) {
    if (chartWidth < 400 || chartHeight < 200) {
        return 5;
    } 

    return 7;
}