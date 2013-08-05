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
    CenteredArc(float radius, float centerX, float centerY, float startAngle, float arcAngle);
    ~CenteredArc();

    void draw();
    virtual void drawToPick();
    virtual void drawSelected();

    float getStartAngle() { return startAngle; }
    void setStartAngle(float a) { startAngle = a; }
    
    float getArcAngle() { return arcAngle; }
    void setArcAngle(float a) { arcAngle = a; }
    
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
    float startAngle, arcAngle;
    float radius;
    float thickness;
    Color *color;
    bool fadingAlpha;
    float startAlpha, finalAlpha;

    static void drawPolygonArc(float x, float y, float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha);
    static void drawLineArc(float x, float y, float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha);

    static void drawPolygonArc(float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha);
    static void drawLineArc(float radius, float thickness, float startAngle, float arcAngle, Color *color, float startAlpha, float finalAlpha);
private:
    GLUquadricObj *quadratic;
    //void drawAsLineStrips();
    //void drawAsPolygons();
    void drawToPickAsLineStrips();
    void drawToPickAsPolygons();
};

#endif /* CENTERED_ARC_H */