#include "MonteCarlo.h"
#include "Parameters.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

MonteCarlo::MonteCarlo() {
    srand(time(NULL));

    numberSimulations = 100;
}

MonteCarlo::~MonteCarlo() {
    
}

int MonteCarlo::getRandomi(int min, int max) {
    return rand() % max + min;
}

double MonteCarlo::getRandomd(double min, double max, int n) {
    double range = max - min;
    double sum = 0;

    for (int i = 0; i < n; i++) {
        sum += min + (double) rand() / ((double) RAND_MAX / range);
    }

    return sum / n;
}
/*
double MonteCarlo::addGaussianJitter(double num, double percentJitter) {
    if (num == 0) {
        return 0;
    }

    double fullRange = percentJitter * 2;
    double sums = 0.0;
    int n = 5;
    
    for (int i = 0; i < n; i++) {
        float random = float(rand() % 1000) / 999.0;
        float jitter = num * (fullRange * random - percentJitter);
        sums = sums + jitter;
    }

    return num + (sums / n);
}

double MonteCarlo::addGaussianJitterWithinRange(double num, double percentJitter, double min, double max) {
    float newNum = addGaussianJitter(num, percentJitter);

    if (newNum < min) {
        return min;
    }
    if (newNum > max) {
        return max;
    }

    return newNum;
}
*/
double MonteCarlo::addJitter(double num, double percentJitter, int n) {
    if (num == 0) {
        return 0;
    }

    double fullRange = percentJitter * 2;
    double sums = 0.0;
    
    for (int i = 0; i < n; i++) {
        float random = float(rand() % 1000) / 999.0;
        float jitter = num * (fullRange * random - percentJitter);
        sums = sums + jitter;
    }

    return num + (sums / n);
}

double MonteCarlo::addJitterWithinRange(double num, double percentJitter, double min, double max, int n) {
    float newNum = addJitter(num, percentJitter, n);

    if (newNum < min) {
        return min;
    }
    if (newNum > max) {
        return max;
    }

    return newNum;
}

QList<QList<double>> MonteCarlo::makeJitteredMatrix(QList<QList<double>> matrix, double percentJitter, int n) {
    QList<QList<double>> newMatrix;

    for (int i = 0; i < matrix.size(); i++) {
        newMatrix.append(makeJitteredList(matrix.at(i), percentJitter, n));
    }

    return newMatrix;
}

QList<QList<double>> MonteCarlo::makeJitteredMatrixWithinRange(QList<QList<double>> matrix, double percentJitter, double min, double max, int n) {
    QList<QList<double>> newMatrix;

    for (int i = 0; i < matrix.size(); i++) {
        newMatrix.append(makeJitteredListWithinRange(matrix.at(i), percentJitter, min, max, n));
    }

    return newMatrix;
}

QList<double> MonteCarlo::makeJitteredList(QList<double> list, double percentJitter, int n) {
    QList<double> newList;

    for (int i = 0; i < list.size(); i++) {
        newList.append(addJitter(list.at(i), percentJitter, n));
    }

    return newList;
}

QList<double> MonteCarlo::makeJitteredListWithinRange(QList<double> list, double percentJitter, double min, double max, int n) {
    QList<double> newList;

    for (int i = 0; i < list.size(); i++) {
        newList.append(addJitterWithinRange(list.at(i), percentJitter, min, max, n));
    }

    return newList;
}

QList<QList<double>> MonteCarlo::makeRandomMatrix(int r, int c, double min, double max, int n) {
    QList<QList<double>> matrix;

    for (int i = 0; i < r; i++) {
        matrix.append(makeRandomList(c, min, max, n));
    }

    return matrix;
}

QList<double> MonteCarlo::makeRandomList(int length, double min, double max, int n) {
    QList<double> list;

    for (int i = 0; i < length; i++) {
        list.append(getRandomd(min, max, n));
    }

    return list;
}