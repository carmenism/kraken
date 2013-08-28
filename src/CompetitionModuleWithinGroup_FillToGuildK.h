#ifndef COMPETITIONMODULEWITHINGROUP_FILLTOGUILDK_H
#define COMPETITIONMODULEWITHINGROUP_FILLTOGUILDK_H

// CompetitionModuleWithinGroup_FillToGuildK.h header file

#include "Parameters.h"
#include "ModelComponent.h"

class CompetitionModuleWithinGroup_FillToGuildK : public ModelComponent {

private:


public:
    CompetitionModuleWithinGroup_FillToGuildK(Parameters *paramObj);
    double getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const;
};

#endif // COMPETITIONMODULEWITHINGROUP_FILLTOGUILDK_H
