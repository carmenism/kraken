#ifndef GROWTHMODULELOGISTIC_H
#define GROWTHMODULELOGISTIC_H

#include "ModelComponent.h"

class GrowthModuleLogistic : public ModelComponent {

private:

public:
    GrowthModuleLogistic(Parameters *paramObj);
    double getModelComponentValue(Parameters *paramObj, int speciesIndex, int timestep) const;
};

#endif // GROWTHMODULELOGISTIC_H
