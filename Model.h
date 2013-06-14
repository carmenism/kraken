#ifndef MODEL_H
#define MODEL_H

// Model creation and execution class

#include <QList>
#include <QString>
#include <QDebug>

#include "GrowthModuleLogistic.h"
#include "GrowthModuleLinear.h"
#include "CompetitionModuleWithinGroup.h"
#include "CompetitionModuleBetweenSpeciesNoK.h"
#include "CompetitionModuleBetweenGroup.h"
#include "PredationModuleTypeI.h"
#include "PredationModuleTypeII.h"
#include "PredationModuleTypeIII.h"
#include "HarvestModuleEffortQ.h"
#include "HarvestModuleExploitRate.h"
#include "HarvestModuleCatch.h"
#include "ModelComponent.h"
#include "Parameters.h"
#include "ModelFormParameters.h"

class Model {

private:

    QList<ModelComponent*> m_ModelComponentList;
    ModelComponent* m_growthFormPtr;
    ModelComponent* m_competitionWithinGroupFormPtr;
    ModelComponent* m_competitionBetweenGroupFormPtr;
    ModelComponent* m_predationFormPtr;
    ModelComponent* m_harvestFormPtr;
    ModelFormParameters* m_modelParamsPtr;
    Parameters* m_paramsPtr;

    void initializeModel();



public:

    Model();
    Model(Parameters* paramObj, ModelFormParameters* modelParamObj);
    void runModel();
    void runModel_noOutput();
    void testRun();

};

#endif // MODEL_H
