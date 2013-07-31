#include "PlotByGroupManager.h"
#include "MultiSpeciesLineChart.h"
#include "MS_PROD_MainWindow.h"

void PlotByGroupManager::draw() {
    for (int i = 0; i < charts.size(); i++) {
        charts[i]->draw();
    }
}

void PlotByGroupManager::updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow) {
    if (charts.empty()) {
        initializeCharts(matrix, labels, mainWindow);
    } else {
        QStringList guilds = mainWindow->getParameters()->getGuildList();
    
        for (int i = 0; i < guilds.size(); i++) {
            QList<QList<double>> matrixForGuild;

            for (int j = 0; j < labels.size(); j++) {
                QString g = mainWindow->getParameters()->getGuildMembership(labels.at(j));
                
                if (QString::compare(g, guilds.at(i)) == 0) {
                    matrixForGuild.append(matrix.at(j));
                }
            }

            charts.at(i)->setValues(matrixForGuild);
        }
    }
}

void PlotByGroupManager::initializeCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow) {
    QStringList guilds = mainWindow->getParameters()->getGuildList();
    
    for (int i = 0; i < guilds.size(); i++) {
        QList<QList<double>> matrixForGuild;
        QStringList labelsForGuild;

        for (int j = 0; j < labels.size(); j++) {
            QString g = mainWindow->getParameters()->getGuildMembership(labels.at(j));
            
            if (QString::compare(g, guilds.at(i)) == 0) {
                matrixForGuild.append(matrix.at(j));
                labelsForGuild.append(labels.at(j));
            }
        }

        MultiSpeciesLineChart *chart = new MultiSpeciesLineChart(matrixForGuild, labelsForGuild);
        chart->setTitle(guilds.at(i).toStdString());
        chart->setWidth(400);
        chart->setHeight(300);
        chart->setAxesFontHeight(10);
        chart->setLegendFontHeight(12);
        charts.push_back(chart);
    }

    charts[0]->setLocation(0, 37);
    charts[1]->setLocation(600, 37);
    charts[2]->setLocation(0, 440);
    charts[3]->setLocation(600, 440);
}

void PlotByGroupManager::captureLastValues() {
    for (int j = 0; j < charts.size(); j++) {
        charts[j]->captureLastValues();
    }
}

ChartPointList PlotByGroupManager::getPoints() {
    ChartPointList allPoints;

    for (int i = 0; i < charts.size(); i++) {
        ChartPointList *points = charts[i]->getPoints();

        for (int j = 0; j < points->size(); j++) {
            allPoints.push_back((*points)[j]);
        }
    }

    return allPoints;
}

void PlotByGroupManager::drawToPick() {
    for (int i = 0; i < charts.size(); i++) {
        charts[i]->drawToPick();
    }
}