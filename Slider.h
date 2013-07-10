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
    void setValue(float value);

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

    void setLocation(float x, float y) { cornerX = x; cornerY = y; }

    std::string getTitle() { return title; }
    bool getDisplayTitle() { return displayTitle; }
    void setDisplayTitle(bool d) { displayTitle = d; }
    void displayTitleOn() { displayTitle = true; }
    void displayTitleOff() { displayTitle = false; }
    float getTitleFontHeight() { return titleFontHeight; }
    void setTitleFontHeight(float h) { titleFontHeight = h; }
    void displayTitleOnRight() { titleRight = true; }
    void displayTitleOnLeft() { titleRight = false; }

    bool getDisplayLabels() { return displayLabels; }
    void setDisplayLabels(bool d) { displayLabels = d; }
    void displayLabelsOn() { displayLabels = true; }
    void displayLabelsOff() { displayLabels = false; }
    float getLabelInteveral() { return labelInterval; }
    void setLabelInterval(float i) { labelInterval = i; }
    void displayLabelsBelow() { labelsBelow = true; }
    void displayLabelsAbove() { labelsBelow = false; }
    float getLabelFontHeight() { return labelFontHeight; }
    void setLabelFontHeight(float h) { labelFontHeight = h; }
protected:
    float minValue, maxValue;
    float cornerX, cornerY;
    float curX, startCurX;
    float border, width, height, cursorWidth;
private:
    bool displayTitle;
    std::string title;
    float titleFontHeight;
    bool titleRight;

    bool active;
    float startX;

    bool displayLabels;
    float labelInterval;
    bool labelsBelow;
    float labelFontHeight;

    Color *mainColor, *cursorColor;
    float shadowAlpha, highlightAlpha;

    bool pointInCursor(float x, float y);
    void drawLabels();
    void drawTitle();
    float valueToPosition(float value);
    float positionToValue(float position);
};

#endif /* SLIDER_H_ */