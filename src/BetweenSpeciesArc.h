#ifndef _BETWEENSPECIESARC_H
#define _BETWEENSPECIESARC_H

class SmallMultiple;
class Triangle;
class PlotManager;

#include "VerticalArc.h"

enum {ADJUST_NONE, ADJUST_LARGER, ADJUST_SMALLER};

class BetweenSpeciesArc : public VerticalArc {
public:
    BetweenSpeciesArc(PlotManager *pm, float coefficient, SmallMultiple *speciesA, SmallMultiple *speciesB, std::string label);
    ~BetweenSpeciesArc();

    float getCoefficient() { return coefficient; }
    void setCoefficient(float c) { coefficient = c; }

    void draw();
    void drawFaded();
    virtual void drawSelected();

    //float getThickness() { return thickness; }
    //void setThickness(float t) { thickness = t; }

    virtual float getCoefficent() { return coefficient; }

    bool getDisplayDynamically() { return displayDynamically; }
    void setDisplayDynamically(bool d) { displayDynamically = d; }
    void displayDynamicallyOn() { displayDynamically = true; }
    void displayDynamicallyOff() { displayDynamically = false; }

    int getAdjustType() { return adjustType; }
    void setAdjustType(int t) { adjustType = t; }
    void adjustNone() { adjustType = ADJUST_NONE; }
    void adjustSmaller() { adjustType = ADJUST_SMALLER; }
    void adjustLarger() { adjustType = ADJUST_LARGER; }

    float getAdjustPercentage() { return adjustPercentage; }
    void setAdjustPercentage(float p) { adjustPercentage = p; }

    float getDynamicThickness();
protected:
    float coefficient;
    //float thickness;
private:
    PlotManager *plotManager;

    int adjustType;
    float adjustPercentage;
    bool displayDynamically;
    std::string betweenSpeciesLabel;
    SmallMultiple *speciesA;
    SmallMultiple *speciesB;
    float fontHeight;
    Triangle *arrowA, *arrowB, *arrowMiddle;

    void positionTriangles();
    void drawTriangles();
};

#endif /* _INTERACTIONARC_H */