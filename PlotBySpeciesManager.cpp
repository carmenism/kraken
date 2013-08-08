#include "PlotBySpeciesManager.h"
#include "SingleSpeciesLineChart.h"
#include "MS_PROD_MainWindow.h"
#include "InteractionArc.h"
#include "PredationArc.h"
#include <QList>
#include <QStringList>

PlotBySpeciesManager::PlotBySpeciesManager() : charts() {
    currentArcs = NULL;
}

void PlotBySpeciesManager::updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow) {
    if (oldIndices.empty()) {
        oldIndices = getNewOrder(labels, mainWindow);
        newLabels = getNewLabels(labels, oldIndices);
    }
    
    QList<QList<double>> newMatrix = getNewTimeSeriesMatrix(matrix, oldIndices);

    if (charts.empty()) {
        initializeCharts(newMatrix, newLabels, mainWindow);
        initializePredationArcs(mainWindow);
        initializeInteractionArcs(mainWindow);
        currentArcs = &arcsPred;
    } else {
        for (int i = 0; i < newMatrix.size(); i++) {
            std::vector<float> x;
            std::vector<float> y;
            
            for (int j = 0; j < newMatrix.at(i).size(); j++) {
                x.push_back(j);
                y.push_back(newMatrix.at(i).at(j));
            }

            charts.at(i)->setValues(x, y);
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

        std::cout<<labels.at(oldIndices.at(i)).toStdString()<<"\n";
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

                std::cout<<s<<"\n";
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
        float yLoc = 90 + (i - 1) * 77;
        if (displayXAxis) {
            yLoc = 0;
        }
        chart->setLocation(450, yLoc);
        charts.push_back(chart);

        displayXAxis = false;
    }
}

void PlotBySpeciesManager::initializeInteractionArcs(MS_PROD_MainWindow *mainWindow) {
    QList<QList<double>> compOrig = mainWindow->getParameters()->getWithinGuildCompMatrix();
    QList<QList<double>> comp = getNewSquareMatrix(compOrig, oldIndices);

    for (int i = 0; i < comp.size(); i++) {
        for (int j = 0; j < comp.at(i).size(); j++) {
            double compCoeff = comp.at(i).at(j);

            if (compCoeff != 0) {
                InteractionArc *arc = new InteractionArc(compCoeff, charts[j], charts[i]);
                arc->setColor(charts[j]->getColor());

                if (j < i) {
                    arc->setArcToLeft();
                }

                arcsInter.push_back(arc);
            }
        }        
    }
}

void PlotBySpeciesManager::initializePredationArcs(MS_PROD_MainWindow *mainWindow) {
    QList<QList<double>> predOrig = mainWindow->getParameters()->getPredationMatrix();
    QList<QList<double>> pred = getNewSquareMatrix(predOrig, oldIndices);

    for (int i = 0; i < pred.size(); i++) {
        for (int j = 0; j < pred.at(i).size(); j++) {
            double predCoeff = pred.at(i).at(j);

            if (predCoeff != 0) {
                PredationArc *arc = new PredationArc(predCoeff, charts[j], charts[i]);
                arc->setColor(charts[j]->getColor());

                if (j < i) {
                    arc->setArcToLeft();
                }

                arcsPred.push_back(arc);
            }
        }        
    }
}

std::vector<LineChart *> PlotBySpeciesManager::getCharts() {
    std::vector<LineChart *> lineCharts;

    for (unsigned int i = 0; i < charts.size(); i++) {
        lineCharts.push_back(charts[i]);
    }

    return lineCharts;
}

void PlotBySpeciesManager::draw() {
    BetweenSpeciesArc *selected = drawArcs();

    PlotManager::draw();

    if (selected != NULL) {
        selected->drawSelected();
    }
}

BetweenSpeciesArc *PlotBySpeciesManager::drawArcs() {
    BetweenSpeciesArc *selected = NULL;
    
    if (currentArcs != NULL) {
        for (unsigned int i = 0; i < currentArcs->size(); i++) {
            if (currentArcs->at(i)->getSelected()){
                selected = currentArcs->at(i);
            }
        }

        if (selected != NULL) {
            for (unsigned int i = 0; i < currentArcs->size(); i++) {
                if (!currentArcs->at(i)->getSelected()){
                    currentArcs->at(i)->drawFaded();
                }
            }
            
            selected->draw();
        } else {
            for (unsigned int i = 0; i < currentArcs->size(); i++) {
                currentArcs->at(i)->draw();
            }
        }
    }

    return selected;
}

void PlotBySpeciesManager::drawToPick() {
    PlotManager::drawToPick();

    for (unsigned int i = 0; i < currentArcs->size(); i++) {
        currentArcs->at(i)->drawToPick();
    }
}

BetweenSpeciesArcList *PlotBySpeciesManager::getArcs() {
    return currentArcs;
}

bool PlotBySpeciesManager::getDisplayAbsoluteSizes() {
    return charts[0]->getDisplayAbsoluteSizes();
}

void PlotBySpeciesManager::setDisplayAbsoluteSizes(bool d) {
    for (unsigned int i = 0; i < charts.size(); i++) {
        charts[i]->setDisplayAbsoluteSizes(d);
    }
}

void PlotBySpeciesManager::displayAbsoluteSizesOn() {
    setDisplayAbsoluteSizes(true);
}

void PlotBySpeciesManager::displayAbsoluteSizesOff() {
    setDisplayAbsoluteSizes(false);
}

bool PlotBySpeciesManager::getDisplayCharts() {
    return charts[0]->getDisplayChart();
}

void PlotBySpeciesManager::setDisplayCharts(bool d) {
    for (unsigned int i = 0; i < charts.size(); i++) {
        charts[i]->setDisplayChart(d);
    }
}

void PlotBySpeciesManager::displayChartsOn() {
    setDisplayCharts(true);
}

void PlotBySpeciesManager::displayChartsOff() {
    setDisplayCharts(false);
}