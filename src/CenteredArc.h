#ifndef CENTERED_ARC_H
#define CENTERED_ARC_H

class Color;
class Triangle;

#include "Animatable.h"
#include "Point.h"
#include "Pickable.h"

class CenteredArc {
public:
    CenteredArc();
    CenteredArc(float radius, float centerX, float centerY);
    ~CenteredArc();
    

    void draw();

    
protected:


private:



    //CW_CODE
};

#endif /* CENTERED_ARC_H */