#include "ModelComponent.h"

QString ModelComponent::getComponentName() {
    return m_ComponentName;
}


QString ModelComponent::getOutputName() {
    return m_OutputName;
}


void ModelComponent::initializeOutputMatrix(Parameters* paramObj) {

    QList<QList<double> > *outputMatrix = new QList<QList<double> >;
    int numSpecies = paramObj->getNumberOfSpecies();
    for (int i = 0; i < numSpecies; i++) {
        QList<double> *newList = new QList<double>;
        newList->append(0.0);
        outputMatrix->append(*newList);
    }
    m_OutputMatrix.setMatrix(*outputMatrix);
}


void ModelComponent::updateLosses(Parameters* paramObj, int speciesIndex, double lossValue) {

    m_OutputMatrix.addMatrixValue(speciesIndex, lossValue);
}


void ModelComponent::outputResults(QTextStream& outputStream, QList<QString> speciesList) {

    outputStream << "\n";
    outputStream << "Time Step,";
    QString species;
    foreach (species, speciesList) {
        outputStream << species << m_OutputName << ",";
    }

    outputStream << "\n";

    QList<QList <double> > outputMatrix = m_OutputMatrix.getMatrix();
    int x = speciesList.size();
    int y = outputMatrix.at(0).size();
    for (int i = 0; i < y; i++) {
        outputStream << i << ",";
        for (int j = 0; j < x; j++) {
            outputStream << outputMatrix.at(j).at(i) << ",";
        }
        outputStream << "\n";
    }

    outputStream << "\n";
}

QList<QList<double>> ModelComponent::getOutputMatrix() {
    return m_OutputMatrix.getMatrix();
}

