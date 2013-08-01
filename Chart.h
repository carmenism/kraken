#include <string>

class Chart {
    public:
        Chart();
        void draw();
        void drawToPick();
     
        virtual void drawAtOrigin() = 0;
        virtual void drawToPickAtOrigin() = 0;
     
        float getXLocation() { return xPos; }
        void setXLocation(float x) { xPos = x; }
     
        float getYLocation() { return yPos; }
        void setYLocation(float y) { yPos = y; }

        void setLocation(float x, float y) { xPos = x; yPos = y; }
        
        float getWidth() { return width; }
        void setWidth(float w) { width = w; }

        float getHeight() { return height; }
        void setHeight(float h) { height = h; }
    
        float getFontHeight() { return fontHeight; }
        void setFontHeight(float h) { fontHeight = h; }

        std::string getTitle() { return title; }
        void setTitle(std::string t) { title = t; }
    protected:
        std::string title;
        float fontHeight;
        float width, height;

    private:  
        float xPos, yPos;
};