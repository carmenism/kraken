#include "MonteCarloPlotManager.h"
#include "MS_PROD_MainWindow.h"
#include "GroupReordering.h"
#include "MonteCarloLineChart.h"

MonteCarloPlotManager::MonteCarloPlotManager() {
    charts = new std::vector<MonteCarloLineChart *>();
}

MonteCarloPlotManager::~MonteCarloPlotManager() {
    while (!charts->empty()) {
        MonteCarloLineChart *c = charts->back();
        charts->pop_back();
        delete c;
    }

    delete charts;
}

void MonteCarloPlotManager::draw(float windowWidth, float windowHeight) {
    if (charts != NULL && !charts->empty()) {
        setChartLocations(windowWidth, windowHeight);
        
        PlotManager::draw(windowWidth, windowHeight);        
    }
}

void MonteCarloPlotManager::removeMarkers() {
    for (int i = 0; i < charts->size(); i++) {
        charts->at(i)->displayMarkersOff();
    }
}

void MonteCarloPlotManager::drawToPick() {

}

void MonteCarloPlotManager::updateCharts(Model *model, MS_PROD_MainWindow *mainWindow) {
    QList<QList<double>> biomassMatrixOrig = mainWindow->getParameters()->getBiomassMatrix();
    QList<QList<double> *> *biomassMatrix = groupReordering->getNewTimeSeriesMatrix(biomassMatrixOrig);
    
    if (getCharts()->empty()) {
        initializeCharts(biomassMatrix, mainWindow);
    }
}

void MonteCarloPlotManager::addValues(int simNum, MS_PROD_MainWindow *mainWindow, bool jittered) {
    QList<QList<double>> biomassMatrixOrig = mainWindow->getParameters()->getBiomassMatrix();
    QList<QList<double> *> *biomassMatrix = groupReordering->getNewTimeSeriesMatrix(biomassMatrixOrig);
    
    for (int i = 0; i < biomassMatrix->size(); i++) {
        std::vector<float> *x = new std::vector<float>;
        std::vector<float> *y = new std::vector<float>;
        
        for (int j = 0; j < biomassMatrix->at(i)->size(); j++) {
            x->push_back(j);
            y->push_back(biomassMatrix->at(i)->at(j));
        }

        if (jittered) {
            charts->at(i)->addSemiTransparentPointSeries(simNum, x, y);
        } else {
            charts->at(i)->addBlackPointSeries(simNum, x, y);
        }

        delete x;
        delete y;
    }
}

void MonteCarloPlotManager::initializeCharts(QList<QList<double> *> *biomassMatrix, MS_PROD_MainWindow *mainWindow) {
    bool displayXAxis = true;
    QStringList guilds = mainWindow->getParameters()->getGuildList();
    QStringList *newLabels = groupReordering->getNewLabels();

    for (int i = 0; i < biomassMatrix->size(); i++) {
        QString guild = mainWindow->getParameters()->getGuildMembership(newLabels->at(i));
        int guildIndex;
        for (int g = 0; g < guilds.size(); g++) {
            if (QString::compare(guilds.at(g), guild) == 0) {
                guildIndex = g;
                break;
            }
        }
            
        MonteCarloLineChart *chart = new MonteCarloLineChart(newLabels->at(i).toStdString(), displayXAxis, guilds.size(), guildIndex);
        chart->setTitle(newLabels->at(i).toStdString());   
        charts->push_back(chart);

        displayXAxis = false;
    }
}

std::vector<LineChart *> *MonteCarloPlotManager::getCharts() {
    std::vector<LineChart *> *lineCharts = new std::vector<LineChart *>();

    for (unsigned int i = 0; i < charts->size(); i++) {
        lineCharts->push_back(charts->at(i));
    }

    return lineCharts;
}