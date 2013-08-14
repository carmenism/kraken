#ifndef _BETWEENSPECIESARC_H
#define _BETWEENSPECIESARC_H

class SingleSpeciesLineChart;
class Triangle;

#include "VerticalArc.h"

class BetweenSpeciesArc : public VerticalArc {
public:
    BetweenSpeciesArc(float coefficient, SingleSpeciesLineChart *speciesA, SingleSpeciesLineChart *speciesB, std::string label);
    ~BetweenSpeciesArc();

    float getCoefficient() { return coefficient; }
    void setCoefficient(float c) { coefficient = c; }

    void draw();
    void drawFaded();
    virtual void drawSelected();

    virtual float getThickness() = 0;

protected:
    float coefficient;
private:
    std::string betweenSpeciesLabel;
    SingleSpeciesLineChart *speciesA;
    SingleSpeciesLineChart *speciesB;
    float fontHeight;
    Triangle *arrowA, *arrowB;
};

#endif /* _INTERACTIONARC_H */