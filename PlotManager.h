#ifndef _PLOTMANAGER_H
#define _PLOTMANAGER_H

#include "LineChart.h"
#include <vector>
#include "ChartPoint.h"

class PlotManager {
    public:
        PlotManager() { display = true; }
     
        bool getDisplay() { return display; }
        void setDisplay(bool d) { display = d; }
        void displayOn() { display = true; }
        void displayOff() { display = false; }

        void draw();
        void drawToPick();
        void captureLastValues();
        ChartPointList getPoints();
        
        bool empty() { return getCharts().empty(); }

        virtual std::vector<LineChart *> getCharts() = 0;
    protected:

    private: 
        bool display;
};

#endif  /* _PLOTMANAGER_H */