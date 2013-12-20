#ifndef _PREDATIONARC_H
#define _PREDATIONARC_H

class SingleSpeciesLineChart;
class PlotManager;

#include "BetweenSpeciesArc.h"

class PredationArc : public BetweenSpeciesArc {
public:
    PredationArc(PlotManager *pm, float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB);
    ~PredationArc();

    virtual float getThickness();
private:
};

#endif /* _PREDATIONARC_H */