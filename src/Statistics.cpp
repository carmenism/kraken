#include "Statistics.h"
#include "LineChart.h"
#include "ChartPointSeries.h"
#include "ChartPoint.h"
#include <limits>
#include <QtAlgorithms>

Statistics::Statistics(LineChart *chart) {
    this->chart = chart;
}

Statistics::~Statistics() {

}

void Statistics::recalculate() {
    mean.clear();
    stdDev.clear();
    min.clear();
    max.clear();

    ChartPointSeriesList * seriesList = chart->getSeriesList();

    int numSeries = seriesList->size();
    int numTimes = seriesList->at(0)->size();

    for (int time = 0; time < numTimes; time++) {
        QList<double> values;
        double sum = 0;
        float minV = (std::numeric_limits<float>::max)();
        float maxV = -1 * minV;

        for (int series = 0; series < numSeries; series++) {
            float value = seriesList->at(series)->getPoints()->at(time)->getY();
            sum += value;

            if (minV > value) {
                minV = value;
            }

            if (maxV < value) {
                maxV = value;
            }

            values.push_back(value);
        }        

        double meanV = sum / numSeries;

        sum = 0;

        for (int series = 0; series < numSeries; series++) {
            float valueMinusMean = values.at(series) - meanV;
            
            sum += valueMinusMean * valueMinusMean;
        }

        double stdDevV = std::sqrt(sum / numSeries);
        
        mean.push_back(meanV);
        min.push_back(minV);
        max.push_back(maxV);
        stdDev.push_back(stdDevV);

        findQuartiles(values);
    }
}

void Statistics::findQuartiles(QList<double> list) {
    qSort(list);

    int size = list.size();
    int i = size / 2;
    double med = findMedian(list, 0, size);

    if (list.at(i) != med) {
        q1.push_back(findMedian(list, 0, i));
        q3.push_back(findMedian(list, i, size));
    } else {
        q1.push_back(findMedian(list, 0, i));
        q3.push_back(findMedian(list, i + 1, size));
    }

    median.push_back(med);
}

// last valid value is actually indexEnd - 1
double Statistics::findMedian(QList<double> sortedList, int indexStart, int indexEnd) {
    int length = indexEnd - indexStart;
    int i = indexStart + length / 2;

    if (length % 2 == 0) {
        return (sortedList.at(i) + sortedList.at(i - 1)) / 2;
    }

    return sortedList.at(i);
}