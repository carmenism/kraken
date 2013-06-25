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

    void draw();
    void drawLabel();
    void drawToPick();

    void calculateLocation(float graphWidth, float graphHeight,
                           float maxValueX,  float maxValueY);

    void setShape(int shape);
    void setSize(float size);
    void setBorderColor(Color *color);
    void setFillColor(Color *color);
    void setBorderWidth(float width);

    void setPickColor(int r, int g, int b); 

    float getValueX() { return valueX; }
    float getValueY() { return valueY; }
    float getPositionX();
    float getPositionY();
    
    void setPositionX(float x);
    void setPositionY(float y);

    void setDisplayLabel( bool d ) { displayLabel = d; }
    bool getDisplayLabel() { return displayLabel; }
    void displayLabelOn() { displayLabel = true; }
    void displayLabelOff() { displayLabel = false; }
private:
    float valueX, valueY;
    Shape *marker;
    std::string label;
    bool displayLabel;
};

#endif /*GRAPHMARKER_H_*/