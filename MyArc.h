#ifndef ARC_H
#define ARC_H

class Color;

#include "Point.h"

class MyArc : public Point {
public:
    MyArc();
    MyArc(float radius, float centerX, float centerY, float startAngle, float arcAngle);
    ~MyArc();

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
private:
    float startAngle, arcAngle;
    float radius;
    float thickness;
    Color *color;
};

#endif /* ARC_H */