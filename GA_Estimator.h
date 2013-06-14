#ifndef GA_ESTIMATOR_H
#define GA_ESTIMATOR_H

// GA_Estimator.h header file

#ifdef _MSC_VER 
    #include <float.h>
    #include <cmath>
    #include <math.h>
#else 
    #include <cmath>
#endif

#include "Parameters.h"
#include "TimeSeriesObservations.h"
#include "ModelFormParameters.h"
#include "MeasureOfFitness.h"
#include "Model.h"
#include "MersenneTwister.h"
#include <QString>

class GA_Estimator {

private:
    int isnan_local(double x) { 
        #ifdef _MSC_VER
            return _isnan(x);
        #else
            return std::isnan(x);
        #endif
    }

    int isinf_local(double x) { 
        #ifdef _MSC_VER
            return !_finite(x);
        #else
            return std::isinf(x);
        #endif
    }


    QString m_OutputFilename;
    QString m_OutputFilenameBest;
    int m_generation;
    double m_mutationRate;
    int m_maxGenerations;
    double m_populationSize;

    Parameters* m_paramsPtr;
    TimeSeriesObservations* m_timeSeriesPtr;
    ModelFormParameters* m_modelFormPtr;
    MeasureOfFitness m_measureOfFitnessObj;

    QList<Parameters> m_chromosomeParametersList;
    QList<double> m_chromosomeFitnessList;
    QList<double> m_chromosomeSelectionProbabilityList;
    QList<int> m_rouletteWheelList;
    QList<double> m_chromosomeRMSElist;

    QFile m_outfile;
    QTextStream m_textStreamOut;

    void initialize_GA_params();
    void initializeOutputFile();

    void createPopulation();
    void calculateFitness();
    void eliminateUnfit();
    void calculateSelectionProbabilities();
    void createNextGeneration();
    void rouletteSelection();
    void tournamentSelection();

    void outputResults();
    void appendResults();

public:
    GA_Estimator();
    GA_Estimator(Parameters* ptr_paramObj, TimeSeriesObservations *ptr_tsObservationsObj, ModelFormParameters* ptr_modelParamters);
    ~GA_Estimator();

    void setSaveFilenames(QString filename, QString filenameBest);

    void estimateParameters();

};


#endif // GA_ESTIMATOR_H
