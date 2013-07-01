#include "Model.h"

Model::Model() {

}

Model::Model(Parameters *paramObj, ModelFormParameters *modelParamObj) {

    m_modelParamsPtr = modelParamObj;
    m_paramsPtr = paramObj;
}


void Model::initializeModel() {

    m_paramsPtr->initializeListsAndTS(0);

    // Set Growth Form
   QString growthFormStr = m_modelParamsPtr->getGrowthForm();
   if (growthFormStr == "LOGISTIC") {
       GrowthModuleLogistic* growthModule = new GrowthModuleLogistic(m_paramsPtr);
       m_growthFormPtr = growthModule;
//       qDebug() << "GrowthForm = LOGISTIC";
   } else if (growthFormStr == "LINEAR") {
       GrowthModuleLinear* growthModule = new GrowthModuleLinear(m_paramsPtr);
       m_growthFormPtr = growthModule;
   } else {
//       qDebug() << "GrowthForm = UNKNOWN";
   }
   m_ModelComponentList.append(m_growthFormPtr);


   // Set within group/between species competition growth form
   QString competitionWithinGroupStr = m_modelParamsPtr->getwgcForm();
   if (competitionWithinGroupStr == "MS-PROD") {
       CompetitionModuleWithinGroup* competitionModule = new CompetitionModuleWithinGroup(m_paramsPtr);
       m_competitionWithinGroupFormPtr = competitionModule;
       m_ModelComponentList.append(m_competitionWithinGroupFormPtr);
   } else if (competitionWithinGroupStr == "NO_K") {
       CompetitionModuleBetweenSpeciesNoK* competitionModule = new CompetitionModuleBetweenSpeciesNoK(m_paramsPtr);
       m_competitionWithinGroupFormPtr = competitionModule;
       m_ModelComponentList.append(m_competitionWithinGroupFormPtr);
   } else {
//       qDebug() << "Competition Between Species Form = UNKNOWN";
   }

   QString competitionBetweenGroupStr = m_modelParamsPtr->getbgcForm();
   if (competitionBetweenGroupStr == "MS-PROD") {
       CompetitionModuleBetweenGroup* competitionModuleBetweenGroup = new CompetitionModuleBetweenGroup(m_paramsPtr);
       m_competitionBetweenGroupFormPtr = competitionModuleBetweenGroup;
       m_ModelComponentList.append(m_competitionBetweenGroupFormPtr);
   } else {
//       qDebug() << "Competition Between Group Form = UNKNOWN";
   }

   // Set Predation Form
   QString predationFormStr = m_modelParamsPtr->getPredationForm();
   if (predationFormStr == "TYPE I") {
       PredationModuleTypeI* predationModule = new PredationModuleTypeI(m_paramsPtr);
       m_predationFormPtr = predationModule;
       m_ModelComponentList.append(m_predationFormPtr);
   } else if (predationFormStr == "TYPE II") {
       PredationModuleTypeII* predationModule = new PredationModuleTypeII(m_paramsPtr);
       m_predationFormPtr = predationModule;
       m_ModelComponentList.append(m_predationFormPtr);
   } else if (predationFormStr == "TYPE III") {
       PredationModuleTypeIII* predationModule = new PredationModuleTypeIII(m_paramsPtr);
       m_predationFormPtr = predationModule;
       m_ModelComponentList.append(m_predationFormPtr);
   } else {
 //      qDebug() << "PredationForm = UNKNOWN";
   }


   // Set Harvest Form
   QString harvestFormStr = m_modelParamsPtr->getHarvestForm();
   if (harvestFormStr == "H") {
       HarvestModuleExploitRate* harvestModule = new HarvestModuleExploitRate(m_paramsPtr);
       m_harvestFormPtr = harvestModule;
       m_ModelComponentList.append(m_harvestFormPtr);
   } else if (harvestFormStr == "QE") {
       HarvestModuleEffortQ* harvestModule = new HarvestModuleEffortQ(m_paramsPtr);
       m_harvestFormPtr = harvestModule;
       m_ModelComponentList.append(m_harvestFormPtr);
   } else if (harvestFormStr == "CATCH") {
       HarvestModuleCatch* harvestModule = new HarvestModuleCatch(m_paramsPtr);
       m_harvestFormPtr = harvestModule;
       m_ModelComponentList.append(m_harvestFormPtr);
   } else {
//       qDebug() << "HarvestModule = UNKNOWN";
   }



}

void Model::runModel() {
    m_paramsPtr->clearBiomassMatrix();

    initializeModel();

    // for every time step
    int runLength = m_modelParamsPtr->getRunLength();
    int timeStep = m_modelParamsPtr->getTimeStep();
    ModelComponent* component;
    QString componentName = "";
    QString outputName = "";
    double dN_fromModelComponentValue = 0.0;
    int numSpecies = m_paramsPtr->getNumberOfSpecies();

    m_paramsPtr->setGuildBiomass(0);
//    m_paramsPtr->initializeOutputs();

    for (int time = 0; time <= runLength-1; time += timeStep) {
        // add up the results of every component in the model by
        for (int speciesIndex = 0; speciesIndex < numSpecies; speciesIndex++) {
            double dN = 0.0;
            foreach (component, m_ModelComponentList) {
                componentName = component->getComponentName();
                outputName = component->getOutputName();
//                qDebug() << "Time Step: " << time << "; Component Name: " << componentName;
                dN_fromModelComponentValue = component->getModelComponentValue(m_paramsPtr, speciesIndex, time);
//                qDebug() << time << " : " << speciesIndex << componentName << " dN : " << dN_fromModelComponentValue;

                // Modify this so that each model component updates the appropriate lists...
                component->updateLosses(m_paramsPtr, speciesIndex, dN_fromModelComponentValue);
                dN += dN_fromModelComponentValue;
            }
            m_paramsPtr->updateBiomass(speciesIndex, time, dN);
        }
        m_paramsPtr->setGuildBiomass(time);
    }
    m_paramsPtr->outputResults(m_ModelComponentList);
        // calculating the change to the old biomass, using a 4th order runge kutta method
        // adding the change to the old biomass
        // which becomes the new biomass

}


void Model::runModel_noOutput() {

    initializeModel();

    // for every time step
    int runLength = m_modelParamsPtr->getRunLength();
    int timeStep = m_modelParamsPtr->getTimeStep();
    ModelComponent* component;
    QString componentName = "";
    QString outputName = "";
    double dN_fromModelComponentValue = 0.0;
    int numSpecies = m_paramsPtr->getNumberOfSpecies();

    m_paramsPtr->setGuildBiomass(0);
//    m_paramsPtr->initializeOutputs();

    for (int time = 0; time <= runLength-1; time += timeStep) {
        // add up the results of every component in the model by
        for (int speciesIndex = 0; speciesIndex < numSpecies; speciesIndex++) {
            double dN = 0.0;
            foreach (component, m_ModelComponentList) {
                componentName = component->getComponentName();
                outputName = component->getOutputName();
                dN_fromModelComponentValue = component->getModelComponentValue(m_paramsPtr, speciesIndex, time);
//                qDebug() << componentName << " dN : " << dN_fromModelComponentValue;

                // Modify this so that each model component updates the appropriate lists...
//                component->updateLosses(m_paramsPtr, speciesIndex, dN_fromModelComponentValue);
                dN += dN_fromModelComponentValue;
            }
            m_paramsPtr->updateBiomass(speciesIndex, time, dN);
        }
//        m_paramsPtr->setGuildBiomass(time);
    }
        // calculating the change to the old biomass, using a 4th order runge kutta method
        // adding the change to the old biomass
        // which becomes the new biomass

}
