#ifndef SHADOWEDRECTANGLE_H_
#define SHADOWEDRECTANGLE_H_

class Color;

class ShadowedRectangle {
public:
    ShadowedRectangle();
    void draw();
    bool containsPoint(float x, float y);

    float getX() { return x; }
    void setX(float x) { this->x = x; }

    float getY() { return y; }
    void setY(float y) { this->y = y; }

    float getWidth() { return width; }
    void setWidth(float w) { width = w; }
    
    float getHeight() { return height; }
    void setHeight(float h) { height = h; }
    
    float getShadowAlpha() { return shadowAlpha; }
    void setShadowAlpha(float a) { shadowAlpha = a; }
    
    float getHighlightAlpha() { return highlightAlpha; }
    void setHighlightAlpha(float a) { highlightAlpha = a; }

    Color *getColor() { return color; }
    void setColor(Color *c) { color = c; }
    
    float getBorder() { return border; }
    void setBorder(float b) { border = b; }
private:
    float border;
    float x, y;
    float width, height;
    float shadowAlpha, highlightAlpha;
    Color *color;   
};

#endif /* SHADOWEDRECTANGLE_H_ */
