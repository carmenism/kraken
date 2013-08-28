#ifndef COMPETITIONMODULEBETWEENGROUP_H
#define COMPETITIONMODULEBETWEENGROUP_H

// CompetitionModuleBetweenGroup.h header file

#include "Parameters.h"
#include "ModelComponent.h"

class CompetitionModuleBetweenGroup : public ModelComponent {

private:


public:
    CompetitionModuleBetweenGroup(Parameters *paramObj);
    double getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const ;
};

#endif // COMPETITIONMODULEBETWEENGROUP_H
