#ifndef SLIDER_H_
#define SLIDER_H_

// a slider that works in the range 0..1.0 in pixel coordinates
class Color;

#include <string>

class Slider
{
public:
    Slider(std::string label, float min, float max, float start);

    // specify the lower left corner, the slider length
    // and the start value between 0 and 1.0
    
    void draw();

    float getValue();
    void Slider::setValue(float value);

    bool mouseMoved(float x, float y);
    bool mousePressed(float x, float y);
    bool mouseReleased();

    float getShadowAlpha() { return shadowAlpha; }
    void setShadowAlpha(float a) { shadowAlpha = a; }
    
    float getHighlightAlpha() { return highlightAlpha; }
    void setHighlightAlpha(float a) { highlightAlpha = a; }

    Color *getMainColor() { return mainColor; }
    void setMainColor(Color *c) { mainColor = c; }

    Color *getCursorColor() { return cursorColor; }
    void setCursorColor(Color *c) { cursorColor = c; }

    float getHeight() { return height; }
    void setHeight(float h) { height = h; }

    float getWidth() { return width; }
    void setWidth(float w);

    float getBorder() { return border; }
    void setBorder(float b) { border = b; }

    float getCursorWidth() { return cursorWidth; }
    void setCursorWidth(float w) { cursorWidth = w; }

    std::string getLabel() { return label; }

    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }

    void setLocation(float x, float y) { cornerX = x; cornerY = y; }
protected:
    float minValue, maxValue;
    float cornerX, cornerY;
    float curX, startCurX;
    float border, width, height, cursorWidth;
private:
    std::string label;

    bool active;
    float startX;

    Color *mainColor, *cursorColor;
    float shadowAlpha, highlightAlpha;

    float fontHeight;

    bool pointInCursor(float x, float y);
};

#endif /* SLIDER_H_ */