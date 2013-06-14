#ifndef GROWTHMODULELINEAR_H
#define GROWTHMODULELINEAR_H

#include "ModelComponent.h"

class GrowthModuleLinear : public ModelComponent {

private:

public:
    GrowthModuleLinear(Parameters *paramObj);
    double getModelComponentValue(Parameters *paramObj, int speciesIndex, int timestep) const;
};


#endif // GROWTHMODULELINEAR_H
