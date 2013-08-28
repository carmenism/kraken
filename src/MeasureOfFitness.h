#ifndef MEASUREOFFITNESS_H
#define MEASUREOFFITNESS_H

// MeasureOfFitness.h header file

#include "Parameters.h"
#include "TimeSeriesObservations.h"

class MeasureOfFitness {

private:

    double m_MAE;
    double m_RMSE;
    double m_RMSE_modified;

    Parameters* m_paramsPtr;
    TimeSeriesObservations* m_tsObservationsPtr;


public:
    MeasureOfFitness();
    void calculateMeasuresOfFitness(Parameters* paramObj, TimeSeriesObservations* tsObservationsObj);
    void outputMeasuresOfFitness();
    double getMAE();
    double getRMSE();
    double getRMSE_modified();
};


#endif // MEASUREOFFITNESS_H
