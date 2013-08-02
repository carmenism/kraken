#ifndef CENTERED_ARC_H
#define CENTERED_ARC_H

class Color;

#include "Point.h"

class CenteredArc : public Point {
public:
    CenteredArc();
    CenteredArc(float radius, float centerX, float centerY, float startAngle, float arcAngle);
    ~CenteredArc();

    void draw();

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
protected:
    float startAngle, arcAngle;
    float radius;
    float thickness;
    Color *color;
private:
    void drawAsLineStrips();
    void drawAsPolygons();
};

#endif /* CENTERED_ARC_H */