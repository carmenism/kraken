#ifndef CHART2D_H_
#define CHART2D_H_

#include "Chart.h"

class Chart2D : public Chart {
    public:
        Chart2D() : Chart() {}
               
        void setGlobalMinX(float x) { globalMinX = x; }
        void setGlobalMaxX(float x) { globalMaxX = x; }
        void setGlobalMinY(float y) { globalMinY = y; }
        void setGlobalMaxY(float y) { globalMaxY = y; } 

        float getGlobalMinX() { return globalMinX; }
        float getGlobalMaxX() { return globalMaxX; }
        float getGlobalMinY() { return globalMinY; }
        float getGlobalMaxY() { return globalMaxY; }    
    protected:
        float globalMinX, globalMaxX;
        float globalMinY, globalMaxY;
    private:      
};

#endif