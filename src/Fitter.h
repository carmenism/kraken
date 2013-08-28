#ifndef FITTER_H
#define FITTER_H

// Fitter class

#include <QDebug>
#include <QString>

#include "Parameters.h"
#include "TimeSeriesObservations.h"
#include "Estimator.h"
#include "GA_Estimator.h"

class Fitter {

private:

    QString m_estimationType;
    Parameters* m_paramsPtr;
    TimeSeriesObservations* m_timeSeriesPtr;

    GA_Estimator m_GA_Estimator;
    Estimator* m_estimatorPtr;


public:

    Fitter();
    Fitter(Parameters* paramObj, TimeSeriesObservations* tsObj, QString estimationType);
    void initializeFitter();
    void runFitter();

};


#endif // FITTER_H
