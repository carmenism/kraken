#ifndef MULTISPECIESLINECHART_H_
#define MULTISPECIESLINECHART_H_

#include "LineChart.h"
#include <QList>

class QStringList;

class MultiSpeciesLineChart : public LineChart {
public:
    MultiSpeciesLineChart(QList<QList<double>> matrix, QStringList labels);

    void setValues(QList<QList<double>> matrix);
};

#endif /*MULTISPECIESLINECHART_H_*/