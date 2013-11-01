#ifndef STATISTICS_H_
#define STATISTICS_H_

class LineChart;

#include <QList>

class Statistics {
public:
    Statistics(LineChart *chart);
    ~Statistics();

    void recalculate();
private:
    LineChart *chart;

    QList<double> mean;   // middle line
    QList<double> stdDev; // box boundaries
    QList<double> min;    // whisker bottom
    QList<double> max;    // whisker end
    QList<double> median; 
    QList<double> q1;
    QList<double> q3;

    void findQuartiles(QList<double> list);
    double findMedian(QList<double> sortedList, int indexStart, int indexEnd);
};

#endif /* STATISTICS_H_ */