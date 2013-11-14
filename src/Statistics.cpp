#include "Statistics.h"
#include "LineChart.h"
#include "ChartPointSeries.h"
#include "ChartPoint.h"
#include <limits>
#include <QtAlgorithms>
#include <GL/glut.h>

Statistics::Statistics(LineChart *chart) {
    display = false;

    this->chart = chart;

    mean = new QList<double>();
    stdDev = new QList<double>();
    min = new QList<double>();
    max = new QList<double>();
    median = new QList<double>(); 
    q1 = new QList<double>();
    q3 = new QList<double>();

    startIndex = 0;
    interval = 5;

    boxTop = NULL;
    boxBottom = NULL;
    boxMiddle = NULL;

    whiskerTop = NULL;
    whiskerBottom = NULL;
}

Statistics::~Statistics() {
    delete mean;
    delete stdDev;
    delete min;
    delete max;
    delete median;
    delete q1;
    delete q3;
}

void Statistics::recalculate() {
    mean->clear();
    stdDev->clear();
    min->clear();
    max->clear();
    median->clear();
    q1->clear();
    q3->clear();

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
        
        mean->push_back(meanV);
        stdDev->push_back(stdDevV);

        findQuartiles(values);

        min->push_back(minV);
        max->push_back(maxV);
    }

    boxTop = q3;
    boxBottom = q1;
    boxMiddle = median;

    whiskerTop = max;
    whiskerBottom = min;
}

void Statistics::print() {

}

void Statistics::findQuartiles(QList<double> list) {
    qSort(list);

    int size = list.size();
    int i = size / 2;
    double med = findMedian(list, 0, size);

    if (list.at(i) != med) {
        q1->push_back(findMedian(list, 0, i));
        q3->push_back(findMedian(list, i, size));
    } else {
        q1->push_back(findMedian(list, 0, i));
        q3->push_back(findMedian(list, i + 1, size));
    }

    median->push_back(med);
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

float Statistics::calculateXLocation(float valueX) {
    return valueX * chart->getInnerWidth() / chart->getGlobalMaxX();
}
    
float Statistics::calculateYLocation(float valueY) {
    return valueY * chart->getInnerHeight() / chart->getGlobalMaxY();
}

void Statistics::draw() {
    if (boxMiddle != NULL) {
    glPushMatrix();
    float transX = chart->getOffsetX();
    float transY = chart->getOffsetY();
    glTranslatef(transX, transY, 0);

    int index = startIndex;
    float boxWidth = 20;
    float halfBoxWidth = boxWidth / 2;

    while (index < boxMiddle->size()) {
        float xPos = calculateXLocation(index);
        float yPosTop = calculateYLocation(boxTop->at(index));
        float yPosBot = calculateYLocation(boxBottom->at(index));
        float yPosMid = calculateYLocation(boxMiddle->at(index));
        float yPosMax = calculateYLocation(max->at(index));
        float yPosMin = calculateYLocation(min->at(index));

        glColor3f(0, 0, 0);
        glLineWidth(1);

        glBegin(GL_LINE_LOOP);
            glVertex2f(xPos - halfBoxWidth, yPosTop);
            glVertex2f(xPos + halfBoxWidth, yPosTop);
            glVertex2f(xPos + halfBoxWidth, yPosBot);
            glVertex2f(xPos - halfBoxWidth, yPosBot);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glVertex2f(xPos - halfBoxWidth, yPosMid);
            glVertex2f(xPos + halfBoxWidth, yPosMid);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glVertex2f(xPos, yPosMax);
            glVertex2f(xPos, yPosTop);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glVertex2f(xPos, yPosMin);
            glVertex2f(xPos, yPosBot);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glVertex2f(xPos - halfBoxWidth / 2, yPosMax);
            glVertex2f(xPos + halfBoxWidth / 2, yPosMax);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glVertex2f(xPos - halfBoxWidth / 2, yPosMin);
            glVertex2f(xPos + halfBoxWidth / 2, yPosMin);
        glEnd();

        index = index + interval;
    }

    glPopMatrix();
    }
}