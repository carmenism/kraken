#include "PlotBySpeciesManager.h"
#include "SingleSpeciesLineChart.h"
#include "MS_PROD_MainWindow.h"
#include <QList>
#include <QStringList>

PlotBySpeciesManager::PlotBySpeciesManager() : charts() {

}

void PlotBySpeciesManager::updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow) {
    if (charts.empty()) {
        initializeCharts(matrix, labels, mainWindow);
    } else {
        for (int i = 0; i < matrix.size(); i++) {
            std::vector<float> x;
            std::vector<float> y;
            
            for (int j = 0; j < matrix.at(i).size(); j++) {
                x.push_back(j);
                y.push_back(matrix.at(i).at(j));
            }

            charts.at(i)->setValues(x, y);
        }
    }
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
        chart->setLocation(300, yLoc);
        chart->setAxesFontHeight(9);
        chart->setLegendFontHeight(12);
        chart->setFontHeight(11);
        charts.push_back(chart);

        displayXAxis = false;
    }
}

std::vector<LineChart *> PlotBySpeciesManager::getCharts() {
    std::vector<LineChart *> lineCharts;

    for (int i = 0; i < charts.size(); i++) {
        lineCharts.push_back(charts[i]);
    }

    return lineCharts;
}