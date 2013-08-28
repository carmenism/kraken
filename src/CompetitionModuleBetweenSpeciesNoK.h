#ifndef COMPETITIONMODULEBETWEENSPECIESNOK_H
#define COMPETITIONMODULEBETWEENSPECIESNOK_H

// CompetitionModuleBetweenSpeciesNoK.h header file

#include "Parameters.h"
#include "ModelComponent.h"

class CompetitionModuleBetweenSpeciesNoK : public ModelComponent {

private:


public:
    CompetitionModuleBetweenSpeciesNoK(Parameters *paramObj);
    double getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const;
};

#endif // COMPETITIONMODULEBETWEENSPECIESNOK_H
