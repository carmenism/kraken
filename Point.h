#ifndef POINT_H_
#define POINT_H_

#include <string>

class Point
{
public:
    Point(float x, float y);
    virtual ~Point();

    void setValues(float x, float y) { valueX = x; valueY = y; }

    void setValueX(float x) { valueX = x; }
    float getValueX() { return valueX; }
    
    void setValueY(float y) { valueY = y; }
    float getValueY() { return valueY; }
protected:
    float valueX, valueY;
};

#endif /*POINT_H_*/