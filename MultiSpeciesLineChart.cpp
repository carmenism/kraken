#include "MultiSpeciesLineChart.h"
#include "ChartPointSeries.h"
#include "Color.h"
#include "LineChartAxis.h"
#include <QList>
#include <QStringList>
#include <QString>

MultiSpeciesLineChart::MultiSpeciesLineChart(QList<QList<double>> matrix, QStringList labels) 
: LineChart() {
    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> *x = new std::vector<float>();
        std::vector<float> *y = new std::vector<float>();
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x->push_back(j);
            y->push_back(matrix.at(i).at(j));
        }

        ChartPointSeries *series = new ChartPointSeries(this, labels.at(i).toStdString(), x, y);
        Color *c = Color::getEvenlyDistributedColor(matrix.size(), i);
        series->setColor(c);

        addPointSeries(series);

        delete x;
        delete y;
    }     

    axes->at(AXIS_BOTTOM)->setLabel("Year");
    axes->at(AXIS_LEFT)->setLabel("Biomass (mt)");
}


void MultiSpeciesLineChart::setValues(QList<QList<double>> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> *x = new std::vector<float>();
        std::vector<float> *y = new std::vector<float>();
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x->push_back(j);
            y->push_back(matrix.at(i).at(j));
        }

        seriesList->at(i)->setValues(x, y);

        delete x;
        delete y;
    }
}