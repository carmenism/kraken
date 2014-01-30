#ifndef CURSOR_H_
#define CURSOR_H_

#include "ShadowedRectangle.h"

class Cursor : public ShadowedRectangle {
public:
    //Cursor();
    //~Cursor();

    virtual void setLocation(float x, float y);

    virtual float getX();
    virtual float getInnerX();
    virtual void setX(float x);
};

#endif /* CURSOR_H_ */
