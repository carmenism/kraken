#ifndef ABSOLUTESIZESCHART_H_
#define ABSOLUTESIZESCHART_H_

class AbsoluteSizeIndicator;
class LineChart;

#include "Chart2D.h"
#include <vector>

class AbsoluteSizesChart : public Chart2D {
    public:
        AbsoluteSizesChart(LineChart *lineChart);
        ~AbsoluteSizesChart() {}

        virtual void draw();
        virtual void drawAtOrigin();
        virtual void drawToPick();
        virtual void drawToPickAtOrigin();

        std::vector<AbsoluteSizeIndicator *> *getPoints() { return points; }
    protected:
    private:
        LineChart *lineChart;
        int startIndex, indexInterval; 
        float maxRadius;
        std::vector<AbsoluteSizeIndicator *> *points;

        void calculateMaxRadius();
        static float radiusFromArea(float area);
};

#endif