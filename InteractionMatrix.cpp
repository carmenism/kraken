#include "InteractionMatrix.h"
#include <iostream>

typedef QList<QList <double> > Int_Matrix;

InteractionMatrix::InteractionMatrix() {

}

void InteractionMatrix::setMatrix(Int_Matrix matrix) {
    m_IntMatrix = matrix;
}

Int_Matrix InteractionMatrix::getMatrix() {
    return m_IntMatrix;
}

void InteractionMatrix::printMatrix() {
    for (int i = 0; i < m_IntMatrix.size(); i++) {
        for (int j = 0; j < m_IntMatrix.at(i).size(); j++) {
            std::cout << m_IntMatrix.at(i).at(j) << " ";
        }

        std::cout << "\n";
    }
}

void InteractionMatrix::printMatrix(const QStringList typeList) {
    int x = m_IntMatrix.size();
    int y = m_IntMatrix.at(0).size();
    qDebug() << x << "   " << y;
    for (int i = 0; i < x; i++) {
        qDebug() << "Name: " << typeList.at(i);
        for (int j = 0; j < y; j++) {
            qDebug() << m_IntMatrix.at(i).at(j);
        }
    }
    QString str;
    foreach (str, typeList) {
        qDebug() << "Name: " << str;
    }
}


void InteractionMatrix::clearMatrix() {

    m_IntMatrix.clear();
}

double InteractionMatrix::getMatrixValue(int i, int j) {

    return m_IntMatrix.at(i).at(j);
}

void InteractionMatrix::setMatrixValue(int i, int j, double value) {
    if (i >= 0 && i < m_IntMatrix.size() 
        && j >= 0 && j < m_IntMatrix.at(i).size()) {
        m_IntMatrix[i][j] = value;
    }
}

void InteractionMatrix::addMatrixValue(int i, double value) {

    m_IntMatrix[i].append(value);
}

int InteractionMatrix::getNumberRows() {
    return m_IntMatrix.size();
}

int InteractionMatrix::getNumberColumns(int rowIndex) {
    return m_IntMatrix.at(rowIndex).size();
}


