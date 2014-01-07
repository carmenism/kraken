#include "FourPanelManager.h"
#include "MultiSpeciesLineChart.h"
#include "MS_PROD_MainWindow.h"
#include <QList>
#include <QStringList>

FourPanelManager::FourPanelManager() {
    charts = new std::vector<MultiSpeciesLineChart *>();
}

FourPanelManager::~FourPanelManager() {
    while (!charts->empty()) {
        MultiSpeciesLineChart *c = charts->back();
        charts->pop_back();
        delete c;
    }

    delete charts;
}

void FourPanelManager::draw(float windowWidth, float windowHeight) {
    if (charts != NULL && !charts->empty()) {
        float legendFontSize = getFontSize(windowWidth, windowHeight);   

        float maxLegendWidth = 0;
        for (int i = 0; i < charts->size(); i++) {
            charts->at(i)->setLegendFontHeight(legendFontSize);
            float w = charts->at(i)->getLegendWidth();

            if (maxLegendWidth < w) {
                maxLegendWidth = w;
            }
        }

        float spacing = 10;
        float topSpacing = 20;
        float sliderHeight = 50;
        float chartWidth = (windowWidth - 3 * spacing - 2 * maxLegendWidth) / 2;
        float chartHeight = (windowHeight - 5 * spacing - 2 * sliderHeight - topSpacing) / 2;

        float leftX = spacing;
        float rightX = 2 * spacing + chartWidth + maxLegendWidth;
        float bottomY = 2 * spacing + sliderHeight;
        float topY = 2 * bottomY + chartHeight;
        
        charts->at(0)->setLocation(leftX, bottomY);
        charts->at(1)->setLocation(rightX, bottomY);
        charts->at(2)->setLocation(leftX, topY);
        charts->at(3)->setLocation(rightX, topY);

        for (int i = 0; i < charts->size(); i++) {
            charts->at(i)->setWidth(chartWidth);
            charts->at(i)->setHeight(chartHeight);
            charts->at(i)->setMarkersSize(getMarkersSize(chartWidth, chartHeight));
            charts->at(i)->draw();
        }        
    }
}

void FourPanelManager::updateCharts(Model *model, MS_PROD_MainWindow *mainWindow) {
    QList<QList<double>> matrix = mainWindow->getParameters()->getBiomassMatrix();
    QStringList labels = mainWindow->getParameters()->getSpeciesList();

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

void FourPanelManager::initializeCharts(QList<QList<double>> matrix, QStringList labels, MS_PROD_MainWindow *mainWindow) {
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
        //chart->setWidth(420);
        //chart->setHeight(330);
        chart->setAxesFontHeight(10);
        chart->setLegendFontHeight(12);
        charts->push_back(chart);
    }

}

std::vector<LineChart *> *FourPanelManager::getCharts() {
    std::vector<LineChart *> *lineCharts = new std::vector<LineChart *>();

    for (int i = 0; i < charts->size(); i++) {
        lineCharts->push_back(charts->at(i));
    }

    return lineCharts;
}