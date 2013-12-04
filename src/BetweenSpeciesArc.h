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

    bool getDisplayDynamically() { return displayDynamically; }
    void setDisplayDynamically(bool d) { displayDynamically = d; }
    void displayDynamicallyOn() { displayDynamically = true; }
    void displayDynamicallyOff() { displayDynamically = false; }
protected:
    float coefficient;
private:
    bool displayDynamically;
    std::string betweenSpeciesLabel;
    SingleSpeciesLineChart *speciesA;
    SingleSpeciesLineChart *speciesB;
    float fontHeight;
    Triangle *arrowA, *arrowB, *arrowMiddle;

    void positionTriangles();
    void drawTriangles();
};

#endif /* _INTERACTIONARC_H */