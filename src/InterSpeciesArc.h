#ifndef _INTERSPECIESARC_H
#define _INTERSPECIESARC_H

class Color;
class SmallMultiple;
class PlotManager;
class Triangle;

#include "Link.h"

enum AdjustType {ADJUST_NONE, ADJUST_LARGER, ADJUST_SMALLER};

class InterSpeciesArc : public Link {
public:
    InterSpeciesArc(PlotManager *pm, float coefficient, SmallMultiple *source, SmallMultiple *recipient, std::string label);
    ~InterSpeciesArc();

    float getCoefficient() { return coefficient; }
    void setCoefficient(float c) { coefficient = c; }

    virtual float getDynamicConstant() = 0;

    void draw();
    void drawFaded();
    virtual void drawSelected();	
    virtual void drawToPick();

    virtual float getCoefficent() { return coefficient; }

    AdjustType getAdjustType() { return adjustType; }
    void setAdjustType(AdjustType t) { adjustType = t; }
    void adjustNone() { adjustType = ADJUST_NONE; }
    void adjustSmaller() { adjustType = ADJUST_SMALLER; }
    void adjustLarger() { adjustType = ADJUST_LARGER; }

    float getAdjustPercentage() { return adjustPercentage; }
    void setAdjustPercentage(float p) { adjustPercentage = p; }

    float getDynamicThickness();
	
	void setArcToLeft() { arcToRight = false; }
	void setArcToRight() { arcToRight = true; }
	bool isArcToRight() { return arcToRight; }

	float getRadius() { return radius; }
    void setRadius(float r) { radius = r; }

    bool getFadingAlpha() { return fadingAlpha; }
    void setFadingAlpha(bool f) { fadingAlpha = f; }
    void fadingAlphaOn() { fadingAlpha = true; }
    void fadingAlphaOff() { fadingAlpha = false; }
protected:
	float coefficient;
private:
    bool arcToRight, fadingAlpha;
    float radius;
    float startAlpha, finalAlpha, signAlpha, yA, yB;
    float fontHeight, adjustPercentage;

    AdjustType adjustType;
    std::string betweenSpeciesLabel;

    Color *highlightColor;
    Triangle *arrowA, *arrowB, *arrowMiddle;
    PlotManager *plotManager;
    SmallMultiple *source, *recipient;
    
    void setUpForDrawing();
    void determineThickness();
    void determineCenterLocation();
    void adjustPositions();
    void positionTriangles();
	
    void drawPolygonArc();
    void drawLineArc();
    void drawHighlight();
    void drawSigns();
    void drawTriangles();
	
    void drawToPickAsLineStrips();
    void drawToPickAsPolygons();
	
	static float *xArc, *yArc;   
	static const int NUM_SEGMENTS = 360; 
};

#endif /* _INTERSPECIESARC_H */