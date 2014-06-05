#ifndef ABSOLUTESIZESCHART_H_
#define ABSOLUTESIZESCHART_H_

class AbsoluteSizeIndicator;
class AbsoluteSizeLegend;
class LineChart;

#include "Chart2D.h"
#include <vector>

class AbsoluteSizesChart : public Chart2D {
    public:
        AbsoluteSizesChart(LineChart *lineChart);
        ~AbsoluteSizesChart();

        virtual void draw();
        virtual void drawAtOrigin();
        virtual void drawToPick();
        virtual void drawToPickAtOrigin();

        int getSize() { return size; }

        AbsoluteSizeIndicator **getPoints() { return points; }

        float getRadiusFromValue(float value);

        float getScalingFactor() { return scalingFactor; }
        void setScalingFactor(float f) { scalingFactor = f; }

        bool getDisplayLegend();
        void setDisplayLegend(bool d);
        void displayLegendOn();
        void displayLegendOff();

        void setLegendLocation(float x, float y);

        AbsoluteSizeLegend *getLegend() { return legend; }
    protected:
    private:
        int size;
        int startIndex, indexInterval; 
        float scalingFactor;

        LineChart *lineChart;
        AbsoluteSizeLegend *legend;
        AbsoluteSizeIndicator **points;
        
        void positionPoint(AbsoluteSizeIndicator *point);

        static float radiusFromArea(float area);
};

#endif