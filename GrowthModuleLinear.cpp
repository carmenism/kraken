
#include "GrowthModuleLinear.h"

GrowthModuleLinear::GrowthModuleLinear(Parameters *paramObj) {

    m_ComponentName = "Linear Growth";
    m_OutputName = " (Linear Growth)";
    initializeOutputMatrix(paramObj);
//    qDebug() << m_OutputName << ": " << m_ComponentName;
}


double GrowthModuleLinear::getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const {

    double Ri = paramObj->getGrowthRate(speciesIndex);
    double Bi = paramObj->getBiomass(speciesIndex, timestep);


    double growthIncrement = (Ri * Bi);

//    qDebug() << "Biomass: " << Bi << "Growth Rate: " << Ri << "K: " << Ki;
//    qDebug() << "Growth Increment: " << growthIncrement;

    return growthIncrement;

}

