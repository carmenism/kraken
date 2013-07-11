#ifndef CHARTPOINT_H_
#define CHARTPOINT_H_

enum {SHAPE_CIRCLE, SHAPE_SQUARE, SHAPE_TRIANGLE};

class LineChart;
class Shape;
class Color;

#include <string>
#include "Point.h"

class ChartPoint : public Point {
public:
    ChartPoint(LineChart *chart, std::string label, float vX, float vY, int shape = SHAPE_CIRCLE);
    virtual ~ChartPoint();

    void draw();
    void drawLabel();
    void drawToPick();

    void calculateLocation();

    void setShape(int shape);
    void setSize(float size);
    void setBorderColor(Color *color);
    void setFillColor(Color *color);
    void setBorderWidth(float width);

    void setPickColor(int r, int g, int b); 
    
    void captureLastValues() { last->setValues(valueX, valueY); }
    Point *getLast() { return last; }
    float getLastPositionX() { return lastPositionX; }
    float getLastPositionY() { return lastPositionY; }

    void setPositionX(float x);  
    float getPositionX();

    void setPositionY(float y);
    float getPositionY();
    
    void setDisplayLabel( bool d ) { displayLabel = d; }
    bool getDisplayLabel() { return displayLabel; }
    void displayLabelOn() { displayLabel = true; }
    void displayLabelOff() { displayLabel = false; }
    
    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }
private:
    LineChart *chart;
    //float valueX, valueY;
    //float lastValueX, lastValueY;
    float lastPositionX, lastPositionY;
    Point *last;

    Shape *marker;
    std::string label;
    bool displayLabel;

    float fontHeight;

    void drawHistoryLine();
    void drawLineToXAxis();
    std::string makeLabel();
};

#endif /*CHARTPOINT_H_*/