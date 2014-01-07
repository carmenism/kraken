#ifndef _INTERACTIONARC_H
#define _INTERACTIONARC_H

class SmallMultiple;
class PlotManager;

#include "BetweenSpeciesArc.h"

class InteractionArc : public BetweenSpeciesArc {
public:
    InteractionArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient);
    ~InteractionArc();
private:
};

#endif /* _INTERACTIONARC_H */