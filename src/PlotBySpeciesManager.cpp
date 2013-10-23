#include "PlotBySpeciesManager.h"
#include "SingleSpeciesLineChart.h"
#include "MS_PROD_MainWindow.h"
#include "InteractionArc.h"
#include "PredationArc.h"
#include "BetweenSpeciesArcCollection.h"
#include "LineChartAxis.h"
#include "GroupReordering.h"
#include <QList>
#include <QStringList>

PlotBySpeciesManager::PlotBySpeciesManager() : charts() {
    arcsCurrent = NULL;

    charts = new std::vector<SingleSpeciesLineChart *>();
}

PlotBySpeciesManager::~PlotBySpeciesManager() {
    while (!charts->empty()) {
        SingleSpeciesLineChart *c = charts->back();
        charts->pop_back();
        delete c;
    }

    delete arcsInter;
    delete arcsPred;
    delete charts;
}

void PlotBySpeciesManager::updateCharts(Model *model, MS_PROD_MainWindow *mainWindow) {
    QList<QList<double>> biomassMatrixOrig = mainWindow->getParameters()->getBiomassMatrix();
    QList<QList<double>> harvestMatrixOrig = model->getHarvestMatrix();
   
    QList<QList<double> *> *biomassMatrix = groupReordering->getNewTimeSeriesMatrix(biomassMatrixOrig);
    QList<QList<double> *> *harvestMatrix = groupReordering->getNewTimeSeriesMatrix(harvestMatrixOrig);

    QList<QList<double> *> *newHarvestMatrix = new QList<QList<double> *>();
    for (int i = 0; i < harvestMatrix->size(); i++) {
        QList<double> *row = new QList<double>();
        
        for (int j = 0; j < harvestMatrix->at(i)->size(); j++) {
            row->append(harvestMatrix->at(i)->at(j) * -1);
        }        
        newHarvestMatrix->append(row);
    }

    if (charts->empty()) {
        initializeCharts(biomassMatrix, newHarvestMatrix, mainWindow);
        initializePredationArcs(mainWindow);
        initializeInteractionArcs(mainWindow);
        arcsCurrent = arcsPred;
    } else {
        for (int i = 0; i < biomassMatrix->size(); i++) {
            std::vector<float> *x = new std::vector<float>;
            std::vector<float> *yBiomass = new std::vector<float>;
            std::vector<float> *yHarvest = new std::vector<float>;
            
            for (int j = 0; j < biomassMatrix->at(i)->size(); j++) {
                x->push_back(j);
                yBiomass->push_back(biomassMatrix->at(i)->at(j));
                yHarvest->push_back(newHarvestMatrix->at(i)->at(j));
            }

            charts->at(i)->setValues(x, yBiomass, yHarvest);

            delete x;
            delete yBiomass;
            delete yHarvest;
        }
    }

    while (!newHarvestMatrix->empty()) {
        QList<double> *r = newHarvestMatrix->at(0);
        newHarvestMatrix->removeFirst();
        delete r;
    }
    while (!harvestMatrix->empty()) {
        QList<double> *r = harvestMatrix->at(0);
        harvestMatrix->removeFirst();
        delete r;
    }
    while (!biomassMatrix->empty()) {
        QList<double> *r = biomassMatrix->at(0);
        biomassMatrix->removeFirst();
        delete r;
    }
    delete newHarvestMatrix;
    delete harvestMatrix;
    delete biomassMatrix;
}

void PlotBySpeciesManager::initializeCharts(QList<QList<double> *> *biomassMatrix, QList<QList<double> *> *harvestMatrix, MS_PROD_MainWindow *mainWindow) {
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
            
        std::vector<float> *x = new std::vector<float>();
        std::vector<float> *yBiomass = new std::vector<float>();
        std::vector<float> *yHarvest = new std::vector<float>();
        
        for (int j = 0; j < biomassMatrix->at(i)->size(); j++) {
            x->push_back(j);
            yBiomass->push_back(biomassMatrix->at(i)->at(j));
            yHarvest->push_back(harvestMatrix->at(i)->at(j));
        }

        SingleSpeciesLineChart *chart = new SingleSpeciesLineChart(x, yBiomass, yHarvest, newLabels->at(i).toStdString(), displayXAxis, guilds.size(), guildIndex);
        chart->setTitle(newLabels->at(i).toStdString());        
        chart->setLineWidths(1);
        charts->push_back(chart);

        displayXAxis = false;

        delete x;
        delete yBiomass;
        delete yHarvest;
    }
}

void PlotBySpeciesManager::initializeInteractionArcs(MS_PROD_MainWindow *mainWindow) {
    arcsInter = initializeArcs("Arcs Represent Species Interaction", ARC_INTERACTION, mainWindow->getParameters()->getWithinGuildCompMatrix());
}

void PlotBySpeciesManager::initializePredationArcs(MS_PROD_MainWindow *mainWindow) {
    arcsPred = initializeArcs("Arcs Represent Species Predation", ARC_PREDATION, mainWindow->getParameters()->getPredationMatrix());
}

