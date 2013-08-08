#ifndef _INTERACTIONARC_H
#define _INTERACTIONARC_H

class SingleSpeciesLineChart;

#include "BetweenSpeciesArc.h"

class InteractionArc : public BetweenSpeciesArc {
public:
    InteractionArc(float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB);
    ~InteractionArc();

    virtual float getThickness();
private:
};

#endif /* _INTERACTIONARC_H */