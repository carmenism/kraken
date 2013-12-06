#ifndef _BETWEENSPECIESARCCOLLECTION_H
#define _BETWEENSPECIESARCCOLLECTION_H

class BetweenSpeciesArc;
class SingleSpeciesLineChart;
class Color;

#include "Displayable.h"
#include <vector>
#include <string>

enum {ARC_PREDATION, ARC_INTERACTION};

typedef std::vector<BetweenSpeciesArc *> BetweenSpeciesArcList;
typedef std::vector<BetweenSpeciesArc *>::const_iterator BetweenSpeciesArcIterator;

#define FOREACH_BETWEENSPECIESARC(it, arcList) \
    for(BetweenSpeciesArcIterator it = arcList.begin(); it != arcList.end(); ++it)
#define FOREACH_BETWEENSPECIESARCP(it, arcList) \
    for(BetweenSpeciesArcIterator it = arcList->begin(); it != arcList->end(); ++it)

class BetweenSpeciesArcCollection : public Displayable {
public:
    BetweenSpeciesArcCollection(std::string title);
    ~BetweenSpeciesArcCollection();

    void draw();
    void drawSelected();
    void drawToPick();

    void setTitleLocation(float x, float y);

    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }

    void addArc(BetweenSpeciesArc *arc);
    void addArc(int type, float coeff, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB, bool setToLeft, Color *c = NULL);
    BetweenSpeciesArcList *getArcs() { return arcs; }

    void setDisplayDynamically(bool d);
    void displayDynamicallyOn();
    void displayDynamicallyOff();

    void setAdjustType(int t);
    void adjustNone();
    void adjustSmaller();
    void adjustLarger();

    void setAdjustPercentage(float p);

    void addArcs(BetweenSpeciesArcCollection *otherArcs);
    void clear();
private:
    BetweenSpeciesArcList *arcs;
    BetweenSpeciesArc *selected;
    std::string title;
    float titleX, titleY;
    float fontHeight;
};

#endif /* _BETWEENSPECIESARCCOLLECTION_H */