#ifndef CENTERED_ARC_H
#define CENTERED_ARC_H

class Color;

#include "Point.h"
#include <QtOpenGL>
#include <GL/glut.h>
#include "Pickable.h"

class CenteredArc : public Point, public Pickable {
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

    bool isAnimated() { return animated; }
    bool getAnimated() { return animated; }
    void setAnimated(bool a) { animated = a; }
    void animatedOn() { animated = true; }
    void animatedOff() { animated = false; }
protected:
    bool arcToRight;
    bool animated;
    float radius;
    float thickness, highlightThickness;
    Color *color, *highlightColor;
    bool fadingAlpha;
    float startAlpha, finalAlpha;

    void drawPolygonArc();
    void drawLineArc();
    void drawHighlight();
private:
    void drawToPickAsLineStrips();
    void drawToPickAsPolygons();

    //CW_CODE
	unsigned char checker[64][64][4];
	unsigned int texName;
	int num_segments;
	float *xArc, *yArc;
};

#endif /* CENTERED_ARC_H */