#include "PlotBySpeciesManager.h"
#include "SingleSpeciesLineChart.h"
#include "MS_PROD_MainWindow.h"
#include <QList>
#include <QStringList>

PlotBySpeciesManager::PlotBySpeciesManager() : charts() {

}

void PlotBySpeciesManager::updateCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow) {
    if (charts.empty()) {
        initializeCharts(matrix, labels);
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

void PlotBySpeciesManager::initializeCharts(QList<QList<double>> matrix, QStringList labels) {
    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        SingleSpeciesLineChart *chart = new SingleSpeciesLineChart(x, y, labels.at(i).toStdString());
        chart->setTitle(labels.at(i).toStdString());
        chart->setLocation(300, i * 85);
        chart->setAxesFontHeight(9);
        chart->setLegendFontHeight(12);
        chart->setFontHeight(9);
        charts.push_back(chart);
    }
}

std::vector<LineChart *> PlotBySpeciesManager::getCharts() {
    std::vector<LineChart *> lineCharts;

    for (int i = 0; i < charts.size(); i++) {
        lineCharts.push_back(charts[i]);
    }

    return lineCharts;
}