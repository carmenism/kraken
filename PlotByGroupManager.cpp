#include "PlotByGroupManager.h"
#include "MultiSpeciesLineChart.h"
#include "MS_PROD_MainWindow.h"
#include <QList>
#include <QStringList>

PlotByGroupManager::PlotByGroupManager() {
    charts = new std::vector<MultiSpeciesLineChart *>();
}

PlotByGroupManager::~PlotByGroupManager() {
    while (!charts->empty()) {
        MultiSpeciesLineChart *c = charts->back();
        charts->pop_back();
        delete c;
    }

    delete charts;
}

void PlotByGroupManager::updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow) {
    if (charts->empty()) {
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

            charts->at(i)->setValues(matrixForGuild);
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
        chart->setWidth(420);
        chart->setHeight(330);
        chart->setAxesFontHeight(10);
        chart->setLegendFontHeight(12);
        charts->push_back(chart);
    }

    charts->at(0)->setLocation(5, 47);
    charts->at(1)->setLocation(620, 47);
    charts->at(2)->setLocation(5, 440);
    charts->at(3)->setLocation(620, 440);
}

std::vector<LineChart *> *PlotByGroupManager::getCharts() {
    std::vector<LineChart *> *lineCharts = new std::vector<LineChart *>();

    for (int i = 0; i < charts->size(); i++) {
        lineCharts->push_back(charts->at(i));
    }

    return lineCharts;
}