#ifndef _PREDATIONARC_H
#define _PREDATIONARC_H

class SmallMultiple;
class PlotManager;

#include "InterSpeciesArc.h"

class PredationArc : public InterSpeciesArc {
public:
    PredationArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient);
    ~PredationArc();

    virtual float getCoefficent();
private:
};

#endif /* _PREDATIONARC_H */