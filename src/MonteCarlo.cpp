#include "MonteCarlo.h"
#include "Parameters.h"
#include <stdlib.h>
#include <time.h>

MonteCarlo::MonteCarlo() {
    srand(time(NULL));

    numberSimulations = 100;
}

MonteCarlo::~MonteCarlo() {
    
}

int MonteCarlo::getRandomi(int min, int max) {
    return rand() % max + min;
}

double MonteCarlo::getRandomd(double min, double max) {
    double range = max - min;

    return min + (double) rand() / ((double) RAND_MAX / range);
}

double MonteCarlo::addJitter(double num, double percentJitter) {
    if (num == 0) {
        return 0;
    }

    float fullRange = percentJitter * 2;
    float random = float(rand() % 1000) / 999.0;

    return num + num * (fullRange * random - percentJitter);
}

double MonteCarlo::addJitterWithinRange(double num, double percentJitter, double min, double max) {
    float newNum = addJitter(num, percentJitter);

    if (newNum < min) {
        return min;
    }
    if (newNum > max) {
        return max;
    }

    return newNum;
}

QList<QList<double>> MonteCarlo::makeJitteredMatrix(QList<QList<double>> matrix, double percentJitter) {
    QList<QList<double>> newMatrix;

    for (int i = 0; i < matrix.size(); i++) {
        newMatrix.append(makeJitteredList(matrix.at(i), percentJitter));
    }

    return newMatrix;
}

QList<QList<double>> MonteCarlo::makeJitteredMatrixWithinRange(QList<QList<double>> matrix, double percentJitter, double min, double max) {
    QList<QList<double>> newMatrix;

    for (int i = 0; i < matrix.size(); i++) {
        newMatrix.append(makeJitteredListWithinRange(matrix.at(i), percentJitter, min, max));
    }

    return newMatrix;
}

QList<double> MonteCarlo::makeJitteredList(QList<double> list, double percentJitter) {
    QList<double> newList;

    for (int i = 0; i < list.size(); i++) {
        newList.append(addJitter(list.at(i), percentJitter));
    }

    return newList;
}

QList<double> MonteCarlo::makeJitteredListWithinRange(QList<double> list, double percentJitter, double min, double max) {
    QList<double> newList;

    for (int i = 0; i < list.size(); i++) {
        newList.append(addJitterWithinRange(list.at(i), percentJitter, min, max));
    }

    return newList;
}

QList<QList<double>> MonteCarlo::makeRandomMatrix(int r, int c, double min, double max) {
    QList<QList<double>> matrix;

    for (int i = 0; i < r; i++) {
        matrix.append(makeRandomList(c, min, max));
    }

    return matrix;
}

QList<double> MonteCarlo::makeRandomList(int length, double min, double max) {
    QList<double> list;

    for (int i = 0; i < length; i++) {
        list.append(getRandomd(min, max));
    }

    return list;
}