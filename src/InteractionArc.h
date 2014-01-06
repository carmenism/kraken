#ifndef _INTERACTIONARC_H
#define _INTERACTIONARC_H

class SingleSpeciesLineChart;
class PlotManager;

#include "BetweenSpeciesArc.h"

class InteractionArc : public BetweenSpeciesArc {
public:
    InteractionArc(PlotManager *pm, float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB);
    ~InteractionArc();
private:
};

#endif /* _INTERACTIONARC_H */