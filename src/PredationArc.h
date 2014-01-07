#ifndef _PREDATIONARC_H
#define _PREDATIONARC_H

class SmallMultiple;
class PlotManager;

#include "BetweenSpeciesArc.h"

class PredationArc : public BetweenSpeciesArc {
public:
    PredationArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient);
    ~PredationArc();

    virtual float getCoefficent();
private:
};

#endif /* _PREDATIONARC_H */