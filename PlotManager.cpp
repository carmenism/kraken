#include "PlotManager.h"

PlotManager::~PlotManager() {

}

void PlotManager::draw() {
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

ChartPointList *PlotManager::getPoints() {
    std::vector<LineChart *> *charts = getCharts();
    ChartPointList *allPoints = new ChartPointList();

    for (int i = 0; i < charts->size(); i++) {
        ChartPointList *points = charts->at(i)->getPoints();

        allPoints->insert(allPoints->end(), points->begin(), points->end());
    }

    return allPoints;
}

void PlotManager::captureLastValues() {
    std::vector<LineChart *> *charts = getCharts();

    for (int j = 0; j < charts->size(); j++) {
        charts->at(j)->captureLastValues();
    }
}