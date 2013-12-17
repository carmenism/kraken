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

    while (!biomassMatrix->empty()) {
        QList<double> *row = biomassMatrix->back();
        biomassMatrix->pop_back();
        delete row;
    }

    delete biomassMatrix;
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

void MonteCarloPlotManager::updateStatistics() {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->updateStatistics();
    }
}

bool MonteCarloPlotManager::getDisplayStreaks() {
    if (!charts->empty()) {
        return charts->at(0)->getDisplayStreaks();
    }

    return false;
}

void MonteCarloPlotManager::setDisplayStreaks(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayStreaks(d);
    }
}

void MonteCarloPlotManager::displayStreaksOn() {
    setDisplayStreaks(true);
}

void MonteCarloPlotManager::displayStreaksOff() {
    setDisplayStreaks(false);
}

bool MonteCarloPlotManager::getDisplayOriginalLine() {
    if (charts == NULL || charts->empty()) {
        return false;
    }

    return charts->at(0)->getDisplayOriginalLine();
}

void MonteCarloPlotManager::setDisplayOriginalLine(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayOriginalLine(d);
    }
}

void MonteCarloPlotManager::displayOriginalLineOn() {
    setDisplayOriginalLine(true);
}

void MonteCarloPlotManager::displayOriginalLineOff() {
    setDisplayOriginalLine(false);
}

bool MonteCarloPlotManager::getDisplayBoxPlots() {
    if (!charts->empty()) {
        return charts->at(0)->getDisplayBoxPlots();
    }

    return false;
}

void MonteCarloPlotManager::setDisplayBoxPlots(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayBoxPlots(d);
    }
}

void MonteCarloPlotManager::displayBoxPlotsOn() {
    setDisplayBoxPlots(true);
}

void MonteCarloPlotManager::displayBoxPlotsOff() {
    setDisplayBoxPlots(false);
}
   
bool MonteCarloPlotManager::getDisplayErrorBands() {
    if (!charts->empty()) {
        return charts->at(0)->getDisplayErrorBands();
    }

    return false;
}

void MonteCarloPlotManager::setDisplayErrorBands(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayErrorBands(d);
    }
}

void MonteCarloPlotManager::displayErrorBandsOn() {
    setDisplayErrorBands(true);
}

void MonteCarloPlotManager::displayErrorBandsOff() {
    setDisplayErrorBands(false);
}

bool MonteCarloPlotManager::getDisplayErrorBars() {
    if (!charts->empty()) {
        return charts->at(0)->getDisplayErrorBars();
    }

    return false;
}

void MonteCarloPlotManager::setDisplayErrorBars(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayErrorBars(d);
    }
}

void MonteCarloPlotManager::displayErrorBarsOn() {
    setDisplayErrorBars(true);
}

void MonteCarloPlotManager::displayErrorBarsOff() {
    setDisplayErrorBars(false);
}

void MonteCarloPlotManager::displayBoxPlots() {
    displayBoxPlotsOn();
    displayStreaksOff();
    displayErrorBandsOff();
    displayErrorBarsOff();
}
    
void MonteCarloPlotManager::displayStreaks() {
    displayBoxPlotsOff();
    displayStreaksOn();
    displayErrorBandsOff();
    displayErrorBarsOff();
}

void MonteCarloPlotManager::displayErrorBands() {
    displayBoxPlotsOff();
    displayStreaksOff();
    displayErrorBandsOn();
    displayErrorBarsOff();
}

void MonteCarloPlotManager::displayErrorBars() {
    displayBoxPlotsOff();
    displayStreaksOff();
    displayErrorBandsOff();
    displayErrorBarsOn();
}

bool MonteCarloPlotManager::getUsingQuartiles() {
    if (charts == NULL || charts->empty()) {
        return false;
    }

    return charts->at(0)->getUsingQuartiles();
}

bool MonteCarloPlotManager::getUsingStandardDeviations() {
    if (charts == NULL || charts->empty()) {
        return false;
    }

    return charts->at(0)->getUsingStandardDeviations();
}

void MonteCarloPlotManager::useQuartiles() {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->useQuartiles();
    }
}

void MonteCarloPlotManager::useStandardDeviations() {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->useStandardDeviations();
    }
}

bool MonteCarloPlotManager::getDisplayMedianLine() {
    if (charts == NULL || charts->empty()) {
        return false;
    }

    return charts->at(0)->getDisplayMedianLine();
}

void MonteCarloPlotManager::setDisplayMedianLine(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayMedianLine(d);
    }
}

void MonteCarloPlotManager::displayMedianLineOn() {
    setDisplayMedianLine(true);
}

void MonteCarloPlotManager::displayMedianLineOff() {
    setDisplayMedianLine(false);
}

bool MonteCarloPlotManager::getDisplayMeanLine() {
    if (charts == NULL || charts->empty()) {
        return false;
    }

    return charts->at(0)->getDisplayMeanLine();
}

void MonteCarloPlotManager::setDisplayMeanLine(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayMeanLine(d);
    }
}

void MonteCarloPlotManager::displayMeanLineOn() {
    setDisplayMeanLine(true);
}

void MonteCarloPlotManager::displayMeanLineOff() {
    setDisplayMeanLine(false);
}