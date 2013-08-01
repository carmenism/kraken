#include "PlotManager.h"

void PlotManager::draw() {
    std::vector<LineChart *> charts = getCharts();

    for (int i = 0; i < charts.size(); i++) {
        charts[i]->draw();
    }
}

void PlotManager::drawToPick() {
    std::vector<LineChart *> charts = getCharts();

    for (int i = 0; i < charts.size(); i++) {
        charts[i]->drawToPick();
    }
}

ChartPointList PlotManager::getPoints() {
    std::vector<LineChart *> charts = getCharts();
    ChartPointList allPoints;

    for (int i = 0; i < charts.size(); i++) {
        ChartPointList *points = charts[i]->getPoints();

        for (int j = 0; j < points->size(); j++) {
            allPoints.push_back((*points)[j]);
        }
    }

    return allPoints;
}

void PlotManager::captureLastValues() {
    std::vector<LineChart *> charts = getCharts();

    for (int j = 0; j < charts.size(); j++) {
        charts[j]->captureLastValues();
    }
}