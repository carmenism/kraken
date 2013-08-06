#ifndef SLIDER_H_
#define SLIDER_H_

// a slider that works in the range 0..1.0 in pixel coordinates
class Color;
class ShadowedRectangle;

#include <string>
#include "ShadowedRectangle.h"

enum {POS_ABOVE, POS_BELOW, POS_LEFT, POS_RIGHT};

class Slider
{
public:
    Slider(std::string title, float min, float max, float start);

    void undo();
    void reset();

    void draw();

    float getValue();
    void setValue(float value);

    bool mouseMoved(float x, float y);
    bool mousePressed(float x, float y);
    bool mouseReleased();

    float getWidth() { return main->getWidth(); }
    void setWidth(float w);

    float getHeight() { return main->getHeight(); }
    void setHeight(float h);

    void setLocation(float x, float y) { main->setX(x); main->setY(y); }

    std::string getTitle() { return title; }
    bool getDisplayTitle() { return displayTitle; }
    void setDisplayTitle(bool d) { displayTitle = d; }
    void displayTitleOn() { displayTitle = true; }
    void displayTitleOff() { displayTitle = false; }
    float getTitleFontHeight() { return titleFontHeight; }
    void setTitleFontHeight(float h) { titleFontHeight = h; }
    
    int getTitlePosition() { return titlePosition; }
    void setTitlePosition(int p) { titlePosition = p; }
    void titlePositionRight() { titlePosition = POS_RIGHT; }
    void titlePositionLeft() { titlePosition = POS_LEFT; }
    void titlePositionAbove() { titlePosition = POS_ABOVE; }
    void titlePositionBelow() { titlePosition = POS_BELOW; }

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
    ShadowedRectangle *main, *cursor;
    float minValue, maxValue;
    float startCurX, curX;
private:
    float undoValue, resetValue;

    bool displayTitle;
    std::string title;
    float titleFontHeight;
    int titlePosition;

    bool active;
    float startX;

    bool displayLabels;
    float labelInterval;
    bool labelsBelow;
    float labelFontHeight;

    bool pointInCursor(float x, float y);
    void drawLabels();
    void drawTitle();
    float valueToPosition(float value);
    float positionToValue(float position);
};

#endif /* SLIDER_H_ */