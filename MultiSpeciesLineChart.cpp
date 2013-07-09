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
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        ChartPointSeries *series = new ChartPointSeries(this, labels.at(i).toStdString(), x, y);
        Color *c = Color::getEvenlyDistributedColor(matrix.size(), i);
        series->setColor(c);

        addPointSeries(series);
    } 
    
    setLineWidths(2);
    setMarkersSize(6);
    setWidth(800);
    setHeight(600);

    axes[AXIS_BOTTOM]->setLabel("Year");
    axes[AXIS_BOTTOM]->displayLabelOn();
    
    axes[AXIS_LEFT]->setLabel("Biomass (mt)");
    axes[AXIS_LEFT]->displayLabelOn();
}


void MultiSpeciesLineChart::setValues(QList<QList<double>> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        std::vector<float> x;
        std::vector<float> y;
        
        for (int j = 0; j < matrix.at(i).size(); j++) {
            x.push_back(j);
            y.push_back(matrix.at(i).at(j));
        }

        //try {
            seriesList[i]->setValues(x, y);
        //} catch () {

        //}
    }
}