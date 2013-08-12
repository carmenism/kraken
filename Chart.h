#ifndef CHART_H_
#define CHART_H_

#include <string>

class Chart {
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

        float getXLocation() { return xPos; }
        void setXLocation(float x) { xPos = x; }
     
        float getYLocation() { return yPos; }
        void setYLocation(float y) { yPos = y; }

        void setLocation(float x, float y) { xPos = x; yPos = y; }
        
        float getWidth() { return width; }
        virtual void setWidth(float w) { width = w; }

        float getHeight() { return height; }
        virtual void setHeight(float h) { height = h; }
    
        float getFontHeight() { return fontHeight; }
        void setFontHeight(float h) { fontHeight = h; }

        std::string getTitle() { return title; }
        void setTitle(std::string t) { title = t; }
    protected:
        std::string title;
        float fontHeight;
        float width, height;
        bool displayTitle;

    private:  
        float xPos, yPos;
};

#endif