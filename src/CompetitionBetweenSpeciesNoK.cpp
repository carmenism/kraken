#include "CompetitionModuleBetweenSpeciesNoK.h"

CompetitionModuleBetweenSpeciesNoK::CompetitionModuleBetweenSpeciesNoK(Parameters* paramObj) {

    m_ComponentName = "Comp Between Species No K";
    m_OutputName = " (Competition Between Species)";
    initializeOutputMatrix(paramObj);
//    qDebug() << m_OutputName << ": " << m_ComponentName;
}


double CompetitionModuleBetweenSpeciesNoK::getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const {

    // Get growth, carrying capacity, and biomass of current species
    int i = speciesIndex;
    double Bi = paramObj->getBiomass(i, timestep);

    // Initialize competition coefficient and biomass for other species
    double compCoefficient = 0.0;
    double Bj = 0.0;

    double Sij = 0.0;           // Spatial overlap between i and j

    // Initialize summed competition term
    double summedTerm = 0.0;

    // For each other species
    int numGroups = paramObj->getNumberOfSpecies();

    for (int j = 0; j < numGroups; j++) {
        // get the interaction coefficient

        compCoefficient = paramObj->getWithinGuildCompParam(i,j);
        // get the interaction species biomass
        Bj = paramObj->getBiomass(j, timestep);

        // get the spatial overlap
        Sij = paramObj->getSpatialOverlap(i,j);

        // Update summed term
        summedTerm += compCoefficient * Bj * Sij;
    }

    // calculate loss to biomass from competition
    double lossToWithinGroupCompetition = -1.0 * (Bi * summedTerm);

    return lossToWithinGroupCompetition;

}

