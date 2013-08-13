#ifndef _BETWEENSPECIESARCCOLLECTION_H
#define _BETWEENSPECIESARCCOLLECTION_H

class BetweenSpeciesArc;
class SingleSpeciesLineChart;
class Color;

#include "Displayable.h"
#include <vector>

enum {ARC_PREDATION, ARC_INTERACTION};

typedef std::vector<BetweenSpeciesArc *> BetweenSpeciesArcList;
typedef std::vector<BetweenSpeciesArc *>::const_iterator BetweenSpeciesArcIterator;

#define FOREACH_BETWEENSPECIESARC(it, arcList) \
    for(BetweenSpeciesArcIterator it = arcList.begin(); it != arcList.end(); ++it)
#define FOREACH_BETWEENSPECIESARCP(it, arcList) \
    for(BetweenSpeciesArcIterator it = arcList->begin(); it != arcList->end(); ++it)

class BetweenSpeciesArcCollection : public Displayable {
public:
    BetweenSpeciesArcCollection();
    ~BetweenSpeciesArcCollection();

    void draw();
    void drawSelected();
    void drawToPick();

    void addArc(BetweenSpeciesArc *arc);
    void addArc(int type, float coeff, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB, Color *c, bool setToLeft);
    BetweenSpeciesArcList *getArcs() { return arcs; }
private:
    BetweenSpeciesArcList *arcs;
    BetweenSpeciesArc *selected;
};

#endif /* _BETWEENSPECIESARCCOLLECTION_H */