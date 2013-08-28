#include "HarvestModuleEffortQ.h"

HarvestModuleEffortQ::HarvestModuleEffortQ(Parameters* paramObj) {

    m_ComponentName = "qE";
    m_OutputName = " (Harvest Losses)";
    initializeOutputMatrix(paramObj);
//    qDebug() << m_OutputName << ": " << m_ComponentName;
}


double HarvestModuleEffortQ::getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const {

    // Q and E

    double Bi = paramObj->getBiomass(speciesIndex, timestep);
    double Qi = paramObj->getCatchability(speciesIndex, timestep);
    double Ei = paramObj->getEffort(speciesIndex, timestep);

    double lossToLandings = -1.0 * Bi * Qi * Ei;

    return lossToLandings;

}


