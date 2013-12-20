#include "PlotBySpeciesWithArcsManager.h"
#include "SingleSpeciesLineChart.h"
#include "MS_PROD_MainWindow.h"
#include "InteractionArc.h"
#include "PredationArc.h"
#include "BetweenSpeciesArcCollection.h"
#include "LineChartAxis.h"
#include "GroupReordering.h"
#include <QList>
#include <QStringList>

PlotBySpeciesWithArcsManager::PlotBySpeciesWithArcsManager() : charts() {
    arcsCurrent = NULL;

    charts = new std::vector<SingleSpeciesLineChart *>();
}

PlotBySpeciesWithArcsManager::~PlotBySpeciesWithArcsManager() {
    while (!charts->empty()) {
        SingleSpeciesLineChart *c = charts->back();
        charts->pop_back();
        delete c;
    }

    delete arcsInter;
    delete arcsPred;
    delete arcsBoth;
    delete charts;
}

void PlotBySpeciesWithArcsManager::updateCharts(Model *model, MS_PROD_MainWindow *mainWindow) {
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
        initializeBothArcs(mainWindow);
        arcsCurrent = arcsBoth;//arcsPred;
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
    //if (biomassMatrix != NULL) {
        while (!biomassMatrix->empty()) {
            QList<double> *r = biomassMatrix->at(0);
            biomassMatrix->removeFirst();
            delete r;
        }
        delete biomassMatrix;
    //}
    delete newHarvestMatrix;
    delete harvestMatrix;
    
    //lastBiomass = biomassMatrix;
}

void PlotBySpeciesWithArcsManager::initializeCharts(QList<QList<double> *> *biomassMatrix, QList<QList<double> *> *harvestMatrix, MS_PROD_MainWindow *mainWindow) {
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

void PlotBySpeciesWithArcsManager::initializeInteractionArcs(MS_PROD_MainWindow *mainWindow) {
    arcsInter = new BetweenSpeciesArcCollection(this, "Arcs Represent Species Interaction");
    initializeArcs(arcsInter, ARC_INTERACTION, mainWindow->getParameters()->getWithinGuildCompMatrix(), true);
}

void PlotBySpeciesWithArcsManager::initializePredationArcs(MS_PROD_MainWindow *mainWindow) {
    arcsPred = new BetweenSpeciesArcCollection(this, "Arcs Represent Species Predation");
    initializeArcs(arcsPred, ARC_PREDATION, mainWindow->getParameters()->getPredationMatrix(), true);
}

void PlotBySpeciesWithArcsManager::initializeBothArcs(MS_PROD_MainWindow *mainWindow) {
    arcsBoth = new BetweenSpeciesArcCollection(this, "Orange: Predation, Gray: Interaction");
    initializeArcs(arcsBoth, ARC_PREDATION, mainWindow->getParameters()->getPredationMatrix(), false);
    arcsBoth->adjustLarger();
    //arcsBoth->setAdjustPercentage(0.5);
    
    BetweenSpeciesArcCollection *tmp = new BetweenSpeciesArcCollection(this, "TEMP");
    initializeArcs(tmp, ARC_INTERACTION, mainWindow->getParameters()->getWithinGuildCompMatrix(), false);
    tmp->adjustSmaller();
    
    arcsBoth->addArcs(tmp);

    tmp->clear();
    delete tmp;
}

void PlotBySpeciesWithArcsManager::initializeArcs(BetweenSpeciesArcCollection *arcs, int arcType, QList<QList<double>> matrix, bool useColorOfChart) {
    QList<QList<double> *> *newMatrix = groupReordering->getNewSquareMatrix(matrix);

    for (int i = 0; i < newMatrix->size(); i++) {
        for (int j = 0; j < newMatrix->at(i)->size(); j++) {
            double coeff = newMatrix->at(i)->at(j);

            if (coeff != 0) {
                Color *color = NULL;

                if (useColorOfChart) {
                    color = charts->at(j)->getColor();
                }

                arcs->addArc(arcType, coeff, charts->at(j), charts->at(i), j < i, color);
            }
        }        
    }   

    delete newMatrix;
}

void PlotBySpeciesWithArcsManager::draw(float windowWidth, float windowHeight) {
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
        setChartLocations(windowWidth, windowHeight);
        charts->at(0)->setAbsLegendLocation(windowWidth - 5, 5);
        PlotManager::draw(windowWidth, windowHeight);        
    }

    if (arcsCurrent != NULL) {
        arcsCurrent->drawSelected();
    }
}

