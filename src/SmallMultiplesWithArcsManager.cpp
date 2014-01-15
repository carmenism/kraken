#include "SmallMultiplesWithArcsManager.h"
#include "SmallMultiple.h"
#include "MS_PROD_MainWindow.h"
#include "InteractionArc.h"
#include "PredationArc.h"
#include "InterSpeciesArcCollection.h"
#include "LineChartAxis.h"
#include "GroupReordering.h"
#include "ChangeSlider.h"
#include "Slider.h"
#include "HarvestSpline.h"
#include <QList>
#include <QStringList>
#include <string>
#include <iostream>

int timer;

SmallMultiplesWithArcsManager::SmallMultiplesWithArcsManager() : charts() {
    arcsCurrent = NULL;
    timer = 0;

    charts = new std::vector<SmallMultiple *>();
    splines = new std::vector<HarvestSpline *>();
}

SmallMultiplesWithArcsManager::~SmallMultiplesWithArcsManager() {
    while (!splines->empty()) {
        HarvestSpline *s = splines->back();
        splines->pop_back();
        delete s;
    }

    while (!charts->empty()) {
        SmallMultiple *c = charts->back();
        charts->pop_back();
        delete c;
    }

    delete arcsInter;
    delete arcsPred;
    delete arcsBoth;
    delete charts;
    delete splines;
}

void SmallMultiplesWithArcsManager::updateCharts(Model *model, MS_PROD_MainWindow *mainWindow) {
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

void SmallMultiplesWithArcsManager::initializeCharts(QList<QList<double> *> *biomassMatrix, QList<QList<double> *> *harvestMatrix, MS_PROD_MainWindow *mainWindow) {
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

        SmallMultiple *chart = new SmallMultiple(x, yBiomass, yHarvest, newLabels->at(i).toStdString(), displayXAxis, guilds.size(), guildIndex);
        chart->setTitle(newLabels->at(i).toStdString());        
        chart->setLineWidths(1);
        charts->push_back(chart);

        displayXAxis = false;

        delete x;
        delete yBiomass;
        delete yHarvest;
    }
}

void SmallMultiplesWithArcsManager::initializeInteractionArcs(MS_PROD_MainWindow *mainWindow) {
    arcsInter = new InterSpeciesArcCollection(this, "Arcs Represent Species Interaction");
    initializeArcs(arcsInter, ARC_INTERACTION, mainWindow->getParameters()->getWithinGuildCompMatrix(), true);
}

void SmallMultiplesWithArcsManager::initializePredationArcs(MS_PROD_MainWindow *mainWindow) {
    arcsPred = new InterSpeciesArcCollection(this, "Arcs Represent Species Predation");
    initializeArcs(arcsPred, ARC_PREDATION, mainWindow->getParameters()->getPredationMatrix(), true);
}

void SmallMultiplesWithArcsManager::initializeBothArcs(MS_PROD_MainWindow *mainWindow) {
    arcsBoth = new InterSpeciesArcCollection(this, "ARCS - Orange: Predation, Blue: Interaction");
    initializeArcs(arcsBoth, ARC_PREDATION, mainWindow->getParameters()->getPredationMatrix(), false);
    arcsBoth->adjustLarger();
    //arcsBoth->setAdjustPercentage(0.5);
    
    InterSpeciesArcCollection *tmp = new InterSpeciesArcCollection(this, "TEMP");
    initializeArcs(tmp, ARC_INTERACTION, mainWindow->getParameters()->getWithinGuildCompMatrix(), false);
    tmp->adjustSmaller();
    
    arcsBoth->addArcs(tmp);

    tmp->clear();
    delete tmp;
}

