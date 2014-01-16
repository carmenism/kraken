#ifndef LEGEND_H_
#define LEGEND_H_

#include "Displayable.h"
#include <string>

class Legend : public Displayable {
public:
    Legend();
    ~Legend();

    void draw();

    void setLocation(float x, float y) { this->x = x; this->y = y; }
    float getX() { return x; }
    float getY() { return y; }

    float getWidth() { return width; }
    float getHeight() { return height; }

    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }
protected:
    float fontHeight;
    float x, y;
    float width, height;
    float padding;
    std::string title;

    virtual void drawAtOrigin() = 0;
    void drawBoundary();
};

#endif /* LEGEND_H_ */