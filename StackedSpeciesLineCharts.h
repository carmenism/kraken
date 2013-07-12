#ifndef STACKEDSPECIESLINECHARTS_H_
#define STACKEDSPECIESLINECHARTS_H_

class LineChart;
class QList;
class QStringList;

class StackedSpeciesLineCharts {
public:
    StackedSpeciesLineCharts(QList<QList<double>> matrix, QStringList labels);

    void setValues(QList<QList<double>> matrix);
};

#endif /*STACKEDSPECIESLINECHARTS_H_*/