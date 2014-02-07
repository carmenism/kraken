#ifndef CHART_H_
#define CHART_H_

#include "Object2D.h"
#include <string>

class Chart : public Object2D {
    public:
        Chart();
        virtual void draw();
        virtual void drawToPick();
     
        virtual void drawAtOrigin() = 0;
        virtual void drawToPickAtOrigin() = 0;
     
        bool getDisplayTitle() { return displayTitle; }
        void setDisplayTitle(bool d) { displayTitle = d; }
        void displayTitleOn() { displayTitle = true; }
        void displayTitleOff() { displayTitle = false; }
    
        float getFontHeight() { return fontHeight; }
        void setFontHeight(float h) { fontHeight = h; }

        std::string getTitle() { return title; }
        void setTitle(std::string t) { title = t; }
    protected:
        std::string title;
        float fontHeight;
        bool displayTitle;
};

#endif