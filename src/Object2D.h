#ifndef _OBJECT2D_H
#define _OBJECT2D_H

class Color;

class Object2D {
public:
    Object2D();
    ~Object2D();

    float getX() { return x; }
    float getY() { return y; }
    
    void setX(float x) { this->x = x; }
    void setY(float y) { this->y = y; }

    void setLocation(float x, float y) { this->x = x; this->y = y; }

    float getWidth() { return width; }
    float getHeight() { return height; }

    void setWidth(float w) { width = w; }
    void setHeight(float h) { height = h; }

    void setSize(float w, float h) { width = w; height = h; }
protected:
    float x, y;
    float width, height;
    Color *borderColor;

    void drawBoundary();
};

#endif /* _OBJECT2D_H */