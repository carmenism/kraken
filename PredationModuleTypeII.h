#ifndef PREDATIONMODULETYPEII_H
#define PREDATIONMODULETYPEII_H

// PredationModuleTypeII.h header file

#include "Parameters.h"
#include "ModelComponent.h"

class PredationModuleTypeII : public ModelComponent {

private:

    double a;               // availability
    double h;               // handling time

public:
    PredationModuleTypeII(Parameters *paramObj);
    double getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const;
};

#endif // PREDATIONMODULETYPEII_H
