#include "HarvestModuleCatch.h"

HarvestModuleCatch::HarvestModuleCatch(Parameters *paramObj) {

    m_ComponentName = "Catch";
    m_OutputName = " (Harvest Losses)";
    initializeOutputMatrix(paramObj);
}


double HarvestModuleCatch::getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const {

    // C
    double Ci = paramObj->getCatch(speciesIndex, timestep);

    double lossToLandings = -1.0 * Ci;

    return lossToLandings;

}

