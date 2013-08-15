#ifndef ABSOLUTESIZELEGEND_H_
#define ABSOLUTESIZELEGEND_H_

class AbsoluteSizesChart;
class AbsoluteSizeIndicator;

#include "Point.h"
#include <vector>

class AbsoluteSizeLegend : public Point {
    public:
        AbsoluteSizeLegend(AbsoluteSizesChart *chart, std::vector<float> sampleValues);
        ~AbsoluteSizeLegend();

        void draw();

        bool getDisplay() { return display; }
        void setDisplay(bool d) { display = d; }
        void displayOn() { display = true; }
        void displayOff() { display = false; }
    protected:
    private:
        bool display;

        AbsoluteSizesChart *chart; 
        std::vector<AbsoluteSizeIndicator *> *points;
};

#endif