void SmallMultiplesWithArcsManager::initializeArcs(InterSpeciesArcCollection *arcs, int arcType, QList<QList<double>> matrix, bool useColorOfChart) {
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

void SmallMultiplesWithArcsManager::draw(float windowWidth, float windowHeight) {
    if (arcsCurrent != NULL) {
        for (int i = 0; i < splines->size(); i++) {
            splines->at(i)->construct();
            splines->at(i)->draw();
        }

        arcsCurrent->setTitleLocation(5, 5);
        //arcsCurrent->setTitleLocation(windowWidth - 5, windowHeight - 5);
    
        if (windowWidth < 900) {
            arcsCurrent->setFontHeight(12);
        } else if (windowWidth < 1000) {
            arcsCurrent->setFontHeight(13);
        } else {
            arcsCurrent->setFontHeight(14);
        }

        arcsCurrent->draw();
        ++timer; //CW_CODE
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

void SmallMultiplesWithArcsManager::drawToPick() {
    PlotManager::drawToPick();

    if (arcsCurrent != NULL) {
        arcsCurrent->drawToPick();
    }
}

InterSpeciesArcList *SmallMultiplesWithArcsManager::getArcs() {
    if (arcsCurrent == NULL) {
        return NULL;
    }

    return arcsCurrent->getVisibleArcs();
}

bool SmallMultiplesWithArcsManager::getDisplayAbsoluteSizes() {
    return charts->front()->getDisplayAbsoluteSizes();
}

void SmallMultiplesWithArcsManager::setDisplayAbsoluteSizes(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayAbsoluteSizes(d);
    }
}

void SmallMultiplesWithArcsManager::displayAbsoluteSizesOn() {
    setDisplayAbsoluteSizes(true);
}

void SmallMultiplesWithArcsManager::displayAbsoluteSizesOff() {
    setDisplayAbsoluteSizes(false);
}

bool SmallMultiplesWithArcsManager::getDisplayCharts() {
    return charts->front()->getDisplayChart();
}

void SmallMultiplesWithArcsManager::setDisplayCharts(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayChart(d);
    }
}

void SmallMultiplesWithArcsManager::displayChartsOn() {
    setDisplayCharts(true);
}

void SmallMultiplesWithArcsManager::displayChartsOff() {
    setDisplayCharts(false);
}

bool SmallMultiplesWithArcsManager::getDisplayHarvest() {
    return charts->front()->getDisplayHarvest();
}

void SmallMultiplesWithArcsManager::setDisplayHarvest(bool d) {
    for (unsigned int i = 0; i < charts->size(); i++) {
        charts->at(i)->setDisplayHarvest(d);
    }
}

void SmallMultiplesWithArcsManager::displayHarvestOn() {
    setDisplayHarvest(true);
}

void SmallMultiplesWithArcsManager::displayHarvestOff() {
    setDisplayHarvest(false);
}

std::vector<AbsoluteSizeIndicator *> *SmallMultiplesWithArcsManager::getAbsPoints() {
    std::vector<AbsoluteSizeIndicator *> *allPoints = new std::vector<AbsoluteSizeIndicator *>();

    for (unsigned int i = 0; i < charts->size(); i++) {
        std::vector<AbsoluteSizeIndicator *> *points = charts->at(i)->getAbsPoints();

        allPoints->insert(allPoints->end(), points->begin(), points->end());
    }

    return allPoints;
}

void SmallMultiplesWithArcsManager::displayPredation() {
    arcsCurrent = arcsPred;
}

void SmallMultiplesWithArcsManager::displayInteraction() {
    arcsCurrent = arcsInter;
}

void SmallMultiplesWithArcsManager::displayBothArcs() {
    arcsCurrent = arcsBoth;
}

void SmallMultiplesWithArcsManager::displayNoArcs() {
    arcsCurrent = NULL;
}

std::vector<LineChart *> *SmallMultiplesWithArcsManager::getCharts() {
    std::vector<LineChart *> *lineCharts = new std::vector<LineChart *>();

    for (unsigned int i = 0; i < charts->size(); i++) {
        lineCharts->push_back(charts->at(i));
    }

    return lineCharts;
}
  
void SmallMultiplesWithArcsManager::setDisplayArcsDynamically(bool d) {
    arcsInter->setDisplayDynamically(d);
    arcsPred->setDisplayDynamically(d);
    arcsBoth->setDisplayDynamically(d);
}

void SmallMultiplesWithArcsManager::displayArcsDynamicallyOn() {
    setDisplayArcsDynamically(true);
}

void SmallMultiplesWithArcsManager::displayArcsDynamicallyOff() {
    setDisplayArcsDynamically(false);
}

bool SmallMultiplesWithArcsManager::getArcsAnimated() {
    return arcsBoth->getAnimated();
}

void SmallMultiplesWithArcsManager::setArcsAnimated(bool a) {
    arcsInter->setAnimated(a);
    arcsPred->setAnimated(a);
    arcsBoth->setAnimated(a);
}

void SmallMultiplesWithArcsManager::arcsAnimatedOn() {
    setArcsAnimated(true);
}

void SmallMultiplesWithArcsManager::arcsAnimatedOff() {
    setArcsAnimated(false);
}

void SmallMultiplesWithArcsManager::initializeSplines(MS_PROD_MainWindow *mainWindow, std::vector<ChangeSlider *> *sliders, std::string labelSuffix) {
    QStringList *newLabels = groupReordering->getNewLabels();
    QStringList guilds = mainWindow->getParameters()->getGuildList();

    for (unsigned int j = 0; j < charts->size(); j++) {
        SmallMultiple *chart = charts->at(j);
        QString speciesGuild = mainWindow->getParameters()->getGuildMembership(newLabels->at(j));

        for (unsigned int i = 0; i < guilds.size(); i++) {
            QString guild = guilds.at(i);

            if (QString::compare(guild, speciesGuild) == 0) {            
                splines->push_back(new HarvestSpline(sliders->at(i), chart));

                break;
            } 
        }            
    }
}