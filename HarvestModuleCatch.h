#ifndef HARVESTMODULECATCH_H
#define HARVESTMODULECATCH_H

#include "ModelComponent.h"

class HarvestModuleCatch : public ModelComponent {

private:
    double C;   // Catch

public:
    HarvestModuleCatch(Parameters *paramObj);
    double getModelComponentValue(Parameters *paramObj, int speciesIndex, int timestep) const;
};


#endif // HARVESTMODULECATCH_H
