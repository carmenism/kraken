#ifndef CHARTPOINT_H_
#define CHARTPOINT_H_

enum {SHAPE_CIRCLE};

class LineChart;
class Shape;
class Color;

#include <string>
#include "Point.h"
#include "Pickable.h"

class ChartPoint : public Point, public Pickable {
public:
    ChartPoint(LineChart *chart, std::string label, float vX, float vY);
    virtual ~ChartPoint();

    void draw();
    void drawSelected();
    virtual void drawToPick();

    void calculateLocation();

    std::string getLabel() { return label; }

    void capturePreviousValues();
    Point *getPrevious() { return previousValues; }
    float getPreviousValue();
    float getPreviousPositionX() { return previousPosition->getX(); }
    float getPreviousPositionY() { return previousPosition->getY(); }

    void setPositionX(float x);  
    float getPositionX();

    void setPositionY(float y);
    float getPositionY();
    
    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }

    float getDifferenceFromPrevious();
    float getPercentIncreaseFromPrevious();

    void setMarker(Shape *s) { marker = s; }
private:
    LineChart *chart;
    Point *position, *previousPosition;
    Point *previousValues;

    Shape *marker;
    std::string label;
    float fontHeight;

    void drawHistoryLine();
    void drawLineToXAxis();
    std::string makeLabel();
};

#endif /*CHARTPOINT_H_*/