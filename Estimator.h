#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include "Parameters.h"
#include "TimeSeriesObservations.h"
#include "MersenneTwister.h"
#include <QDebug>
#include <QString>


class Estimator {

protected:

    Parameters* m_paramsPtr;
    TimeSeriesObservations* m_timeSeriesPtr;
    QString m_Name;

public:

    virtual void estimateParameters();
    QString getEstimatorName();
};

#endif // ESTIMATOR_H
