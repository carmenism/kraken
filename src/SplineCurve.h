#ifndef SPLINECURVE_H_
#define SPLINECURVE_H_

class Color;
class Point;

#include "Link.h"

class SplineCurve : public Link {
public:
    SplineCurve(Point *pointA, Point *pointB, Point *controlA, Point *controlB);
    ~SplineCurve();


protected: 
};

#endif /*SPLINECURVE_H_*/