#ifndef CHARTPOINT_H_
#define CHARTPOINT_H_

enum {SHAPE_CIRCLE, SHAPE_SQUARE, SHAPE_TRIANGLE};

class LineChart;
class Shape;
class Color;

#include <string>
#include "Point.h"
#include "Pickable.h"

class ChartPoint : public Point, public Pickable {
public:
    ChartPoint(LineChart *chart, std::string label, float vX, float vY, int shape = SHAPE_CIRCLE);
    virtual ~ChartPoint();

    void draw();
    void drawSelected();
    void drawToPick();

    void calculateLocation();

    std::string getLabel() { return label; }

    void setShape(int shape);
    void setSize(float size);
    void setBorderColor(Color *color);
    void setFillColor(Color *color);
    void setBorderWidth(float width);

    void captureLastValues();
    Point *getLast() { return last; }
    float getLastPositionX() { return lastPositionX; }
    float getLastPositionY() { return lastPositionY; }

    void setPositionX(float x);  
    float getPositionX();

    void setPositionY(float y);
    float getPositionY();
    
    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }
private:
    LineChart *chart;
    float lastPositionX, lastPositionY;
    Point *last;

    Shape *marker;
    std::string label;

    float fontHeight;

    void drawHistoryLine();
    void drawLineToXAxis();
    std::string makeLabel();
};

#endif /*CHARTPOINT_H_*/