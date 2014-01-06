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

    void capturePreviousValues();
    Point *getPrevious() { return previousValues; }
    float getPreviousPositionX() { return previousPositionX; }
    float getPreviousPositionY() { return previousPositionY; }

    void setPositionX(float x);  
    float getPositionX();

    void setPositionY(float y);
    float getPositionY();
    
    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }

    float getDifferenceFromPrevious();
    float getPercentIncreaseFromPrevious();
private:
    LineChart *chart;
    float previousPositionX, previousPositionY;
    Point *previousValues;

    Shape *marker;
    std::string label;

    float fontHeight;

    void drawHistoryLine();
    void drawLineToXAxis();
    std::string makeLabel();
};

#endif /*CHARTPOINT_H_*/