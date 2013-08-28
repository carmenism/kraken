#ifndef COMPETITIONMODULEWITHINGROUP_H
#define COMPETITIONMODULEWITHINGROUP_H

// CompetitionModuleWithinGroup.h header file

#include "Parameters.h"
#include "ModelComponent.h"

class CompetitionModuleWithinGroup : public ModelComponent {

private:


public:
    CompetitionModuleWithinGroup(Parameters *paramObj);
    double getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const;
};

#endif // COMPETITIONMODULEWITHINGROUP_H
