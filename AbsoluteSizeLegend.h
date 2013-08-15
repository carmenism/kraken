#ifndef ABSOLUTESIZELEGEND_H_
#define ABSOLUTESIZELEGEND_H_

class AbsoluteSizesChart;
class AbsoluteSizeIndicator;

#include "Legend.h"
#include "Point.h"
#include <vector>

class AbsoluteSizeLegend : public Legend {
    public:
        AbsoluteSizeLegend(AbsoluteSizesChart *chart, std::vector<float> sampleValues);
        ~AbsoluteSizeLegend();

        virtual void drawAtOrigin();
    protected:
    private:
        AbsoluteSizesChart *chart; 
        std::vector<AbsoluteSizeIndicator *> *points;

        float getMaxRadius();
};

#endif