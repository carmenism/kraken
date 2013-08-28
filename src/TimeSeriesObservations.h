#ifndef TIMESERIESOBSERVATIONS_H
#define TIMESERIESOBSERVATIONS_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QDebug>
#include <QHash>
#include "InteractionMatrix.h"


class TimeSeriesObservations {

private:
    QHash<QString, QList<double>* > m_BiomassObservedHash;
    QHash<QString, QList<double>* > m_BiomassMultiplierHash;
    QHash<QString, QList<double>* > m_CatchObservedHash;
    int m_tsLength;

    void outputTS(QHash<QString, QList<double>* > tsHash);
    void setTS(QHash<QString, QList<double>* > tsHash, QStringList tsList);



public:
    TimeSeriesObservations();
    void outputBiomass();
    void outputCatch();
    void outputBiomassMultiplier();

    void setBiomass(QStringList);
    void setCatch(QStringList);
    void setBiomassMultiplier(QStringList);

    int getTSlength();

    double getBiomass(QString speciesName, int index);
    double getCatch(QString speciesName, int index);
    double getBiomassMultiplier(QString speciesName, int index);
};

#endif // TIMESERIESOBSERVATIONS_H
