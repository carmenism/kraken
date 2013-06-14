#ifndef HARVESTMODULEEFFORTQ_H
#define HARVESTMODULEEFFORTQ_H

#include "ModelComponent.h"

class HarvestModuleEffortQ : public ModelComponent {

private:
    double E;   // Effort
    double q;   // Catchability

public:
    HarvestModuleEffortQ(Parameters *paramObj);
    double getModelComponentValue(Parameters *paramObj, int speciesIndex, int timestep) const;
};

#endif // HARVESTMODULEEFFORTQ_H
