#include "Fitter.h"

Fitter::Fitter() {

}


Fitter::Fitter(Parameters* paramObj, TimeSeriesObservations* tsObj, QString estimatorType) {

    m_paramsPtr = paramObj;
    m_timeSeriesPtr = tsObj;
    m_estimationType = estimatorType.toUpper();
}


void Fitter::initializeFitter() {

    if (m_estimationType == "GENETIC ALGORITHM") {
        GA_Estimator m_GA_Estimator(m_paramsPtr, m_timeSeriesPtr);
        m_estimatorPtr = &m_GA_Estimator;
        qDebug() << "Estimator: Genetic Algorithm";
    } else if (m_estimationType == "MAXIMUM LIKELIHOOD") {
        qDebug() << "Estimator: Maximum Likelihood";
    } else if (m_estimationType == "LEAST SQUARES") {
        qDebug() << "Estimator: Least Squares";
    }
}


void Fitter::runFitter() {

    initializeFitter();

    qDebug() << "Fitting....";
    m_estimatorPtr->estimateParameters();
}
