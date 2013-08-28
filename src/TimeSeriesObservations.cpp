#include "TimeSeriesObservations.h"


TimeSeriesObservations::TimeSeriesObservations() {

    m_tsLength = 0;
}


void TimeSeriesObservations::outputBiomass() {

    outputTS(m_BiomassObservedHash);
}


void TimeSeriesObservations::outputCatch() {

    outputTS(m_CatchObservedHash);
}


void TimeSeriesObservations::outputBiomassMultiplier() {

    outputTS(m_BiomassMultiplierHash);
}


void TimeSeriesObservations::outputTS(QHash<QString, QList<double>* > tsHash) {

    QList<QString> names = tsHash.keys();
    int numNames = names.size();
    QList<double> ts_list;
    int lengthTS;
    QString species;
    for (int i = 0; i < numNames; i++) {
        species = names.at(i);
        ts_list = *(tsHash.value(species));
        lengthTS = ts_list.size();
        for (int j = 0; j < lengthTS; j++) {
//            qDebug() << "Biomass Multiplier TS Name: " << species << biomass_multiplier_TS_list.at(j);
        }
    }
}


void TimeSeriesObservations::setBiomass(QStringList biomassTS) {

    QString name = biomassTS.at(0);

    biomassTS.removeFirst();
    QList<double> *newBiomassTS_list = new QList<double>;
    QString strValue;
    double value;

    int biomassTS_size = biomassTS.size();
    for (int i=0; i < biomassTS_size; i++) {
        strValue = biomassTS.at(i);
        value = strValue.toDouble();
        newBiomassTS_list->append(value);
    }
    m_BiomassObservedHash.insert(name,newBiomassTS_list);
    if (biomassTS_size > m_tsLength) {
        m_tsLength = biomassTS_size;
    }
}


void TimeSeriesObservations::setCatch(QStringList catchTS) {

//    setTS(m_CatchObservedHash, catchTS);
    QString name = catchTS.at(0);

    catchTS.removeFirst();
    QList<double> *newCatchTS_list = new QList<double>;
    QString strValue;
    double value;

    int catchTS_size = catchTS.size();
    for (int i=0; i < catchTS_size; i++) {
        strValue = catchTS.at(i);
        value = strValue.toDouble();
        newCatchTS_list->append(value);
    }
    m_CatchObservedHash.insert(name,newCatchTS_list);
}


void TimeSeriesObservations::setBiomassMultiplier(QStringList biomassMultiplierTS) {

//    setTS(m_BiomassMultiplierHash, biomassMultiplierTS);
    QString name = biomassMultiplierTS.at(0);

    biomassMultiplierTS.removeFirst();
    QList<double> *newBiomassMultiplierTS_list = new QList<double>;
    QString strValue;
    double value;

    int biomassMultiplierTS_size = biomassMultiplierTS.size();
    for (int i=0; i < biomassMultiplierTS_size; i++) {
        strValue = biomassMultiplierTS.at(i);
        value = strValue.toDouble();
        newBiomassMultiplierTS_list->append(value);
    }
    m_BiomassMultiplierHash.insert(name,newBiomassMultiplierTS_list);
    if (biomassMultiplierTS_size > m_tsLength) {
        m_tsLength = biomassMultiplierTS_size;
    }
}


void TimeSeriesObservations::setTS(QHash<QString, QList<double> *> tsHash, QStringList tsList) {

    QString name = tsList.at(0);

    tsList.removeFirst();
    QList<double> *new_tsList = new QList<double>;
    QString strValue;
    double value;

    int tsSize = tsList.size();
    for (int i=0; i < tsSize; i++) {
        strValue = tsList.at(i);
        value = strValue.toDouble();
        new_tsList->append(value);
    }
    tsHash.insert(name,new_tsList);
    if (tsSize > m_tsLength) {
        m_tsLength = tsSize;
    }
}


double TimeSeriesObservations::getBiomass(QString speciesName, int index) {

    QList<double> biomassTSlist = *(m_BiomassObservedHash.value(speciesName));
    double biomass = biomassTSlist.at(index);

    return biomass;

}


double TimeSeriesObservations::getCatch(QString speciesName, int index) {

    QList<double> catchTSlist = *(m_CatchObservedHash.value(speciesName));
    double catchVal = catchTSlist.at(index);

    return catchVal;

}


double TimeSeriesObservations::getBiomassMultiplier(QString speciesName, int index) {

    QList<double> biomassMultiplierTSlist = *(m_BiomassMultiplierHash.value(speciesName));
    double biomassMultiplier = biomassMultiplierTSlist.at(index);

    return biomassMultiplier;

}


int TimeSeriesObservations::getTSlength() {

    return m_tsLength;
}
