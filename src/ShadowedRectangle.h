#ifndef SHADOWEDRECTANGLE_H_
#define SHADOWEDRECTANGLE_H_

#include "Object2D.h"

class Color;

class ShadowedRectangle : public Object2D {
public:
    ShadowedRectangle();
    ~ShadowedRectangle();

    void draw();
    bool containsPoint(float x, float y);
   
    float getInnerX() { return x + border; }
    float getInnerY() { return y + border; } 

    float getInnerWidth() { return width - 2 * border; }    
    float getInnerHeight() { return height - 2 * border; }
    
    float getShadowAlpha() { return shadowAlpha; }
    void setShadowAlpha(float a) { shadowAlpha = a; }
    
    float getHighlightAlpha() { return highlightAlpha; }
    void setHighlightAlpha(float a) { highlightAlpha = a; }

    Color *getColor() { return color; }
    void setColor(Color *c) { color = c; }
    
    float getBorder() { return border; }
    void setBorder(float b) { border = b; }
protected:
    float border;
    Color *color;   
    bool reverseShadow;
private:
    float shadowAlpha, highlightAlpha;
};

#endif /* SHADOWEDRECTANGLE_H_ */
