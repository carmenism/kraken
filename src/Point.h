#ifndef POINT_H_
#define POINT_H_

#include <string>

class Point
{
public:
    Point(float x, float y);
    virtual ~Point();

    void setValues(float x, float y) { this->x = x; this->y = y; }

    void setX(float x) { this->x = x; }
    float getX() { return x; }
    
    void setY(float y) { this->y = y; }
    float getY() { return y; }
//protected:
    float x, y;
};

#endif /*POINT_H_*/