void PlotBySpeciesWithArcsManager::drawToPick() {
    PlotManager::drawToPick();

    if (arcsCurrent != NULL) {
        arcsCurrent->drawToPick();
    }
}

BetweenSpeciesArcList *PlotBySpeciesWithArcsManager::getArcs() {
    if (arcsCurrent == NULL) {
        return NULL;
    }

    return arcsCurrent->getVisibleArcs();
}

bool PlotBySpeciesWithArcsManager::getDisplayAbsoluteSizes() {
    return charts->front()->getDisplayAbsoluteSizes();
}

void PlotBySpeciesWithArcsManager::setDisplayAbsoluteSizes(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayAbsoluteSizes(d);
    }
}

void PlotBySpeciesWithArcsManager::displayAbsoluteSizesOn() {
    setDisplayAbsoluteSizes(true);
}

void PlotBySpeciesWithArcsManager::displayAbsoluteSizesOff() {
    setDisplayAbsoluteSizes(false);
}

bool PlotBySpeciesWithArcsManager::getDisplayCharts() {
    return charts->front()->getDisplayChart();
}

void PlotBySpeciesWithArcsManager::setDisplayCharts(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayChart(d);
    }
}

void PlotBySpeciesWithArcsManager::displayChartsOn() {
    setDisplayCharts(true);
}

void PlotBySpeciesWithArcsManager::displayChartsOff() {
    setDisplayCharts(false);
}

bool PlotBySpeciesWithArcsManager::getDisplayHarvest() {
    return charts->front()->getDisplayHarvest();
}

void PlotBySpeciesWithArcsManager::setDisplayHarvest(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayHarvest(d);
    }
}

void PlotBySpeciesWithArcsManager::displayHarvestOn() {
    setDisplayHarvest(true);
}

void PlotBySpeciesWithArcsManager::displayHarvestOff() {
    setDisplayHarvest(false);
}

std::vector<AbsoluteSizeIndicator *> *PlotBySpeciesWithArcsManager::getAbsPoints() {
    std::vector<AbsoluteSizeIndicator *> *allPoints = new std::vector<AbsoluteSizeIndicator *>();

    for (unsigned int i = 0; i < charts->size(); i++) {
        std::vector<AbsoluteSizeIndicator *> *points = charts->at(i)->getAbsPoints();

        allPoints->insert(allPoints->end(), points->begin(), points->end());
    }

    return allPoints;
}

void PlotBySpeciesWithArcsManager::displayPredation() {
    arcsCurrent = arcsPred;
}

void PlotBySpeciesWithArcsManager::displayInteraction() {
    arcsCurrent = arcsInter;
}

void PlotBySpeciesWithArcsManager::displayBothArcs() {
    arcsCurrent = arcsBoth;
}

void PlotBySpeciesWithArcsManager::displayNoArcs() {
    arcsCurrent = NULL;
}

std::vector<LineChart *> *PlotBySpeciesWithArcsManager::getCharts() {
    std::vector<LineChart *> *lineCharts = new std::vector<LineChart *>();

    for (unsigned int i = 0; i < charts->size(); i++) {
        lineCharts->push_back(charts->at(i));
    }

    return lineCharts;
}
  
void PlotBySpeciesWithArcsManager::setDisplayArcsDynamically(bool d) {
    arcsInter->setDisplayDynamically(d);
    arcsPred->setDisplayDynamically(d);
    arcsBoth->setDisplayDynamically(d);
}

void PlotBySpeciesWithArcsManager::displayArcsDynamicallyOn() {
    setDisplayArcsDynamically(true);
}

void PlotBySpeciesWithArcsManager::displayArcsDynamicallyOff() {
    setDisplayArcsDynamically(false);
}