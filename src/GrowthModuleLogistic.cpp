#include "GrowthModuleLogistic.h"

GrowthModuleLogistic::GrowthModuleLogistic(Parameters *paramObj) {

    m_ComponentName = "Logistic Growth";
    m_OutputName = " (Logistic Growth)";
    initializeOutputMatrix(paramObj);
//    qDebug() << m_OutputName << ": " << m_ComponentName;
}


double GrowthModuleLogistic::getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const {

    double Ri = paramObj->getGrowthRate(speciesIndex);
    double Bi = paramObj->getBiomass(speciesIndex, timestep);
    double Ki = paramObj->getSpeciesK(speciesIndex);


    double growthIncrement = (Ri * Bi) * (1 - (Bi / Ki));

//    qDebug() << "Biomass: " << Bi << "Growth Rate: " << Ri << "K: " << Ki;
//    qDebug() << "Growth Increment: " << growthIncrement;

    return growthIncrement;

}


