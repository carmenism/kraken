#ifndef _INTERACTIONARC_H
#define _INTERACTIONARC_H

class SingleSpeciesLineChart;

#include "VerticalArc.h"

class InteractionArc : public VerticalArc {
public:
    InteractionArc(float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB);
    ~InteractionArc();

    float getCoefficient() { return coefficient; }
    void setCoefficient(float c) { coefficient = c; }

    void draw();
    virtual void drawSelected();

private:
    float coefficient;
    SingleSpeciesLineChart *speciesA;
    SingleSpeciesLineChart *speciesB;
    float fontHeight;
};

#endif /* _INTERACTIONARC_H */