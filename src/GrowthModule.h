#ifndef GROWTHMODULE_H
#define GROWTHMODULE_H

// GrowthModule.h header file

#include "Parameters.h"

class GrowthModule {

protected:



public:
    virtual double getGrowthIncrement(Parameters* paramObj, int speciesIndex) const = 0;

};

#endif // GROWTHMODULE_H
