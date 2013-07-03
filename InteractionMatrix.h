#ifndef INTERACTIONMATRIX_H
#define INTERACTIONMATRIX_H

// InteractionMatrix.h header file

#include <QList>
#include <QStringList>
#include <QDebug>

class InteractionMatrix {

private:
    QList<QList <double> > m_IntMatrix;

public:
    InteractionMatrix();

    void setMatrix(QList<QList <double> > matrix);
    void clearMatrix();
    QList<QList <double> > getMatrix();
    double getMatrixValue(int i, int j);
    void setMatrixValue(int i, int j, double value);
    void addMatrixValue(int i, double value);
    void printMatrix(QStringList);
    int getNumberRows();
    int getNumberColumns(int rowIndex);

};

#endif // INTERACTIONMATRIX_H