BetweenSpeciesArcCollection *PlotBySpeciesManager::initializeArcs(std::string title, int arcType, QList<QList<double>> matrix) {
    BetweenSpeciesArcCollection *arcs = new BetweenSpeciesArcCollection(title);
    QList<QList<double> *> *newMatrix = groupReordering->getNewSquareMatrix(matrix);

    for (int i = 0; i < newMatrix->size(); i++) {
        for (int j = 0; j < newMatrix->at(i)->size(); j++) {
            double coeff = newMatrix->at(i)->at(j);

            if (coeff != 0) {
                arcs->addArc(arcType, coeff, charts->at(j), charts->at(i), charts->at(j)->getColor(), j < i);
            }
        }        
    }   

    delete newMatrix;
    return arcs;
}

std::vector<LineChart *> *PlotBySpeciesManager::getCharts() {
    std::vector<LineChart *> *lineCharts = new std::vector<LineChart *>();

    for (unsigned int i = 0; i < charts->size(); i++) {
        lineCharts->push_back(charts->at(i));
    }

    return lineCharts;
}

void PlotBySpeciesManager::draw(float windowWidth, float windowHeight) {
    if (arcsCurrent != NULL) {
        arcsCurrent->setTitleLocation(windowWidth - 5, windowHeight - 5);
    
        if (windowWidth < 900) {
            arcsCurrent->setFontHeight(12);
        } else if (windowWidth < 1000) {
            arcsCurrent->setFontHeight(13);
        } else {
            arcsCurrent->setFontHeight(14);
        }

        arcsCurrent->draw();
    }

    if (charts != NULL && !charts->empty()) {
        int numCharts = charts->size();
        float spacing = 10;
        float topSpace = 10;
        float leftSpace = 180;
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
        charts->at(0)->setAbsLegendLocation(windowWidth - 5, 5);
        charts->at(0)->draw();

        for (int i = 1; i < charts->size(); i++) {
            float y = spacing + bottomAxisHeight + i * (chartHeight + spacing);
            charts->at(i)->setLocation(x, y);
            charts->at(i)->setWidth(chartWidth);
            charts->at(i)->setHeight(chartHeight);
            charts->at(i)->setMarkersSize(markerSize);
            charts->at(i)->draw();
        }
    }

    if (arcsCurrent != NULL) {
        arcsCurrent->drawSelected();
    }
}

void PlotBySpeciesManager::drawToPick() {
    PlotManager::drawToPick();

    if (arcsCurrent != NULL) {
        arcsCurrent->drawToPick();
    }
}

BetweenSpeciesArcList *PlotBySpeciesManager::getArcs() {
    if (arcsCurrent == NULL) {
        return NULL;
    }

    return arcsCurrent->getArcs();
}

bool PlotBySpeciesManager::getDisplayAbsoluteSizes() {
    return charts->front()->getDisplayAbsoluteSizes();
}

void PlotBySpeciesManager::setDisplayAbsoluteSizes(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayAbsoluteSizes(d);
    }
}

void PlotBySpeciesManager::displayAbsoluteSizesOn() {
    setDisplayAbsoluteSizes(true);
}

void PlotBySpeciesManager::displayAbsoluteSizesOff() {
    setDisplayAbsoluteSizes(false);
}

bool PlotBySpeciesManager::getDisplayCharts() {
    return charts->front()->getDisplayChart();
}

void PlotBySpeciesManager::setDisplayCharts(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayChart(d);
    }
}

void PlotBySpeciesManager::displayChartsOn() {
    setDisplayCharts(true);
}

void PlotBySpeciesManager::displayChartsOff() {
    setDisplayCharts(false);
}

bool PlotBySpeciesManager::getDisplayHarvest() {
    return charts->front()->getDisplayHarvest();
}

void PlotBySpeciesManager::setDisplayHarvest(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayHarvest(d);
    }
}

void PlotBySpeciesManager::displayHarvestOn() {
    setDisplayHarvest(true);
}

void PlotBySpeciesManager::displayHarvestOff() {
    setDisplayHarvest(false);
}

std::vector<AbsoluteSizeIndicator *> *PlotBySpeciesManager::getAbsPoints() {
    std::vector<AbsoluteSizeIndicator *> *allPoints = new std::vector<AbsoluteSizeIndicator *>();

    for (unsigned int i = 0; i < charts->size(); i++) {
        std::vector<AbsoluteSizeIndicator *> *points = charts->at(i)->getAbsPoints();

        allPoints->insert(allPoints->end(), points->begin(), points->end());
    }

    return allPoints;
}

void PlotBySpeciesManager::displayPredation() {
    arcsCurrent = arcsPred;
}

void PlotBySpeciesManager::displayInteraction() {
    arcsCurrent = arcsInter;
}

void PlotBySpeciesManager::displayNoArcs() {
    arcsCurrent = NULL;
}