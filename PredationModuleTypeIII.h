#ifndef PREDATIONMODULETYPEIII_H
#define PREDATIONMODULETYPEIII_H

// PredationModuleTypeIII.h header file

#include "Parameters.h"
#include "ModelComponent.h"

class PredationModuleTypeIII : public ModelComponent {

private:

    double a;           // availability
    double h;           // handling time
    double n;           // exponent

public:
    PredationModuleTypeIII(Parameters *paramObj);
    double getModelComponentValue(Parameters* paramObj, int speciesIndex, int timestep) const;
};

#endif // PREDATIONMODULETYPEIII_H
