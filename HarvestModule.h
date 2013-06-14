#ifndef HARVESTMODULE_H
#define HARVESTMODULE_H

// HarvestModule.h

#include "Parameters.h"
class HarvestModule {

protected:


public:

    virtual double getHarvestLoss(Parameters* paramObj, int speciesIndex) const = 0;

};

#endif // HARVESTMODULE_H
