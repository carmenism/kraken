#include "MeasureOfFitness.h"
#include "math.h"

MeasureOfFitness::MeasureOfFitness() {

    m_MAE = 0.0;
    m_RMSE = 0.0;
    m_RMSE_modified = 0.0;
}


void MeasureOfFitness::calculateMeasuresOfFitness(Parameters* paramObj, TimeSeriesObservations* tsObj) {

    int numComparisons = tsObj->getTSlength();
    int numSpecies = paramObj->getNumberOfSpecies();
//    qDebug() << "Number of Species = " << numSpecies;

    double modelBiomassValue = 0.0;
    double tsBiomassValue = 0.0;
    double tsBiomassMultiplierValue = 0.0;
    double difference = 0.0;
    double MAE_term = 0.0;
    double RMSE_term = 0.0;
    double RMSE_modified_term = 0.0;

    QString speciesName;
    for (int i = 0; i < numComparisons; i++) {

        for (int j = 0; j < numSpecies; j++) {
            speciesName = paramObj->getSpeciesName(j);
//            qDebug() << "Species Name: " << speciesName;
            modelBiomassValue = paramObj->getBiomass(speciesName, i);
//            qDebug() << "Model Biomass Value: " << modelBiomassValue;
            tsBiomassValue = tsObj->getBiomass(speciesName, i);
//            qDebug() << "Observed Biomass Value: " << tsBiomassValue;
            tsBiomassMultiplierValue = tsObj->getBiomassMultiplier(speciesName, i);
//            qDebug () << "J: " << j << speciesName << " : " << "Model Value: " << modelBiomassValue << " ;    TS Value: " << tsBiomassValue << " ; Multiplier Value: " << tsBiomassMultiplierValue;

            // Get absolute value of difference
            difference = tsBiomassValue - modelBiomassValue;
            if (difference < 0) {
                difference *= -1;
            }

            MAE_term += difference;
            RMSE_term += difference * difference;
            RMSE_modified_term += difference * difference * tsBiomassMultiplierValue;
        }
    }

    int numObservations = numComparisons * numSpecies;

    m_MAE = MAE_term / numObservations;
    m_RMSE = sqrt(RMSE_term / numObservations);
    m_RMSE_modified = sqrt(RMSE_modified_term / numObservations);

}


void MeasureOfFitness::outputMeasuresOfFitness() {

//    qDebug() << "MAE: " << m_MAE;
//    qDebug() << "RMSE: " << m_RMSE;
}


double MeasureOfFitness::getMAE() {
    return m_MAE;
}


double MeasureOfFitness::getRMSE() {
    return m_RMSE;
}


double MeasureOfFitness::getRMSE_modified() {
    return m_RMSE_modified;
}
