#ifndef _INTERSPECIESARC_H
#define _INTERSPECIESARC_H

class SmallMultiple;
class Triangle;
class PlotManager;

#include "VerticalArc.h"

enum {ADJUST_NONE, ADJUST_LARGER, ADJUST_SMALLER};

class InterSpeciesArc : public VerticalArc {
public:
    InterSpeciesArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient, std::string label);
    ~InterSpeciesArc();

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
    SmallMultiple *source, *recipient;
    float fontHeight;
    Triangle *arrowA, *arrowB, *arrowMiddle;

    void positionTriangles();
    void drawTriangles();
};

#endif /* _INTERSPECIESARC_H */