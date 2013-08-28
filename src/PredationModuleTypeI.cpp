#include "PredationModuleTypeI.h"

PredationModuleTypeI::PredationModuleTypeI(Parameters *paramObj) {

    m_ComponentName = "Type I Predation";
    m_OutputName = " (Predation Losses)";
    initializeOutputMatrix(paramObj);
//    qDebug() << m_OutputName << ": " << m_ComponentName;
}


double PredationModuleTypeI::getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const {

    double Bi = paramObj->getBiomass(speciesIndex, timestep);

    // Determine number of predators (in the general case, number of stocks/species)
    int numPredators = paramObj->getNumberOfSpecies();

    // For every predator
    double Bj = 0.0;
    double Aij = 0.0;                       // Availability
    double Sij = 0.0;                       // Spatial overlap
    double lossToPredation = 0.0;

    int i = speciesIndex;                   // Subscript for prey
    for (int j = 0; j < numPredators; j++) {    // Subscript for predator
        // Get Predator Biomass
        Bj = paramObj->getBiomass(j, timestep);

        // Get prey's availability to the predator
        Aij = paramObj->getAvailability(i,j);

        // Get spatial overlap between predator and prey
        Sij = paramObj->getSpatialOverlap(i,j);

        // Calculate loss to predation on prey by predator and
        // Add to running total of loss of predation
        lossToPredation += -1.0 * Bi * Aij * Bj * Sij;
    }

    return lossToPredation;

}


//void PredationModuleTypeI::updateLosses(Parameters* paramObj, int speciesIndex, double lossValue) const {

//    paramObj->updatePredationLosses(speciesIndex, lossValue);
//    updateOutputMatrix(speciesIndex, lossValue);
//}


//void PredationModuleTypeI::outputResults(QTextStream& outputStream, QList<QString> speciesList) const {

//    outputLosses(outputStream, speciesList);

//}
