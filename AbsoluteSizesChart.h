#ifndef ABSOLUTESIZESCHART_H_
#define ABSOLUTESIZESCHART_H_

class AbsoluteSizeIndicator;
class LineChart;

#include "Chart2D.h"
#include <vector>

class AbsoluteSizesChart : public Chart2D {
    public:
        AbsoluteSizesChart(LineChart *lineChart);//, std::vector<float> x, std::vector<float> y);
        ~AbsoluteSizesChart() {}

        virtual void drawAtOrigin();
        virtual void drawToPickAtOrigin();
        virtual void draw();
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