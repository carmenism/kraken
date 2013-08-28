#ifndef PREDATIONMODULE_H
#define PREDATIONMODULE_H

// PredationModule.h header file

#include "Parameters.h"

class PredationModule {

protected:

    double B;                   // Biomass of prey
    double P;                   // Biomass of predator

    double m_predationLoss;

public:
    virtual double getPredationLoss(Parameters* paramObj, int speciesIndex) const = 0;

};

#endif // PREDATIONMODULE_H
