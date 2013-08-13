#include "PlotBySpeciesManager.h"
#include "SingleSpeciesLineChart.h"
#include "MS_PROD_MainWindow.h"
#include "InteractionArc.h"
#include "PredationArc.h"
#include "BetweenSpeciesArcCollection.h"
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

void PlotBySpeciesManager::updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow) {
    if (oldIndices.empty()) {
        oldIndices = getNewOrder(labels, mainWindow);
        newLabels = getNewLabels(labels, oldIndices);
    }
    
    QList<QList<double>> newMatrix = getNewTimeSeriesMatrix(matrix, oldIndices);

    if (charts->empty()) {
        initializeCharts(newMatrix, newLabels, mainWindow);
        initializePredationArcs(mainWindow);
        initializeInteractionArcs(mainWindow);
        arcsCurrent = arcsPred;
    } else {
        for (int i = 0; i < newMatrix.size(); i++) {
            std::vector<float> x;
            std::vector<float> y;
            
            for (int j = 0; j < newMatrix.at(i).size(); j++) {
                x.push_back(j);
                y.push_back(newMatrix.at(i).at(j));
            }

            charts->at(i)->setValues(x, y);
        }
    }
}

QList<QList<double>> PlotBySpeciesManager::getNewTimeSeriesMatrix(QList<QList<double>> matrix, QList<int> oldIndices) {
    QList<QList<double>> newMatrix;

    for (int i = 0; i < oldIndices.size(); i++) {
        QList<double> newRow;

        for (int j = 0; j < matrix.at(oldIndices.at(i)).size(); j++) {
            newRow.append( matrix.at(oldIndices.at(i)).at(j) );
        }

        newMatrix.append(newRow);
    }

    return newMatrix;
}

QList<QList<double>> PlotBySpeciesManager::getNewSquareMatrix(QList<QList<double>> matrix, QList<int> oldIndices) {
    QList<QList<double>> newMatrix;

    for (int i = 0; i < oldIndices.size(); i++) {
        QList<double> newRow;

        for (int j = 0; j < matrix.at(oldIndices.at(i)).size(); j++) {
            newRow.append( matrix.at(oldIndices.at(i)).at(oldIndices.at(j)) );
        }

        newMatrix.append(newRow);
    }

    return newMatrix;
}

QStringList PlotBySpeciesManager::getNewLabels(QStringList labels, QList<int> oldIndices) {
    QStringList newList;
    
    for (int i = 0; i < oldIndices.size(); i++) {
        newList.append(labels.at(oldIndices.at(i)));
    }

    return newList;
}

QList<int> PlotBySpeciesManager::getNewOrder(QStringList labels, MS_PROD_MainWindow *mainWindow) {
    QList<int> oldIndices;

    QStringList guilds = mainWindow->getParameters()->getGuildList();

    for (int g = 0; g < guilds.size(); g++) {
        for (int s = 0; s < labels.size(); s++) {
            QString guild = mainWindow->getParameters()->getGuildMembership(labels.at(s));

            if (QString::compare(guilds.at(g), guild) == 0) {
                oldIndices.append(s);
            }
        }
    }

    return oldIndices;
}

void PlotBySpeciesManager::initializeCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow) {
    bool displayXAxis = true;
    QStringList guilds = mainWindow->getParameters()->getGuildList();

    for (int i = 0; i < matrix.size(); i++) {
        QString guild = mainWindow->getParameters()->getGuildMembership(labels.at(i));
        int guildIndex;
        for (int g = 0; g < guilds.size(); g++) {
            if (QString::compare(guilds.at(g), guild) == 0) {
                guildIndex = g;
                break;
            }
        }
            
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        SingleSpeciesLineChart *chart = new SingleSpeciesLineChart(x, y, labels.at(i).toStdString(), displayXAxis, guilds.size(), guildIndex);
        chart->setTitle(labels.at(i).toStdString());
        float yLoc = 100 + (i - 1) * 78;
        if (displayXAxis) {
            yLoc = 0;
        }
        chart->setLocation(450, yLoc);
        charts->push_back(chart);

        displayXAxis = false;
    }
}

void PlotBySpeciesManager::initializeInteractionArcs(MS_PROD_MainWindow *mainWindow) {
    arcsInter = initializeArcs(ARC_INTERACTION, mainWindow->getParameters()->getWithinGuildCompMatrix());
}

void PlotBySpeciesManager::initializePredationArcs(MS_PROD_MainWindow *mainWindow) {
    arcsPred = initializeArcs(ARC_PREDATION, mainWindow->getParameters()->getPredationMatrix());
}

BetweenSpeciesArcCollection *PlotBySpeciesManager::initializeArcs(int arcType, QList<QList<double>> matrix) {
    BetweenSpeciesArcCollection *arcs = new BetweenSpeciesArcCollection();
    QList<QList<double>> newMatrix = getNewSquareMatrix(matrix, oldIndices);

    for (int i = 0; i < newMatrix.size(); i++) {
        for (int j = 0; j < newMatrix.at(i).size(); j++) {
            double coeff = newMatrix.at(i).at(j);

            if (coeff != 0) {
                arcs->addArc(arcType, coeff, charts->at(j), charts->at(i), charts->at(j)->getColor(), j < i);
            }
        }        
    }

    return arcs;
}

std::vector<LineChart *> *PlotBySpeciesManager::getCharts() {
    std::vector<LineChart *> *lineCharts = new std::vector<LineChart *>();

    for (unsigned int i = 0; i < charts->size(); i++) {
        lineCharts->push_back(charts->at(i));
    }

    return lineCharts;
}

void PlotBySpeciesManager::draw() {
    if (arcsCurrent != NULL) {
        arcsCurrent->draw();
    }

    PlotManager::draw();

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