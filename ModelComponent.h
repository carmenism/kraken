#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

// ModelComponent.h

#include "Parameters.h"
#include "InteractionMatrix.h"
#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QList>

class ModelComponent {

protected:

    QString m_ComponentName;
    QString m_OutputName;

    void outputLosses(QTextStream& outputStream, QList<QString> speciesList);
    void initializeOutputMatrix(Parameters* paramObj);
    void updateOutputMatrix(int speciesIndex, double lossValue);



public:

    virtual double getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const = 0;
//    virtual void updateLosses(Parameters* paramObj, int speciesIndex, double lossValue) const = 0;
    void updateLosses(Parameters* paramObj, int speciesIndex, double lossValue);
    void outputResults(QTextStream& outputStream, QList<QString> speciesList);
    QString getComponentName();
    QString getOutputName();
    InteractionMatrix m_OutputMatrix;

    //QString getComponentName() { return m_ComponentName; }
    //QString getOutputName() { return m_OutputName; }
    QList<QList<double>> getOutputMatrix();
};

#endif // MODELCOMPONENT_H
