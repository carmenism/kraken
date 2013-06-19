#ifndef GRAPHMARKER_H_
#define GRAPHMARKER_H_

enum {SHAPE_CIRCLE, SHAPE_SQUARE, SHAPE_TRIANGLE};

class Shape;
class Color;

#include <string>

class GraphMarker
{
public:
    GraphMarker(float vX, float vY, int shape = SHAPE_CIRCLE);
    virtual ~GraphMarker();

    void redraw();

    void calculateLocation(float graphWidth, float graphHeight,
                           float maxValueX,  float maxValueY);

    void setShape(int shape);
    void setSize(float size);
    void setBorderColor(Color *color);
    void setFillColor(Color *color);
    void setBorderWidth(float width);

    float getValueX() { return valueX; }
    float getValueY() { return valueY; }
    float getPositionX();
    float getPositionY();
private:
    float valueX, valueY;
    Shape *marker;
};

#endif /*GRAPHMARKER_H_*/