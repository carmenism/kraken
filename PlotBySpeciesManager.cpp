#include "PlotBySpeciesManager.h"
#include "SingleSpeciesLineChart.h"
#include "MS_PROD_MainWindow.h"
#include "InteractionArc.h"
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

    QList<QList<double>> comp = mainWindow->getParameters()->getWithinGuildCompMatrix();
    
    for (int i = 0; i < comp.size(); i++) {
        for (int j = 0; j < comp.at(i).size(); j++) {
            double compCoeff = comp.at(i).at(j);

            if (compCoeff != 0) {
                InteractionArc *arc = new InteractionArc(compCoeff, charts[i], charts[j]);
                arc->setColor(charts[i]->getColor());

                if (j < i) {
                    arc->setArcToLeft();
                }

                arcs.push_back(arc);
            }
        }        
    }
}

std::vector<LineChart *> PlotBySpeciesManager::getCharts() {
    std::vector<LineChart *> lineCharts;

    for (int i = 0; i < charts.size(); i++) {
        lineCharts.push_back(charts[i]);
    }

    return lineCharts;
}

void PlotBySpeciesManager::draw() {
    for (int i = 0; i < arcs.size(); i++) {
        if (!arcs[i]->getSelected()){
            arcs[i]->draw();
        }
    }

    for (int i = 0; i < arcs.size(); i++) {
        if (arcs[i]->getSelected()){
            arcs[i]->draw();
        }
    }

    PlotManager::draw();

    for (int i = 0; i < arcs.size(); i++) {
        arcs[i]->drawSelected();
    }
}

void PlotBySpeciesManager::drawToPick() {
    PlotManager::drawToPick();

    for (int i = 0; i < arcs.size(); i++) {
        arcs[i]->drawToPick();
    }
}

InteractionArcList PlotBySpeciesManager::getArcs() {
    /*std::vector<InteractionArc *> newList;// = new std::vector<InteractionArc *>();

    for (int i = 0; i < arcs.size(); i++) {
        newList->push_back(arcs[i]); 
    }*/

    return arcs;//newList;
}