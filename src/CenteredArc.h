#ifndef CENTERED_ARC_H
#define CENTERED_ARC_H

class Color;

#include "Animatable.h"
#include "Point.h"
#include "Pickable.h"

class CenteredArc : public Point, public Pickable, public Animatable {
public:
    CenteredArc();
    CenteredArc(float radius, float centerX, float centerY);
    ~CenteredArc();
    
    void setArcToLeft();
    void setArcToRight();

    bool isArcToRight();

    void draw();
    virtual void drawToPick();
    virtual void drawSelected();

    float getRadius() { return radius; }
    void setRadius(float r) { radius = r; }

    float getThickness() { return thickness; }
    void setThickness(float t) { thickness = t; }

    Color *getColor() { return color; }
    void setColor(Color *c) { color = c; }

    bool getFadingAlpha() { return fadingAlpha; }
    void setFadingAlpha(bool f) { fadingAlpha = f; }
    void fadingAlphaOn() { fadingAlpha = true; }
    void fadingAlphaOff() { fadingAlpha = false; }
protected:
    bool arcToRight;
    float radius;
    float thickness, highlightThickness;
    Color *color, *highlightColor;
    bool fadingAlpha;
    float startAlpha, finalAlpha;    
    float signAlpha;

    void drawPolygonArc();
    void drawLineArc();
    void drawHighlight();
    void drawSigns();

	static float *xArc, *yArc;   
	static const int NUM_SEGMENTS = 360; 
private:
    void drawToPickAsLineStrips();
    void drawToPickAsPolygons();

    //CW_CODE
};

#endif /* CENTERED_ARC_H */