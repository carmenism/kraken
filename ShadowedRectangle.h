#ifndef SHADOWEDRECTANGLE_H_
#define SHADOWEDRECTANGLE_H_

class Color;

class ShadowedRectangle {
public:
    ShadowedRectangle();
    void draw();
    bool containsPoint(float x, float y);

    void setLocation(float x, float y) { this->x = x; this->y = y; }

    float getX() { return x; }
    float getInnerX() { return x + border; }
    void setX(float x) { this->x = x; }

    float getY() { return y; }
    float getInnerY() { return y + border; } 
    void setY(float y) { this->y = y; }

    float getWidth() { return width; }
    float getInnerWidth() { return width - 2 * border; }
    void setWidth(float w) { width = w; }
    
    float getHeight() { return height; }
    float getInnerHeight() { return height - 2 * border; }
    void setHeight(float h) { height = h; }
    
    float getShadowAlpha() { return shadowAlpha; }
    void setShadowAlpha(float a) { shadowAlpha = a; }
    
    float getHighlightAlpha() { return highlightAlpha; }
    void setHighlightAlpha(float a) { highlightAlpha = a; }

    Color *getColor() { return color; }
    void setColor(Color *c) { color = c; }
    
    float getBorder() { return border; }
    void setBorder(float b) { border = b; }
protected:
    float x, y;
    float width, height;
    float border;
    Color *color;   
    bool reverseShadow;
private:
    float shadowAlpha, highlightAlpha;
};

#endif /* SHADOWEDRECTANGLE_H_ */
