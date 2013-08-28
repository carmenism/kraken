#ifndef PREDATIONMODULETYPEI_H
#define PREDATIONMODULETYPEI_H

// PredationModuleTypeI.h header file

#include "Parameters.h"
#include "ModelComponent.h"

class PredationModuleTypeI : public ModelComponent {

private:


public:
    PredationModuleTypeI(Parameters *paramObj);
    double getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const;
};

#endif // PREDATIONMODULETYPEI_H
