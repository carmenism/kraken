#ifndef _PREDATIONARC_H
#define _PREDATIONARC_H

class SingleSpeciesLineChart;

#include "BetweenSpeciesArc.h"

class PredationArc : public BetweenSpeciesArc {
public:
    PredationArc(float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB);
    ~PredationArc();

    virtual float getThickness();
private:
};

#endif /* _PREDATIONARC_H */