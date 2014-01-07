#ifndef _INTERACTIONARC_H
#define _INTERACTIONARC_H

class SmallMultiple;
class PlotManager;

#include "InterSpeciesArc.h"

class InteractionArc : public InterSpeciesArc {
public:
    InteractionArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient);
    ~InteractionArc();
private:
};

#endif /* _INTERACTIONARC_H */