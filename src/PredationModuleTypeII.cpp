#include "PredationModuleTypeII.h"

PredationModuleTypeII::PredationModuleTypeII(Parameters *paramObj) {

    m_ComponentName = "Type II Predation";
    m_OutputName = " (Predation Losses)";
    initializeOutputMatrix(paramObj);
//    qDebug() << m_OutputName << ": " << m_ComponentName;
}


double PredationModuleTypeII::getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const {

    // From Koen-Alonso, 2007
    double predationLoss = 0.0;

    double Bi = paramObj->getBiomass(speciesIndex, timestep);

    // Determine number of predators (in the general case, number of stocks/species)
    int numGroups = paramObj->getNumberOfSpecies();

    double Bj = 0.0;                            // Predator biomass
    double Bk = 0.0;                            // Other prey biomass;
    double Aij = 0.0;                           // Main prey availability term
    double Akj = 0.0;                           // Other prey availability term
    double h_kj = 0.0;                          // Handling time
    double Sij = 0.0;                           // Spatial overlap with main prey
    double Skj = 0.0;                           // Spatial overlap with other prey
    double lossToPredation = 0.0;
    int i = speciesIndex;                       // Subscript for main prey

    double summedTerm = 0.0;                           // Summed term in denominator
    double denominator = 0.0;
    // For every predator
    for (int j = 0; j < numGroups; j++) {    // Subscript for predator
        // Get Predator Biomass
        Bj = paramObj->getBiomass(j, timestep);

        // Get main prey's availability to the predator
        Aij = paramObj->getAvailability(i,j);

        // Get main prey's spatial overlap with predator
        Sij = paramObj->getSpatialOverlap(i,j);

        // Calculate summed term in demoninator by
        // Looping through each prey of the current predator

        // Reset summed term and denominator
        summedTerm = 0.0;
        denominator = 0.0;
        for (int k = 0; k < numGroups; k++) {
            // Get prey k's biomass
            Bk = paramObj->getBiomass(k, timestep);
            // Get predator/prey availability term
            Akj = paramObj->getAvailability(k,j);
            // Get predator/prey handling time
            h_kj = paramObj->getHandlingTime(k,j);
            // Get predator/prey spatial overlap
            Skj = paramObj->getSpatialOverlap(k,j);
            // Add to running total of summed term
            summedTerm += h_kj * Akj * Bk * Skj;
        }
        // Add 1 to above to finish calculating the demoninator
        denominator = 1 + summedTerm;

        // Calculate loss to predation on prey by predator and
        // Add to running total of loss of predation
        lossToPredation += -1.0 * (Bi * Bj * Aij * Sij) / denominator;
    }

    return lossToPredation;
}
