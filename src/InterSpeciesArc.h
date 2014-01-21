#ifndef _INTERSPECIESARC_H
#define _INTERSPECIESARC_H

class SmallMultiple;
class PlotManager;

#include "CenteredArc.h"

enum AdjustType {ADJUST_NONE, ADJUST_LARGER, ADJUST_SMALLER};

class InterSpeciesArc : public CenteredArc {
public:
    InterSpeciesArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient, std::string label);
    ~InterSpeciesArc();

    float getCoefficient() { return coefficient; }
    void setCoefficient(float c) { coefficient = c; }

    virtual float getDynamicConstant() = 0;

    virtual void drawToPick();
    void draw();
    void drawFaded();
    virtual void drawSelected();

    virtual float getCoefficent() { return coefficient; }

    AdjustType getAdjustType() { return adjustType; }
    void setAdjustType(AdjustType t) { adjustType = t; }
    void adjustNone() { adjustType = ADJUST_NONE; }
    void adjustSmaller() { adjustType = ADJUST_SMALLER; }
    void adjustLarger() { adjustType = ADJUST_LARGER; }

    float getAdjustPercentage() { return adjustPercentage; }
    void setAdjustPercentage(float p) { adjustPercentage = p; }

    float getDynamicThickness();
protected:
    float coefficient;
private:
    PlotManager *plotManager;
    float yA, yB;

    AdjustType adjustType;
    
    float fontHeight;
    float adjustPercentage;
    std::string betweenSpeciesLabel;
    SmallMultiple *source, *recipient;

    
    void setUpForDrawing();
    void determineThickness();
    void determineCenterLocation();
    void adjustPositions();
};

#endif /* _INTERSPECIESARC_H */