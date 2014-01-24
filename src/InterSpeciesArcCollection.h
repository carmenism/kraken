#ifndef _INTERSPECIESARCCOLLECTION_H
#define _INTERSPECIESARCCOLLECTION_H

#include "InterSpeciesArc.h"
class SmallMultiple;
class Color;
class ColorLegend;
class PlotManager;

#include "Displayable.h"
#include <vector>
#include <string>

enum {ARC_PREDATION, ARC_INTERACTION};

typedef std::vector<InterSpeciesArc *> InterSpeciesArcList;
typedef std::vector<InterSpeciesArc *>::const_iterator InterSpeciesArcIterator;

#define FOREACH_INTERSPECIESARC(it, arcList) \
    for(InterSpeciesArcIterator it = arcList.begin(); it != arcList.end(); ++it)
#define FOREACH_INTERSPECIESARCP(it, arcList) \
    for(InterSpeciesArcIterator it = arcList->begin(); it != arcList->end(); ++it)

class InterSpeciesArcCollection : public Displayable {
public:
    InterSpeciesArcCollection(PlotManager *pm, std::string title);
    ~InterSpeciesArcCollection();

    void findSelected();
    void draw();
    void drawToPick();

    void setTitleLocation(float x, float y);

    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }

	int size();
	InterSpeciesArc *at(int i);

    void addArc(InterSpeciesArc *arc);
    void addArc(int type, float coeff, SmallMultiple *source, SmallMultiple *recipient, bool setToLeft);
    InterSpeciesArcList *getVisibleArcs();

    void setDisplayDynamically(bool d);
    void displayDynamicallyOn();
    void displayDynamicallyOff();

    void setAdjustType(AdjustType t);
    void adjustNone();
    void adjustSmaller();
    void adjustLarger();

    void setAdjustPercentage(float p);

    void addArcs(InterSpeciesArcCollection *otherArcs);
    void clear();

    bool isAnimated();
    bool getAnimated();
    void setAnimated(bool a);
    void animatedOn();
    void animatedOff();

    void setLegend(ColorLegend *legend) { this->legend = legend; }
    ColorLegend *getLegend() { return legend; }

	Link *getSelected() { return selected; }
	void setSelected(Link *s) { selected = s; }
private:
    PlotManager *plotManager;
    InterSpeciesArcList *arcs;
    Link *selected;
    std::string title;
    float titleX, titleY;
    float fontHeight;

    ColorLegend *legend;
};

#endif /* _INTERSPECIESARCCOLLECTION_H */