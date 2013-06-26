#ifndef GRAPHMARKER_H_
#define GRAPHMARKER_H_

enum {SHAPE_CIRCLE, SHAPE_SQUARE, SHAPE_TRIANGLE};

class Shape;
class Color;

#include <string>

class GraphMarker
{
public:
    GraphMarker(std::string label, float vX, float vY, int shape = SHAPE_CIRCLE);
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

    void setValueX(float x) { valueX = x; }
    float getValueX() { return valueX; }
    
    void setValueY(float y) { valueY = y; }
    float getValueY() { return valueY; }

    void setPositionX(float x);  
    float getPositionX();

    void setPositionY(float y);
    float getPositionY();
    
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