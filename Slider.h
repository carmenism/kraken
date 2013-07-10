#ifndef SLIDER_H_
#define SLIDER_H_

// a slider that works in the range 0..1.0 in pixel coordinates
class Color;

#include <string>

class Slider
{
public:
    Slider(std::string title, float min, float max, float start);

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

    std::string getTitle() { return title; }

    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }

    void setLocation(float x, float y) { cornerX = x; cornerY = y; }

    float getLabelInteveral() { return labelInterval; }
    void setLabelInterval(float i) { labelInterval = i; }

    bool getDisplayLabels() { return displayLabels; }
    void setDisplayLabels(bool d) { displayLabels = d; }
    void displayLabelsOn() { displayLabels = true; }
    void displayLabelsOff() { displayLabels = false; }
protected:
    float minValue, maxValue;
    float cornerX, cornerY;
    float curX, startCurX;
    float border, width, height, cursorWidth;
private:
    std::string title;

    bool active;
    float startX;

    bool displayLabels;
    float labelInterval;

    Color *mainColor, *cursorColor;
    float shadowAlpha, highlightAlpha;

    float fontHeight;

    bool pointInCursor(float x, float y);
    void drawLabels();

    float valueToPosition(float value);
    float positionToValue(float position);
};

#endif /* SLIDER_H_ */