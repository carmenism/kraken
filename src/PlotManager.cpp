#include "PlotManager.h"
#include "Pickable.h"
#include "ChartPoint.h"
#include <limits>

PlotManager::~PlotManager() {

}

void PlotManager::draw(float windowWidth, float windowHeight) {
    std::vector<LineChart *> *charts = getCharts();

    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->draw();
    }
}

void PlotManager::drawToPick() {
    std::vector<LineChart *> *charts = getCharts();

    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->drawToPick();
    }
}

std::vector<Pickable *> *PlotManager::getPickables() {
    std::vector<LineChart *> *charts = getCharts();
    std::vector<Pickable *> *allPicks = new std::vector<Pickable *>();

    for (unsigned int i = 0; i < charts->size(); i++) {
        ChartPointList *points = charts->at(i)->getPoints();

        allPicks->insert(allPicks->end(), points->begin(), points->end());
    }

    return allPicks;
}

void PlotManager::capturePreviousValues() {
    std::vector<LineChart *> *charts = getCharts();

    for (unsigned int j = 0; j < charts->size(); j++) {
        charts->at(j)->capturePreviousValues();
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
    if (chartWidth < 100 || chartHeight < 75) {
        return 4;
    }

    if (chartWidth < 200 || chartHeight < 150) {
        return 5;
    } 

    if (chartWidth < 400 || chartHeight < 300) {
        return 6;
    }

    return 7;
}

void PlotManager::displayGhostOff() {
    std::vector<LineChart *> *charts = getCharts();

    for (unsigned int j = 0; j < charts->size(); j++) {
        charts->at(j)->displayGhostOff();
    }
}

void PlotManager::displayGhostAsLine() {
    std::vector<LineChart *> *charts = getCharts();
    
    for (unsigned int j = 0; j < charts->size(); j++) {
        charts->at(j)->displayGhostOn();
        charts->at(j)->displayGhostAsALine();
    }
}

void PlotManager::displayGhostAsBlend() {
    std::vector<LineChart *> *charts = getCharts();

    for (unsigned int j = 0; j < charts->size(); j++) {
        charts->at(j)->displayGhostOn();
        charts->at(j)->displayGhostAsABlend();
    }
}

float PlotManager::getLargestValue() {
    std::vector<LineChart *> *charts = getCharts();
    float maxValue = -1 * (std::numeric_limits<float>::max)();

    for (unsigned int i = 0; i < charts->size(); i++) {
        maxValue = std::max(maxValue, charts->at(i)->getGlobalMaxY());
    }

    return maxValue;
}   

float PlotManager::getLargestFinalValue() {
    std::vector<LineChart *> *charts = getCharts();
    float maxValue = -1 * (std::numeric_limits<float>::max)();

    for (unsigned int i = 0; i < charts->size(); i++) {
        maxValue = std::max(maxValue, charts->at(i)->getLargestFinalValue());
    }

    return maxValue;
} 